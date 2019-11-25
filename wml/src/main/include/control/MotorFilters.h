#pragma once

#include "Gearbox.h"
#include "control/Filter.h"

namespace curtinfrc {
namespace control {
  /**
   * The CurrentFFFilter is a feed-forward filter based on the Current given by a motor model.
   * 
   * This allows for pre-emptive current limiting without requiring a measurement of the current itself. 
   * In some cases, this may be too conservative, while in others it may not be conservative enough. 
   * 
   * The units given to the filter for the input and output are in the form of voltage.
   */
  class CurrentFFFilter : public Filter {
   public:
    /**
     * Constructor for a feed-forward current filter.
     * 
     * @param min The minimum amount of current to allow. Note that this must be negative, else the motor will never
     *            spin backwards. Measured in Amps.
     * @param max The maximum amount of current to allow. Note that this must be positive, else the motor will never
     *            spin forwards. Measured in Amps.
     * @param gearbox The gearbox that is being driven, used to obtain current speed (therefore, the encoder mustn't be null), 
     *                and also the motor model and reduction to use.
     */
    CurrentFFFilter(double min, double max, curtinfrc::Gearbox &gearbox);

    /**
     * Calculate the next output of the filter provided an input.
     * 
     * @param voltage The input voltage expected to be sent to the motor.
     * 
     * @return The filtered voltage, in volts.
     */
    double Get(double voltage) override;

    /**
     * Set the minimum and maximum limits for the current.
     * 
     * @param min The minimum amount of current to allow. Note that this must be negative, else the motor will never
     *            spin backwards. Measured in Amps.
     * @param max The maximum amount of current to allow. Note that this must be positive, else the motor will never
     *            spin forwards. Measured in Amps.
     */
    virtual void SetLimits(double min, double max);

   protected:
    curtinfrc::Gearbox _gearbox;

    double _min_curr, _max_curr;
  };

  /**
   * The AccelerationFFFilter is a feed-forward filter based on the Acceleration given by a motor model.
   * 
   * This allows for pre-emptive acceleration limiting without requiring a measurement of the acceleration itself. 
   * In some cases, this may be too conservative, while in others it may not be conservative enough. 
   * 
   * The units given to the filter for the input and output are in the form of voltage.
   */
  class AccelerationFFFilter : public CurrentFFFilter {
   public:
    /**
     * Constructor for a feed-forward acceleration filter.
     * 
     * @param min The minimum amount of acceleration to allow. Note that this must be negative, else the motor will never
     *            slow down. Measured in m/s/s.
     * @param max The maximum amount of acceleration to allow. Note that this must be positive, else the motor will never
     *            speed up. Measured in m/s/s.
     * @param gearbox The gearbox that is being driven, used to obtain current speed (therefore, the encoder mustn't be null), 
     *                and also the motor model and reduction to use.
     * @param radius  The radius of the wheels, in metres.
     * @param mass    The mass of the robot, in kilograms.
     */
    AccelerationFFFilter(double min, double max, curtinfrc::Gearbox &gearbox, double radius, double mass);

    /**
     * Set the minimum and maximum limits for the acceleration.
     * 
     * @param min The minimum amount of acceleration to allow. Note that this must be negative, else the motor will never
     *            slow down. Measured in m/s/s.
     * @param max The maximum amount of acceleration to allow. Note that this must be positive, else the motor will never
     *            speed up. Measured in m/s/s.
     */
    void SetLimits(double min, double max) override;

   private:
    double _radius, _mass;
  };
}  // namespace control
}  // namespace curtinfrc