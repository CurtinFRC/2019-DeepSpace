#include <frc/RobotBase.h>

namespace curtinfrc {

/**
 * Class to be implemented in src/robot and src/simulation, as its behaviour will change depending
 * on if we're in sim or if we're on the robot. In sim, it starts up the simulator. On the robot,
 * it simply does nothing. 
 */
class robot_startup {
 public:
  static void start();
};

/**
 * Function to start up the FRC user program, including starting up simulation if required
 */
template <class robot_class>
int start_robot() {
  robot_startup::start();
  return frc::StartRobot<robot_class>();
}

#ifndef RUNNING_FRC_TESTS
#define CURTINFRC_ROBOT_MAIN(robot_clz) int main() { curtinfrc::start_robot<robot_clz>(); }
#else
#define CURTINFRC_ROBOT_MAIN(robot_clz)
#endif

}  // namespace curtinfrc