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
#include "custom_engine.h"
#include "electronic_throttle_impl.h"
#include "hellen_meta.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = H176_LS_1; // 96 - INJ_1
	engineConfiguration->injectionPins[1] = H176_LS_2;
	engineConfiguration->injectionPins[2] = H176_LS_3; // 97 - INJ_3
	engineConfiguration->injectionPins[3] = H176_LS_4;

	// Disable remainder
	for (int i = 4; i < MAX_CYLINDER_COUNT;i++) {
		engineConfiguration->injectionPins[i] = Gpio::Unassigned;
	}

	engineConfiguration->injectionPinMode = OM_DEFAULT;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = H176_IGN_1; // 102 - IGN_1
	engineConfiguration->ignitionPins[1] = Gpio::E5 ; // 7 - IGN_2
	engineConfiguration->ignitionPins[2] = Gpio::E4; // 111 - IGN_3
	engineConfiguration->ignitionPins[3] = Gpio::E3; // 94 - IGN_4

	//engineConfiguration->ignitionPins[4] = Gpio::E2;
	//engineConfiguration->ignitionPins[5] = Gpio::I5;
	//engineConfiguration->ignitionPins[6] = Gpio::I6;
	//engineConfiguration->ignitionPins[7] = Gpio::I7;
	
	// disable remainder
	for (int i = 4; i < MAX_CYLINDER_COUNT; i++) {
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
	// trigger inputs
	engineConfiguration->triggerInputPins[0] = Gpio::B1; // 82 - VR
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
	// Direct hall-only cam input
	engineConfiguration->camInputs[0] = Gpio::A6; // 86 - CAM1

	engineConfiguration->tps1_1AdcChannel = H144_IN_TPS; // 92 - TPS 1
	engineConfiguration->tps1_2AdcChannel = H144_IN_AUX1;

	engineConfiguration->tps2_1AdcChannel = EFI_ADC_NONE;

	engineConfiguration->throttlePedalPositionAdcChannel = H144_IN_PPS; // 34 In PPS1
	engineConfiguration->throttlePedalPositionSecondAdcChannel = H144_IN_AUX2; // 35 In PPS2

	engineConfiguration->throttlePedalUpVoltage = 0.4;
	engineConfiguration->throttlePedalWOTVoltage = 2;
	engineConfiguration->throttlePedalSecondaryUpVoltage = 0.7;
	engineConfiguration->throttlePedalSecondaryWOTVoltage = 4.1;

	engineConfiguration->mafAdcChannel = EFI_ADC_10;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_11;

	engineConfiguration->afr.hwChannel = EFI_ADC_1;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;

	engineConfiguration->iat.adcChannel = H144_IN_IAT;

	engineConfiguration->auxTempSensor1.adcChannel = EFI_ADC_NONE;
	engineConfiguration->auxTempSensor2.adcChannel = EFI_ADC_NONE;
}

void setBoardConfigOverrides() {
	setHellen176LedPins();
	setupVbatt();

	setHellenSdCardSpi3();

	engineConfiguration->clt.config.bias_resistor = 4700;
	engineConfiguration->iat.config.bias_resistor = 4700;

	engineConfiguration->canTxPin = Gpio::D1;
	engineConfiguration->canRxPin = Gpio::D0;
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

	engineConfiguration->etbIo[0].directionPin1 = Gpio::C6; // out_pwm2
	engineConfiguration->etbIo[0].directionPin2 = Gpio::C7; // out_pwm3
	engineConfiguration->etbIo[0].controlPin = Gpio::A8; // ETB_EN out_io12
	engineConfiguration->etb_use_two_wires = true;

	setBoschVAGETB();

	engineConfiguration->globalTriggerAngleOffset = 93;

	engineConfiguration->isSdCardEnabled = true;

	// todo: should this be a global default not just Hellen121?
	engineConfiguration->boostCutPressure = 200;

	engineConfiguration->vvtMode[0] = VVT_BOSCH_QUICK_START;
	engineConfiguration->map.sensor.type = MT_BOSCH_2_5;

	engineConfiguration->enableSoftwareKnock = true;

	engineConfiguration->fuelPumpPin = H144_OUT_IO3;
	engineConfiguration->malfunctionIndicatorPin = Gpio::G4; // 47 - CEL
	engineConfiguration->tachOutputPin = H144_OUT_PWM7;

// earlier revisions?	engineConfiguration->idle.solenoidPin = Gpio::D14;	// OUT_PWM5
    engineConfiguration->idle.solenoidPin = H144_OUT_IO4;

	engineConfiguration->fanPin = H144_OUT_PWM8;
	engineConfiguration->mainRelayPin = H144_OUT_IO1;

//	engineConfiguration->injectorCompensationMode
	engineConfiguration->fuelReferencePressure = 300;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_VAG);
	strcpy(engineConfiguration->engineCode, "base");

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;//IM_BATCH;// IM_SEQUENTIAL;

	engineConfiguration->vrThreshold[0].pin = H144_OUT_PWM6;
	hellenWbo();
}
