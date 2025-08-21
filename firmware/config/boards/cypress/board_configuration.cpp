/**
 * @file boards/cypress/board_configuration.h
 *
 * @brief In this file we can override engine_configuration.cpp.
 *
 * @date Jan 27, 2020
 * @author andreika <prometheus.pcb@gmail.com>
 */

#include "pch.h"
#include "trigger_input.h"

Gpio getWarningLedPin() {
	// this board has no warning led
	return Gpio::Unassigned;
}

Gpio getRunningLedPin() {
	return Gpio::Unassigned;
}

Gpio getCommsLedPin() {
	// this board has no comms led
	return Gpio::Unassigned;
}

static void cypressBoardDefaultConfiguration() {
	setCrankOperationMode();

	setAlgorithm(engine_load_mode_e::LM_SPEED_DENSITY);

	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;

	engineConfiguration->map.sensor.type = MT_MPX4250A;

	engineConfiguration->idleStepperReactionTime = 10;
	engineConfiguration->stepperDirectionPinMode = OM_INVERTED;

	engineConfiguration->useLinearCltSensor = true;
	// todo:
	engineConfiguration->clt.config.resistance_1 = 0;
	engineConfiguration->clt.config.tempC_1 = -40.0f;
	engineConfiguration->clt.config.resistance_2 = 5.0f;
	engineConfiguration->clt.config.tempC_2 = 120.0f,
	engineConfiguration->clt.config.bias_resistor = 3300;

	engineConfiguration->tpsMin = convertVoltageTo10bitADC(0.250);
	engineConfiguration->tpsMax = convertVoltageTo10bitADC(4.538);
	engineConfiguration->tpsErrorDetectionTooLow = -10; // -10% open
	engineConfiguration->tpsErrorDetectionTooHigh = 110; // 110% open

	engineConfiguration->mapMinBufferLength = 4;

	engineConfiguration->map.sensor.hwChannel = EFI_ADC_13;
	engineConfiguration->clt.adcChannel = EFI_ADC_26;
	engineConfiguration->iat.adcChannel = EFI_ADC_27;
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_3;
	engineConfiguration->afr.hwChannel = EFI_ADC_4;
	engineConfiguration->vbattAdcChannel = EFI_ADC_2;

	engineConfiguration->triggerInputPins[0] = Gpio::B0;

#if 0

	// todo:
	int i;
	for (i = 0; i < MAX_CYLINDER_COUNT; i++)
		engineConfiguration->injectionPins[i] = Gpio::Unassigned;
	for (i = 0; i < MAX_CYLINDER_COUNT; i++)
		engineConfiguration->ignitionPins[i] = Gpio::Unassigned;

	engineConfiguration->adcVcc = 5.0f;
	engineConfiguration->analogInputDividerCoefficient = 1;
#endif

	//!!!!!!!!!!!!!!!!!!!
	//engineConfiguration->isFastAdcEnabled = false;
}

void setAdcChannelOverrides() {
	addAdcChannelForTrigger();
}

void setup_custom_board_overrides() {
	custom_board_ConfigOverrides = cypressBoardDefaultConfiguration;
}
