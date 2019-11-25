#include "controllers/Controllers.h"

using namespace wml::controllers::inputs;


// SmartControllerGroup

int wml::controllers::SmartControllerGroup::GetPort(int cont) {
  return GetController(cont).GetPort();
}


double wml::controllers::SmartControllerGroup::Get(tAxis axis) {
  if (axis == noAxis) return false;
  return GetController(axis.cont).Get(axis);
}

bool wml::controllers::SmartControllerGroup::Get(tButton button, SmartController::ButtonMode mode) {
  if (button == noButton) return false;
  return GetController(button.cont).Get(button, mode);
}

bool wml::controllers::SmartControllerGroup::Get(std::vector<tButton> buttons, SmartController::ButtonMode mode) {
  bool val = false;

  for (auto button : buttons) val |= Get(button, mode);

  return val;
}

int wml::controllers::SmartControllerGroup::Get(tPOV pov) {
  if (pov == noPOV) return false;
  return GetController(pov.cont).Get(pov);
}


wml::controllers::SmartController &wml::controllers::SmartControllerGroup::GetController(int cont) {
  return m_conts[cont - 1];
}
