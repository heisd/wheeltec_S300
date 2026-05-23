# turn_on_wheeltec_robot 功能包说明

本包是 Wheeltec S300 机器人的"底盘启动总入口"。它的核心节点 `wheeltec_robot_node`
是上位机（ROS 2）和下位机（STM32 主控板）之间的桥梁：

- 向下：把 ROS 的 `cmd_vel` 速度指令通过串口下发给 STM32，由 STM32 控制电机。
- 向上：把 STM32 通过串口回传的编码器里程、IMU、电池电压、超声波、自动回充等
  数据解析后，以 ROS 话题发布出去，供巡线、SLAM、Nav2 等上层模块使用。

巡线、建图、导航、键盘控制等几乎所有上层功能包都依赖本包先启动底盘。

---

## 一、目录结构

```
turn_on_wheeltec_robot/
├── CMakeLists.txt
├── package.xml
├── wheeltec_udev.sh                    # 串口设备 udev 规则脚本
├── config/
│   ├── ekf.yaml                        # robot_localization EKF 参数
│   ├── ekf_carto.yaml                  # Cartographer 场景下的 EKF 参数
│   ├── imu.yaml                        # imu_filter_madgwick 参数
│   └── camera_info.yaml                # 相机内参
├── include/turn_on_wheeltec_robot/
│   ├── wheeltec_robot.hpp              # 底盘节点头文件（数据结构 + 类声明）
│   └── Quaternion_Solution.hpp         # IMU 姿态四元数解算
├── src/
│   ├── wheeltec_robot.cpp              # 底盘节点主实现
│   └── Quaternion_Solution.cpp
├── msg/
│   └── Position.msg
└── launch/
    ├── turn_on_wheeltec_robot.launch.py    # 底盘一键启动入口（最常用）
    ├── base_serial.launch.py               # 仅启动串口通信节点
    ├── wheeltec_ekf.launch.py              # 启动 robot_localization 的 EKF
    ├── wheeltec_camera.launch.py           # 启动相机驱动
    ├── wheeltec_lidar.launch.py            # 启动激光雷达
    ├── wheeltec_sensors.launch.py          # 一次启动多种外设
    ├── robot_mode_description.launch.py    # 加载 URDF、TF
    └── include/
        ├── base_serial_h30imu.launch.py
        └── turn_on_wheeltec_robot_h30imu.launch.py
```

---

## 二、启动文件概览

### 2.1 `turn_on_wheeltec_robot.launch.py`（最常用）

这是绝大多数上层 launch 都会 `IncludeLaunchDescription` 进来的入口。它会拉起：

| 子项 | 作用 |
| --- | --- |
| `base_serial.launch.py` → **`wheeltec_robot_node`** | 串口通信主节点（本包核心） |
| `robot_mode_description.launch.py` | 加载 URDF、`joint_state_publisher`、`robot_state_publisher` |
| `wheeltec_ekf.launch.py` | 启动 `robot_localization` 的 EKF，把 odom + IMU 融合 |
| `base_to_link`（static TF） | 发布 `base_footprint → base_link` 静态 TF（位置与 `ROBOT_TYPE` 环境变量有关） |
| `imu_filter_madgwick_node` | 把 IMU 原始数据用 Madgwick 滤波器解算成姿态 |

支持的 launch 参数：

| 参数 | 默认 | 含义 |
| --- | --- | --- |
| `carto_slam` | `false` | 是否切到 Cartographer 模式的 EKF |

环境变量 `ROBOT_TYPE` 决定底盘几何参数（影响 `base_footprint → base_link` 的偏移），取值：
`s300_pro`、`s300_mini`，默认 `s300_pro`。

### 2.2 `base_serial.launch.py`

只启动 `wheeltec_robot_node`，给它一组关键参数：

```python
usart_port_name:         '/dev/wheeltec_controller'   # STM32 串口设备
serial_baud_rate:        115200
robot_frame_id:          'base_footprint'             # 底盘坐标系
odom_frame_id:           'odom_combined'              # 里程计父坐标系
cmd_vel:                 'cmd_vel'                    # 订阅的速度话题
akm_cmd_vel:             'none'
product_number:          0
odom_x_scale:            1.0                          # x 方向里程标定
odom_y_scale:            1.0                          # y 方向里程标定
odom_z_scale_positive:   0.95                         # 左转 yaw 里程标定
odom_z_scale_negative:   0.95                         # 右转 yaw 里程标定
```

调试小车走偏 / 转角不准时，主要改这几个 `odom_*_scale`。

---

## 三、核心节点 `wheeltec_robot_node`

源码：`src/wheeltec_robot.cpp` + `include/turn_on_wheeltec_robot/wheeltec_robot.hpp`。

### 3.1 ROS 接口一览

#### 订阅（Subscribe）

| 话题 | 类型 | 回调 | 用途 |
| --- | --- | --- | --- |
| `cmd_vel` | `geometry_msgs/Twist` | `Cmd_Vel_Callback` | 上层（巡线 / Nav2 / 键盘）发的速度指令，打包成串口帧下发 STM32 |
| `red_vel` | `Twist` | `Red_Vel_Callback` | 红外对接（自动回充对桩）速度 |
| `robot_recharge_flag` | `Int8` | `Recharge_Flag_Callback` | 标记当前是普通速度还是回充速度 |
| `chassis_security` | `Int8` | `Security_Callback` | 底盘安全防护开关位 |

#### 发布（Publish）

| 话题 | 类型 | 含义 |
| --- | --- | --- |
| `odom` | `nav_msgs/Odometry` | 里程计：x/y/yaw 位姿 + 三轴速度，含动/静态两套协方差矩阵 |
| `imu/data_raw` | `sensor_msgs/Imu` | IMU 原始数据，交给 `imu_filter_madgwick` 进一步解算 |
| `PowerVoltage` | `std_msgs/Float32` | 电池电压（V），低于阈值会红字告警 |
| `Distance` | `robot_interfaces/Supersonic` | 8 路超声波距离（mini 没有 F 路） |
| `robot_charging_flag` | `Bool` | 是否正在充电 |
| `robot_charging_current` | `Float32` | 充电电流 |
| `robot_red_flag` | `Bool` | 是否搜寻到充电桩红外信号 |
| `/self_check_data` | `UInt32` | 底盘自检状态字 |

> 注意：本节点本身**不发布 TF**。`odom → base_footprint` 的 TF 由 EKF 节点
> （`wheeltec_ekf.launch.py`）发布，本节点只发 `nav_msgs/Odometry` 数据。

### 3.2 串口协议

#### 3.2.1 下行：ROS → STM32（11 字节）

`Cmd_Vel_Callback` 把 Twist 打包：

```
0x7B │ flag(AutoRecharge) │ flag(Security) │ Vx(2B) │ Vy(2B) │ Wz(2B) │ BCC │ 0x7D
```

- 速度先放大 1000 倍变 `short`，再拆成高/低字节，便于定点传输。
- `flag` 区分"普通速度"、"红外对接速度"、"灯带 RGB"等用途。
- BCC = 前若干字节的逐字节异或。

#### 3.2.2 上行：STM32 → ROS（多类型帧混在同一根串口）

同一个串口上轮流出现 3 种数据帧，靠"上一字节是不是上一帧的帧尾"做状态机识别：

1. **24 字节数据帧**（编码器速度 + IMU + 电压）
   ```
   0x7B │ flag │ Vx(2B) Vy(2B) Vz(2B) │ ax ay az gx gy gz (12B) │ Volt(2B) │ BCC │ 0x7D
   ```
2. **19 字节超声波帧**
   ```
   0xFA │ 6 路距离(12B) │ 自检数据(4B) │ BCC │ 0xFC
   ```
3. **8 字节自动回充帧**
   ```
   0x7C │ 充电电流(2B) │ 红外状态 │ 充电状态 │ 预留 │ BCC │ 0x7F
   ```

解析时：
- 速度 `(short)/1000 + (short%1000)*0.001` 还原成 m/s。
- IMU 用宏 `GYROSCOPE_RATIO = 0.00026644` （量程 ±500°/s）、
  `ACCEl_RATIO = 1671.84`（量程 ±2g）换算成 rad/s 和 m/s²。
- 电压同样除 1000 还原成 V。

### 3.3 主循环 `Control()`

```cpp
while (rclcpp::ok()) {
    if (distance_flag)         Publish_distance();          // 来超声波了
    if (check_AutoCharge_data) Publish_Charging / RED / ChargingCurrent;

    if (Get_Sensor_Data()) {                                // 拿到 24B 帧
        // 1) 里程标定
        Robot_Vel.X *= odom_x_scale;
        Robot_Vel.Y *= odom_y_scale;
        Robot_Vel.Z *= (Vz >= 0 ? odom_z_scale_positive : odom_z_scale_negative);

        // 2) 航迹推算（dead reckoning）
        Robot_Pos.X += (Vx*cosθ - Vy*sinθ) * dt;
        Robot_Pos.Y += (Vx*sinθ + Vy*cosθ) * dt;
        Robot_Pos.Z += Vz * dt;

        // 3) IMU 解算四元数姿态
        Quaternion_Solution(gx, gy, gz, ax, ay, az);

        // 4) 发布 odom / imu / 电压
        Publish_Odom();
        Publish_ImuSensor();
        Publish_Voltage();
    }

    rclcpp::spin_some(...);
}
```

里程计的协方差有两套（`wheeltec_robot.hpp` 顶部定义）：
- **静止**（Vx=Vy=Vz=0）：用 `odom_pose_covariance2 / odom_twist_covariance2`，
  对编码器更"自信"（编码器静止时不易出错）。
- **运动**：用 `odom_pose_covariance / odom_twist_covariance`，
  考虑到编码器在运动中可能滑动，让 EKF 更信 IMU。

### 3.4 析构行为（关键）

`~turn_on_robot()` 在节点退出前会主动给 STM32 下发两条"零速度 + 复位"指令再关串口。
**所以正常 `Ctrl+C` 关掉上层节点时小车会立刻停下来，是上位机兜的底**，
而不是 STM32 在自己做超时停车。

如果是异常杀进程（`kill -9`），析构不会执行，STM32 才会按自己的看门狗停车。

---

## 四、数据流（巡线场景为例）

```
  相机 ──/camera/color/image_raw──► line_follow (Python)
                                          │
                                  PD 算出 Twist
                                          ▼
                                       cmd_vel
                                          │
              ┌──────────── wheeltec_robot_node ────────────┐
              │  打包成 11B 串口帧 → /dev/wheeltec_controller │
              └─────────────┬───────────────────────────────┘
                            ▼
                         STM32 主控
                            │ 驱动电机
                            ▼
                          小车跑
                            │ 编码器 + IMU + 超声波
                            ▼
              ┌─────── 回传多类型串口帧 ───────┐
              │ 解析为 odom / imu / 电压 / 距离  │
              └───┬────────────────┬───────────┘
                  ▼                ▼
                odom         imu/data_raw
                  │                │
                  └──► EKF ────────► /odometry/filtered + odom→base_footprint TF
                                                │
                                                ▼
                                       供 SLAM / Nav2 / 上层使用
```

---

## 五、相关配置文件

### `config/ekf.yaml`

`robot_localization` 的扩展卡尔曼滤波参数：哪些维度信 odom、哪些维度信 IMU、
噪声、协方差等。EKF 节点订阅 `odom` 和 `imu/data_raw`，融合后发布
`/odometry/filtered` 以及 `odom → base_footprint` 的 TF。

`ekf_carto.yaml` 是 Cartographer SLAM 使用的备选参数，由 launch 参数
`carto_slam:=true` 切换。

### `config/imu.yaml`

`imu_filter_madgwick_node` 的参数：增益、是否使用磁力计、是否发布 TF 等。
它订阅 `imu/data_raw`，发布带姿态四元数的 `imu/data`。

### `wheeltec_udev.sh`

注册 udev 规则，把 STM32 的 USB 转串口设备固定映射为
`/dev/wheeltec_controller`，避免插拔顺序导致 `/dev/ttyUSB*` 编号变化。
首次部署或换主机后需要执行一次。

---

## 六、常见问题与调试

| 现象 | 可能原因 / 排查方向 |
| --- | --- |
| 启动报错 `wheeltec_robot can not open serial port` | 设备没插好、udev 规则未生效（`/dev/wheeltec_controller` 不存在）、当前用户没有串口权限（`dialout` 组） |
| `cmd_vel` 有数据但小车不动 | 看 `ros2 topic echo /odom` 是否有回传：如有 → 下行串口写失败/STM32 急停；如无 → 串口校验失败或线松了 |
| 小车走直线偏一边 | 调 `odom_x_scale`、`odom_y_scale` |
| 原地转角误差大 | 调 `odom_z_scale_positive / odom_z_scale_negative`（左右转可分别标定） |
| odom 漂移、定位飘 | 看 `config/ekf.yaml` 中 odom 和 imu 的权重 / 协方差是否合理 |
| 弹出电压告警 | `Publish_Voltage()` 中阈值：Plus < 20V、Mini < 10V 时报警，先量电池 |
| `Ctrl+C` 后小车继续往前冲 | 节点没正常析构（被 kill -9 或崩溃），下次启动前手动用键盘节点发 0 速度 |
| 超声波 F 路一直为 0 | 正常：mini 版没有 F 路超声波，代码里被强制清零 |

---

## 七、快速命令速查

```bash
# 单独启动底盘（不包含其它传感器/相机）
ros2 launch turn_on_wheeltec_robot turn_on_wheeltec_robot.launch.py

# 看里程计
ros2 topic echo /odom

# 看 IMU 原始数据
ros2 topic echo /imu/data_raw

# 看电池电压
ros2 topic echo /PowerVoltage

# 看超声波
ros2 topic echo /Distance

# 用键盘手动发速度测试底盘
ros2 run wheeltec_robot_keyboard wheeltec_keyboard
```

---

## 八、和其他包的关系

- `simple_follower_ros2`（巡线、视觉跟随、激光跟随）→ 发 `cmd_vel`
- `wheeltec_robot_nav2`（导航）→ 发 `cmd_vel`，依赖 `odom` 和 TF
- `wheeltec_robot_slam`、`wheeltec_robot_rtab`（建图）→ 依赖 `odom`、`imu/data` 和 TF
- `wheeltec_robot_keyboard`（键盘控制）→ 发 `cmd_vel`

**所有这些包能跑的前提，都是本包先把 `wheeltec_robot_node` 起来。**
