#!/bin/sh
export TURTLEBOT_GAZEBO_WORLD_FILE=`rospack find simulation_data`/worlds/small_world.world
export TURTLEBOT_GAZEBO_MAP_FILE=`rospack find simulation_data`/maps/small_world_map.yaml

source  devel/setup.sh
sleep 5
xterm -e " roslaunch turtlebot_gazebo turtlebot_world.launch" &
sleep 5
xterm -e " roslaunch turtlebot_teleop keyboard_teleop.launch" &
sleep 5
xterm -e " roslaunch turtlebot_gazebo gmapping_demo.launch" &
sleep 5
xterm -e " roslaunch turtlebot_rviz_launchers view_navigation.launch" &
