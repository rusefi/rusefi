/**
 * @file boards/hellen/hellen-112-17/board_configuration.cpp
 *
 * See https://rusefi.com/s/XX
 *
 * @author Andrey Belomutskiy, (c) 2012-2023
 */

#include "pch.h"
#include "defaults.h"
#include "hellen_meta.h"
#include "board_overrides.h"

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
	engineConfiguration->tps1_2AdcChannel = MM100_IN_O2S_ANALOG;

	setPPSInputs(MM100_IN_PPS_ANALOG, MM100_IN_AUX2_ANALOG);


	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP1;

	engineConfiguration->clt.adcChannel = MM100_IN_CLT_ANALOG;

	engineConfiguration->iat.adcChannel = MM100_IN_IAT_ANALOG;
}

#include "hellen_leds_100.cpp"

static void hellen112_17_boardConfigOverrides() {
	// rev A needs EN pin but newer revisions would potentially not use it
	setHellenMegaEnPin();
	setHellenVbatt();

	hellenMegaSdWithAccelerometer();
	configureHellenCanTerminator();
	setHellenCan();

	setDefaultHellenAtPullUps();

	engineConfiguration->triggerInputPins[0] = Gpio::MM100_IN_CRANK;
	engineConfiguration->camInputs[0] = Gpio::MM100_IN_D1;

    //ETB1
    // PWM pin
    engineConfiguration->etbIo[0].controlPin = Gpio::MM100_OUT_PWM6;
    // DIR pin
	engineConfiguration->etbIo[0].directionPin1 = Gpio::MM100_SPI2_MISO;
   	// Disable pin
   	engineConfiguration->etbIo[0].disablePin = Gpio::MM100_SPI2_MOSI;
}

/**
 * @brief   Board-specific configuration defaults.
 *

 *

 */
static void hellen112_17_boardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();

    // not override since sometimes we have issues?
//	setHellenMMbaro();

	engineConfiguration->displayLogicLevelsInEngineSniffer = true;

	engineConfiguration->enableSoftwareKnock = true;

	engineConfiguration->canTxPin = Gpio::MM100_CAN_TX;
	engineConfiguration->canRxPin = Gpio::MM100_CAN_RX;

	engineConfiguration->fuelPumpPin = Gpio::MM100_OUT_PWM5;
//	engineConfiguration->idle.solenoidPin = Gpio::H144_LS_6;
//	engineConfiguration->fanPin = Gpio::H144_OUT_IO12;
	engineConfiguration->mainRelayPin = Gpio::MM100_IGN8;
//	engineConfiguration->malfunctionIndicatorPin = Gpio::H144_OUT_IO7;

	// how come this is not denso 183?!
	setCustomMap(/*lowValue*/ 11.4, /*mapLowValueVoltage*/ 0.5, /*highValue*/ 170.7, 4.8);

//	engineConfiguration->brakePedalPin = Gpio::H144_IN_CAM;
//	engineConfiguration->acRelayPin = Gpio::H144_LS_5;
//    engineConfiguration->tachOutputPin = Gpio::H144_OUT_IO10;
    // A/C switch on k-line

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	engineConfiguration->etbFunctions[0] = DC_Throttle1;

	// Some sensible defaults for other options
	setCrankOperationMode();

	setAlgorithm(engine_load_mode_e::LM_SPEED_DENSITY);

	engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure;

	setCommonNTCSensorParameters(&engineConfiguration->clt);
	setCommonNTCSensorParameters(&engineConfiguration->iat);

    setTPS1Calibration(100, 650);
	//hellenWbo();
}

static Gpio OUTPUTS[] = {
	Gpio::MM100_INJ1, // 244 injector output 1
	Gpio::MM100_INJ2, // 242 injector output 2
	Gpio::MM100_INJ3, // 243 injector output 3
	Gpio::MM100_INJ4, // 245 injector output 4
	Gpio::MM100_IGN1, // 256 Coil 1
	Gpio::MM100_IGN2, // 253 Coil 2
	Gpio::MM100_IGN3, // 254 Coil 3
	Gpio::MM100_IGN4, // 255 Coil 4
	Gpio::MM100_INJ6, // 9 235 Low Side Output
	Gpio::MM100_INJ5, // 101 low side 1.5A output
	Gpio::MM100_INJ7, // 102 low side 1.5A output
	Gpio::MM100_IGN8, // 12 115 Main Relay
	Gpio::MM100_OUT_PWM3, // 131 Relay Output
	Gpio::MM100_IGN7, // 140 Relay Output
	Gpio::MM100_OUT_PWM4, // 141 Engine Radiator Fan
	Gpio::MM100_OUT_PWM5, // 142 Fuel Pump Relay
	Gpio::MM100_IGN5, // 151 relay output
	Gpio::MM100_IGN6, // 18 152 relay output
	Gpio::MM100_INJ8, // 246 low side 1.5A output
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
	custom_board_DefaultConfiguration = hellen112_17_boardDefaultConfiguration;
	custom_board_ConfigOverrides =  hellen112_17_boardConfigOverrides;
}
