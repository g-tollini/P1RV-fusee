#pragma once

#include "my_arduino_layer/serial/myserial.hpp"
#include "my_arduino_layer/rocket-booster/ideal-booster/ideal-booster.hpp"
#include "my_arduino_layer/sensors/height-sensor/height-sensor.hpp"

#include "../../threading.hpp"

extern SerialObj Serial;

extern IdealBooster idealBooster;

extern HeightSensor heightSensor;

extern SimulationData *pSd;

void silent_delay(int ms);
void delay(int ms);

void setup();

void loop();