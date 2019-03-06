#pragma once

#include <utility>
#include <vector>

#include "controllers/SmartController.h"

#include <frc/Joystick.h>
#include <frc/XboxController.h>

namespace curtinfrc {
  namespace controllers {
    class SmartControllerGroup {
     public:
      template <class... SmartControllers>
      explicit SmartControllerGroup(SmartController& cont, SmartControllers&... conts) : m_conts{cont, conts...} {}

      SmartControllerGroup(SmartControllerGroup&&) = default;
      SmartControllerGroup& operator=(SmartControllerGroup&&) = default;

      int GetPort(int cont);

      double Get(tAxis axis);
      bool Get(tButton button, SmartController::ButtonMode mode = SmartController::RAW);
      bool Get(std::vector<tButton> buttons, SmartController::ButtonMode mode = SmartController::RAW);
      int Get(tPOV pov);

      SmartController &GetController(int cont);

     private:
      std::vector<std::reference_wrapper<SmartController>> m_conts;
    };

    class Joystick : public SmartController {
     public:
      Joystick(int port, std::pair<tAxis, tAxis> pair = { { -1, kYAxis }, { -1, kXAxis } }) : SmartController(new frc::Joystick(port), { 4, 12, 1 }) {
        if (pair.first != noAxis && pair.second != noAxis) PairAxis(pair.first, pair.second, true);
      };
      
      enum AxisMap {
        kXAxis = 0,
        kYAxis = 1,
        kZAxis = 2, kTwistAxis = 2,
        kThrottleAxis = 3
      };
    };

    class XboxController : public SmartController {
     public:
      XboxController(int port) : SmartController(new frc::XboxController(port), { 6, 11, 1 }) {};

      enum AxisMap {
        kLeftXAxis = 0,
        kLeftYAxis = 1,
        // Throttles?
        kRightXAxis = 4,
        kRightYAxis = 5,
      };

      enum ButtonMap {
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
  } // ns controllers
} // ns curtinfrc
