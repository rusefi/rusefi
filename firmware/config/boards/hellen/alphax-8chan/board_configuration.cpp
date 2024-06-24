/**
 * @file boards/hellen/hellen-nb1/board_configuration.cpp
 *
 *
 * @brief Configuration defaults for the 8chan board
 *
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "hellen_meta.h"
#include "defaults.h"

// mm176 matches mm144 in terms of LED pinout
#include "hellen_leds_144.cpp"

static OutputPin alphaTachPullUp;
static OutputPin alphaTempPullUp;
static OutputPin alphaCrankPPullUp;
static OutputPin alpha2stepPullDown;
static OutputPin alphaCamPullDown;
//static OutputPin alphaCamVrPullUp;
static OutputPin alphaD2PullDown;
static OutputPin alphaD3PullDown;
static OutputPin alphaD4PullDown;
//static OutputPin alphaD5PullDown;

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = Gpio::MM176_INJ1;
	engineConfiguration->injectionPins[1] = Gpio::MM176_INJ2;
	engineConfiguration->injectionPins[2] = Gpio::MM176_INJ3;
	engineConfiguration->injectionPins[3] = Gpio::MM176_INJ4;

	engineConfiguration->clutchDownPin = Gpio::Unassigned;
	engineConfiguration->clutchDownPinMode = PI_PULLDOWN;
	engineConfiguration->launchActivationMode = CLUTCH_INPUT_LAUNCH;
	engineConfiguration->malfunctionIndicatorPin = Gpio::Unassigned;
}

static void setupEtb() {
	// TLE9201 driver
	// This chip has three control pins:
	// DIR - sets direction of the motor
	// PWM - pwm control (enable high, coast low)
	// DIS - disables motor (enable low)

	// PWM pin
	engineConfiguration->etbIo[0].controlPin = Gpio::MM176_OUT_PWM9;
	// DIR pin
	engineConfiguration->etbIo[0].directionPin1 = Gpio::MM176_GP6;
	// Disable pin
	engineConfiguration->etbIo[0].disablePin = Gpio::MM176_GP7;

	// PWM pin
	engineConfiguration->etbIo[1].controlPin = Gpio::MM176_OUT_PWM18;
	// DIR pin
	engineConfiguration->etbIo[1].directionPin1 = Gpio::MM176_GP10;
	// Disable pin
	engineConfiguration->etbIo[1].disablePin = Gpio::MM176_GP11;

	// we only have pwm/dir, no dira/dirb
	engineConfiguration->etb_use_two_wires = false;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::MM176_IGN1;
	engineConfiguration->ignitionPins[1] = Gpio::MM176_IGN2;
	engineConfiguration->ignitionPins[2] = Gpio::MM176_IGN3;
	engineConfiguration->ignitionPins[3] = Gpio::MM176_IGN4;
}

static void setupDefaultSensorInputs() {
	// trigger inputs, hall
	engineConfiguration->triggerInputPins[0] = Gpio::MM176_USB1ID;
//	engineConfiguration->triggerInputPins[1] = Gpio::H144_IN_CAM;
	engineConfiguration->camInputs[0] = Gpio::MM176_IN_D1;
	engineConfiguration->camInputs[1] = Gpio::MM176_IN_D2;
	engineConfiguration->camInputs[2] = Gpio::MM176_IN_D3;
	engineConfiguration->camInputs[3] = Gpio::MM176_IN_D4;
	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH;
	engineConfiguration->vvtMode[1] = VVT_SINGLE_TOOTH;

	setTPS1Inputs(MM176_IN_TPS_ANALOG, MM176_IN_TPS2_ANALOG);

	setPPSInputs(MM176_IN_PPS1_ANALOG, MM176_IN_PPS2_ANALOG);

	// random values to have valid config
	engineConfiguration->tps1SecondaryMin = 1000;
	engineConfiguration->tps1SecondaryMax = 0;


//	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP2;
	engineConfiguration->baroSensor.type = MT_MPXH6400;
//	engineConfiguration->baroSensor.hwChannel = H144_IN_MAP3;

	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;

	engineConfiguration->clt.adcChannel = MM176_IN_CLT_ANALOG;

	engineConfiguration->iat.adcChannel = MM176_IN_IAT_ANALOG;
}

void boardInitHardware() {
	setHellenEnPin(Gpio::MM176_EN_PIN);

	alphaCrankPPullUp.initPin("a-crank-p", Gpio::MM176_GP16);
	alphaD2PullDown.initPin("a-d2", Gpio::MM176_GP21);
	alphaD3PullDown.initPin("a-d3", Gpio::MM176_GP22);
	alphaD4PullDown.initPin("a-d4", Gpio::MM176_GP23);
	boardOnConfigurationChange(nullptr);
}

void boardOnConfigurationChange(engine_configuration_s * /*previousConfiguration*/) {
	alphaCrankPPullUp.setValue(engineConfiguration->boardUseCrankPullUp);
	alphaD2PullDown.setValue(engineConfiguration->boardUseD2PullDown);
	alphaD3PullDown.setValue(engineConfiguration->boardUseD3PullDown);
}

void setBoardConfigOverrides() {
	setHellenVbatt();

	setHellenSdCardSpi1();

    setDefaultHellenAtPullUps();

	setHellenCan();
}

/**
 * @brief   Board-specific configuration defaults.
 *
 * See also setDefaultEngineConfiguration
 *
 */
void setBoardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();
	setupEtb();
//	engineConfiguration->vvtPins[0] = Gpio::H144_OUT_PWM7;
//	engineConfiguration->vvtPins[1] = Gpio::H144_OUT_PWM8;

    config->boardUseTempPullUp = true;

    setHellenMMbaro();

//	engineConfiguration->mainRelayPin = Gpio::H144_OUT_IO10;
//	engineConfiguration->fanPin = Gpio::H144_OUT_IO11;
//	engineConfiguration->fuelPumpPin = Gpio::H144_OUT_IO12;
//    engineConfiguration->tachOutputPin = Gpio::H144_OUT_IO13;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK

	engineConfiguration->launchActivationMode = CLUTCH_INPUT_LAUNCH;

	engineConfiguration->vrThreshold[0].pin = Gpio::MM176_OUT_PWM11;
	engineConfiguration->vrThreshold[1].pin = Gpio::MM176_OUT_PWM12;
	//engineConfiguration->vrThreshold[2].pin = Gpio::MM176_OUT_PWM13;

	hellenWbo();
}

void boardPrepareForStop() {
	// Wake on the CAN RX pin
	palEnableLineEvent(PAL_LINE(GPIOD, 0), PAL_EVENT_MODE_RISING_EDGE);
}

static Gpio OUTPUTS[] = {
	Gpio::MM176_INJ1, // 1D - Injector 1
	Gpio::MM176_INJ2, // 2D - Injector 2
	Gpio::MM176_INJ3, // 3D - Injector 3
	Gpio::MM176_INJ4, // 4D - Injector 4
	Gpio::MM176_INJ5, // 5D - Injector 5

	Gpio::MM176_INJ6, // 6D - Injector 6
	Gpio::MM176_INJ7, // 7D - Injector 7
	Gpio::MM176_OUT_PWM1, // 8D - VVT 1
	Gpio::MM176_GP3, // 9D - Fuel Pump
	Gpio::MM176_GP2, // 10D - Fan

	Gpio::MM176_GP1, // 11D - Main Relay
	Gpio::MM176_OUT_IO8, // 12D - NOS
	Gpio::MM176_INJ8, // 13D - Injector 8
	Gpio::MM176_OUT_PWM2, // 14D - VVT 2
	Gpio::MM176_OUT_IO13, // 15D - Tachometer

	Gpio::MM176_OUT_PWM6, // 16C - Low Side 1
	Gpio::MM176_GP17, // 19D - Injector 9
	Gpio::MM176_OUT_PWM3, // 20D - VVT 3
	Gpio::MM176_OUT_PWM4, // 21D - VVT 4
	Gpio::MM176_OUT_PWM5, // 22C - Boost

	Gpio::MM176_OUT_PWM7, // 23C - Low Side 2
	Gpio::MM176_GP20, // 24D - Injector 12
	Gpio::MM176_GP19, // 25D - Injector 11
	Gpio::MM176_GP18, // 26D - Injector 10
	Gpio::MM176_OUT_IO3, // 8C - Low Side 3

	Gpio::MM176_OUT_IO4, // 9C - Low Side 4
	Gpio::MM176_OUT_IO5, // 17C - Low Side 5
	Gpio::MM176_OUT_IO6, // 25C - Low Side 6
	Gpio::MM176_OUT_IO7, // 34C - Low Side 7
	Gpio::MM176_OUT_PWM8, // 5C - High Side 1

	Gpio::MM176_OUT_IO2, // 6C - High Side 2
	Gpio::MM176_OUT_IO1, // 7C - High Side 3
	Gpio::MM176_IGN1, // 11C - Ignition 1
	Gpio::MM176_IGN3, // 13C - Ignition 3
	Gpio::MM176_IGN5, // 14C - Ignition 5

//	Gpio::MM176_IGN8, // 15C - Ignition 8
//
//	Gpio::MM176_IGN2, // 19C - Ignition 2
//	Gpio::MM176_IGN4, // 20C - Ignition 4
//	Gpio::MM176_IGN7, // 21C - Ignition 7
//	Gpio::MM176_SPI3_MISO, // 23C - Ignition 10
//
//	Gpio::MM176_IGN6, // 28C - Ignition 6
//	Gpio::MM176_SPI3_SCK, // 29C - Ignition 9
//	Gpio::MM176_SPI3_MOSI, // 30C - Ignition 11
//	Gpio::MM176_SPI3_CS, // 31C - Ignition 12


};

int getBoardMetaOutputsCount() {
    return efi::size(OUTPUTS);
}

int getBoardMetaLowSideOutputsCount() {
    return getBoardMetaOutputsCount() - 6;
}

Gpio* getBoardMetaOutputs() {
    return OUTPUTS;
}

int getBoardMetaDcOutputsCount() {
    return 2;
}
