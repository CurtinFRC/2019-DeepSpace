#pragma once

#include <functional>
#include <utility>
#include <vector>

#include <frc/Joystick.h>
#include <frc/XboxController.h>

#include "Toggle.h"

namespace curtinfrc {
  typedef std::pair<int, int> tControllerButton;
  constexpr tControllerButton noButton{ -1, -1 };
  typedef std::vector<tControllerButton> tControllerButtonMap;

  typedef std::pair<int, int> tControllerAxis;
  constexpr tControllerAxis noAxis{ -1, -1 };
  // typedef std::vector<tControllerAxis> tControllerAxisMap;

  class Controller {
   public:
    Controller(frc::GenericHID *cont, int nButtons = 12) : _cont(cont), _nButtons(nButtons), _buttonRiseToggle(nButtons, Toggle(ToggleEvent::ONRISE)), _buttonFallToggle(nButtons, Toggle(ToggleEvent::ONFALL, false)) {};

    /* virtual */ int GetButtonCount() const { return _nButtons; };

    int GetPort() { return _cont->GetPort(); };
    double GetRawAxis(int axis = 1) { return _cont->GetRawAxis(axis); };
    bool GetRawButton(int button = 1) { return _cont->GetRawButton(button); };

    virtual double GetAxis(int axis = 1) { return GetRawAxis(axis); };
    virtual bool GetButton(int button = 1) { return GetRawButton(button); };

    virtual bool GetButtonRise(int button = 1) { return _buttonRiseToggle[button - 1].Update(GetButton(button)); };
    virtual bool GetButtonFall(int button = 1) { return _buttonFallToggle[button - 1].Update(GetButton(button)); };

    // Joystick specific override
    virtual double GetCircularisedAxisAgainst(int primaryAxis = 1, int compareAxis = 0) { return GetCircularisedAxis(primaryAxis); };
    virtual double GetCircularisedAxis(int axis = 1) { return GetAxis(axis); };

   protected:
    frc::GenericHID *_cont;

   private:
    const int _nButtons;
    std::vector<Toggle> _buttonRiseToggle, _buttonFallToggle;
  };

  class ControllerGroup {
   public:
    template <class... Controllers>
    explicit ControllerGroup(Controller& cont, Controllers&... conts) : m_conts{cont, conts...} {};

    ControllerGroup(ControllerGroup&&) = default;
    ControllerGroup& operator=(ControllerGroup&&) = default;

    int GetPort(int cont);

    double GetRawAxis(int cont, int axis = 1);
    double GetAxis(tControllerAxis contAxis);

    double GetCircularisedAxisAgainst(int cont, int primaryAxis = 1, int compareAxis = 0);
    double GetCircularisedAxisAgainst(tControllerAxis primaryAxis, tControllerAxis compareAxis);
    double GetCircularisedAxis(int cont, int axis = 1);
    double GetCircularisedAxis(tControllerAxis axis);


    bool GetRawButton(int cont, int button = 1);
    bool GetRawButtonRise(int cont, int button = 1);
    bool GetRawButtonFall(int cont, int button = 1);

    bool GetButton(tControllerButton pair);
    bool GetButtonRise(tControllerButton pair);
    bool GetButtonFall(tControllerButton pair);

    bool GetButton(tControllerButtonMap map);
    bool GetButtonRise(tControllerButtonMap map);
    bool GetButtonFall(tControllerButtonMap map);

    Controller &GetController(int cont);

   private:
    std::vector<std::reference_wrapper<Controller>> m_conts;
  };

  class Joystick : public Controller {
   public:
    Joystick(int port) : Controller(new frc::Joystick(port), 12) {};
    
    enum JoyAxis {
      kXAxis = 0,
      kYAxis = 1,
      kZAxis = 2, kTwistAxis = 2,
      kThrottleAxis = 3
    };

    virtual double GetCircularisedAxisAgainst(int primaryAxis, int compareAxis) override;
    virtual double GetCircularisedAxis(int axis) override;
  };

  class XboxController : public Controller {
   public:
    XboxController(int port) : Controller(new frc::XboxController(port), 11) {};

    enum XboxAxis {
      kLeftXAxis = 0,
      kLeftYAxis = 1,
      // Throttles?
      kRightXAxis = 4,
      kRightYAxis = 5,
    };

    enum XboxButton {
      kBumperLeft = 5,
      kBumperRight = 6,
      kStickLeft = 9,
      kStickRight = 10,
      kA = 1,
      kB = 2,
      kX = 3,
      kY = 4,
      kBack = 7,
      kStart = 8
    };
  };
} // ns curtinfrc
