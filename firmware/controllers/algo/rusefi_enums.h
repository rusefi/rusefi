/**
 * @file	rusefi_enums.h
 * @brief	Fundamental rusEfi enumerable types live here
 *
 * @note this file should probably not include any other files
 *
 * @date Jan 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef RUSEFI_ENUMS_H_
#define RUSEFI_ENUMS_H_

#include "efifeatures.h"

// for now I want most enums to be 32 bit integers. At some point maybe we will make the one-byte
// this is about offsets and sizes in TunerStudio
#define ENUM_32_BITS 2000000000

#define HIP_NAME "HIP"
#define TACH_NAME "tach"
#define DIZZY_NAME "dizzy"

#define ENUM_16_BITS 20000

#define DIGIPOT_COUNT 4

#define TRIGGER_SIMULATOR_PIN_COUNT 3

#define LOGIC_ANALYZER_CHANNEL_COUNT 4

// I believe that TunerStudio curve editor has a bug with F32 support
// because of that bug we cannot have '1.05' for 5% extra multiplier
#define PERCENT_MULT 100.0f

/**
 * http://rusefi.com/wiki/index.php?title=Manual:Engine_Type
 */
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

	MAZDA_MIATA_NB1 = 9,

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

	// Frankenstein board
	MIATA_1990 = 19,
	MIATA_1994_DEVIATOR = 20,
	MIATA_1996 = 21,

	SUBARU_2003_WRX = 22,

	DODGE_NEON_2003_CAM = 23,
	MIATA_1994_SPAGS = 24,

	BMW_E34 = 25,

	TEST_ENGINE = 26,

	ACURA_RSX = 27,

	MAZDA_626 = 28,

	SACHS = 29,

	GM_2_2 = 30,

	DODGE_RAM = 31,

	VW_ABA = 32,

	DODGE_STRATUS = 33,

	DAIHATSU = 34,

	CAMARO_4 = 35,

	SUZUKI_VITARA = 36,

	CHEVY_C20_1973 = 37,

	TOYOTA_JZS147 = 38, // 2JZ-GTE NON VVTi

	LADA_KALINA = 39,

	GEO_STORM = 40,

	// Frankenso board
	MIATA_NA6_MAP = 41,

	ZIL_130 = 42,

	HONDA_600 = 43,

	TOYOTA_2JZ_GTE_VVTi = 44,

	TEST_ENGINE_VVT = 45,

	DODGE_NEON_2003_CRANK = 46,

	/**
	 * proper NB2 setup, 2003 red test mule car
	 */
	MAZDA_MIATA_2003 = 47,

	HONDA_ACCORD_1_24_SHIFTED = 48,

	FRANKENSO_QA_ENGINE = 49,

	/**
	 * this is about unit-testing skipped wheel trigger
	 */
	TEST_CIVIC_4_0_BOTH = 50,

	/**
	 * this is about unit-testing skipped wheel trigger
	 */
	TEST_CIVIC_4_0_RISE = 51,


	TEST_ISSUE_366_BOTH = 52,
	TEST_ISSUE_366_RISE = 53,

	/**
	 * green Hunchback race car - VVT engine on a NA body with NA return fuel lines which
	 * means different fuel pressure situation
	 */
	MAZDA_MIATA_2003_NA_RAIL = 54,

	MAZDA_MIATA_2003_BOARD_TEST = 55,

	MAZDA_MIATA_NA8 = 56,

	// see also	MIATA_NA6_MAP = 41
	MIATA_NA6_VAF = 57,

	ETB_BENCH_ENGINE = 58,

	TLE8888_BENCH_ENGINE = 59,

	/**
	 * this configuration has as few pins configured as possible
	 */
	MINIMAL_PINS = 99,
	PROMETHEUS_DEFAULTS = 100,

	Force_4_bytes_size_engine_type = ENUM_32_BITS,
} engine_type_e;


/**
 * @see http://rusefi.com/wiki/index.php?title=Manual:Software:Trigger
 */
typedef enum {
	TT_TOOTHED_WHEEL = 0,
	TT_FORD_ASPIRE = 1,
	TT_DODGE_NEON_1995 = 2,
	TT_MAZDA_MIATA_NA = 3,
	/**
	 * NB1 means non-VVT NB, 99 and 00 1.8 engine
	 */
	TT_MAZDA_MIATA_NB1 = 4,
	TT_GM_7X = 5,
	TT_MINI_COOPER_R50 = 6,
	TT_MAZDA_SOHC_4 = 7,
	/**
	 * "60/2"
	 * See also TT_ONE_PLUS_TOOTHED_WHEEL_60_2
	 */
	TT_TOOTHED_WHEEL_60_2 = 8,
	TT_TOOTHED_WHEEL_36_1 = 9,

	TT_HONDA_4_24_1 = 10,

	TT_MITSUBISHI = 11,

	// this makes sense because mechanical spark distribution does not require synchronization
	TT_HONDA_4_24 = 12,

	TT_HONDA_1_4_24 = 13,

	// cam-based
	TT_DODGE_NEON_2003_CAM = 14,

	TT_MAZDA_DOHC_1_4 = 15,

	// "1+1"
	TT_ONE_PLUS_ONE = 16,
	// "1+60/2"
	TT_ONE_PLUS_TOOTHED_WHEEL_60_2 = 17,
	TT_ONE = 18,

	TT_DODGE_RAM = 19,
	TT_60_2_VW = 20,

	TT_HONDA_1_24 = 21,

	TT_DODGE_STRATUS = 22,

	TT_36_2_2_2 = 23,

	/**
	 * only the 4 tooth signal, without the 360 signal
	 * 8,2,2,2 Nissan pattern
	 * See also TT_NISSAN_SR20VE_360
	 */
	TT_NISSAN_SR20VE = 24,

	TT_2JZ_3_34 = 25,

	TT_ROVER_K = 26,

	TT_GM_LS_24 = 27,

	TT_HONDA_CBR_600 = 28,

	TT_2JZ_1_12 = 29,

	TT_HONDA_CBR_600_CUSTOM = 30,

	// skipped 3/1 with cam sensor for testing
	TT_3_1_CAM = 31,

	// crank-based in case your cam is broken
	TT_DODGE_NEON_2003_CRANK = 32,

	/**
	 * this takes care of crank sensor, VVT sensor should be configured separately
	 * for VVT simulated trigger signal we have https://github.com/rusefi/rusefi/issues/566 gap
	 * See also TT_MAZDA_MIATA_VVT_TEST
	 */
	TT_MIATA_VVT = 33,

	/**
	 * This is a different version of TT_HONDA_ACCORD_1_24
	 * See https://sourceforge.net/p/rusefi/tickets/319/
	 */
	TT_HONDA_ACCORD_1_24_SHIFTED = 34,

	/**
	 * a version of NB1 with shifted CAM, useful for VVT testing & development
	 */
	TT_MAZDA_MIATA_VVT_TEST = 35,

	TT_SUBARU_7_6 = 36,

	// this one is 6 cylinder, see TT_JEEP_4_cyl for 4 cylinders
	TT_JEEP_18_2_2_2 = 37,

	/*
	 * See also TT_NISSAN_SR20VE
	 */
	TT_NISSAN_SR20VE_360 = 38,

	TT_DODGE_NEON_1995_ONLY_CRANK = 39,

	// Jeep XJ 2500cc 4 cylinder. See also TT_JEEP_18_2_2_2 for 6 cylinders
	TT_JEEP_4_CYL = 40,

	// magneti marelli Fiat/Lancia IAW P8 from the 90', 2.0 16 v turbo engine - Lancia Coupe
	// https://rusefi.com/forum/viewtopic.php?f=5&t=1440
	TT_FIAT_IAW_P8 = 41,

	TT_MAZDA_Z5 = 42,

	// do not forget to edit "#define trigger_type_e_enum" line in integration/rusefi_config.txt file to propogate new value to rusefi.ini TS project
	// do not forget to invoke "gen_config.bat" once you make changes to integration/rusefi_config.txt
	// todo: one day a hero would integrate some of these things into Makefile in order to reduce manual magic
	//
	// Another point: once you add a new trigger, run get_trigger_images.bat which would run rusefi_test.exe from unit_tests
	//
	TT_UNUSED = 43, // this is used if we want to iterate over all trigger types

	Force_4_bytes_size_trigger_type = ENUM_32_BITS,
} trigger_type_e;

typedef enum {
	ADC_OFF = 0,
	ADC_SLOW = 1,
	ADC_FAST = 2,

	Force_4_bytes_size_adc_channel_mode = ENUM_32_BITS,
} adc_channel_mode_e;

typedef enum {
	TV_FALL = 0,
	TV_RISE = 1
} trigger_value_e;

// todo: better names?
typedef enum {
	T_PRIMARY = 0,
	T_SECONDARY = 1,
	// todo: I really do not want to call this 'tertiary'. maybe we should rename all of these?
	T_CHANNEL_3 = 2,
	T_NONE = 15
} trigger_wheel_e;

typedef enum {
	SHAFT_PRIMARY_FALLING = 0,
	SHAFT_PRIMARY_RISING = 1,
	SHAFT_SECONDARY_FALLING = 2,
	SHAFT_SECONDARY_RISING = 3,
	SHAFT_3RD_FALLING = 4,
	SHAFT_3RD_RISING = 5,
} trigger_event_e;

typedef enum {
	VVT_FIRST_HALF = 0,
	VVT_SECOND_HALF = 1,
	VVT_2GZ = 2,
	MIATA_NB2 = 3,
	Force_4_bytes_size_vvt_mode = ENUM_32_BITS,
} vvt_mode_e;

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
	 * Speed Density algorithm - Engine Load is a function of MAP, VE and target AFR
	 * http://articles.sae.org/8539/
	 */
	LM_SPEED_DENSITY = 3,

	/**
	 * MAF with a known kg/hour function
	 */
	LM_REAL_MAF = 4,

	Force_4_bytes_size_engine_load_mode = ENUM_32_BITS,
} engine_load_mode_e;

typedef enum {
	DM_NONE = 0,
	DM_HD44780 = 1,
	DM_HD44780_OVER_PCF8574 = 2,

	Force_4_bytes_size_display_mode = ENUM_32_BITS,

} display_mode_e;

typedef enum {
	LF_NATIVE = 0,
	/**
	 * http://www.efianalytics.com/MegaLogViewer/
	 * log example: http://svn.code.sf.net/p/rusefi/code/trunk/misc/ms_logs/
	 */
	LM_MLV = 1,

	Force_4_bytes_size_log_format = ENUM_32_BITS,
} log_format_e;

typedef enum {
	/**
	 * In auto mode we currently have some pid-like-but-not really PID logic which is trying
	 * to get idle RPM to desired value by dynamically adjusting idle valve position.
	 * TODO: convert to PID
	 */
	IM_AUTO = 0,
	/**
	 * Manual idle control is extremely simple: user just specifies desired idle valve position
	 * which could be adjusted according to current CLT
	 */
	IM_MANUAL = 1,
	Force_4_bytes_size_idle_mode = ENUM_32_BITS,
} idle_mode_e;

typedef enum __attribute__ ((__packed__)) {
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
	OM_OPENDRAIN_INVERTED = 3
} pin_output_mode_e;

// todo: migrate/unify with pin_output_mode_e? rename? something is messy here
// this enum is currently only used for SPI pins
typedef enum __attribute__ ((__packed__)) {
// todo: here we have a rare example of stm32-specific enum, todo: make this not stm32 specific?
	PO_DEFAULT = 0,
	PO_OPENDRAIN = 4, // PAL_STM32_OTYPE_OPENDRAIN
	PO_PULLUP = 32, // PAL_STM32_PUDR_PULLUP
	PO_PULLDOWN = 64 // PAL_STM32_PUPDR_PULLDOWN
} pin_mode_e;

typedef enum __attribute__ ((__packed__)) {
	PI_DEFAULT = 0,
	PI_PULLUP = 1,
	PI_PULLDOWN = 2
} pin_input_mode_e;

#define CRANK_MODE_MULTIPLIER 2.0f

// todo: better enum name
typedef enum {
	OM_NONE = 0,
	/**
	 * 720 degree engine cycle but trigger is defined using a 360 cycle which is when repeated.
	 * For historical reasons we have a pretty weird approach where one crank trigger revolution is
	 * defined as if it's stretched to 720 degress. See CRANK_MODE_MULTIPLIER
	 */
	FOUR_STROKE_CRANK_SENSOR = 1,
	/**
	 * 720 degree engine and trigger cycle
	 */
	FOUR_STROKE_CAM_SENSOR = 2,
	/**
	 * 360 degree cycle
	 */
	TWO_STROKE = 3,

	/**
	 * 720 degree engine cycle but trigger is defined using a 180 cycle which is when repeated three more times
	 */
	FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR = 4,

	Force_4_bytes_size_operation_mode_e = ENUM_32_BITS,
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

	/**
	 * some v12 engines line BMW M70 and M73 run two distributors, one for each bank of cylinders
	 */
	IM_TWO_COILS = 3,

	Force_4_bytes_size_ignition_mode = ENUM_32_BITS,
} ignition_mode_e;

/**
 * @see getNumberOfInjections
 */
typedef enum {
	/**
	 * each cylinder has it's own injector but they all works in parallel
	 */
	IM_SIMULTANEOUS = 0,
	/**
	 * each cylinder has it's own injector, each injector is wired separately
	 */
	IM_SEQUENTIAL = 1,
	/**
	 * each cylinder has it's own injector but these injectors work in pairs. Injectors could be wired in pairs or separately.
	 */
	IM_BATCH = 2,
	/**
	 * only one injector located in throttle body
	 */
	IM_SINGLE_POINT = 3,


	Force_4_bytes_size_injection_mode = ENUM_32_BITS,
} injection_mode_e;

/**
 * @brief Ignition Mode while cranking
 */
typedef enum {
	CIM_DEFAULT = 0,
	CIM_FIXED_ANGLE = 1,

	// todo: make this a one byte enum
	Force_4_bytes_size_cranking_ignition_mode = ENUM_32_BITS,
} cranking_ignition_mode_e;

typedef enum {
	UART_NONE = 0,
	UART_DEVICE_1 = 1,
	UART_DEVICE_2 = 2,
	UART_DEVICE_3 = 3,
	UART_DEVICE_4 = 4,
// todo: make this a one byte enum
	Force_4_bytes_size_uart_device = ENUM_32_BITS,
} uart_device_e;

typedef enum __attribute__ ((__packed__)) {
	_5MHz,
	_2_5MHz,
	_1_25MHz,
	_150KHz
} spi_speed_e;

typedef enum {
	SPI_NONE = 0,
	SPI_DEVICE_1 = 1,
	SPI_DEVICE_2 = 2,
	SPI_DEVICE_3 = 3,
	SPI_DEVICE_4 = 4,

	// todo: make this a one byte enum
	Force_4_bytes_size_spi_device = ENUM_32_BITS,
} spi_device_e;



/**
 * Frankenso analog #1 PC2 ADC12
 * Frankenso analog #2 PC1 ADC11
 * Frankenso analog #3 PA0 ADC0
 * Frankenso analog #4 PC3 ADC13
 * Frankenso analog #5 PA2 ADC2
 * Frankenso analog #6 PA1 ADC1
 * Frankenso analog #7 PA4 ADC4
 * Frankenso analog #8 PA3 ADC3
 * Frankenso analog #9 PA7 ADC7
 * Frankenso analog #10 PA6 ADC6
 * Frankenso analog #11 PC5 ADC15
 * Frankenso analog #12 PC4 ADC14
 */

typedef enum __attribute__ ((__packed__)) {
	EFI_ADC_0 = 0, // PA0
	EFI_ADC_1 = 1, // PA1
	EFI_ADC_2 = 2, // PA2
	EFI_ADC_3 = 3, // PA3
	EFI_ADC_4 = 4, // PA4
	EFI_ADC_5 = 5, // PA5
	EFI_ADC_6 = 6, // PA6
	EFI_ADC_7 = 7, // PA7
	EFI_ADC_8 = 8, // PB0
	EFI_ADC_9 = 9, // PB1
	EFI_ADC_10 = 10, // PC0
	EFI_ADC_11 = 11, // PC1
	EFI_ADC_12 = 12, // PC2
	EFI_ADC_13 = 13, // PC3
	EFI_ADC_14 = 14, // PC4
	EFI_ADC_15 = 15, // PC5

	// todo: bad choice of value since now we have ADC_CHANNEL_SENSOR and could end up with 17 and 18 also
	EFI_ADC_NONE = 16,
	EFI_ADC_ERROR = 17,
#if EFI_UNIT_TEST
    TEST_MAF_CHANNEL = 113,
    TEST_CLT_CHANNEL = 114,
    TEST_IAT_CHANNEL = 115,
#endif
} adc_channel_e;

typedef enum {
	MS_AUTO = 0,
	MS_ALWAYS = 1,
	MS_NEVER = 2,
	Force_4_bytes_size_mass_storage = ENUM_32_BITS,
} mass_storage_e;

typedef enum {
	ES_BPSX_D1 = 0,
	/**
	 * same as innovate LC2
	 * 0v->7.35afr, 5v->22.39
	 */
	ES_Innovate_MTX_L = 1,
	/**
	 * Same as AEM
	 * 0v->10.0afr
	 * 5v->20.0afr
	 */
	ES_14Point7_Free = 2,

	ES_NarrowBand = 3,

	ES_PLX = 4,

	ES_Custom = 5,

	ES_AEM = 6,

	Force_4_bytes_size_ego_sensor = ENUM_32_BITS,
} ego_sensor_e;

/**
 * Hardware pin. This enum is platform-specific.
 */
typedef enum __attribute__ ((__packed__)) {
	GPIO_UNASSIGNED = 0,
	GPIO_INVALID = 1,

	GPIOA_0 = 2,
	GPIOA_1 = 3,
	GPIOA_2 = 4,
	GPIOA_3 = 5,
	GPIOA_4 = 6,
	GPIOA_5 = 7,
	GPIOA_6 = 8,
	GPIOA_7 = 9,
	GPIOA_8 = 10,
	GPIOA_9 = 11,
	GPIOA_10 = 12,
	GPIOA_11 = 13,
	GPIOA_12 = 14,
	GPIOA_13 = 15,
	GPIOA_14 = 16,
	GPIOA_15 = 17,

	GPIOB_0 = 18,
	GPIOB_1 = 19,
	GPIOB_2 = 20,
	GPIOB_3 = 21,
	GPIOB_4 = 22,
	GPIOB_5 = 23,
	GPIOB_6 = 24,
	GPIOB_7 = 25,
	GPIOB_8 = 26,
	GPIOB_9 = 27,
	GPIOB_10 = 28,
	GPIOB_11 = 29,
	GPIOB_12 = 30,
	GPIOB_13 = 31,
	GPIOB_14 = 32,
	GPIOB_15 = 33,

	GPIOC_0 = 34,
	GPIOC_1 = 35,
	GPIOC_2 = 36,
	GPIOC_3 = 37,
	GPIOC_4 = 38,
	GPIOC_5 = 39,
	GPIOC_6 = 40,
	GPIOC_7 = 41,
	GPIOC_8 = 42,
	GPIOC_9 = 43,
	GPIOC_10 = 44,
	GPIOC_11 = 45,
	GPIOC_12 = 46,
	GPIOC_13 = 47,
	GPIOC_14 = 48,
	GPIOC_15 = 49,

	GPIOD_0 = 50,
	GPIOD_1 = 51,
	GPIOD_2 = 52,
	GPIOD_3 = 53,
	GPIOD_4 = 54,
	GPIOD_5 = 55,
	GPIOD_6 = 56,
	GPIOD_7 = 57,
	GPIOD_8 = 58,
	GPIOD_9 = 59,
	GPIOD_10 = 60,
	GPIOD_11 = 61,
	GPIOD_12 = 62,
	GPIOD_13 = 63,
	GPIOD_14 = 64,
	GPIOD_15 = 65,

	GPIOE_0 = 66,
	GPIOE_1 = 67,
	GPIOE_2 = 68,
	GPIOE_3 = 69,
	GPIOE_4 = 70,
	GPIOE_5 = 71,
	GPIOE_6 = 72,
	GPIOE_7 = 73,
	GPIOE_8 = 74,
	GPIOE_9 = 75,
	GPIOE_10 = 76,
	GPIOE_11 = 77,
	GPIOE_12 = 78,
	GPIOE_13 = 79,
	GPIOE_14 = 80,
	GPIOE_15 = 81,

	GPIOF_0 = 82,
	GPIOF_1 = 83,
	GPIOF_2 = 84,
	GPIOF_3 = 85,
	GPIOF_4 = 86,
	GPIOF_5 = 87,
	GPIOF_6 = 88,
	GPIOF_7 = 89,
	GPIOF_8 = 90,
	GPIOF_9 = 91,
	GPIOF_10 = 92,
	GPIOF_11 = 93,
	GPIOF_12 = 94,
	GPIOF_13 = 95,
	GPIOF_14 = 96,
	GPIOF_15 = 97,

	GPIOG_0 = 98,
	GPIOG_1 = 99,
	GPIOG_2 = 100,
	GPIOG_3 = 101,
	GPIOG_4 = 102,
	GPIOG_5 = 103,
	GPIOG_6 = 104,
	GPIOG_7 = 105,
	GPIOG_8 = 106,
	GPIOG_9 = 107,
	GPIOG_10 = 108,
	GPIOG_11 = 109,
	GPIOG_12 = 110,
	GPIOG_13 = 111,
	GPIOG_14 = 112,
	GPIOG_15 = 113,

	GPIOH_0 = 114,
	GPIOH_1 = 115,
	GPIOH_2 = 116,
	GPIOH_3 = 117,
	GPIOH_4 = 118,
	GPIOH_5 = 119,
	GPIOH_6 = 120,
	GPIOH_7 = 121,
	GPIOH_8 = 122,
	GPIOH_9 = 123,
	GPIOH_10 = 124,
	GPIOH_11 = 125,
	GPIOH_12 = 126,
	GPIOH_13 = 127,
	GPIOH_14 = 128,
	GPIOH_15 = 129,

} brain_pin_e;

/**
 * https://rusefi.com//wiki/index.php?title=Manual:Debug_fields
 */
typedef enum {
	DBG_ALTERNATOR_PID = 0,
	DBG_TPS_ACCEL = 1,
	DBG_WARMUP_ENRICH = 2,
	DBG_IDLE_CONTROL = 3,
	DBG_EL_ACCEL = 4,
	DBG_TRIGGER_INPUT = 5,
	DBG_FSIO_ADC = 6,
	/**
	 * VVT valve control often uses AUX pid #1
	 */
	DBG_AUX_PID_1 = 7,
	/**
	 * VVT position debugging - not VVT valve control. See AUX pid #1 debug for valve position.
	 */
	DBG_VVT = 8,
	DBG_CRANKING_DETAILS = 9,
	DBG_IGNITION_TIMING = 10,
	DBG_FUEL_PID_CORRECTION = 11,
	DBG_VEHICLE_SPEED_SENSOR = 12,
	DBG_SD_CARD = 13,
	DBG_SR5_PROTOCOL = 14,
	DBG_KNOCK = 15,
	DBG_TRIGGER_SYNC = 16,
	/**
	 * See also DBG_ELECTRONIC_THROTTLE_EXTRA
	 */
	DBG_ELECTRONIC_THROTTLE_PID = 17,
	DBG_EXECUTOR = 18,
	/**
	 * See tunerstudio.cpp
	 */
	DBG_BENCH_TEST = 19,
	DBG_AUX_VALVES = 20,
	DBG_ADC = 21,
	
	DBG_INSTANT_RPM = 22,
	DBG_FSIO_EXPRESSION = 23,
	DBG_STATUS = 24,
	DBG_CJ125 = 25,
	DBG_CAN = 26,
	DBG_MAP = 27,
	DBG_METRICS = 28,
	DBG_ELECTRONIC_THROTTLE_EXTRA = 29,
	DBG_ION = 30,
	DBG_TLE8888 = 31,
	DBG_32 = 32,
	DBG_33 = 33,
	DBG_34 = 34,

	Force_4_bytes_size_debug_mode_e = ENUM_32_BITS,
} debug_mode_e;

typedef enum {
	MT_CUSTOM = 0,
	MT_DENSO183 = 1,
	/**
	 * 20 to 250 kPa (2.9 to 36.3 psi) 0.2 to 4.9 V OUTPUT
	 */
	MT_MPX4250 = 2,
	MT_HONDA3BAR = 3,
	MT_DODGE_NEON_2003 = 4,
	/**
	 * 22012AA090
	 */
	MT_SUBY_DENSO = 5,
	/**
	 * 16040749
	 */
	MT_GM_3_BAR = 6,

	/**
	 * 20 to 105 kPa (2.9 to 15.2 psi) 0.3 to 4.9 V Output
	 */
	MT_MPX4100 = 7,

	/**
	 * http://rusefi.com/forum/viewtopic.php?f=3&t=906&p=18976#p18976
	 * Toyota 89420-02010
	 */
	MT_TOYOTA_89420_02010 = 8,

	/**
	 * 20 to 250 kPa (2.9 to 36.3 psi) 0.25 to 4.875 OUTPUT
	 * More precise calibration data for new NXP sensor revisions MPX4250A and MPXA4250A.
	 * For an old Freescale MPX4250D use "MT_MPX4250".
	 * See https://www.nxp.com/docs/en/data-sheet/MPX4250A.pdf
	 */
	MT_MPX4250A = 9, 
	
	Force_4_bytes_size_cranking_map_type = ENUM_32_BITS,
} air_pressure_sensor_type_e;

typedef enum {
	CD_OFF = 0,
	CD_USE_CAN1 = 1,
	CD_USE_CAN2 = 2,

	Internal_ForceMyEnumIntSize_can_device_mode = ENUM_32_BITS,
} can_device_mode_e;

typedef enum {
	SC_OFF = 0,
	/**
	 * You would use this value if you want to see a detailed graph of your trigger events
	 */
	SC_TRIGGER = 1,
	SC_MAP = 2,
	SC_RPM_ACCEL = 3,
	SC_DETAILED_RPM = 4,

	Internal_ForceMyEnumIntSize_sensor_chart = ENUM_32_BITS,
} sensor_chart_e;

typedef enum {
//todo fix enum generator  java tool to support negative	REVERSE = -1,
	NEUTRAL = 0,
	GEAR_1 = 1,
	GEAR_2 = 2,
	GEAR_3 = 3,
	GEAR_4 = 4,

} gear_e;

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

typedef enum {
    CS_OPEN = 0,
    CS_CLOSED = 1,
    CS_SWIRL_TUMBLE = 2,

	Internal_ForceMyEnumIntSize_chamber_stype = ENUM_32_BITS,
} chamber_style_e;

/**
 * Net Body Computer types
 */
typedef enum {
	CAN_BUS_NBC_BMW = 0,
	CAN_BUS_NBC_FIAT = 1,
	CAN_BUS_NBC_VAG = 2,
	CAN_BUS_MAZDA_RX8 = 3,

	Internal_ForceMyEnumIntSize_can_nbc = ENUM_32_BITS,
} can_nbc_e;

typedef enum {
	NOT_READY,
	/**
	 * the step after this one is always IS_INTEGRATING
	 * We only integrate if we have RPM
	 */
	READY_TO_INTEGRATE,
	/**
	 * the step after this one is always WAITING_FOR_ADC_TO_SKIP
	 */
	IS_INTEGRATING,
	/**
	 * the step after this one is always WAITING_FOR_RESULT_ADC
	 */
	WAITING_FOR_ADC_TO_SKIP,
	/**
	 * the step after this one is always IS_SENDING_SPI_COMMAND or READY_TO_INTEGRATE
	 */
	WAITING_FOR_RESULT_ADC,
	/**
	 * the step after this one is always READY_TO_INTEGRATE
	 */
	IS_SENDING_SPI_COMMAND,
} hip_state_e;

typedef enum {
	TCHARGE_MODE_RPM_TPS = 0,
	TCHARGE_MODE_AIR_INTERP = 1,
	Force_4bytes_size_tChargeMode_e = ENUM_32_BITS,
} tChargeMode_e;

// peak type
typedef enum {
  MINIMUM = -1,
  NOT_A_PEAK = 0,
  MAXIMUM = 1
} PidAutoTune_Peak;

// auto tuner state
typedef enum {
  AUTOTUNER_OFF = 0,
  STEADY_STATE_AT_BASELINE = 1,
  STEADY_STATE_AFTER_STEP_UP = 2,
  RELAY_STEP_UP = 4,
  RELAY_STEP_DOWN = 8,
  CONVERGED = 16,
  FAILED = 128
} PidAutoTune_AutoTunerState;


#endif /* RUSEFI_ENUMS_H_ */
