#pragma once

#include <utility>
#include <vector>

#include "controllers/Controller.h"


#include "Toggle.h"
#include "Selector.h"

namespace curtinfrc {
  namespace controllers {
    namespace inputs {
      // ----------------------------------------- CONTINPUT TYPES -----------------------------------------

      template <typename GetType>
      class ContInput {
       public:
        ContInput(Controller &cont, int id) : _cont(static_cast<Controller&>(cont)), _id(id) {};

        virtual GetType Get() = 0;

       protected:
        Controller &_cont;
        const int _id;
      };

      class ContAxis { public: virtual double Get() = 0; };
      class Axis : public ContInput<double>, public ContAxis {
       public:
        Axis(Controller &cont, int id) : ContInput(cont, id) {};

        virtual double Get() override;
      };

      class Field { // wrapper for two axi
       public:
        Field(std::pair<ContAxis*, ContAxis*> field, bool square = false) : _field(field), _square(square) {};
        enum FieldAxisType { primary = 0, secondary = 1 };

        double Get(FieldAxisType axis);
        virtual std::pair<double, double> Get() { return { Get(primary), Get(secondary) }; };

        double GetMag() { return sqrt(pow(Get(primary), 2) + pow(Get(secondary), 2)); };
        double GetAngle() { return atan2(Get(primary), Get(secondary)); };

        std::pair<ContAxis*, ContAxis*> GetAxi() { return _field; };
        ContAxis *GetAxis(FieldAxisType axis) {
          switch (axis) {
           case primary:
            return GetAxi().first;

           case secondary:
            return GetAxi().second;
          };

          Controller cont_(nullptr);
          return new Axis(cont_, 0);
        };
        ContAxis *GetPrimaryAxis() { return GetAxis(primary); };
        ContAxis *GetSecondaryAxis() { return GetAxis(secondary); };

        bool IsSquare() { return _square; };

       private:
        std::pair<ContAxis*, ContAxis*> _field;
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

      class Button : public ContInput<bool>, public ContButton {
       public:
        Button(Controller &cont, int id) : ContInput(cont, id) {};

        virtual bool Get() override;
      };

      class ContPOV { public: virtual Controller::POVPos Get() = 0; };
      class POV : public ContInput<Controller::POVPos>, public ContPOV {
       public:
        POV(Controller &cont, int id = 0) : ContInput(cont, id) {};

        virtual Controller::POVPos Get() override; // raw POV output
      };


      // ------------------------------------------- CAST TYPES -------------------------------------------

      class AxisButton : public ContButton {
       public:
        AxisButton(ContAxis *axis, double threshold) : _axis(axis), _threshold(threshold) {};

        virtual bool Get() override;

        void SetThreshold(double threshold) { _threshold = threshold; };
        double GetThreshold() { return _threshold; };
      
       private:
        ContAxis *_axis;
        double _threshold;
      };

      class AxisSelector {
       public:
        AxisSelector(ContAxis *axis, int divisions) : _axis(axis), _divisions(divisions) {};

        int Get();

       private:
        ContAxis *_axis;
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
        FieldAxis(Field *field, Field::FieldAxisType axis) : _field(field), _axis(axis) {};

        virtual double Get() override;

        operator ContAxis*() { return _field->GetAxis(_axis); }

       private:
        Field *_field;
        Field::FieldAxisType _axis;
      };
      // class FieldPOV : public POV {}; // Why. Just why would you be using this. Just use GetAngle() on the field.


      // class ButtonAxis : public ContAxis {
      //  public:
      //   ButtonAxis() {};
      // };

      class ButtonSelector {
       public:
        ButtonSelector(std::pair<ContButton*, ContButton*> buttons, int nButtons) : _buttons(buttons), _selector(nButtons) {};

        int Get();

       private:
        std::pair<ContButton*, ContButton*> _buttons;
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
      class POVButton : public ContButton {
       public:
        POVButton(ContPOV *pov, int id) : _pov(pov), _id(id) {};

        virtual bool Get() override;

       private:
        ContPOV *_pov;
        const int _id;
      };

      // class POVField : public Field { // uhhhhh, needs to be stored as FieldAxis, so...
      //  public:
      // };


      // ------------------------------------- 'CONSTRUCTOR' FUNCTIONS -------------------------------------
      std::vector<AxisButton*> MakeAxisButton(ContAxis *axis, double threshold = 0.3);  // returns 1
      std::vector<AxisSelectorButton*> MakeAxisSelectorButtons(ContAxis *axis, int n);  // returns n

      std::vector<FieldAxis*> MakeFieldAxi(Field *field);                               // returns 2

      std::vector<ButtonSelectorButton*> MakeButtonSelectorButtons(std::pair<ContButton*, ContButton*> buttons, int n); // returns n

      std::vector<POVButton*> MakePOVButtons(ContPOV *pov);                             // returns 8
    } // ns inputs
  } // ns controllers
} // ns curtinfrc
