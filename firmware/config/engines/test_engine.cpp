/**
 * @file	test_engine.cpp
 *
 * set engine_type 26
 * VVT
 * set engine_type 45
 *
 * See also custom_engine.cpp
 *
 * @date Nov 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "test_engine.h"

// TEST_ENGINE
void setTestEngineConfiguration() {
	setDefaultFrankensoConfiguration();

	setOperationMode(engineConfiguration, FOUR_STROKE_CAM_SENSOR);
	engineConfiguration->trigger.type = TT_ONE_PLUS_ONE;

//	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
//	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;
//	trigger_config_s *triggerConfig = &engineConfiguration->trigger;
//	triggerConfig->customTotalToothCount = 60;
//	triggerConfig->customSkippedToothCount = 0;
	engineConfiguration->useOnlyRisingEdgeForTrigger = false;

	engineConfiguration->mafAdcChannel = EFI_ADC_1;
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_2;
	engineConfiguration->vbattAdcChannel = EFI_ADC_NONE;

	setWholeIatCorrTimingTable(0);

	// Many tests were written when the default target AFR was 14.0, so use that for tests by default.
	engineConfiguration->stoichRatioPrimary = 14.0f;

	engineConfiguration->ignitionMode = IM_ONE_COIL;
	setConstantDwell(3); // 50% duty cycle @ 5000 rpm

	engineConfiguration->malfunctionIndicatorPin = GPIO_UNASSIGNED;

	engineConfiguration->ignitionPins[0] = GPIOC_7; // #1
	engineConfiguration->ignitionPins[1] = GPIO_UNASSIGNED; // #2
	engineConfiguration->ignitionPins[2] = GPIO_UNASSIGNED; // #3
	engineConfiguration->ignitionPins[3] = GPIO_UNASSIGNED; // #4
	engineConfiguration->ignitionPins[4] = GPIO_UNASSIGNED; // #5
	engineConfiguration->ignitionPins[5] = GPIO_UNASSIGNED; // #6
}

void setTestVVTEngineConfiguration() {
	setDefaultFrankensoConfiguration();
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);

	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;
	engineConfiguration->trigger.customTotalToothCount = 3;
	engineConfiguration->trigger.customSkippedToothCount = 1;

	// set algorithm 3
	setAlgorithm(LM_SPEED_DENSITY);

	engineConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->camInputs[0] = GPIOA_5;

	// set global_trigger_offset_angle 0
	engineConfiguration->globalTriggerAngleOffset = 0;

	engineConfiguration->vvtMode[0] = VVT_SECOND_HALF;
	engineConfiguration->debugMode = DBG_VVT;
}

#if EFI_UNIT_TEST
void setTestEngineIssue366both() {
	setTestEngineConfiguration();


	engineConfiguration->useOnlyRisingEdgeForTrigger = false;
	engineConfiguration->trigger.customTotalToothCount = 2;
	engineConfiguration->trigger.customSkippedToothCount = 1;

	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;

}

void setTestEngineIssue366rise() {
	setTestEngineIssue366both();


	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
}
#endif /* EFI_UNIT_TEST */

#ifdef HARDWARE_CI
void setProteusAnalogPwmTest() {
	// lowest cpu trigger possible
	engineConfiguration->trigger.type = TT_ONE;

	// Disable trigger stim
	engineConfiguration->triggerSimulatorPins[0] = GPIO_UNASSIGNED;
	engineConfiguration->triggerSimulatorPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->triggerSimulatorPins[2] = GPIO_UNASSIGNED;

	// The idle control pin is connected to the default TPS input, analog volt 2
	engineConfiguration->idle.solenoidPin = GPIOG_4;

	// 5893hz is coprime with the analog sample rate, 500hz, so hopefully we get less aliasing
	engineConfiguration->idle.solenoidFrequency = 5893;

	// Test range is 20% to 80%
	engineConfiguration->tpsMin = 200;
	engineConfiguration->tpsMax = 800;
}
#endif
