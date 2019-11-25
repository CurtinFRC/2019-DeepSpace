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
    wml::TalonSrx *TalonL, *TalonR;
    wml::VictorSpx *VictorL, *VictorR;
    wml::control::PIDGains _gains{"Turning gains", 0.03, 0.0003, 0.003, 0.0}, _driveGains{"Drive forward gains", 0.03, 0.0, 0.0};
    wml::control::PIDController *_turnController, *_driveControllerR, *_driveControllerL;
    wml::sensors::NavX *Nav;
    wml::sensors::NavXGyro *NavG;
    const int kToleranceDegrees = 1;
    int encoderTicks;
    std::vector<double> powers;
};