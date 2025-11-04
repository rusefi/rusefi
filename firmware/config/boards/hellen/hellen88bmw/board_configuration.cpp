/**
 * @file board_configuration.cpp
 *
 *
 * See https://rusefi.com/s/XXX
 *
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "hellen_meta.h"
#include "defaults.h"
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

	engineConfiguration->tps1_1AdcChannel = H144_IN_TPS;
	engineConfiguration->tps1_2AdcChannel = H144_IN_AUX1;

	setPPSInputs(EFI_ADC_3, EFI_ADC_14);
	engineConfiguration->mafAdcChannel = EFI_ADC_10;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_11;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;

	engineConfiguration->iat.adcChannel = H144_IN_IAT;
}



static void hellen88_boardConfigOverrides() {
	setHellenVbatt();

    // hellen88bmw uses L6205PD
	engineConfiguration->etbIo[0].directionPin1 = Gpio::H144_OUT_PWM2; // ETB+
	engineConfiguration->etbIo[0].directionPin2 = Gpio::H144_OUT_PWM3; // ETB-
	engineConfiguration->etbIo[0].controlPin = Gpio::H144_OUT_PWM1; // ETB_EN
	engineConfiguration->etb_use_two_wires = true;

	setHellenSdCardSpi3();

	engineConfiguration->clt.config.bias_resistor = 2700;
	engineConfiguration->iat.config.bias_resistor = 2700;
}

/**
 * @brief   Board-specific configuration defaults.
 *

 *

 */
static void hellen88_boardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();

	engineConfiguration->displayLogicLevelsInEngineSniffer = true;

	engineConfiguration->enableSoftwareKnock = true;
	engineConfiguration->canNbcType = CAN_BUS_NISSAN_VQ;

	setHellenCan();

	engineConfiguration->fuelPumpPin = Gpio::D12;	// OUT_IO9 // 113 Fuel Pump Relay
	engineConfiguration->idle.solenoidPin = Gpio::H144_OUT_PWM5;
//	engineConfiguration->fanPin = Gpio::D12;	// OUT_PWM8
	engineConfiguration->mainRelayPin = Gpio::H144_OUT_IO3;	// pin: 111a
	engineConfiguration->tachOutputPin = Gpio::H144_OUT_PWM7;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	// Some sensible defaults for other options
	setCrankOperationMode();

	setAlgorithm(engine_load_mode_e::LM_SPEED_DENSITY);

	// Bosch VQ40 VR56 VK56 0280158007
	engineConfiguration->injector.flow = 296.2;

	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_BMW);

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK

	engineConfiguration->luaOutputPins[0] = Gpio::G5; // 104 ETB Relay

	setPPSCalibration(0.75, 4.45, 0.43, 2.20);

	// this calibration reminds me of VAG just flipped?
	engineConfiguration->tpsMin = 100;
	engineConfiguration->tpsMax = 889;

	engineConfiguration->tps1SecondaryMin = 891;
	engineConfiguration->tps1SecondaryMax = 102;
	hellenWbo();
}

void setup_custom_board_overrides() {
	custom_board_DefaultConfiguration = hellen88_boardDefaultConfiguration;
	custom_board_ConfigOverrides =  hellen88_boardConfigOverrides;
}

