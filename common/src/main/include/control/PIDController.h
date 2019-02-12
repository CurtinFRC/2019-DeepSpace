#pragma once

#include "NTUtil.h"
#include "Filter.h"

#include <string>
#include <memory>

namespace curtinfrc {
namespace control {

  class PIDGains {
   public:
    PIDGains(std::string name, double kP = 0, double kI = 0, double kD = 0, double kF = 0);

    // Needed since we need to reinit NT Bound Doubles with new address
    PIDGains(const PIDGains &other) : PIDGains(other._name, other._kP, other._kI, other._kD, other._kF) {}

    double GetkP() const;
    double GetkI() const;
    double GetkD() const;
    double GetkF() const;

   private:
    std::shared_ptr<nt::NetworkTable> _table;
    double _kP, _kI, _kD, _kF;
    std::string _name;

    wpi::SmallVector<NTBoundDouble, 4> _ntbounds;
  };

  class PIDController {
   public:
    PIDController(PIDGains gains, double setpoint = 0);

    void SetSetpoint(double setpoint);
    double GetSetpoint();

    void SetIZone(double threshIZone);
    void SetWrap(double range);
    bool IsDone();
    void SetIsDoneThreshold(double threshAvg);

    double Calculate(double processVariable, double dt, double feedforward = 0.0);

   protected:
    void Reset();

   private:
    PIDGains _gains;
    LinearFilter _movingAverage;

    double Wrap(double val);

    double _setpoint;

    double _integral;
    double _derivative;
    double _lastError;
    double _avgError;
    int _iterations = 0;    // Used to check if we have sufficient size in avgError.

    double _threshIZone = -1;
    double _threshAvg = -1;
    double _wrapRange = -1;
    bool _threshAvgSet = false; // Used to check if _threshAvg has already been manually set.
  };

}  // namespace control
}  // namespace curtinfrc