#include "Robot5333.h"

#include <math.h>
#include <iostream>

using namespace frc;
using namespace curtinfrc;

double lastTimestamp;

void Robot::RobotInit() {
  joy = new curtinfrc::Joystick(0);
  
  leftMotors[0] = new Spark(2);
  leftMotors[0]->SetInverted(false);
  left = new SensoredTransmission{ new SpeedControllerGroup(*leftMotors[0]), nullptr };

  rightMotors[0] = new Spark(3);
  rightMotors[0]->SetInverted(true);
  right = new SensoredTransmission{ new SpeedControllerGroup(*rightMotors[0]), nullptr };

  DrivetrainConfig drivetrainConfig{*left, *right};
  drivetrain = new Drivetrain(drivetrainConfig);


  liftMotors[0] = new Spark(5);
  liftGearbox = new Gearbox{ new SpeedControllerGroup(*liftMotors[0]), nullptr };

  ElevatorConfig elevatorConfig{ *liftGearbox, 1, nullptr, nullptr };
  beElevator = new Lift(elevatorConfig);
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() { lastTimestamp = Timer::GetFPGATimestamp(); }
void Robot::TeleopPeriodic() {
  double dt = -lastTimestamp + (lastTimestamp = Timer::GetFPGATimestamp());
  // Calc dt for update functions

  double joyY = -joy->GetCircularisedAxisAgainst(joy->kYAxis, joy->kZAxis);
  double joyZ = joy->GetCircularisedAxisAgainst(joy->kZAxis, joy->kYAxis);

  double leftSpeed = joyY + joyZ;
  double rightSpeed = joyY - joyZ;

  leftSpeed *= abs(leftSpeed);
  rightSpeed *= abs(rightSpeed);

  drivetrain->Set(leftSpeed, rightSpeed);


  double beElevatorSpeed = (joy->GetRawButton(8) - joy->GetRawButton(7)) * 0.8;

  beElevator->Set(beElevatorSpeed);

  // Class update events
  beElevator->Update(dt);
}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
