#include "Cargo.h"

#include <cmath>

Cargo::Cargo(int SrxID1, int SrxID2, int intakeID) {
    motorSrx1 = new curtinfrc::TalonSrx(SrxID1, 1024);
    motorSrx1->ModifyConfig([](curtinfrc::TalonSrx::Configuration &config) {
        config.slot0.kP = 0.1;
        config.slot0.kI = 0;
        config.slot0.kD = 0.002;
        config.slot0.kF = 0;

        config.nominalOutputForward = 0;
        config.nominalOutputReverse = 0;
        config.peakOutputForward = 1;
        config.peakOutputReverse = -1;
        config.motionCruiseVelocity = 500000;
        config.motionAcceleration = 90000;

        
    });
    motorSrx2 = new curtinfrc::TalonSrx(SrxID2, 1024);
    motorSrx2->ModifyConfig([](curtinfrc::TalonSrx::Configuration &config) {
        config.slot0.kP = 0.1;
        config.slot0.kI = 0;
        config.slot0.kD = 0.002;
        config.slot0.kF = 0;

        config.nominalOutputForward = 0;
        config.nominalOutputReverse = 0;
        config.peakOutputForward = 1;
        config.peakOutputReverse = -1;
        config.motionCruiseVelocity = 500000;
        config.motionAcceleration = 90000;

        
    });
    motorSrx1->SetInverted(true);
    intakeSpx = new curtinfrc::VictorSpx(intakeID);
}

void Cargo::setRotationSpeed(double speed) { //Percent speed
    if(std::abs(speed) < deadzone) speed = 0;
    motorSrx1->Set(-speed);
    motorSrx2->Set(speed);
    }


void Cargo::setAngle(double newAngle) { //Set intake to a specific angle
    motorSrx1->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, newAngle);
    motorSrx2->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, -newAngle);
}


void Cargo::setIntakeSpeed(double speed) {
    if(std::abs(speed) < deadzone) speed = 0;
    intakeSpx->Set(speed);
}

void Cargo::zeroEncoder() {
    motorSrx1->ZeroEncoder();
    motorSrx2->ZeroEncoder();
}

void Cargo::update() {
    frc::SmartDashboard::PutNumber("Cargo encoder 1", motorSrx1->GetEncoderTicks());
    frc::SmartDashboard::PutNumber("Cargo encoder 2", motorSrx2->GetEncoderTicks());
}