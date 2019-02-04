#include "DriveFunc.h"

DriveFunc::DriveFunc(double LSRXID, double RSRXID, double LSPXID, double RSPXID){
    TalonL = new curtinfrc::TalonSrx(LSRXID, 1024);
    TalonL->ModifyConfig([](curtinfrc::TalonSrx::Configuration &config) {
        config.slot0.kP = 0.25;
        config.slot1.kI = 0.002;
        config.slot2.kD = 0.004;
        config.slot3.kF = 0;

        config.nominalOutputForward = 0;
        config.nominalOutputReverse = 0;
        config.peakOutputForward = 1;
        config.peakOutputReverse = -1;
        config.motionCruiseVelocity = 100000;
        config.motionAcceleration = 7500;
    });
    TalonR = new curtinfrc::TalonSrx(RSRXID, 1024);
    TalonR->ModifyConfig([](curtinfrc::TalonSrx::Configuration &config) {
        config.slot0.kP = 0.25;
        config.slot1.kI = 0.002;
        config.slot2.kD = 0.004;
        config.slot3.kF = 0;

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
    TalonL->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, distance);
    TalonR->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, distance);
}

void DriveFunc::TurnNinety(bool direction){
    if (direction == true){
        TalonL->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, 10000);
        TalonR->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, -10000);
    } else {
        TalonL->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, -10000);
        TalonR->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, 10000);
    }
}

void DriveFunc::TurnAround(double Angle){
    double AngleA = 1024*Angle;
    TalonL->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, AngleA);
    TalonR->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, -AngleA);
}
