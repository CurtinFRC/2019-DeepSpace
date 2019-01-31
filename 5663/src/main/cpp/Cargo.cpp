#include "Cargo.h"

#include <cmath>

Cargo::Cargo(int SrxID, int SpxID, int intakeID) {
    motorSrx = new curtinfrc::TalonSrx(SrxID, 1024);
    motorSrx->ModifyConfig([](curtinfrc::TalonSrx::Configuration &config) {
        config.slot0.kP = 1;
        config.slot1.kI = 0;
        config.slot2.kD = 0.001;
        config.slot3.kF = 0;

        config.nominalOutputForward = 0;
        config.nominalOutputReverse = 0;
        config.peakOutputForward = 1;
        config.peakOutputReverse = -1;
        config.motionCruiseVelocity = 200;
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

    motorSrx->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, finalEncoderCount);
}


void Cargo::setIntakeSpeed(double speed) {
    if(std::abs(speed) < deadzone) speed = 0;
    intakeSpx->Set(speed);
}

void Cargo::zeroEncoder() {
    motorSrx->ResetEncoder();
    angle = 330;
}

void Cargo::update() {
    frc::SmartDashboard::PutNumber("Cargo encoder", motorSrx->GetEncoderTicks());
}