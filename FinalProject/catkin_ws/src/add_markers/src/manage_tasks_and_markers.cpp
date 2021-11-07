
#include <add_markers/marker_manager.h>
#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

int main( int argc, char** argv )
{

  std::vector<ObjectPickingTask> tasks;
  tasks.push_back(ObjectPickingTask(0, 1, 2, 0, 1, 0, 0));
  tasks.push_back(ObjectPickingTask(0, 2, 5, 5, 1, 1, 0));
  tasks.push_back(ObjectPickingTask(-2, 5, 3, -4, 1, 0, 1));
  
  ros::init(argc, argv, "marker_manager");

  MarkerManager m(tasks);
  m.start();

  ROS_INFO("All tasks completed.");

}
