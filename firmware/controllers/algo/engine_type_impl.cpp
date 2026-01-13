#include "pch.h"

#include "bmw_m52.h"
#include "bmw_m73.h"

#include "canam.h"
#include "custom_engine.h"
#include "dodge_neon.h"
#include "dodge_ram.h"

#include "ford_aspire.h"
#include "ford_1995_inline_6.h"
#include "ford_ecoboost.h"

#include "honda_k_dbc.h"
#include "honda_600.h"
#include "honda.h"
#include "honda_obd1.h"
#include "hyundai.h"

#include "GY6_139QMB.h"

#include "nissan_vq.h"
#include "tc_4.h"
#include "../board_id/qc_stim_meta.h"

#include "mazda_miata_1_6.h"
#include "mazda_miata_na8.h"
#include "mazda_miata_vvt.h"
#include "m111.h"
#include "mercedes.h"

#include "gm_ls_4.h"
#include "gm_sbc.h"
#include "subaru.h"
#include "slingshot.h"
#include "test_engine.h"
#include "sachs.h"
#include "harley.h"
#include "vw.h"
#include "vw_b6.h"
#include "toyota_jz.h"
#include "toyota_1NZ_FE.h"
#include "mitsubishi_3A92.h"
#include "ford_festiva.h"

#include "board_overrides.h"

static_assert(libPROTEUS_STIM_QC == (int)engine_type_e::PROTEUS_STIM_QC);
static_assert(libHELLEN_2CHAN_STIM_QC == (int)engine_type_e::HELLEN_2CHAN_STIM_QC);
static_assert(libHELLEN_4CHAN_STIM_QC == (int)engine_type_e::HELLEN_4CHAN_STIM_QC);

void applyUnknownEngineType(engine_type_e engineType) {
		// placeholder
}

void boardAfterTuneDefaults(engine_type_e engineType) {
  // placeholder
}

void applyEngineType(engine_type_e engineType) {
	/**
	 * And override them with engine-specific defaults
	 */
	switch (engineType) {
	case engine_type_e::FORD_COYOTE:
	case engine_type_e::MAZDA_MIATA_NC:
	case engine_type_e::DISCOVERY_PDM:
	case engine_type_e::UNUSED94:
	case engine_type_e::UNUSED_97:
	case engine_type_e::TEST_100:
	case engine_type_e::TEST_101:
	case engine_type_e::UNUSED102:
	case engine_type_e::UNUSED_105:
	case engine_type_e::HELLEN_4CHAN_STIM_QC:
	case engine_type_e::HELLEN_2CHAN_STIM_QC:
	case engine_type_e::HELLEN_154_VAG:
	case engine_type_e::HELLEN_121_NISSAN_8_CYL:
	case engine_type_e::HELLEN_121_NISSAN_ALMERA_N16:
	case engine_type_e::SIMULATOR_CONFIG:
	case engine_type_e::HELLEN_121_VAG_4_CYL:
	case engine_type_e::MINIMAL_PINS:
	case engine_type_e::UNUSED_5:
	case engine_type_e::UNUSED_16:
		// all basic settings are already set in prepareVoidConfiguration(), no need to set anything here
		// nothing to do - we do it all in setBoardDefaultConfiguration
		break;
#if EFI_UNIT_TEST || EFI_SIMULATOR
	case engine_type_e::TEST_ISSUE_366_BOTH:
	  #if EFI_UNIT_TEST
		setTestEngineIssue366both();
		#endif
		break;
	case engine_type_e::TEST_ISSUE_366_RISE:
	  #if EFI_UNIT_TEST
		setTestEngineIssue366rise();
		#endif
		break;
	case engine_type_e::ET_BOSCH_QUICK_START:
	  setTestEngineBoschQuickStart();
	  break;
	case engine_type_e::ET_TEST_WITH_BOSCH_QUICK_START:
	  setTestEngineWithBoschQuickStartCam();
	  break;
#endif // EFI_UNIT_TEST

#if EFI_TCU || EFI_SIMULATOR
	case engine_type_e::TCU_4R70W:
		#if EFI_TCU
		configureTcu4R70W();
		#endif // EFI_TCU
		break;
#endif //EFI_TCU

#if defined(HW_SUBARU_2011) || defined(HW_HELLEN_8CHAN) || HW_PROTEUS
	case engine_type_e::SUBARU_2011:
	  setSubaru2011();
		break;
#endif

#if HW_MICRO_RUSEFI || HW_PROTEUS
	case engine_type_e::MERCEDES_M111:
		setMercedesM111EngineConfiguration();
		break;
#endif

#if HW_MICRO_RUSEFI
	case engine_type_e::MRE_VW_B6:
		setMreVwPassatB6();
		break;
	case engine_type_e::MRE_SECONDARY_CAN:
		mreSecondaryCan();
		break;
	case engine_type_e::MRE_SUBARU_EJ18:
		setSubaruEJ18_MRE();
		break;
	case engine_type_e::UNUSED31:
	case engine_type_e::BMW_M73_MRE:
	case engine_type_e::BMW_M73_MRE_SLAVE:
		setEngineBMW_M73_microRusEfi();
		break;
	case engine_type_e::MRE_BODY_CONTROL:
		mreBCM();
		break;
#endif // HW_MICRO_RUSEFI
	case engine_type_e::HONDA_OBD1:
		setHondaObd1();
		break;
	case engine_type_e::FUEL_BENCH:
	  fuelBenchMode();
		break;

#if HW_PROTEUS || HW_HELLEN_4CHAN || HW_HELLEN_8CHAN || HW_HELLEN_4K_GDI
	case engine_type_e::FORD_ECOBOOST:
	  setFordEcoboost();
		break;
  case engine_type_e::HYUNDAI_PB:
    setHyundaiPb();
	  break;
#endif

#if HW_PROTEUS || HW_HELLEN_HONDA
	case engine_type_e::HONDA_K:
		setHondaK();
		break;
#endif

#if HW_HELLEN
	case engine_type_e::MAZDA_MIATA_NA96:
		setMazdaMiata96();
		break;
	case engine_type_e::MAZDA_MIATA_NA6:
		setMazdaMiataNA6();
		break;
	case engine_type_e::MAZDA_MIATA_NA94:
		setMazdaMiataNA94();
		break;
	case engine_type_e::MAZDA_MIATA_NB1:
		setMazdaMiataNB1();
		break;
	case engine_type_e::MAZDA_MIATA_NB2:
		setMazdaMiataNB2();
		break;
	case engine_type_e::MAZDA_MIATA_NB2_36:
		setMazdaMiataNB2_36();
		break;
#endif

#if HW_PROTEUS || HW_HELLEN
	case engine_type_e::HARLEY:
		setHarley();
		break;
#endif

#if HW_PROTEUS || HW_HELLEN
	case engine_type_e::POLARIS_RZR:
	case engine_type_e::MAVERICK_X3:
	    setMaverickX3();
		break;
	case engine_type_e::ME17_9_MISC:
	case engine_type_e::POLARIS:
	    setSlingshot();
		break;
#endif

	case engine_type_e::GM_LCV:
	  setGmLcv();
		break;
	case engine_type_e::GM_LNF:
	  setGmLnf();
		break;
	case engine_type_e::GM_LTG:
	  setGmLtg();
		break;
	case engine_type_e::GM_SBC_GEN5:
	  setGmSbcGen5();
		break;

#if defined(HW_HELLEN_UAEFI) || defined(HW_HELLEN_UAEFI121) || defined(HW_HELLEN_SUPER_UAEFI) || defined(HW_HELLEN_8CHAN) || HW_PROTEUS || EFI_SIMULATOR
	case engine_type_e::GM_SBC_GEN4:
		setGmLs4();
		break;
	case engine_type_e::GM_SBC:
	  setGmSbc();
    break;
#endif

#if HW_PROTEUS || EFI_SIMULATOR
    case engine_type_e::WASTEGATE_PROTEUS_TEST:
        proteusDcWastegateTest();
        break;
    case engine_type_e::PROTEUS_NISSAN_VQ35:
        setProteusNissanVQ();
		break;
	case engine_type_e::PROTEUS_VW_B6:
		setProteusVwPassatB6();
		break;
	case engine_type_e::UNUSED42:
	case engine_type_e::PROTEUS_LUA_DEMO:
		proteusLuaDemo();
		break;
	case engine_type_e::PROTEUS_BMW_M73:
		setEngineBMW_M73_Proteus();
		break;
	case engine_type_e::MIATA_PROTEUS_TCU:
		setMiataNB2_Proteus_TCU();
		break;
	case engine_type_e::HONDA_OBD2A:
		setProteusHondaOBD2A();
		break;
	case engine_type_e::PROTEUS_STIM_QC:
	    proteusStimQc();
		break;
#if defined(HARDWARE_CI) || EFI_SIMULATOR
	case engine_type_e::PROTEUS_ANALOG_PWM_TEST:
    #if defined(HARDWARE_CI)
		setProteusAnalogPwmTest();
    #endif
		break;
#endif // HARDWARE_CI
#endif // HW_PROTEUS

#if defined(HW_HELLEN_MERCEDES) || EFI_SIMULATOR
	case engine_type_e::HELLEN_128_MERCEDES_4_CYL:
		setHellenMercedes128_4_cyl();
		break;
	case engine_type_e::HELLEN_128_MERCEDES_6_CYL:
		setHellenMercedes128_6_cyl();
		break;
	case engine_type_e::HELLEN_128_MERCEDES_8_CYL:
		setHellenMercedes128_8_cyl();
		break;
#endif

#ifdef HW_HELLEN_NISSAN
	case engine_type_e::HELLEN_121_NISSAN_4_CYL:
		setHellen121nissanQR();
		break;
	case engine_type_e::HELLEN_121_NISSAN_6_CYL:
		setHellen121nissanVQ();
		break;
#endif

#ifdef HW_HELLEN_HYUNDAI
	case engine_type_e::HELLEN_154_HYUNDAI_COUPE_BK1:
		setGenesisCoupeBK1();
		break;
	case engine_type_e::HELLEN_154_HYUNDAI_COUPE_BK2:
		setGenesisCoupeBK2();
		break;
#endif

#if defined(HW_HELLEN_121_VAG) || defined(HW_HELLEN_UAEFI) || defined(HW_HELLEN_UAEFI121) || EFI_SIMULATOR
	case engine_type_e::VAG_5_CYL:
	    setVag_5_cyl();
        break;
#endif

#if defined(HW_HELLEN_121_VAG) || EFI_SIMULATOR
	case engine_type_e::HELLEN_121_VAG_V6_CYL:
	    setHellen121Vag_v6_cyl();
        break;
	case engine_type_e::HELLEN_121_VAG_VR6_CYL:
	    setHellen121Vag_vr6_cyl();
        break;
	case engine_type_e::HELLEN_121_VAG_8_CYL:
	    setHellen121Vag_8_cyl();
        break;
#endif

#ifdef HW_HELLEN
	case engine_type_e::TOYOTA_1NZ_FE:
	    setToyota1NZFE();
	    break;
	case engine_type_e::HELLEN_HONDA_BCM:
	    setHondaCivicBcm();
        break;
#endif // HW_HELLEN
#if defined(HW_NUCLEO_F767) || defined(HW_NUCLEO_H743) || defined(HW_FRANKENSO) || HW_PROTEUS || EFI_SIMULATOR
    // used in HW CI
	case engine_type_e::VW_ABA:
		setVwAba();
		break;
	case engine_type_e::FRANKENSO_BMW_M73_F:
		setBMW_M73_TwoCoilUnitTest();
		break;
#endif // HW_FRANKENSO || HW_PROTEUS

	case engine_type_e::TEST_ISSUE_6451:
		testEngine6451();
		break;

#if defined(HW_FRANKENSO) || EFI_SIMULATOR || defined(HW_NUCLEO_F767) || defined(HW_NUCLEO_H743) || defined(SUPPORT_GY6)
	case engine_type_e::GY6_139QMB:
		setGy6139qmbDefaultEngineConfiguration();
		break;
#endif

#if defined(HW_FRANKENSO) || EFI_SIMULATOR || defined(HW_NUCLEO_F767) || defined(HW_NUCLEO_H743)
	case engine_type_e::DEFAULT_FRANKENSO:
		setFrankensoConfiguration();
		break;
	case engine_type_e::FRANKENSO_TEST_33810:
#if EFI_PROD_CODE
		setDiscovery33810Test();
#endif
		break;
	case engine_type_e::TEST_ENGINE:
		setTestCamEngineConfiguration();
		break;
	case engine_type_e::TEST_CRANK_ENGINE:
		setTestCrankEngineConfiguration();
		break;
	case engine_type_e::DODGE_NEON_1995:
		setDodgeNeon1995EngineConfiguration();
		break;
	case engine_type_e::DODGE_NEON_2003_CRANK:
		setDodgeNeonNGCEngineConfiguration();
		break;
	case engine_type_e::FORD_ASPIRE_1996:
		setFordAspireEngineConfiguration();
		break;
	case engine_type_e::FRANKENSO_MIATA_NA6_MAP:
		setMiataNA6_MAP_Frankenso();
		break;
	case engine_type_e::ETB_BENCH_ENGINE:
		setEtbTestConfiguration();
		break;
	case engine_type_e::L9779_BENCH_ENGINE:
		setL9779TestConfiguration();
		break;
	case engine_type_e::EEPROM_BENCH_ENGINE:
#if EFI_PROD_CODE && HAL_USE_EEPROM
		setEepromTestConfiguration();
#endif
		break;
	case engine_type_e::MITSUBISHI_3A92:
	    setMitsubishi3A92();
	    break;
	case engine_type_e::FORD_INLINE_6_1995:
		setFordInline6();
		break;
	case engine_type_e::HONDA_600:
		setHonda600();
		break;
	case engine_type_e::FORD_ESCORT_GT:
		setFordEscortGt();
		break;
	case engine_type_e::DODGE_RAM:
		setDodgeRam1996();
		break;
	case engine_type_e::TEST_ENGINE_VVT:
		setTestVVTEngineConfiguration();
		break;
	case engine_type_e::TEST_DC_WASTEGATE_DISCOVERY:
		setTestDcWastegateConfiguration();
		break;
	case engine_type_e::SACHS:
		setSachs();
		break;
	case engine_type_e::TOYOTA_2JZ_GTE_VVTi:
		setToyota_2jz_vics();
		break;
	case engine_type_e::TEST_33816:
		setTest33816EngineConfiguration();
		break;
	case engine_type_e::TEST_ROTARY:
		setRotary();
		break;
#endif // HW_FRANKENSO
	case engine_type_e::BMW_M52:
	  bmwM52();
    break;
#if defined(HW_SUBARU_EG33) || EFI_SIMULATOR
	case engine_type_e::SUBARU_EG33:
		setSubaruEG33Defaults();
		break;
#endif //HW_SUBARU_EG33
	default:
	  if (custom_board_applyUnknownType.has_value()) {
	    call_board_override(custom_board_applyUnknownType, engineType);
	  } else {
		  firmwareError(ObdCode::CUSTOM_UNEXPECTED_ENGINE_TYPE, "Unexpected engine type: %d", (int)engineType);
	  }
	}

  call_board_override(custom_board_AfterTuneDefaults, engineType);
}

PUBLIC_API_WEAK_SOMETHING_WEIRD engine_type_e getLastEngineType() {
  return engine_type_e::UNUSED_105;
}
