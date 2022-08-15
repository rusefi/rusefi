// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/trigger/trigger_central.txt Mon Aug 15 18:43:34 UTC 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
#define adc_channel_e_enum "Disabled", "PA2", "PA3", "INVALID", "PD3", "INVALID", "INVALID", "INVALID", "PB12", "PB13", "INVALID", "PE2", "INVALID", "PC14", "PC15", "PC16", "PC17"
#define BANKS_COUNT 1
#define brain_input_pin_e_enum "NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "N/A", "N/A", "PA10", "PA11", "PA12", "PA13", "N/A", "N/A", "N/A", "N/A", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "N/A", "N/A", "N/A", "N/A", "PB12", "PB13", "N/A", "N/A", "N/A", "N/A", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "N/A", "N/A", "N/A", "N/A", "PC14", "PC15", "PC16", "PC17", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "PD15", "PD16", "N/A", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A"
#define CAM_INPUTS_COUNT_padding 3
#define CAMS_PER_BANK 1
#define CAMS_PER_BANK_padding 1
#define Gpio_enum "NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PA8", "PA9", "PA10", "PA11", "PA12", "PA13", "PA14", "PA15", "PA16", "PA17", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "PB8", "PB9", "PB10", "PB11", "PB12", "PB13", "PB14", "PB15", "PB16", "PB17", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "PC10", "PC11", "PC12", "PC13", "PC14", "PC15", "PC16", "PC17", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "PD8", "PD9", "PD10", "PD11", "PD12", "PD13", "PD14", "PD15", "PD16", "PD17", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "PE12", "PE13", "PE14", "PE15", "PE16", "PE17", "TLE6240_1", "TLE6240_2", "TLE6240_3", "TLE6240_4", "TLE6240_5", "TLE6240_6", "TLE6240_7", "TLE6240_8", "TLE6240_9", "TLE6240_10", "TLE6240_11", "TLE6240_12", "TLE6240_13", "TLE6240_14", "TLE6240_15", "TLE6240_16"
#define HW_EVENT_TYPES 6
#define output_pin_e_enum "NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "N/A", "N/A", "PA10", "PA11", "PA12", "PA13", "N/A", "N/A", "N/A", "N/A", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "N/A", "N/A", "N/A", "N/A", "PB12", "PB13", "N/A", "N/A", "N/A", "N/A", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "N/A", "N/A", "N/A", "N/A", "PC14", "PC15", "PC16", "PC17", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "PD15", "PD16", "N/A", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "TLE6240_1", "TLE6240_2", "TLE6240_3", "TLE6240_4", "TLE6240_5", "TLE6240_6", "TLE6240_7", "TLE6240_8", "TLE6240_9", "TLE6240_10", "TLE6240_11", "TLE6240_12", "TLE6240_13", "TLE6240_14", "TLE6240_15", "TLE6240_16"
#define switch_input_pin_e_enum "NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "N/A", "N/A", "PA10", "PA11", "PA12", "PA13", "N/A", "N/A", "N/A", "N/A", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "N/A", "N/A", "N/A", "N/A", "PB12", "PB13", "N/A", "N/A", "N/A", "N/A", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "N/A", "N/A", "N/A", "N/A", "PC14", "PC15", "PC16", "PC17", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "PD15", "PD16", "N/A", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A"
#define ts_show_trigger_comparator true
// start of trigger_central_s
struct trigger_central_s {
	/**
	 * Hardware events since boot
	 * offset 0
	 */
	int hwEventCounters[HW_EVENT_TYPES];
	/**
	 * offset 24
	 */
	int vvtCamCounter = (int)0;
	/**
	 * offset 28
	 */
	scaled_channel<float, 1, 1> mapVvt_MAP_AT_SPECIAL_POINT = (float)0;
	/**
	 * offset 32
	 */
	scaled_channel<float, 1, 1> mapVvt_MAP_AT_DIFF = (float)0;
	/**
	 * offset 36
	 */
	uint8_t mapVvt_MAP_AT_CYCLE_COUNT = (uint8_t)0;
	/**
	 * offset 37
	 */
	uint8_t mapVvt_map_peak = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 38
	 */
	uint8_t alignmentFill_at_38[2];
};
static_assert(sizeof(trigger_central_s) == 40);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/trigger/trigger_central.txt Mon Aug 15 18:43:34 UTC 2022
