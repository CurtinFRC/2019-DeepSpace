#include "controllers/SmartController.h"

using namespace curtinfrc::controllers;


bool SmartController::Exists(tAxis axis, bool value) {
  try {
    _axi.at(axis.id);
  } catch (std::out_of_range) {
    return !value;
  }

  return value;
}

bool SmartController::Exists(tButton button, bool value) {
  try {
    _buttons.at(button.id);
  } catch (std::out_of_range) {
    return !value;
  }

  return value;
}

bool SmartController::Exists(tPOV pov, bool value) {
  try {
    _POVs.at(pov.id);
  } catch (std::out_of_range) {
    return !value;
  }

  return value;
}

bool SmartController::Exists(std::vector<tAxis> axi, bool value) {
  bool val = value;
  for (auto axis : axi) val |= Exists(axis, value);
  return val;
}

bool SmartController::Exists(std::vector<tButton> buttons, bool value) {
  bool val = value;
  for (auto button : buttons) val |= Exists(button, value);
  return val;
}

bool SmartController::Exists(std::vector<tPOV> povs, bool value) {
  bool val = value;
  for (auto pov : povs) val |= Exists(pov, value);
  return val;
}


inputs::ContAxis *SmartController::GetObj(tAxis axis) {
  return Exists(axis) ? _axi.at(axis.id) : nullptr;
}

inputs::ContButton *SmartController::GetObj(tButton button) {
  return Exists(button) ? _buttons.at(button.id) : nullptr;
}

inputs::ContPOV *SmartController::GetObj(tPOV pov) {
  return Exists(pov) ? _POVs.at(pov.id) : nullptr;
}


void SmartController::Map(tAxis map_axis, tButton virt_button, double threshold) {
  if (Exists(map_axis)) {
    if (Exists(virt_button, false)) {
      _buttons[virt_button.id] = inputs::MakeAxisButton(GetObj(map_axis), threshold).at(0);

      // _axi.erase(_axi.find(map_axis.id));
    }
  }
}

void SmartController::Map(tAxis map_axis, std::vector<tButton> virt_buttons) {
  if (Exists(map_axis)) {
    if (Exists(virt_buttons, false)) {
      std::vector<inputs::AxisSelectorButton*> buttons = inputs::MakeAxisSelectorButtons(GetObj(map_axis), virt_buttons.size());
      for (int i = 0; i < buttons.size(); i++) {
        if (virt_buttons.at(i) != noButton) _buttons[virt_buttons.at(i).id] = buttons.at(i);
      }

      // _axi.erase(_axi.find(map_axis.id));
    }
  }
}

void SmartController::PairAxis(tAxis primary_axis, tAxis secondary_axis, bool squared) {
  if (Exists(primary_axis)) {
    if (Exists(secondary_axis)) {
      std::vector<inputs::FieldAxis*> axi = inputs::MakeFieldAxi(new inputs::Field(std::make_pair<inputs::ContAxis*, inputs::ContAxis*>(GetObj(primary_axis), GetObj(secondary_axis)), squared));
      _axi[primary_axis.id] = axi.at(0);
      _axi[secondary_axis.id] = axi.at(1);
    }
  }
}


void SmartController::Map(std::pair<tButton, tButton> map_buttons, std::vector<tButton> virt_buttons, bool wrap) {
  if (Exists(std::vector<tButton>({ map_buttons.first, map_buttons.second }))) {
    if (Exists(virt_buttons, false)) {
      std::vector<inputs::ButtonSelectorButton*> buttons = inputs::MakeButtonSelectorButtons({ GetObj(map_buttons.first), GetObj(map_buttons.second) }, virt_buttons.size(), wrap);
      for (int i = 0; i < buttons.size(); i++) {
        if (virt_buttons.at(i) != noButton) _buttons[virt_buttons.at(i).id] = buttons.at(i);
      }

      // _buttons.erase(_buttons.find(map_buttons.first.id));
      // _buttons.erase(_buttons.find(map_buttons.second.id));
    }
  }
}


void SmartController::Map(tPOV map_POV, std::vector<tButton> virt_buttons) {
  if (Exists(map_POV)) {
    if (Exists(virt_buttons, false)) {
      std::vector<inputs::POVButton*> buttons = inputs::MakePOVButtons(GetObj(map_POV));
      for (int i = 0; i < 8; i++) {
        if (virt_buttons.at(i) != noButton) _buttons[virt_buttons.at(i).id] = buttons.at(i);
      }

      // _POVs.erase(_POVs.find(map_POV.id));
    }
  }
}



// --------------------------------------------- INPUT GETTERS ---------------------------------------------

double SmartController::Get(tAxis axis) {
  if (Exists(axis, false)) return 0;
  return GetObj(axis)->Get();
}

bool SmartController::Get(tButton button, SmartController::ButtonMode mode) {
  if (Exists(button, false)) return false;

  if (mode == ButtonMode::RAW) return GetObj(button)->Get();
  else if (mode == ButtonMode::ONRISE) return GetObj(button)->GetOnRise();
  else if (mode == ButtonMode::ONFALL) return GetObj(button)->GetOnFall();
  else return false;
}

curtinfrc::controllers::Controller::POVPos SmartController::Get(tPOV pov) {
  if (Exists(pov, false)) return kNone;
  return GetObj(pov)->Get();
}



// --------------------------------------------- UPDATE FUNCS ---------------------------------------------

void SmartController::UpdateButtonSelectors() {
  for (auto pair : _buttons) UpdateButtonSelector(tButton(-1, pair.first));
}
