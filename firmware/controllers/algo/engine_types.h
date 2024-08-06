/*
 * @file engine_types.h
 *
 * See 'integration API' note in rusefi_enums.h
 * TODO: spell out which enums belong here and which enums belong in rusefi_enums.h for what reasons
 *
 * @date Oct 23, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#pragma once

#include "generated_lookup_meta.h"

/**
 * https://github.com/rusefi/rusefi/wiki/Engine-Types
 */
enum class engine_type_e : uint16_t {
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
	HARLEY = 6,
	FORD_INLINE_6_1995 = 7,
	/**
	 * one cylinder engine
	 * 139qmb 50-90cc
	 * http://rusefi.com/forum/viewtopic.php?f=3&t=332
	 */
	GY6_139QMB = 8,

	POLARIS_RZR = 9,
	MAZDA_MIATA_NA96 = 10,
	FORD_COYOTE = 11,
	MITSUBISHI_3A92 = 12,
	TOYOTA_1NZ_FE = 13,

	FORD_ESCORT_GT = 14,
	FUEL_BENCH = 15,
	MITSUBISHI_4G93 = 16,
	TCU_4R70W = 17,

	TEST_33816 = 18,

	TEST_ROTARY = 19,
	// Frankenso board
	FRANKENSO_MIATA_NA6_MAP = 41,
	MAZDA_MIATA_NC = 20,
	ET_BOSCH_QUICK_START = 21,

  // BMW e36 etc
	BMW_M52 = 22,

	/**
	 * microRusEFI used as Body Control Module BCM BCU
	 */
	MRE_BODY_CONTROL = 23,
	ET_TEST_WITH_BOSCH_QUICK_START = 24,

	/**
	 * See also MRE_SECONDARY_CAN
	 */
	PROTEUS_LUA_DEMO = 25,

	TEST_ENGINE = 26,

	PROTEUS_GM_LS_4 = 27,

	TEST_CRANK_ENGINE = 28,

  // two stroke
	SACHS = 29,

	PROTEUS_ANALOG_PWM_TEST = 30,

	MRE_BOARD_NEW_TEST = 31,

	VW_ABA = 32,

	ME17_9_MISC = 33,

	MAZDA_MIATA_NA6 = 34,

	POLARIS = 35,

	HELLEN_128_MERCEDES_4_CYL = 36,

	MRE_SUBARU_EJ18 = 37,

	DISCOVERY_PDM = 38,

	PROTEUS_VW_B6 = 39,

  // used by unit tests and HW CI
	FRANKENSO_BMW_M73_F = 40,

	PROTEUS_QC_TEST_BOARD = 42,

	HONDA_600 = 43,

	TOYOTA_2JZ_GTE_VVTi = 44,

	TEST_ENGINE_VVT = 45,

	DODGE_NEON_2003_CRANK = 46,

	UNUSED47 = 47,

    // todo: add wastegate case into HW CI
	TEST_DC_WASTEGATE_DISCOVERY = 48,

	FRANKENSO_QA_ENGINE = 49,

	BMW_M73_MRE = 50,

	BMW_M73_MRE_SLAVE = 51,


	TEST_ISSUE_366_BOTH = 52,
	TEST_ISSUE_366_RISE = 53,
	MAVERICK_X3 = 54,
	ET_UNUSED_55 = 55,
	FRANKENSO_TEST_TLE9104 = 56,
	HELLEN_HONDA_BCM = 57,
	ETB_BENCH_ENGINE = 58,
	FRANKENSO_TEST_33810 = 59,

	L9779_BENCH_ENGINE = 60,
	EEPROM_BENCH_ENGINE = 61,
	MRE_VW_B6 = 62,
	PROTEUS_BMW_M73 = 63,
	DODGE_RAM = 64,
	UNUSED_65 = 65,
	HONDA_OBD1 = 66,
	UNUSED67 = 67,
	MERCEDES_M111 = 68,

	MAZDA_MIATA_NB2 = 69,

	SUBARU_EG33 = 70,

	HELLEN_121_VAG_4_CYL = 71,
	HELLEN_121_NISSAN_6_CYL = 72,
	PROTEUS_STIM_QC = 73,
	HELLEN_4CHAN_STIM_QC = 74,
	HELLEN_2CHAN_STIM_QC = 75,
	HELLEN_154_VAG = 76,

	HELLEN_121_VAG_5_CYL = 77,
	HELLEN_121_VAG_V6_CYL = 78,
	HELLEN_121_VAG_VR6_CYL = 79,
	HELLEN_121_VAG_8_CYL = 81,

	MAZDA_MIATA_NA94 = 80,

    // 82
	HELLEN_154_HYUNDAI_COUPE_BK1 = 82,
	MAZDA_MIATA_NB1 = 83,
	// 84
	HELLEN_121_NISSAN_4_CYL = 84,

    HELLEN_121_NISSAN_8_CYL = 85,

	MAZDA_MIATA_NB2_36 = 86,

	HELLEN_121_NISSAN_ALMERA_N16  = 87,

	HELLEN_128_MERCEDES_6_CYL = 88,

	HELLEN_128_MERCEDES_8_CYL = 89,

	HONDA_K = 90,

	HONDA_OBD2A = 91,

	SIMULATOR_CONFIG = 92,

	TEST_ISSUE_6451 = 93,

  UNUSED94 = 94,

	HELLEN_154_HYUNDAI_COUPE_BK2 = 95,

    WASTEGATE_PROTEUS_TEST = 96,
    UNUSED_97 = 97,
    GM_SBC = 98,

	/**
	 * this configuration has as few pins configured as possible
	 */
	MINIMAL_PINS = 99,

    // use this in case of emergency without github actions? :)
	TEST_100 = 100,
	TEST_101 = 101,
	UNUSED102 = 102,

	PROTEUS_NISSAN_VQ35 = 103,

    // both Proteus and 4chan/8chan
	HYUNDAI_PB = 104,
  FERRARI_F136 = 105,
};

/**
 * https://rusefi.com//wiki/index.php?title=Manual:Debug_fields
 */
typedef enum __attribute__ ((__packed__)) {
	DBG_0 = 0,
	DBG_1 = 1,
	DBG_2 = 2,
	DBG_STEPPER_IDLE_CONTROL = 3,
	DBG_EL_ACCEL = 4,
	DBG_5 = 5,
	DBG_6 = 6,
	DBG_7 = 7,
	DBG_8 = 8,
	DBG_9 = 9,
	DBG_10 = 10,
	DBG_11 = 11,
	DBG_12 = 12,
	DBG_13 = 13,
	DBG_SR5_PROTOCOL = 14,
	DBG_KNOCK = 15,
	DBG_16 = 16,
	/**
	 * See also DBG_ELECTRONIC_THROTTLE_EXTRA
	 */
	DBG_ELECTRONIC_THROTTLE_PID = 17,
	DBG_EXECUTOR = 18,

	DBG_19 = 19,
	DBG_20 = 20,
	DBG_21 = 21,

	DBG_22 = 22,
	UNUSED23 = 23,
	DBG_24 = 24,
	DBG_25 = 25,
	DBG_26 = 26,
	DBG_27 = 27,
	DBG_METRICS = 28,
	DBG_29 = 29,
	DBG_ION = 30,
	DBG_TLE8888 = 31,
	DBG_32 = 32,
	DBG_DWELL_METRIC = 33,
	DBG_34 = 34,
	DBG_35 = 35,
	DBG_36 = 36,
	DBG_37 = 37,
	DBG_38 = 38,
	DBG_ETB_AUTOTUNE = 39,
	DBG_COMPOSITE_LOG = 40,
	DBG_UNUSED41 = 41,
	DBG_UNUSED_42 = 42,
	DBG_43 = 43,
	DBG_DYNO_VIEW = 44,
	DBG_LOGIC_ANALYZER = 45,
	DBG_46 = 46,
	DBG_TCU = 47,
	DBG_LUA = 48,

} debug_mode_e;

/**
 * @see http://rusefi.com/wiki/index.php?title=Manual:Software:Trigger
 * TriggerProcessor.java has this "trigger_type_e" name hard-coded!
 */
enum class trigger_type_e : uint32_t {

	TT_TOOTHED_WHEEL = 0,
	TT_FORD_ASPIRE = 1,
	TT_DODGE_NEON_1995 = 2,
	// https://rusefi.com/wiki/index.php?title=Manual:Software:Trigger#Mazda_Miata_NA
	TT_MAZDA_MIATA_NA = 3,
	TT_VVT_FORD_COYOTE = 4,
	TT_GM_7X = 5,
	TT_DAIHATSU_3_CYL = 6,
	TT_MAZDA_SOHC_4 = 7,
	// "60/2"
	// See also TT_ONE_PLUS_TOOTHED_WHEEL_60_2
	TT_TOOTHED_WHEEL_60_2 = 8,
	TT_TOOTHED_WHEEL_36_1 = 9,
	TT_MERCEDES_2_SEGMENT = 10,
    // a single tooth on lazy Hall or VR. See also TT_HALF_MOON
	TT_NARROW_SINGLE_TOOTH = 11,
	TT_SUBARU_EZ30 = 12,
	TT_VVT_MAZDA_SKYACTIV = 13,
	// cam-based
	TT_DODGE_NEON_2003_CAM = 14,
	TT_MAZDA_DOHC_1_4 = 15,
	//  * "1+1" - one tooth on primary channel, one tooth on secondary channel
	//  * Note: this trigger is used only by unit tests
	//  * see also TT_HALF_MOON a bit below
	TT_ONE_PLUS_ONE = 16,
	/**
	 * VVT for 2JZ
	 * three evenly spaces teeth
	 */
	TT_VVT_TOYOTA_3_TOOTH = 17,
	// just one channel with one perfectly 180 degree tooth with proper reliable Hall sensor, see also TT_NARROW_SINGLE_TOOTH
	TT_HALF_MOON = 18,

	TT_DODGE_RAM = 19,
	// keeping for compatibility with M73 PnP harnesses which until 2023 were build with VR wires flipped
	TT_60_2_WRONG_POLARITY = 20,
	TT_BENELLI_TRE = 21,
	TT_DODGE_STRATUS = 22,
        // Subaru but also Mazda RX-8, we suspect that it's VR
	TT_36_2_2_2 = 23,
	//  * only the 4 tooth signal, without the 360 signal
	//  * 8,2,2,2 Nissan pattern
	TT_NISSAN_SR20VE = 24,
	TT_2JZ_3_34_SIMULATION_ONLY = 25,
	TT_ROVER_K = 26,
	// GM 24x with 5/10 degree gaps
	TT_GM_24x_5 = 27,
	TT_HONDA_CBR_600 = 28,
	TT_MITSU_4G9x_CAM = 29,
	TT_CHRYSLER_NGC_36_2_2 = 30,
	// skipped 3/1 with cam sensor for testing
	TT_3_1_CAM = 31,
	// crank-based in case your cam is broken
	TT_DODGE_NEON_2003_CRANK = 32,
	//  * this takes care of crank sensor, VVT sensor should be configured separately
	//  * for VVT simulated trigger signal we have https://github.com/rusefi/rusefi/issues/566 gap
	//  * See also TT_MAZDA_MIATA_VVT_TEST
	TT_MIATA_VVT = 33,
	TT_MITSU_4G63_CAM = 34,
	// * a version of NB1 with shifted CAM, useful for VVT testing & development
	TT_MAZDA_MIATA_VVT_TEST = 35,
	TT_SUBARU_7_6 = 36,
	// this one is 6 cylinder, see TT_JEEP_4_cyl for 4 cylinders
	TT_JEEP_18_2_2_2 = 37,
	TT_12_TOOTH_CRANK = 38,
	TT_DODGE_NEON_1995_ONLY_CRANK = 39,
	// Jeep XJ 2500cc 4 cylinder. See also TT_JEEP_18_2_2_2 for 6 cylinders
	TT_JEEP_4_CYL = 40,
	// magneti marelli Fiat/Lancia IAW P8 from the 90', 2.0 16 v turbo engine - Lancia Coupe
	// https://rusefi.com/forum/viewtopic.php?f=5&t=1440
	TT_FIAT_IAW_P8 = 41,
	TT_FORD_ST170 = 42,
	// * cam sensor of Mazda Miata NB2 - the VVT signal shape
	TT_VVT_MIATA_NB = 43,
	TT_RENIX_44_2_2 = 44,
	//* Same as TT_RENIX_44_2_2 but repeated three times, not two.
	TT_RENIX_66_2_2_2 = 45,
	// * Honda K crank shape
	TT_HONDA_K_CRANK_12_1 = 46,
	TT_VVT_BOSCH_QUICK_START = 47,
	TT_TOOTHED_WHEEL_36_2 = 48,
	TT_SUBARU_SVX = 49,
	TT_SUZUKI_K6A = 50,
	// todo: remove this trigger once we have https://github.com/rusefi/rusefi/issues/2073
	TT_SUBARU_7_WITHOUT_6 = 51,
	TT_NISSAN_MR18_CAM_VVT = 52,
	// https://rusefi.com/forum/viewtopic.php?f=5&t=1912
	TT_TRI_TACH = 53,
	TT_GM_60_2_2_2 = 54,
	// * https://rusefi.com/forum/viewtopic.php?f=5&t=1937
	// * HALL sensor, and can be used on all Skoda's engines (from 1000MB to 130, Favorit, Felicia)
	TT_SKODA_FAVORIT = 55,
	TT_VVT_BARRA_3_PLUS_1 = 56,
	TT_KAWA_KX450F = 57,
	TT_NISSAN_VQ35 = 58,
	TT_VVT_NISSAN_VQ35 = 59,
	TT_NISSAN_VQ30 = 60,
	TT_NISSAN_QR25 = 61,
	TT_VVT_MITSUBISHI_3A92 = 62,
	TT_SUBARU_SVX_CRANK_1 = 63,
	TT_SUBARU_SVX_CAM_VVT = 64,
	TT_FORD_TFI_PIP = 65,
	TT_SUZUKI_G13B = 66,
	// * Honda K exhaust cam shaft
	TT_HONDA_K_CAM_4_1 = 67,
	TT_NISSAN_MR18_CRANK = 68,
	TT_TOOTHED_WHEEL_32_2 = 69,
	// Mitsubishi 4B11 would be this with half moon camshaft
	// 4G69 would use it with different cam
	TT_36_2_1 = 70,
	// Mitsubishi 3 cyl and 6 cyl
    TT_36_2_1_1 = 71,
  TT_3_TOOTH_CRANK = 72,
	TT_VVT_TOYOTA_4_1 = 73,
	// GM 24x with 3/12 degree gaps
	TT_GM_24x_3 = 74,
	// Renault F3R
	TT_60_2_2_F3R = 75,
	TT_MITSU_4G63_CRANK = 76,
	/* pull-up'ed(inverted in): pull down 2x 30 deg before tdc on cam, till tdc.
	   e.g. Sovek, Saruman ignition blind type */
	TT_60DEG_TOOTH = 77,
	// trigger setup matches 4G15T
	TT_VVT_MITSUBISHI_4G69 = 78,

	TT_VVT_MITSU_6G72 = 79,

  TT_6_TOOTH_CRANK = 80,

	TT_DAIHATSU_4_CYL = 81,

	TT_VVT_MAZDA_L = 82,

	TT_DEV = 83,

	TT_NISSAN_HR = 84,

	// do not forget to edit "#define trigger_type_e_enum" line in integration/rusefi_config.txt file to propogate new value to rusefi.ini TS project
	// do not forget to invoke "gen_config.bat" once you make changes to integration/rusefi_config.txt
	// todo: one day a hero would integrate some of these things into Makefile in order to reduce manual magic
	//
	// Another point: once you add a new trigger, run get_trigger_images.bat which would run rusefi_test.exe from unit_tests
	//
	TT_UNUSED = 85, // this is used if we want to iterate over all trigger types
};

typedef enum {
    TS_14_0 = 0,
    TS_14_1 = 1,
	TS_GRAB_TPS_CLOSED = 2,
	TS_GRAB_TPS_WOT = 3,
    TS_14_4 = 4,
    TS_14_5 = 5,
	TS_GRAB_PEDAL_UP = 6,
	TS_GRAB_PEDAL_WOT = 7,
	TS_RESET_TLE8888 = 8,
    TS_14_9 = 9,
	TS_WRITE_FLASH = 0x0A,
	TS_ETB_RESET = 0x0B,
	TS_ETB_START_AUTOTUNE = 0x0C,
	TS_TRIGGER_STIMULATOR_ENABLE = 0x0D,
	TS_ETB_AUTOCAL_0 = 0x0E,
	TS_TRIGGER_STIMULATOR_DISABLE = 0x0F,
	TS_ETB_STOP_AUTOTUNE = 0x10,
	TS_ETB_AUTOCAL_1 = 0x11,
	TS_WIDEBAND_UPDATE = 0x12,
	TS_EXTERNAL_TRIGGER_STIMULATOR_ENABLE = 0x13,
	TS_RESET_MC33810 = 0x14,
	TS_BURN_WITHOUT_FLASH = 0x15,
} ts_14_command;

typedef enum {
	TS_DEBUG_MODE = 0,
	TS_COMMAND_1 = 1,
	TS_COMMAND_2 = 2,
	TS_COMMAND_3 = 3,
	TS_COMMAND_4 = 4,
	TS_COMMAND_5 = 5,
	TS_COMMAND_6 = 6,
	TS_COMMAND_7 = 7,
	TS_COMMAND_8 = 8,
	TS_COMMAND_9 = 9,
	TS_COMMAND_10 = 10,
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
	// 0x15
	TS_WIDEBAND = 21,
	TS_BENCH_CATEGORY = 22,
	TS_UNUSED_23 = 23,
	TS_UNUSED_24 = 24,
	TS_SOLENOID_CATEGORY = 25,
	TS_UNUSED_26 = 26,
	TS_UNUSED_27 = 27,
	TS_UNUSED_28 = 28,
	TS_UNUSED_29 = 29,
	TS_SET_ENGINE_TYPE = 30,
	TS_SET_DEFAULT_ENGINE = 31,
	TS_LUA_OUTPUT_CATEGORY = 32,
} ts_command_e;

typedef enum {
	BENCH_MAIN_RELAY, // 0
	BENCH_FUEL_PUMP,
	BENCH_STARTER_ENABLE_RELAY,
	BENCH_STARTER_DISABLE_RELAY,
	BENCH_FAN_RELAY,
	BENCH_FAN_RELAY_2, // 5
	BENCH_AC_COMPRESSOR_RELAY,
	BENCH_CHECK_ENGINE_LIGHT,
	BENCH_IDLE_VALVE, // 8
	BENCH_HPFP_VALVE,
	BENCH_GPPWM1_VALVE,
	BENCH_GPPWM2_VALVE,
	BENCH_GPPWM3_VALVE,
	BENCH_GPPWM4_VALVE,
	BENCH_SECOND_IDLE_VALVE,
	BENCH_CANCEL,
	BENCH_VVT0_VALVE, // 16
	BENCH_VVT1_VALVE,
	BENCH_VVT2_VALVE,
	BENCH_VVT3_VALVE,
	BENCH_AUXOUT0,
	BENCH_AUXOUT1,
	BENCH_AUXOUT2,
	BENCH_AUXOUT3,
	BENCH_AUXOUT4,
	BENCH_AUXOUT5,
	BENCH_AUXOUT6,
	BENCH_AUXOUT7,
	HD_ACR,
	HD_ACR2,
} bench_mode_e;
