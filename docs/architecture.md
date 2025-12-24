# Wheeltec S300 机器人系统架构

## 1. 整体系统架构图

```mermaid
%%{init: {'theme': 'base', 'themeVariables': { 'primaryColor': '#4a9eff', 'primaryTextColor': '#fff'}}}%%

flowchart TB
    subgraph HARDWARE["🔧 硬件层"]
        LIDAR["激光雷达<br/>LD/LS Lidar"]
        CAM["📷 USB相机<br/>Astra深度相机"]
        IMU["🧭 IMU<br/>Yesense"]
        ULTRA["📡 超声波"]
        MIC["🎤 麦克风"]
        MOTOR["⚙️ 电机驱动"]
    end

    subgraph DRIVER["驱动层"]
        LD["ldlidar_stl_ros2<br/>ldlidar_sl_ros2"]
        LS["lslidar_driver"]
        UCAM["usb_cam_node"]
        ASTRA["astra_camera"]
        YESENSE["yesense_std_ros2"]
        ULTRAD["wheeltec_ultrasonic"]
        MICD["wheeltec_mic_ros2"]
        SERIAL["serial_ros2"]
    end

    subgraph PERCEPTION["感知层"]
        FUSION["double_lidar_fusion<br/>双雷达融合"]
        ARUCO["aruco_ros<br/>标记检测"]
        BODY["bodyreader<br/>人体识别"]
    end

    subgraph SLAM["SLAM层"]
        CARTO["cartographer"]
        STOOL["slam_toolbox"]
        GMAP["gmapping"]
        RTAB["rtab_map"]
    end

    subgraph NAV["导航层"]
        NAV2["wheeltec_nav2<br/>Nav2导航栈"]
        RRT["wheeltec_robot_rrt<br/>RRT规划"]
        PATH["wheeltec_path_follow<br/>路径跟随"]
        WAYPOINT["nav2_waypoint_cycle<br/>航点循环"]
    end

    subgraph TRACK["追踪层"]
        FOLLOW["simple_follower_ros2"]
        KCF["wheeltec_robot_kcf"]
    end

    subgraph CONTROL["控制层"]
        MAIN["turn_on_wheeltec_robot<br/>🤖 主控节点"]
        JOY["wheeltec_joy<br/>🎮 手柄"]
        KEY["keyboard<br/>⌨️ 键盘"]
    end

    subgraph APP["应用层"]
        OLLAMA["ollama_ros_chat<br/>🧠 AI对话"]
        TTS["tts<br/>🔊 语音合成"]
        WEB["web_video_server<br/>🌐 Web监控"]
        QT["qt_ros_test<br/>🖥️ GUI"]
        RVIZ["wheeltec_rviz2<br/>📊 可视化"]
    end

    %% 硬件到驱动
    LIDAR --> LD & LS
    CAM --> UCAM & ASTRA
    IMU --> YESENSE
    ULTRA --> ULTRAD
    MIC --> MICD
    MOTOR <--> SERIAL

    %% 驱动到感知
    LD & LS --> FUSION
    UCAM & ASTRA --> ARUCO & BODY

    %% 感知到SLAM
    FUSION --> CARTO & STOOL & GMAP
    ASTRA --> RTAB

    %% SLAM到导航
    CARTO & STOOL & GMAP & RTAB --> NAV2
    NAV2 --> RRT & PATH & WAYPOINT

    %% 追踪
    ARUCO --> FOLLOW
    BODY --> KCF
    FUSION --> FOLLOW

    %% 到主控
    NAV2 & PATH & FOLLOW & KCF --> MAIN
    JOY & KEY --> MAIN
    YESENSE --> MAIN
    MAIN --> SERIAL

    %% 应用层
    MICD --> OLLAMA
    OLLAMA --> TTS
    UCAM & ASTRA --> WEB
    MAIN --> QT & RVIZ

    classDef hw fill:#607d8b,stroke:#455a64,color:#fff
    classDef driver fill:#795548,stroke:#5d4037,color:#fff
    classDef percept fill:#ff9800,stroke:#f57c00,color:#fff
    classDef slam fill:#e91e63,stroke:#c2185b,color:#fff
    classDef nav fill:#4caf50,stroke:#388e3c,color:#fff
    classDef track fill:#00bcd4,stroke:#0097a7,color:#fff
    classDef ctrl fill:#f44336,stroke:#d32f2f,color:#fff
    classDef app fill:#9c27b0,stroke:#7b1fa2,color:#fff

    class LIDAR,CAM,IMU,ULTRA,MIC,MOTOR hw
    class LD,LS,UCAM,ASTRA,YESENSE,ULTRAD,MICD,SERIAL driver
    class FUSION,ARUCO,BODY percept
    class CARTO,STOOL,GMAP,RTAB slam
    class NAV2,RRT,PATH,WAYPOINT nav
    class FOLLOW,KCF track
    class MAIN,JOY,KEY ctrl
    class OLLAMA,TTS,WEB,QT,RVIZ app
```

---

## 2. 导航系统数据流

```mermaid
%%{init: {'theme': 'base', 'themeVariables': { 'primaryColor': '#4a9eff'}}}%%

flowchart LR
    subgraph SENSORS["传感器"]
        LIDAR["🔴 激光雷达"]
        ODOM["📍 里程计"]
        IMU["🧭 IMU"]
    end

    subgraph TOPICS_IN["输入 Topics"]
        T1["/scan"]
        T2["/odom"]
        T3["/imu/data"]
        T4["/tf"]
    end

    subgraph SLAM["SLAM"]
        SLAM_NODE["slam_toolbox<br/>cartographer"]
    end

    subgraph MAP["地图"]
        T5["/map"]
        T6["/map_metadata"]
    end

    subgraph NAV["Nav2 导航栈"]
        PLAN["planner_server<br/>全局规划"]
        CTRL["controller_server<br/>局部控制"]
        BT["bt_navigator<br/>行为树"]
        REC["recoveries_server<br/>恢复行为"]
    end

    subgraph TOPICS_OUT["输出 Topics"]
        T7["/cmd_vel"]
        T8["/plan"]
        T9["/local_costmap"]
        T10["/global_costmap"]
    end

    subgraph CONTROL["机器人控制"]
        ROBOT["turn_on_wheeltec_robot"]
    end

    subgraph ACTIONS["Action Servers"]
        A1["/navigate_to_pose"]
        A2["/follow_path"]
        A3["/navigate_through_poses"]
    end

    LIDAR --> T1
    ODOM --> T2
    IMU --> T3

    T1 & T2 --> SLAM_NODE
    SLAM_NODE --> T5 & T6
    T5 --> PLAN & CTRL

    T1 --> CTRL
    T2 & T4 --> PLAN & CTRL

    BT --> PLAN & CTRL & REC
    PLAN --> T8
    CTRL --> T7 & T9
    PLAN --> T10

    T7 --> ROBOT

    A1 & A2 & A3 -.-> BT

    classDef sensor fill:#607d8b,stroke:#455a64,color:#fff
    classDef topic fill:#2196f3,stroke:#1565c0,color:#fff
    classDef slam fill:#e91e63,stroke:#c2185b,color:#fff
    classDef nav fill:#4caf50,stroke:#388e3c,color:#fff
    classDef action fill:#ff9800,stroke:#f57c00,color:#fff
    classDef ctrl fill:#f44336,stroke:#d32f2f,color:#fff

    class LIDAR,ODOM,IMU sensor
    class T1,T2,T3,T4,T5,T6,T7,T8,T9,T10 topic
    class SLAM_NODE slam
    class PLAN,CTRL,BT,REC nav
    class A1,A2,A3 action
    class ROBOT ctrl
```

---

## 3. 视觉追踪系统

```mermaid
%%{init: {'theme': 'base', 'themeVariables': { 'primaryColor': '#4a9eff'}}}%%

flowchart LR
    subgraph HARDWARE["硬件"]
        USB["📷 USB相机"]
        ASTRA["📷 Astra深度相机"]
        LIDAR["🔴 激光雷达"]
    end

    subgraph DRIVER["驱动节点"]
        USB_NODE["usb_cam_node"]
        ASTRA_NODE["astra_camera_node"]
        LIDAR_NODE["ldlidar_node"]
    end

    subgraph TOPICS_CAM["相机 Topics"]
        T1["/camera/image_raw"]
        T2["/camera/camera_info"]
        T3["/camera/depth/image_raw"]
    end

    subgraph TOPICS_LIDAR["激光 Topics"]
        T4["/scan"]
    end

    subgraph FOLLOWER["simple_follower_ros2"]
        VIS["visualFollower<br/>🎨 颜色追踪"]
        LASER["laserFollower<br/>📏 激光追踪"]
        LINE["lineFollower<br/>➖ 线条追踪"]
        AR["arFollower<br/>🏷️ 标记追踪"]
    end

    subgraph DETECTION["检测节点"]
        ARUCO["aruco_ros<br/>ArUco检测"]
        KCF["wheeltec_robot_kcf<br/>KCF追踪"]
        BODY["bodyreader<br/>人体检测"]
    end

    subgraph TOPICS_OUT["输出 Topics"]
        T5["/cmd_vel"]
        T6["/aruco/markers"]
        T7["/body/poses"]
        T8["/target/position"]
    end

    subgraph CONTROL["控制"]
        ROBOT["turn_on_wheeltec_robot"]
    end

    USB --> USB_NODE
    ASTRA --> ASTRA_NODE
    LIDAR --> LIDAR_NODE

    USB_NODE --> T1 & T2
    ASTRA_NODE --> T1 & T2 & T3
    LIDAR_NODE --> T4

    T1 --> VIS & LINE & ARUCO & KCF & BODY
    T2 --> ARUCO
    T3 --> BODY
    T4 --> LASER

    ARUCO --> T6 --> AR
    BODY --> T7
    KCF --> T8

    VIS & LASER & LINE & AR --> T5
    T5 --> ROBOT

    classDef hw fill:#607d8b,stroke:#455a64,color:#fff
    classDef driver fill:#795548,stroke:#5d4037,color:#fff
    classDef topic fill:#2196f3,stroke:#1565c0,color:#fff
    classDef follow fill:#4caf50,stroke:#388e3c,color:#fff
    classDef detect fill:#ff9800,stroke:#f57c00,color:#fff
    classDef ctrl fill:#f44336,stroke:#d32f2f,color:#fff

    class USB,ASTRA,LIDAR hw
    class USB_NODE,ASTRA_NODE,LIDAR_NODE driver
    class T1,T2,T3,T4,T5,T6,T7,T8 topic
    class VIS,LASER,LINE,AR follow
    class ARUCO,KCF,BODY detect
    class ROBOT ctrl
```

---

## 4. AI 交互系统

```mermaid
%%{init: {'theme': 'base', 'themeVariables': { 'primaryColor': '#9c27b0'}}}%%

flowchart LR
    subgraph INPUT["输入设备"]
        MIC["🎤 麦克风"]
        CAM["📷 相机"]
    end

    subgraph DRIVER["驱动"]
        MIC_D["wheeltec_mic_ros2"]
        CAM_D["usb_cam_node"]
    end

    subgraph TOPICS_IN["输入 Topics"]
        T1["/audio/raw"]
        T2["/camera/image_raw"]
    end

    subgraph AI["AI 处理"]
        ASR["语音识别<br/>(ASR)"]
        OLLAMA["ollama_ros_chat<br/>🧠 Ollama LLM"]
        VISION["视觉理解"]
    end

    subgraph TOPICS_AI["AI Topics"]
        T3["/speech/text"]
        T4["/chat/response"]
        T5["/vision/description"]
    end

    subgraph OUTPUT["输出"]
        TTS["tts_node<br/>🔊 语音合成"]
        SPEAKER["🔈 扬声器"]
    end

    subgraph SERVICES["Services"]
        S1["/chat/send<br/>String"]
        S2["/chat/clear<br/>Trigger"]
    end

    MIC --> MIC_D --> T1
    CAM --> CAM_D --> T2

    T1 --> ASR --> T3
    T3 --> OLLAMA
    T2 --> VISION --> T5
    T5 -.-> OLLAMA
    OLLAMA --> T4

    T4 --> TTS --> SPEAKER

    S1 & S2 -.-> OLLAMA

    classDef input fill:#607d8b,stroke:#455a64,color:#fff
    classDef driver fill:#795548,stroke:#5d4037,color:#fff
    classDef topic fill:#2196f3,stroke:#1565c0,color:#fff
    classDef ai fill:#9c27b0,stroke:#7b1fa2,color:#fff
    classDef output fill:#4caf50,stroke:#388e3c,color:#fff
    classDef service fill:#ff9800,stroke:#f57c00,color:#fff

    class MIC,CAM input
    class MIC_D,CAM_D driver
    class T1,T2,T3,T4,T5 topic
    class ASR,OLLAMA,VISION ai
    class TTS,SPEAKER output
    class S1,S2 service
```

---

## 5. 包依赖关系图

```mermaid
%%{init: {'theme': 'base'}}%%

flowchart TD
    subgraph CORE["核心依赖"]
        RCLCPP["rclcpp"]
        RCLPY["rclpy"]
        TF2["tf2_ros"]
    end

    subgraph MSGS["消息类型"]
        SENSOR["sensor_msgs"]
        GEO["geometry_msgs"]
        NAV_M["nav_msgs"]
        STD["std_msgs"]
    end

    subgraph CUSTOM_MSG["自定义消息"]
        ARUCO_M["aruco_msgs"]
        BODY_M["bodyreader_msg"]
        LS_M["lslidar_msgs"]
        MIC_M["wheeltec_mic_msg"]
        OLLAMA_M["ollama_ros_msgs"]
        RRT_M["wheeltec_rrt_msg"]
        ROBOT_I["robot_interfaces"]
    end

    subgraph LIBS["第三方库"]
        OPENCV["OpenCV"]
        PCL["PCL"]
        EIGEN["Eigen3"]
        QT["Qt5"]
    end

    subgraph PACKAGES["功能包"]
        MAIN["turn_on_wheeltec_robot"]
        ARUCO["aruco_ros"]
        SLAM["slam_gmapping"]
        NAV2["wheeltec_nav2"]
        FOLLOW["simple_follower"]
    end

    RCLCPP --> MAIN & ARUCO & SLAM & NAV2
    TF2 --> MAIN & SLAM & NAV2

    SENSOR & GEO --> MAIN
    NAV_M --> SLAM & NAV2

    OPENCV --> ARUCO
    PCL --> SLAM

    ARUCO_M --> ARUCO
    BODY_M --> FOLLOW

    classDef core fill:#f44336,stroke:#d32f2f,color:#fff
    classDef msg fill:#2196f3,stroke:#1565c0,color:#fff
    classDef custom fill:#ff9800,stroke:#f57c00,color:#fff
    classDef lib fill:#607d8b,stroke:#455a64,color:#fff
    classDef pkg fill:#4caf50,stroke:#388e3c,color:#fff

    class RCLCPP,RCLPY,TF2 core
    class SENSOR,GEO,NAV_M,STD msg
    class ARUCO_M,BODY_M,LS_M,MIC_M,OLLAMA_M,RRT_M,ROBOT_I custom
    class OPENCV,PCL,EIGEN,QT lib
    class MAIN,ARUCO,SLAM,NAV2,FOLLOW pkg
```

---

## 功能模块分类表

| 模块 | 包数量 | 主要包 | 功能 |
|------|--------|--------|------|
| **底盘控制** | 5 | turn_on_wheeltec_robot, serial_ros2 | 运动控制、串口通信 |
| **激光雷达** | 4 | ldlidar_*, lslidar_*, double_lidar_fusion | 2D/3D激光扫描 |
| **视觉感知** | 4 | usb_cam, astra_camera, aruco_ros | RGB/深度图像 |
| **SLAM建图** | 5 | cartographer, slam_toolbox, gmapping, rtab_map | 地图构建 |
| **自主导航** | 4 | wheeltec_nav2, path_follow, waypoint_cycle | 路径规划导航 |
| **目标追踪** | 4 | simple_follower, kcf, bodyreader | 人/物体追踪 |
| **AI交互** | 2 | ollama_ros_chat, tts | 语音对话 |
| **Web/GUI** | 2 | web_video_server, qt_ros_test | 远程监控 |
