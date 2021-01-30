#include <gazebo/gazebo.hh>

namespace gazebo
{

class WelcomePlugin : public WorldPlugin {
  public:  
    WelcomePlugin() : WorldPlugin() {
        printf("Welcome to Gianni's world\n");
    }
    void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf) {}
};

GZ_REGISTER_WORLD_PLUGIN(WelcomePlugin)

} // namespace gazebo

