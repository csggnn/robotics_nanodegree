# Home Service Robot Project #

This projects simulates a robot autonomously navigating trough an environment to pick objects and move them to their target locations. 
A robot is manually navigated trough a simulated environment while a SLAM algorithm is used to localize the robot and build a map. 
Using the generated map, path planning and localization are then jointly used to autonomously navigate the robot across goal locations in the environment, simulating the picking up and dropping of objects.

## External Packages ##

The project relies on the following external packages, for which empty directories are provided. The packages should be cloned into the specified directories before testing the project.

- [**gmapping**](https://github.com/ros-perception/slam_gmapping): Simultaneous Localization and Mapping based on [OpenSlam GMapping](https://openslam-org.github.io/gmapping.html).
- [**turtlebot_gazebo**](https://github.com/turtlebot/turtlebot_simulator):  turtlebot robot model which can be deployed in a gazebo environment.
- [**turtlebot/turtlebot_teleop**](https://github.com/turtlebot/turtlebot): provides the ability of manually controlling the robot in the simulated environment. While performing SLAM, manual driving eases the process of exploring the environment and building a map.
- [**turtlebot_interactions/turtlebot_rviz_launchers**](https://github.com/turtlebot/turtlebot_interactions): allows to launch rviz with a default configuration which pre loads visualizations useful for inspecting SLAM, map and robot trajectory.

## Project Packages ##

The packages defined in this project are the following:

- The **pick_object** project implements nodes which publish commands to the actual navigation algorithms
  - *pick_objects_test* drives the robot towards 2 fixed locations.
  - *pick_objects_managed* receives drive target locations published on a `drive_to_point` topic. Publishes the outcome of this drive to point attempt to a `pick_objects_result` topic.
- The **add_markers** project implement nodes to display objects and target locations in rviz and to request drive tasks to *pick_object_managed*
  - *add_markers_test* simply display a marker in 2 different positions
  - *manage_tasks_and_markers* :
    - display a set of objects to be moved and a set of destination locations. In its default configuration, 4 markers and target locations are defined, an one of the target locations can not be reached.
    - publishes target drive locations to a `drive_to_point` topic
    - subscribes to a `pick_objects_result` topic to know whether its `drive_to_point` requests have been fulfilled. 
    - Hangles marker visualization to simulate pick-ups and drop-offs.
- the **simulation_data** package includes the world fle where the robot navigates, and some maps used for testing.


## Project Scripts ##

`home service robot.sh` is the main script used to run the project. Task schefuling and visualization, localization and navigation are used by this script to simulate a robot navigating trough an environment and picking up and dropping objects. AMCL is used for localization on a saved map. 
`test_slam.sh` is the script used to actually run gmapping and build a map of the environment, which is then used by AMCL.
