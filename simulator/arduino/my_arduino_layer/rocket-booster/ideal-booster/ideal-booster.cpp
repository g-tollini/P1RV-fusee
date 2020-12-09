#include "ideal-booster.hpp"

#include "../../../myarduino.hpp"

void IdealBooster::SetThrust(double _thrust)
{
    pSd->pShm->booster_thrust = _thrust;
}