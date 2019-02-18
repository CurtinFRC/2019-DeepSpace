//#include <iostream>

#include "CurtinControllers.h"

double curtinfrc::Joystick::GetAxis(AxisType axis) {
  switch (axis) {
   case kXAxis:
    return frc::Joystick::GetX();

   case kYAxis:
    return frc::Joystick::GetY();

   case kZAxis:
    return frc::Joystick::GetZ();

   case kTwistAxis:
    return frc::Joystick::GetTwist();
  
   case kThrottleAxis:
    return frc::Joystick::GetThrottle();
  }

  return 0;
}

bool curtinfrc::Joystick::GetButtonRise(int button) {
  return buttonRiseToggle[button - 1]->Update(GetButton(button));  
}

bool curtinfrc::Joystick::GetButtonFall(int button) {
  return buttonFallToggle[button - 1]->Update(GetButton(button));
}

double curtinfrc::Joystick::GetCircularisedAxisAgainst(AxisType primaryAxis, AxisType compareAxis) {
  double primary = GetAxis(primaryAxis), compare = GetAxis(compareAxis);
  //std::cout << "prim: " << primary << std::endl;
  //std::cout << "comp: " << compare << std::endl;

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

  //std::cout << "    mag: " << mag << std::endl;
  //std::cout << "max_mag: " << max_mag << std::endl;

  // Divide current magnitude by maximum magnitude and calculate new axial magnitude
  mag /= max_mag;

  return mag * cos(theta);
}

double curtinfrc::Joystick::GetCircularisedAxis(AxisType axis) {
  if (axis == kXAxis) {
    return GetCircularisedAxisAgainst(kXAxis, kYAxis);
  } else if (axis == kYAxis) {
    return GetCircularisedAxisAgainst(kYAxis, kXAxis);
  }

  return GetAxis(axis);
}


bool curtinfrc::ControllerGroup::GetRawButton(ContNum cont, int button) {
  bool val = false;

  switch (cont) {
   case first:
    val = _cont1.GetRawButton(button);
    break;

   case second:
    val = _cont2.GetRawButton(button);
    break;
  }

  return val;
}

bool curtinfrc::ControllerGroup::GetRawButtonRise(ContNum cont, int button) {
  bool val = false;

  switch (cont) {
   case first:
    val = _cont1.GetButtonRise(button);
    break;

   case second:
    val = _cont2.GetButtonRise(button);
    break;
  }

  return val;
}

bool curtinfrc::ControllerGroup::GetRawButtonFall(ContNum cont, int button) {
  bool val = false;

  switch (cont) {
   case first:
    val = _cont1.GetButtonFall(button);
    break;

   case second:
    val = _cont2.GetButtonFall(button);
    break;
  }

  return val;
}


bool curtinfrc::ControllerGroup::GetButton(tControllerButton cont) {
  if (cont == noButton) return false;
  return GetRawButton((ContNum)cont.first, cont.second);
}

bool curtinfrc::ControllerGroup::GetButtonRise(tControllerButton cont) {
  if (cont == noButton) return false;
  return GetRawButtonRise((ContNum)cont.first, cont.second);
}

bool curtinfrc::ControllerGroup::GetButtonFall(tControllerButton cont) {
  if (cont == noButton) return false;
  return GetRawButtonFall((ContNum)cont.first, cont.second);
}


bool curtinfrc::ControllerGroup::GetButton(tControllerButtonMap cont) {
  return GetButton(cont.first) || GetButton(cont.second);
}

bool curtinfrc::ControllerGroup::GetButtonRise(tControllerButtonMap cont) {
  return GetButtonRise(cont.first) || GetButtonRise(cont.second);
}

bool curtinfrc::ControllerGroup::GetButtonFall(tControllerButtonMap cont) {
  return GetButtonFall(cont.first) || GetButtonFall(cont.second);
}


curtinfrc::Controller &curtinfrc::ControllerGroup::GetController(curtinfrc::ControllerGroup::ContNum cont) {
  switch (cont) {
   case first:
    return _cont1;

   case second:
    return _cont2;
  }
  
  return _cont1;
}
