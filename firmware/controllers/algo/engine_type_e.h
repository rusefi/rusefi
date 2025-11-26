#pragma once

/**
 * https://wiki.rusefi.com/Engine-Types
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

	UNUSED_5 = 5,
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
	// this is pretty close to body control module/BCM use-case!
	FUEL_BENCH = 15,
	UNUSED_16 = 16,
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

	GM_SBC_GEN4 = 27,

	TEST_CRANK_ENGINE = 28,

  // two stroke
	SACHS = 29,

	PROTEUS_ANALOG_PWM_TEST = 30,

	UNUSED31 = 31,

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

	UNUSED42 = 42,

	HONDA_600 = 43,

	TOYOTA_2JZ_GTE_VVTi = 44,

	TEST_ENGINE_VVT = 45,

	DODGE_NEON_2003_CRANK = 46,

	GM_LCV = 47,

    // todo: add wastegate case into HW CI
	TEST_DC_WASTEGATE_DISCOVERY = 48,

	GM_SBC_GEN5 = 49,

	BMW_M73_MRE = 50,

	BMW_M73_MRE_SLAVE = 51,


	TEST_ISSUE_366_BOTH = 52,
	TEST_ISSUE_366_RISE = 53,
	MAVERICK_X3 = 54,
	GM_LTG = 55,
	SUBARU_2011 = 56,
	// see also 'FUEL_BENCH'
	HELLEN_HONDA_BCM = 57,
	ETB_BENCH_ENGINE = 58,
	FRANKENSO_TEST_33810 = 59,

	L9779_BENCH_ENGINE = 60,
	EEPROM_BENCH_ENGINE = 61,
	MRE_VW_B6 = 62,
	PROTEUS_BMW_M73 = 63,
	DODGE_RAM = 64,
	GM_LNF = 65,
	HONDA_OBD1 = 66,
	FORD_ECOBOOST = 67,
	MERCEDES_M111 = 68,

	MAZDA_MIATA_NB2 = 69,

	SUBARU_EG33 = 70,

	HELLEN_121_VAG_4_CYL = 71,
	HELLEN_121_NISSAN_6_CYL = 72,
	PROTEUS_STIM_QC = 73,
	HELLEN_4CHAN_STIM_QC = 74,
	HELLEN_2CHAN_STIM_QC = 75,
	HELLEN_154_VAG = 76,

	VAG_5_CYL = 77,
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
  UNUSED_105 = 105,

// board_engine_types_tag

  // [CannedTunes] please keep this always last always unused
};

engine_type_e getLastEngineType();
