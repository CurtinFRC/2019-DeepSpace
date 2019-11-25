#include "Toggle.h"

using namespace curtinfrc;

Toggle::Toggle(ToggleEvent mode) {
  _mode = mode;
  _triggered = false;
  _lstate = mode == ONCHANGE ? false : !mode; // requires ToggleEvent to have 0: ONFALL, 1: ONRISE
}

Toggle::Toggle(ToggleEvent mode, bool initState) {
  _mode = mode;
  _triggered = false;
  _lstate = initState;
}

bool Toggle::Update(bool val) {
  _triggered = _lstate != val && !val != _mode;
  _lstate = val;

  return _triggered;
}

bool Toggle::GetTriggered() { return _triggered; }
