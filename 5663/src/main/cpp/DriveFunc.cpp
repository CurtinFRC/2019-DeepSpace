#include "DriveFunc.h"


DriveFunc::DriveFunc(double LSRXID, double RSRXID, double LSPXID, double RSPXID){
    TalonL = new curtinfrc::TalonSrx(LSRXID, 1024);
    TalonR = new curtinfrc::TalonSrx(RSRXID, 1024);

    VictorL = new curtinfrc::VictorSpx(LSPXID);
    VictorR = new curtinfrc::VictorSpx(RSPXID);

    VictorL->Set(curtinfrc::VictorSpx::ControlMode::Follower, LSPXID);
    VictorR->Set(curtinfrc::VictorSpx::ControlMode::Follower, RSPXID);

    Nav = new curtinfrc::sensors::NavX(frc::SPI::Port::kMXP);
    NavG = new curtinfrc::sensors::NavXGyro(*Nav, curtinfrc::sensors::AngularAxis::YAW);
}

void DriveFunc::Forward(double distance){
    encoderTicks = distance * 22165; //tune this number, but it should be close
    
}


void DriveFunc::TurnAngle(double TargetAngle, double dt){
    Nav->Angular(curtinfrc::sensors::AngularAxis::YAW);
    double CurrentAngle = NavG->GetAngle();
    double LeftAngle =2.84;
    double LeftTarget = TargetAngle + CurrentAngle;
}

void DriveFunc::update(){
    frc::SmartDashboard::PutNumber("Right encoder", TalonR->GetSensorPosition());
    frc::SmartDashboard::PutNumber("Left encoder", TalonL->GetSensorPosition());
    frc::SmartDashboard::PutNumber("YAW", NavG->GetAngle());
}

void DriveFunc::zero(){
    TalonL->ZeroEncoder();
    TalonR->ZeroEncoder();
}
