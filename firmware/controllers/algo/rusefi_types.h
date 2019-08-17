/**
 * @file	rusefi_types.h
 *
 * @date Jan 12, 2015
 * @author Andrey Belomutskiy, (c) 2012-2017
 */
#ifndef CONTROLLERS_ALGO_RUSEFI_TYPES_H_
#define CONTROLLERS_ALGO_RUSEFI_TYPES_H_

#include <string.h>
#include <stdbool.h>
#include <float.h>
#include <stdint.h>
#if defined __GNUC__
// GCC
#include <sys/types.h>
#else
// IAR
typedef unsigned int time_t;
#endif

#include "rusefi_enums.h"
#include "firing_order.h"
#include "rusefi_generated.h"

#define DEFAULT_FUEL_LOAD_COUNT 16
#define DEFAULT_IGN_LOAD_COUNT 16

// time in seconds
typedef time_t efitimesec_t;

/**
 * integer time in milliseconds
 * 32 bit 4B / 1000 = 4M seconds = 1111.11 hours = 46 days.
 * Please restart your ECU every 46 days? :)
 * See getTimeNowUs()
 */
typedef uint32_t efitimems_t;

typedef int pid_dt;

/**
 * We use a signed type here so that subtraction result is a proper negative value.
 * A typical use-case negative result is when we do 'timeNow() - timeOfEvent' where timeOfEvent
 * is actually after timeNow() due to interrupt context switches
 *
 * See getTimeNowNt()
 */
typedef int64_t efitime_t;

/**
 * 64 bit time in microseconds, since boot
 */
typedef efitime_t efitimeus_t;

/**
 * platform-dependent tick since boot
 * in case of stm32f4 that's a CPU tick
 */
typedef efitime_t efitick_t;

typedef float angle_t;

// temperature, in Celsius
typedef float temperature_t;
typedef float floatms_t;
typedef float floatus_t;

/**
 * numeric value from 0 to 100
 */
typedef float percent_t;

typedef void (*Void)(void);

typedef char le_formula_t[LE_COMMAND_LENGTH];

typedef brain_pin_e egt_cs_array_t[EGT_CHANNEL_COUNT];

typedef uint8_t afr_table_t[FUEL_LOAD_COUNT][FUEL_RPM_COUNT];
// todo: merge these two types together? but these tables have different TS parameters like ranges etc
typedef float fuel_table_t[FUEL_LOAD_COUNT][FUEL_RPM_COUNT];
typedef float ignition_table_t[IGN_LOAD_COUNT][IGN_RPM_COUNT];
typedef int16_t ignition_tps_table_t[IGN_LOAD_COUNT][IGN_RPM_COUNT];
typedef uint8_t pedal_to_tps_t[PEDAL_TO_TPS_SIZE][PEDAL_TO_TPS_SIZE];

typedef float baro_corr_table_t[BARO_CORR_SIZE][BARO_CORR_SIZE];

typedef float fsio_table_8x8_f32t[FSIO_TABLE_8][FSIO_TABLE_8];
typedef uint8_t fsio_table_8x8_u8t[FSIO_TABLE_8][FSIO_TABLE_8];
typedef float tps_tps_table_t[TPS_TPS_ACCEL_TABLE][TPS_TPS_ACCEL_TABLE];
typedef uint8_t fsio_table_8x8_u8t[FSIO_TABLE_8][FSIO_TABLE_8];

typedef uint8_t boost_table_t[BOOST_LOAD_COUNT][BOOST_RPM_COUNT];
typedef uint8_t vvt_table_t[VVT_LOAD_COUNT][VVT_RPM_COUNT];

// this is different type simply to have different hi/low range in rusefi.ini
typedef ignition_table_t angle_table_t;

typedef int cylinders_count_t;

typedef int32_t bool32_t;

typedef int16_t fsio_pwm_freq_t;

typedef float fsio_setting_t;
typedef float cfg_float_t_1f;

typedef brain_pin_e brain_input_pin_e;
typedef brain_pin_e switch_input_pin_e;

typedef fuel_table_t ve_table_t;

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
