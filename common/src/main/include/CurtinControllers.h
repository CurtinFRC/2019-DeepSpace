#pragma once

#include <utility>

#include <frc/Joystick.h>
#include <frc/XboxController.h>

#include "Toggle.h"

namespace curtinfrc {
  typedef std::pair<int, int> tJoypair;
  typedef std::pair<tJoypair, tJoypair> tJoymap;
  constexpr tJoypair nopair{ -1, -1 };

  class Joystick : private frc::Joystick {
   public:
    Joystick(int port) : frc::Joystick(port) {
      for (int i = 0; i < 12; i++) {
        buttonRiseToggle[i] = new Toggle(ToggleEvent::ONRISE);
        buttonFallToggle[i] = new Toggle(ToggleEvent::ONFALL, false);
      }
    };

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
    bool GetButtonRise(int button);
    bool GetButtonFall(int button);
    using frc::Joystick::GetPOV;

    virtual bool GetButton(int button);
    virtual double GetAxis(AxisType axis);

    double GetCircularisedAxisAgainst(AxisType primaryAxis, AxisType compareAxis);
    double GetCircularisedAxis(AxisType axis);

   private:
    Toggle *buttonRiseToggle[12];
    Toggle *buttonFallToggle[12];
  };

  class JoystickGroup {
   public:
    JoystickGroup(Joystick &joy1, Joystick &joy2) : _joy1(joy1), _joy2(joy2) {};
    enum JoyNum { first = 1, second = 2 }; // TEMP

    bool GetRawButton(JoyNum joy, int button);
    bool GetRawButtonRise(JoyNum joy, int button);
    bool GetRawButtonFall(JoyNum joy, int button);    

    bool GetButton(tJoypair joyPair);
    bool GetButtonRise(tJoypair joyPair);
    bool GetButtonFall(tJoypair joyPair);

    bool GetButton(tJoymap joyMap);
    bool GetButtonRise(tJoymap joyMap);
    bool GetButtonFall(tJoymap joyMap);

    Joystick &GetJoystick(JoyNum joy);

   private:
    Joystick &_joy1, &_joy2;
  };
} // ns curtinfrc
