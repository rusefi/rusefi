/**
 * @file boards/hellen/hellen-nb1/board_configuration.cpp
 *
 *
 * @brief Configuration defaults for the hellen-nb1 board
 *
 * See http://rusefi.com/s/hellenNB1
 *
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "custom_engine.h"
#include "hellen_meta.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = H144_LS_1;
	engineConfiguration->injectionPins[1] = H144_LS_2;
	engineConfiguration->injectionPins[2] = H144_LS_3;
	engineConfiguration->injectionPins[3] = H144_LS_4;
	engineConfiguration->injectionPins[4] = H144_LS_5;
	engineConfiguration->injectionPins[5] = H144_LS_6;
	engineConfiguration->injectionPins[6] = H144_LS_7;
	engineConfiguration->injectionPins[7] = H144_LS_8;

	// Disable remainder
	for (int i = 8; i < MAX_CYLINDER_COUNT;i++) {
		engineConfiguration->injectionPins[i] = Gpio::Unassigned;
	}

	engineConfiguration->injectionPinMode = OM_DEFAULT;

	engineConfiguration->clutchDownPin = Gpio::C4; // Clutch switch input
	engineConfiguration->clutchDownPinMode = PI_PULLDOWN;
	engineConfiguration->launchActivationMode = CLUTCH_INPUT_LAUNCH;
	engineConfiguration->malfunctionIndicatorPin = H144_OUT_IO8;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = H144_IGN_1;
	engineConfiguration->ignitionPins[1] = H144_IGN_2;
	engineConfiguration->ignitionPins[2] = H144_IGN_3;
	engineConfiguration->ignitionPins[3] = H144_IGN_4;
	engineConfiguration->ignitionPins[4] = H144_IGN_5;
	engineConfiguration->ignitionPins[5] = H144_IGN_6;
	engineConfiguration->ignitionPins[6] = H144_IGN_7;
	engineConfiguration->ignitionPins[7] = H144_IGN_8;

	// disable remainder
	for (int i = 8; i < MAX_CYLINDER_COUNT; i++) {
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
	// trigger inputs, hall
	engineConfiguration->triggerInputPins[0] = H144_IN_SENS2;
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
	engineConfiguration->camInputs[0] = H144_IN_SENS4;

	engineConfiguration->tps1_1AdcChannel = H144_IN_TPS;
	engineConfiguration->tps1_2AdcChannel = H144_IN_AUX1;
	engineConfiguration->tps2_1AdcChannel = EFI_ADC_NONE;

	engineConfiguration->throttlePedalPositionAdcChannel = H144_IN_PPS;
	engineConfiguration->throttlePedalPositionSecondAdcChannel = H144_IN_AUX2;

	engineConfiguration->mafAdcChannel = EFI_ADC_10;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_11;

	engineConfiguration->afr.hwChannel = EFI_ADC_1;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;

	engineConfiguration->iat.adcChannel = H144_IN_IAT;

	engineConfiguration->auxTempSensor1.adcChannel = EFI_ADC_NONE;
	engineConfiguration->auxTempSensor2.adcChannel = EFI_ADC_NONE;
}

void setBoardConfigOverrides() {
	setHellen144LedPins();
	setupVbatt();

	setHellenSdCardSpi2();
	engineConfiguration->clt.config.bias_resistor = 4700;
	engineConfiguration->iat.config.bias_resistor = 4700;

	engineConfiguration->canTxPin = Gpio::D1;
	engineConfiguration->canRxPin = Gpio::D0;

	// TLE9201 driver
    // This chip has three control pins:
    // DIR - sets direction of the motor
    // PWM - pwm control (enable high, coast low)
    // DIS - disables motor (enable low)

    //ETB1
    // PWM pin
    engineConfiguration->etbIo[0].controlPin = H144_OUT_PWM8;
    // DIR pin
	engineConfiguration->etbIo[0].directionPin1 = H144_OUT_IO13;
   	// Disable pin
   	engineConfiguration->etbIo[0].disablePin = H144_OUT_IO4;
   	// Unused
 	engineConfiguration->etbIo[0].directionPin2 = Gpio::Unassigned;


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

	engineConfiguration->boostControlPin = H144_OUT_PWM5;
	engineConfiguration->brakePedalPin = H144_IN_RES2;
//	engineConfiguration->acSwitch = H144_IN_D_AUX3;
//	engineConfiguration->acRelayPin = H144_OUT_IO6;
	engineConfiguration->fuelPumpPin = H144_OUT_IO5;
	engineConfiguration->fanPin = H144_OUT_IO12;
	engineConfiguration->mainRelayPin = H144_OUT_IO3;
    engineConfiguration->tachOutputPin = H144_OUT_PWM7;
	engineConfiguration->alternatorControlPin = H144_OUT_PWM1;
//	engineConfiguration->fan2Pin = H144_OUT_IO2;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	engineConfiguration->specs.cylindersCount = 8;
	engineConfiguration->specs.firingOrder = FO_1_8_7_2_6_5_4_3;

	engineConfiguration->enableSoftwareKnock = true;

	// random values to have valid config
	engineConfiguration->tpsMin = 0;
	engineConfiguration->tpsMax = 1000;
	// random values to have valid config
	engineConfiguration->tps1SecondaryMin = 1000;
	engineConfiguration->tps1SecondaryMax = 0;
	// random values to have valid config
	engineConfiguration->throttlePedalUpVoltage = 0;
	engineConfiguration->throttlePedalWOTVoltage = 5.0;
	// random values to have valid config
	engineConfiguration->throttlePedalSecondaryUpVoltage = 5.0;
	engineConfiguration->throttlePedalSecondaryWOTVoltage = 0.0;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	engineConfiguration->clutchDownPin = H144_IN_D_2;
	engineConfiguration->clutchDownPinMode = PI_PULLDOWN;
	engineConfiguration->launchActivationMode = CLUTCH_INPUT_LAUNCH;
// ?	engineConfiguration->malfunctionIndicatorPin = Gpio::G4; //1E - Check Engine Light
}
