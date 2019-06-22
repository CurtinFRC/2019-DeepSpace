#include "Robot.h"

#include <iostream>

using namespace frc;
using namespace curtinfrc;

double lastTimestamp;

void Robot::RobotInit() {
  lastTimestamp = Timer::GetFPGATimestamp();

  NTProvider::Register(&pressureSensor);  // Register the pressure sensor as a sensor to be logged to NetworkTables
}

void Robot::RobotPeriodic() {
  double dt = Timer::GetFPGATimestamp() - lastTimestamp;
  lastTimestamp = Timer::GetFPGATimestamp();
  // use the provided timers to calculate the time since the last cycle was run

  
  if (pressureSensor.GetScaled() < 80) {                            // If the pressure drops below 80 psi...
    compressor.SetTarget(actuators::BinaryActuatorState::kForward); // ... turn the compressor on.
  }


  compressor.Update(dt);    // Calls whatever is required to make the compressor do what it's told to

  NTProvider::Update();     // Updates values sent to NetworkTables
}
