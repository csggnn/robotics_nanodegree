#!/bin/sh

export TURTLEBOT_GAZEBO_WORLD_FILE=`rospack find turtlebot_gazebo`/worlds/small_world.world
export TURTLEBOT_GAZEBO_MAP_FILE=`rospack find turtlebot_gazebo`/maps/small_world_map.yaml
sleep 5
xterm -e " roslaunch turtlebot_gazebo turtlebot_world.launch" &

sleep 5
xterm -e " roslaunch turtlebot_gazebo amcl_demo.launch" &

sleep 5
xterm -e " roslaunch turtlebot_rviz_launchers view_navigation.launch" &

sleep 5
xterm -e " rosrun add_markers manage_tasks_and_makers" &

sleep 5
xterm -e " rosrun add_markers pick_objects_managed" &
