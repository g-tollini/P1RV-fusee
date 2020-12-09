#include "height-sensor.hpp"

#include "../../../myarduino.hpp"

double HeightSensor::MeasureHeight()
{
    silent_delay(5);
    return pSd->pShm->position.z;
}