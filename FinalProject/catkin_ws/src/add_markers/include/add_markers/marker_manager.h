#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <vector>

#pragma once

struct ObjectPickingTask
{
  ObjectPickingTask(double x, double y, double to_x, double to_y, double col_r = 0.5, double col_g = 0.5, double col_b = 0.5) : src_x(x), src_y(y), dst_x(to_x), dst_y(to_y), r(col_r), g(col_g), b(col_b) {}
  double src_x;
  double src_y;
  double dst_x;
  double dst_y;
  double r;
  double g;
  double b;
};

/**
 * @brief MarkerManager publishes pick/drop tasks, and visualizes the markers and drop off locations.
 * 
 * When robot reaches the navigation goal for a task, MarkerManager simulates a pick up/ drop off by hiding/visualizing 
 * a marker and, then publishes a new pick/up / drop off task until all tasks are completed. 
 * 
 */
class MarkerManager
{

  public:
 
  MarkerManager(std::vector<ObjectPickingTask> const& tasks);

/**
 * @brief  draw all objects and drop off locations
 * 
 */
  void start(); 

  enum DrawMarkerType {
    kDst = 0,
    kStart,
    kPick, 
    kDrop
  };

  private:

  std::vector<ObjectPickingTask> tasks_;
  ros::NodeHandle n_;
  ros::Publisher marker_pub_; /* publishes markers for visualization */
  ros::Publisher drive_pub_;  /* publishes drive targets, read by the pick object node */

  int curr_obj_id_ = -1;         /* which object are we picking up/delivering */
  bool pickup_task_ = true;      /* true if performing a pick-up, false if performing a drop-off */

  //bool target_point_valid_ = false; /* whether this target point is valid (starts at false) */
   

  /**
   * @brief assess whether the current task_id_ is valid *
   * 
   * @return true  curr_obj_id_ is valid
   * @return false curr_obj_id_ corresponds to no available task
   */
  bool validCurrTask() const;

  /**
   * @brief Get the target location of the current task (may be a pick-up or a drop-off location)
   * 
   * @param x output x coordinate
   * @param y output y coordinate
   * @return true    there is and actual target location: @p x and @p y are valid
   * @return false   there is no object task up at the moment, @p x and @p y have not been modified.
   */
  bool getCurrTaskTg(double& x, double& y) const;  


  /**
   * @brief Publish the drive goal corresponding to the current task.
   * 
   */
  void publishDriveGoal() const;



  visualization_msgs::Marker drawMarker(int obj_id, DrawMarkerType type) const;


  /**
   * @brief check if the robot reached the current goal location.
   * if it is the case, simulate a pick-up/drop-off and trigger the next task
   * 
   * @param odom_pose 
   */
  void checkGoalReached(geometry_msgs::Pose const& odom_pose);

   
  /**
   * @brief subscribes to odometry and keeps on checking whether a goal has been reched.
   * 
   */
  void checkOdomPos();



};
