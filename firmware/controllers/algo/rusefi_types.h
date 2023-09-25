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
using time_t = uint32_t;
#endif

#include "rusefi_generated.h"
#include "rusefi_enums.h"
#include "firing_order.h"

#if __cplusplus
#include "efi_scaled_channel.h"
#endif

#define DEFAULT_FUEL_LOAD_COUNT 16
#define DEFAULT_IGN_LOAD_COUNT 16

#define TWO_STROKE_CYCLE_DURATION 360
#define FOUR_STROKE_CYCLE_DURATION 720

// gasoline E0
#define STOICH_RATIO 14.7f
#define CONST_PI 3.14159265358979323846


// time in seconds
using efitimesec_t = time_t;

/**
 * We use a signed type here so that subtraction result is a proper negative value.
 * A typical use-case negative result is when we do 'timeNow() - timeOfEvent' where timeOfEvent
 * is actually after timeNow() due to interrupt context switches
 *
 * See getTimeNowNt()
 * See US2NT
 * See MS2US
 */

/**
 * platform-dependent tick since boot
 * in case of stm32f4 that's 32-bit timer ticks (SCHEDULER_TIMER_DEVICE == TIM5) extended to 64 bits
 */
using efitick_t = int64_t;

/**
 * 64 bit time in microseconds (1/1_000_000 of a second), since boot
 */
using efitimeus_t = int64_t;

/**
 * 64 bit time in milliseconds (1/1_000 of a second), since boot
 */
using efitimems64_t = int64_t;

/**
 * integer time in milliseconds (1/1_000 of a second)
 * 32 bit 4B / 1000 = 4M seconds = 1111.11 hours = 23(or46?) days.
 * Please restart your ECU every 23(or46?) days? :) See issue https://github.com/rusefi/rusefi/issues/4554 tag#4554
 * See getTimeNowUs()
 */
using efitimems_t = uint32_t;

// date-time struct a la ctime struct tm
typedef struct {
	uint32_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	// uint16_t millisecond;
} efidatetime_t;

using angle_t = float;

using engineSyncCam_e = uint8_t;

// mass in grams
using mass_t = float;

// temperature, in Celsius
using temperature_t = float;
using floatms_t = float;
using floatus_t = float;

/**
 * numeric value from 0 to 100
 */
using percent_t = float;

typedef void (*Void)(void);

using lua_script_t = char[LUA_SCRIPT_SIZE];

using vehicle_info_t = char[VEHICLE_INFO_SIZE];

using vin_number_t = char[VIN_NUMBER_SIZE];

using gppwm_note_t = char[GPPWM_NOTE_SIZE];

using brain_pin_e = Gpio;

using egt_cs_array_t = brain_pin_e[EGT_CHANNEL_COUNT];

using pwm_freq_t = int16_t;

using script_setting_t = float;

using brain_input_pin_e = brain_pin_e;
using switch_input_pin_e = brain_pin_e;
using output_pin_e = brain_pin_e;

typedef void (*VoidPtr)(void*);

typedef void (*VoidInt)(int);
typedef void (*VoidIntVoidPtr)(int, void*);
typedef void (*VoidFloat)(float);
typedef void (*VoidFloatFloat)(float, float);
typedef void (*VoidFloatFloatFloat)(float, float, float);
typedef void (*VoidFloatFloatFloatFloat)(float, float, float, float);
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
