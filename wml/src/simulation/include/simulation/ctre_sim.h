#pragma once

#include "WMLCtre.h"

#include <map>

namespace simulation {

  class ctre {
   public:
    struct talon_data {
      int port;

      bool inverted;
      double value;
      wml::TalonSrx::ControlMode mode = wml::TalonSrx::ControlMode::Disabled;

      int sensor_pos;
      int sensor_vel;

      wml::TalonSrx::Configuration config;
    };

    struct victor_data {
      int port;

      bool inverted;
      double value;
      wml::VictorSpx::ControlMode mode = wml::VictorSpx::ControlMode::Disabled;

      wml::VictorSpx::Configuration config;
    };

    static std::map<int, simulation::ctre::talon_data> &all_talons();
    static std::map<int, simulation::ctre::victor_data> &all_victors();
  };

}