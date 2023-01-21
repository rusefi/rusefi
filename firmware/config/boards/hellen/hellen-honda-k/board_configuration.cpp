/**
 * @file boards/hellen/hellen-honda-k/board_configuration.cpp
 *
 * See https://rusefi.com/s/XX
 *
 * @author Andrey Belomutskiy, (c) 2012-2023
 */

#include "pch.h"
#include "defaults.h"
#include "hellen_meta.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = H144_LS_1;
	engineConfiguration->injectionPins[1] = H144_LS_2;
	engineConfiguration->injectionPins[2] = H144_LS_3;
	engineConfiguration->injectionPins[3] = H144_LS_4;

	engineConfiguration->injectionPinMode = OM_DEFAULT;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = H144_IGN_1;
	engineConfiguration->ignitionPins[1] = H144_IGN_2;
	engineConfiguration->ignitionPins[2] = H144_IGN_3;
	engineConfiguration->ignitionPins[3] = H144_IGN_4;
	
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
	engineConfiguration->vvtMode[0] = VVT_SECOND_HALF;
	engineConfiguration->vvtMode[1] = VVT_SECOND_HALF;

    engineConfiguration->vehicleSpeedSensorInputPin = H144_IN_VSS;

	engineConfiguration->tps1_1AdcChannel = H144_IN_TPS;
	engineConfiguration->useETBforIdleControl = true;

	engineConfiguration->mafAdcChannel = EFI_ADC_NONE;
	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP1;

	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;

	engineConfiguration->iat.adcChannel = H144_IN_IAT;
}

void setBoardConfigOverrides() {
	setHellen144LedPins();
	setupVbatt();

	setHellenSdCardSpi2();

	engineConfiguration->clt.config.bias_resistor = 4700;
	engineConfiguration->iat.config.bias_resistor = 4700;

	// trigger inputs
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
	// Direct hall-only cam input
	// exhaust input same on both revisions
	engineConfiguration->camInputs[1] = H144_IN_D_AUX4;

//    //ETB1
//    // PWM pin
//    engineConfiguration->etbIo[0].controlPin = H144_OUT_PWM2;
//    // DIR pin
//	engineConfiguration->etbIo[0].directionPin1 = H144_OUT_PWM3;
//   	// Disable pin
//   	engineConfiguration->etbIo[0].disablePin = H144_OUT_IO12;
//   	// Unused
// 	engineConfiguration->etbIo[0].directionPin2 = Gpio::Unassigned;

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

	engineConfiguration->displayLogicLevelsInEngineSniffer = true;
	engineConfiguration->isSdCardEnabled = true;

	engineConfiguration->enableSoftwareKnock = true;

	engineConfiguration->canTxPin = H176_CAN_TX;
	engineConfiguration->canRxPin = H176_CAN_RX;

	engineConfiguration->fuelPumpPin = H144_OUT_IO9;
//	engineConfiguration->idle.solenoidPin = Gpio::D14;	// OUT_PWM5
//	engineConfiguration->fanPin = Gpio::D12;	// OUT_PWM8
	engineConfiguration->mainRelayPin = Gpio::G14;	// pin: 111a, OUT_IO3
	engineConfiguration->malfunctionIndicatorPin = H144_OUT_PWM8;

	engineConfiguration->brakePedalPin = H144_IN_RES3;
	engineConfiguration->clutchUpPin = H144_IN_RES2;
	engineConfiguration->acSwitch = H144_IN_RES1;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	engineConfiguration->etbFunctions[1] = ETB_Wastegate;

	// Some sensible defaults for other options
	setCrankOperationMode();

	setAlgorithm(LM_SPEED_DENSITY);

	setEtbPID(8.8944, 70.2307, 0.1855);

	engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure;

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;
	engineConfiguration->specs.displacement = 1.998;
	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_HONDA);
	strcpy(engineConfiguration->engineCode, "K");
	engineConfiguration->globalTriggerAngleOffset = 90;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;//IM_BATCH;// IM_SEQUENTIAL;

    setTPS1Calibration(98, 926, 1000, 0);
}
