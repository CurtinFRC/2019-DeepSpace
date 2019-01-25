#include "Hatch.h"

Hatch::Hatch(int motorID, int eject, int retract, int align, int faceplant) {
    Flooper = new curtinfrc::TalonSrx(motorID, 1);
    Flooper->ModifyConfig([](curtinfrc::TalonSrx::Configuration &config) {
        config.slot0.kP = 5;
        config.slot1.kI = 0;
        config.slot2.kD = 0.00;
        config.slot3.kF = 0;

        config.nominalOutputForward = 0;
        config.nominalOutputReverse = 0;
        config.peakOutputForward = 1;
        config.peakOutputReverse = -1;
        config.motionCruiseVelocity = 200;
        config.motionAcceleration = 100;
    });

    ejection = new frc::DoubleSolenoid(9,eject, retract);
    alignment = new frc::DoubleSolenoid(9,align, faceplant);    
}

void Hatch::setRotationSpeed(double speed) {
    if(std::abs(speed) < deadzone) speed = 0;
    Flooper->Set(speed);
}

void Hatch::setAngle(double newAngle) {
    //double encoderTicks = ((ninetyDegrees - zeroDegrees)/90) * (angle - newAngle);
  //  double finalEncoderCount = Flooper->GetSensorPosition() + encoderTicks;

   //  Flooper->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, finalEncoderCount);
}

void Hatch::downPosition() {
    Flooper->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, 22000);
}


void Hatch::upPosition() {
    Flooper->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, 100);
}

void Hatch::ejectHatch(bool eject) {
    if(eject) ejection->Set(frc::DoubleSolenoid::kForward);
    else ejection->Set(frc::DoubleSolenoid::kReverse);
}

void Hatch::alignmentPiston(bool extended) {
    if(extended) alignment->Set(frc::DoubleSolenoid::kForward);
    else alignment->Set(frc::DoubleSolenoid::kReverse);
}

void Hatch::zeroEncoder() {
    Flooper->ResetEncoder();
}

void Hatch::update() {
    frc::SmartDashboard::PutNumber("Hatch encoder", Flooper->GetSensorPosition());
    frc::SmartDashboard::PutBoolean("Ejector?", ejection->Get());
    frc::SmartDashboard::PutBoolean("Aligner?", alignment->Get());
}
