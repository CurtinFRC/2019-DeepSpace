#pragma once

#include <frc/Timer.h>
#include <frc/TimedRobot.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/Spark.h>
#include <frc/DoubleSolenoid.h>

#include "CurtinCtre.h"
#include "SensoredTransmission.h"
#include "Drivetrain.h"
#include "CurtinControllers.h"

class Robot : public frc::TimedRobot {
 public:
  void RobotInit() override;

  void AutonomousInit() override;
  void AutonomousPeriodic() override;

  void TeleopInit() override;
  void TeleopPeriodic() override;

  void TestInit() override;
  void TestPeriodic() override;

  curtinfrc::Joystick *joy;
  frc::Spark *leftMotors[1], *rightMotors[1];
  curtinfrc::SensoredTransmission *left, *right;
  curtinfrc::Drivetrain *drivetrain;
};
