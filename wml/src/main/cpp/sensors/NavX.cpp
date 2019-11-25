#include "sensors/NavX.h"

using namespace wml::sensors;

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
  double angle = GetActualAngle() - _offset;
  if (_axis != AngularAxis::YAW) {
    angle = std::fmod(angle, 360);
    angle = std::fmod(angle + 360, 360);
    if (angle > 180)
      angle -= 360;
  }
  return angle;
}

NavX &NavXGyro::GetNavX() const {
  return _navx;
}

AngularAxis NavXGyro::GetAxis() const {
  return _axis;
}