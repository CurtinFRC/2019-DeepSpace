#include "controllers/Inputs.h"

using namespace curtinfrc::controllers::inputs;


double Axis::Get() {
  return _cont.GetAxis(_id);
}


double Field::Get(Field::FieldAxisType axis) {
  if (IsSquare()) {
    double primary = GetAxis(axis)->Get(), compare = GetAxis(axis == FieldAxisType::primary ? FieldAxisType::secondary : FieldAxisType::primary)->Get();

    // Break into polar components (with angle as a bearing)
    double theta = atan2(compare, primary), mag = sqrt(pow(primary, 2) + pow(compare, 2));

    // Calculate maximum magnitude for given angle
    double max_mag;


    if (fmod(theta * 57.2957795131 + 225, 180) < 90) { // Offset anticlockwise by 45 degrees, then break up into quadrants
      max_mag = 1 / cos(theta);
    } else {
      max_mag = 1 / sin(theta);
    }

    max_mag = fabs(max_mag);


    // Divide current magnitude by maximum magnitude and calculate new axial magnitude
    mag /= max_mag;

    return mag * cos(theta);
  } else {
    return GetAxis(axis)->Get();
  }
}


bool Button::Get() {
  return _cont.GetButton(_id);
}


curtinfrc::controllers::Controller::POVPos POV::Get() {
  return _cont.GetPOV(_id);
}


// ---------------------------------------------- CAST TYPES ----------------------------------------------


// AXIS CASTS

bool AxisButton::Get() {
  return std::abs(_axis->Get()) > _threshold;
}


int AxisSelector::Get() {
  double value = _axis->Get();
  if (value == 1) return _divisions - 1;

  double size = 2 / _divisions;
  if (_divisions % 2) if (std::abs(value) < size * 0.05) return -1; // deadzone - no buttons pressed (only for odd divisions)

  return (value + 1) / size;
}


bool AxisSelectorButton::Get() {
  return _selector.Get() == _id;
}


// FIELD CASTS

double FieldAxis::Get() {
  return _field->Get(_axis);
}


// BUTTON CASTS

int ButtonSelector::Get() {
  if (_buttons.first->GetOnRise()) _selector.ShiftLeft();
  if (_buttons.second->GetOnRise()) _selector.ShiftRight();

  return _selector.Get();
}


bool ButtonSelectorButton::Get() {
  return _selector.Get() == _id;
}


// POV CASTS

bool POVButton::Get() {
  return _pov->Get() == _id;
}



// --------------------------------------- 'CONSTRUCTOR' FUNCTIONS ---------------------------------------


std::vector<AxisButton*> curtinfrc::controllers::inputs::MakeAxisButton(ContAxis *axis, double threshold) {
  return { new AxisButton(axis, threshold) };
}

std::vector<AxisSelectorButton*> curtinfrc::controllers::inputs::MakeAxisSelectorButtons(ContAxis *axis, int n) {
  std::vector<AxisSelectorButton*> buttons;

  AxisSelector selector(axis, n);
  for (int i = 0; i < n; i++) buttons.push_back(new AxisSelectorButton(selector, i));

  return buttons;
}


std::vector<FieldAxis*> curtinfrc::controllers::inputs::MakeFieldAxi(Field *field) {
  return { new FieldAxis(field, Field::FieldAxisType::primary), new FieldAxis(field, Field::FieldAxisType::secondary) };
}


std::vector<ButtonSelectorButton*> curtinfrc::controllers::inputs::MakeButtonSelectorButtons(std::pair<ContButton*, ContButton*> buttonPair, int n) {
  std::vector<ButtonSelectorButton*> buttons;

  ButtonSelector selector(buttonPair, n);
  for (int i = 0; i < n; i++) buttons.push_back(new ButtonSelectorButton(selector, i));

  return buttons;
}


std::vector<POVButton*> curtinfrc::controllers::inputs::MakePOVButtons(ContPOV *pov) {
  std::vector<POVButton*> buttons;

  for (int i = 0; i < 8; i++) buttons.push_back(new POVButton(pov, i + 1));

  return buttons;
}
