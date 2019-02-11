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
    
}


void DriveFunc::TurnAngle(double Angle, double dt){
    Nav->Angular(curtinfrc::sensors::AngularAxis::YAW);
    double YAW = NavG->GetAngle();
}

void DriveFunc::update(){
    frc::SmartDashboard::PutNumber("Right encoder", TalonR->GetSensorPosition());
    frc::SmartDashboard::PutNumber("Left encoder", TalonL->GetSensorPosition());
   // frc::SmartDashboard::PutNumber("YAW", Nav->Angular(curtinfrc::sensors::AngularAxis::YAW));
}

void DriveFunc::zero(){
    TalonL->ZeroEncoder();
    TalonR->ZeroEncoder();
}
