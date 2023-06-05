/**
 * @file boards/hellen/hellen81/board_configuration.cpp
 *
 *
 * @brief Configuration defaults for the Hellen81 board
 *
 * See https://rusefi.com/s/hellen81
 *
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"
#include "hellen_meta.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = Gpio::G7;
	engineConfiguration->injectionPins[1] = Gpio::G8;
	engineConfiguration->injectionPins[2] = Gpio::D11;
	engineConfiguration->injectionPins[3] = Gpio::D10;

	engineConfiguration->injectionPinMode = OM_DEFAULT;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::I8;
	engineConfiguration->ignitionPins[1] = Gpio::E5;
	engineConfiguration->ignitionPins[2] = Gpio::E4;
	engineConfiguration->ignitionPins[3] = Gpio::E3;

	engineConfiguration->ignitionPinMode = OM_DEFAULT;
}

Gpio getCommsLedPin() {
	return Gpio::H10;
}

Gpio getWarningLedPin() {
	return Gpio::H11;
}

static void setupVbatt() {
	// 4.7k high side/4.7k low side = 2.0 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 2.0f;

	// set vbatt_divider 5.835
	// 33k / 6.8k
	engineConfiguration->vbattDividerCoeff = (33 + 6.8) / 6.8; // 5.835

	// pin input +12 from Main Relay
	engineConfiguration->vbattAdcChannel = EFI_ADC_5;

	engineConfiguration->adcVcc = 3.29f;
}

static void setupDefaultSensorInputs() {
	// trigger inputs
	engineConfiguration->triggerInputPins[0] = Gpio::B1;
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
	// Direct hall-only cam input
	engineConfiguration->camInputs[0] = Gpio::A6;

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_4;

	engineConfiguration->mafAdcChannel = EFI_ADC_NONE;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_10;

	engineConfiguration->afr.hwChannel = EFI_ADC_0;	// ADC1_16

	engineConfiguration->clt.adcChannel = H144_IN_CLT;	// ADC3_0

	engineConfiguration->iat.adcChannel = H144_IN_IAT;	// ADC3_1
}

void setBoardConfigOverrides() {
	setupVbatt();

// Hellen81a uses SPI2 for SD-card
#if 1
	setHellenSdCardSpi2();
#else
	setHellenSdCardSpi3();
#endif

    setDefaultHellenAtPullUps();

	setHellenCan();
}

/**
 * @brief   Board-specific configuration defaults.
 *
 * See also setDefaultEngineConfiguration
 *
 * @todo    Add your board-specific code, if any.
 */
void setBoardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();

	engineConfiguration->isSdCardEnabled = true;

	engineConfiguration->fuelPumpPin = Gpio::G2;	// OUT_IO9
	engineConfiguration->fanPin = Gpio::D12;	// OUT_PWM8
	engineConfiguration->mainRelayPin = Gpio::I2;	// OUT_LOW3
	engineConfiguration->tachOutputPin = Gpio::D14;	// OUT_PWM6

	engineConfiguration->useStepperIdle = true;
	engineConfiguration->useHbridgesToDriveIdleStepper = true;
	engineConfiguration->stepperDcIo[0].directionPin1 = Gpio::C6;	// COIL_A1 = OUT_PWM2
	engineConfiguration->stepperDcIo[0].directionPin2 = Gpio::C7;	// COIL_A2 = OUT_PWM3
	engineConfiguration->stepperDcIo[1].directionPin1 = Gpio::C8;	// COIL_B1 = OUT_PWM4
	engineConfiguration->stepperDcIo[1].directionPin2 = Gpio::C9;	// COIL_B2 = OUT_PWM5

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK

	engineConfiguration->injectionMode = IM_SEQUENTIAL;	// IM_SIMULTANEOUS; //IM_BATCH;
}
