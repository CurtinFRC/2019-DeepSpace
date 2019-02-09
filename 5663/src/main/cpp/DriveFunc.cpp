#include "DriveFunc.h"
//#include "NavX.h"

DriveFunc::DriveFunc(double LSRXID, double RSRXID, double LSPXID, double RSPXID){
    TalonL = new curtinfrc::TalonSrx(LSRXID, 1024);
    TalonL->ModifyConfig([](curtinfrc::TalonSrx::Configuration &config) {
        config.slot0.kP = 0.4;
        config.slot0.kI = 0.001;
        config.slot0.kD = 0.06;
        config.slot0.kF = 0.0;

        config.nominalOutputForward = 0;
        config.nominalOutputReverse = 0;
        config.peakOutputForward = 1;
        config.peakOutputReverse = -1;
        config.motionCruiseVelocity = 100000;
        config.motionAcceleration = 7500;
    });
    TalonR = new curtinfrc::TalonSrx(RSRXID, 1024);
    TalonR->ModifyConfig([](curtinfrc::TalonSrx::Configuration &config) {
        config.slot0.kP = 0.4;
        config.slot0.kI = 0.001;
        config.slot0.kD = 0.06;
        config.slot0.kF = 0.0;

        config.nominalOutputForward = 0;
        config.nominalOutputReverse = 0;
        config.peakOutputForward = 1;
        config.peakOutputReverse = -1;
        config.motionCruiseVelocity = 100000;
        config.motionAcceleration = 7500;
    });

    VictorL = new curtinfrc::VictorSpx(LSPXID);
    VictorR = new curtinfrc::VictorSpx(RSPXID);

    VictorL->Set(curtinfrc::VictorSpx::ControlMode::Follower, LSPXID);
    VictorR->Set(curtinfrc::VictorSpx::ControlMode::Follower, RSPXID);
}

void DriveFunc::Forward(double distance){
    double encoderL = TalonL->GetSensorPosition();
    double encoderR = TalonR->GetSensorPosition();
    TalonL->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, encoderL + distance);
    TalonR->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, encoderR + distance);
}

void DriveFunc::TurnNinety(){
    //if (){
        TalonR->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, -911);
        TalonL->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, 911);
    //} //else {
    //     TalonL->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, -10000);
    //     TalonR->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, 10000);
    // }
}

void DriveFunc::TurnAround(double Angle){
    double AngleA = 1024*Angle;
    TalonL->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, AngleA);
    TalonR->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, AngleA);
}

void DriveFunc::update(){
    frc::SmartDashboard::PutNumber("Right encoder", TalonR->GetSensorPosition());
    frc::SmartDashboard::PutNumber("Left encoder", TalonL->GetSensorPosition());
}

void DriveFunc::zeroEncoder(){
    TalonL->ZeroEncoder();
    TalonR->ZeroEncoder();
}

void DriveFunc::left(){
    TalonL->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, 10000);
}