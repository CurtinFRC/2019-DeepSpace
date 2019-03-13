#include "NTProvider.h"

using namespace curtinfrc;
using namespace curtinfrc::sensors;
using namespace curtinfrc::devices;


void NTProvider::Update() {
  for (auto binarySensor : _binarySensors)
    _table->GetEntry(binarySensor->GetName()).SetBoolean(binarySensor->Get());

  for (auto doubleSensor : _doubleSensors)
    _table->GetEntry(doubleSensor->GetName()).SetDouble(doubleSensor->GetScaled());


  for (auto stateDevice : _stateDevices)
    _table->GetEntry(stateDevice->GetName()).SetString(stateDevice->GetStateString());
}


void NTProvider::Register(BinarySensor *binarySensor) {
  _binarySensors.push_back(binarySensor);
}

void NTProvider::Register(DoubleSensor *doubleSensor) {
  _doubleSensors.push_back(doubleSensor);
}


void NTProvider::Register(StateDeviceBase *stateDevice) {
  _stateDevices.push_back(stateDevice);
}
