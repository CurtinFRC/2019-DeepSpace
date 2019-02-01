#include "Toggle.h"

using namespace curtinfrc;

Toggle::Toggle(ToggleEvent mode) {
  _mode = mode;
  _triggered = false;
  _lstate = mode == 2 ? false : !mode;
}

Toggle::Toggle(ToggleEvent mode, bool initState) {
  _mode = mode;
  _triggered = false;
  _lstate = initState;
}

bool Toggle::tick(bool val) {
  _triggered = _lstate != val && !val != _mode;
  _lstate = val;

  return _triggered;
}

bool Toggle::getTriggered() { return _triggered; }
