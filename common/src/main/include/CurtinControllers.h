#pragma once

#include <functional>
#include <map>
#include <tuple>
#include <utility>
#include <vector>

#include <frc/Joystick.h>
#include <frc/XboxController.h>

#include "Selector.h"
#include "Toggle.h"

namespace curtinfrc {
  typedef std::pair<int, int> tControllerButton; // < nCont, nButton >
  constexpr tControllerButton noButton{ -1, -1 };
  typedef std::vector<tControllerButton> tControllerButtonMap;

  typedef std::pair<int, int> tControllerAxis; // < nCont, nAxis >
  constexpr tControllerAxis noAxis{ -1, -1 };
  // typedef std::vector<tControllerAxis> tControllerAxisMap;

  typedef std::tuple<int, int, int> tControllerSelectorButton; // < nCont, nButton, ID >
  constexpr tControllerSelectorButton noSelectorButton{ -1, -1, -1 };
  typedef std::vector<tControllerSelectorButton> tControllerSelectorButtonMap;

  typedef std::tuple<tControllerButton, tControllerButton, int, int, int> tControllerSelectorMapping; // < decButton, incButton, size, initPos, ID >


  class Controller {
   public:
    Controller(frc::GenericHID *cont, int nButtons = 12) : _cont(cont), _nButtons(nButtons), _enabled(true) {
      for (int i = 1; i <= nButtons; i++) {
        _buttonToggles[i] = { Toggle(ToggleEvent::ONRISE), Toggle(ToggleEvent::ONFALL, false) };
      }
    };

    enum POVPos {
      kNone = -1,
      kTop = 0,
      kTopRight = 45,
      kRight = 90,
      kBottomRight = 135,
      kBottom = 180,
      kBottomLeft = 215,
      kLeft = 270,
      kTopLeft = 315
    };

    /* virtual */ int GetButtonCount() const { return _nButtons; };

    int GetPort() { return _cont->GetPort(); };
    virtual void SetEnabled() { Enable(); };
    virtual void SetDisabled() { Disable(); };
    virtual bool GetEnabled() { return _enabled; };

    bool MakeSelector(int decrementButton, int incrementButton, int size, int position = 0, int id = 0); // Should throw errors in future as opposed to just returning false

    double GetRawAxis(int axis = 1);
    bool GetRawButton(int button = 1);
    int GetRawPOVAngle();
    int GetRawSelectorValue(int id = 0);

    virtual double GetAxis(int axis = 1) { return GetRawAxis(axis); };
    virtual bool GetButton(int button = 1) { return GetRawButton(button); };
    virtual double GetPOVAngle() { return GetRawPOVAngle(); };
    virtual POVPos GetPOV() { return (POVPos)GetRawPOVAngle(); };

    void UpdateSelectors();
    int UpdateSelector(int id = 0);
    int GetSelectorLength(int id = 0);

    int IncrementSelector(int amount = 1, int id = 0);
    int DecrementSelector(int amount = 1, int id = 0);
    int SetSelector(int value, int id = 0);
    bool GetSelectorValue(int button, int id = 0);
    bool GetSelectorRise(int button, int id = 0);
    bool GetSelectorFall(int button, int id = 0);

    virtual bool GetButtonRise(int button = 1) { return _buttonToggles[button].first.Update(GetButton(button)); };
    virtual bool GetButtonFall(int button = 1) { return _buttonToggles[button].second.Update(GetButton(button)); };

    // Joystick specific override
    virtual double GetCircularisedAxisAgainst(int primaryAxis = 1, int compareAxis = 0) { return GetCircularisedAxis(primaryAxis); };
    virtual double GetCircularisedAxis(int axis = 1) { return GetAxis(axis); };

   protected:
    frc::GenericHID *_cont;

    void Enable() { _enabled = true; };
    void Disable() { _enabled = false; };

   private:
    int _nButtons;
    std::map<int, std::pair<Toggle, Toggle>> _buttonToggles;

    struct ButtonSelector {
      int decrementButton, incrementButton;
      Toggle decrementButtonToggle, incrementButtonToggle;

      Selector selector;
      std::vector<std::pair<Toggle, Toggle>> buttonToggles;

      ButtonSelector(int decrementButtonIn, int incrementButtonIn, Selector selectorIn) : decrementButton(decrementButtonIn), incrementButton(incrementButtonIn), decrementButtonToggle(Toggle(ToggleEvent::ONRISE)), incrementButtonToggle(Toggle(ToggleEvent::ONRISE)), selector(selectorIn), buttonToggles(selectorIn.GetLength(), { Toggle(ToggleEvent::ONRISE), Toggle(ToggleEvent::ONFALL, false) }) {};
    };

    Selector &GetRawSelector(int id) { return _selectors.at(id).selector; };
    std::map<int, ButtonSelector> _selectors;

    bool _enabled;
  };

  class ControllerGroup {
   public:
    template <class... Controllers>
    explicit ControllerGroup(Controller& cont, Controllers&... conts) : m_conts{cont, conts...} {}

    ControllerGroup(ControllerGroup&&) = default;
    ControllerGroup& operator=(ControllerGroup&&) = default;

    int GetPort(int cont);
    bool MakeSelector(tControllerSelectorMapping config);

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


    void UpdateSelectors();
    int GetSelectorLength(int cont, int id = 0);

    bool GetRawSelectorValue(int cont, int button, int id = 0);
    bool GetRawSelectorRise(int cont, int button, int id = 0);
    bool GetRawSelectorFall(int cont, int button, int id = 0);

    bool GetSelectorValue(tControllerSelectorButton tuple);
    bool GetSelectorRise(tControllerSelectorButton tuple);
    bool GetSelectorFall(tControllerSelectorButton tuple);


    bool GetInput(tControllerButtonMap map);
    bool GetInputRise(tControllerButtonMap map);
    bool GetInputFall(tControllerButtonMap map);

    bool GetInput(tControllerSelectorButtonMap map);
    bool GetInputRise(tControllerSelectorButtonMap map);
    bool GetInputFall(tControllerSelectorButtonMap map);


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
