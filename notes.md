#Generic#
pass: robo-nd

#Ros essentials#
2 options for communication  between nodes: publish/subscribe or Services

##pubish/subscribe##
In the C++ code, the publisher must advertise its messages:
```c++
   ros::Publisher pub = n.advertise<std_msgs::Float64>("message_name", N /* queue size */);`
   pub.publish(message  /* of type std_msgs::Float64 */)
```
use `ros::Rate loop_rate(rate /*hz*/);` & `loop_rate.sleep()` to control message publishing rate.

The CmakeLists.txt file must include 
```javascript
   find_package(catkin REQUIRED COMPONENTS
        std_msgs
        message_generation
        controller_manager
)
```
moreover, aside from the standard:
```
   include_directories(include ${catkin_INCLUDE_DIRS})
   add_executable(node_name node_src_file.cpp)
   target_link_libraries(node_name ${catkin_LIBRARIES})
```
I must add the following to generate the message header files:
`add_dependencies(node_name package_name_generate_messages_cpp)`


