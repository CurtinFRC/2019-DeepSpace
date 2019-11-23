#include "Hatch.h"

Hatch::Hatch(int motorID, int eject, int retract, int align, int faceplant, int servoID) {
    Flooper = new curtinfrc::TalonSrx(motorID, 1024);
    Flooper->ModifyConfig([](curtinfrc::TalonSrx::Configuration &config) {
        config.slot0.kP = 0.04;
        config.slot0.kI = 0.0000;
        config.slot0.kD = 0.5;
        // left you a gift harry
        //    - jaci
        // original: 0.1, 0.0001, 0.0 (w/ motion magic)
        config.slot0.kF = 0;

        config.nominalOutputForward = 0;
        config.nominalOutputReverse = 0;
        config.peakOutputForward = 0.8;
        config.peakOutputReverse = -0.8;
        config.motionCruiseVelocity = 5000;
        config.motionAcceleration = 2000;
    });

    ejection = new frc::DoubleSolenoid(9,eject, retract);
    alignment = new frc::DoubleSolenoid(9,align, faceplant);
    Flooper->SetInverted(true);
    lock = new curtinfrc::actuators::BinaryServo(servoID, 180, 180 - 30);
}

void Hatch::setRotationSpeed(double speed) {
    if(std::abs(speed) < deadzone) speed = 0;
    Flooper->Set(speed);
}

void Hatch::downPosition() {
        Flooper->Set(curtinfrc::TalonSrx::ControlMode::Position, 30000);
        targetpos = false;
        
}

void Hatch::upPosition() {
    Flooper->Set(curtinfrc::TalonSrx::ControlMode::Position, -3000);
    targetpos = true;
    
}

void Hatch::ejectHatch(bool eject) {
    if(eject) ejection->Set(frc::DoubleSolenoid::kForward);
    else ejection->Set(frc::DoubleSolenoid::kReverse);
}

void Hatch::lockHatch(bool state) {
    lock->SetTarget(state ? curtinfrc::actuators::kReverse : curtinfrc::actuators::kForward);

    if (lock->IsDone()) lock->Stop();
}

void Hatch::alignmentPiston(bool extended) {
    if(extended) alignment->Set(frc::DoubleSolenoid::kReverse);
    else alignment->Set(frc::DoubleSolenoid::kForward);
}

bool Hatch::isLocked() {
    return alignment->Get() == frc::DoubleSolenoid::kForward;
}

double Hatch::encoderIn(){
   return Flooper->GetEncoderTicks();
}

double Hatch::velocityIn(){
    return Flooper->GetSensorVelocity();
}

void Hatch::zeroEncoder() {
    Flooper->ZeroEncoder();
}

void Hatch::update() {
    frc::SmartDashboard::PutNumber("Hatch encoder", Flooper->GetSensorPosition());
    frc::SmartDashboard::PutNumber("Hatch Velocity", velocityIn());
    frc::SmartDashboard::PutBoolean("Ejector?", ejection->Get());
    frc::SmartDashboard::PutBoolean("Aligner?", isLocked());
    frc::SmartDashboard::PutBoolean("target pos", targetpos);

    lock->Update(0); // NOTE NEEDS TO BE CHANGED IF PID IS USED
}
