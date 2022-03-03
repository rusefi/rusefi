/**
 * @file boards/core8/board_configuration.cpp
 *
 * @brief Configuration defaults for the core8 board
 *
 * @author Ben Brazdziunas,  2022
 */

#include "pch.h"

static void setInjectorPins() {
	engineConfiguration->injectionPinMode = OM_DEFAULT;

	engineConfiguration->injectionPins[0] = GPIOF_13;
	engineConfiguration->injectionPins[1] = GPIOF_14;
	engineConfiguration->injectionPins[2] = GPIOD_8;
	engineConfiguration->injectionPins[3] = GPIOD_9;
	engineConfiguration->injectionPins[4] = GPIOD_10;
	engineConfiguration->injectionPins[5] = GPIOD_11;
	engineConfiguration->injectionPins[6] = GPIOD_12;
	engineConfiguration->injectionPins[7] = GPIOD_13;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPinMode = OM_DEFAULT;

	engineConfiguration->ignitionPins[0] = GPIOE_15;
	engineConfiguration->ignitionPins[1] = GPIOE_14;
	engineConfiguration->ignitionPins[2] = GPIOE_13;
	engineConfiguration->ignitionPins[3] = GPIOE_12;
	engineConfiguration->ignitionPins[4] = GPIOE_11;
	engineConfiguration->ignitionPins[5] = GPIOF_15;
	engineConfiguration->ignitionPins[6] = GPIOG_0;
	engineConfiguration->ignitionPins[7] = GPIOG_1;
}


void setSdCardConfigurationOverrides(void) {
}

static void setEtbConfig() {
	// TLE9201 driver
	// This chip has three control pins:
	// DIR - sets direction of the motor
	// PWM - pwm control (enable high, coast low)
	// DIS - disables motor (enable low)

	// Throttle #1
	// PWM pin
	engineConfiguration->etbIo[0].controlPin = GPIOB_8;
	// DIR pin
	engineConfiguration->etbIo[0].directionPin1 = GPIOB_9;
	// Disable pin
	engineConfiguration->etbIo[0].disablePin = GPIOB_7;
	// Unused
	engineConfiguration->etbIo[0].directionPin2 = GPIO_UNASSIGNED;

	// Throttle #2
	// PWM pin
	engineConfiguration->etbIo[1].controlPin = GPIO_UNASSIGNED;
	// DIR pin
	engineConfiguration->etbIo[1].directionPin1 = GPIO_UNASSIGNED;
	// Disable pin
	engineConfiguration->etbIo[1].disablePin = GPIO_UNASSIGNED;
	// Unused
	engineConfiguration->etbIo[1].directionPin2 = GPIO_UNASSIGNED;

	// we only have pwm/dir, no dira/dirb
	engineConfiguration->etb_use_two_wires = false;
}

static void setupVbatt() {
	// 5.6k high side/10k low side = 1.56 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 1.47f;

	// 6.34k high side/1k low side = 9.2
	engineConfiguration->vbattDividerCoeff = (6.34f / 1.0f);

	// Battery sense on PA7
	engineConfiguration->vbattAdcChannel = EFI_ADC_0;

	engineConfiguration->adcVcc = 3.3f;
}

static void setStepperConfig() {
	engineConfiguration->idle.stepperDirectionPin = GPIOF_7;
	engineConfiguration->idle.stepperStepPin = GPIOF_8;
	engineConfiguration->stepperEnablePin = GPIOF_9;
}

void setBoardConfigOverrides() {
	setupVbatt();
	setEtbConfig();
	setStepperConfig();

	// PE3 is error LED, configured in board.mk
	engineConfiguration->communicationLedPin = GPIOG_12;
	engineConfiguration->runningLedPin = GPIOG_9;
	engineConfiguration->warningLedPin = GPIOG_10;

	engineConfiguration->clt.config.bias_resistor = 2490;
	engineConfiguration->iat.config.bias_resistor = 2490;

	//CAN 1 bus overwrites
	engineConfiguration->canRxPin = GPIOD_0;
	engineConfiguration->canTxPin = GPIOD_1;

	//CAN 2 bus overwrites
	engineConfiguration->can2RxPin = GPIOB_5;
	engineConfiguration->can2TxPin = GPIOB_6;
}

static void setupDefaultSensorInputs() {

	engineConfiguration->afr.hwChannel = EFI_ADC_11;
	setEgoSensor(ES_14Point7_Free);
	
	engineConfiguration->baroSensor.hwChannel = EFI_ADC_NONE;

	engineConfiguration->lps25BaroSensorScl = GPIOB_10;
	engineConfiguration->lps25BaroSensorSda = GPIOB_11;

}

void setBoardDefaultConfiguration(void) {
	setInjectorPins();
	setIgnitionPins();

	engineConfiguration->sdCardPeriodMs = 50;
	engineConfiguration->isSdCardEnabled = true;

	engineConfiguration->canWriteEnabled = true;
	engineConfiguration->canReadEnabled = true;
	engineConfiguration->canSleepPeriodMs = 50;

	engineConfiguration->canBaudRate = B500KBPS;
	engineConfiguration->can2BaudRate = B500KBPS;
}
