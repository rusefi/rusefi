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
	ThermistorConf *config;
	adc_channel_e channel;
} Thermistor;

#endif /* THERMISTOR_TYPES_H_ */
