#pragma once

#include "CurtinCtre.h"

#include <map>

namespace simulation {

  class ctre {
   public:
    struct talon_data {
      int port;

      bool inverted;
      double value;
      curtinfrc::TalonSrx::ControlMode mode = curtinfrc::TalonSrx::ControlMode::Disabled;

      int sensor_pos;
      int sensor_vel;

      curtinfrc::TalonSrx::Configuration config;
    };

    struct victor_data {
      int port;

      bool inverted;
      double value;
      curtinfrc::VictorSpx::ControlMode mode = curtinfrc::VictorSpx::ControlMode::Disabled;

      curtinfrc::VictorSpx::Configuration config;
    };

    static std::map<int, simulation::ctre::talon_data> &all_talons();
    static std::map<int, simulation::ctre::victor_data> &all_victors();
  };

}