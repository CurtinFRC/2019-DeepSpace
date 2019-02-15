#pragma once

#include <utility>

#include <frc/Joystick.h>
#include <frc/XboxController.h>

namespace curtinfrc {
  typedef std::pair<int, int> tJoypair;
  typedef std::pair<tJoypair, tJoypair> tJoymap;
  constexpr tJoypair nopair{ -1, -1 };

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

  class JoystickGroup {
   public:
    JoystickGroup(Joystick &joy1, Joystick &joy2) : _joy1(joy1), _joy2(joy2) {};
    enum JoyNum { first = 1, second = 2 }; // TEMP

    bool GetRawButton(JoyNum joy, int button);
    bool GetRawButton(tJoypair joyPair);
    bool GetButton(tJoymap joyMap);

    Joystick &GetJoystick(JoyNum joy);

   private:
    Joystick &_joy1, &_joy2;
  };
} // ns curtinfrc
