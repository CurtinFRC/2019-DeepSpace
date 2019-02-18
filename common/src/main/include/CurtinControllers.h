#pragma once

#include <utility>

#include <frc/Joystick.h>
#include <frc/XboxController.h>

#include "Toggle.h"

namespace curtinfrc {
  typedef std::pair<int, int> tControllerButton;
  constexpr tControllerButton noButton{ -1, -1 };
  typedef std::pair<tControllerButton, tControllerButton> tControllerButtonMap;

  typedef std::pair<int, int> tControllerAxis;
  constexpr tControllerAxis noAxis{ -1, -1 };
  // typedef std::pair<tControllerAxis, tControllerAxis> tControllerAxisMap;

  class Controller : private frc::GenericHID {
   public:
    Controller(int port) : GenericHID(port) {};

    using frc::GenericHID::GetPort;
    virtual double GetRawAxis(int axis) { return frc::GenericHID::GetRawAxis(axis); };
    virtual bool GetRawButton(int button) { return frc::GenericHID::GetRawButton(button); };

    virtual bool GetButtonRise(int button) = 0;
    virtual bool GetButtonFall(int button) = 0;

    // only required for Joystick
    virtual double GetCircularisedAxisAgainst(int primaryAxis, int compareAxis) { return GetCircularisedAxis(primaryAxis); };
    virtual double GetCircularisedAxis(int axis) { return GetRawAxis(axis); };

   private:
    virtual double GetX(JoystickHand hand = kRightHand) const override { return 0; };
    virtual double GetY(JoystickHand hand = kRightHand) const override { return 0; };
  };

  class ControllerGroup {
   public:
    ControllerGroup(Controller &cont1, Controller &cont2) : _cont1(cont1), _cont2(cont2) {};
    enum ContNum { first = 1, second = 2 }; // TEMP

    double GetRawAxis(ContNum cont, int axis);
    double GetAxis(tControllerAxis contAxis);

    virtual double GetCircularisedAxisAgainst(ContNum cont, int primaryAxis, int compareAxis);
    virtual double GetCircularisedAxisAgainst(tControllerAxis primaryAxis, tControllerAxis compareAxis);
    virtual double GetCircularisedAxis(ContNum cont, int axis);
    virtual double GetCircularisedAxis(tControllerAxis axis);


    bool GetRawButton(ContNum cont, int button);
    bool GetRawButtonRise(ContNum cont, int button);
    bool GetRawButtonFall(ContNum cont, int button);

    bool GetButton(tControllerButton contButton);
    bool GetButtonRise(tControllerButton contButton);
    bool GetButtonFall(tControllerButton contButton);

    bool GetButton(tControllerButtonMap contMap);
    bool GetButtonRise(tControllerButtonMap contMap);
    bool GetButtonFall(tControllerButtonMap contMap);

    Controller &GetController(ContNum cont);

   private:
    Controller &_cont1, &_cont2;
  };

  class Joystick : public Controller, private frc::Joystick {
   public:
    Joystick(int port) : Controller(port), frc::Joystick(port) {
      for (int i = 0; i < 12; i++) {
        buttonRiseToggle.push_back(new Toggle(ToggleEvent::ONRISE));
        buttonFallToggle.push_back(new Toggle(ToggleEvent::ONFALL, false));
      }
    };

    using frc::Joystick::kDefaultXChannel;
    using frc::Joystick::kDefaultYChannel;
    using frc::Joystick::kDefaultZChannel;
    using frc::Joystick::kDefaultTwistChannel;
    using frc::Joystick::kDefaultThrottleChannel;

    using Controller::GetPort;
    // virtual double GetRawAxis(int axis);     // inherited
    // virtual bool GetRawButton(int button);   // inherited
    virtual bool GetButtonRise(int button) override;
    virtual bool GetButtonFall(int button) override;
    using frc::Joystick::GetPOV;

    virtual double GetAxis(int axis);
    virtual bool GetButton(int button) { return Controller::GetRawButton(button); };

    virtual double GetCircularisedAxisAgainst(int primaryAxis, int compareAxis) override;
    virtual double GetCircularisedAxis(int axis) override;

   private:
    wpi::SmallVector<Toggle *, 12>  buttonRiseToggle, buttonFallToggle;

    // using frc::Joystick::GetX;
    // using frc::Joystick::GetY;
  };

  class XboxController : public Controller, private frc::XboxController {
   public:
    XboxController(int port) : Controller(port), frc::XboxController(port) {
      for (int i = 0; i < 12; i++) {
        buttonRiseToggle.push_back(new Toggle(ToggleEvent::ONRISE));
        buttonFallToggle.push_back(new Toggle(ToggleEvent::ONFALL, false));
      }
    };

   private:
    wpi::SmallVector<Toggle *, 12>  buttonRiseToggle, buttonFallToggle;
  };
} // ns curtinfrc
