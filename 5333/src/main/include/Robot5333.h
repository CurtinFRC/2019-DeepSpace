#pragma once

#include <frc/Timer.h>
#include <frc/TimedRobot.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/Spark.h>
#include <frc/DoubleSolenoid.h>

#include <networktables/NetworkTableInstance.h>


#include "RobotMap.h"

#include "Lift.h"
#include "HarvesterIntake.h"

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
  curtinfrc::DrivetrainController *drivetrainController;

  HarvesterIntake *harvester;
  HarvesterIntakeController *harvesterController;

  Lift *beElevator;
};
