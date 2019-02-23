#include "CurtinControllers.h"


// CONROLLER
double curtinfrc::Controller::GetRawAxis(int axis) {
  if (GetEnabled()) return _cont->GetRawAxis(axis);
  else return 0;
}

bool curtinfrc::Controller::GetRawButton(int button) {
  if (GetEnabled()) return _cont->GetRawButton(button);
  else return false;
}

int curtinfrc::Controller::GetRawPOVAngle() {
  if (GetEnabled()) return _cont->GetPOV();
  else return -1;
}

int curtinfrc::Controller::GetRawSelectorValue(int id) {
  /* if (GetEnabled()) */ return GetRawSelector(id).Get();
  // else return -1;
}


void curtinfrc::Controller::UpdateSelectors() {
  if (GetEnabled()) {
    for (auto pair : _selectors) UpdateSelector(pair.first);
  }
}

int curtinfrc::Controller::UpdateSelector(int id) {
  if (GetEnabled()) {
    ButtonSelector &buttonSelector = _selectors.at(id);

    if (buttonSelector.incrementButtonToggle.Update(GetButton(buttonSelector.incrementButton))) IncrementSelector(1, id);
    if (buttonSelector.decrementButtonToggle.Update(GetButton(buttonSelector.decrementButton))) DecrementSelector(1, id);
  }

  return GetRawSelectorValue(id);
}

int curtinfrc::Controller::GetSelectorLength(int id) {
  return _selectors.at(id).selector.GetLength();
}


int curtinfrc::Controller::IncrementSelector(int amount, int id) {
  if (GetEnabled()) return GetRawSelector(id).ShiftRight(amount);
  else return GetRawSelectorValue(id);
}

int curtinfrc::Controller::DecrementSelector(int amount, int id) {
  if (GetEnabled()) return GetRawSelector(id).ShiftLeft(amount);
  else return GetRawSelectorValue(id);
}

int curtinfrc::Controller::SetSelector(int value, int id) {
  if (GetEnabled()) return GetRawSelector(id).Set(value);
  else return GetRawSelectorValue(id);
}

bool curtinfrc::Controller::GetSelectorValue(int button, int id) {
  if (GetEnabled()) return GetRawSelectorValue(id) == button;
  else return false;
}

bool curtinfrc::Controller::GetSelectorRise(int button, int id) {
  if (GetEnabled()) return _selectors.at(id).buttonToggles[button].first.Update(GetSelectorValue(button, id));
  else return false;
}

bool curtinfrc::Controller::GetSelectorFall(int button, int id) {
  if (GetEnabled()) return _selectors.at(id).buttonToggles[button].second.Update(GetSelectorValue(button, id));
  else return false;
}


bool curtinfrc::Controller::MakeSelector(int decrementButton, int incrementButton, int size, int position, int id) {
  if (decrementButton == incrementButton) return false;

  // Test buttons still exist
  auto dec = _buttonToggles.find(decrementButton), inc = _buttonToggles.find(incrementButton), end = _buttonToggles.end();

  if (dec == end || inc == end) {
    return false;
  } else {
    _buttonToggles.erase(dec);
    _buttonToggles.erase(inc);
    _nButtons = _buttonToggles.size();

    if (id < 0) id = _selectors.size();
    _selectors.insert({ id, ButtonSelector(decrementButton, incrementButton, Selector(size, position)) });
  }

  return true;
}


// CONTROLLERGROUP

int curtinfrc::ControllerGroup::GetPort(int cont) {
  return GetController(cont).GetPort();
}

bool curtinfrc::ControllerGroup::MakeSelector(curtinfrc::tControllerSelectorMapping config) {
  tControllerButton dec = std::get<0>(config), inc = std::get<1>(config);
  int cont = dec.first;

  if (cont != inc.first) return false;
  return GetController(cont).MakeSelector(dec.second, inc.second, std::get<2>(config), std::get<3>(config), std::get<4>(config));
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
  return GetController(cont).GetRawButton(button);
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


void curtinfrc::ControllerGroup::UpdateSelectors() {
  for (auto cont : m_conts) cont.get().UpdateSelectors();
}

int curtinfrc::ControllerGroup::GetSelectorLength(int cont, int id) {
  return GetController(cont).GetSelectorLength(id);
}


bool curtinfrc::ControllerGroup::GetRawSelectorValue(int cont, int button, int id) {
  return GetController(cont).GetSelectorValue(button, id);
}

bool curtinfrc::ControllerGroup::GetRawSelectorRise(int cont, int button, int id) {
  return GetController(cont).GetSelectorRise(button, id);
}

bool curtinfrc::ControllerGroup::GetRawSelectorFall(int cont, int button, int id) {
  return GetController(cont).GetSelectorFall(button, id);
}


bool curtinfrc::ControllerGroup::GetSelectorValue(tControllerSelectorButton tuple) {
  return GetRawSelectorValue(std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple));
}

bool curtinfrc::ControllerGroup::GetSelectorRise(tControllerSelectorButton tuple) {
  return GetRawSelectorRise(std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple));
}

bool curtinfrc::ControllerGroup::GetSelectorFall(tControllerSelectorButton tuple) {
  return GetRawSelectorFall(std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple));
}


bool curtinfrc::ControllerGroup::GetInput(tControllerButtonMap map) {
  bool val = false;

  for (auto pair : map) val |= GetButton(pair);

  return val;
}

bool curtinfrc::ControllerGroup::GetInputRise(tControllerButtonMap map) {
  bool val = false;

  for (auto pair : map) val |= GetButtonRise(pair);

  return val;
}

bool curtinfrc::ControllerGroup::GetInputFall(tControllerButtonMap map) {
  bool val = false;

  for (auto pair : map) val |= GetButtonFall(pair);

  return val;
}


bool curtinfrc::ControllerGroup::GetInput(tControllerSelectorButtonMap map) {
  bool val = false;

  for (auto tuple : map) val |= GetSelectorValue(tuple);

  return val;
}

bool curtinfrc::ControllerGroup::GetInputRise(tControllerSelectorButtonMap map) {
  bool val = false;

  for (auto tuple : map) val |= GetSelectorRise(tuple);

  return val;
}

bool curtinfrc::ControllerGroup::GetInputFall(tControllerSelectorButtonMap map) {
  bool val = false;

  for (auto tuple : map) val |= GetSelectorFall(tuple);

  return val;
}


curtinfrc::Controller &curtinfrc::ControllerGroup::GetController(int cont) {
  return m_conts[cont - 1];
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
