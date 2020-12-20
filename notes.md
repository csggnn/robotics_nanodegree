#Generic#
pass: robo-nd

#Ros essentials#
2 options for communication  between nodes: publish/subscribe or Services

##Publisher/Subscriber##
In the C++ code, the publisher must advertise its messages:
```c++
   ros::Publisher pub = n.advertise<std_msgs::Float64>("message_name", N /* queue size */);
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

In the `simple_mover` node which publishes commands for `simple_arm`. `simple_arm` has its own launch file and I suppose it was already designed to read messages on the specific topic where i was publishing. To run the project, I first start `simple_arm`: 
```
   source devel/setup.bash
   roslaunch simple_arm robot_spawn.launch
```
and then `simple_mover` in separate shells:
```
   source devel/setup.bash
   rosrun simple_arm simple_mover
```

##Services##

Services are provided by a node for "on-deamand" interaction. 
Services have an input and an output and the input/output structure must be defined in a `.srv` file in `node/srv` folder. 

Example content of a `.srv` file:
```
   float64 input
   float64 input2
   ---
   string output_msg
```

These service files must also be specified in the CMakeLists.txt
```
   add_service_files(
      FILES
      GoToPosition.srv
   )

```
Moreover in order to generate the actual message code:
```
   generate_messages(
      DEPENDENCIES
      std_msgs  # Or other packages containing msgs
   )
   add_compile_options(-std=c++11)
```

In package.xml, the following is needed for message generation:
```xml
   <build_depend>message_generation</build_depend>
   <run_depend>message_runtime</run_depend>
```

The service is typically provided by a separate executable, and is advertised as follows:
```c++
   ros::ServiceServer service = n.advertiseService("/service_node/service_name", service_callback);
```
The service callback function interface is linked to the data type defined via the .srv file: 
`bool service_calliback(package_name::SrvFile::Request& req, package_name::SrvFile::Response& res)`
The service function can for example check validate the req input and publish it, the set the res output. 
If the callback is to publish some message, a Publisher must be defined, initialized in the main file and be accessible to the callback.

#Parameter Server#

Nodes to be launched can be added to a launch file as follows:
```.launch
  <node name="nodename" type="executable_name" pkg="pkgname" output="screen" />
```
If parameters are used, these may be specified directly in the launch file as:
```
```.launch

  <node name="nodename" type="executable_name" pkg="pkgname" output="screen">
    <rosparam>
      param0: 0
      param1: 1.57
    </rosparam>
  </node>
```

parameters can be modified manually after startup as
```
   source devel/setup.bash
   rosparam set /nodename/paramname VALUE
```

to use a service via command line 
```
   rosservice call /pkgname/nodename ": param0: VALUE
   param1: VALUE"
```


 


