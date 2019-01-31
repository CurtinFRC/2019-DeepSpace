#pragma once

#include <frc/TimedRobot.h>
#include <frc/XboxController.h>
#include <frc/Spark.h>
#include <frc/DoubleSolenoid.h>
#include <frc/Encoder.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/Compressor.h>

#include "CurtinCtre.h"
#include "Drivetrain.h"
#include "Gearbox.h"

#include "Cargo.h"
#include "Hatch.h"

#include "Toggle.h"

class Robot : public frc::TimedRobot {
 public:
  void RobotInit() override;

  void AutonomousInit() override;
  void AutonomousPeriodic() override;

  void TeleopInit() override;
  void TeleopPeriodic() override;

  void TestInit() override;
  void TestPeriodic() override;

  Cargo *cargo;
  Hatch *hatch;

  frc::XboxController *xbox1, *xbox2;

  frc::DoubleSolenoid *hatch_deploy1, *hatch_deploy2, *hatch_deploy3;

  curtinfrc::Drivetrain *drivetrain;
  curtinfrc::TalonSrx *leftTalon, *rightTalon, *rotateTalon1, *rotateTalon2;
  curtinfrc::VictorSpx *leftVictor, *rightVictor, *rotateVictor;
  curtinfrc::Gearbox *Left, *Right;

  frc::Compressor *compressor;

  curtinfrc::Toggle lockToggle{};
  bool lockState = false;
};
