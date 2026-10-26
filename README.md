# wheeltec_S300

Wheeltec S300 移动机器人 ROS 2 Humble 工作空间，集成底盘、传感器、建图导航、视觉跟随与交互功能。

## 内容导航

- [src/README.md](./src/README.md)：完整环境配置、功能包索引与启动命令
- [src/turn_on_wheeltec_robot/README.md](./src/turn_on_wheeltec_robot/README.md)：底盘驱动说明
- [src/wheeltec_dashboard/README.md](./src/wheeltec_dashboard/README.md)：Web 仪表盘
- [src/wheeltec_yolo/README.md](./src/wheeltec_yolo/README.md)：YOLO 功能与外部依赖
- [yolo_ros.repos](./yolo_ros.repos)：外部源码清单

## 开始使用

先阅读 [工作空间指南](./src/README.md)。`src/` 为源码目录，应在其上一级、即本仓库根目录执行依赖安装与构建。

车型、串口、传感器和外部视觉依赖需要按实际设备配置。
