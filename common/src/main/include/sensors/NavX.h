#pragma once

#include <frc/I2C.h>
#include <frc/SPI.h>
#include <frc/interfaces/Gyro.h>

#include <memory>

namespace curtinfrc {
namespace sensors {
  /**
   * Angular Axis for any sensor. This is a rotational axis, that is,
   * roll, pitch and yaw. Yaw is the default axis.
   */
  enum class AngularAxis { YAW, ROLL, PITCH };

  /**
   * Descriptor for a NavX port, since a NavX may be plugged into multiple
   * types of interface.
   */
  struct NavXPort {
    enum class Type { SPI, I2C };

    Type type;
    union {
      frc::SPI::Port spi;
      frc::I2C::Port i2c;
    };

    NavXPort(frc::SPI::Port spiPort) : type(Type::SPI), spi(spiPort) {}
    NavXPort(frc::I2C::Port i2cPort) : type(Type::I2C), i2c(i2cPort) {}
  };

  class NavXGyro;

  /**
   * Curtin FRC Wrapper for the KauaiLabs NavX-MXP and NavX-micro IMUs.
   *
   * This class allows the use of the NavX-MXP in both simulation and robot
   * environments.
   * 
   * Note that it does not support NavX over serial tty / uart, as the serial
   * interface does not have feature parity with SPI and I2C.
   */
  class NavX {
   public:
    class AxisGyro : public frc::Gyro {};

    /**
     * Create a NavX on the default port (SPI MXP) with the default update rate (100Hz)
     */
    NavX() : NavX(frc::SPI::Port::kMXP) {}

    /**
     * Create an SPI NavX, with an optional update rate.
     * 
     * @param spi       The SPI Port of the NavX.
     * @param update_hz The update rate of the NavX. Default 100Hz
     */
    NavX(frc::SPI::Port spi, uint8_t update_hz = 100);

    /**
     * Create an I2C NavX, with an optional update rate.
     * 
     * @param i2c       The I2C Port of the NavX.
     * @param update_hz The update rate of the NavX. Default 100Hz
     */
    NavX(frc::I2C::Port i2c, uint8_t update_hz = 100);

    ~NavX();

    /**
     * Get the Port this NavX is on.
     */
    NavXPort GetPort();

    /**
     * Get access to a specific rotational (angular) axis of the NavX.
     */
    NavXGyro Angular(AngularAxis axis);

   private:
    NavXPort _port;

    struct Impl;
    std::unique_ptr<Impl> _impl;

    friend class NavXGyro;
  };

  /**
   * A single, rotational (angular) axis of a NavX Gyro.
   * 
   * This class exists so that the NavX may be used as a generic Gyro, 
   * while still being able to access all 3 of its axis.
   */
  class NavXGyro : public frc::Gyro {
   public:
    NavXGyro(NavX &navx, AngularAxis axis) : _navx(navx), _axis(axis) {}

    void   Calibrate() override {}  // NavX has auto-cal
    void   Reset() override;
    double GetAngle() const override;
    double GetRate() const override;

    NavX &      GetNavX() const;
    AngularAxis GetAxis() const;

   protected:
    double GetActualAngle() const;

    NavX &      _navx;
    AngularAxis _axis;

    double _offset = 0;
  };
}  // namespace sensors
}  // namespace curtinfrc