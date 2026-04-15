#include "rog_map/rog_map.h"
#include <rclcpp/rclcpp.hpp>

int main(int argc, char** argv) {
    // 1. 初始化 ROS 2 节点环境
    rclcpp::init(argc, argv);

    // 2. 创建 ROS 2 Node 实例
    // rclcpp::NodeOptions().allow_undeclared_parameters(true) 允许我们在外部 launch 文件中随意传入参数
    rclcpp::NodeOptions options;
    options.allow_undeclared_parameters(true);
    options.automatically_declare_parameters_from_overrides(true);
    auto node = std::make_shared<rclcpp::Node>("rm_node", options);

    // 3. 设置 PCL 终端输出级别 (保持不变)
    pcl::console::setVerbosityLevel(pcl::console::L_ALWAYS);

    // 4. 实例化 ROGMap 指针，并将 node 句柄传入
    rog_map::ROGMap::Ptr rog_map_ptr = std::make_shared<rog_map::ROGMap>(node);

    RCLCPP_INFO(node->get_logger(), "\033[1;32mROG-Map Node has been successfully started!\033[0m");

    // 5. 相当于 ROS 1 中的 ros::AsyncSpinner(0); 自动利用所有可用的 CPU 核心来处理回调
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);

    // 6. 阻塞并处理所有回调 (相当于 ros::waitForShutdown())
    executor.spin();

    // 7. 关闭 ROS 2
    rclcpp::shutdown();
    return 0;
}