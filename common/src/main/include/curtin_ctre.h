// Bindings to CTRE's Phoenix API, since they don't provide working simulation builds.

#pragma once

#include <frc/SpeedController.h>
#include <ctre/phoenix/motorcontrol/can/TalonSRX.h>

#include <functional>

namespace curtinfrc {

  class talon_srx : public frc::SpeedController {
   public:
    using configuration = ctre::phoenix::motorcontrol::can::TalonSRXConfiguration;
    using control_mode = ctre::phoenix::motorcontrol::ControlMode;

    talon_srx(int port);
    ~talon_srx();

    int get_port();

    void SetInverted(bool invert) override;
    bool GetInverted() const override;

    void Disable() override;
    void StopMotor() override;

    void PIDWrite(double output) override;
    void Set(double speed) override;
    void Set(control_mode mode, double value);
    control_mode GetMode();
    double Get() const override;

    int GetSensorPosition();
    int GetSensorVelocity();

    void load_config(configuration &config);
    configuration save_config();
    void modify_config(std::function<void(configuration &)> func);
    
    int _port;
    void *_handle;
    double _value;
  };

}