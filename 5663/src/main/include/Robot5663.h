#pragma once

#include <frc/TimedRobot.h>
#include <frc/XboxController.h>
#include <frc/Spark.h>
#include <frc/DoubleSolenoid.h>
#include <frc/Encoder.h>

#include "CurtinCtre.h"

#include <networktables/NetworkTableInstance.h>

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
  frc::Spark *Cargo, *Rotation;
  frc::DoubleSolenoid *hatch_deploy1, *hatch_deploy2, *hatch_deploy3;

  curtinfrc::TalonSrx *left_motor1, *right_motor1, *left_motor2, *right_motor2;

  std::shared_ptr<nt::NetworkTable> table;
  nt::NetworkTableEntry targetAngle, targetDistance, targetOffset;

  int stage;
  int snapshots;
  float avgDistance;
  float avgAngle;
};
