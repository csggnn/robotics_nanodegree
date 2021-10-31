#include <add_marker/marker_manager.h>


MarketManager::MarketManager(std::vector<ObjectPickingTask> const& tasks) { 
  tasks_ = tasks;
}


void MarketManager::initialize() {
  visualization_msgs::Marker dst_marker;

  dst_marker.scale.x = 0.5;
  dst_marker.scale.y = 0.5;
  dst_marker.scale.z = 0.05;

  dst_marker.pose.orientation.x = 0.0;
  dst_marker.pose.orientation.y = 0.0;
  dst_marker.pose.orientation.z = 0.0;
  dst_marker.pose.orientation.w = 1.0;

  dst_marker.color.a = 1.0;

  dst_marker.pose.position.z = 0;
  
  dst_marker.ns = "destination_locations";
  
  dst_marker.header.frame_id = "/map";

  dst_marker.type = visualization_msgs::Marker::CYLINDER;
  dst_marker.action = visualization_msgs::Marker::ADD;
  dst_marker.lifetime = ros::Duration();

  visualization_msgs::Marker obj_marker = dst_marker;

  obj_marker.ns = "objects";

  obj_marker.scale.x = 0.2;
  obj_marker.scale.y = 0.2;
  obj_marker.scale.z = 0.3;

  for (int i = 0; i < tasks.size(); i++) {
    ObjectPickingTask const& t = tasks_[i];
    dst_marker.id = i;
    dst_marker.header.stamp = ros::Time::now();

    dst_marker.pose.position.x = t.dst_x;
    dst_marker.pose.position.y = t.dst_y;
  
    dst_marker.color.r = t.r*0.7;
    dst_marker.color.g = t.g*0.7;
    dst_marker.color.b = t.b*0.7;

    obj_marker.id = i;
    obj_marker.header.stamp = ros::Time::now();

    obj_marker.pose.position.x = t.src_x;
    obj_marker.pose.position.y = t.src_y;
  
    obj_marker.color.r = t.r;
    obj_marker.color.g = t.g;
    obj_marker.color.b = t.b;

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
}

MarketManager::publishDriveGoal(){

  if (curr_tg == 0){
    target_point_.x = tasks[curr_task].src_x;
    target_point_.y = tasks[curr_task].src_y;
  } else {
    target_point_.x = tasks[curr_task].dst_x;
    target_point_.y = tasks[curr_task].dst_y;
  }
  target_point_.z = 0
  while (drive_tg_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the drive commands");
      sleep(1);
    }
  drive_tg_pub_.publish(target_point_);
  target_point_valid_ = true;
}

MarketManager::checkGoalReached(geometry_msgs::Pose const& odom_pose) {
  double dist_th_sq = 0.1*0.1;
  if ((odom_pose.position.x - target_point_.x)*(odom_pose.position.x - target_point_.x) + 
      (odom_pose.position.y - target_point_.y)*(odom_pose.position.y - target_point_.y) < dist_th_sq) {
  
	sleep(3);
        /* deal with the current marker */
       
        if 
  }

}

MarkerManager::checkOdomPos(){
  ros::Subscriber sub = n.subscribe("/odom", 3, &MarketManager::checkGoalReched, this);
  ros::spin();
}

