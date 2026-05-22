# simple_follower_ros2 巡线功能说明

本文档介绍本功能包中"视觉巡线"功能的整体逻辑、启动方式以及参数含义，便于后续调试与二次开发。

巡线相关的关键文件：

| 文件 | 作用 |
| --- | --- |
| `launch/line_follower.launch.py` | 巡线一键启动脚本（底盘 + 相机 + 巡线节点） |
| `simple_follower_ros2/line_follow.py` | 巡线核心节点：图像处理 + PID 控制 + 发布 `cmd_vel` |
| `setup.py` 中的 `line_follow` 入口 | 把 `line_follow.py` 注册为可执行节点 |

> 注意：launch 文件名是 `line_follower.launch.py`，可执行节点名是 `line_follow`，不要混淆。

---

## 一、如何启动

```bash
# 1. 编译（首次或修改后）
cd ~/wheeltec_S300
colcon build --packages-select simple_follower_ros2
source install/setup.bash

# 2. 启动巡线
ros2 launch simple_follower_ros2 line_follower.launch.py
```

启动后会弹出一个名为 `Adjust_hsv` 的 OpenCV 窗口，窗口顶部有一个滑动条：

```
0:Red  1:Green  2:Blue  3:Yellow  4:Black
```

通过滑动条切换要跟踪的线条颜色。窗口中显示的是经过 HSV 阈值分割后的二值掩码图，调试时可以直接观察当前颜色识别是否稳定。

---

## 二、launch 文件做了什么

`launch/line_follower.launch.py` 主要完成三件事：

1. **启动小车底盘**
   ```python
   IncludeLaunchDescription(... 'turn_on_wheeltec_robot.launch.py')
   ```
   引入 `turn_on_wheeltec_robot` 包的底盘启动文件，负责串口通信、odom/tf 发布等。

2. **启动相机驱动**
   ```python
   IncludeLaunchDescription(... 'wheeltec_camera.launch.py')
   ```
   打开彩色相机，发布话题 `/camera/color/image_raw`。

3. **启动巡线节点**
   ```python
   launch_ros.actions.Node(
       package='simple_follower_ros2',
       executable='line_follow',
       name='line_follow')
   ```
   即运行 `line_follow.py` 中的 `main()` 函数。

---

## 三、巡线节点 `line_follow.py` 详细逻辑

### 1. 节点接口

| 角色 | 话题 | 类型 |
| --- | --- | --- |
| 订阅 | `/camera/color/image_raw` | `sensor_msgs/Image` |
| 发布 | `cmd_vel` | `geometry_msgs/Twist` |

并使用 `cv_bridge` 将 ROS 图像消息转成 OpenCV 的 BGR 图。

### 2. 预设的颜色 HSV 阈值

```python
col_black  = (0,   0,   0,   180, 255, 46)   # 黑色
col_red    = (0,   100, 80,  10,  255, 255)  # 红色
col_blue   = (100, 43,  46,  124, 255, 255)  # 蓝色
col_green  = (35,  43,  46,  77,  255, 255)  # 绿色
col_yellow = (26,  43,  46,  34,  255, 255)  # 黄色
```

每一组 6 个值分别表示 HSV 的下限 `(H,S,V)` 与上限 `(H,S,V)`。
窗口上的滑动条根据当前选项加载对应颜色的阈值。如果现场光线变化大、识别不稳定，可以使用本包提供的 `adjust_hsv.py` 工具实时标定后，把新的阈值改进此处。

### 3. 图像处理流程

每收到一帧图像，节点按以下步骤处理：

1. **格式转换**：`bridge.imgmsg_to_cv2(msg, 'bgr8')` 得到 BGR 图。
2. **色彩空间转换**：`cv2.cvtColor(image, cv2.COLOR_BGR2HSV)` 转到 HSV 空间，对光照变化更鲁棒。
3. **形态学去噪**：先 `cv2.erode` 腐蚀，再 `cv2.dilate` 膨胀（开运算），去除小颗粒噪声。
4. **颜色阈值分割**：根据当前滑动条选择的颜色，使用 `cv2.inRange` 得到只保留目标颜色像素的二值掩码 `mask`。
5. **限定 ROI（感兴趣区域）**：只保留图像最下方 30 行，其余清零。
   ```python
   search_top = h - 30
   search_bot = h
   mask[0:search_top, 0:w] = 0
   mask[search_bot:h, 0:w] = 0
   ```
   这样小车只关注"正前方紧邻的一段线"，对远处弯道、画面边角的干扰更不敏感，控制更平稳。
6. **几何中心计算**：用图像矩求线的质心：
   ```python
   M  = cv2.moments(mask)
   cx = int(M['m10'] / M['m00'])
   cy = int(M['m01'] / M['m00'])
   ```
   `(cx, cy)` 即为当前 ROI 内线条的几何中心。

### 4. 偏差计算与 PID 控制

```python
erro   = cx - w/2 - 60
d_erro = erro - last_erro

self.twist.linear.x  = 0.11
self.twist.angular.z = -(float(erro)*0.0011 - float(d_erro)*0.0000)
```

- `w/2`：图像的水平中心像素列。
- `-60`：相机相对车体的安装横向偏移补偿（让小车以画面中心偏左 60 像素处视为"对中"），如更换相机位置需要重新调整该值。
- `erro`：横向误差。`erro > 0` 表示线在小车右侧，`erro < 0` 表示在左侧。
- `d_erro = erro - last_erro`：误差的变化量，相当于微分项；当前 Kd = 0，保留 PD 结构便于后续扩展。
- `angular.z = -(erro·Kp - d_erro·Kd)`：
  - `Kp = 0.0011`
  - `Kd = 0.0000`（默认关闭）
  - 加负号是因为图像 x 轴向右为正，而 ROS 中 `angular.z` 正方向为逆时针（左转）。
- `linear.x = 0.11`：恒定前进线速度，单位 m/s。

如需调速或调整跟随灵敏度，主要改这三个值：`linear.x`、`Kp`、`Kd`。

### 5. 丢线保护

```python
if M['m00'] > 0:
    ... # 正常巡线
else:
    self.twist.linear.x  = 0.0
    self.twist.angular.z = 0.0
```

当 ROI 中没有任何目标颜色像素（即掩码为空）时，立即把线速度与角速度全部置零，小车停下来等待，避免脱线后冲出场地。

### 6. 节点主循环

```python
while rclpy.ok():
    rclpy.spin_once(follower)
    time.sleep(0.1)
```

约 10 Hz 节奏地处理一次回调（图像处理实际由话题触发）。

---

## 四、整体数据流

```
camera 驱动 ──► /camera/color/image_raw ──► line_follow 节点
                                                  │
                                          BGR→HSV→去噪→颜色阈值
                                                  │
                                            底部 30 行 ROI
                                                  │
                                            质心 cx, cy
                                                  │
                                      偏差 erro = cx - w/2 - 60
                                                  │
                                          PD 控制器 (Kp, Kd)
                                                  │
                                          Twist (v, ω) → cmd_vel
                                                  │
                                       turn_on_wheeltec_robot → 底盘
```

---

## 五、常见调试技巧

1. **看不到线 / 识别抖动**：使用 `adjust_hsv.launch.py` 现场标定颜色阈值，再把新的 HSV 上下限替换进 `line_follow.py` 顶部的 `col_xxx` 常量。
2. **走得偏一边**：调整 `erro = cx - w/2 - 60` 中的 `-60`，匹配相机的安装偏移。
3. **转得太猛或不跟手**：增大或减小 `Kp = 0.0011`；如需抑制超调，给 `Kd` 设一个小值（如 `0.0003`）。
4. **速度太快冲出弯道**：降低 `linear.x = 0.11`。
5. **弯道丢线**：可以适当增大 ROI（把 `h-30` 改成 `h-50`），让小车提前看到更远处的线。

---

## 六、文件位置速查

```
simple_follower_ros2/
├── launch/
│   └── line_follower.launch.py        # 巡线启动脚本
├── simple_follower_ros2/
│   ├── line_follow.py                 # 巡线核心节点
│   └── adjust_hsv.py                  # HSV 阈值标定工具
├── setup.py                           # 注册 line_follow 可执行入口
└── README.md                          # 本说明文档
```
