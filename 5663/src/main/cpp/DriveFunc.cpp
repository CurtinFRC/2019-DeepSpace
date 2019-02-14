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
    _controller = new curtinfrc::control::PIDController(_gains);
    
}

void DriveFunc::Forward(double distance){
    encoderTicks = distance * 22165; //tune this number, but it should be close
    
}


double DriveFunc::TurnAngle(double TargetAngle, double dt, bool firstPress){
    Nav->Angular(curtinfrc::sensors::AngularAxis::YAW);
    double CurrentAngle = NavG->GetAngle();
    if (firstPress) _controller->SetSetpoint(CurrentAngle + TargetAngle);
    double power = _controller->Calculate(CurrentAngle, dt);
    frc::SmartDashboard::PutNumber("goal", CurrentAngle + TargetAngle);
    frc::SmartDashboard::PutNumber("power", power);
    frc::SmartDashboard::PutNumber("dt", dt);
    return power;
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
