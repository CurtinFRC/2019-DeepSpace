// Bindings to CTRE's Phoenix API, since they don't provide working simulation builds.
// See src/main/cpp, src/robot/cpp, and src/simulation/cpp for implementations.

#pragma once

#include <frc/SpeedController.h>
#include <ctre/phoenix/motorcontrol/can/TalonSRX.h>

#include <functional>

namespace curtinfrc {

  /**
   * Curtin FRC Wrapper around the CTRE Talon SRX.
   */
  class talon_srx : public frc::SpeedController {
   public:
    using configuration = ctre::phoenix::motorcontrol::can::TalonSRXConfiguration;
    using control_mode = ctre::phoenix::motorcontrol::ControlMode;

    /**
     * Create a new Talon SRX.
     * 
     * @param port The device ID of the Talon SRX on the CAN Bus.
     */
    talon_srx(int port);
    ~talon_srx();

    /**
     * Get the CAN Device ID of the Talon SRX.
     */
    int get_port();

    /**
     * Set or unset this Talon SRX as 'inverted' for all calls to .Set().
     */
    void SetInverted(bool invert) override;

    /**
     * Get whether this Talon SRX is inverted.
     */
    bool GetInverted() const override;

    /**
     * Stop the motor
     */
    void Disable() override;

    /**
     * Stop the motor
     */
    void StopMotor() override;

    void PIDWrite(double output) override;

    /**
     * Set the speed of the Talon SRX, in the range -1 Full Reverse, 0 Neutral and 1 Full Forward
     * 
     * @param speed The speed. -1 Full Reverse, 0 Neutral, 1 Full Forward 
     */
    void Set(double speed) override;
    
    /**
     * Set the value of the Talon SRX in a given control mode. 
     * 
     * @param mode The control mode of the Talon SRX. See @ref control_mode.
     * @param value The value to set. Units dependent on value of mode.
     */
    void Set(control_mode mode, double value);

    /**
     * Get the currently active control mode of the Talon SRX.
     * 
     * @returns The control mode of the Talon SRX. See @ref control_mode.
     */
    control_mode GetMode();

    /**
     * Get the current value of the Talon SRX.
     * 
     * @returns The value of the motor controller, dependent on the active control mode.
     */
    double Get() const override;

    /**
     * Get the current sensor position, in encoder ticks.
     * 
     * @return The current sensor position, in encoder ticks.
     */
    int GetSensorPosition();

    /**
     * Get the current sensor velocity, in encoder ticks per 100 millisecond.
     * 
     * @return The current sensor velocity, in encoder ticks per 100 millisecond.
     */
    int GetSensorVelocity();

    /**
     * Load a talon configuration.
     * 
     * @param configuration The talon configuration
     */
    void load_config(configuration &config);

    /**
     * Save (get) the current talon configuration
     * 
     * @returns The current talon configuration
     */
    configuration save_config();

    /**
     * Modify a talon configuration. This is the equivilent of calling @ref save_config(), changing
     * a value, followed by @ref load_config(configuration &).
     * 
     * @param func The configuration function. This is a function that takes in a configuration reference.
     */
    void modify_config(std::function<void(configuration &)> func);
    
    int _port;
    void *_handle;
    double _value;
  };

}