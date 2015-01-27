#ifndef MAP_H_
#define MAP_H_

#include "engine_configuration.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sensor_types.h"

/**
 * @return Raw MAP sensor value right now
 */
float getRawMap(void);
float getBaroPressure(void);
/**
 * @return MAP value averaged within a window of measurement
 */
float getMap(void);
float getMapVoltage(void);
float getMapByVoltage(float voltage);
float decodePressure(float voltage, air_pressure_sensor_config_s * config);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
