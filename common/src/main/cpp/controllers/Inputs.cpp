#include "controllers/Inputs.h"

double curtinfrc::controllers::inputs::Axis::Get() {
  return _cont.GetAxis(_id);
}


double curtinfrc::controllers::inputs::Field::Get(curtinfrc::controllers::inputs::Field::FieldAxisType axis) {
  if (IsSquare()) {
    double primary = GetAxis(axis).Get(), compare = GetAxis(axis == FieldAxisType::primary ? FieldAxisType::secondary : FieldAxisType::primary).Get();

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
    return GetAxis(axis).Get();
  }
}


bool curtinfrc::controllers::inputs::Button::Get() {
  return _cont.GetButton(_id);
}


int curtinfrc::controllers::inputs::POV::Get() {
  return _cont.GetPOV(_id);
}



// AXIS CASTS

bool curtinfrc::controllers::inputs::AxisButton::Get() {
  return std::abs(_axis.Get()) > _threshold;
}


int curtinfrc::controllers::inputs::AxisSelector::Get() {
  double value = _axis.Get();
  if (value == 1) return _divisions - 1;

  double size = 2 / _divisions;
  if (_divisions % 2) if (std::abs(value) < size * 0.05) return -1; // deadzone - no buttons pressed (only for odd divisions)

  return (value + 1) / size;
}


bool curtinfrc::controllers::inputs::AxisSelectorButton::Get() {
  return _selector.Get() == _id;
}



// FIELD CASTS

double curtinfrc::controllers::inputs::FieldAxis::Get() {
  return _field.Get(_axis);
}



// BUTTON CASTS

int curtinfrc::controllers::inputs::ButtonSelector::Get() {
  if (_buttons.first.GetOnRise()) _selector.ShiftLeft();
  if (_buttons.second.GetOnRise()) _selector.ShiftRight();

  return _selector.Get();
}


bool curtinfrc::controllers::inputs::ButtonSelectorButton::Get() {
  return _selector.Get() == _id;
}
