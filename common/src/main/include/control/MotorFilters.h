#pragma once

#include "Gearbox.h"
#include "control/Filter.h"

namespace curtinfrc {
namespace control {
  class AccelerationFilter : public Filter {
   public:
    AccelerationFilter(double min, double max, curtinfrc::Gearbox &gearbox, double radius, double mass); 

    double Get(double voltage) override;

   private:
    curtinfrc::Gearbox _gearbox;
    double _min_torque, _max_torque;
    double _radius, _mass;
  };
}  // namespace control
}  // namespace curtinfrc