#include "startup.h"

void wml::RobotStartup::Start(std::function<int()> robotFunc) {
  robotFunc();
}