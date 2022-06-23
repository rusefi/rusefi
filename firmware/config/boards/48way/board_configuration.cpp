/**
 * @file boards/48way/board_configuration.cpp
 *
 * @brief Configuration defaults for the 48way STM32 board
 *
 * @author Ben Brazdziunas,  2022
 */

#include "pch.h"

static void setInjectorPins() {
	engineConfiguration->injectionPinMode = OM_DEFAULT;

	engineConfiguration->injectionPins[0] = Gpio::E15;
	engineConfiguration->injectionPins[1] = Gpio::E14;
	engineConfiguration->injectionPins[2] = Gpio::E13;
	engineConfiguration->injectionPins[3] = Gpio::E12;
	engineConfiguration->injectionPins[4] = Gpio::E11;
	engineConfiguration->injectionPins[5] = Gpio::E10;
	engineConfiguration->injectionPins[6] = Gpio::E9;
	engineConfiguration->injectionPins[7] = Gpio::E8;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPinMode = OM_DEFAULT;

	engineConfiguration->ignitionPins[0] = Gpio::D12;
	engineConfiguration->ignitionPins[1] = Gpio::D13;
	engineConfiguration->ignitionPins[2] = Gpio::B15;
	engineConfiguration->ignitionPins[3] = Gpio::B14;
	engineConfiguration->ignitionPins[4] = Gpio::D8;
	engineConfiguration->ignitionPins[5] = Gpio::D9;
	engineConfiguration->ignitionPins[6] = Gpio::D11;
	engineConfiguration->ignitionPins[7] = Gpio::D10;
}

static void setEtbConfig() {

}

static void setupVbatt() {
	// 5.6k high side/10k low side = 1.56 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 1.56f;
	
	// 6.34k high side/ 1k low side
	engineConfiguration->vbattDividerCoeff = (6.34 + 1) / 1; 

	// Battery sense on PA7
	engineConfiguration->vbattAdcChannel = EFI_ADC_0;

	engineConfiguration->adcVcc = 3.3f;
}

static void setStepperConfig() {
	engineConfiguration->idle.stepperDirectionPin = Gpio::C9;
	engineConfiguration->idle.stepperStepPin = Gpio::C8;
	engineConfiguration->stepperEnablePin = Gpio::A8;
}

void setBoardConfigOverrides() {
	setupVbatt();
	//setEtbConfig();
	setStepperConfig();

	// PE3 is error LED, configured in board.mk
	engineConfiguration->communicationLedPin = Gpio::Unassigned;
	engineConfiguration->runningLedPin = Gpio::C10;
	engineConfiguration->warningLedPin = Gpio::Unassigned;

	engineConfiguration->clt.config.bias_resistor = 2490;
	engineConfiguration->iat.config.bias_resistor = 2490;

	//CAN 1 bus overwrites
	engineConfiguration->canRxPin = Gpio::D0;
	engineConfiguration->canTxPin = Gpio::D1;

	//CAN 2 bus overwrites
	engineConfiguration->can2RxPin = Gpio::B12;
	engineConfiguration->can2TxPin = Gpio::B13;
}

static void setupDefaultSensorInputs() {

	engineConfiguration->afr.hwChannel = EFI_ADC_4;
	setEgoSensor(ES_14Point7_Free);
	
	engineConfiguration->baroSensor.hwChannel = EFI_ADC_9;

}

void setBoardDefaultConfiguration(void) {
	setInjectorPins();
	setIgnitionPins();
	engineConfiguration->isSdCardEnabled = false;
	engineConfiguration->canBaudRate = B500KBPS;
	engineConfiguration->can2BaudRate = B500KBPS;
}
