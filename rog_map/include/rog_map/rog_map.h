/**
* This file is part of ROG-Map
*
* Copyright 2024 Yunfan REN, MaRS Lab, University of Hong Kong, <mars.hku.hk>
* Developed by Yunfan REN <renyf at connect dot hku dot hk>
* for more information see <https://github.com/hku-mars/ROG-Map>.
* If you use this code, please cite the respective publications as
* listed on the above website.
*
* ROG-Map is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ROG-Map is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with ROG-Map. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <rog_map/prob_map.h>

// ROS 2 相关头文件替换
#include <rclcpp/rclcpp.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <rcl_interfaces/msg/set_parameters_result.hpp> // 用于 ROS 2 动态调参

// 移除原有的 ROS 1 dynamic_reconfigure 头文件
// #include <dynamic_reconfigure/server.h>
// #include <rog_map/VizConfig.h>

#include <utils/common_lib.hpp>
#include <utils/visual_utils.hpp>

namespace rog_map {
    using namespace std;

    typedef pcl::PointXYZINormal PointType;
    typedef pcl::PointCloud<PointType> PointCloudXYZIN;

    class ROGMap : public ProbMap {
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW

        typedef shared_ptr<ROGMap> Ptr;

        // 使用 rclcpp::Node 的共享指针替代 ros::NodeHandle
        ROGMap(rclcpp::Node::SharedPtr nh);

        ~ROGMap() = default;


        bool isLineFree(const Vec3f &start_pt, const Vec3f &end_pt,
                        const double &max_dis = 999999,
                        const vec_Vec3i &neighbor_list = vec_Vec3i{}) const;

        bool isLineFree(const Vec3f &start_pt, const Vec3f &end_pt,
                        Vec3f &free_local_goal, const double &max_dis = 999999,
                        const vec_Vec3i &neighbor_list = vec_Vec3i{}) const;

        bool isLineFree(const Vec3f &start_pt, const Vec3f &end_pt,
                        const bool & use_inf_map = false,
                        const bool & use_unk_as_occ = false) const;


        void updateMap(const PointCloud &cloud, const Pose &pose);

        RobotState getRobotState() const;

    private:
        // 节点句柄变为 ROS 2 智能指针
        rclcpp::Node::SharedPtr nh_;

        RobotState robot_state_;

        // 声明用于替代 dynamic_reconfigure 的参数回调句柄
        rclcpp::node_interfaces::OnSetParametersCallbackHandle::SharedPtr param_callback_handle_;

        struct ROSCallback {
            // 订阅器变为 ROS 2 模板类智能指针
            rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub;
            rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr cloud_sub;
            int unfinished_frame_cnt{0};
            Pose pc_pose;
            PointCloud pc;
            // 定时器变为 ROS 2 类型
            rclcpp::TimerBase::SharedPtr update_timer;
            mutex updete_lock;
        } rc_;

        struct VisualizeMap {
            // 发布器变为 ROS 2 模板类智能指针
            rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr occ_pub, unknown_pub,
                    occ_inf_pub, unknown_inf_pub,
                    frontier_pub, esdf_pub, esdf_neg_pub, esdf_occ_pub;
            rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr mkr_arr_pub;
            
            visualization_msgs::msg::MarkerArray mkr_arr;
            rclcpp::TimerBase::SharedPtr viz_timer;
            
            struct VizCfg {
                // 彻底移除了 dynamic_reconfigure::Server 相关变量
                bool use_body_center{false};
                Vec3f box_min, box_max;
            } vizcfg;
        } vm_;

        std::ofstream time_log_file_, map_info_log_file_;

        void updateRobotState(const Pose &pose);

        // 回调函数参数修改为 ROS 2 的 ConstSharedPtr
        void odomCallback(const nav_msgs::msg::Odometry::ConstSharedPtr msg);

        void cloudCallback(const sensor_msgs::msg::PointCloud2::ConstSharedPtr msg);

        // 移除 ROS 1 的 ros::TimerEvent 参数
        void updateCallback();

        // 消息类型加上 msg:: 命名空间
        // static void vecEVec3fToPC2(const vec_E<Vec3f> &points, sensor_msgs::msg::PointCloud2 &cloud);
        void vecEVec3fToPC2(const vec_E<Vec3f> &points, sensor_msgs::msg::PointCloud2 &cloud);

        void vizCallback();

        // 将 dynamic_reconfigure 的回调替换为 ROS 2 参数回调签名
        rcl_interfaces::msg::SetParametersResult VizCfgCallback(const std::vector<rclcpp::Parameter> &parameters);

    };
}