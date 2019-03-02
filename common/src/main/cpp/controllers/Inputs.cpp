#include "controllers/Inputs.h"

double curtinfrc::controllers::inputs::Axis::Get() {
  return _cont.GetAxis(_id);
}

double curtinfrc::controllers::inputs::Field::Get(curtinfrc::controllers::inputs::Field::FieldAxis axis) {
  if (IsSquare()) {
    double primary = GetAxis(axis).Get(), compare = GetAxis((FieldAxis)!(bool)axis).Get();

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
