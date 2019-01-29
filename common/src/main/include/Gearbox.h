#pragma once

#include <frc/SpeedController.h>

#include "sensors/Encoder.h"
#include "physics/DcMotor.h"

namespace curtinfrc {

/**
 * Struct for motor and encoder pairs.
 *
 * Combines the motor and encoder parts into one data structure,
 * so that both Spark + Encoder and Talon SRX are treated the same.
 */
struct Gearbox {

  /**
   * The SpeedController (Motor Controller). May not be null.
   */
  frc::SpeedController *transmission;

  /**
   * The Encoder. May be null, depending on the consumer of this structure.
   */
  sensors::Encoder *encoder;

  /**
   * The gearbox reduction, as a ratio
   */
  double reduction = 1;

  /**
   * The motor being used. By default, this is a dual CIM.
   */
  physics::DcMotor motor = physics::DcMotor::mCIM() * 2;
};
}  // namespace curtinfrc