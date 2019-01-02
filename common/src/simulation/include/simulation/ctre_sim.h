#pragma once

#include "curtin_ctre.h"

#include <map>

namespace simulation {

  class ctre {
   public:
    struct talon_data {
      int port;

      bool inverted;
      double value;
      curtinfrc::talon_srx::control_mode mode = curtinfrc::talon_srx::control_mode::Disabled;

      int sensor_pos;
      int sensor_vel;

      curtinfrc::talon_srx::configuration config;
    };

    static std::map<int, simulation::ctre::talon_data> &all_talons();
  };

}