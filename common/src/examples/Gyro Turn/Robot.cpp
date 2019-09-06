#include "Robot.h"

#include <iostream>

using namespace frc;
using namespace curtinfrc;

double lastTimestamp;

void Robot::RobotInit() {
  lastTimestamp = Timer::GetFPGATimestamp();


  curtinfrc::DrivetrainConfig drivetrainConfig{
    leftGearbox, rightGearbox,                              // The left and right gearboxes
    &navx.Angular(curtinfrc::sensors::AngularAxis::YAW),    // The appropriate gyro is the navx's YAW axis

    // Values reflect Team 5333's robot from the 2019 FRC Season (DeepSpace) - all measurements in SI Units (m and kg)
    0.71,     // Distance between left and right wheels
    0.71,     // Distance from front to back wheels
    0.0762,   // Radius of the wheels
    50        // Weight of the robot
  };

  drivetrain = new Drivetrain(drivetrainConfig, { "Drivetrain Velocity", 1 });
  drivetrain->StartLoop(100);

  NTProvider::Register(drivetrain);  // Register the drivetrain to log to NetworkTables


  // Set the robot to turn according to the PID controller, running in an external high-accuracy loop
  drivetrain->SetExternalLoop([&](Drivetrain &, double dt) {
    double offset = pid.Calculate(drivetrain->GetConfig().gyro->GetAngle(), dt);
    return std::pair<double, double>(offset, -offset);
  });
}

void Robot::RobotPeriodic() {
  double dt = Timer::GetFPGATimestamp() - lastTimestamp;
  lastTimestamp = Timer::GetFPGATimestamp();
  // use the provided timers to calculate the time since the last cycle was run

  // TODO: register the drivetrain with the robot
  drivetrain->Update(dt);     // Update the drivetrain

  if (pid.IsDone()) {       // If the turn has finished...
    drivetrain->Set(0, 0);  // ... stop the drivetrain
  }
  
  NTProvider::Update();     // Updates values sent to NetworkTables
}
