#include "Hatch.h"

Hatch::Hatch(int motorID, int eject, int retract, int align, int faceplant, int servoID) {
    Flooper = new wml::TalonSrx(motorID, 1024);
    Flooper->ModifyConfig([](wml::TalonSrx::Configuration &config) {
        config.slot0.kP = 0.1;
        config.slot0.kI = 0.0;
        config.slot0.kD = 0.0;
        config.slot0.kF = 0;

        config.nominalOutputForward = 0;
        config.nominalOutputReverse = 0;
        config.peakOutputForward = 1;
        config.peakOutputReverse = -1;
        config.motionCruiseVelocity =3000;
        config.motionAcceleration = 1000;
    });

    ejection = new frc::DoubleSolenoid(9,eject, retract);
    alignment = new frc::DoubleSolenoid(9,align, faceplant);
    Flooper->SetInverted(true);
    lock = new wml::actuators::BinaryServo(servoID, 180, 180 - 30);
}

void Hatch::setRotationSpeed(double speed) {
    if(std::abs(speed) < deadzone) speed = 0;
    Flooper->Set(speed);
}

void Hatch::downPosition() {
        Flooper->Set(wml::TalonSrx::ControlMode::MotionMagic, 30000);
        targetpos = false;
        
}

void Hatch::upPosition() {
    Flooper->Set(wml::TalonSrx::ControlMode::MotionMagic, -1000);
    targetpos = true;
    
}

void Hatch::ejectHatch(bool eject) {
    if(eject) ejection->Set(frc::DoubleSolenoid::kForward);
    else ejection->Set(frc::DoubleSolenoid::kReverse);
}

void Hatch::lockHatch(bool state) {
    lock->SetTarget(state ? wml::actuators::kReverse : wml::actuators::kForward);
}

void Hatch::alignmentPiston(bool extended) {
    if(extended) alignment->Set(frc::DoubleSolenoid::kReverse);
    else alignment->Set(frc::DoubleSolenoid::kForward);
}

void Hatch::zeroEncoder() {
    Flooper->ZeroEncoder();
}

void Hatch::update() {
    frc::SmartDashboard::PutNumber("Hatch encoder", Flooper->GetSensorPosition());
    frc::SmartDashboard::PutBoolean("Ejector?", ejection->Get());
    frc::SmartDashboard::PutBoolean("Aligner?", alignment->Get());
    frc::SmartDashboard::PutBoolean("target pos", targetpos);

    lock->Update(0); // NOTE NEEDS TO BE CHANGED IF PID IS USED
}
