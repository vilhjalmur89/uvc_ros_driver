cmake_minimum_required(VERSION 2.8.3)
project(uvc_ros_driver)

if(NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
endif()

# if traget is an ARM processor uncomment this line to compile with NEON instruction
# set(NEON_FLAGS "-DENABLE_NEON -mfloat-abi=hard -mfpu=neon-vfpv4 -mcpu=cortex-a15 -Ofast")
set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -Wno-format-security ${NEON_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${NEON_FLAGS}")

find_package(rosidl_default_generators REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

find_package(example_interfaces REQUIRED)
find_package(rclc REQUIRED)
find_package(rcl REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rmw REQUIRED)
find_package(rmw_implementation_cmake REQUIRED)
find_package(rmw_implementation REQUIRED)

find_package(ait_ros_messages REQUIRED)
# find_package(image_transport REQUIRED)

find_package(OpenCV REQUIRED)

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})
add_definitions(${EIGEN3_DEFINITIONS})


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Wall -std=c++0x -D__STRICT_ANSI__")


include_directories(
  include
  ros_api/ros2_api/uvc_ros_driver
  /usr/include/libusb-1.0
  src/nodes/c_library
  src/nodes
)

# catkin_package(
#   INCLUDE_DIRS include ros_api/ros1_api/uvc_ros_driver
#   LIBRARIES uvc_ros_driver
#   CATKIN_DEPENDS roscpp roslib image_transport
# #  DEPENDS system_lib
#   DEPENDS yaml-cpp
# )





function(custom_executable subfolder target)
  cmake_parse_arguments(
    ARG "" "" "TARGET_DEPENDENCIES" ${ARGN})

  ## Declare a C++ library
  # add_library(uvc_ros_driver${target_suffix} SHARED
  #   src/uvc_ros_driver.cpp
  #   src/serial_port.cpp
  #   src/stereo_homography.cpp
  #   src/fpga_calibration.cpp
  # )
  # ament_target_dependencies(uvc_ros_driver${target_suffix} "ait_ros_messages")
  # target_link_libraries(uvc_ros_driver${target_suffix}
  #   yaml-cpp
  #   /usr/local/lib/libuvc.so #TODO link by usign find_package
  # )

  # install(TARGETS uvc_ros_driver${target_suffix} 
  #         ARCHIVE DESTINATION lib
  #         LIBRARY DESTINATION lib 
  #         RUNTIME DESTINATION bin)

  add_executable(${target}${target_suffix} src/${subfolder}/${target}.cpp
                      src/uvc_ros_driver.cpp
                      src/serial_port.cpp
                      src/stereo_homography.cpp
                      src/fpga_calibration.cpp)
  
  ament_target_dependencies(${target}${target_suffix}
    "rclcpp${target_suffix}"
    "sensor_msgs"
    "std_msgs"
    "OpenCV"
    ${ARG_TARGET_DEPENDENCIES})

  # target_link_libraries(${target}${target_suffix} uvc_ros_driver${target_suffix})
    # ament_target_dependencies(uvc_ros_driver${target_suffix} "ait_ros_messages")
  target_link_libraries(${target}${target_suffix}
    yaml-cpp
    /usr/local/lib/libuvc.so #TODO link by usign find_package
  )
  
  install(TARGETS ${target}${target_suffix}
    DESTINATION bin)
endfunction()


macro(targets)
  if(NOT target_suffix STREQUAL "")
    get_rclcpp_information("${rmw_implementation}" "rclcpp${target_suffix}")
  endif()


  custom_executable(nodes uvc_ros_driver_node 
                    TARGET_DEPENDENCIES "ait_ros_messages")

endmacro()

call_for_each_rmw_implementation(targets GENERATE_DEFAULT)


# ## Declare a C++ executable
# add_executable(uvc_ros_driver_node
# 	src/nodes/uvc_ros_driver_node.cpp
# )

# ament_target_dependencies(uvc_ros_driver_node "ait_ros_messages")

# ## Specify libraries to link a library or executable target against
# target_link_libraries(uvc_ros_driver_node
#   uvc_ros_driver
#   ${rclcpp_LIBRARIES}
#   ${rmw_implementation_LIBRARIES}
#   ${tf2_ros_LIBRARIES}
#   ${tf2_LIBRARIES}
#   ${std_msgs_LIBRARIES}
#   ${nav_msgs_LIBRARIES}
#   ${geometry_msgs_LIBRARIES}
# )


# ament_export_dependencies(
#   rclcpp
#   std_interfaces
# )

# ament_export_include_directories(include)
# ament_export_libraries(uvc_ros_driver)

# install(TARGETS uvc_ros_driver uvc_ros_driver_node
#         ARCHIVE DESTINATION lib
#         LIBRARY DESTINATION lib 
#         RUNTIME DESTINATION bin)

ament_package()
