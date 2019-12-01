#pragma once

#include "WMLCtre.h"
#include <frc/smartdashboard/SmartDashboard.h>

class Cargo{
    public:
        Cargo(int SrxID1, int SrxID2, int intakeID);
        void setRotationSpeed(double speed);
        void setAngularSpeed(double speed);
        void setAngle(double newAngle);
        void setIntakeSpeed(double speed);

        void zeroEncoder();
        void update();


    private:
        wml::TalonSrx *motorSrx1, *motorSrx2;
        wml::VictorSpx *intakeSpx;

        double angle = 0;

        const double F = 1, P = 1, I = 0, D = 0.1;
        const double maxSpeed = 200;
        const double maxAcceleration = 100;
        const double deadzone = 0.01;
        const double zeroDegrees = 300; 
        const double ninetyDegrees = 850;
}; //Neo... follow the white rabbit.