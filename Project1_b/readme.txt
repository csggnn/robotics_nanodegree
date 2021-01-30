In this project I build a robot model, a building mode, a world and a dummy plugin with gazebo. 

The robot and the building models are saved in separate directories, but their content is also fully included in the world file

There are ways to integrate objects and buildings into a world without actually including their full description, but these involve editing the gazebo model serch path or using a launchfile to integrate world, building and robot separately.

The plugin is saved in script and build using the CMakeFile. Once it is built in a given folder (e.g. in the `build` folder, by cmake ..; make) a `libwelcome.so` should be available.
The world file has been edited to run the plugin by adding `<plugin name='welcome' filename='libwelcome.so'/>`

The path to `libwelcome.so` must be included in the  GAZEBO_PLUGIN_PATH in order for the world file to find it. (e.g. by export GAZEBO_PLUGIN_PATH=${GAZEBO
_PLUGIN_PATH}:/home/workspace/Project1_b/build/)

