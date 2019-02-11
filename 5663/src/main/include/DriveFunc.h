#pragma once

#include <frc/smartdashboard/SmartDashboard.h>
#include "CurtinCtre.h"
#include "Drivetrain.h"

#include "sensors/NavX.h"

class DriveFunc {
    public:
    DriveFunc(double LSRXID, double RSRXID, double LSPXID, double RSPXID);
    void Forward(double Distance);
    void TurnNinety();
    void TurnAngle(double angle, double dt);
    void update();
    void zero();
    void left();
    private:
    curtinfrc::TalonSrx *TalonL, *TalonR;
    curtinfrc::VictorSpx *VictorL, *VictorR;
    curtinfrc::DrivetrainFieldOrientedControlStrategy *DriveControl;
    curtinfrc::control::PIDController *_controller;
    curtinfrc::control::PIDGains _gains{"Turning gains", 0.03, 0.0, 0.0, 0.0};
    curtinfrc::sensors::NavX *Nav;
    curtinfrc::sensors::NavXGyro *NavG;
    const int kToleranceDegrees = 3;
};