/**
 * @file	rusefi_enums.h
 * @brief	Fundamental rusEfi enumerable types live here
 *
 * @note this file should probably not include any other files
 *
 * @date Jan 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef RUSEFI_ENUMS_H_
#define RUSEFI_ENUMS_H_

#include "efifeatures.h"

// for now I want most enums to be 32 bit integers. At some point maybe we will make the one-byte
// this is about offsets and sizes in TunerStudio
#define ENUM_32_BITS 2000000000

#define ENUM_16_BITS 20000

#define DIGIPOT_COUNT 4

#define TRIGGER_SIMULATOR_PIN_COUNT 3

#define LOGIC_ANALYZER_CHANNEL_COUNT 4

typedef enum {
	CUSTOM_ENGINE = 0,
	AUDI_AAN = 1,
	/**
	 * 1995 Dodge Neon
	 * http://rusefi.com/forum/viewtopic.php?t=360
	 */
	DODGE_NEON_1995 = 2,
	/**
	 * 1996 1.3 Ford Aspire
	 * http://rusefi.com/forum/viewtopic.php?t=375
	 */
	FORD_ASPIRE_1996 = 3,
	/**
	 * 36-1 toothed wheel engine
	 * http://rusefi.com/forum/viewtopic.php?t=282
	 */
	FORD_FIESTA = 4,
	NISSAN_PRIMERA = 5,
	HONDA_ACCORD_CD = 6,
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

	MITSU_4G93 = 16,

	/**
	 * a version of HONDA_ACCORD_CD which only uses two of three trigger input sensors
	 */
	HONDA_ACCORD_CD_TWO_WIRES = 17,

	HONDA_ACCORD_CD_DIP = 18,

	MIATA_1990 = 19,
	MIATA_1994_DEVIATOR = 20,
	MIATA_1996 = 21,

	SUBARU_2003_WRX = 22,

	DODGE_NEON_2003 = 23,
	MIATA_1994_SPAGS = 24,

	BMW_E34 = 25,

	TEST_ENGINE = 26,

	ACURA_RSX = 27,

	MAZDA_626 = 28,

	SACHS = 29,

	ET_UNUSED = 30,

	Force_4b_engine_type = ENUM_32_BITS,
} engine_type_e;

#define DEFAULT_ENGINE_TYPE CUSTOM_ENGINE

typedef enum {
	TT_TOOTHED_WHEEL = 0,
	TT_FORD_ASPIRE = 1,
	TT_DODGE_NEON_1995 = 2,
	TT_MAZDA_MIATA_NA = 3,
	TT_MAZDA_MIATA_NB = 4,
	TT_GM_7X = 5,
	TT_MINI_COOPER_R50 = 6,
	TT_MAZDA_SOHC_4 = 7,
	TT_TOOTHED_WHEEL_60_2 = 8,
	TT_TOOTHED_WHEEL_36_1 = 9,

	TT_HONDA_ACCORD_CD = 10,

	TT_MITSU = 11,

	TT_HONDA_ACCORD_CD_TWO_WIRES = 12,

	TT_HONDA_ACCORD_CD_DIP = 13,

	TT_DODGE_NEON_2003 = 14,

	TT_MAZDA_DOHC_1_4 = 15,

	TT_ONE_PLUS_ONE = 16,

	Force_4b_trigger_type = ENUM_32_BITS,
} trigger_type_e;

typedef enum {
	ADC_OFF = 0,
	ADC_SLOW = 1,
	ADC_FAST = 2,

	Force_4b_adc_channel_mode = ENUM_32_BITS,
} adc_channel_mode_e;

// todo: better names?
typedef enum {
	TV_LOW = 0,
	TV_HIGH = 1
} trigger_value_e;

// todo: better names?
typedef enum {
	T_PRIMARY = 0,
	T_SECONDARY = 1,
	// todo: I really do not want to call this 'tertiary'. maybe we should rename all of these?
	T_CHANNEL_3 = 2
} trigger_wheel_e;

// todo: better names?
typedef enum {
	SHAFT_PRIMARY_DOWN = 0,
	SHAFT_PRIMARY_UP = 1,
	SHAFT_SECONDARY_DOWN = 2,
	SHAFT_SECONDARY_UP = 3,
	SHAFT_3RD_DOWN = 4,
	SHAFT_3RD_UP = 5,
} trigger_event_e;

/**
 * This enum is used to select your desired Engine Load calculation algorithm
 */
typedef enum {
	/**
	 * raw Mass Air Flow sensor value algorithm. http://en.wikipedia.org/wiki/Mass_flow_sensor
	 */
	LM_PLAIN_MAF = 0,
	/**
	 * Throttle Position Sensor value is used as engine load. http://en.wikipedia.org/wiki/Throttle_position_sensor
	 */
	LM_ALPHA_N = 1,
	/**
	 * raw Manifold Absolute Pressure sensor value is used as engine load http://en.wikipedia.org/wiki/MAP_sensor
	 */
	LM_MAP = 2,
	/**
	 * Speed Density algorithm - Engile Load is a function of MAP and ... TODO
	 * http://articles.sae.org/8539/
	 */
	LM_SPEED_DENSITY = 3,

	/**
	 * MAF with a known kg/hour function
	 */
	LM_REAL_MAF = 4,

	Force_4b_engine_load_mode = ENUM_32_BITS,
} engine_load_mode_e;

typedef enum {
	DM_NONE = 0,
	DM_HD44780 = 1,
	DM_HD44780_OVER_PCF8574 = 2,

	Force_4b_display_mode = ENUM_32_BITS,

} display_mode_e;

typedef enum {
	LF_NATIVE = 0,
	/**
	 * http://www.efianalytics.com/MegaLogViewer/
	 * log example: http://svn.code.sf.net/p/rusefi/code/trunk/misc/ms_logs/
	 */
	LM_MLV = 1,

	Force_4b_log_format = ENUM_32_BITS,
} log_format_e;


typedef enum {
	IM_AUTO = 0,
	IM_MANUAL = 1,
	Force_4b_idle_mode = ENUM_32_BITS,
} idle_mode_e;

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

	Force_4b_pin_output_mode = ENUM_32_BITS,
} pin_output_mode_e;

typedef enum {

	Force_4b_gpio_mode = ENUM_32_BITS,
} gpio_mode_e;

typedef enum {
	PI_DEFAULT = 0,
	PI_PULLUP = 1,
	PI_PULLDOWN = 2,

	Force_4b_pin_input_mode = ENUM_32_BITS,
} pin_input_mode_e;

typedef enum {
	FO_ONE_CYLINDER = 0,
	// 4 cylinder
	FO_1_THEN_3_THEN_4_THEN2 = 1,
	FO_1_THEN_2_THEN_4_THEN3 = 2,
	FO_1_THEN_3_THEN_2_THEN4 = 3,
	// 8 cylinder
	FO_1_8_4_3_6_5_7_2 = 5,
	// 5 cylinder
	FO_1_2_4_5_3 = 6,

	// 6 cylinder
	FO_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4 = 4,
	FO_1_THEN_4_THEN_2_THEN_5_THEN_3_THEN_6 = 7,


	Force_4b_firing_order = ENUM_32_BITS,
} firing_order_e;

// todo: better enum name
typedef enum {
	OM_NONE = 0,
	FOUR_STROKE_CRANK_SENSOR = 1,
	FOUR_STROKE_CAM_SENSOR = 2,

	Force_4b_operation_mode_e = ENUM_32_BITS,
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

	Force_4b_ignition_mode = ENUM_32_BITS,
} ignition_mode_e;

typedef enum {
	IM_SIMULTANEOUS = 0,
	IM_SEQUENTIAL = 1,
	IM_BATCH = 2,

	Force_4b_injection_mode = ENUM_32_BITS,
} injection_mode_e;

/**
 * @brief Ignition Mode while cranking
 */
typedef enum {
	CIM_DEFAULT = 0,
	CIM_FIXED_ANGLE = 1,

	Force_4b_cranking_ignition_mode = ENUM_32_BITS,
} cranking_ignition_mode_e;

typedef enum {
	SPI_NONE = 0,
	SPI_DEVICE_1 = 1,
	SPI_DEVICE_2 = 2,
	SPI_DEVICE_3 = 3,
	SPI_DEVICE_4 = 4,

	Force_4b_spi_device = ENUM_32_BITS,
} spi_device_e;

typedef enum {
	IE_NO_ERROR = 0,
	IE_UNEXPECTED_FIRING_ORDER = 1,

	Force_4b_cranking_internal_error = ENUM_32_BITS,
} internal_error_e;

typedef enum {
	EFI_ADC_0 = 0,
	EFI_ADC_1 = 1,
	EFI_ADC_2 = 2,
	EFI_ADC_3 = 3,
	EFI_ADC_4 = 4,
	EFI_ADC_5 = 5,
	EFI_ADC_6 = 6,
	EFI_ADC_7 = 7,
	EFI_ADC_8 = 8,
	EFI_ADC_9 = 9,
	EFI_ADC_10 = 10,
	EFI_ADC_11 = 11,
	EFI_ADC_12 = 12,
	EFI_ADC_13 = 13,
	EFI_ADC_14 = 14,
	EFI_ADC_15 = 15,


	EFI_ADC_NONE = 888,
	EFI_ADC_ERROR = 999,

	Force_4b_cranking_adc_channel = ENUM_32_BITS,
} adc_channel_e;


typedef enum {
	ES_BPSX_D1 = 0,
	ES_Innovate_MTX_L = 1,
	ES_14Point7_Free = 2,

	Force_4b_ego_sensor = ENUM_32_BITS,
} ego_sensor_e;



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

//	GPIOF_0 = 80,
//	GPIOF_1 = 81,
//	GPIOF_2 = 82,
//	GPIOF_3 = 83,
//	GPIOF_4 = 84,
//	GPIOF_5 = 85,
//	GPIOF_6 = 86,
//	GPIOF_7 = 87,
//	GPIOF_8 = 88,
//	GPIOF_9 = 89,
//	GPIOF_10 = 90,
//	GPIOF_11 = 91,
//	GPIOF_12 = 92,
//	GPIOF_13 = 93,
//	GPIOF_14 = 94,
//	GPIOF_15 = 95,
//
//	GPIOG_0 = 96,
//	GPIOG_1 = 97,
//	GPIOG_2 = 98,
//	GPIOG_3 = 99,
//	GPIOG_4 = 100,
//	GPIOG_5 = 101,
//	GPIOG_6 = 102,
//	GPIOG_7 = 103,
//	GPIOG_8 = 104,
//	GPIOG_9 = 105,
//	GPIOG_10 = 106,
//	GPIOG_11 = 107,
//	GPIOG_12 = 108,
//	GPIOG_13 = 109,
//	GPIOG_14 = 110,
//	GPIOG_15 = 111,
//
//	GPIOH_0 = 112,
//	GPIOH_1 = 113,
//	GPIOH_2 = 114,
//	GPIOH_3 = 115,
//	GPIOH_4 = 116,
//	GPIOH_5 = 117,
//	GPIOH_6 = 118,
//	GPIOH_7 = 119,
//	GPIOH_8 = 120,
//	GPIOH_9 = 121,
//	GPIOH_10 = 122,
//	GPIOH_11 = 123,
//	GPIOH_12 = 124,
//	GPIOH_13 = 125,
//	GPIOH_14 = 126,
//	GPIOH_15 = 128,

	GPIO_UNASSIGNED = 80,
	GPIO_INVALID = 81,

	Force_4b_cranking_brain_pin = ENUM_32_BITS,
} brain_pin_e;

typedef enum {
	MT_CUSTOM = 0,
	MT_DENSO183 = 1,
	MT_MPX4250 = 2,
	MT_HONDA3BAR = 3,
	MT_DODGE_NEON_2003 = 4,

	Force_4b_cranking_map_type = ENUM_32_BITS,
} air_pressure_sensor_type_e;

typedef enum {
	CD_OFF = 0,
	CD_USE_CAN1 = 1,
	CD_USE_CAN2 = 2,

	Internal_ForceMyEnumIntSize_can_device_mode = ENUM_32_BITS,
} can_device_mode_e;

typedef enum {
	AC_OFF = 0,
	/**
	 * You would use this value if you want to see a detailed graph of your trigger events
	 */
	AC_TRIGGER = 1, AC_MAP = 2,

	Internal_ForceMyEnumIntSize_analog_chart = ENUM_32_BITS,
} analog_chart_e;

typedef enum {
	CUSTOM = 0,
	Bosch0280218037 = 1,
	Bosch0280218004 = 2,
	DensoTODO = 3,
	Internal_ForceMyEnumIntSize_maf_sensor = ENUM_32_BITS,
} maf_sensor_type_e;

typedef enum {
	/**
	 * This is the default mode in which ECU controls timing dynamically
	 */
	TM_DYNAMIC = 0,
	/**
	 * Fixed timing is useful while you are playing with a timing gun - you need to have fixed
	 * timing if you want to install your distributor at some specific angle
	 */
	TM_FIXED = 1,

	Internal_ForceMyEnumIntSize_timing_mode = ENUM_32_BITS,
} timing_mode_e;

#endif /* RUSEFI_ENUMS_H_ */
