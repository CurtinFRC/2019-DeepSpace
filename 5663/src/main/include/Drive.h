#pragma once
#include "CurtinCtre.h"

using namespace frc;
using namespace curtinfrc;

class Drive{
    public:
        Drive(int l1, int l2, int r1, int r2);
        void Stop();
        void TankDrive(double l, double r);
        bool DriveForward(double distance , double speed, double timeout=0);
    private:
        TalonSrx *leftMotor1, *rightMotor1;
        VictorSpx *leftMotor2, *rightMotor2;
        Timer *timeoutCheck;
        double deadzone = 0.01;
        const double encoderConst = 1;
        bool driving = false;
};