/*
 * honda_k_dbc.cpp
 *
 * @date Oct 2, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "honda_k_dbc.h"
#include "table_helper.h"

#if HW_PROTEUS & EFI_PROD_CODE
#include "proteus_meta.h"
#endif // HW_PROTEUS

void setHondaK() {
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->displacement = 2.4;
	engineConfiguration->firingOrder = FO_1_3_4_2;
	engineConfiguration->engineSyncCam = SC_Exhaust_First;
	engineConfiguration->trigger.type = trigger_type_e::TT_HONDA_K_CRANK_12_1;
	engineConfiguration->camSyncOnSecondCrankRevolution = true;
	int magic = 0; // note that offset and VVT are related
	engineConfiguration->globalTriggerAngleOffset = 303 - magic;

	// VVT is here just single tooth? and we do not even use it!?
	engineConfiguration->vvtMode[0] = VVT_HONDA_K_INTAKE;
	// also known as 'CAM sync'? i am pretty confused at this point
	engineConfiguration->vvtMode[1] = VVT_HONDA_K_EXHAUST;
	engineConfiguration->vvtOffsets[0] = -21;
	engineConfiguration->vvtOffsets[1] = 183 + magic;

	// set cranking_fuel 15
	setTable(config->crankingCycleBaseFuel, 75);

	engineConfiguration->map.sensor.type = MT_DENSO183;
// todo: quote some source?
	engineConfiguration->injector.flow = 270;
	engineConfiguration->injector.flow = 310;
	engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure;
	engineConfiguration->fuelReferencePressure = 350; // TODO: what is real value?!

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	setArrayValues(config->cltCrankingCorr, 70);
#if CRANKING_ENRICH_COUNT == 6
	static const uint16_t defaultPostCrankinDurationBins[] = {
		0, 65, 100, 125, 150, 250
	};
	copyArray(config->postCrankingDurationBins, defaultPostCrankinDurationBins);
#endif // CRANKING_ENRICH_COUNT
	setTable(config->postCrankingFactor, 1.25f);
    engineConfiguration->useRunningMathForCranking = true;

	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_HONDA);
	strcpy(engineConfiguration->engineCode, "K24");

	engineConfiguration->tpsMin = 100;
	engineConfiguration->tpsMax = 830;

	engineConfiguration->displayLogicLevelsInEngineSniffer = true;

	gppwm_channel *vtsControl = &engineConfiguration->gppwm[0];
	vtsControl->pwmFrequency = 0;
#if EFI_SIMULATOR
    // simulator canned config XML toolset cares to see perfect empty memory region
	memset(engineConfiguration->gpPwmNote[0], 0, sizeof(gppwm_note_t));
#endif
	strcpy(engineConfiguration->gpPwmNote[0], "VTS");

/**
 * K24A4 engine
 * HONDA_K
 */

#if HW_PROTEUS & EFI_PROD_CODE
	strcpy(engineConfiguration->vehicleName, "test");
//	engineConfiguration->triggerInputPins[0] = PROTEUS_DIGITAL_2; // crank
//	engineConfiguration->camInputs[0] = PROTEUS_DIGITAL_4; // intake
//	engineConfiguration->camInputs[1 * CAMS_PER_BANK] = PROTEUS_DIGITAL_1; // exhaust

	engineConfiguration->triggerInputPins[0] = PROTEUS_DIGITAL_1; // exhaust
	engineConfiguration->camInputs[0] = PROTEUS_DIGITAL_4; // intake
// inverted
	// offset -41


	engineConfiguration->injectionPins[0] = Gpio::PROTEUS_LS_8;
	engineConfiguration->injectionPins[1] = Gpio::PROTEUS_LS_7;
	engineConfiguration->injectionPins[2] = Gpio::PROTEUS_LS_6;
	engineConfiguration->injectionPins[3] = Gpio::PROTEUS_LS_5;

	vtsControl->pin = Gpio::PROTEUS_HS_1;
	engineConfiguration->vvtPins[0] = Gpio::PROTEUS_HS_2;

	engineConfiguration->malfunctionIndicatorPin = Gpio::PROTEUS_LS_10;
	engineConfiguration->idle.solenoidPin = Gpio::PROTEUS_LS_15;
	engineConfiguration->fanPin = Gpio::PROTEUS_LS_1;

	engineConfiguration->iat.adcChannel = PROTEUS_IN_ANALOG_TEMP_1;
	engineConfiguration->clt.adcChannel = PROTEUS_IN_ANALOG_TEMP_2;
	engineConfiguration->tps1_1AdcChannel = PROTEUS_IN_ANALOG_VOLT_3;
	engineConfiguration->map.sensor.hwChannel = PROTEUS_IN_ANALOG_VOLT_6;
	engineConfiguration->fanPin = Gpio::Unassigned;

	engineConfiguration->mainRelayPin = Gpio::PROTEUS_LS_9;
	engineConfiguration->fuelPumpPin = Gpio::PROTEUS_LS_11;

#endif // HW_PROTEUS
}

void setProteusHondaOBD2A() {

}
