#include "sensors/NavX.h"

using namespace curtinfrc::sensors;

NavXGyro NavX::Angular(AngularAxis axis) {
  return NavXGyro{*this, axis};
}

NavXPort NavX::GetPort() {
  return _port;
}

void NavXGyro::Reset() {
  _offset = GetActualAngle();
}

double NavXGyro::GetAngle() const {
  return GetActualAngle() - _offset;
}

NavX &NavXGyro::GetNavX() const {
  return _navx;
}

AngularAxis NavXGyro::GetAxis() const {
  return _axis;
}