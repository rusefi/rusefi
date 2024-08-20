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
#include "proteus_meta.h"

void setTestEngineBoschQuickStart() {
	engineConfiguration->trigger.type = trigger_type_e::TT_VVT_BOSCH_QUICK_START;
}

void setTestEngineWithBoschQuickStartCam() {
  engineConfiguration->trigger.type = trigger_type_e::TT_HALF_MOON;
  engineConfiguration->vvtMode[0] = VVT_BOSCH_QUICK_START;
}

// TEST_ENGINE
void setTestCamEngineConfiguration() {
	engineConfiguration->trigger.type = trigger_type_e::TT_ONE_PLUS_ONE;
	setCamOperationMode();

	engineConfiguration->mafAdcChannel = EFI_ADC_1;
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_2;
	engineConfiguration->vbattAdcChannel = EFI_ADC_NONE;

	setWholeIatCorrTimingTable(0);

	// Many tests were written when the default target AFR was 14.0, so use that for tests by default.
	engineConfiguration->stoichRatioPrimary = 14.0f;

	engineConfiguration->ignitionMode = IM_ONE_COIL;
	setConstantDwell(3); // 50% duty cycle @ 5000 rpm

	engineConfiguration->malfunctionIndicatorPin = Gpio::Unassigned;

	engineConfiguration->ignitionPins[0] = Gpio::C7; // #1
	engineConfiguration->ignitionPins[1] = Gpio::Unassigned; // #2
	engineConfiguration->ignitionPins[2] = Gpio::Unassigned; // #3
	engineConfiguration->ignitionPins[3] = Gpio::Unassigned; // #4
	engineConfiguration->ignitionPins[4] = Gpio::Unassigned; // #5
	engineConfiguration->ignitionPins[5] = Gpio::Unassigned; // #6
}

void setTestCrankEngineConfiguration() {
	setTestCamEngineConfiguration();

	engineConfiguration->trigger.type = trigger_type_e::TT_HALF_MOON;

	setCrankOperationMode();
	// this is related to 'setDefaultBaseEngine' having 'skippedWheelOnCam = true' which is a weird fact by itself
	engineConfiguration->skippedWheelOnCam = false;
}

void setTestDcWastegateConfiguration() {
    engineConfiguration->isBoostControlEnabled = 1;
	engineConfiguration->etbFunctions[0] = DC_Wastegate;
	setTable(config->boostTableOpenLoop, 50);
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_1; // PA1

	// PWM pin
	engineConfiguration->etbIo[0].controlPin = Gpio::C0;
	// DIR pin
	engineConfiguration->etbIo[0].directionPin1 = Gpio::C1;
	// Disable pin
	engineConfiguration->etbIo[0].disablePin = Gpio::C2;
	// we only have pwm/dir, no dira/dirb
	engineConfiguration->etb_use_two_wires = false;
}

void setTestVVTEngineConfiguration() {
	setCrankOperationMode();

	engineConfiguration->trigger.type = trigger_type_e::TT_TOOTHED_WHEEL;
	engineConfiguration->trigger.customTotalToothCount = 3;
	engineConfiguration->trigger.customSkippedToothCount = 1;

	// set algorithm 3
	setAlgorithm(LM_SPEED_DENSITY);

	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
	engineConfiguration->camInputs[0] = Gpio::A5;

	// set global_trigger_offset_angle 0
	engineConfiguration->globalTriggerAngleOffset = 0;

	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH;
}

#if EFI_UNIT_TEST
void setTestEngineIssue366both() {
	setTestCamEngineConfiguration();

	engineConfiguration->trigger.customTotalToothCount = 2;
	engineConfiguration->trigger.customSkippedToothCount = 1;

	engineConfiguration->trigger.type = trigger_type_e::TT_TOOTHED_WHEEL;
}

void setTestEngineIssue366rise() {
	setTestEngineIssue366both();
}
#endif /* EFI_UNIT_TEST */

#ifdef HARDWARE_CI
// set engine_type 30
void setProteusAnalogPwmTest() {
  engineConfiguration->vvtPins[0] = Gpio::E0; // a random unused pin needed for happy HW CI. Huh, why do we have cam stimulation?!

	// lowest cpu trigger possible
	engineConfiguration->trigger.type = trigger_type_e::TT_HALF_MOON;

	// Disable trigger stim
	engineConfiguration->triggerSimulatorPins[0] = Gpio::Unassigned;
	engineConfiguration->triggerSimulatorPins[1] = Gpio::Unassigned;

	// The idle control pin is connected to the default TPS input, analog volt 2
	engineConfiguration->idle.solenoidPin = Gpio::PROTEUS_IGN_10;

	// 5893hz is coprime with the analog sample rate, 500hz, so hopefully we get less aliasing
	engineConfiguration->idle.solenoidFrequency = 5893;

	// Test range is 20% to 80%
	engineConfiguration->tpsMin = 200;
	engineConfiguration->tpsMax = 800;
}
#endif // HARDWARE_CI
