/**
 * @file	rusefi_enums.h
 * @brief	Fundamental rusEfi enumerable types live here
 *
 * @note this file should probably not include any other files
 *
 * @date Jan 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "efifeatures.h"
#include "obd_error_codes.h"
#include "rusefi_generated.h"
// we do not want to start the search for header from current folder so we use brackets here
// https://stackoverflow.com/questions/21593/what-is-the-difference-between-include-filename-and-include-filename
#include <rusefi_hw_enums.h>

// I believe that TunerStudio curve editor has a bug with F32 support
// because of that bug we cannot have '1.05' for 5% extra multiplier
/**
 * *0.01 because of https://sourceforge.net/p/rusefi/tickets/153/
 */

#define PERCENT_MULT 100.0f
#define PERCENT_DIV 0.01f

/**
 * http://rusefi.com/wiki/index.php?title=Manual:Engine_Type
 */
typedef enum {
	DEFAULT_FRANKENSO = ET_DEFAULT_FRANKENSO,
	/**
	 * 1995 Dodge Neon
	 * http://rusefi.com/forum/viewtopic.php?t=360
	 */
	DODGE_NEON_1995 = ET_DODGE_NEON_1995,
	/**
	 * 1996 1.3 Ford Aspire
	 * http://rusefi.com/forum/viewtopic.php?t=375
	 */
	FORD_ASPIRE_1996 = ET_FORD_ASPIRE,

	NISSAN_PRIMERA = ET_NISSAN_PRIMERA,
	HONDA_ACCORD_CD = 6,
	FORD_INLINE_6_1995 = 7,
	/**
	 * one cylinder engine
	 * 139qmb 50-90cc
	 * http://rusefi.com/forum/viewtopic.php?f=3&t=332
	 */
	GY6_139QMB = 8,

	MIATA_PROTEUS_TCU = ET_MIATA_TCU_PROTEUS,
	MAZDA_MIATA_NB1 = 9,
	MRE_MIATA_NB2_MAP = ET_MRE_MIATA_NB2_MAP,
	MRE_MIATA_NA6_VAF = ET_MRE_MIATA_NA6_VAF,
	MRE_MIATA_NB2_ETB = 13,
	MRE_MIATA_NA6_MAP = ET_MRE_MIATA_NA6_MAP,
	MRE_MIATA_NB2_MAF = ET_MRE_MIATA_NB2_MAF,

	// Frankenstein board
	MIATA_1990 = 19,
	// Frankenso board
	MIATA_NA6_MAP = ET_FRANKENSO_MIATA_NA6,
	MRE_MIATA_94_MAP = ET_MRE_MIATA_94_MAP,
	MIATA_1996 = 21,

	FORD_ESCORT_GT = ET_FORD_ESCORT_GT,


	MITSU_4G93 = 16,

	/**
	 * a version of HONDA_ACCORD_CD which only uses two of three trigger input sensors
	 */
	HONDA_ACCORD_CD_TWO_WIRES = 17,

	HONDA_ACCORD_CD_DIP = 18,


	SUBARU_2003_WRX = 22,

	/**
	 * microRusEFI used as Body Control Module BCM BCU
	 */
	MRE_BODY_CONTROL = ET_MRE_BODY_CONTROL,
	BMW_M73_M = 24,

	TEST_ENGINE = 26,

	// used by unit test
	// see https://github.com/rusefi/rusefi/issues/898
	// see TriggerWaveform::bothFrontsRequired
	TEST_ISSUE_898 = 27,

	MAZDA_626 = 28,

	SACHS = ET_SACHS,

	// LED physical order set for older test fixtures
	MRE_BOARD_OLD_TEST = 30,

	MRE_BOARD_NEW_TEST = 31,

	VW_ABA = ET_VW_ABA,

	HELLEN72_ETB = 33,

	HELLEN_NA6 = ET_HELLEN_NA6,

	CAMARO_4 = ET_CAMARO,

	HELLEN_128_MERCEDES_4_CYL = ET_HELLEN_128_MERCEDES_4_CYL,

	MRE_SUBARU_EJ18 = ET_MRE_SUBARU_EJ18,

	TOYOTA_JZS147 = 38, // 2JZ-GTE NON VVTi

	LADA_KALINA = 39,

	BMW_M73_F = ET_BMW_M73_F,

	PROTEUS_QC_TEST_BOARD = ET_PROTEUS_QC_TEST_BOARD,

	HONDA_600 = 43,

	TOYOTA_2JZ_GTE_VVTi = 44,

	TEST_ENGINE_VVT = 45,

	DODGE_NEON_2003_CRANK = 46,

	/**
	 * proper NB2 setup, 2003 red test mule car
	 */
	MAZDA_MIATA_2003 = ET_FRANKENSO_MIATA_NB2,

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
	MIATA_NA6_VAF = ET_FRANKENSO_MIATA_NA6_VAF,

	ETB_BENCH_ENGINE = 58,

	TLE8888_BENCH_ENGINE = 59,

	MICRO_RUS_EFI = ET_MRE_DEFAULTS,

	PROTEUS_DEFAULTS = 61,

	PROTEUS_ANALOG_PWM_TEST = ET_PROTEUS_ANALOG_PWM_TEST,

	VW_B6 = ET_VW_B6,

	BMW_M73_PROTEUS = ET_PROTEUS_BMW_M73,

	DODGE_RAM = 64,
	CITROEN_TU3JP = ET_CITROEN_TU3JP,

	MRE_M111 = ET_MRE_M111,

	PROTEUS_MIATA_NB2 = ET_PROTEUS_MIATA_NB2,

	HELLEN_NB2 = ET_HELLEN_NB2,

	SUBARUEG33_DEFAULTS = 70,

	HELLEN_121_VAG = ET_HELLEN_121_VAG,
	HELLEN_121_NISSAN_6_CYL = ET_HELLEN_121_NISSAN_6_CYL,
	HELLEN_55_BMW = ET_HELLEN_55_BMW,
	HELLEN_88_BMW = ET_HELLEN_88_BMW,
	HELLEN_134_BMW = ET_HELLEN_134_BMW,
	HELLEN_154_VAG = ET_HELLEN_154_VAG,

	HELLEN_121_VAG_5_CYL = ET_HELLEN_121_VAG_5_CYL,
	HELLEN_121_VAG_V6_CYL = ET_HELLEN_121_VAG_V6_CYL,
	HELLEN_121_VAG_VR6_CYL = ET_HELLEN_121_VAG_VR6_CYL,
	HELLEN_121_VAG_8_CYL = ET_HELLEN_121_VAG_8_CYL,

	HELLEN_NA94 = ET_HELLEN_NA94,

	HELLEN_154_HYUNDAI = ET_HELLEN_154_HYUNDAI,
	HELLEN_121_NISSAN_4_CYL = ET_HELLEN_121_NISSAN_4_CYL,

	HELLEN_NB2_36 = ET_HELLEN_NB2_36,

	HELLEN_128_MERCEDES_6_CYL = ET_HELLEN_128_MERCEDES_6_CYL,

	HELLEN_128_MERCEDES_8_CYL = ET_HELLEN_128_MERCEDES_8_CYL,


	/**
	 * this configuration has as few pins configured as possible
	 */
	MINIMAL_PINS = 99,
	PROMETHEUS_DEFAULTS = 100,
	SUBARUEJ20G_DEFAULTS = 101,
	VAG_18_TURBO = 102,

	TEST_33816 = 103,

	BMW_M73_MRE = 104,
	BMW_M73_MRE_SLAVE = 105,

	TEST_ROTARY = ET_TEST_ROTARY,

	TEST_108 = 108,
	TEST_109 = 109,
	TEST_110 = 110,

	Force_4_bytes_size_engine_type = ENUM_32_BITS,
} engine_type_e;


/**
 * @see http://rusefi.com/wiki/index.php?title=Manual:Software:Trigger
 */
typedef enum {

	TT_TOOTHED_WHEEL = TT_TT_TOOTHED_WHEEL,
	TT_FORD_ASPIRE = TT_TT_FORD_ASPIRE,
	TT_DODGE_NEON_1995 = 2,
	/**
	 * https://rusefi.com/wiki/index.php?title=Manual:Software:Trigger#Mazda_Miata_NA
	 */
	TT_MAZDA_MIATA_NA = TT_TT_MAZDA_MIATA_NA,
	/**
	 * NB1 means non-VVT NB, 99 and 00 1.8 engine
	 */
	TT_MAZDA_MIATA_NB1 = TT_TT_MAZDA_MIATA_NB1,
	TT_GM_7X = TT_TT_GM_7X,
	TT_MAZDA_SOHC_4 = TT_TT_MAZDA_SOHC_4,
	TT_DAIHATSU = TT_TT_DAIHATSU,
	/**
	 * "60/2"
	 * See also TT_ONE_PLUS_TOOTHED_WHEEL_60_2
	 */
	TT_TOOTHED_WHEEL_60_2 = TT_TT_TOOTHED_WHEEL_60_2,
	TT_TOOTHED_WHEEL_36_1 = TT_TT_TOOTHED_WHEEL_36_1,

	// todo: remove this weird trigger?
	TT_HONDA_4_24_1 = TT_TT_HONDA_4_24_1,

	// todo: this really looks to be same as Miata_NA shall we remove?
	TT_MITSUBISHI = 11,

	// this makes sense because mechanical spark distribution does not require synchronization
	TT_HONDA_4_24 = 12,

	TT_HONDA_1_4_24 = TT_TT_HONDA_1_4_24,

	// cam-based
	TT_DODGE_NEON_2003_CAM = 14,

	TT_MAZDA_DOHC_1_4 = TT_TT_MAZDA_DOHC_1_4,

	/**
	 * "1+1" - one tooth on primary channel, one tooth on secondary channel
	 * this trigger is used only by unit tests
	 * see also TT_ONE a bit below
	 */
	TT_ONE_PLUS_ONE = 16,
	// VVT for 2JZ
	TT_VVT_JZ = TT_TT_VVT_JZ,
	// just one channel with just one tooth
	TT_ONE = TT_TT_ONE,

	TT_DODGE_RAM = 19,
	/**
	 * It looks like this is the VR shape if you have your wires flipped
	 */
	TT_60_2_VW = TT_TT_60_2_VW,

	TT_HONDA_1_24 = TT_TT_HONDA_1_24,

	TT_DODGE_STRATUS = 22,

    /**
     * Subaru but also Mazda RX-8
     * We suspect that it's VR
     */
	TT_36_2_2_2 = TT_TT_36_2_2_2,

	/**
	 * only the 4 tooth signal, without the 360 signal
	 * 8,2,2,2 Nissan pattern
	 * See also TT_NISSAN_SR20VE_360
	 */
	TT_NISSAN_SR20VE = 24,

	TT_2JZ_3_34 = TT_TT_2JZ_3_34,

	TT_ROVER_K = 26,

	TT_GM_LS_24 = TT_TT_GM_LS_24,

	TT_HONDA_CBR_600 = 28,

	TT_2JZ_1_12 = TT_TT_2JZ_1_12,

	TT_CHRYSLER_NGC_36_2_2 = TT_TT_CHRYSLER_NGC_36_2_2,

	// skipped 3/1 with cam sensor for testing
	TT_3_1_CAM = 31,

	// crank-based in case your cam is broken
	TT_DODGE_NEON_2003_CRANK = 32,

	/**
	 * this takes care of crank sensor, VVT sensor should be configured separately
	 * for VVT simulated trigger signal we have https://github.com/rusefi/rusefi/issues/566 gap
	 * See also TT_MAZDA_MIATA_VVT_TEST
	 */
	TT_MIATA_VVT = TT_TT_MIATA_VVT,

	/**
	 * This is a different version of TT_HONDA_ACCORD_1_24
	 * See https://sourceforge.net/p/rusefi/tickets/319/
	 */
	TT_HONDA_ACCORD_1_24_SHIFTED = 34,

	/**
	 * a version of NB1 with shifted CAM, useful for VVT testing & development
	 */
	TT_MAZDA_MIATA_VVT_TEST = 35,

	TT_SUBARU_7_6 = TT_TT_SUBARU_7_6,

	// this one is 6 cylinder, see TT_JEEP_4_cyl for 4 cylinders
	TT_JEEP_18_2_2_2 = TT_TT_JEEP_18_2_2_2,

	/*
	 * See also TT_NISSAN_SR20VE
	 */
	TT_NISSAN_SR20VE_360 = 38,

	TT_DODGE_NEON_1995_ONLY_CRANK = 39,

	// Jeep XJ 2500cc 4 cylinder. See also TT_JEEP_18_2_2_2 for 6 cylinders
	TT_JEEP_4_CYL = TT_TT_JEEP_4_CYL,

	// magneti marelli Fiat/Lancia IAW P8 from the 90', 2.0 16 v turbo engine - Lancia Coupe
	// https://rusefi.com/forum/viewtopic.php?f=5&t=1440
	TT_FIAT_IAW_P8 = 41,

	TT_FORD_ST170 = TT_TT_FORD_ST170,

	/**
	 * cam sensor of Mazda Miata NB2 - the VVT signal shape
	 */
	TT_VVT_MIATA_NB2 = TT_TT_VVT_MIATA_NB2,

	TT_RENIX_44_2_2 = TT_TT_RENIX_44_2_2,

	/**
	 * Same as TT_RENIX_44_2_2 but repeated three times, not two.
	 */
	TT_RENIX_66_2_2_2 = TT_TT_RENIX_66_2_2_2,

	TT_HONDA_K_12_1 = TT_TT_HONDA_K_12_1,

	TT_VVT_BOSCH_QUICK_START = TT_TT_VVT_BOSCH_QUICK_START,

	TT_TOOTHED_WHEEL_36_2 = TT_TT_TOOTHED_WHEEL_36_2,

	TT_SUBARU_SVX = TT_TT_SUBARU_SVX,

	TT_1_16 = 50,

	// todo: remove this trigger once we have https://github.com/rusefi/rusefi/issues/2073
	TT_SUBARU_7_WITHOUT_6 = TT_TT_SUBARU_7_WITHOUT_6,

	TT_52 = TT_TT_52,

	// https://rusefi.com/forum/viewtopic.php?f=5&t=1912
	TT_TRI_TACH = TT_TT_TRI_TACH,

	TT_GM_60_2_2_2 = TT_TT_GM_60_2_2_2,

	/**
	 * https://rusefi.com/forum/viewtopic.php?f=5&t=1937
	 * HALL sensor, and can be used on all Skoda's engines (from 1000MB to 130, Favorit, Felicia)
	 */
	TT_SKODA_FAVORIT = TT_TT_SKODA_FAVORIT,

	TT_VVT_BARRA_3_PLUS_1 = TT_TT_VVT_BARRA_3_PLUS_1,

	TT_KAWA_KX450F = TT_TT_KAWA_KX450F,

	TT_NISSAN_VQ35 = TT_TT_NISSAN_VQ35,

	TT_VVT_NISSAN_VQ35 = TT_TT_VVT_NISSAN_VQ35,

	TT_NISSAN_VQ30 = TT_TT_NISSAN_VQ30,

	TT_NISSAN_QR25 = TT_TT_NISSAN_QR25,

	TT_TEMP_62 = TT_TT_TEMP_62,

	TT_SUBARU_SVX_CRANK_1 = TT_TT_SUBARU_SVX_CRANK_1,

	TT_SUBARU_SVX_CAM_VVT = TT_TT_SUBARU_SVX_CAM_VVT,

	TT_FORD_TFI_PIP = TT_TT_FORD_TFI_PIP,

	// do not forget to edit "#define trigger_type_e_enum" line in integration/rusefi_config.txt file to propogate new value to rusefi.ini TS project
	// do not forget to invoke "gen_config.bat" once you make changes to integration/rusefi_config.txt
	// todo: one day a hero would integrate some of these things into Makefile in order to reduce manual magic
	//
	// Another point: once you add a new trigger, run get_trigger_images.bat which would run rusefi_test.exe from unit_tests
	//
	TT_UNUSED = 66, // this is used if we want to iterate over all trigger types

	// todo: convert to ENUM_16_BITS? I can see 257 triggers but not 65K triggers
	Force_4_bytes_size_trigger_type = ENUM_32_BITS,
} trigger_type_e; // TriggerProcessor.java has this "trigger_type_e" name hard-coded!

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

// see also PWM_PHASE_MAX_WAVE_PER_PWM
// todo: better names?
typedef enum {
	T_PRIMARY = 0,
	T_SECONDARY = 1,
	// todo: I really do not want to call this 'tertiary'. maybe we should rename all of these?
	T_CHANNEL_3 = 2,
	T_NONE = 15
} trigger_wheel_e;

// see also 'HW_EVENT_TYPES'
typedef enum {
	SHAFT_PRIMARY_FALLING = 0,
	SHAFT_PRIMARY_RISING = 1,
	SHAFT_SECONDARY_FALLING = 2,
	SHAFT_SECONDARY_RISING = 3,
	SHAFT_3RD_FALLING = 4,
	SHAFT_3RD_RISING = 5,
} trigger_event_e;

typedef enum  __attribute__ ((__packed__)) {
	/**
	 * This mode is useful for troubleshooting and research - events are logged but no effects on phase synchronization
	 */
	VVT_INACTIVE = VM_VVT_INACTIVE,

	/**
	 * Single-tooth cam sensor mode where TDC and cam signal happen in opposite 360 degree of 720 degree engine cycle
	 */
	VVT_SECOND_HALF = 1,
	/**
	 * Toyota 2JZ has three cam tooth. We pick one of these three tooth to synchronize based on the expected angle position of the event
	 */
	VVT_2JZ = 2,
	/**
	 * Mazda NB2 has three cam tooth. We synchronize based on gap ratio.
	 * @see TT_VVT_MIATA_NB2
	 */
	VVT_MIATA_NB2 = 3,

	/**
	 * Single-tooth cam sensor mode where TDC and cam signal happen in the same 360 degree of 720 degree engine cycle
	 */
	VVT_FIRST_HALF = 4,
	/**
	 * @see TT_VVT_BOSCH_QUICK_START
	 */
	VVT_BOSCH_QUICK_START = 5,

	/**
	 * 1.8l Toyota 1ZZ-FE https://rusefi.com/forum/viewtopic.php?f=3&t=1735
	 */
	VVT_4_1 = 6,

	VVT_FORD_ST170 = 7,

	VVT_BARRA_3_PLUS_1 = 8,

	VVT_NISSAN_VQ = 9,
} vvt_mode_e;

/**
 * This enum is used to select your desired Engine Load calculation algorithm
 */
typedef enum {
	/**
	 * Speed Density algorithm - Engine Load is a function of MAP, VE and target AFR
	 * http://articles.sae.org/8539/
	 */
	LM_SPEED_DENSITY = 3,

	/**
	 * MAF with a known kg/hour function
	 */
	LM_REAL_MAF = 4,

	LM_ALPHA_N = 5,

	LM_LUA = 6,

	// This mode is for unit testing only, so that tests don't have to rely on a particular real airmass mode
	LM_MOCK = 100,

	Force_4_bytes_size_engine_load_mode = ENUM_32_BITS,
} engine_load_mode_e;

typedef enum {
	DM_NONE = 0,
	DM_HD44780 = 1,
	DM_HD44780_OVER_PCF8574 = 2,

	Force_4_bytes_size_display_mode = ENUM_32_BITS,

} display_mode_e;

typedef enum  __attribute__ ((__packed__)) {
	TL_AUTO = 0,
	TL_SEMI_AUTO = 1,
	TL_MANUAL = 2,
	TL_HALL = 3,

} tle8888_mode_e;


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

typedef enum __attribute__ ((__packed__)) {
	PI_DEFAULT = 0,
	PI_PULLUP = 1,
	PI_PULLDOWN = 2
} pin_input_mode_e;

#define CRANK_MODE_MULTIPLIER 2.0f

/**
 * @see getCycleDuration
 * @see getEngineCycle
 */
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
	 * In other words, same pattern is repeated on the crank wheel twice.
	 */
	FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR = 4,

	/**
	 * Same pattern repeated three times on crank wheel. Crazy, I know!
	 */
	FOUR_STROKE_THREE_TIMES_CRANK_SENSOR = 5,

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
	IM_WASTED_SPARK = IM_IM_WASTED_SPARK,

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
	 * Each pair is fired once per engine cycle
	 * todo: we might want to implement one additional mode where each pair of injectors is floating twice per engine cycle.
	 * todo: this could reduce phase offset from injection to stroke but would not work great for large injectors
	 */
	IM_BATCH = IM_IM_BATCH,
	/**
	 * only one injector located in throttle body
	 */
	IM_SINGLE_POINT = IM_IM_SINGLE_POINT,


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

typedef enum __attribute__ ((__packed__)) {
	UART_NONE = 0,
	UART_DEVICE_1 = 1,
	UART_DEVICE_2 = 2,
	UART_DEVICE_3 = 3,
	UART_DEVICE_4 = 4,
} uart_device_e;

typedef enum __attribute__ ((__packed__)) {
	_5MHz,
	_2_5MHz,
	_1_25MHz,
	_150KHz
} spi_speed_e;


/**
 * See spi3mosiPin
 * See spi2MisoMode
 */
typedef enum __attribute__ ((__packed__)) {
	SPI_NONE = 0,
	SPI_DEVICE_1 = 1,
	SPI_DEVICE_2 = 2,
	SPI_DEVICE_3 = 3,
	SPI_DEVICE_4 = 4,
} spi_device_e;

typedef enum {
	BMW_e46 = 0,
	W202 = 1,
	Force_4_bytes_size_can_vss_nbc_e = ENUM_32_BITS,
} can_vss_nbc_e;

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

typedef brain_pin_e output_pin_e;

/**
 * https://rusefi.com//wiki/index.php?title=Manual:Debug_fields
 */
typedef enum {
	DBG_ALTERNATOR_PID = 0,
	DBG_TPS_ACCEL = 1,
	DBG_GPPWM = 2,
	DBG_IDLE_CONTROL = 3,
	DBG_EL_ACCEL = 4,
	DBG_TRIGGER_COUNTERS = 5,
	DBG_FSIO_ADC = 6,

	DBG_VVT_1_PID = 7,
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
	DBG_16 = 16,
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
	/**
	 * ADC
	 * See also DBG_ANALOG_INPUTS2
	 */
	DBG_ANALOG_INPUTS = 21,
	
	DBG_INSTANT_RPM = 22,
	DBG_FSIO_EXPRESSION_1_7 = 23,
	DBG_STATUS = 24,
	DBG_CJ125 = 25,
	DBG_CAN = 26,
	DBG_MAP = 27,
	DBG_METRICS = 28,
	DBG_ELECTRONIC_THROTTLE_EXTRA = 29,
	DBG_ION = 30,
	DBG_TLE8888 = 31,
	/**
	 * See also DBG_ANALOG_INPUTS
	 */
	DBG_ANALOG_INPUTS2 = 32,
	DBG_DWELL_METRIC = 33,
	DBG_34 = 34,
	DBG_ETB_LOGIC = 35,
	DBG_BOOST = 36,
	DBG_START_STOP = 37,
	DBG_LAUNCH = 38,
	DBG_ETB_AUTOTUNE = 39,
	DBG_COMPOSITE_LOG = 40,
	DBG_FSIO_EXPRESSION_8_14 = 41,
	DBG_UNUSED_42 = 42,
	DBG_INJECTOR_COMPENSATION = 43,
	DBG_DYNO_VIEW = 44,
	// todo: because of getEnumOptionsForTunerStudio 'getEnumOptionsForTunerStudio' would not work here
	// todo: https://github.com/rusefi/rusefi/issues/2102
	DBG_LOGIC_ANALYZER = 45,
	DBG_RUSEFI_WIDEBAND = 46,
	DBG_TCU = 47,
	DBG_LUA = 48,
	DBG_VVT_2_PID = 49,
	DBG_VVT_3_PID = 50,
	DBG_VVT_4_PID = 51,
	MODE_52 = 52,
	MODE_53 = 53,

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
	

	/**
	 * Bosch 2.5 Bar TMap Map Sensor with IAT
	 * 20 kPa at 0.40V, 250 kPa at 4.65V
	 * 4 pin:
	 *    Pin 1 : Sensor Ground
	 *    Pin 2 : Temp Signal
	 *    Pin 3 : 5v
	 *    Pin 4 : Map Signal
	 * Volkswagen Passat B6
	 */

	MT_BOSCH_2_5 = 10,

	MT_MAZDA_1_BAR = 11,

	MT_GM_2_BAR = 12,

	MT_GM_1_BAR = 13,

	Force_4_bytes_size_cranking_map_type = ENUM_32_BITS,
} air_pressure_sensor_type_e;

typedef enum {
	SC_OFF = 0,
	/**
	 * You would use this value if you want to see a detailed graph of your trigger events
	 */
	SC_TRIGGER = 1,
	SC_MAP = 2,
	SC_RPM_ACCEL = 3,
	SC_DETAILED_RPM = 4,
	SC_AUX_FAST1 = 5,

	Internal_ForceMyEnumIntSize_sensor_chart = ENUM_32_BITS,
} sensor_chart_e;

typedef enum {
	REVERSE = -1,
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
	CAN_BUS_NBC_NONE = 0,
	CAN_BUS_NBC_FIAT = 1,
	CAN_BUS_NBC_VAG = 2,
	CAN_BUS_MAZDA_RX8 = 3,
	CAN_BUS_NBC_BMW = 4,
	CAN_BUS_W202_C180 = 5,
    CAN_BUS_BMW_E90 = 6,
	CAN_BUS_Haltech = 7,
	CAN_BUS_MQB = 8,
	CAN_BUS_NISSAN_VQ = 9,
	CAN_BUS_GENESIS_COUPE = 10,
	CAN_BUS_HONDA_K = 11,


	Internal_ForceMyEnumIntSize_can_nbc = ENUM_32_BITS,
} can_nbc_e;

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

typedef enum {
	INIT = 0,
	TPS_THRESHOLD = 1,
	RPM_DEAD_ZONE = 2,
	PID_VALUE = 4,
	PID_UPPER = 16,
	BLIP = 64,
	/**
	 * Live Docs reads 4 byte value so we want 4 byte enum
	 */
	Force_4bytes_size_idle_state_e = ENUM_32_BITS,
} idle_state_e;

typedef enum {
	OPEN_LOOP = 0,
	CLOSED_LOOP = 1,
	Force_4bytes_size_boostType_e = ENUM_32_BITS,
} boostType_e;

typedef enum {
	SWITCH_INPUT_LAUNCH = 0,
	CLUTCH_INPUT_LAUNCH = 1,
	ALWAYS_ACTIVE_LAUNCH = 2,
	Force_4bytes_size_launchActivationMode_e = ENUM_32_BITS,
} launchActivationMode_e;

typedef enum {
	SWITCH_INPUT_ANTILAG = 0,
	ALWAYS_ON_ANTILAG = 1,
	Force_4bytes_size_antiLagActivationMode_e = ENUM_32_BITS,
} antiLagActivationMode_e;

typedef enum __attribute__ ((__packed__)) {
	GPPWM_Tps = 0,
	GPPWM_Map = 1,
	GPPWM_Clt = 2,
	GPPWM_Iat = 3,
	GPPWM_FuelLoad = 4,
	GPPWM_IgnLoad = 5,
	GPPWM_AuxTemp1 = 6,
	GPPWM_AuxTemp2 = 7,
	GPPWM_Zero = 8,
	GPPWM_AccelPedal = 9,
	GPPWM_Vbatt = 10,
} gppwm_channel_e;

typedef enum __attribute__ ((__packed__)) {
	B100KBPS = 0, // 100kbps
	B250KBPS = 1, // 250kbps
	B500KBPS = 2, // 500kbps
	B1MBPS = 3, // 1Mbps
} can_baudrate_e;

typedef enum __attribute__ ((__packed__)) {
	GPPWM_GreaterThan = 0,
	GPPWM_LessThan = 1,
} gppwm_compare_mode_e;

typedef enum __attribute__ ((__packed__)) {
	VE_None = 0,
	VE_MAP = 1,
	VE_TPS = 2,
} ve_override_e;

typedef enum __attribute__ ((__packed__)) {
	AFR_None = 0,
	AFR_MAP = 1,
	AFR_Tps = 2,
	AFR_AccPedal = 3,
	AFR_CylFilling = 4,
} afr_override_e;

typedef enum __attribute__ ((__packed__)) {
	ETB_None = 0,
	ETB_Throttle1 = 1,
	ETB_Throttle2 = 2,
	ETB_IdleValve = 3,
	ETB_Wastegate = 4,
} etb_function_e;

typedef enum __attribute__ ((__packed__)) {
	IPT_Low = 0,
	IPT_High = 1,
} injector_pressure_type_e;

typedef enum __attribute__ ((__packed__)) {
	ICM_None = 0,
	ICM_FixedRailPressure = 1,
	ICM_SensedRailPressure = 2,
} injector_compensation_mode_e;

typedef enum __attribute__ ((__packed__)) {
	INJ_None = 0,
	INJ_PolynomialAdder = 1,
} InjectorNonlinearMode;
