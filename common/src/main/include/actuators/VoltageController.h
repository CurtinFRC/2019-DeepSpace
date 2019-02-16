#pragma once

#include <frc/SpeedController.h>

namespace curtinfrc {
namespace actuators { 
  class VoltageController {
   public:
    virtual void SetVoltage(double voltage) = 0;
    virtual double GetVoltage() = 0;

    virtual void SetInverted(bool invert) = 0;
    virtual bool GetInverted() = 0;
  };

  class MotorVoltageController : public VoltageController {
   public:
    MotorVoltageController(frc::SpeedController *speedController);

    void SetVoltage(double voltage) override;
    double GetVoltage() override;

    void SetInverted(bool invert) override;
    bool GetInverted() override;

    double GetBusVoltage();

    template<typename T, typename ...Args>
    static MotorVoltageController Of(Args ...args) {
      T *t = new T(args...);  // Be warned, does not deallocate!
      return MotorVoltageController{t};
    }

   private:
    frc::SpeedController *_speedController;
  };
}
}  // namespace curtinfrc