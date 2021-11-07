/*
 * @file engine_types.h
 *
 * @date Oct 23, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#pragma once

#include "rusefi_generated.h"


/**
 * http://rusefi.com/wiki/index.php?title=Manual:Engine_Type
 */
typedef enum {
	DEFAULT_FRANKENSO = 0,

	MIATA_PROTEUS_TCU = 1,

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
	 * See also PROTEUS_LUA_DEMO
	 */
	MRE_SECONDARY_CAN = 4,

	NISSAN_PRIMERA = 5,
	ET_UNUSED_6 = 6,
	FORD_INLINE_6_1995 = 7,
	/**
	 * one cylinder engine
	 * 139qmb 50-90cc
	 * http://rusefi.com/forum/viewtopic.php?f=3&t=332
	 */
	GY6_139QMB = 8,

	UNUSED9 = 9,
	UNUSED10 = 10,
	MRE_MIATA_NB2_MAP = 11,
	MRE_MIATA_NA6_VAF = 12,
	MRE_MIATA_NB2_ETB = 13,
	MRE_MIATA_NA6_MAP = 66,
	MRE_MIATA_NB2_MAF = 15,

	UNUSED_19 = 19,
	// Frankenso board
	FRANKENSO_MIATA_NA6_MAP = 41,
	MRE_MIATA_94_MAP = 20,
	MIATA_1996 = 21,

	FORD_ESCORT_GT = 14,

	MITSU_4G93 = 16,

	/**
	 * a version of HONDA_ACCORD_CD which only uses two of three trigger input sensors
	 */
	HONDA_ACCORD_CD_TWO_WIRES = 17,

	UNUSED18 = 18,


	SUBARU_2003_WRX = 22,

	/**
	 * microRusEFI used as Body Control Module BCM BCU
	 */
	MRE_BODY_CONTROL = 23,
	BMW_M73_M = 24,

	/**
	 * See also MRE_SECONDARY_CAN
	 */
	PROTEUS_LUA_DEMO = 25,

	TEST_ENGINE = 26,

	// used by unit test
	// see https://github.com/rusefi/rusefi/issues/898
	// see TriggerWaveform::bothFrontsRequired
	TEST_ISSUE_898 = 27,

	UNUSED28 = 28,

	SACHS = 29,

	UNUSED30 = 30,

	MRE_BOARD_NEW_TEST = 31,

	VW_ABA = 32,

	HELLEN72_ETB = 33,

	HELLEN_NA6 = 34,

	CAMARO_4 = 35,

	HELLEN_128_MERCEDES_4_CYL = 36,

	MRE_SUBARU_EJ18 = 37,

	TOYOTA_JZS147 = 38, // 2JZ-GTE NON VVTi

	UNUSED39 = 39,

	FRANKENSO_BMW_M73_F = 40,

	PROTEUS_QC_TEST_BOARD = 42,

	HONDA_600 = 43,

	TOYOTA_2JZ_GTE_VVTi = 44,

	TEST_ENGINE_VVT = 45,

	DODGE_NEON_2003_CRANK = 46,

	/**
	 * proper NB2 setup, 2003 red test mule car
	 */
	FRANKENSO_MAZDA_MIATA_2003 = 47,

	UNUSED_48 = 48,

	FRANKENSO_QA_ENGINE = 49,

	UNUSED_50 = 50,

	UNUSED_51 = 51,


	TEST_ISSUE_366_BOTH = 52,
	TEST_ISSUE_366_RISE = 53,

	/**
	 * green Hunchback race car - VVT engine on a NA body with NA return fuel lines which
	 * means different fuel pressure situation
	 */
	MAZDA_MIATA_2003_NA_RAIL = 54,

	MAZDA_MIATA_2003_BOARD_TEST = 55,

	FRANKENSO_MAZDA_MIATA_NA8 = 56,

	FRANKENSO_MIATA_NA6_VAF = 57,

	ETB_BENCH_ENGINE = 58,

	TLE8888_BENCH_ENGINE = 59,

	UNUSED60 = 60,

	UNUSED61 = 61,

	PROTEUS_ANALOG_PWM_TEST = 106,

	VW_B6 = 62,

	PROTEUS_BMW_M73 = 63,

	DODGE_RAM = 64,
	CITROEN_TU3JP = 65,

	PROTEUS_MIATA_NB2 = 67,
	MRE_M111 = 68,

	HELLEN_NB2 = 69,

	SUBARUEG33_DEFAULTS = 70,

	HELLEN_121_VAG_4_CYL = 71,
	HELLEN_121_NISSAN_6_CYL = 72,
	HELLEN_55_BMW = 73,
	HELLEN_88_BMW = 74,
	HELLEN_134_BMW = 75,
	HELLEN_154_VAG = 76,

	HELLEN_121_VAG_5_CYL = 77,
	HELLEN_121_VAG_V6_CYL = 78,
	HELLEN_121_VAG_VR6_CYL = 79,
	HELLEN_121_VAG_8_CYL = 81,

	HELLEN_NA94 = 80,

    // 82
	HELLEN_154_HYUNDAI_COUPE_BK1 = 82,
	HELLEN_NB1 = 83,
	// 84
	HELLEN_121_NISSAN_4_CYL = 84,

    HELLEN_121_NISSAN_8_CYL = 85,

	HELLEN_NB2_36 = 86,

	HELLEN_121_NISSAN_ALMERA_N16  = 87,

	HELLEN_128_MERCEDES_6_CYL = 88,

	HELLEN_128_MERCEDES_8_CYL = 89,

	PROTEUS_HONDA_ELEMENT_2003 = 90,

	PROTEUS_HONDA_OBD2A = 91,

	PROTEUS_VAG_80_18T = 92,

	PROTEUS_N73 = 93,

    HELLEN_NB1_36 = 94,

	HELLEN_154_HYUNDAI_COUPE_BK2 = 95,

    ET_UNUSED96 = 96,
    ET_UNUSED97 = 97,
    ET_UNUSED98 = 98,

	/**
	 * this configuration has as few pins configured as possible
	 */
	MINIMAL_PINS = 99,
	UNUSED100 = 100,
	UNUSED101 = 101,
	VAG_18_TURBO = 102,

	TEST_33816 = 103,

	BMW_M73_MRE = 104,
	BMW_M73_MRE_SLAVE = 105,

	TEST_ROTARY = 107,

	TEST_108 = 108,
	TEST_109 = 109,
	TEST_110 = 110,

	Force_4_bytes_size_engine_type = ENUM_32_BITS,
} engine_type_e;

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
	DBG_UNUSED_6 = 6,
	DBG_VVT_1_PID = 7,
	/**
	 * VVT position debugging - not VVT valve control. See AUX pid #1 debug for valve position.
	 */
	DBG_VVT = 8,
	DBG_CRANKING_DETAILS = 9,
	DBG_IGNITION_TIMING = 10,
	DBG_FUEL_PID_CORRECTION = 11,
	DBG_UNUSED12 = 12,
	DBG_SD_CARD = 13,
	DBG_SR5_PROTOCOL = 14,
	DBG_KNOCK = 15,
	DBG_WALL_WETTING = 16,
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
	UNUSED23 = 23,
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
	DBG_UNUSED41 = 41,
	DBG_UNUSED_42 = 42,
	DBG_INJECTOR_COMPENSATION = 43,
	DBG_DYNO_VIEW = 44,
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

/**
 * @see http://rusefi.com/wiki/index.php?title=Manual:Software:Trigger
 */
typedef enum {

	TT_TOOTHED_WHEEL = 0,
	TT_FORD_ASPIRE = 1,
	TT_DODGE_NEON_1995 = 2,
	/**
	 * https://rusefi.com/wiki/index.php?title=Manual:Software:Trigger#Mazda_Miata_NA
	 */
	TT_MAZDA_MIATA_NA = 3,
	/**
	 * NB1 means non-VVT NB, 99 and 00 1.8 engine
	 * TODO: remove it? Remove Miata MX5 NB1 trigger #3488
	 */
	TT_MAZDA_MIATA_NB1 = 4,
	TT_GM_7X = 5,
	TT_DAIHATSU = 6,
	TT_MAZDA_SOHC_4 = 7,
	/**
	 * "60/2"
	 * See also TT_ONE_PLUS_TOOTHED_WHEEL_60_2
	 */
	TT_TOOTHED_WHEEL_60_2 = 8,
	TT_TOOTHED_WHEEL_36_1 = 9,

	// todo: remove this weird trigger?
	TT_UNUSED_10 = 10,

	// todo: this really looks to be same as Miata_NA shall we remove?
	TT_MITSUBISHI = 11,

	// this makes sense because mechanical spark distribution does not require synchronization
	TT_HONDA_4_24 = 12,

	TT_HONDA_1_4_24 = 13,

	// cam-based
	TT_DODGE_NEON_2003_CAM = 14,

	TT_MAZDA_DOHC_1_4 = 15,

	/**
	 * "1+1" - one tooth on primary channel, one tooth on secondary channel
	 * this trigger is used only by unit tests
	 * see also TT_ONE a bit below
	 */
	TT_ONE_PLUS_ONE = 16,
	// VVT for 2JZ
	TT_VVT_JZ = 17,
	// just one channel with just one tooth
	TT_ONE = 18,

	TT_DODGE_RAM = 19,
	/**
	 * It looks like this is the VR shape if you have your wires flipped
	 */
	TT_60_2_VW = 20,

	TT_HONDA_1_24 = 21,

	TT_DODGE_STRATUS = 22,

    /**
     * Subaru but also Mazda RX-8
     * We suspect that it's VR
     */
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

	TT_CHRYSLER_NGC_36_2_2 = 30,

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

	TT_FORD_ST170 = 42,

	/**
	 * cam sensor of Mazda Miata NB2 - the VVT signal shape
	 */
	TT_VVT_MIATA_NB2 = 43,

	TT_RENIX_44_2_2 = 44,

	/**
	 * Same as TT_RENIX_44_2_2 but repeated three times, not two.
	 */
	TT_RENIX_66_2_2_2 = 45,

	TT_HONDA_K_12_1 = 46,

	TT_VVT_BOSCH_QUICK_START = 47,

	TT_TOOTHED_WHEEL_36_2 = 48,

	TT_SUBARU_SVX = 49,

	TT_1_16 = 50,

	// todo: remove this trigger once we have https://github.com/rusefi/rusefi/issues/2073
	TT_SUBARU_7_WITHOUT_6 = 51,

	TT_NISSAN_MR18_CAM_VVT = 52,

	// https://rusefi.com/forum/viewtopic.php?f=5&t=1912
	TT_TRI_TACH = 53,

	TT_GM_60_2_2_2 = 54,

	/**
	 * https://rusefi.com/forum/viewtopic.php?f=5&t=1937
	 * HALL sensor, and can be used on all Skoda's engines (from 1000MB to 130, Favorit, Felicia)
	 */
	TT_SKODA_FAVORIT = 55,

	TT_VVT_BARRA_3_PLUS_1 = 56,

	TT_KAWA_KX450F = 57,

	TT_NISSAN_VQ35 = 58,

	TT_VVT_NISSAN_VQ35 = 59,

	TT_NISSAN_VQ30 = 60,

	TT_NISSAN_QR25 = 61,

	TT_UNUSED_62 = 62,

	TT_SUBARU_SVX_CRANK_1 = 63,

	TT_SUBARU_SVX_CAM_VVT = 64,

	TT_FORD_TFI_PIP = 65,

	TT_SUZUKI_G13B = 66,

	TT_HONDA_K_4_1 = 67,

	TT_NISSAN_MR18_CRANK = 68,

	// do not forget to edit "#define trigger_type_e_enum" line in integration/rusefi_config.txt file to propogate new value to rusefi.ini TS project
	// do not forget to invoke "gen_config.bat" once you make changes to integration/rusefi_config.txt
	// todo: one day a hero would integrate some of these things into Makefile in order to reduce manual magic
	//
	// Another point: once you add a new trigger, run get_trigger_images.bat which would run rusefi_test.exe from unit_tests
	//
	// todo: before increasing from 69 to 70 we shall use the half dozen of unused IDs
	TT_UNUSED = 69, // this is used if we want to iterate over all trigger types

	// todo: convert to ENUM_16_BITS? I can see 257 triggers but not 65K triggers
	Force_4_bytes_size_trigger_type = ENUM_32_BITS,
} trigger_type_e; // TriggerProcessor.java has this "trigger_type_e" name hard-coded!


typedef enum {
	TS_DEBUG_MODE = 0,
	TS_COMMAND_1 = 1,
	TS_GRAB_TPS_CLOSED = 2,
	TS_GRAB_TPS_WOT = 3,
	TS_COMMAND_4 = 4,
	TS_COMMAND_5 = 5,
	TS_GRAB_PEDAL_UP = 6,
	TS_GRAB_PEDAL_WOT = 7,
	TS_RESET_TLE8888 = 8,
	TS_COMMAND_9 = 9,
	TS_WRITE_FLASH = 10,
	TS_COMMAND_11 = 11,
	TS_COMMAND_12 = 12,
	TS_COMMAND_13 = 13,
	TS_COMMAND_14 = 14,
	TS_COMMAND_15 = 15,
	TS_COMMAND_16 = 16,
	TS_CLEAR_WARNINGS = 17,
	TS_IGNITION_CATEGORY = 18,
	TS_INJECTOR_CATEGORY = 19,
	TS_X14 = 20,
} ts_command_e;
