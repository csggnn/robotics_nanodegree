#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <vector>

struct ObjectPickingTask {
  ObjectPickingTask(double x, double y, double to_x, double to_y, double col_r=0.5, double col_g=0.5, double col_b=0.5) :src_x(x), src_y(y), dst_x(to_x), dst_y(to_y), r(col_r), g(col_g), b(col_b) {}
  double src_x;
  double src_y;
  double dst_x;
  double dst_y;
  double r;
  double g;
  double b;
};


class MarkerManager{

  std::vector<ObjectPickingTask> tasks_;
  ros::NodeHandle n_;
  ros::Publisher marker_pub_;
  ros::Publisher drive_pub_;

  public: 
  void initialize(); 
}


  

