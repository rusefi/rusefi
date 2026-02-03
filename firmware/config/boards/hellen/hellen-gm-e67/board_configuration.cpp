/**
 * @file board_configuration.cpp
 *
 *
 * @brief Configuration defaults for the hellen-gm-e67 board
 *
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "hellen_meta.h"
#include "gm_ls_4.h"
#include "defaults.h"
#include "board_overrides.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = Gpio::H144_LS_1;
	engineConfiguration->injectionPins[1] = Gpio::H144_LS_2;
	engineConfiguration->injectionPins[2] = Gpio::H144_LS_3;
	engineConfiguration->injectionPins[3] = Gpio::H144_LS_4;
	engineConfiguration->injectionPins[4] = Gpio::H144_LS_5;
	engineConfiguration->injectionPins[5] = Gpio::H144_LS_6;
	engineConfiguration->injectionPins[6] = Gpio::H144_LS_7;
	engineConfiguration->injectionPins[7] = Gpio::H144_LS_8;

	engineConfiguration->clutchDownPin = Gpio::C4; // Clutch switch input
	engineConfiguration->clutchDownPinMode = PI_PULLDOWN;
	engineConfiguration->launchActivationMode = CLUTCH_INPUT_LAUNCH;
	engineConfiguration->malfunctionIndicatorPin = Gpio::H144_OUT_IO8;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::H144_IGN_1;
	engineConfiguration->ignitionPins[1] = Gpio::H144_IGN_2;
	engineConfiguration->ignitionPins[2] = Gpio::H144_IGN_3;
	engineConfiguration->ignitionPins[3] = Gpio::H144_IGN_4;
	engineConfiguration->ignitionPins[4] = Gpio::H144_IGN_5;
	engineConfiguration->ignitionPins[5] = Gpio::H144_IGN_6;
	engineConfiguration->ignitionPins[6] = Gpio::H144_IGN_7;
	engineConfiguration->ignitionPins[7] = Gpio::H144_IGN_8;
}

static void setupDefaultSensorInputs() {
	// trigger inputs, hall
	engineConfiguration->triggerInputPins[0] = Gpio::H144_IN_SENS2;
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
	engineConfiguration->camInputs[0] = Gpio::H144_IN_SENS4;

	setTPS1Inputs(H144_IN_TPS, H144_IN_AUX1_ANALOG);

	setPPSInputs(H144_IN_PPS, H144_IN_AUX2_ANALOG);

	engineConfiguration->mafAdcChannel = EFI_ADC_10;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_11;

	engineConfiguration->afr.hwChannel = EFI_ADC_1;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;

	engineConfiguration->iat.adcChannel = H144_IN_IAT;
}



static void hellen_gm_e67_boardConfigOverrides() {
	setHellenVbatt();

	setHellenSdCardSpi2();
    setDefaultHellenAtPullUps();

	setHellenCan();

	setupTLE9201(/*controlPin*/Gpio::H144_OUT_PWM8, Gpio::H144_OUT_IO13, Gpio::H144_OUT_IO4);
}

/**
 * @brief   Board-specific configuration defaults.
 *

 *

 */
static void hellen_gm_e67_boardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();

	engineConfiguration->enableSoftwareKnock = true;

	engineConfiguration->invertPrimaryTriggerSignal = true;

	engineConfiguration->luaOutputPins[0] = Gpio::H144_OUT_IO6; // starter relay

	engineConfiguration->boostControlPin = Gpio::H144_OUT_PWM5;
	engineConfiguration->brakePedalPin = Gpio::H144_IN_RES2;
//	engineConfiguration->acSwitch =
//	engineConfiguration->acRelayPin =
	engineConfiguration->fuelPumpPin = Gpio::H144_OUT_IO5;
	engineConfiguration->fanPin = Gpio::H144_OUT_IO12;
	engineConfiguration->mainRelayPin = Gpio::H144_OUT_IO3;
    engineConfiguration->tachOutputPin = Gpio::H144_OUT_PWM7;
	engineConfiguration->alternatorControlPin = Gpio::H144_OUT_PWM1;
//	engineConfiguration->fan2Pin =

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	setGmLs4();
	setEtbPID(7.4320, 117.6542, 0.0766);

	engineConfiguration->enableSoftwareKnock = true;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;

	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	engineConfiguration->clutchDownPin = Gpio::H144_IN_D_2;
	engineConfiguration->clutchDownPinMode = PI_PULLDOWN;
	engineConfiguration->launchActivationMode = CLUTCH_INPUT_LAUNCH;
// ?	engineConfiguration->malfunctionIndicatorPin = Gpio::G4; //1E - Check Engine Light

}

void setup_custom_board_overrides() {
	custom_board_DefaultConfiguration = hellen_gm_e67_boardDefaultConfiguration;
	custom_board_ConfigOverrides = hellen_gm_e67_boardConfigOverrides;
}
