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
#include <sys/types.h>

#include <board_types.h>
#include <rusefi/rusefi_time_types.h>
#include "generated_lookup_meta.h"
#include "rusefi_enums.h"
#include "firing_order.h"

#if __cplusplus
#include "efi_scaled_channel.h"
#endif

#define DEFAULT_FUEL_LOAD_COUNT 16
#define DEFAULT_FUEL_RPM_COUNT 16
#define DEFAULT_IGN_LOAD_COUNT 16
#define DEFAULT_IGN_RPM_COUNT 16

#define TWO_STROKE_CYCLE_DURATION 360
#define FOUR_STROKE_CYCLE_DURATION 720

// gasoline E0
#define STOICH_RATIO 14.7f
#define CONST_PI 3.14159265358979323846

/**
 * integer time in milliseconds (1/1_000 of a second)
 * 32 bit 4B / 1000 = 4M seconds = 1111.11 hours = 23(or46?) days.
 * Please restart your ECU every 23(or46?) days? :) See issue https://github.com/rusefi/rusefi/issues/4554 tag#4554
 * See getTimeNowUs()
 */
using efitimems_t = uint32_t;

using efidur_t = efitick_t;

// date-time struct a la ctime struct tm
typedef struct {
	uint32_t year = 0;
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

using lua_script_t = char[LUA_SCRIPT_SIZE];

using warning_message_t = char[WARNING_BUFFER_SIZE];

using vehicle_info_t = char[VEHICLE_INFO_SIZE];

using vin_number_t = char[VIN_NUMBER_SIZE];

using gppwm_note_t = char[GPPWM_NOTE_SIZE];

using brain_pin_e = Gpio;

using egt_cs_array_t = brain_pin_e[EGT_CHANNEL_COUNT];

using pwm_freq_t = int16_t;

using script_setting_t = float;

using brain_input_pin_e = brain_pin_e;
using switch_input_pin_e = brain_pin_e;
using sent_input_pin_e = brain_pin_e;
using output_pin_e = brain_pin_e;
