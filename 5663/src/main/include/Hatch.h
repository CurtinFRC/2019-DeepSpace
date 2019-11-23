#pragma once

#include "CurtinCtre.h"
#include <frc/DoubleSolenoid.h>
#include <cmath>
#include <frc/smartdashboard/SmartDashboard.h>

#include "actuators/BinaryServo.h"

class Hatch {
    public:
        Hatch(int motorID, int eject, int retract, int align, int faceplant, int servoID);
        void setRotationSpeed(double speed);
        void setAngle(double newAngle);
        void downPosition();
        void upPosition();
        void ejectHatch(bool eject);
        void lockHatch(bool state);
        void alignmentPiston(bool extended);   
        bool isLocked();
        double encoderIn();
        double velocityIn();
        void zeroEncoder();
        void update();
    private:
        curtinfrc::TalonSrx *Flooper;
        frc::DoubleSolenoid *ejection, *alignment;
        curtinfrc::actuators::BinaryServo *lock;
        bool targetpos;
        double angle = 0;

        double F = 1, P = 1, I = 0, D = 0.1;
        double maxSpeed = 200;
        double maxAcceleration = 100;
        double deadzone = 0.01;
        double upValue = 0; 
        double downValue = 25000;
};