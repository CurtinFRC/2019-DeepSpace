#pragma once

#include <utility>

#include "controllers/Controller.h"
#include "Selector"

namespace curtinfrc {
  namespace controllers {
    namespace inputs {
      class ContInput {
       public:
        ContInput(Controller &cont, int id) : _cont(static_cast<Controller&>(cont)), _id(id) {};

       protected:
        Controller &_cont;
        const int _id;
      };

      class ContAxis { public: virtual double Get() = 0; };
      class Axis : public ContInput, public ContAxis {
       public:
        Axis(Controller &cont, int id) : ContInput(cont, id) {};

        virtual double Get() override;
      };

      class Field { // wrapper for two axi
       public:
        Field(std::pair<Axis&, Axis&> field, bool square = false) : _field(field), _square(square) {};
        enum FieldAxis { primary = 0, secondary = 1 };

        double Get(FieldAxis axis);
        virtual std::pair<double, double> Get() { return { Get(primary), Get(secondary) }; };

        std::pair<Axis&, Axis&> GetAxi() { return _field; };
        Axis &GetAxis(FieldAxis axis) {
          switch (axis) {
           case primary:
            return GetAxi().first;

           case secondary:
            return GetAxi().second;
          };

          return Axis(Controller(nullptr), 0); // help
        };
        Axis &GetPrimaryAxis() { return GetAxis(primary); };
        Axis &GetSecondaryAxis() { return GetAxis(secondary); };

        bool IsSquare() { return _square; };

       private:
        std::pair<Axis&, Axis&> _field;
        bool _square; // whether the field is a square or circle (xbox => circle, joy/raw => square)
      };

      class ContButton { public: virtual bool Get() = 0; };
      class Button : public ContInput, public ContButton {
       public:
        Button(Controller &cont, int id) : ContInput(cont, id) {};

        virtual bool Get() override;
      };

      class ContPOV { public: virtual int Get() = 0; };
      class POV : public ContInput, public ContPOV {
       public:
        POV(Controller &cont, int id = 0) : ContInput(cont, id) {};

        virtual int Get() override; // raw POV output
      };


      class AxisButton : public ContButton {
       public:
        AxisButton(Axis &axis, double threshold) : _axis(axis), _threshold(threshold) {};

        virtual bool Get();

        void SetThreshold(double threshold) { _threshold = threshold; };
        double GetThreshold() { return _threshold; };
      
       private:
        Axis &_axis;
        double _threshold;
      };

      class AxisSelector {
       public:
        AxisSelector(Axis &axis) : _axis(axis) {};

       private:
        Axis &_axis;
      };

      class AxisSelectorButton : public ContButton {
       public:
        AxisSelectorButton(AxisSelector &selector) : _selector(selector) {};

       private:
        AxisSelector &_selector;
      };

      class AxisPOV : public ContPOV {
       public:
      };

      class FieldAxis : public ContAxis { // Fields need to be stored as 2 axi, not actually a cast/conversion
       public:
      };
      // class FieldPOV : public POV {}; // Why. Just why would you be using this.

      class ButtonAxis : public ContAxis {
       public:
      };
      class ButtonSelector {
       public:
      };
      class ButtonSelectorButton : public ContButton {
       public:
      };
      class ButtonPOV : public ContPOV {
       public:
      };

      class POVButton : public ContButton {
       public:
      };
      class POVField : public Field { // uhhhhh, needs to be stored as FieldAxis, so...
       public:
      };
    } // ns inputs
  } // ns controllers
} // ns curtinfrc
