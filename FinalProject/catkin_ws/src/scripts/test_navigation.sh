#!/bin/sh
export TURTLEBOT_GAZEBO_WORLD_FILE=`rospack find simulation_data`/worlds/small_world.world
export TURTLEBOT_GAZEBO_MAP_FILE=`rospack find simulation_data`/maps/small_world_gmap_1.yaml
sleep 5
xterm -e " roslaunch turtlebot_gazebo turtlebot_world.launch" &

sleep 5
#xterm -e " roslaunch turtlebot_gazebo amcl_demo.launch initial_pose_a:=-1.57" &
xterm -e " roslaunch turtlebot_gazebo amcl_demo.launch" &

sleep 5
xterm -e " roslaunch turtlebot_rviz_launchers view_navigation.launch" &
