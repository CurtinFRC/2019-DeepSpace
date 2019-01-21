#pragma once

#include <frc/Timer.h>
#include <frc/TimedRobot.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/Spark.h>
#include <frc/DoubleSolenoid.h>

#include "CurtinCtre.h"
#include "Drivetrain.h"
#include "CurtinControllers.h"
#include "Gearbox.h"

#include "Lift.h"

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
  
  curtinfrc::TalonSrx *leftSRX, *rightSRX;
  curtinfrc::VictorSpx *leftSPX, *rightSPX;
  curtinfrc::Gearbox *left, *right;
  curtinfrc::Drivetrain *drivetrain;

  frc::Spark *liftMotors[1];
  curtinfrc::Gearbox *liftGearbox;
  Lift *beElevator;
};
