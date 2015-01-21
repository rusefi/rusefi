/**
 * @file	sensor_types.h
 *
 * @date Dec 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef THERMISTOR_TYPES_H_
#define THERMISTOR_TYPES_H_

#define MAP_ANGLE_SIZE 8
#define MAP_WINDOW_SIZE 8

#include "rusefi_enums.h"
#include "engine_configuration_generated_structures.h"

typedef struct {
	/**
	 * kPa value at zero volts
	 */
	float customValueAt0;
	/**
	 * kPa value at 5 volts
	 */
	float customValueAt5;
	air_pressure_sensor_type_e sensorType;
	adc_channel_e hwChannel;
} air_pressure_sensor_config_s;

/**
 * @brief MAP averaging configuration
 */
typedef struct {
	float samplingAngleBins[MAP_ANGLE_SIZE];
	/**
	 * @brief MAP averaging sampling start angle, by RPM
	 */
	float samplingAngle[MAP_ANGLE_SIZE];

	float samplingWindowBins[MAP_WINDOW_SIZE];
	/**
	 * @brief MAP averaging angle duration, by RPM
	 */
	float samplingWindow[MAP_WINDOW_SIZE];

	air_pressure_sensor_config_s sensor;

} MAP_sensor_config_s;


typedef struct {
	ThermistorConf *config;
	adc_channel_e channel;
} Thermistor;

#endif /* THERMISTOR_TYPES_H_ */
