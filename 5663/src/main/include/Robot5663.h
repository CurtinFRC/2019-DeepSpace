#pragma once

#include <frc/TimedRobot.h>
#include <frc/XboxController.h>
#include <frc/Spark.h>
#include <frc/DoubleSolenoid.h>
#include <frc/Encoder.h>

#include "CurtinCtre.h"

class Robot : public frc::TimedRobot {
 public:
  void RobotInit() override;

  void AutonomousInit() override;
  void AutonomousPeriodic() override;

  void TeleopInit() override;
  void TeleopPeriodic() override;

  void TestInit() override;
  void TestPeriodic() override;

  frc::XboxController *xbox1, *xbox2;
  frc::DoubleSolenoid *hatch_deploy1, *hatch_deploy2, *hatch_deploy3;
  curtinfrc::VictorSpx *leftMotor2, *rightMotor2;
  curtinfrc::TalonSrx *leftMotor1, *rightMotor1, *Cargo, *Rotation;
};
