/**
 * @file    stored_value_sensor.h
 * @brief Base class for a sensor that has its value asynchronously
 * set, then later retrieved by a consumer.
 *
 * @date September 12, 2019
 * @author Matthew Kennedy, (c) 2019-2020
 */

#include "stored_value_sensor.h"

#include "global.h"

StoredValueSensor::StoredValueSensor(SensorType type)
    : StoredValueSensor(type, MS2NT(50))
{
}
