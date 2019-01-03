#include "startup.h"

void curtinfrc::robot_startup::start(std::function<int()> robot_func) {
  robot_func();
}