#pragma once

#include <frc/TimedRobot.h>
#include <frc/Joystick.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/Spark.h>
#include <frc/DoubleSolenoid.h>

#include "CurtinCtre.h"
#include <frc/Timer.h>

class Robot : public frc::TimedRobot {
 public:
  void RobotInit() override;

  void AutonomousInit() override;
  void AutonomousPeriodic() override;

  void TeleopInit() override;
  void TeleopPeriodic() override;

  void TestInit() override;
  void TestPeriodic() override;

  frc::Joystick *joy;
  frc::Spark *leftMotors[1], *rightMotors[1];
  frc::SpeedControllerGroup *left, *right;
};
