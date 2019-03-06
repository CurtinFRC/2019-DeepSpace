#include "controllers/CurtinControllers.h"

using namespace curtinfrc::controllers::inputs;


// SmartControllerGroup

int curtinfrc::controllers::SmartControllerGroup::GetPort(int cont) {
  return GetController(cont).GetPort();
}


double curtinfrc::controllers::SmartControllerGroup::Get(tAxis axis) {
  if (axis == noAxis) return false;
  return GetController(axis.cont).Get(axis);
}

bool curtinfrc::controllers::SmartControllerGroup::Get(tButton button, SmartController::ButtonMode mode) {
  if (button == noButton) return false;
  return GetController(button.cont).Get(button, mode);
}

bool curtinfrc::controllers::SmartControllerGroup::Get(std::vector<tButton> buttons, SmartController::ButtonMode mode) {
  bool val = false;

  for (auto button : buttons) val |= Get(button, mode);

  return val;
}

int curtinfrc::controllers::SmartControllerGroup::Get(tPOV pov) {
  if (pov == noPOV) return false;
  return GetController(pov.cont).Get(pov);
}


curtinfrc::controllers::SmartController &curtinfrc::controllers::SmartControllerGroup::GetController(int cont) {
  return m_conts[cont - 1];
}
