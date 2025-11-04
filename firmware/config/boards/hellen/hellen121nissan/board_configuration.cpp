/**
 * @file boards/hellen/hellen121nissan/board_configuration.cpp
 *
 * All fabricated boards use 144 pin MCU
 *
 * @brief Configuration defaults for the hellen121nissan board
 *
 * See https://rusefi.com/s/hellen121nissan
 *
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "defaults.h"
#include "hellen_meta.h"
#include "board_overrides.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = Gpio::H144_LS_1;
	engineConfiguration->injectionPins[1] = Gpio::H144_LS_2;
	engineConfiguration->injectionPins[2] = Gpio::H144_LS_3;
	engineConfiguration->injectionPins[3] = Gpio::H144_LS_4;
	engineConfiguration->injectionPins[4] = Gpio::H144_LS_5;
	engineConfiguration->injectionPins[5] = Gpio::H144_LS_6;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::H144_IGN_1;
	engineConfiguration->ignitionPins[1] = Gpio::H144_IGN_2;
	engineConfiguration->ignitionPins[2] = Gpio::H144_IGN_3;
	engineConfiguration->ignitionPins[3] = Gpio::H144_IGN_4;
	engineConfiguration->ignitionPins[4] = Gpio::H144_IGN_5;
	engineConfiguration->ignitionPins[5] = Gpio::H144_IGN_6;
}

static void setupDefaultSensorInputs() {
	// trigger inputs
	engineConfiguration->triggerInputPins[0] = Gpio::H144_IN_CRANK;
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
	// Direct hall-only cam input
	engineConfiguration->camInputs[0] = Gpio::H144_IN_CAM;
	// todo: remove from default since 4 cylinder does not use it
	// todo: this requires unit test change
	engineConfiguration->camInputs[1 * CAMS_PER_BANK] = Gpio::H144_ORIGINAL_MCU_IN_D_AUX4;

	setTPS1Inputs(H144_IN_TPS, H144_IN_AUX1_ANALOG);

	setPPSInputs(H144_IN_PPS, EFI_ADC_14);
	engineConfiguration->mafAdcChannel = H144_IN_O2S;
	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP2;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;

	engineConfiguration->iat.adcChannel = H144_IN_IAT;
}

static void hellen121_nissan_boardConfigOverrides() {
	setHellenVbatt();
	setHellenSdCardSpi3();

	engineConfiguration->etbIo[0].directionPin1 = Gpio::H144_OUT_PWM7;
	engineConfiguration->etbIo[0].directionPin2 = Gpio::H144_OUT_PWM6;
	engineConfiguration->etbIo[0].controlPin = Gpio::D13; // ETB_EN out_pwm1
	engineConfiguration->etb_use_two_wires = true;

    setDefaultHellenAtPullUps();
}

/**
 * @brief   Board-specific configuration defaults.
 *

 *

 */
static void hellen121_nissan_boardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();

	engineConfiguration->displayLogicLevelsInEngineSniffer = true;

	engineConfiguration->enableSoftwareKnock = true;
	engineConfiguration->canNbcType = CAN_BUS_NBC_NONE; // none because handled by Lua!

	setHellenCan();

	engineConfiguration->fuelPumpPin = Gpio::D12;	// OUT_IO9 // 113 Fuel Pump Relay
	engineConfiguration->idle.solenoidPin = Gpio::Unassigned;
//	engineConfiguration->fanPin = Gpio::D12;	// OUT_PWM8
	engineConfiguration->mainRelayPin = Gpio::H144_OUT_IO3;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();


	// Some sensible defaults for other options
	setCrankOperationMode();

//	setAlgorithm(engine_load_mode_e::LM_SPEED_DENSITY);
    // at least this starts
	engineConfiguration->fuelAlgorithm = engine_load_mode_e::LM_ALPHA_N;

	engineConfiguration->cranking.rpm = 400;
	engineConfiguration->fanOnTemperature = 85;
	engineConfiguration->fanOffTemperature = 81;

	engineConfiguration->cutFuelOnHardLimit = false;
	engineConfiguration->idlePidRpmUpperLimit = 300;
	engineConfiguration->mapErrorDetectionTooLow = 10;


	// Bosch VQ40 VR56 VK56 0280158007
	engineConfiguration->injector.flow = 296.2;

	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_NISSAN);

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	engineConfiguration->luaOutputPins[0] = Gpio::H144_OUT_IO4; // 104 ETB Relay
	engineConfiguration->luaOutputPins[1] = Gpio::H144_OUT_PWM2; // aux LS1, upstream oxygen heaters

	setPPSCalibration(0.75, 4.45, 0.43, 2.20);

	engineConfiguration->startUpFuelPumpDuration = 4;

    setEtbPID(6.1350, 87.7182, 0.0702);

	// this calibration reminds me of VAG just flipped?
	engineConfiguration->tpsMin = 100;
	engineConfiguration->tpsMax = 889;

	engineConfiguration->tps1SecondaryMin = 891;
	engineConfiguration->tps1SecondaryMax = 102;
	hellenWbo();
}

static Gpio OUTPUTS[] = {
	Gpio::G14, // 111 Main Relay
	Gpio::D12, // 113 Fuel Pump Relay
	Gpio::D9, // 21 - INJ_5
	Gpio::D11, // 22 - INJ_3
	Gpio::G7, // 23 - INJ_1
	Gpio::F12, // 40 - INJ_6
	Gpio::D10, // 41 - INJ_4
	Gpio::G8, // 42 - INJ_2
	Gpio::F13, // 10 - VTC Left
	Gpio::F14, // 11 - VTC Right
};

int getBoardMetaOutputsCount() {
    return efi::size(OUTPUTS);
}

Gpio* getBoardMetaOutputs() {
    return OUTPUTS;
}

int getBoardMetaDcOutputsCount() {
    return 1;
}

void setup_custom_board_overrides() {
	custom_board_DefaultConfiguration = hellen121_nissan_boardDefaultConfiguration;
	custom_board_ConfigOverrides = hellen121_nissan_boardConfigOverrides;
}
