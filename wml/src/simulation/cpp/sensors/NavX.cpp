#include "simulation/sensors/navx_sim.h"

using namespace curtinfrc::sensors;

NavX::NavX(frc::SPI::Port spi, uint8_t update_hz) : _port{spi} {
  _impl = std::make_unique<NavX::Impl>();
}

NavX::NavX(frc::I2C::Port i2c, uint8_t update_hz) : _port{i2c} {
  _impl = std::make_unique<NavX::Impl>();
}

NavX::~NavX() {}

double NavXGyro::GetActualAngle() const {
  return _navx._impl->angles[(int)_axis];
}

double NavXGyro::GetRate() const {
  return _navx._impl->angular_rates[(int)_axis];
}