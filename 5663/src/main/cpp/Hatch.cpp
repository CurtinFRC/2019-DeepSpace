#include "Hatch.h"

Hatch::Hatch(int motorID, int eject, int retract, int align, int faceplant, int servoID) {
    Flooper = new curtinfrc::TalonSrx(motorID, 1024);
    Flooper->ModifyConfig([](curtinfrc::TalonSrx::Configuration &config) {
        config.slot0.kP = 0.25;
        config.slot1.kI = 0.002;
        config.slot2.kD = 0.004;
        config.slot3.kF = 0;

        config.nominalOutputForward = 0;
        config.nominalOutputReverse = 0;
        config.peakOutputForward = 1;
        config.peakOutputReverse = -1;
        config.motionCruiseVelocity =100000;
        config.motionAcceleration = 7500;
    });

    ejection = new frc::DoubleSolenoid(9,eject, retract);
    alignment = new frc::DoubleSolenoid(9,align, faceplant);

    curtinfrc::actuators::BinaryServoConfig lockConfig{ servoID, 180, 180 - 30 };
    lock = new curtinfrc::actuators::BinaryServo(lockConfig);
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
    lock->SetTarget(curtinfrc::actuators::kForward);
    if (lock->IsDone()) {
        lock->Stop();
        Flooper->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, 31000);
    } else Flooper->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, -30000);
}


void Hatch::upPosition() {
    Flooper->Set(curtinfrc::TalonSrx::ControlMode::MotionMagic, -30000);
}

void Hatch::ejectHatch(bool eject) {
    if(eject) ejection->Set(frc::DoubleSolenoid::kReverse);
    else ejection->Set(frc::DoubleSolenoid::kForward);
}

void Hatch::lockHatch(bool state) {
    lock->SetTarget(state ? curtinfrc::actuators::kReverse : curtinfrc::actuators::kForward);

    if (lock->IsDone()) lock->Stop();
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

    lock->Update(0); // NOTE NEEDS TO BE CHANGED IF PID IS USED
}
