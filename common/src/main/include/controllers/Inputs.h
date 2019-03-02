#pragma once

#include <utility>

#include "controllers/Controller.h"


#include "Toggle.h"
#include "Selector.h"

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
        enum FieldAxisType { primary = 0, secondary = 1 };

        double Get(FieldAxisType axis);
        virtual std::pair<double, double> Get() { return { Get(primary), Get(secondary) }; };

        double GetMag() { return sqrt(pow(Get(primary), 2) + pow(Get(secondary), 2)); };
        double GetAngle() { return atan2(Get(primary), Get(secondary)); };

        std::pair<Axis&, Axis&> GetAxi() { return _field; };
        Axis &GetAxis(FieldAxisType axis) {
          switch (axis) {
           case primary:
            return GetAxi().first;

           case secondary:
            return GetAxi().second;
          };

          Controller cont_(nullptr);
          Axis axis_(cont_, 0);
          return axis_;
        };
        Axis &GetPrimaryAxis() { return GetAxis(primary); };
        Axis &GetSecondaryAxis() { return GetAxis(secondary); };

        bool IsSquare() { return _square; };

       private:
        std::pair<Axis&, Axis&> _field;
        bool _square; // whether the field is a square or circle (xbox => circle, joy/raw => square)
      };

      class ContButton {
       public:
        ContButton() : _toggles({ Toggle(ToggleEvent::ONRISE), Toggle(ToggleEvent::ONFALL, false) }) {};
        
        virtual bool Get() = 0;

        bool GetOnRise() { return _toggles.first.Update(Get()); };
        bool GetOnFall() { return _toggles.second.Update(Get()); };

       private:
        std::pair<Toggle, Toggle> _toggles;
      };

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


      // CAST TYPES

      class AxisButton : public ContButton {
       public:
        AxisButton(Axis &axis, double threshold) : _axis(axis), _threshold(threshold) {};

        virtual bool Get() override;

        void SetThreshold(double threshold) { _threshold = threshold; };
        double GetThreshold() { return _threshold; };
      
       private:
        Axis &_axis;
        double _threshold;
      };

      class AxisSelector {
       public:
        AxisSelector(Axis &axis, int divisions) : _axis(axis), _divisions(divisions) {};

        int Get();

       private:
        Axis &_axis;
        int _divisions;
      };

      class AxisSelectorButton : public ContButton {
       public:
        AxisSelectorButton(AxisSelector &selector, int id) : _selector(selector), _id(id) {};

        virtual bool Get() override;

       private:
        AxisSelector &_selector;
        const int _id;
      };


      class FieldAxis : public ContAxis { // Fields need to be stored as 2 axi, not actually a cast/conversion
       public:
        FieldAxis(Field &field, Field::FieldAxisType axis) : _field(field), _axis(axis) {};

        virtual double Get() override;

       private:
        Field &_field;
        Field::FieldAxisType _axis;
      };
      // class FieldPOV : public POV {}; // Why. Just why would you be using this. Just use GetAngle() on the field.


      // class ButtonAxis : public ContAxis {
      //  public:
      //   ButtonAxis() {};
      // };

      class ButtonSelector {
       public:
        ButtonSelector(Button &dec, Button &inc, int nButtons) : _buttons({ dec, inc }), _selector(nButtons) {};
        ButtonSelector(std::pair<Button&, Button&> buttons, int nButtons) : _buttons(buttons), _selector(nButtons) {};

        int Get();

       private:
        std::pair<Button&, Button&> _buttons;
        Selector _selector;
      };

      class ButtonSelectorButton : public ContButton {
       public:
        ButtonSelectorButton(ButtonSelector &selector, int id) : _selector(selector), _id(id) {};

        virtual bool Get() override;

       private:
        ButtonSelector &_selector;
        const int _id;
      };

      // class ButtonPOV : public ContPOV {
      //  public:
      //   ButtonPOV() {};
      // };

      // @TODO: POV casts;
      // class POVButton : public ContButton {
      //  public:
      // };

      // class POVField : public Field { // uhhhhh, needs to be stored as FieldAxis, so...
      //  public:
      // };
    } // ns inputs
  } // ns controllers
} // ns curtinfrc
