#include "Robot5333.h"

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


  if ((int)(lastTimestamp) % 2) {
    // Set the solenoid to extend on 'even' seconds...
    solenoid.SetTarget(actuators::BinaryActuatorState::kForward);
  } else {
    // ... and to retract on 'odd' seconds...
    solenoid.SetTarget(actuators::BinaryActuatorState::kForward);
  }

  
  if (pressureSensor.GetScaled() < 80) {                            // If the pressure drops below 80 psi...
    compressor.SetTarget(actuators::BinaryActuatorState::kForward); // ... turn the compressor on.
  }


  // The update methods need to be called every cycle for the actuators to actualy do anything
  compressor.Update(dt);
  solenoid.Update(dt);

  // Stop the solenoid if it's finished
  if (solenoid.IsDone()) solenoid.Stop();

  NTProvider::Update();     // Updates values sent to NetworkTables
}
