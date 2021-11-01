

This projects simulates a robot aunonomously navigating trough an environment to pick objects and move them to their target locations.

The project uses the following packages:

- turtlebot simulates the robot, actuators and sensors. 
  - new small_world map and gazebo files have been added to the maps and worlds folders in turtlebot_simulator/turtlebot_gazebo. The additional small_world_trick map has been used for testing, to trick the algorithm in considering one of the map doors as closed.  

- The **pick_object** project implements nodes which publish commands to the actual navigation algorithms
  - *pick_objects_test* drives the robot towards 2 fixed locations.
  - *pick_objects_manages* receives and forwards drive targets published on a `drive_to_point` topic.
- The **add_markers** project implement nodes to display objects and target locations in rviz and to request drive tasks to *pick_object_managed*
  - *add_markers_test* simply display a marker in 2 different positions
  - *manage_tasks_and_markers* :
    - display a set of objects to be moved and a set of destination locations
    - publishes target drive locations to a `drive_to_point` topic
    - checks when the requested drive location have been reached, by subscribing and comparing to odometry messages
    - simulates pick-ups and drop-offs.
