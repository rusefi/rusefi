#include "pch.h"

#include "gearbox_indicator.h"

static bool hasinitGearbox_Indicator = false;

void Gearbox_IndicatorUpdate() {
    float gearboxPosition = Sensor::get(SensorType::GearboxPositionSensor);
}

void initGearbox_Indicator() {
    hasinitGearbox_Indicator = true;
}
