#pragma once

#include <frc/Timer.h>
#include <frc/TimedRobot.h>
#include <frc/XboxController.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/Spark.h>
#include <frc/Talon.h>
#include <frc/DoubleSolenoid.h>
#include "controllers/Controllers.h"

#include "WMLCtre.h"
#include "Gearbox.h"
#include "Drivetrain.h"

class Robot : public frc::TimedRobot {
 public:
  void RobotInit() override;

  void AutonomousInit() override;
  void AutonomousPeriodic() override;

  void TeleopInit() override;
  void TeleopPeriodic() override;

  void TestInit() override;
  void TestPeriodic() override;

  wml::controllers::XboxController *xbox;
  
  frc::Spark *leftMotors[1], *rightMotors[1];
  wml::Gearbox *left, *right;
  wml::Drivetrain *drivetrain;

  frc::DoubleSolenoid *hatchEjector;
};
