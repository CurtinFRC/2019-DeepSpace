#pragma once

#include "sensors/Encoder.h"
#include "physics/DcMotor.h"
#include "actuators/VoltageController.h"

namespace curtinfrc {

/**
 * Struct for motor and encoder pairs.
 *
 * Combines the motor and encoder parts into one data structure,
 * so that both Spark + Encoder and Talon SRX are treated the same.
 */
struct Gearbox {

  /**
   * The VoltageController (Motor Controller). May not be null.
   */
  actuators::VoltageController *transmission;

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