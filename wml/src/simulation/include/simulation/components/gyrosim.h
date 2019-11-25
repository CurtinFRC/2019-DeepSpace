#pragma once

#include "simulation/sensors/navx_sim.h"

#include <algorithm>
#include <iostream>

namespace simulation {
namespace components {
  class gyro_sim {
   public:
    virtual void add_angle(double angle) = 0;
    virtual void set_rate(double rate) = 0;
  };

  class navx_gyro : public gyro_sim {
   public:
    wml::sensors::NavXGyro *navx;

    navx_gyro(wml::sensors::NavXGyro *n) : navx(n) {}

    void add_angle(double angle) override {
      navx->GetNavX()._impl->angles[(int)navx->GetAxis()] += angle;
    }

    void set_rate(double rate) override {
      navx->GetNavX()._impl->angular_rates[(int)navx->GetAxis()] = rate;
    }
  };

  inline std::shared_ptr<gyro_sim> create_gyro(frc::Gyro *gyro) {
    if (gyro == nullptr)
      return nullptr;

    if (wml::sensors::NavXGyro *navx = dynamic_cast<wml::sensors::NavXGyro *>(gyro)) {
      return (std::shared_ptr<gyro_sim>) std::make_shared<navx_gyro>(navx);
    } else {
      std::cout << "Unknown Gyro Type" << std::endl; 
      return nullptr;
    }
  }
}
}