import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    pkg_share_dir = get_package_share_directory('rog_map')
    
    # 拼接 YAML 参数文件的完整路径
    config_file = os.path.join(pkg_share_dir, 'config', 'rog_map.yaml')

    # 定义 rm_node 节点
    rm_node = Node(
        package='rog_map',
        executable='rog_map_node', # ROS 2 中 type 改为了 executable
        name='rm_node',
        output='log',                     # 或者改为 'screen' 以在终端显示日志
        parameters=[config_file]          # 直接在这里加载 YAML 参数文件
    )

    return LaunchDescription([
        rm_node,
        # keyboard_control_node
    ])