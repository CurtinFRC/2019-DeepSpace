#include "Robot5663.h"
#include <math.h>
#include "Drivetrain.h"

using namespace curtinfrc;
using namespace frc;
using hand = frc::XboxController::JoystickHand; // Type alias for hand


void Robot::RobotInit() {
  // Motor_Controllers
  leftTalon = new TalonSrx(2, 2048);
  leftVictor = new VictorSpx(3);
  Left = new SensoredTransmission{ new SpeedControllerGroup(*leftTalon, *leftVictor), nullptr };

  rightTalon = new TalonSrx(5, 2048);
  rightVictor = new VictorSpx(4);
  Right = new SensoredTransmission{ new SpeedControllerGroup(*rightTalon, *rightVictor), nullptr };

    rotateTalon1 = new TalonSrx(1, 2048);
    rotateTalon2 = new TalonSrx(6, 0);
  DrivetrainConfig drivetrainConfig{*Left, *Right};
  drivetrain = new Drivetrain(drivetrainConfig);

  // pistonss
  hatch_deploy1 = new frc::DoubleSolenoid(0, 1);

  
  xbox1 = new frc::XboxController(0);
  xbox2 = new frc::XboxController(1);
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic() {

  // Tank drive 
   double left_speed = -xbox1->GetY(hand::kLeftHand);
   double right_speed = xbox1->GetY(hand::kRightHand);
   drivetrain->Set(left_speed, right_speed);
  
  //flooper mech
  double Hatch = xbox2->GetY(hand::kRightHand);
  Hatch = std::abs(Hatch);      //lachlan's mad code
  rotateTalon2->Set(Hatch);

  //cargor mech
  double cargo = xbox2->GetY(hand::kLeftHand);
  cargo = std::abs(cargo);      //lachlan's mad code
  rotateTalon1->Set(cargo);

  //Hatch Ejection
  if(xbox1->GetBumper(hand::kLeftHand) == 1){
    hatch_deploy1->frc::DoubleSolenoid::Set(frc::DoubleSolenoid::kForward);
    } else  { 
    hatch_deploy1-> frc::DoubleSolenoid::Set(frc::DoubleSolenoid::kReverse);
    }

  }

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
