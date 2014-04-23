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

	float Min;
	float Max;
	map_type_e mapType;
} MapConf_s;

typedef struct {
	MapConf_s config;
	int channel;
} map_s;


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
	int channel;
} Thermistor;

#endif /* THERMISTOR_TYPES_H_ */
