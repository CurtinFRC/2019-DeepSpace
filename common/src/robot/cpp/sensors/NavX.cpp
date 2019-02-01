#include "sensors/NavX.h"
#include "AHRS.h"

using namespace curtinfrc::sensors;

struct NavX::Impl : public AHRS {
  using AHRS::AHRS;
};

NavX::NavX(frc::SPI::Port spi, uint8_t update_hz) : _port{spi} {
  _impl = std::make_unique<NavX::Impl>(spi, update_hz);
}

NavX::NavX(frc::I2C::Port i2c, uint8_t update_hz) : _port{i2c} {
  _impl = std::make_unique<NavX::Impl>(i2c, update_hz);
}

NavX::~NavX() {}

double NavXGyro::GetActualAngle() const {
  switch (_axis) {
    case AngularAxis::PITCH:
      return _navx._impl->GetPitch();
    case AngularAxis::ROLL:
      return _navx._impl->GetRoll();
    case AngularAxis::YAW:
      return _navx._impl->GetYaw();
  }
}

double NavXGyro::GetRate() const {
  switch (_axis) {
    // The NavX doesn't provide a way to measure rate for non-yaw axis.
    case AngularAxis::PITCH:
    case AngularAxis::ROLL:
      return 0;
    case AngularAxis::YAW:
      return _navx._impl->GetRate();
  }
}