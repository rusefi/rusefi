#include "pch.h"

/**
 * @brief   Board-specific configuration defaults.
 * @todo    Add your board-specific code, if any.
 */

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = Gpio::D8;
	engineConfiguration->injectionPins[1] = Gpio::D9;
	engineConfiguration->injectionPins[2] = Gpio::D10;
	engineConfiguration->injectionPins[3] = Gpio::D11;

	engineConfiguration->injectionPinMode = OM_OPENDRAIN;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::D13;
	engineConfiguration->ignitionPins[1] = Gpio::D12;
	engineConfiguration->ignitionPins[2] = Gpio::D15;
	engineConfiguration->ignitionPins[3] = Gpio::D14;

	engineConfiguration->ignitionPinMode = OM_OPENDRAIN;
}

void setBoardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();

	// No LEDs on this board
	engineConfiguration->communicationLedPin = Gpio::Unassigned;
	engineConfiguration->runningLedPin = Gpio::Unassigned;
	engineConfiguration->warningLedPin = Gpio::Unassigned;

	engineConfiguration->malfunctionIndicatorPin = Gpio::E14;
	engineConfiguration->tachOutputPin = Gpio::B8;	/* not populated by default */
	//engineConfiguration->idle.solenoidPin = ?
	engineConfiguration->fanPin = Gpio::D6;			/* not populated by default */
	//engineConfiguration->mainRelayPin = ?;

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_2_4_3;

	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
}

static void setupVbatt() {
	// There are two different input dividers:
	// 22K (high) + 22K (low) on IAT and CLT
	// 22K (high) + 41.2K (low) on TPS and MAP
	engineConfiguration->analogInputDividerCoefficient = (22.0 + 22.0) / 22.0;
	// TODO: TPS and MAP
	//engineConfiguration->analogInputDividerCoefficient = (22.0 + 41.2) / 22.0;

	// 68K + 6.8k
	engineConfiguration->vbattDividerCoeff = (68 + 6.8) / 6.8;

	// pin input +12 from Main Relay
	engineConfiguration->vbattAdcChannel = EFI_ADC_9;

	engineConfiguration->adcVcc = 3.3f;
}

void setBoardConfigOverrides() {
	setupVbatt();

	//1.5K
	engineConfiguration->clt.config.bias_resistor = 1500;
	engineConfiguration->iat.config.bias_resistor = 1500;

	//CAN 1 bus overwrites
	engineConfiguration->canRxPin = Gpio::A11;
	engineConfiguration->canTxPin = Gpio::A12;
}
