#include "slros_initialize.h"

ros::NodeHandle * SLROSNodePtr;
const std::string SLROSNodeName = "integrator";

// For Block integrator/Subscribe1
SimulinkSubscriber<std_msgs::Float64, SL_Bus_integrator_std_msgs_Float64> Sub_integrator_14;

// For Block integrator/Publish
SimulinkPublisher<geometry_msgs::Twist, SL_Bus_integrator_geometry_msgs_Twist> Pub_integrator_19;

void slros_node_init(int argc, char** argv)
{
  ros::init(argc, argv, SLROSNodeName);
  SLROSNodePtr = new ros::NodeHandle();
}

