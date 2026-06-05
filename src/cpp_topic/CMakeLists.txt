cmake_minimum_required(VERSION 3.8)
project(cpp_topic)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(turtlesim REQUIRED)
find_package(std_msgs)
find_package(status_interfaces)

add_executable(turtle_circle src/turtle_circle.cpp)
add_executable(turtle_control src/turtle_control.cpp)
add_executable(turtle_patrol src/turtle_patrol.cpp)
add_executable(listenr src/listenr.cpp)
add_executable(talker src/talker.cpp)
add_executable(custom_get_msg src/custom_get_msg.cpp)
add_executable(custom_send_msg src/custom_send_msg.cpp)
add_executable(system_status_publisher src/system_status_publisher.cpp)
add_executable(talker_param src/talker_param.cpp)
add_executable(listenr_param src/listenr_param.cpp)

ament_target_dependencies(turtle_circle rclcpp geometry_msgs turtlesim)
ament_target_dependencies(turtle_control rclcpp geometry_msgs turtlesim)
ament_target_dependencies(turtle_patrol rclcpp geometry_msgs turtlesim)
ament_target_dependencies(listenr rclcpp geometry_msgs turtlesim)
ament_target_dependencies(talker rclcpp geometry_msgs turtlesim)
ament_target_dependencies(custom_get_msg rclcpp status_interfaces)
ament_target_dependencies(custom_send_msg rclcpp status_interfaces)
ament_target_dependencies(system_status_publisher rclcpp status_interfaces)
ament_target_dependencies(talker_param rclcpp std_msgs)
ament_target_dependencies(listenr_param rclcpp std_msgs)
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()
install(TARGETS turtle_circle turtle_control turtle_patrol talker listenr custom_send_msg custom_get_msg system_status_publisher talker_param listenr_param
DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch
DESTINATION share/${PROJECT_NAME}
)
ament_package()
