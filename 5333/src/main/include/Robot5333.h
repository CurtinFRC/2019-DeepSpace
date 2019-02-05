#pragma once

#include <frc/Timer.h>
#include <frc/TimedRobot.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/Spark.h>
#include <frc/DoubleSolenoid.h>

#include <networktables/NetworkTableInstance.h>


#include "RobotMap.h"
#include "strategy/StrategyController.h"

#include "Lift.h"
#include "HarvesterIntake.h"

class Robot : public frc::TimedRobot, protected curtinfrc::StrategyController {
 public:
  void RobotInit() override;
  void RobotPeriodic() override;

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

  HarvesterIntake *harvester;

  Lift *beElevator;
};
