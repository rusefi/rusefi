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
#include "honda_k_dbc.h"

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
    engineConfiguration->vehicleSpeedSensorInputPin = H144_IN_VSS;

	engineConfiguration->tps1_1AdcChannel = H144_IN_TPS;

	engineConfiguration->mafAdcChannel = EFI_ADC_NONE;
	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP1;

	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;

	engineConfiguration->iat.adcChannel = H144_IN_IAT;
}

void setBoardConfigOverrides() {
	setHellen144LedPins();
	setHellenMegaEnPin();
	setupVbatt();

	setHellenSdCardSpi1();

	engineConfiguration->clt.config.bias_resistor = 4700;
	engineConfiguration->iat.config.bias_resistor = 4700;

	engineConfiguration->triggerInputPins[0] = H144_IN_RES1;
	engineConfiguration->camInputs[0] = H144_IN_RES3;
	// ex or in?
	engineConfiguration->camInputs[1] = H144_IN_RES2;


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
	setHondaK();

	engineConfiguration->isSdCardEnabled = true;

	engineConfiguration->globalTriggerAngleOffset = 663;

	engineConfiguration->enableSoftwareKnock = true;

	engineConfiguration->canTxPin = H176_CAN_TX;
	engineConfiguration->canRxPin = H176_CAN_RX;

	engineConfiguration->fuelPumpPin = H144_OUT_IO13;
	engineConfiguration->idle.solenoidPin = H144_LS_6;
	engineConfiguration->fanPin = H144_OUT_IO12;
	engineConfiguration->mainRelayPin = H144_OUT_IO3;
	engineConfiguration->malfunctionIndicatorPin = H144_OUT_IO7;

	engineConfiguration->brakePedalPin = H144_IN_CAM;
	engineConfiguration->acRelayPin = H144_LS_5;
//    engineConfiguration->tachOutputPin = ;
//	engineConfiguration->acSwitch = H144_IN_RES1;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	engineConfiguration->etbFunctions[0] = DC_None;
	engineConfiguration->etbFunctions[1] = DC_Wastegate;

	// Some sensible defaults for other options
	setCrankOperationMode();

	setAlgorithm(LM_SPEED_DENSITY);

	engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure;

	setCommonNTCSensor(&engineConfiguration->clt, HELLEN_DEFAULT_AT_PULLUP);
	setCommonNTCSensor(&engineConfiguration->iat, HELLEN_DEFAULT_AT_PULLUP);

    setTPS1Calibration(90, 830, 1000, 0);
}
