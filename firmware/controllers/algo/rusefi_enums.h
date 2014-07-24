/**
 * @file	rusefi_enums.h
 * @brief	Fundamental rusEfi enumerable types live here
 *
 * @note this file should probably not include any other files
 *
 * @date Jan 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef RUSEFI_ENUMS_H_
#define RUSEFI_ENUMS_H_

#include "efifeatures.h"

// for now I want all enums to be 32 bit integers. At some point maybe we will make the one-byte
// this is about offsets and sizes in TunerStudio
#define ENUM_SIZE_HACK 2000000000

#define DIGIPOT_COUNT 4

typedef enum {
	AUDI_AAN = 1,
#if EFI_SUPPORT_DODGE_NEON
	/**
	 * 1995 Dodge Neon
	 * http://rusefi.com/forum/viewtopic.php?t=360
	 */
	DODGE_NEON_1995 = 2,
#endif /* EFI_SUPPORT_DODGE_NEON */
#if EFI_SUPPORT_FORD_ASPIRE
	/**
	 * 1996 1.3 Ford Aspire
	 * http://rusefi.com/forum/viewtopic.php?t=375
	 */
	FORD_ASPIRE_1996 = 3,
#endif /* EFI_SUPPORT_FORD_ASPIRE */
#if EFI_SUPPORT_FORD_FIESTA
	/**
	 * 36-1 toothed wheel engine
	 * http://rusefi.com/forum/viewtopic.php?t=282
	 */
	FORD_FIESTA = 4,
#endif /* EFI_SUPPORT_FORD_FIESTA */
#if EFI_SUPPORT_NISSAN_PRIMERA
	NISSAN_PRIMERA = 5,
#endif /* EFI_SUPPORT_NISSAN_PRIMERA */

	HONDA_ACCORD = 6,

	FORD_INLINE_6_1995 = 7,
/**
 * one cylinder engine
 * 139qmb 50-90cc
 * http://rusefi.com/forum/viewtopic.php?f=3&t=332
 */
	GY6_139QMB = 8,

	MAZDA_MIATA_NB = 9,

	ROVER_V8 = 10,

	MAZDA_323 = 11,

	SATURN_ION_2004 = 12,

	MINI_COOPER_R50 = 13,

	FORD_ESCORT_GT = 14,

	CITROEN_TU3JP = 15,

	Internal_ForceMyEnumIntSize_engine_type = ENUM_SIZE_HACK,
} engine_type_e;

typedef enum {
	TT_TOOTHED_WHEEL = 0,
	TT_FORD_ASPIRE = 1,
	TT_DODGE_NEON = 2,
	TT_MAZDA_MIATA_NA = 3,
	TT_MAZDA_MIATA_NB = 4,
	TT_GM_7X = 5,
	TT_MINI_COOPER_R50 = 6,
	TT_FORD_ESCORT_GT = 7,
	TT_TOOTHED_WHEEL_60_2 = 8,
	TT_TOOTHED_WHEEL_36_1 = 9,

	Internal_ForceMyEnumIntSize_trigger_type = ENUM_SIZE_HACK,
} trigger_type_e;

typedef enum {
	ADC_OFF = 0,
	ADC_SLOW = 1,
	ADC_FAST = 2,

	Internal_ForceMyEnumIntSize_adc_channel_mode = ENUM_SIZE_HACK,
} adc_channel_mode_e;

typedef enum {
	SHAFT_PRIMARY_UP = 0,
	SHAFT_PRIMARY_DOWN = 1,
	SHAFT_SECONDARY_UP = 2,
	SHAFT_SECONDARY_DOWN = 3,
} trigger_event_e;

/**
 * This enum is used to select your desired Engine Load calculation algorithm
 */
typedef enum {
	/**
	 * raw Mass Air Flow sensor value algorithm. http://en.wikipedia.org/wiki/Mass_flow_sensor
	 */
	LM_MAF = 0,
	/**
	 * Throttle Position Sensor value is used as engine load. http://en.wikipedia.org/wiki/Throttle_position_sensor
	 * That's know as Alpha N
	 */
	LM_TPS = 1,
	/**
	 * raw Manifold Absolute Pressure sensor value is used as engine load http://en.wikipedia.org/wiki/MAP_sensor
	 */
	LM_MAP = 2,
	/**
	 * Speed Density algorithm - Engile Load is a function of MAP and ... TODO
	 * http://articles.sae.org/8539/
	 */
	LM_SPEED_DENSITY = 3,

	Internal_ForceMyEnumIntSize_engine_load_mode = ENUM_SIZE_HACK,
} engine_load_mode_e;

typedef enum {
	DM_NONE = 0,
	DM_HD44780 = 1,
	DM_HD44780_OVER_PCF8574 = 2,

	Internal_ForceMyEnumIntSize_display_mode = ENUM_SIZE_HACK,

} display_mode_e;

typedef enum {
	LF_NATIVE = 0,
	/**
	 * http://www.efianalytics.com/MegaLogViewer/
	 * log example: http://svn.code.sf.net/p/rusefi/code/trunk/misc/ms_logs/
	 */
	LM_MLV = 1,

	Internal_ForceMyEnumIntSize_log_format = ENUM_SIZE_HACK,
} log_format_e;

typedef enum {
	/**
	 * GND for logical OFF, VCC for logical ON
	 */
	OM_DEFAULT = 0,
	/**
	 * GND for logical ON, VCC for logical OFF
	 */
	OM_INVERTED = 1,
	/**
	 * logical OFF is floating, logical ON is GND
	 */
	OM_OPENDRAIN = 2,
	OM_OPENDRAIN_INVERTED = 3,

	Internal_ForceMyEnumIntSize_pin_output_mode = ENUM_SIZE_HACK,
} pin_output_mode_e;

typedef enum {
	PI_DEFAULT = 0,

	Internal_ForceMyEnumIntSize_pin_input_mode = ENUM_SIZE_HACK,
} pin_input_mode_e;

typedef enum {
	FO_ONE_CYLINDER = 0,
	FO_1_THEN_3_THEN_4_THEN2 = 1,
	FO_1_THEN_2_THEN_4_THEN3 = 2,
	FO_1_THEN_3_THEN_2_THEN4 = 3,
	FO_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4 = 4,
	FO_1_8_4_3_6_5_7_2 = 5,
	Internal_ForceMyEnumIntSize_firing_order = ENUM_SIZE_HACK,
} firing_order_e;

// todo: better enum name
typedef enum {
	OM_NONE = 0,
	FOUR_STROKE_CRANK_SENSOR = 1,
	FOUR_STROKE_CAM_SENSOR = 2,

	Internal_ForceMyEnumIntSize_operation_mode_e = ENUM_SIZE_HACK,
} operation_mode_e;

/**
 * @brief Ignition Mode
 */
typedef enum {
	/**
	 * in this mode only SPARKOUT_1_OUTPUT is used
	 */
	IM_ONE_COIL = 0,
	/**
	 * in this mode we use as many coils as we have cylinders
	 */
	IM_INDIVIDUAL_COILS = 1,
	IM_WASTED_SPARK = 2,

	Internal_ForceMyEnumIntSize_ignition_mode = ENUM_SIZE_HACK,
} ignition_mode_e;

typedef enum {
	IM_SIMULTANEOUS = 0,
	IM_SEQUENTIAL = 1,
	IM_BATCH = 2,

	Internal_ForceMyEnumIntSize_injection_mode = ENUM_SIZE_HACK,
} injection_mode_e;

/**
 * @brief Ignition Mode while cranking
 */
typedef enum {
	CIM_DEFAULT = 0,
	CIM_FIXED_ANGLE = 1,

	Internal_ForceMyEnumIntSize_cranking_ignition_mode = ENUM_SIZE_HACK,
} cranking_ignition_mode_e;

typedef enum {
	SPI_NONE = 0,
	SPI_DEVICE_1 = 1,
	SPI_DEVICE_2 = 2,
	SPI_DEVICE_3 = 3,
	SPI_DEVICE_4 = 4,

	Internal_ForceMyEnumIntSize_spi_device = ENUM_SIZE_HACK,
} spi_device_e;

typedef enum {
	IE_NO_ERROR = 0,
	IE_UNEXPECTED_FIRING_ORDER = 1,

	Internal_ForceMyEnumIntSize_cranking_internal_error = ENUM_SIZE_HACK,
} internal_error_e;

/**
 * Hardware pin. This enum is platform-specific.
 */
typedef enum {
	GPIOA_0 = 0,
	GPIOA_1 = 1,
	GPIOA_2 = 2,
	GPIOA_3 = 3,
	GPIOA_4 = 4,
	GPIOA_5 = 5,
	GPIOA_6 = 6,
	GPIOA_7 = 7,
	GPIOA_8 = 8,
	GPIOA_9 = 9,
	GPIOA_10 = 10,
	GPIOA_11 = 11,
	GPIOA_12 = 12,
	GPIOA_13 = 13,
	GPIOA_14 = 14,
	GPIOA_15 = 15,

	GPIOB_0 = 16,
	GPIOB_1 = 17,
	GPIOB_2 = 18,
	GPIOB_3 = 19,
	GPIOB_4 = 20,
	GPIOB_5 = 21,
	GPIOB_6 = 22,
	GPIOB_7 = 23,
	GPIOB_8 = 24,
	GPIOB_9 = 25,
	GPIOB_10 = 26,
	GPIOB_11 = 27,
	GPIOB_12 = 28,
	GPIOB_13 = 29,
	GPIOB_14 = 30,
	GPIOB_15 = 31,

	GPIOC_0 = 32,
	GPIOC_1 = 33,
	GPIOC_2 = 34,
	GPIOC_3 = 35,
	GPIOC_4 = 36,
	GPIOC_5 = 37,
	GPIOC_6 = 38,
	GPIOC_7 = 39,
	GPIOC_8 = 40,
	GPIOC_9 = 41,
	GPIOC_10 = 42,
	GPIOC_11 = 43,
	GPIOC_12 = 44,
	GPIOC_13 = 45,
	GPIOC_14 = 46,
	GPIOC_15 = 47,

	GPIOD_0 = 48,
	GPIOD_1 = 49,
	GPIOD_2 = 50,
	GPIOD_3 = 51,
	GPIOD_4 = 52,
	GPIOD_5 = 53,
	GPIOD_6 = 54,
	GPIOD_7 = 55,
	GPIOD_8 = 56,
	GPIOD_9 = 57,
	GPIOD_10 = 58,
	GPIOD_11 = 59,
	GPIOD_12 = 60,
	GPIOD_13 = 61,
	GPIOD_14 = 62,
	GPIOD_15 = 63,

	GPIOE_0 = 64,
	GPIOE_1 = 65,
	GPIOE_2 = 66,
	GPIOE_3 = 67,
	GPIOE_4 = 68,
	GPIOE_5 = 69,
	GPIOE_6 = 70,
	GPIOE_7 = 71,
	GPIOE_8 = 72,
	GPIOE_9 = 73,
	GPIOE_10 = 74,
	GPIOE_11 = 75,
	GPIOE_12 = 76,
	GPIOE_13 = 77,
	GPIOE_14 = 78,
	GPIOE_15 = 79,

	GPIO_NONE = 80,

	Internal_ForceMyEnumIntSize_cranking_brain_pin = ENUM_SIZE_HACK,
} brain_pin_e;

typedef enum {
	MT_CUSTOM = 0,
	MT_DENSO183 = 1,
	MT_MPX4250 = 2,
	MT_HONDA3BAR = 3,

	Internal_ForceMyEnumIntSize_cranking_map_type = ENUM_SIZE_HACK,
} air_pressure_sensor_type_e;

#endif /* RUSEFI_ENUMS_H_ */
