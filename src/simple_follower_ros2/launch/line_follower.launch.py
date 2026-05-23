import os
import launch_ros.actions
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import (DeclareLaunchArgument, IncludeLaunchDescription)
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch.conditions import IfCondition
from launch.conditions import UnlessCondition
from launch.actions import ExecuteProcess

def generate_launch_description():
    is_uvc_cam = LaunchConfiguration('is_uvc_cam', default='false')

    bringup_dir = get_package_share_directory('turn_on_wheeltec_robot')
    launch_dir = os.path.join(bringup_dir, 'launch')

    wheeltec_camera = IncludeLaunchDescription(
            PythonLaunchDescriptionSource(os.path.join(launch_dir, 'wheeltec_camera.launch.py')),
    )
    wheeltec_robot = IncludeLaunchDescription(
            PythonLaunchDescriptionSource(os.path.join(launch_dir, 'turn_on_wheeltec_robot.launch.py')),
    )
    return LaunchDescription([
        wheeltec_robot,wheeltec_camera,

        launch_ros.actions.Node(
            package='simple_follower_ros2',
            executable='line_follow',
            name='line_follow',
            parameters=[{'cmd_vel_topic': 'cmd_vel_raw'}],
            ),
        launch_ros.actions.Node(
            package='simple_follower_ros2',
            executable='safety_guard',
            name='safety_guard',
            output='screen',
            parameters=[{
                'stop_distance': 0.25,
                'slow_distance': 0.60,
                'min_valid_distance': 0.05,
                'front_sonars': ['B', 'C', 'D', 'E'],
                'accel_spike_threshold': 6.0,
                'accel_baseline_alpha': 0.02,
                'min_cmd_for_collision': 0.05,
                'recovery_duration': 1.5,
                'backup_duration': 0.4,
                'backup_speed': 0.05,
                'cmd_timeout': 0.5,
            }],
            ),
        ]
    )

