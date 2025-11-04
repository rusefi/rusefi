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
#include "board_overrides.h"

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
	engineConfiguration->injectionPins[0] = Gpio::H144_LS_1;
	engineConfiguration->injectionPins[1] = Gpio::H144_LS_2;
	engineConfiguration->injectionPins[2] = Gpio::H144_LS_3;
	engineConfiguration->injectionPins[3] = Gpio::H144_LS_4;

	engineConfiguration->clutchDownPin = Gpio::Unassigned;
	engineConfiguration->clutchDownPinMode = PI_PULLDOWN;
	engineConfiguration->launchActivationMode = CLUTCH_INPUT_LAUNCH;
	engineConfiguration->malfunctionIndicatorPin = Gpio::Unassigned;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::H144_IGN_1;
	engineConfiguration->ignitionPins[1] = Gpio::H144_IGN_2;
	engineConfiguration->ignitionPins[2] = Gpio::H144_IGN_3;
	engineConfiguration->ignitionPins[3] = Gpio::H144_IGN_4;
}

static void setupDefaultSensorInputs() {
	// trigger inputs, hall
	engineConfiguration->triggerInputPins[0] = H144_USB1ID;
	engineConfiguration->triggerInputPins[1] = Gpio::H144_IN_CAM;
	engineConfiguration->camInputs[0] = Gpio::Unassigned;

	setTPS1Inputs(H144_IN_TPS, H144_IN_AUX1);

	setPPSInputs(H144_IN_PPS, H144_IN_AUX2);

	// random values to have valid config
	engineConfiguration->tps1SecondaryMin = 1000;
	engineConfiguration->tps1SecondaryMax = 0;


	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP2;
	engineConfiguration->baroSensor.type = MT_MPXH6400;
//	engineConfiguration->baroSensor.hwChannel = H144_IN_MAP3;

	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;

	engineConfiguration->iat.adcChannel = H144_IN_IAT;
}

static void alphax_8chan_reva_boardInitHardware() {
	setHellenEnPin(Gpio::H144_OUT_IO3);

	alphaTempPullUp.initPin("a-temp", Gpio::H144_OUT_IO4);
	alphaCrankPPullUp.initPin("a-crank-p", Gpio::H144_OUT_IO5);
	alphaTachPullUp.initPin("a-tach", Gpio::H144_OUT_IO6);
	alpha2stepPullDown.initPin("a-2step", Gpio::H144_OUT_IO7);
	alphaCamPullDown.initPin("a-cam", Gpio::H144_OUT_IO8);
	//alphaCamVrPullUp.initPin("a-cam-vr", Gpio::H144_OUT_IO9);
	alphaD2PullDown.initPin("a-d2", H_SPI3_SCK);
	alphaD3PullDown.initPin("a-d3", H_SPI3_MISO);
	alphaD4PullDown.initPin("a-d4", H_SPI3_MOSI);
	//alphaD5PullDown.initPin("a-d5", Gpio::H144_LS_8);
}

static void customBoardOnConfigurationChange(engine_configuration_s * /*previousConfiguration*/) {
	alphaTachPullUp.setValue(config->boardUseTachPullUp);
	alphaTempPullUp.setValue(config->boardUseTempPullUp);
	alphaCrankPPullUp.setValue(config->boardUseCrankPullUp);
	alpha2stepPullDown.setValue(config->boardUse2stepPullDown);
	alphaCamPullDown.setValue(config->boardUseCamPullDown);

	alphaD2PullDown.setValue(config->boardUseD2PullDown);
	alphaD3PullDown.setValue(config->boardUseD3PullDown);
	alphaD4PullDown.setValue(config->boardUseD4PullDown);
}

static void alphax_8chan_reva_boardConfigOverrides() {
	setHellenVbatt();

	setHellenSdCardSpi2();

    setDefaultHellenAtPullUps();

	setHellenCan();
	setHellenCan2();
}

void set8chanDefaultETBPins() {
	setupTLE9201IncludingStepper(/*controlPin*/Gpio::H144_OUT_PWM2, Gpio::H144_GP_IO1, Gpio::H144_GP_IO5);
	setupTLE9201IncludingStepper(/*controlPin*/Gpio::H144_GP_IO4, Gpio::H144_GP_IO3, Gpio::Unassigned, 1);
}

static void alphax_8chan_reva_boardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();

  set8chanDefaultETBPins();
	engineConfiguration->vvtPins[0] = Gpio::H144_OUT_PWM7;
	engineConfiguration->vvtPins[1] = Gpio::H144_OUT_PWM8;

    engineConfiguration->boardUseTempPullUp = true;

	engineConfiguration->mainRelayPin = Gpio::H144_OUT_IO10;
	engineConfiguration->fanPin = Gpio::H144_OUT_IO11;
	engineConfiguration->fuelPumpPin = Gpio::H144_OUT_IO12;
    engineConfiguration->tachOutputPin = Gpio::H144_OUT_IO13;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK


	engineConfiguration->launchActivationMode = CLUTCH_INPUT_LAUNCH;
// ?	engineConfiguration->malfunctionIndicatorPin = Gpio::G4; //1E - Check Engine Light
	engineConfiguration->vrThreshold[0].pin = Gpio::H144_OUT_PWM6;
	engineConfiguration->vrThreshold[1].pin = Gpio::H144_OUT_PWM4;

	hellenWbo();
}

void boardPrepareForStop() {
	// Wake on the CAN RX pin
	palEnableLineEvent(PAL_LINE(GPIOD, 0), PAL_EVENT_MODE_RISING_EDGE);
}

static Gpio OUTPUTS[] = {
		Gpio::H144_LS_1,
		Gpio::H144_LS_2,
		Gpio::H144_LS_3,
		Gpio::H144_LS_4,
};

int getBoardMetaOutputsCount() {
    return efi::size(OUTPUTS);
}

Gpio* getBoardMetaOutputs() {
    return OUTPUTS;
}

void setup_custom_board_overrides() {
	custom_board_InitHardware = alphax_8chan_reva_boardInitHardware;
	custom_board_DefaultConfiguration = alphax_8chan_reva_boardDefaultConfiguration;
	custom_board_ConfigOverrides = alphax_8chan_reva_boardConfigOverrides;

	custom_board_OnConfigurationChange = customBoardOnConfigurationChange;
}
