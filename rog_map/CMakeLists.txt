cmake_minimum_required(VERSION 3.8) # ROS 2 推荐的最低 CMake 版本
project(rog_map)

# ROS 2 Humble 默认需要 C++17 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE "Release")
# 保留你原有的编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -g -fPIC")
add_compile_options(-DQT_NO_VERSION_TAGGING)
add_compile_options(-Werror=unused-variable)
add_compile_options(-Werror=return-type)

# 宏定义保持不变
add_definitions(-DROOT_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/\")
add_definitions(-DORIGIN_AT_CORNER)
#add_definitions(-DORIGIN_AT_CENTER)

# 1. 寻找 ROS 2 核心构建包和依赖项
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(message_filters REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(pcl_conversions REQUIRED) # 用于替代 ROS 1 的 pcl_ros 点云转换机制

# 2. 寻找第三方依赖库
find_package(Eigen3 REQUIRED)
find_package(PCL REQUIRED)

# 注意：已彻底移除 ROS 1 的 dynamic_reconfigure 相关代码

# 3. 收集源文件
file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h include/*.hpp include/*.cpp)

# 4. 创建静态库目标
add_library(${PROJECT_NAME} STATIC 
    ${srcs}
)

# 5. 设置头文件目录 (Modern CMake 推荐的 Target 形式)
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${Eigen3_INCLUDE_DIRS}
    ${PCL_INCLUDE_DIRS}
)

# 6. 链接 ROS 2 的包 (使用 ament_target_dependencies 自动处理依赖)
ament_target_dependencies(${PROJECT_NAME} PUBLIC
    rclcpp
    std_msgs
    geometry_msgs
    nav_msgs
    visualization_msgs
    message_filters
    sensor_msgs
    tf2_ros
    pcl_conversions
)

# 7. 链接非 ROS 的第三方库
target_link_libraries(${PROJECT_NAME} PUBLIC
    ${PCL_LIBRARIES}
    -ldw
)

# 8. 导出依赖项和目录给依赖该库的其他 ROS 2 包使用
ament_export_include_directories(include)
ament_export_dependencies(
    rclcpp
    std_msgs
    geometry_msgs
    nav_msgs
    visualization_msgs
    message_filters
    sensor_msgs
    tf2_ros
    pcl_conversions
)
ament_export_targets(${PROJECT_NAME}Targets HAS_LIBRARY_TARGET)

# 9. 安装规则
install(
    DIRECTORY include/
    DESTINATION include
)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# 10. 打包
ament_package()