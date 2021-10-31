#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
  // Initialize the simple_navigation_goals node
  ros::init(argc, argv, "pick_objects");

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  // Wait 5 sec for move_base action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  // set up the frame parameters
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();

  std::vector<geometry_msgs::Pose> target_poses(2);
  
  target_poses[0].position.x = 1.0;
  target_poses[0].orientation.w = 1.0;
  target_poses[1].position.x = 1.0;
  target_poses[1].position.y = 1.0;
  target_poses[1].orientation.w = 1.5;


  for (int p_i = 0; p_i < (int)target_poses.size(); p_i++) {  // Define a position and orientation for the robot to reach
    geometry_msgs::Pose const&  p = target_poses[p_i];
    goal.target_pose.pose = p;

    // Send the goal position and orientation for the robot to reach
    ROS_INFO("Sending goal");
    ac.sendGoal(goal);

    // Wait an infinite time for the results
    ac.waitForResult();

    // Check if the robot reached its goal
    if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
      ROS_INFO("Hooray, the base moved to pose %d : %f, %f, %f!", p_i, p.position.x, p.position.y, p.position.z);
    else
      ROS_INFO("The base failed to move for some reason");
    if (p_i == 0 ) {
      sleep(5);
    }
  }

  return 0;
}
