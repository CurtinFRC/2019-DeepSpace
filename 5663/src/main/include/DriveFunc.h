#pragma once

#include <frc/smartdashboard/SmartDashboard.h>
#include "CurtinCtre.h"

class DriveFunc {
    public:
    DriveFunc(double LSRXID, double RSRXID, double LSPXID, double RSPXID);
    void Forward(double Distance);
    void TurnNinety();
    void TurnAround(double angle);
    void update();
    void zeroEncoder();
    void left();
    private:
    curtinfrc::TalonSrx *TalonL, *TalonR;
    curtinfrc::VictorSpx *VictorL, *VictorR;

};