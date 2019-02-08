#pragma once

#include <frc/Joystick.h>
#include <frc/XboxController.h>

namespace curtinfrc {
  class Joystick : private frc::Joystick {
   public:
    using frc::Joystick::Joystick;

    using frc::Joystick::AxisType;
    using frc::Joystick::kXAxis;
    using frc::Joystick::kYAxis;
    using frc::Joystick::kZAxis;
    using frc::Joystick::kTwistAxis;
    using frc::Joystick::kThrottleAxis;
    
    using frc::Joystick::kDefaultXChannel;
    using frc::Joystick::kDefaultYChannel;
    using frc::Joystick::kDefaultZChannel;
    using frc::Joystick::kDefaultTwistChannel;
    using frc::Joystick::kDefaultThrottleChannel;

    using frc::Joystick::GetPort;
    using frc::Joystick::GetRawButton;
    using frc::Joystick::GetPOV;

    virtual double GetAxis(AxisType axis);

    double GetCircularisedAxisAgainst(AxisType primaryAxis, AxisType compareAxis);
    double GetCircularisedAxis(AxisType axis);
  };
} // ns curtinfrc
