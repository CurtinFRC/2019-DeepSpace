#pragma once

#include "CurtinCtre.h"

class DriveFunc {
    public:
    DriveFunc(double LSRXID, double RSRXID, double LSPXID, double RSPXID);
    void Forward(double Distance);
    void TurnNinety(bool Direction);
    void TurnAround(double angle);
    private:
    curtinfrc::TalonSrx *TalonL, *TalonR;
    curtinfrc::VictorSpx *VictorL, *VictorR;

};