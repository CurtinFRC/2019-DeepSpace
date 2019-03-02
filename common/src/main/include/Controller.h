#pragma once

#include <functional>
#include <utility>
#include <vector>

#include <frc/GenericHID.h>

#include "Toggle.h"

namespace curtinfrc {
  class Controller {
   public:
    Controller(frc::GenericHID *cont) : _cont(cont) {};

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
    virtual double GetPOVAngle() { return GetRawPOVAngle(); };
    virtual POVPos GetPOV() { return (POVPos)GetRawPOVAngle(); };

   protected:
    frc::GenericHID *_cont;
  };

  namespace controllers {
    namespace inputs {
      class BaseInput {};

      class Axis : public BaseInput {};
      class Field : public BaseInput {
       public:
        Field(std::pair<*Axis, *Axis> field, bool square = false) : _field(field), _square(square) {};

       private:
        std::pair<*Axis, *Axis> _field;
        bool _square; // whether the field is a square or circle (xbox => circle, joy => square)
      }; // wrapper for two Axi
      class Button : public BaseInput {};
      class POV : public BaseInput {};


      class AxisButton : public Button {};
      class AxisSelector : public Button {};
      class AxisPOV : public POV {};

      class ButtonAxis : public Axis {};
      class ButtonSelector : public Button {};
      class ButtonPOV : public POV {};

      class POVButton : public Button {};
      class POVField : public Field {};
    } // ns inputs

    class SmartController : public Controller {
     public:
      SmartController() : Controller() {};
    };
  } // ns controllers
} // ns curtinfrc
