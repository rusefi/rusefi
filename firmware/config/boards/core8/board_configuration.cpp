/**
 * @file boards/core8/board_configuration.cpp
 *
 * @brief Configuration defaults for the core8 board
 *
 * @author Ben Brazdziunas,  2022
 */

#include "pch.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = Gpio::F13;
	engineConfiguration->injectionPins[1] = Gpio::F14;
	engineConfiguration->injectionPins[2] = Gpio::D8;
	engineConfiguration->injectionPins[3] = Gpio::D9;
	engineConfiguration->injectionPins[4] = Gpio::D10;
	engineConfiguration->injectionPins[5] = Gpio::D11;
	engineConfiguration->injectionPins[6] = Gpio::D12;
	engineConfiguration->injectionPins[7] = Gpio::D13;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::E15;
	engineConfiguration->ignitionPins[1] = Gpio::E14;
	engineConfiguration->ignitionPins[2] = Gpio::E13;
	engineConfiguration->ignitionPins[3] = Gpio::E12;
	engineConfiguration->ignitionPins[4] = Gpio::E11;
	engineConfiguration->ignitionPins[5] = Gpio::F15;
	engineConfiguration->ignitionPins[6] = Gpio::G0;
	engineConfiguration->ignitionPins[7] = Gpio::G1;
}

static void setEtbConfig() {
	// TLE9201 driver
	// This chip has three control pins:
	// DIR - sets direction of the motor
	// PWM - pwm control (enable high, coast low)
	// DIS - disables motor (enable low)

	// Throttle #1
	// PWM pin
	engineConfiguration->etbIo[0].controlPin = Gpio::B8;
	// DIR pin
	engineConfiguration->etbIo[0].directionPin1 = Gpio::B9;
	// Disable pin
	engineConfiguration->etbIo[0].disablePin = Gpio::B7;

	// Throttle #2
	// PWM pin
	engineConfiguration->etbIo[1].controlPin = Gpio::Unassigned;
	// DIR pin
	engineConfiguration->etbIo[1].directionPin1 = Gpio::Unassigned;
	// Disable pin
	engineConfiguration->etbIo[1].disablePin = Gpio::Unassigned;

	// we only have pwm/dir, no dira/dirb
	engineConfiguration->etb_use_two_wires = false;
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
	engineConfiguration->idle.stepperDirectionPin = Gpio::F7;
	engineConfiguration->idle.stepperStepPin = Gpio::F8;
	engineConfiguration->stepperEnablePin = Gpio::F9;
}

// PE3 is error LED, configured in board.mk
Gpio getCommsLedPin() {
	return Gpio::G12;
}

Gpio getRunningLedPin() {
	return Gpio::G9;
}

Gpio getWarningLedPin() {
	return Gpio::G10;
}

void setBoardConfigOverrides() {
	setupVbatt();
	setEtbConfig();
	setStepperConfig();

	engineConfiguration->clt.config.bias_resistor = 2490;
	engineConfiguration->iat.config.bias_resistor = 2490;

	//CAN 1 bus overwrites
	engineConfiguration->canRxPin = Gpio::D0;
	engineConfiguration->canTxPin = Gpio::D1;

	//CAN 2 bus overwrites
	engineConfiguration->can2RxPin = Gpio::B5;
	engineConfiguration->can2TxPin = Gpio::B6;
}

void setBoardDefaultConfiguration(void) {
	setInjectorPins();
	setIgnitionPins();

	engineConfiguration->isSdCardEnabled = true;
}
