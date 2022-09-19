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
#include "custom_engine.h"
#include "hellen_meta.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = Gpio::G7;
	engineConfiguration->injectionPins[1] = Gpio::G8;
	engineConfiguration->injectionPins[2] = Gpio::D11;
	engineConfiguration->injectionPins[3] = Gpio::D10;

	//engineConfiguration->injectionPins[4] = Gpio::D9;
	//engineConfiguration->injectionPins[5] = Gpio::F12;
	//engineConfiguration->injectionPins[6] = Gpio::F13;
	//engineConfiguration->injectionPins[7] = Gpio::F14;

	// Disable remainder
	for (int i = 4; i < MAX_CYLINDER_COUNT;i++) {
		engineConfiguration->injectionPins[i] = Gpio::Unassigned;
	}

	engineConfiguration->injectionPinMode = OM_DEFAULT;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = H176_IGN_1; // 3F - IGN_1 (1&4)
	engineConfiguration->ignitionPins[1] = Gpio::Unassigned ; // Gpio::E4
	engineConfiguration->ignitionPins[2] = Gpio::E5; // 3I - IGN_2 (2&3)
	engineConfiguration->ignitionPins[3] = Gpio::Unassigned; // Gpio::E3

	//engineConfiguration->ignitionPins[4] = Gpio::E2;
	//engineConfiguration->ignitionPins[5] = Gpio::I5;
	//engineConfiguration->ignitionPins[6] = Gpio::I6;
	//engineConfiguration->ignitionPins[7] = Gpio::I7;
	
	// disable remainder
	for (int i = 4; i < MAX_CYLINDER_COUNT; i++) {
		engineConfiguration->ignitionPins[i] = Gpio::Unassigned;
	}

	engineConfiguration->ignitionPinMode = OM_DEFAULT;
}

static void setupVbatt() {
	// 4.7k high side/4.7k low side = 2.0 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 2.0f;

	// set vbatt_divider 5.835
	// 33k / 6.8k
	engineConfiguration->vbattDividerCoeff = (33 + 6.8) / 6.8; // 5.835

	// pin input +12 from Main Relay
	engineConfiguration->vbattAdcChannel = EFI_ADC_5; // 4T

	engineConfiguration->adcVcc = 3.29f;
}

static void setupDefaultSensorInputs() {
	// trigger inputs
	engineConfiguration->triggerInputPins[0] = Gpio::B1;
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
	// Direct hall-only cam input
	engineConfiguration->camInputs[0] = Gpio::A6;

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_4;
	engineConfiguration->tps2_1AdcChannel = EFI_ADC_NONE;

	engineConfiguration->mafAdcChannel = EFI_ADC_10;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_11;

	engineConfiguration->afr.hwChannel = EFI_ADC_1;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;

	engineConfiguration->iat.adcChannel = H144_IN_IAT;

	engineConfiguration->auxTempSensor1.adcChannel = EFI_ADC_NONE;
	engineConfiguration->auxTempSensor2.adcChannel = EFI_ADC_NONE;
}

void setBoardConfigOverrides() {
	setHellen176LedPins();
	setupVbatt();

	if (engine->engineState.hellenBoardId == -1) {
		// Rev a-d use SPI3 for SD card
		setHellenSdCardSpi3();
	} else {
		// Revs E and later use SPI2 for SD card
		setHellenSdCardSpi2();
	}

    // NB2 still uses L6205PD
	engineConfiguration->etbIo[0].directionPin1 = H144_OUT_PWM3; // ETB+
	engineConfiguration->etbIo[0].directionPin2 = H144_OUT_PWM4; // ETB-
	engineConfiguration->etbIo[0].controlPin = H144_OUT_PWM2; // ETB_EN
	engineConfiguration->etb_use_two_wires = true;

	engineConfiguration->clt.config.bias_resistor = 4700;
	engineConfiguration->iat.config.bias_resistor = 4700;

	engineConfiguration->canTxPin = Gpio::D1;
	engineConfiguration->canRxPin = Gpio::D0;
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

	engineConfiguration->enableSoftwareKnock = true;

	engineConfiguration->acRelayPin = Gpio::H15;
	engineConfiguration->acSwitch = Gpio::B0;
	engineConfiguration->acSwitchMode = PI_PULLUP;

	engineConfiguration->vehicleSpeedSensorInputPin = H144_IN_VSS;
	engineConfiguration->clutchDownPin = H144_IN_RES3;
	engineConfiguration->clutchDownPinInverted = true;

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

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;//IM_BATCH;// IM_SEQUENTIAL;

	hellenWbo();
}
