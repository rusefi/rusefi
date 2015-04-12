/**
 * @file	rusefi_types.h
 *
 * @date Jan 12, 2015
 * @author Andrey Belomutskiy, (c) 2012-2015
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

typedef float floatms_t;

/**
 * numeric value from 0 to 100
 */
typedef float percent_t;

typedef void (*Void)(void);

#define INJECTION_PIN_COUNT 12
#define IGNITION_PIN_COUNT 12

#define EGT_CHANNEL_COUNT 8

#define JOYSTICK_PIN_COUNT 5

#define FUEL_RPM_COUNT 16
#define FUEL_LOAD_COUNT 16

#define LE_COMMAND_LENGTH 200
#define LE_COMMAND_COUNT 16

#define MAF_DECODING_COUNT 256

typedef char le_formula_t[LE_COMMAND_LENGTH];

#define HW_MAX_ADC_INDEX 16

typedef brain_pin_e egt_cs_array_t[EGT_CHANNEL_COUNT];

#define MAP_ANGLE_SIZE 8
#define MAP_WINDOW_SIZE 8

#define DWELL_COUNT 8

#define DWELL_CURVE_SIZE 8

#define CRANKING_CURVE_SIZE 8

#define CLT_CURVE_SIZE 16
#define IAT_CURVE_SIZE 16
#define VBAT_INJECTOR_CURVE_SIZE 8

#define IGN_LOAD_COUNT 16
#define IGN_RPM_COUNT 16

// todo: merge these two types together?
typedef float fuel_table_t[FUEL_LOAD_COUNT][FUEL_RPM_COUNT];
typedef float ignition_table_t[IGN_LOAD_COUNT][IGN_RPM_COUNT];

// todo: rename this structure one all tables migrated
typedef struct {
	float loadBins[IGN_LOAD_COUNT];
	float rpmBins[IGN_RPM_COUNT];
	ignition_table_t map;
} full_i_table_s;

typedef int cylinders_count_t;

typedef int32_t bool32_t;

typedef int16_t fsio_pwm_freq_t;

typedef float fsio_setting_t;

typedef brain_pin_e brain_input_pin_e;

typedef fuel_table_t ve_table_t;
typedef fuel_table_t afr_table_t;

typedef void (*VoidPtr)(void*);

typedef void (*VoidInt)(int);
typedef void (*VoidIntVoidPtr)(int, void*);
typedef void (*VoidFloat)(float);
typedef void (*VoidFloatFloat)(float, float);
typedef void (*VoidFloatFloatVoidPtr)(float, float, void*);
typedef void (*VoidIntInt)(int, int);
typedef void (*VoidIntIntVoidPtr)(int, int, void*);

typedef void (*VoidCharPtr)(const char *);
typedef void (*VoidCharPtrVoidPtr)(const char *, void*);

typedef void (*VoidCharPtrCharPtr)(const char *, const char *);
typedef void (*VoidCharPtrCharPtrVoidPtr)(const char *, const char *, void*);

typedef void (*VoidCharPtrCharPtrCharPtr)(const char *, const char *, const char *);
typedef void (*VoidCharPtrCharPtrCharPtrCharPtrCharPtr)(const char *, const char *, const char *, const char *, const char *);

#endif /* CONTROLLERS_ALGO_RUSEFI_TYPES_H_ */
