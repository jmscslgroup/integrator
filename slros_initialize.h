#ifndef _SLROS_INITIALIZE_H_
#define _SLROS_INITIALIZE_H_

#include "slros_busmsg_conversion.h"
#include "slros_generic.h"

extern ros::NodeHandle * SLROSNodePtr;
extern const std::string SLROSNodeName;

// For Block integrator/Subscribe1
extern SimulinkSubscriber<std_msgs::Float64, SL_Bus_integrator_std_msgs_Float64> Sub_integrator_14;

// For Block integrator/Publish
extern SimulinkPublisher<geometry_msgs::Twist, SL_Bus_integrator_geometry_msgs_Twist> Pub_integrator_19;

void slros_node_init(int argc, char** argv);

#endif
