/*
 * Copyright (c) 2010, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <add_markers/marker_manager.h>


int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

  ros::Publisher drive_tg_pub = n.advertise<geometry_msgs::Point>("drive_target", 1);
	
  /* a vector of objects to be moved, from source to destination. The destination will be permanently maked by a low wide cylinder of the same color as the object to be moved */
  std::vector<ObjectPickingTask> tasks;
  tasks.push_back( ObjectPickingTask( 0,1,  2, 0, 1,0,0)); 
  tasks.push_back( ObjectPickingTask( 0,2,  5, 5, 1,1,0));
  tasks.push_back( ObjectPickingTask(-2,5,  3,-4, 1,0,1));

  MarkerManager mm (tasks);

  mm.initialize();
  
  mm.start();

  



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
    ObjectPickingTask const& t = tasks[i];
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

  int curr_task = 0;
  int curr_tg = 0; /* 0 :pick up, 1: deliver */

  geometry_msgs::Point target_point;
  if (curr_tg == 0){
    target_point.x = tasks[curr_task].src_x;
    target_point.y = tasks[curr_task].src_y;
  } else {
    target_point.x = tasks[curr_task].dst_x;
    target_point.y = tasks[curr_task].dst_y;
  }
  target_point.z = 0
  while (drive_tg_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the drive commands");
      sleep(1);
    }
  drive_tg_pub.publish(target_point);


	

  sleep(20);

  

}

