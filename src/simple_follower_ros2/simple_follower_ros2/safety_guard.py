#!/usr/bin/env python3
"""
安全防护节点 safety_guard

作用：
  1) 超声波前置保护：根据前向超声波最小距离，对来自 /cmd_vel_raw 的前进线速度
     做线性衰减，必要时强制为 0；角速度保持放行，便于上层算法转向脱困。
  2) IMU 碰撞兜底：监测 IMU x 轴线加速度相对低通基线的偏离。当处于前进状态
     且发生明显加速度突变时判为碰撞，进入恢复期：先零速度（可选短暂后退）
     若干秒，再恢复正常。

输入：
  /cmd_vel_raw    geometry_msgs/Twist     上层（如巡线节点）期望的速度
  /Distance       robot_interfaces/Supersonic   底盘发布的 6/8 路超声波距离
  /imu/data_raw   sensor_msgs/Imu          底盘发布的 IMU 原始数据

输出：
  /cmd_vel        geometry_msgs/Twist     送给底盘的安全速度
  /safety_status  std_msgs/String         调试用状态字符串
"""

import math
import time

import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile

from geometry_msgs.msg import Twist
from sensor_msgs.msg import Imu
from std_msgs.msg import String
from robot_interfaces.msg import Supersonic


STATE_NORMAL = 'NORMAL'
STATE_SLOWING = 'SLOWING'
STATE_STOPPED = 'STOPPED_OBSTACLE'
STATE_COLLIDED = 'COLLIDED_RECOVERY'


class SafetyGuard(Node):
    def __init__(self):
        super().__init__('safety_guard')

        self.declare_parameter('stop_distance', 0.25)
        self.declare_parameter('slow_distance', 0.60)
        self.declare_parameter('min_valid_distance', 0.05)
        self.declare_parameter('front_sonars', ['B', 'C', 'D', 'E'])

        self.declare_parameter('accel_spike_threshold', 6.0)
        self.declare_parameter('accel_baseline_alpha', 0.02)
        self.declare_parameter('min_cmd_for_collision', 0.05)
        self.declare_parameter('recovery_duration', 1.5)
        self.declare_parameter('backup_duration', 0.4)
        self.declare_parameter('backup_speed', 0.05)

        self.declare_parameter('cmd_timeout', 0.5)
        self.declare_parameter('status_period', 0.5)

        self.stop_distance = float(self.get_parameter('stop_distance').value)
        self.slow_distance = float(self.get_parameter('slow_distance').value)
        self.min_valid_distance = float(self.get_parameter('min_valid_distance').value)
        self.front_sonars = [s.upper() for s in self.get_parameter('front_sonars').value]

        self.accel_spike_threshold = float(self.get_parameter('accel_spike_threshold').value)
        self.accel_baseline_alpha = float(self.get_parameter('accel_baseline_alpha').value)
        self.min_cmd_for_collision = float(self.get_parameter('min_cmd_for_collision').value)
        self.recovery_duration = float(self.get_parameter('recovery_duration').value)
        self.backup_duration = float(self.get_parameter('backup_duration').value)
        self.backup_speed = float(self.get_parameter('backup_speed').value)

        self.cmd_timeout = float(self.get_parameter('cmd_timeout').value)

        qos = QoSProfile(depth=10)
        self.cmd_pub = self.create_publisher(Twist, 'cmd_vel', qos)
        self.status_pub = self.create_publisher(String, 'safety_status', qos)

        self.cmd_sub = self.create_subscription(
            Twist, 'cmd_vel_raw', self.cmd_callback, qos)
        self.dist_sub = self.create_subscription(
            Supersonic, 'Distance', self.distance_callback, qos)
        self.imu_sub = self.create_subscription(
            Imu, 'imu/data_raw', self.imu_callback, qos)

        self.front_min_distance = float('inf')
        self.last_distance_time = 0.0

        self.accel_baseline = None
        self.last_imu_time = None

        self.last_cmd = Twist()
        self.last_cmd_time = 0.0

        self.state = STATE_NORMAL
        self.recovery_start = 0.0

        period = float(self.get_parameter('status_period').value)
        self.create_timer(0.05, self.control_loop)
        self.create_timer(period, self.publish_status)

        self.get_logger().info(
            f'safety_guard started: stop={self.stop_distance}m, '
            f'slow={self.slow_distance}m, accel_th={self.accel_spike_threshold} m/s^2, '
            f'front_sonars={self.front_sonars}')

    def cmd_callback(self, msg: Twist):
        self.last_cmd = msg
        self.last_cmd_time = self.now_sec()

    def distance_callback(self, msg: Supersonic):
        mapping = {
            'A': msg.distance_a, 'B': msg.distance_b,
            'C': msg.distance_c, 'D': msg.distance_d,
            'E': msg.distance_e, 'F': msg.distance_f,
            'G': msg.distance_g, 'H': msg.distance_h,
        }
        valid = [mapping[k] for k in self.front_sonars
                 if k in mapping and mapping[k] >= self.min_valid_distance]
        self.front_min_distance = min(valid) if valid else float('inf')
        self.last_distance_time = self.now_sec()

    def imu_callback(self, msg: Imu):
        ax = msg.linear_acceleration.x
        now = self.now_sec()

        if self.accel_baseline is None:
            self.accel_baseline = ax
            self.last_imu_time = now
            return

        alpha = self.accel_baseline_alpha
        self.accel_baseline = (1.0 - alpha) * self.accel_baseline + alpha * ax
        self.last_imu_time = now

        if self.state == STATE_COLLIDED:
            return

        deviation = ax - self.accel_baseline
        commanded_forward = self.last_cmd.linear.x > self.min_cmd_for_collision
        fresh_cmd = (now - self.last_cmd_time) < self.cmd_timeout

        if commanded_forward and fresh_cmd and abs(deviation) > self.accel_spike_threshold:
            self.get_logger().warn(
                f'Collision detected! ax={ax:.2f}, baseline={self.accel_baseline:.2f}, '
                f'deviation={deviation:.2f} m/s^2')
            self.state = STATE_COLLIDED
            self.recovery_start = now

    def control_loop(self):
        now = self.now_sec()
        out = Twist()

        if self.state == STATE_COLLIDED:
            elapsed = now - self.recovery_start
            if elapsed < self.backup_duration:
                out.linear.x = -float(self.backup_speed)
                out.angular.z = 0.0
            elif elapsed < self.recovery_duration:
                out.linear.x = 0.0
                out.angular.z = 0.0
            else:
                self.state = STATE_NORMAL
                self.get_logger().info('Collision recovery finished, resuming.')
                return
            self.cmd_pub.publish(out)
            return

        if (now - self.last_cmd_time) > self.cmd_timeout:
            return

        vx = float(self.last_cmd.linear.x)
        vy = float(self.last_cmd.linear.y)
        wz = float(self.last_cmd.angular.z)
        d = self.front_min_distance

        if vx <= 0.0 or math.isinf(d):
            self.state = STATE_NORMAL
        elif d <= self.stop_distance:
            vx = 0.0
            self.state = STATE_STOPPED
        elif d < self.slow_distance:
            scale = (d - self.stop_distance) / (self.slow_distance - self.stop_distance)
            vx = vx * max(0.0, min(1.0, scale))
            self.state = STATE_SLOWING
        else:
            self.state = STATE_NORMAL

        out.linear.x = vx
        out.linear.y = vy
        out.angular.z = wz
        self.cmd_pub.publish(out)

    def publish_status(self):
        msg = String()
        d = self.front_min_distance
        d_str = 'inf' if math.isinf(d) else f'{d:.2f}m'
        base = f'{self.accel_baseline:.2f}' if self.accel_baseline is not None else 'NA'
        msg.data = f'state={self.state} front={d_str} ax_baseline={base}'
        self.status_pub.publish(msg)

    def now_sec(self):
        return self.get_clock().now().nanoseconds * 1e-9


def main(args=None):
    rclpy.init(args=args)
    node = SafetyGuard()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        stop = Twist()
        node.cmd_pub.publish(stop)
        time.sleep(0.05)
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
