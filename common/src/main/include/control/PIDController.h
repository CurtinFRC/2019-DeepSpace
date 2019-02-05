#pragma once

#include "NTUtil.h"

#include <string>
#include <memory>

namespace curtinfrc {
namespace control {

  class PIDGains {
   public:
    PIDGains(std::string name, double kP, double kI, double kD, double kF);

    // Needed since we need to reinit NT Bound Doubles with new address
    PIDGains(const PIDGains &other) : PIDGains(other._name, other._kP, other._kI, other._kD, other._kF) {}

    double kP() const;
    double kI() const;
    double kD() const;
    double kF() const;

   private:
    std::shared_ptr<nt::NetworkTable> _table;
    double _kP, _kI, _kD, _kF;
    std::string _name;

    wpi::SmallVector<NTBoundDouble, 4> _ntbounds;
  };

  class PIDController {
   public:
    PIDController(PIDGains gains);
    void SetSetpoint(double setpoint);
    double GetSetpoint();
    double Calculate(double processVariable, double dt);
   private:
    PIDGains _gains;
    double _derivative;
    double _integral;
    double _lastError;
    double _setpoint;
  };

}  // namespace control
}  // namespace curtinfrc