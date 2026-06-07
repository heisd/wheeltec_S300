# Wheeltec S300 ROS2 工作空间（src）

本目录为 **Wheeltec S300** 移动机器人的 ROS2 源码工作空间，基于 **ROS2 Humble** 开发。
工作空间下层（STM32 控制板）通过串口与上位机通信，上位机运行底盘驱动、传感器驱动、
SLAM 建图、Nav2 导航、视觉/雷达跟随、骨架识别、语音交互、大模型对话、自动回充以及
Web 仪表盘等完整功能。

> 配套命令速查见同目录下的 `ROS2-V3.5(humble)常用指令.txt`。

---

## 目录

- [系统架构](#系统架构)
- [核心话题与坐标系](#核心话题与坐标系)
- [环境与依赖](#环境与依赖)
- [编译与运行](#编译与运行)
- [功能包详解](#功能包详解)
  - [底盘与基础驱动](#一底盘与基础驱动)
  - [传感器驱动](#二传感器驱动)
  - [SLAM 建图](#三slam-建图)
  - [Nav2 导航与扩展](#四nav2-导航与扩展)
  - [视觉 / 跟随 / 交互](#五视觉--跟随--交互)
  - [语音 / AI](#六语音--ai)
  - [可视化与工具](#七可视化与工具)
- [常用功能命令](#常用功能命令)
- [常用维护命令](#常用维护命令)

---

## 系统架构

```
                 ┌───────────────────────────────────────────────┐
   STM32 下位机 ──串口──►│ turn_on_wheeltec_robot (wheeltec_robot_node) │
   (编码器/IMU/电压/超声) │  发布 /odom /imu/data_raw /PowerVoltage ...   │
                 │  订阅 /cmd_vel                                  │
                 └───────────────┬───────────────────────────────┘
                                 │ /odom + /imu/data_raw
                                 ▼
                   robot_localization EKF  ──► /odom_combined (TF: odom_combined→base_footprint)
                                 │
   雷达 LSlidar ─► /scan1  ┐     │
   雷达 LSlidar ─► /scan2  ┼─ double_lidar_fusion ─► /scan
   Astra 相机  ─► /camera/* ┘    │
                                 ▼
        ┌──────────── SLAM (gmapping/cartographer/slam_toolbox/rtabmap/orb_slam2) ──► /map
        │
        ▼
     Nav2 (AMCL + BT Navigator) ◄── /goal_pose ◄── waypoint_cycle / path_follow / auto_recharge
        │
        ▼  /cmd_vel ──► 回到底盘
```

所有节点统一使用 **ROS2 Humble + tf2 + Nav2**。下位机协议帧头 `0x7B`、帧尾 `0x7D`，
默认波特率 `115200`，串口设备 `/dev/wheeltec_controller`。

---

## 核心话题与坐标系

| 话题 | 类型 | 发布者 | 订阅者 |
| --- | --- | --- | --- |
| `/cmd_vel` | geometry_msgs/Twist | 遥控 / 导航 / 跟随 | `wheeltec_robot_node`（底盘） |
| `/odom` | nav_msgs/Odometry | 底盘 | EKF、SLAM |
| `/odom_combined` | nav_msgs/Odometry | EKF | Nav2、SLAM、路径跟踪 |
| `/imu/data_raw` | sensor_msgs/Imu | 底盘 | imu_filter_madgwick、EKF |
| `/scan1` `/scan2` | sensor_msgs/LaserScan | 双激光雷达 | `double_lidar_fusion` |
| `/scan` | sensor_msgs/LaserScan | 雷达融合 | SLAM、Nav2、跟随 |
| `/map` | nav_msgs/OccupancyGrid | SLAM | Nav2、RRT |
| `/goal_pose` | geometry_msgs/PoseStamped | waypoint_cycle / 路径跟踪 / 回充 | Nav2 action |
| `/PowerVoltage` | std_msgs/Float32 | 底盘 | 自动回充、仪表盘 |
| `/Distance` | robot_interfaces/Supersonic | 底盘 | 超声波转换、避障 |
| `/charger_position_update` | geometry_msgs/PoseStamped | 用户（RViz 标定） | 自动回充 |

**主要 TF 坐标系**：`map → odom_combined → base_footprint → base_link`，
传感器子坐标系 `gyro_link`、`laser/laser1/laser2`、`ultrasonic_A…F`、`camera_link`。

---

## 环境与依赖

- **ROS 版本**：ROS2 Humble（Ubuntu 22.04）
- **机器人主机**：默认 IP `192.168.0.100`，用户 `wheeltec`
- **车型选择**：环境变量 `ROBOT_TYPE` 决定 URDF/参数（如 `s300_pro`、`s300_mini`、`s300`）

  ```bash
  echo 'export ROBOT_TYPE=s300_pro' >> ~/.bashrc
  ```

- **额外系统依赖**：

  ```bash
  sudo apt install ros-humble-rosbridge-suite   # Web 仪表盘需要
  rosdep install --from-paths src --ignore-src -r -y
  ```

---

## 编译与运行

在工作空间根目录（`src` 的上一级）执行：

```bash
# 编译全部功能包
colcon build --packages-skip-build-finished --continue-on-error

# 单独编译某个功能包
colcon build --packages-select turn_on_wheeltec_robot

# 设置环境变量
source install/setup.bash
```

> 注意：修改 launch / 参数文件后需重新编译才能生效。

---

## 功能包详解

### 一、底盘与基础驱动

#### `turn_on_wheeltec_robot`（核心）
底盘控制核心，节点 **`wheeltec_robot_node`** 通过串口与 STM32 通信。

- **发布**：`/odom`、`/imu/data_raw`、`/PowerVoltage`（电压）、`/Distance`（8 路超声 A–H）、
  `/robot_charging_flag`、`/robot_charging_current`、`/robot_red_flag`（红外对桩）、`/self_check_data`
- **订阅**：`/cmd_vel`、`/red_vel`（红外对接速度）、`/robot_recharge_flag`、`/chassis_security`
- **服务**：`set_rgb_color`（robot_interfaces/SetRgb，设置 LED 灯带）
- **关键参数**：`usart_port_name`(`/dev/wheeltec_controller`)、`serial_baud_rate`(115200)、
  `odom_frame_id`(`odom_combined`)、`robot_frame_id`(`base_footprint`)、`gyro_frame_id`(`gyro_link`)、
  `car_mode`、`odom_x/y/z_scale*`（里程计标定）
- **9 个 launch**：
  | launch | 作用 |
  | --- | --- |
  | `turn_on_wheeltec_robot.launch.py` | 主入口：底盘串口 + URDF/TF + EKF + IMU 滤波 |
  | `base_serial.launch.py` | 仅启动底盘串口节点与里程计 |
  | `robot_mode_description.launch.py` | 按 `ROBOT_TYPE` 选 URDF，发布全部静态 TF |
  | `wheeltec_ekf.launch.py` | robot_localization EKF（标准 20Hz / cartographer 30Hz 两套配置） |
  | `wheeltec_lidar.launch.py` | 启动双雷达驱动 + `double_lidar_fusion` |
  | `wheeltec_camera.launch.py` | 启动 Astra 深度相机 |
  | `wheeltec_sensors.launch.py` | 底盘 + 雷达 + 相机一键启动 |
  | `wheeltec_joy.launch.py` | 整车 + 手柄控制 |
  | `only_joy.launch.py` | 仅手柄控制（不启动底盘） |
- **配置**：`ekf.yaml`/`ekf_carto.yaml`（融合 odom+IMU，输出 `/odom_combined`）、`imu.yaml`（Madgwick，ENU）

#### `serial_ros2`
串口通信相关占位/底层包（当前目录无源码文件）。

#### `robot_interfaces`
自定义接口包：
- `Supersonic.msg`：`header` + `distance_a…distance_h`（8 路超声）
- `SetRgb.srv`：请求 `bool en, uint8 r,g,b` → 响应 `string res`

#### `wheeltec_robot_keyboard`（Python）
键盘遥控节点 **`wheeltec_keyboard`**，发布 `/cmd_vel`。
`u/i/o/j/k/l/m/,/.` 控制 8 方向，`q/z`、`w/x`、`e/c` 调速，空格急停，`b` 切换差速/麦轮（全向）模式。
默认线速度 0.1 m/s、角速度 0.5 rad/s。

#### `wheeltec_joy`（C++）
USB 手柄遥控节点 **`wheeltec_joy`**，订阅 `/joy`、发布 `/cmd_vel`。
参数 `axis_linear`(1)、`axis_angular`(0)、`v_linear`(0.3)、`v_angular`(0.5)。
A 键差速模式、B 键麦轮模式，右摇杆作加减速倍率。

---

### 二、传感器驱动

#### `wheeltec_lidar_ros2`
含 **`LSlidar`**（镭神）与 **`LDlidar`** 两套激光雷达驱动。S300 默认双 LSlidar，
经 `lslidar_double_launch.py` 分别发布 `/scan1`、`/scan2`。

#### `double_lidar_fusion`
双雷达数据融合，把 `/scan1` + `/scan2` 合并为统一的 `/scan`。
参数 `scan1_topic`(`/scan1`)、`scan2_topic`(`/scan2`)、`scan_topic`(`scan`)、`frame_id`(`laser`)。

#### `wheeltec_ultrasonic`
节点 **`supersonic_converter_node`**（C++），将底盘 `/Distance`（robot_interfaces/Supersonic）
转换为 `sensor_msgs/Range` 与 `sensor_msgs/PointCloud2`（`/ultrasonic/points`），供 Nav2 超声避障层使用。
launch：`supersonic+converter.launch.py`。

#### `wheeltec_imu`
含 **`yesense_ros2`** 元也惯导 IMU 串口驱动（S300 默认使用底盘内置 IMU，此为可选外置 IMU）。

#### `ros2_astra_camera-master`
奥比中光 **Astra / Gemini 深度相机**驱动（`astra_camera`、`astra_camera_msgs`），
发布 `/camera/color/image_raw`、`/camera/color/camera_info`、`/camera/depth/image_raw` 等，含 25 个 launch。

#### `usb_cam-ros2`
通用 V4L USB 相机驱动。

#### `web_video_server-ros2`
将图像话题转为 HTTP MJPEG 流，浏览器访问 `http://<IP>:8080/` 实时查看。

---

### 三、SLAM 建图

`wheeltec_robot_slam` 元包集合多种 2D/3D SLAM：

| 子包 | 节点 | 说明 |
| --- | --- | --- |
| `slam_gmapping` / `openslam_gmapping` | `slam_gmapping` | 经典 2D 激光建图，输入 `/scan`+TF+`/odom`，输出 `/map` |
| `wheeltec_cartographer` | `cartographer_node` + `cartographer_occupancy_grid_node` | 图优化 2D 建图，`cartographer.lua`（laser 0.15–20m，每 45 帧子图） |
| `wheeltec_slam_toolbox` | `sync_slam_toolbox_node` | slam_toolbox 在线同步建图，Ceres 求解，分辨率 0.05m，支持回环 |
| `orb_slam_2_ros-ros2` | `MonoNode`/`RGBDNode`/`StereoNode` | 视觉 ORB-SLAM2，发布 `/orb_slam2_ros/pose`、`map_points`，服务 `save_map` |

---

### 四、Nav2 导航与扩展

#### `wheeltec_robot_nav2`
Nav2 导航封装。`wheeltec_nav2.launch.py` 启动底盘、雷达、（可选）超声，并按 `ROBOT_TYPE`
加载 `nav_param_s300_pro.yaml`，同时拉起多点巡航节点。
- **AMCL**：`global_frame=map`、`odom_frame=odom_combined`、`base_frame=base_footprint`、
  `scan_topic=/scan`、差速运动模型、粒子数 200–500
- **代价地图**：雷达层 `/scan`（局部 0.2–3.5m），可选超声层 `/ultrasonic/points`（0.03–0.8m，marking）
- `save_map.launch.py`：保存地图

#### `nav2_waypoint_cycle`（Python）
节点 **`waypoint_cycle`** 实现多点循环巡航：
订阅 `/clicked_point`（RViz 标点）与导航状态，发布 `/path_point`（MarkerArray 可视化）与 `/goal_pose`，
依次循环导航、失败重试。

#### `wheeltec_robot_rtab`
RTAB-Map 视觉/激光三维建图与导航：
- `wheeltec_slam_rtab.launch.py`：RGBD 同步 + rtabmap 建图（`Reg/Force3DoF=true` 锁 2D）
- `rtabmap_localization.launch.py`：建图或纯定位模式
- `wheeltec_nav2_rtab.launch.py`：RTAB 定位 + Nav2 导航

#### `wheeltec_robot_rrt2` + `wheeltec_rrt_msg`
RRT 自主探索建图：节点 `local_rrt`、`global_rrt`、`filter`、`assigner`、`robot_picker` 等。
发布 `/detected_frontiers`、`/filtered_goal_points`、`/goal_pose`、`/rrt_flag`、`/overmap_flag`。
消息 `PointArray`、服务 `DeleteShape`。launch：`wheeltec_rrt_slam.launch.py`（含 nav2 + slam_toolbox + RRT）。

#### `wheeltec_path_follow`
- **记录**：`save_path`（C++）订阅 TF，位移 >0.05m 或转角 >10° 时记录位姿到文件（`x y yaw`，结尾 `EOP`），发布 `/followpath`
- **跟踪**：`follow_path.py` 读取路径文件，用 Nav2 `navigate_to_pose` 逐点导航，参数 `run_in_loop` 控制循环

#### `auto_recharge_ros2`（Python）
节点 **`auto_recharger`** 自动回充：
- **订阅**：`/PowerVoltage`、`/robot_charging_flag`、`/robot_charging_current`、`/robot_red_flag`、
  `/charger_position_update`、`/odom`
- **发布**：`/goal_pose`、`/goal_marker`、`/cmd_vel`、`robot_recharge_flag`、`/chassis_security`
- **流程**：监测电压 → 低电量导航至充电桩 → 旋转寻找红外信号 → 对桩充电 → 充满检测
- **配置 `robot_info.yaml`**：`car_mode`、`BatteryCapacity`、`diff_point`(1.2m)、`diff_angle`(-15°)；
  桩位置存于 `Charger_Position.json`。RViz 用 `/charger_position_update` 标定桩位

---

### 五、视觉 / 跟随 / 交互

#### `simple_follower_ros2`
三种跟随模式，节点发布 `/cmd_vel`、`/laser_follow_flag` 等，PID 参数可调：

| 模式 | launch | 节点 | 原理 / 参数 |
| --- | --- | --- | --- |
| 雷达跟随 | `laser_follower.launch.py` | `lasertracker`+`laserfollower` | 在 `/scan` 中找最近目标，PID `pid_laser_param.yaml`（P=[1.6,0.5]，D=[0.03,0.005]） |
| 视觉巡线 | `line_follower.launch.py` | `line_follow` | HSV 颜色阈值识别线，`adjust_hsv.launch.py` 可在线调阈值 |
| 视觉跟踪 | `visual_follower.launch.py` | `visualtracker`+`visualfollow` | 目标颜色块跟踪，PID `PID_visual_param.yaml`（P=[1.4,0.4]） |
| 二维码跟随 | `aruco_follower.launch.py` | `arfollower` | 跟随 ArUco 码，`ar_param.yaml` |

#### `wheeltec_robot_kcf`（C++）
节点 **`run_tracker_node`**（KCF + fhog + PID 跟踪器），基于相机图像做单目标视觉跟随，
需在 ROS 主机上运行。launch：`wheeltec_robot_kcf.launch.py`。

#### `aruco_ros-humble-devel`
ArUco 二维码识别（`aruco`、`aruco_msgs`、`aruco_ros`），发布 `/marker`、`/marker_array`、位姿等。

#### `wheeltec_bodyreader`
人体骨架识别（`bodyreader` + `bodyreader_msg`），基于深度相机：
- `bodyinteraction.launch.py`：姿态控制（含多人定向控制、融合 RGB 记忆人物）
- `bodyfollow.launch.py`：人体骨架跟随
- `final.launch.py`：姿态控制 + 跟随（双手胸前交叉切换模式）

---

### 六、语音 / AI

#### `wheeltec_mic`（C++）
麦克风阵列语音控制，含 `wheeltec_mic_ros2` 与消息包 `wheeltec_mic_msg`：
- **节点**：`wheeltec_mic`（阵列初始化/服务）、`call_recognition`（唤醒）、
  `command_recognition`（命令词）、`voice_control`（语音识别）、`motion_control`（运动控制）、`node_feedback`
- **消息/服务**：`PcmMsg`、`MotionControl`；`SetMajorMic`、`SwitchMic`、`SetAwakeWord`、
  `GetOfflineResult`、`GetDeviceType`
- **流程**：`mic_init.launch.py` 初始化阵列；`base.launch.py` 拉起识别 + 运动控制 +
  导航(`voi_navigation`) + 跟随(`lasertracker`/`laserfollower`)，把语音命令映射为机器人动作

#### `wheeltec_aiui`
讯飞 **AIUI** 语音交互（`AIUI` 子目录）。

#### `tts_make_ros2`
文本转语音。`tts_make.launch.py` 中参数 `tts_text`（默认「你好小微」）配置待合成文本，
`tts_params.yaml` 配置发音参数。

#### `ollama_ros_chat`
本地大模型（**Ollama**）对话桥接，含消息包 `ollama_ros_msgs`：
- **节点**：`topic_server` / `topic_client`（话题方式）、`chat_service` / `chat_client`（服务方式）
- **服务**：`Chat.srv`

---

### 七、可视化与工具

| 功能包 | 说明 |
| --- | --- |
| `wheeltec_dashboard` | **Web 仪表盘**（Python 节点 `web_server` + rosbridge）。前端原生 HTML/JS + roslibjs + Chart.js + ros3djs/three.js，可视化遥测、发 `/cmd_vel`、在线调参。`http_port` 默认 8080，rosbridge 默认 `ws://<host>:9090`。启动：`ros2 launch wheeltec_dashboard dashboard.launch.py` |
| `rm_description` | 机械臂/机器人 URDF 模型描述 |
| `wheeltec_rviz2` | RViz2 可视化配置 |
| `qt_ros_test` | ROS2 Qt 图形界面示例，`qt_ros_test.launch.py` |

---

## 常用功能命令

> 启动任意功能前，请先 `source install/setup.bash`。

### 1. 底盘与传感器
```bash
ros2 launch turn_on_wheeltec_robot turn_on_wheeltec_robot.launch.py   # 底盘控制
ros2 launch turn_on_wheeltec_robot wheeltec_camera.launch.py          # 相机
ros2 launch turn_on_wheeltec_robot wheeltec_lidar.launch.py           # 雷达
ros2 launch turn_on_wheeltec_robot wheeltec_sensors.launch.py         # 底盘+雷达+相机
```

### 2. 遥控
```bash
ros2 run wheeltec_robot_keyboard wheeltec_keyboard      # 键盘
ros2 launch wheeltec_joy wheeltec_joy.launch.py         # 手柄
```

### 3. 跟随
```bash
ros2 launch simple_follower_ros2 laser_follower.launch.py    # 雷达跟随
ros2 launch simple_follower_ros2 line_follower.launch.py     # 视觉巡线
ros2 launch simple_follower_ros2 visual_follower.launch.py   # 视觉跟踪
ros2 launch wheeltec_robot_kcf wheeltec_robot_kcf.launch.py  # KCF 跟随（ROS 主机）
```

### 4. 2D 建图与导航
```bash
ros2 launch slam_gmapping slam_gmapping.launch.py            # gmapping 建图
ros2 launch wheeltec_cartographer cartographer.launch.py     # cartographer 建图
ros2 launch wheeltec_nav2 save_map.launch.py                 # 保存地图
ros2 launch wheeltec_nav2 wheeltec_nav2.launch.py            # 2D 导航（含多点）
```

### 5. RTAB-Map 建图与导航
```bash
ros2 launch wheeltec_robot_rtab wheeltec_slam_rtab.launch.py     # 建图
ros2 launch wheeltec_nav2 save_map.launch.py                     # 保存
ros2 launch wheeltec_robot_rtab rtabmap_localization.launch.py   # 定位
ros2 launch wheeltec_robot_rtab wheeltec_nav2_rtab.launch.py     # 导航
```

### 6. RRT 自主探索建图
```bash
ros2 launch slam_gmapping slam_gmapping.launch.py
ros2 launch wheeltec_robot_rrt wheeltec_rrt_slam.launch.py
# 顺/逆时针发布四个点，最后一个点发布在已知地图中
```

### 7. 路径跟踪
```bash
# 记录路径
ros2 launch wheeltec_nav2 wheeltec_nav2.launch.py
ros2 launch wheeltec_path_follow save_path.launch.py
ros2 run wheeltec_robot_keyboard wheeltec_keyboard
# 跟踪路径
ros2 launch wheeltec_nav2 wheeltec_nav2.launch.py
ros2 launch wheeltec_path_follow follow_path.launch.py
```

### 8. Web 浏览器查看摄像头 / 仪表盘
```bash
ros2 launch turn_on_wheeltec_robot wheeltec_camera.launch.py
ros2 run web_video_server web_video_server          # http://192.168.0.100:8080/
ros2 launch wheeltec_dashboard dashboard.launch.py  # Web 仪表盘
```

### 9. 语音 / 交互
```bash
ros2 launch tts tts_make.launch.py                  # 文本转语音
ros2 launch wheeltec_mic_ros2 mic_init.launch.py    # 麦克风阵列初始化
ros2 launch wheeltec_mic_ros2 base.launch.py        # 语音控制小车功能
```

### 10. 骨架识别
```bash
ros2 launch bodyreader bodyinteraction.launch.py    # 姿态控制
ros2 launch bodyreader bodyfollow.launch.py         # 人体跟随
ros2 launch bodyreader final.launch.py              # 姿态控制+跟随（交叉双手切换）
```

### 11. 自动回充
```bash
# 1) 编辑 auto_recharge_ros2/robot_info.yaml 选择车型与电池容量
# 2) 建图并保存
ros2 launch slam_gmapping slam_gmapping.launch.py
ros2 launch wheeltec_nav2 save_map.launch.py
# 3) 导航 + 回充
ros2 launch wheeltec_nav2 wheeltec_nav2.launch.py
ros2 run auto_recharge_ros2 auto_recharge
# 在 rviz 上用话题 charger_position_update 标定充电桩位置
```

### 12. 其它
```bash
ros2 launch qt_ros_test qt_ros_test.launch.py                       # Qt 界面
ros2 launch orb_slam2_ros orb_slam2_Astra_rgbd_launch.py            # ORB-SLAM2
```

---

## 常用维护命令

```bash
rqt_image_view                                  # 查看图像话题
rqt_graph                                       # 查看节点/话题关系
ros2 run tf2_tools view_frames                  # 生成 TF 树 pdf
ros2 run nav2_map_server map_saver_cli -f ~/map # 手动保存地图
sudo chmod -R 777 文件夹                         # 赋可执行权限
ssh -Y wheeltec@192.168.0.100                   # ssh 登录机器人
xrandr --fb 1024x768                            # vnc 调整分辨率
echo 'export ROBOT_TYPE=s300_pro'>> ~/.bashrc   # 切换车型
```

---

> 更多详细操作请参考各功能包内文档及官方功能手册。
