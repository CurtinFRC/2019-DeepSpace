#pragma once

#include <frc/GenericHID.h>

namespace curtinfrc {
  namespace controllers {
    class Controller {
     public:
      Controller(frc::GenericHID *cont) : _cont(cont) {};

      enum ButtonMode {
        RAW = 0,
        ONRISE = 1,
        ONFALL = -1
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

      int GetPort() { return _cont->GetPort(); };
      double GetRawAxis(int axis = 1) { return _cont->GetRawAxis(axis); };
      bool GetRawButton(int button = 1) { return _cont->GetRawButton(button); };
      int GetRawPOVAngle(int pov = 0) { return _cont->GetPOV(pov); };

      virtual double GetAxis(int axis = 1) { return GetRawAxis(axis); };
      virtual bool GetButton(int button = 1) { return GetRawButton(button); };
      virtual int GetPOVAngle(int pov = 0) { return GetRawPOVAngle(pov); };
      virtual POVPos GetPOV(int pov = 0) { return (POVPos)GetRawPOVAngle(pov); };

     protected:
      frc::GenericHID *_cont;
    };

    class GenericHID : public frc::GenericHID {
     public:
      using frc::GenericHID::GenericHID;

      virtual double GetX(JoystickHand hand = kRightHand) const override { return GetRawAxis(1); };
      virtual double GetY(JoystickHand hand = kRightHand) const override { return GetRawAxis(0); };
    };
  } // ns controllers
} // ns curtinfrc
