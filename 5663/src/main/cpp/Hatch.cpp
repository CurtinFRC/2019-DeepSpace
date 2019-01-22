#pragma once
#include "Hatch.h"
#include "CurtinCtre.h"

using namespace curtinfrc;
using namespace frc;

Hatch::Hatch(int m1){
    Flooper = new TalonSrx(4, 0);
}

void Hatch::stop(){
    Flooper->Set(curtinfrc::TalonSrx::ControlMode::PercentOutput, 0);

}

void Hatch::rotate(double distance){
    Flooper->Set(curtinfrc::TalonSrx::ControlMode::Position, distance);
}
