#pragma once

#include <frc/smartdashboard/SmartDashboard.h>
#include "CurtinCtre.h"
#include "Drivetrain.h"
#include "control/PIDController.h"
#include "sensors/NavX.h"

class DriveFunc {
    public:
    DriveFunc(double LSRXID, double RSRXID, double LSPXID, double RSPXID);
    std::vector<double> Forward(double Distance, double dt, bool firstPress);
    void TurnNinety();
    double TurnAngle(double TargetAngle, double dt, bool firstPress);
    void update();
    void zero();
    void left();
    private:
    curtinfrc::TalonSrx *TalonL, *TalonR;
    curtinfrc::VictorSpx *VictorL, *VictorR;
    curtinfrc::DrivetrainFieldOrientedControlStrategy *DriveControl;
    curtinfrc::control::PIDGains _gains{"Turning gains", 0.03, 0.0003, 0.003, 0.0}, _driveGains{"Drive forward gains", 0.03, 0.0, 0.0};
    curtinfrc::control::PIDController *_turnController, *_driveControllerR, *_driveControllerL;
    curtinfrc::sensors::NavX *Nav;
    curtinfrc::sensors::NavXGyro *NavG;
    const int kToleranceDegrees = 1;
    int encoderTicks;
    std::vector<double> powers;
};