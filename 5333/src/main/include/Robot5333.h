#pragma once

#include <frc/TimedRobot.h>
#include <frc/XboxController.h>
#include <frc/DoubleSolenoid.h>

#include "curtin_ctre.h"
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

  frc::XboxController *Xbox, *Xbox2;
  curtinfrc::talon_srx *LeftMotors[2], *RightMotors[2], *ConveyorMotors[2];
  frc::DoubleSolenoid *FlappyBoi;
};
