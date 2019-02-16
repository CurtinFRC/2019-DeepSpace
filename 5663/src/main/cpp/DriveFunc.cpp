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
    _turnController = new curtinfrc::control::PIDController(_gains);
    _driveControllerR = new curtinfrc::control::PIDController(_driveGains);
    _driveControllerL = new curtinfrc::control::PIDController(_driveGains);

}

std::vector<double> DriveFunc::Forward(double distance, double dt, bool firstPress){
    double currentRight = TalonR->GetSensorPosition();
    double currentLeft = TalonL->GetSensorPosition();
    if (firstPress) {
        encoderTicks = distance * 1747; //tune this number, but it should be close
        _driveControllerR->SetSetpoint(currentRight + encoderTicks);
        _driveControllerL->SetSetpoint(currentLeft + encoderTicks);
    }
    powers.clear();
    powers.push_back(_driveControllerL->Calculate(currentLeft, dt));
    powers.push_back(_driveControllerR->Calculate(currentRight, dt));
    return powers;
}


double DriveFunc::TurnAngle(double TargetAngle, double dt, bool firstPress){
    Nav->Angular(curtinfrc::sensors::AngularAxis::YAW);
    double CurrentAngle = NavG->GetAngle();
    if (firstPress) _turnController->SetSetpoint(CurrentAngle + TargetAngle);
    double power = _turnController->Calculate(CurrentAngle, dt);
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
