/**
 * @file	rusefi_types.h
 *
 * @date Jan 12, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

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

#include "rusefi_generated.h"
#include "rusefi_enums.h"
#include "firing_order.h"

#if __cplusplus
#include "scaled_channel.h"
#endif

#define DEFAULT_FUEL_LOAD_COUNT 16
#define DEFAULT_IGN_LOAD_COUNT 16

// gasoline E0
#define STOICH_RATIO				14.7f

// time in seconds
typedef time_t efitimesec_t;

/**
 * integer time in milliseconds (1/1_000 of a second)
 * 32 bit 4B / 1000 = 4M seconds = 1111.11 hours = 46 days.
 * Please restart your ECU every 46 days? :)
 * See getTimeNowUs()
 */
typedef uint32_t efitimems_t;

/**
 * We use a signed type here so that subtraction result is a proper negative value.
 * A typical use-case negative result is when we do 'timeNow() - timeOfEvent' where timeOfEvent
 * is actually after timeNow() due to interrupt context switches
 *
 * See getTimeNowNt()
 * See US2NT
 * See MS2US
 */
typedef int64_t efitime_t;

/**
 * 64 bit time in microseconds (1/1_000_000 of a second), since boot
 */
typedef efitime_t efitimeus_t;

/**
 * 64 bit time in milliseconds (1/1_000 of a second), since boot
 */
typedef efitime_t efitimems64_t;


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

typedef char lua_script_t[LUA_SCRIPT_SIZE];

typedef char error_message_t[ERROR_BUFFER_SIZE];

typedef char vehicle_info_t[VEHICLE_INFO_SIZE];

typedef char gppwm_note_t[GPPWM_NOTE_SIZE];

typedef char le_formula_t[LE_COMMAND_LENGTH];

typedef brain_pin_e egt_cs_array_t[EGT_CHANNEL_COUNT];


typedef int16_t fsio_pwm_freq_t;

typedef float fsio_setting_t;

typedef brain_pin_e brain_input_pin_e;
typedef brain_pin_e switch_input_pin_e;

typedef void (*VoidPtr)(void*);

typedef void (*VoidInt)(int);
typedef void (*VoidIntVoidPtr)(int, void*);
typedef void (*VoidFloat)(float);
typedef void (*VoidFloatFloat)(float, float);
typedef void (*VoidFloatFloatVoidPtr)(float, float, void*);
typedef void (*VoidIntInt)(int, int);
typedef void (*VoidIntIntVoidPtr)(int, int, void*);
typedef void (*VoidIntFloat)(int, float);

typedef void (*VoidCharPtr)(const char *);
typedef void (*VoidCharPtrVoidPtr)(const char *, void*);

typedef void (*VoidCharPtrCharPtr)(const char *, const char *);
typedef void (*VoidCharPtrCharPtrVoidPtr)(const char *, const char *, void*);

typedef void (*VoidCharPtrCharPtrCharPtr)(const char *, const char *, const char *);
typedef void (*VoidCharPtrCharPtrCharPtrCharPtrCharPtr)(const char *, const char *, const char *, const char *, const char *);
