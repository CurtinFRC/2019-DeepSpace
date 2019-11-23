#pragma once

#include <frc/TimedRobot.h>
#include <frc/XboxController.h>
#include <frc/Spark.h>
#include <frc/DoubleSolenoid.h>
#include <frc/Encoder.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/Compressor.h>
#include "frc/AnalogInput.h"
#include "frc/DigitalInput.h"
#include "frc/I2C.h"
#include <frc/Timer.h>

#include <networktables/NetworkTableInstance.h>

#include "CurtinCtre.h"
#include "Drivetrain.h"
#include "Gearbox.h"

#include "Cargo.h"
#include "Hatch.h"
#include "DriveFunc.h"

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
  DriveFunc *driveFunct;
  frc::Timer *timer;
  frc::I2C *arduino;

  frc::XboxController *xbox1, *xbox2;

  frc::DoubleSolenoid *hatch_deploy1, *hatch_deploy2, *hatch_deploy3, *BIGBOYS;

  curtinfrc::Drivetrain *drivetrain;
  curtinfrc::TalonSrx *leftTalon, *rightTalon, *rotateTalon1, *rotateTalon2, *TalonL;
  curtinfrc::VictorSpx *leftVictor, *rightVictor, *rotateVictor;
  frc::Spark *ClimbLeft, *ClimbRight;
  curtinfrc::Gearbox *Left, *Right;

  frc::Compressor *compressor;
  frc::AnalogInput *AI;
  frc::DigitalInput *DI;

  curtinfrc::Toggle lockToggle{};
  bool lockState = true;

  uint8_t message = 72;
 private:
  double lastTimer;
  double power;
  bool pressRBumper;
  bool pressLBumper;
  bool pressBButton;
  std::vector<double> powers;
  std::shared_ptr<nt::NetworkTable> visionTable, tapeTable, hatchTable;
  nt::NetworkTableEntry targetAngle, targetDistance, targetOffset;
  int stage;
  int snapshots;
  float avgDistance, avgAngle, avgOffset;
};
