/**
 * @file boards/hellen/hellen121vag/board_configuration.cpp
 *
 *
 * @brief Configuration defaults for the hellen121vag board
 *
 * See https://rusefi.com/s/hellen121vag
 *
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "hellen_all_meta.h"
#include "defaults.h"
#include "board_overrides.h"

#ifndef EFI_BOOTLOADER
#include "electronic_throttle.h"
#endif

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = H176_LS_1; // 96 - INJ_1
	engineConfiguration->injectionPins[1] = H176_LS_2;
	engineConfiguration->injectionPins[2] = H176_LS_3; // 97 - INJ_3
	engineConfiguration->injectionPins[3] = H176_LS_4;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = H176_IGN_1; // 102 - IGN_1
	engineConfiguration->ignitionPins[1] = Gpio::E5 ; // 7 - IGN_2
	engineConfiguration->ignitionPins[2] = Gpio::E4; // 111 - IGN_3
	engineConfiguration->ignitionPins[3] = Gpio::E3; // 94 - IGN_4
}

static void setupDefaultSensorInputs() {
	// trigger inputs
	engineConfiguration->triggerInputPins[0] = Gpio::B1; // 82 - VR
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
	// Direct hall-only cam input
	engineConfiguration->camInputs[0] = Gpio::A6; // 86 - CAM1

    // 92 - TPS 1
	setTPS1Inputs(H144_IN_TPS, H144_IN_AUX1_ANALOG);

    // 34 In PPS1
    // 35 In PPS2
    setPPSInputs(H144_IN_PPS, H144_IN_AUX2_ANALOG);

	setPPSCalibration(0.4, 2, 0.7, 4.1);

	engineConfiguration->mafAdcChannel = EFI_ADC_10;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_11;

	engineConfiguration->afr.hwChannel = EFI_ADC_1;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;

	engineConfiguration->iat.adcChannel = H144_IN_IAT;
}

#include "hellen_leds_176.cpp"

static void hellen121_vag_boardConfigOverrides() {
	setHellenVbatt();

	setHellenSdCardSpi3();

    setDefaultHellenAtPullUps();

	setHellenCan();
}

/**
 * @brief   Board-specific configuration defaults.
 *

 *

 */
static void hellen121_vag_boardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();

	engineConfiguration->etbIo[0].directionPin1 = Gpio::C6; // out_pwm2
	engineConfiguration->etbIo[0].directionPin2 = Gpio::C7; // out_pwm3
	engineConfiguration->etbIo[0].controlPin = Gpio::A8; // ETB_EN out_io12
	engineConfiguration->etb_use_two_wires = true;

#ifndef EFI_BOOTLOADER
	setBoschVAGETB();
#endif

	engineConfiguration->globalTriggerAngleOffset = 93;

	engineConfiguration->boostCutPressure = 200;

	engineConfiguration->vvtMode[0] = VVT_BOSCH_QUICK_START;
	engineConfiguration->map.sensor.type = MT_BOSCH_2_5;

	engineConfiguration->enableSoftwareKnock = true;

	engineConfiguration->fuelPumpPin = Gpio::MM176_GP13;
	engineConfiguration->malfunctionIndicatorPin = Gpio::G4; // 47 - CEL
	engineConfiguration->tachOutputPin = Gpio::H144_OUT_PWM7;

// earlier revisions?	engineConfiguration->idle.solenoidPin = Gpio::D14;	// OUT_PWM5
    engineConfiguration->idle.solenoidPin = Gpio::H144_OUT_IO4;

	engineConfiguration->fanPin = Gpio::H144_OUT_PWM8;
	engineConfiguration->mainRelayPin = Gpio::MM176_OUT_PWM17;

//	engineConfiguration->injectorCompensationMode
	engineConfiguration->fuelReferencePressure = 300;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_VAG);
	strcpy(engineConfiguration->engineCode, "base");

	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK

	engineConfiguration->vrThreshold[0].pin = Gpio::H144_OUT_PWM6;
}

int getBoardMetaDcOutputsCount() {
    return 1;
}

void setup_custom_board_overrides() {
	custom_board_DefaultConfiguration = hellen121_vag_boardDefaultConfiguration;
	custom_board_ConfigOverrides =  hellen121_vag_boardConfigOverrides;
}
