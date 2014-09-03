/**
 * @file	sensor_types.h
 *
 * @date Dec 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef THERMISTOR_TYPES_H_
#define THERMISTOR_TYPES_H_

#define MAP_ANGLE_SIZE 8
#define MAP_WINDOW_SIZE 8

#include "rusefi_enums.h"

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


/**
 * @brief Thermistor curve parameters
 */
typedef struct {
	// these values is in Celcuus
	float tempC_1;
	float tempC_2;
	float tempC_3;
	float resistance_1;
	float resistance_2;
	float resistance_3;

	float bias_resistor;

	float s_h_a;
	float s_h_b;
	float s_h_c;
} ThermistorConf;

typedef struct {
	ThermistorConf *config;
	adc_channel_e channel;
} Thermistor;

#endif /* THERMISTOR_TYPES_H_ */
