#pragma once

#include "controllers/SmartController.h"

#include <frc/Joystick.h>
#include <frc/XboxController.h>

namespace curtinfrc {
  // class ControllerGroup {
  //  public:
  //   template <class... Controllers>
  //   explicit ControllerGroup(Controller& cont, Controllers&... conts) : m_conts{cont, conts...} {}

  //   ControllerGroup(ControllerGroup&&) = default;
  //   ControllerGroup& operator=(ControllerGroup&&) = default;

  //   int GetPort(int cont);

  //   double GetRawAxis(int cont, int axis = 1);
  //   double GetAxis(tControllerAxis contAxis);

  //   double GetCircularisedAxisAgainst(int cont, int primaryAxis = 1, int compareAxis = 0);
  //   double GetCircularisedAxisAgainst(tControllerAxis primaryAxis, tControllerAxis compareAxis);
  //   double GetCircularisedAxis(int cont, int axis = 1);
  //   double GetCircularisedAxis(tControllerAxis axis);


  //   bool GetRawButton(int cont, int button = 1);
  //   bool GetRawButtonRise(int cont, int button = 1);
  //   bool GetRawButtonFall(int cont, int button = 1);

  //   bool GetButton(tControllerButton pair);
  //   bool GetButtonRise(tControllerButton pair);
  //   bool GetButtonFall(tControllerButton pair);

  //   bool GetButton(tControllerButtonMap map);
  //   bool GetButtonRise(tControllerButtonMap map);
  //   bool GetButtonFall(tControllerButtonMap map);

  //   Controller &GetController(int cont);

  //  private:
  //   std::vector<std::reference_wrapper<Controller>> m_conts;
  // };

  // class Joystick : public Controller {
  //  public:
  //   Joystick(int port) : Controller(new frc::Joystick(port), 12) {};
    
  //   enum JoyAxis {
  //     kXAxis = 0,
  //     kYAxis = 1,
  //     kZAxis = 2, kTwistAxis = 2,
  //     kThrottleAxis = 3
  //   };

  //   virtual double GetCircularisedAxisAgainst(int primaryAxis, int compareAxis) override;
  //   virtual double GetCircularisedAxis(int axis) override;
  // };

  // class XboxController : public Controller {
  //  public:
  //   XboxController(int port) : Controller(new frc::XboxController(port), 11) {};

  //   enum XboxAxis {
  //     kLeftXAxis = 0,
  //     kLeftYAxis = 1,
  //     // Throttles?
  //     kRightXAxis = 4,
  //     kRightYAxis = 5,
  //   };

  //   enum XboxButton {
  //     kBumperLeft = 5,
  //     kBumperRight = 6,
  //     kStickLeft = 9,
  //     kStickRight = 10,
  //     kA = 1,
  //     kB = 2,
  //     kX = 3,
  //     kY = 4,
  //     kBack = 7,
  //     kStart = 8
  //   };
  // };
} // ns curtinfrc
