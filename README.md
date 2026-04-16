# 克隆代码

git clone https://github.com/Renwang-Huang/ROG-Map.git

# 编译代码

cd ROG-Map

colcon build

# 修改话题

cd ROG-Map/rog_map/config

修改下面这个部分，把点云话题和里程计话题修改成fast-lio的有关话题名称：

      # ROS 2 Topics configuration
      ros_callback:
        enable: true
        cloud_topic: "/cloud_registered"
        odom_topic: "/Odometry"
        odom_timeout: 2.0

# 启动节点

ros2 launch rog_map rog_map.launch.py

# 启用rviz2可视化

rviz2的配置文件参考：ROG-Map/rog_map/rviz/rog_map_ros2.rviz
