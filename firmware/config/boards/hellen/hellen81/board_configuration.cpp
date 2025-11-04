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
#include "board_overrides.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = Gpio::G7;
	engineConfiguration->injectionPins[1] = Gpio::G8;
	engineConfiguration->injectionPins[2] = Gpio::D11;
	engineConfiguration->injectionPins[3] = Gpio::D10;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::I8;
	engineConfiguration->ignitionPins[1] = Gpio::E5;
	engineConfiguration->ignitionPins[2] = Gpio::E4;
	engineConfiguration->ignitionPins[3] = Gpio::E3;
}

Gpio getRunningLedPin() {
	return Gpio::Unassigned;
}

Gpio getCommsLedPin() {
	return Gpio::H10;
}

Gpio getWarningLedPin() {
	return Gpio::H11;
}

static void setupDefaultSensorInputs() {
	// trigger inputs
	engineConfiguration->triggerInputPins[0] = Gpio::B1;
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
	// Direct hall-only cam input
	engineConfiguration->camInputs[0] = Gpio::A6;

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_4;


	engineConfiguration->map.sensor.hwChannel = EFI_ADC_10;

	engineConfiguration->afr.hwChannel = EFI_ADC_0;	// ADC1_16

	engineConfiguration->clt.adcChannel = H144_IN_CLT;	// ADC3_0

	engineConfiguration->iat.adcChannel = H144_IN_IAT;	// ADC3_1
}

static void hellen81_boardConfigOverrides() {
	setHellenVbatt();

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

 *

 */
static void hellen81_boardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();

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

void setup_custom_board_overrides() {
	custom_board_DefaultConfiguration = hellen81_boardDefaultConfiguration;
	custom_board_ConfigOverrides = hellen81_boardConfigOverrides;
}

