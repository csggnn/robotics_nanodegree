#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    
	ball_chaser::DriveToTarget srv;
	srv.request.linear_x  = lin_x;
	srv.request.angular_z = ang_z;

	// Call the command_robot service and pass the requested joint angles
	if (!client.call(srv))
        ROS_ERROR("Failed to call service command_robot");
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    const unsigned char kWhiteColor = 255;
    const int kMinWhitePixels = 4;
    
    const float kTurnLimit = 0.2;
    
    double white_pos_x_sum = 0.0;
    int    white_count = 0;
    
        

    if (img.encoding.compare("rgb8")!=0) {
        ROS_INFO("unexpected image encoding > %s", img.encoding.c_str());
                drive_robot(0., 0.);
                return;
    }
   
    
    for (unsigned int ix = 0; ix < img.width; ix++) {
        for (unsigned int iy = 0; iy < img.height; iy++) {
            bool white = true;
            for (int i=0; i<3; i++)
                if (img.data[3*ix+i + img.step*iy] < kWhiteColor) {
                    white = false;
                    break;
                }
            if (white) {
                white_pos_x_sum += (double)ix;
                white_count++;
            }
        }
    }
    double ball_dir;
    if (white_count <= kMinWhitePixels) {
        /* stay where you are */
        drive_robot(0., 0.);
            ROS_INFO("ProcIm: stay");
    } else {
        /* ball direction in [-1, +1] range */
        ball_dir = (white_pos_x_sum/(double) white_count) * (2.0/(double)(img.width-1))-1.0;
        if (ball_dir > kTurnLimit) {
            /* turn right */
            drive_robot(0., -1.);
            ROS_INFO("ProcIm: turn right");
        } else if (ball_dir < -kTurnLimit) {
            /* turn left */
            ROS_INFO("ProcIm: turn left");
            drive_robot(0., 1.);
        } else {
            /* go forward */ 
            ROS_INFO("ProcIm: got forward");
            drive_robot(1., 0.);
        }
    }    

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
