#include "NTProvider.h"

using namespace curtinfrc;
using namespace curtinfrc::sensors;


void NTProvider::Update() {
  for (auto binarySensor : _binarySensors)
    _table->GetEntry(binarySensor->GetName()).SetBoolean(binarySensor->Get());

  for (auto doubleSensor : _doubleSensors)
    _table->GetEntry(doubleSensor->GetName()).SetDouble(doubleSensor->GetScaled());
}


void NTProvider::Register(BinarySensor *binarySensor) {
  _binarySensors.push_back(binarySensor);
}

void NTProvider::Register(DoubleSensor *doubleSensor) {
  _doubleSensors.push_back(doubleSensor);
}
