//#include <iostream>

#include "CurtinControllers.h"

double curtinfrc::Joystick::GetAxis(AxisType axis) {
  switch (axis) {
   case kXAxis:
    return GetX();

   case kYAxis:
    return GetY();

   case kZAxis:
    return GetZ();

   case kTwistAxis:
    return GetTwist();
  
   case kThrottleAxis:
    return GetThrottle();
  }

  return 0;
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


bool curtinfrc::JoystickGroup::GetRawButton(JoyNum joy, int button) {
  bool val = false;

  switch (joy) {
   case first:
    val = _joy1.GetRawButton(button);
    break;

   case second:
    val = _joy2.GetRawButton(button);
    break;
  }

  return val;
}

bool curtinfrc::JoystickGroup::GetRawButton(tJoypair joyPair) {
  if (joyPair == nopair) return false;
  return GetRawButton((JoyNum)joyPair.first, joyPair.second);
}

bool curtinfrc::JoystickGroup::GetButton(tJoymap joyMap) {
  return GetRawButton(joyMap.first) || GetRawButton(joyMap.second);
}

curtinfrc::Joystick &curtinfrc::JoystickGroup::GetJoystick(curtinfrc::JoystickGroup::JoyNum joy) {
  switch (joy) {
   case first:
    return _joy1;

   case second:
    return _joy2;
  }
  
  return _joy1;
}
