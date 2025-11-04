/**
 * @file boards/hellen/hellen72/board_configuration.cpp
 *
 *
 * @brief Configuration defaults for the Hellen72 board
 *
 * See https://rusefi.com/s/hellen72
 *
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
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
	engineConfiguration->ignitionPins[0] = H176_IGN_1; // 3F - IGN_1 (1&4)
	engineConfiguration->ignitionPins[1] = Gpio::Unassigned ; // Gpio::E4
	engineConfiguration->ignitionPins[2] = Gpio::E5; // 3I - IGN_2 (2&3)
	engineConfiguration->ignitionPins[3] = Gpio::Unassigned; // Gpio::E3
}

static void setupDefaultSensorInputs() {
	// trigger inputs
	engineConfiguration->triggerInputPins[0] = Gpio::B1;
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
	// Direct hall-only cam input
	engineConfiguration->camInputs[0] = Gpio::A6;

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_4;

	engineConfiguration->mafAdcChannel = EFI_ADC_10;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_11;

	engineConfiguration->afr.hwChannel = EFI_ADC_1;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;

	engineConfiguration->iat.adcChannel = H144_IN_IAT;
}

#include "hellen_leds_176.cpp"

static void hellen_MiataNB2_boardConfigOverrides() {
	setHellenVbatt();

	if (engine->engineState.hellenBoardId == -1) {
		// Rev a-d use SPI3 for SD card
		setHellenSdCardSpi3();
	} else {
		// Revs E and later use SPI2 for SD card
		setHellenSdCardSpi2();
	}

    // NB2 still uses L6205PD
	engineConfiguration->etbIo[0].directionPin1 = Gpio::H144_OUT_PWM3; // ETB+
	engineConfiguration->etbIo[0].directionPin2 = Gpio::H144_OUT_PWM4; // ETB-
	engineConfiguration->etbIo[0].controlPin = Gpio::H144_OUT_PWM2; // ETB_EN
	engineConfiguration->etb_use_two_wires = true;

    setDefaultHellenAtPullUps();

	setHellenCan();
}

/**
 * @brief   Board-specific configuration defaults.
 *

 *

 */
static void hellen_MiataNB2_boardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();

	engineConfiguration->enableSoftwareKnock = true;

	engineConfiguration->acRelayPin = Gpio::H15;
	engineConfiguration->acSwitch = Gpio::B0;
	engineConfiguration->acSwitchMode = PI_PULLUP;

	engineConfiguration->vehicleSpeedSensorInputPin = Gpio::H144_IN_VSS;
	engineConfiguration->clutchDownPin = Gpio::H144_IN_RES3;
	engineConfiguration->clutchDownPinMode = PI_INVERTED_DEFAULT;

	engineConfiguration->fuelPumpPin = Gpio::G2;	// OUT_IO9
	engineConfiguration->idle.solenoidPin = Gpio::D14;	// OUT_PWM5
	engineConfiguration->fanPin = Gpio::D12;	// OUT_PWM8
	engineConfiguration->fan2Pin = Gpio::D9;
	engineConfiguration->enableFan2WithAc = true;
	engineConfiguration->mainRelayPin = Gpio::I2;	// OUT_LOW3
	engineConfiguration->vvtPins[0] = Gpio::I0;    // 4R - VVT (O5)

    engineConfiguration->tachOutputPin = Gpio::D13; // 3O - TACH (PWM7)
    engineConfiguration->alternatorControlPin = Gpio::D15; // 3M - ALTERN (PWM6)


	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK



	hellenWbo();
}

void setup_custom_board_overrides() {
	custom_board_DefaultConfiguration = hellen_MiataNB2_boardDefaultConfiguration;
	custom_board_ConfigOverrides = hellen_MiataNB2_boardConfigOverrides;
}
