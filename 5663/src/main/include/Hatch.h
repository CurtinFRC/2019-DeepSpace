#pragma once
#include "CurtinCtre.h"
#include "WPILib.h"
#include <DoubleSolenoid.h>

using namespace curtinfrc;
using namespace frc;

class Hatch{
    public:
        Hatch(int m1);
        void stop();
        void rotate(double distance);
    private:
        TalonSrx *Flooper;
};