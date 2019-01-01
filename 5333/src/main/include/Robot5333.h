#pragma once

#include <frc/TimedRobot.h>
#include "frc/Spark.h"

class Robot : public frc::TimedRobot {
 public:
  void RobotInit() override;

  void AutonomousInit() override;
  void AutonomousPeriodic() override;

  void TeleopInit() override;
  void TeleopPeriodic() override;

  void TestInit() override;
  void TestPeriodic() override;

 private:
  frc::Spark s1{1}, s2{2}, s9{9};
};
