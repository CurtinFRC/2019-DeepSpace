#pragma once

#include <frc/SpeedController.h>

#include "sensors/Encoder.h"

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

};
}  // namespace curtinfrc