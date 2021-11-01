#include <add_marker/marker_manager.h>

MarkerManager::MarkerManager(std::vector<ObjectPickingTask> const &tasks)
{
  tasks_ = tasks;
}

visualization_msgs::Marker drawMarker(int obj_id, DrawMarkerType type) const
{

  auto const& t = tasks_[obj_id];
  visualization_msgs::Marker marker;

  marker.pose.orientation.x = 0.0;
  marker.pose.orientation.y = 0.0;
  marker.pose.orientation.z = 0.0;
  marker.pose.orientation.w = 1.0;

  marker.pose.position.x = 0.0;

  marker.header.frame_id = "/map";
  marker.header.stamp = ros::Time::now();
  marker.lifetime = ros::Duration();

  marker.id = obj_id;

  marker.type = visualization_msgs::Marker::CYLINDER;

  marker.color.a = 1.0;

  /* destination markers look different */
  if (type == kDst)
  {
    marker.scale.x = 0.5;
    marker.scale.y = 0.5;
    marker.scale.z = 0.05;

    marker.color.r = t.r * 0.7;
    marker.color.g = t.g * 0.7;
    marker.color.b = t.b * 0.7;

    marker.ns = "destination_locations";
  }
  else
  {
    obj_marker.scale.x = 0.2;
    obj_marker.scale.y = 0.2;
    obj_marker.scale.z = 0.3;

    marker.color.r = t.r;
    marker.color.g = t.g;
    marker.color.b = t.b;

    marker.ns = "objects";
  }

  /* pick up and start markers are at the start location (although pick up location may not matter with a DELETE) */
  if ((type = kStart) || (type == kPick))
  {
    marker.pose.position.x = t.src_x;
    marker.pose.position.y = t.src_y;
  }
  else
  {
    marker.pose.position.x = t.dst_x;
    marker.pose.position.y = t.dst_y;
  }

  /* pick up is represented by deleting a marker, all other cases by adding  one */
  if (type == kPick)
  {
    marker.action = visualization_msgs::Marker::DELETE;
  }
  else
  {
    marker.action = visualization_msgs::Marker::ADD;
  }

}

void MarkerManager::start()
{
  marker_pub_ = n_.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  dive_tg_pub_ = n_.advertise<geometry_msgs::Point>("drive_to_point", 1);

  for (int i = 0; i < tasks_.size(); i++)
  {
    auto obj_marker = drawMarker(tasks_[i], i, kStart);
    auto dst_marker = drawMarker(tasks_[i], i, kDst);

    while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
    }
    marker_pub.publish(dst_marker);
    marker_pub.publish(obj_marker);
    sleep(1);
  }
  
  if (tasks.size() >0 ) {
    curr_obj_id_ =0;
    pickup_task_ = true;
    publishDriveGoal();
    checkOdomPos();
  } else {
    ROS_INFO("No tasks!");
  }
}



bool MarkerManager::validCurrTask() const
{
  return ((curr_obj_id_ > 0) && (curr_obj_id_ < tasks.size()));
}

bool MarkerManager::getCurrTaskTg(double &x, double &y) const
{
  if (validCurrTask())
  {
    x = pickup_task_ ? tasks_[curr_obj_id_].src_x : tasks_[curr_obj_id_].dst_x;
    y = pickup_task_ ? tasks_[curr_obj_id_].src_y : tasks_[curr_obj_id_].dst_y;
    return true;
  }
  return false;
}

void MarkerManager::publishDriveGoal()
{

  geometry_msgs::Point target_point;
  target_point.z = 0;
   bool valid = getCurrTaskTg(target_point.x, target_point.y);

  if (!valid)
  {
    ROS_WARN_ONCE("MarkerManager::publishDriveGoal: there is no valid goal location for current task");
    return;
  }

  while (drive_tg_pub.getNumSubscribers() < 1)
  {
    if (!ros::ok())
    {
      return 0;
    }
    ROS_WARN_ONCE("Please create a subscriber to the drive commands");
    sleep(1);
  }
  drive_tg_pub_.publish(target_point);
}

void MarkerManager::checkGoalReached(geometry_msgs::Pose const &odom_pose)
{
  double dist_th_sq = 0.1 * 0.1;
  double tg_x, tg_y;
  bool tg_valid = getCurrTaskTg(tg_x, tg_y);
  if (!tg_valid)
  {
    ROS_WARN_ONCE("MarkerManager::checkGolaReached call but no object movement task is active");
    return;
  }

  if ((odom_pose.position.x - tg_x) * (odom_pose.position.x - tg_x) +
          (odom_pose.position.y - tg_y) * (odom_pose.position.y - tg_y) <
      dist_th_sq)
  {
    sleep(2.5);
    drawMarker(curr_obj_id_, pickup_task_ ? kPick : kDrop);
    sleep(2.5);
    pickup_task_ ! pickup_task_;
    if (pickup_task_) {
      curr_obj_id_ = curr_obj_id_ + 1;
    } 

    if (validCurrTask()) 
      publishDriveGoal();
    else 
      ROS_INFO("All tasks completed");
  }
}

void MarkerManager::checkOdomPos()
{
  ros::Subscriber sub = n_.subscribe("/odom", 3, &MarkerManager::checkGoalReached, this);
  ros::spin();
}
