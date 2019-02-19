#include "CurtinControllers.h"

// #include <cmath> // ?


// CONTROLLERGROUP

int curtinfrc::ControllerGroup::GetPort(int cont) {
  return GetController(cont).GetPort();
}


double curtinfrc::ControllerGroup::GetRawAxis(int cont, int axis) {
  return GetController(cont).GetRawAxis(axis);
}

double curtinfrc::ControllerGroup::GetAxis(tControllerAxis contAxis) {
  return GetRawAxis(contAxis.first, contAxis.second);
}


double curtinfrc::ControllerGroup::GetCircularisedAxisAgainst(int cont, int primaryAxis, int compareAxis) {
  return GetController(cont).GetCircularisedAxisAgainst(primaryAxis, compareAxis);
}

double curtinfrc::ControllerGroup::GetCircularisedAxisAgainst(tControllerAxis primaryAxis, tControllerAxis compareAxis) {
  return GetCircularisedAxisAgainst(primaryAxis.first, primaryAxis.second, compareAxis.second);
}

double curtinfrc::ControllerGroup::GetCircularisedAxis(int cont, int axis) {
  return GetController(cont).GetCircularisedAxis(axis);
}

double curtinfrc::ControllerGroup::GetCircularisedAxis(tControllerAxis pair) {
  return GetCircularisedAxis(pair.first, pair.second);
}


bool curtinfrc::ControllerGroup::GetRawButton(int cont, int button) {
  return GetController(cont).GetButtonRise(button);
}

bool curtinfrc::ControllerGroup::GetRawButtonRise(int cont, int button) {
  return GetController(cont).GetButtonRise(button);
}

bool curtinfrc::ControllerGroup::GetRawButtonFall(int cont, int button) {
  return GetController(cont).GetButtonFall(button);
}


bool curtinfrc::ControllerGroup::GetButton(tControllerButton pair) {
  if (pair == noButton) return false;
  return GetRawButton(pair.first, pair.second);
}

bool curtinfrc::ControllerGroup::GetButtonRise(tControllerButton pair) {
  if (pair == noButton) return false;
  return GetRawButtonRise(pair.first, pair.second);
}

bool curtinfrc::ControllerGroup::GetButtonFall(tControllerButton pair) {
  if (pair == noButton) return false;
  return GetRawButtonFall(pair.first, pair.second);
}


bool curtinfrc::ControllerGroup::GetButton(tControllerButtonMap map) {
  bool val = false;

  for (auto pair : map) val |= GetButton(pair);

  return val;
}

bool curtinfrc::ControllerGroup::GetButtonRise(tControllerButtonMap map) {
  bool val = false;

  for (auto pair : map) val |= GetButtonRise(pair);

  return val;
}

bool curtinfrc::ControllerGroup::GetButtonFall(tControllerButtonMap map) {
  bool val = false;

  for (auto pair : map) val |= GetButtonFall(pair);

  return val;
}


curtinfrc::Controller &curtinfrc::ControllerGroup::GetController(int cont) {
  return m_conts[cont];
}


// JOYSTICK

double curtinfrc::Joystick::GetCircularisedAxisAgainst(int primaryAxis, int compareAxis) {
  double primary = GetAxis(primaryAxis), compare = GetAxis(compareAxis);

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
}

double curtinfrc::Joystick::GetCircularisedAxis(int axis) {
  if (axis == kXAxis) {
    return GetCircularisedAxisAgainst(kXAxis, kYAxis);
  } else if (axis == kYAxis) {
    return GetCircularisedAxisAgainst(kYAxis, kXAxis);
  }

  return GetAxis(axis);
}
