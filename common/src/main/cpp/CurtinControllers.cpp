//#include <iostream>

#include "CurtinControllers.h"


// CONTROLLERGROUP

double curtinfrc::ControllerGroup::GetRawAxis(ContNum cont, int axis) {
  return GetController(cont).GetRawAxis(axis);
}

double curtinfrc::ControllerGroup::GetAxis(tControllerAxis contAxis) {
  return GetRawAxis((ContNum)contAxis.first, contAxis.second);
}


double curtinfrc::ControllerGroup::GetCircularisedAxisAgainst(ContNum cont, int primaryAxis, int compareAxis) {
  return GetController(cont).GetCircularisedAxisAgainst(primaryAxis, compareAxis);
}

double curtinfrc::ControllerGroup::GetCircularisedAxisAgainst(tControllerAxis primaryAxis, tControllerAxis compareAxis) {
  return GetCircularisedAxisAgainst((ContNum)primaryAxis.first, primaryAxis.second, compareAxis.second);
}

double curtinfrc::ControllerGroup::GetCircularisedAxis(ContNum cont, int axis) {
  return GetController(cont).GetCircularisedAxis(axis);
}

double curtinfrc::ControllerGroup::GetCircularisedAxis(tControllerAxis axis) {
  return GetCircularisedAxis((ContNum)axis.first, axis.second);
}


bool curtinfrc::ControllerGroup::GetRawButton(ContNum cont, int button) {
  return GetController(cont).GetRawButton(button);
}

bool curtinfrc::ControllerGroup::GetRawButtonRise(ContNum cont, int button) {
  return GetController(cont).GetButtonRise(button);
}

bool curtinfrc::ControllerGroup::GetRawButtonFall(ContNum cont, int button) {
  return GetController(cont).GetButtonFall(button);
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


// JOYSTICK

double curtinfrc::Joystick::GetAxis(int axis) {
  switch (axis) {
   case kDefaultXChannel:
    return frc::Joystick::GetX();

   case kDefaultYChannel:
    return frc::Joystick::GetY();

   case kDefaultZChannel:
    return frc::Joystick::GetZ();

   case kTwistAxis:
    return frc::Joystick::GetTwist();
  
   case kThrottleAxis:
    return frc::Joystick::GetThrottle();
  }

  return 0;
}

bool curtinfrc::Joystick::GetButton(int button) {
  return Controller::GetRawButton(button);
}

bool curtinfrc::Joystick::GetButtonRise(int button) {
  return buttonRiseToggle[button - 1]->Update(GetButton(button));  
}

bool curtinfrc::Joystick::GetButtonFall(int button) {
  return buttonFallToggle[button - 1]->Update(GetButton(button));
}

double curtinfrc::Joystick::GetCircularisedAxisAgainst(int primaryAxis, int compareAxis) {
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

double curtinfrc::Joystick::GetCircularisedAxis(int axis) {
  if (axis == kDefaultXChannel) {
    return GetCircularisedAxisAgainst(kDefaultXChannel, kDefaultYChannel);
  } else if (axis == kDefaultYChannel) {
    return GetCircularisedAxisAgainst(kDefaultYChannel, kDefaultXChannel);
  }

  return GetAxis(axis);
}


// XBOXCONTROLLER

double curtinfrc::XboxController::GetAxis(int axis) {
  return Controller::GetRawAxis(axis);
}

bool curtinfrc::XboxController::GetButton(int button) {
  return Controller::GetRawButton(button);
}

bool curtinfrc::XboxController::GetButtonRise(int button) {
  return buttonRiseToggle[button - 1]->Update(GetButton(button));  
}

bool curtinfrc::XboxController::GetButtonFall(int button) {
  return buttonFallToggle[button - 1]->Update(GetButton(button));
}
