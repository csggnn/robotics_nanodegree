#include <add_markers/marker_manager.h>

MarkerManager::MarkerManager(std::vector<ObjectPickingTask> const &tasks)
{
  tasks_ = tasks;
}

void MarkerManager::publishObjectMarkers(int obj_id, bool publish_dst_loc) const
{

  ObjectPickingTask const &t = tasks_[obj_id];
  visualization_msgs::Marker marker;

  marker.pose.orientation.x = 0.0;
  marker.pose.orientation.y = 0.0;
  marker.pose.orientation.z = 0.0;
  marker.pose.orientation.w = 1.0;

  marker.pose.position.z = 0.0;

  marker.header.frame_id = "map";
  marker.header.stamp = ros::Time::now();
  marker.lifetime = ros::Duration();

  marker.id = obj_id;

  marker.type = visualization_msgs::Marker::CYLINDER;

  marker.color.a = 1.0;

  marker.scale.x = 0.2;
  marker.scale.y = 0.2;
  marker.scale.z = 0.3;

  marker.color.r = t.r;
  marker.color.g = t.g;
  marker.color.b = t.b;

  marker.ns = "objects";

  switch (t.status)
  {
  case TaskStatus::kWaiting:
  case TaskStatus::kPicking:
  {
    marker.pose.position.x = t.src_x;
    marker.pose.position.y = t.src_y;
    marker.action = visualization_msgs::Marker::ADD;
    break;
  }
  case TaskStatus::kMoving:
  {
    marker.pose.position.x = t.src_x;
    marker.pose.position.y = t.src_y;
    marker.action = visualization_msgs::Marker::DELETE;
    break;
  }
  case TaskStatus::kDropped:
  {
    marker.pose.position.x = t.dst_x;
    marker.pose.position.y = t.dst_y;
    marker.action = visualization_msgs::Marker::ADD;
    break;
  }
  case TaskStatus::kFailed:
  {
    marker.pose.position.x = t.fail_x;
    marker.pose.position.y = t.fail_y;
      /* Failed markers are plotted darker. */

    marker.color.r = t.r * 0.7;
    marker.color.g = t.g * 0.7;
    marker.color.b = t.b * 0.7;
    marker.action = visualization_msgs::Marker::ADD;
    break;
  }
  default:
    ROS_WARN_ONCE("publishObjectMarkers: TaskStatus %d is unknown", (int)(t.status));
  }
  while (marker_pub_.getNumSubscribers() < 1)
  {
    if (!ros::ok())
    {
      return;
    }
    ROS_WARN_ONCE("Please create a subscriber to the marker");
    sleep(1);
  }
  marker_pub_.publish(marker);
  ROS_INFO("Published marker %d, at [%f, %f, %f], color [%4.0f %4.0f %4.0f]",
           obj_id,
           marker.pose.position.x, marker.pose.position.y, marker.pose.position.z,
           marker.color.r, marker.color.g, marker.color.b);


  /* Stop here if you do not want to publish destination locations. */

  if (!publish_dst_loc)
    return;

  visualization_msgs::Marker dst_marker = marker;

  dst_marker.scale.x = 0.5;
  dst_marker.scale.y = 0.5;
  dst_marker.scale.z = 0.05;

  /* destinations are darker than objects so that it is easy to see the object at its destination. */
  dst_marker.color.r *= 0.8;
  dst_marker.color.g *= 0.8;
  dst_marker.color.b *= 0.8;

  dst_marker.ns = "destination_locations";
  dst_marker.pose.position.x = t.dst_x;
  dst_marker.pose.position.y = t.dst_y;

  dst_marker.action = visualization_msgs::Marker::ADD;

  while (marker_pub_.getNumSubscribers() < 1)
  {
    if (!ros::ok())
    {
      return;
    }
    ROS_WARN_ONCE("Please create a subscriber to the marker");
    sleep(1);
  }
  marker_pub_.publish(dst_marker);
  ROS_INFO("Published dst marker %d at [%f, %f, %f], color [%4.0f %4.0f %4.0f]",
           obj_id,
           dst_marker.pose.position.x, dst_marker.pose.position.y, dst_marker.pose.position.z,
           dst_marker.color.r, dst_marker.color.g, dst_marker.color.b);
}


void MarkerManager::publishAllMarkers(bool publish_dst_loc) const {
  for (int i = 0; i< tasks_.size(); i++ ) 
    publishObjectMarkers(i, publish_dst_loc);
}

void MarkerManager::start()
{
  marker_pub_ = n_.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  drive_pub_ = n_.advertise<geometry_msgs::Point>("drive_to_point", 1);
  if (tasks_.size() > 0)
  {
    curr_obj_id_ = 0;
    tasks_[curr_obj_id_].status = TaskStatus::kPicking;
    spinOnOdomPos();
  }
  else
  {
    ROS_INFO("No tasks!");
  }
}

bool MarkerManager::validCurrTask() const
{
  return ((curr_obj_id_ >= 0) && (curr_obj_id_ < tasks_.size()));
}

bool MarkerManager::getCurrTaskTg(double &x, double &y) const
{
  if (validCurrTask())
  {
    switch (tasks_[curr_obj_id_].status)
    {
    case TaskStatus::kMoving:
    {
      x = tasks_[curr_obj_id_].dst_x;
      y = tasks_[curr_obj_id_].dst_y;
      break;
    }
    case TaskStatus::kPicking:
    {
      x = tasks_[curr_obj_id_].src_x;
      y = tasks_[curr_obj_id_].src_y;
      break;
    }
    default:
    {
      ROS_WARN_ONCE("MarkerManager::getCurrTaskTg: the current task %d is in status %d, there is nothing to do apparently", curr_obj_id_, (int) tasks_[curr_obj_id_].status);
      return false;
    }
    }
    return true;
  }
  return false;
}

void MarkerManager::publishDriveGoal() const
{
  geometry_msgs::Point target_point;
  target_point.z = 0;
  bool valid = getCurrTaskTg(target_point.x, target_point.y);

  if (!valid)
  {
    ROS_WARN_ONCE("MarkerManager::publishDriveGoal: there is no valid goal location for current task %d", curr_obj_id_);
    return;
  }

  ROS_INFO("publishing goal");

  while (drive_pub_.getNumSubscribers() < 1)
  {
    if (!ros::ok())
    {
      return;
    }
    ROS_WARN_ONCE("Please create a subscriber to the drive commands");
    sleep(1);
  }
  drive_pub_.publish(target_point);
  ROS_INFO("goal location %f, %f published", target_point.x, target_point.y);
}

void MarkerManager::checkReactGoalReached(geometry_msgs::PoseWithCovarianceStamped::ConstPtr const &robot_pose)
{
  static int i = 0;
  double dist_th = 0.25;
  double tg_x, tg_y;
  bool tg_valid = getCurrTaskTg(tg_x, tg_y);
  if (!tg_valid)
  {
    ROS_WARN_ONCE("MarkerManager::checkGoalReached call but no object movement task is active");
    return;
  }
  /* the current task can only be in status  kPicking, or kMoving */
  bool pickup_task = (tasks_[curr_obj_id_].status == TaskStatus::kPicking);

  geometry_msgs::Point const &pos = robot_pose->pose.pose.position;
  double dist = std::sqrt((pos.x - tg_x) * (pos.x - tg_x) + (pos.y - tg_y) * (pos.y - tg_y));

  if (i % 10 == 0) 
    ROS_INFO("robot is at %f, %f : %f m far from the next goal at %f, %f", pos.x, pos.y, dist, tg_x, tg_y);
  i = (i + 1) % 10;
  
  if (dist < dist_th)
  {
    ROS_INFO("Object %d can be %s", curr_obj_id_, pickup_task ? "picked up" : "dropped");
    sleep(2.5); /* pick up/ drop time simulation */
    if (pickup_task)
    {
      tasks_[curr_obj_id_].status = TaskStatus::kMoving;
    }
    else
    {
      tasks_[curr_obj_id_].status = TaskStatus::kDropped;
      if (curr_obj_id_ < tasks_.size()-1)
      {
        curr_obj_id_++;
        if (tasks_[curr_obj_id_].status != TaskStatus::kWaiting)
        {
          ROS_WARN_ONCE("checkReactGoalReached moving to task %d but found in status %d which is not kWaiting", curr_obj_id_, (int) tasks_[curr_obj_id_].status);
        }
        tasks_[curr_obj_id_].status = TaskStatus::kPicking;
      }
      else
      {
        ROS_INFO("All tasks are completed!");
      }
    }
    sleep(2.5); /* pick up/ drop time simulation */
  }
  publishAllMarkers();
  publishDriveGoal();
}

void MarkerManager::spinOnOdomPos()
{
  ros::Subscriber sub = n_.subscribe("/amcl_pose", 3, &MarkerManager::checkReactGoalReached, this);

  ros::spin();
}
