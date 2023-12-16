/**
 * @file boards/hellen/uaefi/board_configuration.cpp
 *
 * See https://rusefi.com/s/uaefi
 *
 * @author Andrey Belomutskiy, (c) 2012-2023
 */

#include "pch.h"
#include "defaults.h"
#include "hellen_meta.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = Gpio::MM100_INJ1;
	engineConfiguration->injectionPins[1] = Gpio::MM100_INJ2;
	engineConfiguration->injectionPins[2] = Gpio::MM100_INJ3;
	engineConfiguration->injectionPins[3] = Gpio::MM100_INJ4;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::MM100_IGN1;
	engineConfiguration->ignitionPins[1] = Gpio::MM100_IGN2;
	engineConfiguration->ignitionPins[2] = Gpio::MM100_IGN3;
	engineConfiguration->ignitionPins[3] = Gpio::MM100_IGN4;
}

static void setupDefaultSensorInputs() {
    engineConfiguration->vehicleSpeedSensorInputPin = Gpio::MM100_IN_D2;

	engineConfiguration->tps1_1AdcChannel = MM100_IN_TPS_ANALOG;
	engineConfiguration->tps1_2AdcChannel = MM100_IN_AUX1_ANALOG;

	setPPSInputs(MM100_IN_PPS_ANALOG, MM100_IN_AUX2_ANALOG);

	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP1;

	engineConfiguration->clt.adcChannel = MM100_IN_CLT_ANALOG;

	engineConfiguration->iat.adcChannel = MM100_IN_IAT_ANALOG;
}

#include "hellen_leds_100.cpp"

void setBoardConfigOverrides() {
  chThdSleepMilliseconds(5); // experiment for https://github.com/andreika-git/hellen-one/issues/353
	setHellenMegaEnPin();
	setHellenVbatt();

	setHellenSdCardSpi1();
	configureHellenMegaAccCS2Pin();
	configureHellenCanTerminator();
	setHellenCan();

  engineConfiguration->mainRelayPin = Gpio::MM100_IGN7;
 	engineConfiguration->fanPin = Gpio::MM100_IGN8;

	engineConfiguration->clt.adcChannel = MM100_IN_CLT_ANALOG;
	engineConfiguration->iat.adcChannel = MM100_IN_IAT_ANALOG;
	engineConfiguration->tps1_1AdcChannel = MM100_IN_TPS_ANALOG;
	engineConfiguration->map.sensor.hwChannel = MM100_IN_MAP1_ANALOG;

	engineConfiguration->clt.config.bias_resistor = 4700;
	engineConfiguration->iat.config.bias_resistor = 4700;

	engineConfiguration->triggerInputPins[0] = Gpio::MM100_IN_CRANK;
	engineConfiguration->camInputs[0] = Gpio::MM100_IN_D1;

    // PWM pin
    engineConfiguration->etbIo[0].controlPin = Gpio::MM100_OUT_PWM3;
    // DIR pin
	engineConfiguration->etbIo[0].directionPin1 = Gpio::MM100_OUT_PWM4;
   	// Disable pin
   	engineConfiguration->etbIo[0].disablePin = Gpio::MM100_SPI2_MISO;
    // PWM pin
    engineConfiguration->etbIo[1].controlPin = Gpio::MM100_OUT_PWM5;
    // DIR pin
	engineConfiguration->etbIo[1].directionPin1 = Gpio::MM100_SPI2_MOSI;
   	// Disable pin
   	engineConfiguration->etbIo[1].disablePin = Gpio::MM100_USB1ID;
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

    // not override since sometimes we have issues?
//	setHellenMMbaro();

	engineConfiguration->displayLogicLevelsInEngineSniffer = true;

	engineConfiguration->globalTriggerAngleOffset = 0;

	engineConfiguration->enableSoftwareKnock = true;

	engineConfiguration->canTxPin = Gpio::MM100_CAN_TX;
	engineConfiguration->canRxPin = Gpio::MM100_CAN_RX;

//	engineConfiguration->fuelPumpPin = Gpio::MM100_OUT_PWM5;
//	engineConfiguration->idle.solenoidPin = Gpio::H144_LS_6;
//	engineConfiguration->fanPin = Gpio::H144_OUT_IO12;
//	engineConfiguration->mainRelayPin = Gpio::MM100_IGN8;
//	engineConfiguration->malfunctionIndicatorPin = Gpio::H144_OUT_IO7;

//	engineConfiguration->brakePedalPin = Gpio::H144_IN_CAM;
//	engineConfiguration->acRelayPin = Gpio::H144_LS_5;
//    engineConfiguration->tachOutputPin = Gpio::H144_OUT_IO10;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();
	engineConfiguration->enableVerboseCanTx = true;

	engineConfiguration->etbFunctions[0] = DC_Throttle1;

	// Some sensible defaults for other options
	setCrankOperationMode();

	setAlgorithm(LM_SPEED_DENSITY);

	engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure;

	setCommonNTCSensor(&engineConfiguration->clt, HELLEN_DEFAULT_AT_PULLUP);
	setCommonNTCSensor(&engineConfiguration->iat, HELLEN_DEFAULT_AT_PULLUP);

    setTPS1Calibration(100, 650);
	//hellenWbo();
}

static Gpio OUTPUTS[] = {
	Gpio::MM100_INJ6, // B1 injector output 6
	Gpio::MM100_INJ5, // B2 injector output 5
	Gpio::MM100_INJ4, // B3 injector output 4
	Gpio::MM100_INJ3, // B4 injector output 3
	Gpio::MM100_INJ2, // B5 injector output 2
	Gpio::MM100_INJ1, // B6 injector output 1
	Gpio::MM100_INJ7, // B7 Low Side output 1
	Gpio::MM100_IGN8, // B8 Weak Low Side output 2
	Gpio::MM100_IGN7, // B9 Weak Low Side output 1
	Gpio::MM100_OUT_PWM2, // B16 Low Side output 4
	Gpio::MM100_OUT_PWM1, // B17 Low Side output 3
	Gpio::MM100_INJ8, // B18 Low Side output 2
	Gpio::MM100_IGN6, // B10 Coil 6
	Gpio::MM100_IGN4, // B11 Coil 4
};

int getBoardMetaOutputsCount() {
    return efi::size(OUTPUTS);
}

Gpio* getBoardMetaOutputs() {
    return OUTPUTS;
}

int getBoardMetaDcOutputsCount() {
    return 2;
}
