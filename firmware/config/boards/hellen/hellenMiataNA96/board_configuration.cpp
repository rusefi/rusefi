/**
 * @file board_configuration.cpp
 *
 *
 * @brief Configuration defaults for the hellen-na8-96 board
 *
 * See http://rusefi.com/s/hellenNB1
 *
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "hellen_meta.h"
#include "board_overrides.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = Gpio::H144_LS_1;
	engineConfiguration->injectionPins[1] = Gpio::H144_LS_2;
	engineConfiguration->injectionPins[2] = Gpio::H144_LS_3;
	engineConfiguration->injectionPins[3] = Gpio::H144_LS_4;

	engineConfiguration->clutchDownPin = Gpio::C4; // Clutch switch input
	engineConfiguration->clutchDownPinMode = PI_PULLDOWN;
	engineConfiguration->launchActivationMode = CLUTCH_INPUT_LAUNCH;
	engineConfiguration->malfunctionIndicatorPin = Gpio::H144_OUT_IO8;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::H144_IGN_1;
	engineConfiguration->ignitionPins[1] = Gpio::Unassigned;
	engineConfiguration->ignitionPins[2] = Gpio::H144_IGN_2;
	engineConfiguration->ignitionPins[3] = Gpio::Unassigned;
}

static void setupDefaultSensorInputs() {
	// trigger inputs, hall
	engineConfiguration->triggerInputPins[0] = Gpio::H144_IN_CAM;
	engineConfiguration->triggerInputPins[1] = Gpio::H144_IN_CRANK;
	engineConfiguration->camInputs[0] = Gpio::Unassigned;

	engineConfiguration->tps1_1AdcChannel = H144_IN_TPS;

	engineConfiguration->mafAdcChannel = EFI_ADC_10;
	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP3;

	engineConfiguration->afr.hwChannel = EFI_ADC_1;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;

	engineConfiguration->iat.adcChannel = H144_IN_IAT;
}



static void hellen_MiataNA96_boardConfigOverrides() {
	setHellenVbatt();

	setHellenSdCardSpi2();

    setDefaultHellenAtPullUps();

	setHellenCan();

	engineConfiguration->etb_use_two_wires = true;
	engineConfiguration->etbIo[0].directionPin1 = Gpio::C7;	// PWM 3
	engineConfiguration->etbIo[0].directionPin2 = Gpio::C8;	// PWM 4
	engineConfiguration->etbIo[0].controlPin = Gpio::C6;		// PWM 2
}

/**
 * @brief   Board-specific configuration defaults.
 *

 *

 */
static void hellen_MiataNA96_boardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();

	engineConfiguration->enableSoftwareKnock = true;

	engineConfiguration->boostControlPin = Gpio::H144_LS_6;
	engineConfiguration->acSwitch = Gpio::H144_ORIGINAL_MCU_IN_D_AUX3;
	engineConfiguration->acRelayPin = Gpio::H144_OUT_IO6;
	engineConfiguration->fuelPumpPin = Gpio::G2;	// OUT_IO9
	engineConfiguration->idle.solenoidPin = Gpio::D14;	// OUT_PWM5
	engineConfiguration->fanPin = Gpio::D12;	// OUT_PWM8
    engineConfiguration->tachOutputPin = Gpio::H144_OUT_PWM1;
	engineConfiguration->fan2Pin = Gpio::H144_OUT_IO2;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK



	engineConfiguration->clutchDownPin = Gpio::H144_IN_D_2;
	engineConfiguration->clutchDownPinMode = PI_PULLDOWN;
	engineConfiguration->launchActivationMode = CLUTCH_INPUT_LAUNCH;
// ?	engineConfiguration->malfunctionIndicatorPin = Gpio::G4; //1E - Check Engine Light
}

void setup_custom_board_overrides() {
	custom_board_DefaultConfiguration = hellen_MiataNA96_boardDefaultConfiguration;
	custom_board_ConfigOverrides =  hellen_MiataNA96_boardConfigOverrides;
}
