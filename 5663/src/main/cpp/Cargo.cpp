#include "Cargo.h"

#include <cmath>

Cargo::Cargo(int SrxID, int SpxID, int intakeID) {
    motorSrx = new curtinfrc::TalonSrx(SrxID, 1024);
    motorSrx->ModifyConfig([](curtinfrc::TalonSrx::Configuration &config) {
        config.slot0.kP = 0.1;
        config.slot0.kI = 0;
        config.slot0.kD = 0.0;
        config.slot0.kF = 0;

        config.nominalOutputForward = 0;
        config.nominalOutputReverse = 0;
        config.peakOutputForward = 1;
        config.peakOutputReverse = -1;
        config.motionCruiseVelocity = 1000;
        config.motionAcceleration = 100;

        
    });
    motorSpx = new curtinfrc::VictorSpx(SpxID);
    intakeSpx = new curtinfrc::VictorSpx(intakeID);

    //motorSpx->Set(curtinfrc::VictorSpx::ControlMode::Follower, SrxID);
}

void Cargo::setRotationSpeed(double speed) { //Percent speed
    if(std::abs(speed) < deadzone) speed = 0;
    motorSrx->Set(speed);
    motorSpx->Set(speed);
    }

void Cargo::setAngularSpeed(double speed) { //Speed in degrees per second
    
}

void Cargo::setAngle(double speed, double newAngle) { //Set intake to a specific angle
    double encoderTicks = ((ninetyDegrees - zeroDegrees)/90) * (angle - newAngle);
    double finalEncoderCount = motorSrx->GetSensorPosition() + encoderTicks;
    motorSrx->SetInverted(false);
    motorSrx->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, 21000);
}


void Cargo::setIntakeSpeed(double speed) {
    if(std::abs(speed) < deadzone) speed = 0;
    intakeSpx->Set(speed);
}

void Cargo::zeroEncoder() {
    motorSrx->ZeroEncoder();
    angle = 330;
}

void Cargo::update() {
    frc::SmartDashboard::PutNumber("Cargo encoder", motorSrx->GetEncoderTicks());
}