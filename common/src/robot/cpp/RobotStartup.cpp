#include "startup.h"

void curtinfrc::RobotStartup::Start(std::function<int()> robotFunc) {
  robotFunc();
}