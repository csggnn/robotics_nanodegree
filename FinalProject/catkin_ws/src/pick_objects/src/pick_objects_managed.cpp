/**
 * @file pick_object_managed.cpp
 * @author Gianni Casagrande (gianni.casagrande.mail@gmail.com)
 * @brief this version of pick object reads and drives to target locations published to drive_to_point
 * @version 0.1
 * @date 2021-11-01
 */

#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <std_msgs/Bool.h>

// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

class Driver
{
public:
  Driver(MoveBaseClient *mbc) : mbc_(mbc)
  {
    result_pub_ = n_.advertise<std_msgs::Bool>("pick_objects_result", 1);
  };

  void driveToGoal(geometry_msgs::Point tg_pos)
  {
    ROS_INFO("drive_to_point triggers driveToGoal");
    move_base_msgs::MoveBaseGoal goal;

    goal.target_pose.pose.orientation.x = 0.0;
    goal.target_pose.pose.orientation.y = 0.0;
    goal.target_pose.pose.orientation.z = 0.0;
    goal.target_pose.pose.orientation.w = 1.0;
    goal.target_pose.pose.position = tg_pos;
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
    ROS_INFO("Sending goal");
    mbc_->sendGoal(goal);
    // Wait an infinite time for the results
    mbc_->waitForResult();
    // Check if the robot reached its goal
    while (result_pub_.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return;
      }
      ROS_WARN_ONCE("Please create a subscriber to the pick_objects_result topic");
      sleep(1);
    }
    std_msgs::Bool goal_reached;
    if (mbc_->getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
      ROS_INFO("Hooray, the base moved to: %f, %f, %f!", tg_pos.x, tg_pos.y, tg_pos.z);
      goal_reached.data = true;
      result_pub_.publish(goal_reached);
    }
    else
    {
      ROS_INFO("The base failed to move for some reason");
      goal_reached.data = false;
      result_pub_.publish(goal_reached);
    }
  };

private:
  ros::Publisher result_pub_;
  ros::NodeHandle n_;
  MoveBaseClient *mbc_ = nullptr;
};

int main(int argc, char **argv)
{
  // Initialize the "pick_objects_managed" node
  ros::init(argc, argv, "pick_objects_managed");
  ROS_INFO("pick_objects_managed initialized");
  //tell the action client that we want to spin a thread by default
  MoveBaseClient move_base_client("move_base", true);
  ROS_INFO("MoveBaseClient created");
  Driver d(&move_base_client);
  ROS_INFO("Driver created");

  // Wait 5 sec for move_base action server to come up
  while (!move_base_client.waitForServer(ros::Duration(5.0)))
  {
    ROS_INFO("Waiting for the move_base action server to come up");
  }
  ros::NodeHandle n;
  ROS_INFO("subscribing to `drive_to_point`");
  ros::Subscriber sub = n.subscribe("/drive_to_point", 3, &Driver::driveToGoal, &d);
  ros::spin();

  return 0;
}
