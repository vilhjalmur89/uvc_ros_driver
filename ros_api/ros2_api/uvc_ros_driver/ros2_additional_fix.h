#ifndef GLOBAL_PLANNER_ROS2_ADDITIONAL_FIX
#define GLOBAL_PLANNER_ROS2_ADDITIONAL_FIX

#include <octomap/octomap.h>
#include <octomap_msgs/msg/octomap.hpp>

// #include "avoidance/msg/three_point_msg.hpp"


// Ros2 functionality

// Ros2 added an msg namespace
namespace octomap_msgs { using namespace msg; }
// namespace avoidance { using namespace msg; }

// From octomap_msgs/conversions.h
static inline octomap::AbstractOcTree* msgToMap(const octomap_msgs::msg::Octomap & msg){
  octomap::AbstractOcTree* tree = octomap::AbstractOcTree::createTree(msg.id, msg.resolution);    
  if (tree){
    std::stringstream datastream;
    if (msg.data.size() > 0){
      datastream.write((const char*) &msg.data[0], msg.data.size());
      tree->readData(datastream);
    }
  }
    return tree;      
}

#endif /* GLOBAL_PLANNER_ROS2_ADDITIONAL_FIX */
