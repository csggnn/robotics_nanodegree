/**
 * @file pick_object_managed.cpp
 * @author Gianni Casagrande (gianni.casagrande@be.bosch.com)
 * @brief this version of pick object reads and drives to target locaions published to drive_to_point
 * @version 0.1
 * @date 2021-11-01
 * 
 * Copyright 2021 The Kobi Company. All Rights Reserved.
 * CONFIDENTIAL. AUTHORIZED USE ONLY. DO NOT REDISTRIBUTE.
 * 
 */

#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

void driveToGoal(geometry_msgs::Point tg_pos)
{
  move_base_msgs::MoveBaseGoal goal;

  goal.position = tg_pos;
  goal.header.frame_id = "map";
  goal.header.stamp = ros::Time::now();
  ROS_INFO("Sending goal");
  ac.sendGoal(goal);
  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached its goal
  if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Hooray, the base moved to: %f, %f, %f!", tg_pos.x, tg_pos.y, tg_pos.z);
  else
    ROS_INFO("The base failed to move for some reason");
}

int main(int argc, char **argv)
{
  // Initialize the simple_navigation_goals node
  ros::init(argc, argv, "pick_objects");

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  // Wait 5 sec for move_base action server to come up
  while (!ac.waitForServer(ros::Duration(5.0)))
  {
    ROS_INFO("Waiting for the move_base action server to come up");
  }
  ros::NodeHandle n;
  n.subscribe("/drive_to_point", 3, &driveToGoal);
  ros::spin();

  return 0;
}
