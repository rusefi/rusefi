/**
 * @file boards/coreECU/board_configuration.cpp
 *
 * @brief Configuration defaults for the coreECU board
 *
 * @author Turbo Marian,  2022
 */

#include "pch.h"

static void setInjectorPins() {
	
	engineConfiguration->injectionPinMode = OM_DEFAULT;

	engineConfiguration->injectionPins[0] = Gpio::D12;
	engineConfiguration->injectionPins[1] = Gpio::D13;
	engineConfiguration->injectionPins[2] = Gpio::D14;
	engineConfiguration->injectionPins[3] = Gpio::D15;
	engineConfiguration->injectionPins[4] = Gpio::C6;
	engineConfiguration->injectionPins[5] = Gpio::C7;
	engineConfiguration->injectionPins[6] = Gpio::C9;
	engineConfiguration->injectionPins[7] = Gpio::A8;
}

static void setIgnitionPins() {
	
	engineConfiguration->ignitionPinMode = OM_DEFAULT;

	engineConfiguration->ignitionPins[0] = Gpio::E15;
	engineConfiguration->ignitionPins[1] = Gpio::E14;
	engineConfiguration->ignitionPins[2] = Gpio::E13;
	engineConfiguration->ignitionPins[3] = Gpio::E12;
	engineConfiguration->ignitionPins[4] = Gpio::E11;
	engineConfiguration->ignitionPins[5] = Gpio::E10;
	engineConfiguration->ignitionPins[6] = Gpio::E9;
	engineConfiguration->ignitionPins[7] = Gpio::E8;
}


void setSdCardConfigurationOverrides(void) {
}

void setBoardConfigOverrides(void) {
	
	// TLE9201 driver
	// This chip has three control pins:
	// DIR - sets direction of the motor
	// PWM - pwm control (enable high, coast low)
	// DIS - disables motor (enable low)

	// Throttle #1
	// PWM pin
	engineConfiguration->etbIo[0].controlPin = Gpio::E7;
	// DIR pin
	engineConfiguration->etbIo[0].directionPin1 = Gpio::B1;
	// Disable pin
	engineConfiguration->etbIo[0].disablePin = Gpio::B0;
	// Unused
	engineConfiguration->etbIo[0].directionPin2 = Gpio::Unassigned;

	// Throttle #2
	// PWM pin
	engineConfiguration->etbIo[1].controlPin = Gpio::Unassigned;
	// DIR pin
	engineConfiguration->etbIo[1].directionPin1 = Gpio::Unassigned;
	// Disable pin
	engineConfiguration->etbIo[1].disablePin = Gpio::Unassigned;
	// Unused
	engineConfiguration->etbIo[1].directionPin2 = Gpio::Unassigned;

	// we only have pwm/dir, no dira/dirb
	engineConfiguration->etb_use_two_wires = false;
}

void setPinConfigurationOverrides(void) {

	//CAN 1 bus overwrites
	engineConfiguration->canTxPin = Gpio::D1;
	engineConfiguration->canRxPin = Gpio::D0;

	//CAN 2 bus overwrites
	engineConfiguration->can2RxPin = Gpio::Unassigned;
	engineConfiguration->can2TxPin = Gpio::Unassigned;
}

/**
 * @brief   Board-specific configuration defaults.
 *
 * See also setDefaultEngineConfiguration
 *
 * @todo    Add your board-specific code, if any.
 */
static void setupVbatt() {
	// 5.6k high side/10k low side = 1.56 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 1.47f;

	// 6.34k high side/1k low side = 9.2
	engineConfiguration->vbattDividerCoeff = (6.34f / 1.0f);

	// Battery sense on PC4
	engineConfiguration->vbattAdcChannel = EFI_ADC_14;

	engineConfiguration->adcVcc = 3.3f;
}

static void setupDefaultSensorInputs() {

	engineConfiguration->afr.hwChannel = EFI_ADC_6;
	setEgoSensor(ES_14Point7_Free);
	
	engineConfiguration->clt.config.bias_resistor = 2490;
	engineConfiguration->iat.config.bias_resistor = 2490;

	engineConfiguration->baroSensor.hwChannel = EFI_ADC_NONE;

	engineConfiguration->lps25BaroSensorScl = Gpio::Unassigned;
	engineConfiguration->lps25BaroSensorSda = Gpio::Unassigned;
}


static void setLedPins() {
	// PE3 is error LED, configured in board.mk
	engineConfiguration->communicationLedPin = Gpio::B8;
	engineConfiguration->runningLedPin = Gpio::B7;
	engineConfiguration->warningLedPin = Gpio::B6;
}

void setBoardDefaultConfiguration(void) {
	
	setInjectorPins();
	setIgnitionPins();
	setupVbatt();	
	setLedPins();

	
	//engineConfiguration->sdCardPeriodMs = 50;
	engineConfiguration->isSdCardEnabled = true;


	engineConfiguration->canWriteEnabled = true;
	engineConfiguration->canReadEnabled = true;
	engineConfiguration->canSleepPeriodMs = 50;

	engineConfiguration->canBaudRate = B500KBPS;
	engineConfiguration->can2BaudRate = B500KBPS;
	

}