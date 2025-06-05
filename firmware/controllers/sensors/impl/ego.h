/**
 * @file    ego.h
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "sensor_type.h"
#include "rusefi_enums.h"

namespace Ego {
    float getAfr(SensorType type);
    void setEgoSensor(ego_sensor_e type);
    void registerStoredValueSensors();
}
