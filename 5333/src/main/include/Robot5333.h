#pragma once

#include <frc/Timer.h>
#include <frc/TimedRobot.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/Spark.h>
#include <frc/DoubleSolenoid.h>

#include <networktables/NetworkTableInstance.h>

#include "CurtinCtre.h"
#include "Drivetrain.h"
#include "CurtinControllers.h"
#include "Gearbox.h"

#include "Lift.h"
#include "HarvesterIntake.h"
#include "RobotMap.h"

class Robot : public frc::TimedRobot {
 public:
  void RobotInit() override;

  void AutonomousInit() override;
  void AutonomousPeriodic() override;

  void TeleopInit() override;
  void TeleopPeriodic() override;

  void TestInit() override;
  void TestPeriodic() override;

  std::shared_ptr<nt::NetworkTable> table;
  nt::NetworkTableEntry yOffset, xOffset, endAngle;

  RobotMap robotmap;

  curtinfrc::Drivetrain *drivetrain;

  frc::Spark *harvesterMotor;
  frc::DoubleSolenoid *harvesterSolenoid;
  curtinfrc::Gearbox *harvesterGearbox;

  HarvesterIntake *harvester;

  Lift *beElevator;
};
