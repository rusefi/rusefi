/**
 * @file	rusefi_types.h
 *
 * @date Jan 12, 2015
 * @author Andrey Belomutskiy, (c) 2012-2014
 */
#ifndef CONTROLLERS_ALGO_RUSEFI_TYPES_H_
#define CONTROLLERS_ALGO_RUSEFI_TYPES_H_

#include <stdint.h>
#include "rusefi_enums.h"

/**
 * integer time in milliseconds
 * 32 bit 4B / 1000 = 4M seconds = 1111.11 hours = 46 days.
 * Please restart your ECU every 46 days? :)
 */
typedef uint32_t efitimems_t;

/**
 * 64 bit time in microseconds, since boot
 */
typedef uint64_t efitimeus_t;

/**
 * platform-dependent tick since boot
 * in case of stm32f4 that's a CPU tick
 */
typedef uint64_t efitick_t;

typedef float angle_t;

/**
 * numeric value from 0 to 100
 */
typedef float percent_t;

typedef void (*Void)(void);

#define INJECTION_PIN_COUNT 12
#define IGNITION_PIN_COUNT 12

#define MAX31855_CS_COUNT 8

#define JOYSTICK_PIN_COUNT 5

#define FUEL_RPM_COUNT 16
#define FUEL_LOAD_COUNT 16

#define LE_COMMAND_LENGTH 200
#define LE_COMMAND_COUNT 16

typedef char le_formula_t[LE_COMMAND_LENGTH];

#define HW_MAX_ADC_INDEX 16

typedef brain_pin_e egt_cs_array_t[MAX31855_CS_COUNT];

#define MAP_ANGLE_SIZE 8
#define MAP_WINDOW_SIZE 8

/**
 * @brief Thermistor curve parameters

 */
typedef struct {
	/**
	 * these values are in Celcuus
	 * offset 0
	 */
	float tempC_1;
	/**
	 * offset 4
	 */
	float tempC_2;
	/**
	 * offset 8
	 */
	float tempC_3;
	/**
	 * offset 12
	 */
	float resistance_1;
	/**
	 * offset 16
	 */
	float resistance_2;
	/**
	 * offset 20
	 */
	float resistance_3;
	/**
	 * offset 24
	 */
	float bias_resistor;
	/**
	 * offset 28
	 */
	float s_h_a;
	/**
	 * offset 32
	 */
	float s_h_b;
	/**
	 * offset 36
	 */
	float s_h_c;
	/** total size 40*/
} ThermistorConf;


typedef struct {
	ThermistorConf *config;
	adc_channel_e channel;
} Thermistor;

#endif /* CONTROLLERS_ALGO_RUSEFI_TYPES_H_ */
