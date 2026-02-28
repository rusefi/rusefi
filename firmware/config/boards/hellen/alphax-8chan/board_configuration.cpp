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

static OutputPin alphaCrankPPullUp;

static OutputPin alphaHall1PullDown;
static OutputPin alphaHall2PullDown;
static OutputPin alphaHall3PullDown;
static OutputPin alphaHall4PullDown;
static OutputPin alphaHall5PullDown;
static OutputPin alphaFlexPullDown;
static OutputPin tempPullUp;

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

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::MM176_IGN1;
	engineConfiguration->ignitionPins[1] = Gpio::MM176_IGN2;
	engineConfiguration->ignitionPins[2] = Gpio::MM176_IGN3;
	engineConfiguration->ignitionPins[3] = Gpio::MM176_IGN4;
}

static void setupDefaultSensorInputs() {
	// trigger inputs, VR/hall crank
	engineConfiguration->triggerInputPins[0] = Gpio::MM176_USB1ID; // 1B
//	engineConfiguration->triggerInputPins[1] = Gpio::H144_IN_CAM;
	engineConfiguration->camInputs[0] = Gpio::MM176_IN_D1; // 15A
	engineConfiguration->camInputs[1] = Gpio::MM176_IN_D2;
	engineConfiguration->camInputs[2] = Gpio::MM176_IN_D3; // 24A
	engineConfiguration->camInputs[3] = Gpio::MM176_IN_D4; // 9A
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

static void alphax_8chan_boardInitHardware() {
  // technically same thing as setHellenMegaEnPin() since underlying pin E10 is same as H144_GP8
	setHellenEnPin(Gpio::MM176_EN_PIN);

	alphaCrankPPullUp.initPin("Crank-PullUp", Gpio::MM176_GP16);
	alphaHall1PullDown.initPin("PullDown-Hall1", Gpio::MM176_OUT_IO10);
	alphaHall2PullDown.initPin("PullDown-Hall2", Gpio::MM176_GP21);
	alphaHall3PullDown.initPin("PullDown-Hall3", Gpio::MM176_GP22);
	alphaHall4PullDown.initPin("PullDown-Hall4", Gpio::MM176_GP23);
	alphaHall5PullDown.initPin("PullDown-Hall5", Gpio::MM176_GP24);
	alphaFlexPullDown.initPin("PullDown-Flex",   Gpio::MM176_GP25);
	tempPullUp.initPin("Temp PullUp", Gpio::MM176_OUT_IO12);
}

static void customBoardOnConfigurationChange(engine_configuration_s * /*previousConfiguration*/) {
	alphaCrankPPullUp.setValue(config->boardUseCrankPullUp);
	alphaHall1PullDown.setValue(config->boardUseH1PullDown);
	alphaHall2PullDown.setValue(config->boardUseH2PullDown);
	alphaHall3PullDown.setValue(config->boardUseH3PullDown);
	alphaHall4PullDown.setValue(config->boardUseH4PullDown);
	alphaHall5PullDown.setValue(config->boardUseH5PullDown);
	alphaFlexPullDown.setValue(config->boardUseFlexPullDown);
	tempPullUp.setValue(config->boardUseTempPullUp);
}

static void alphax_8chan_boardConfigOverrides() {
	hellenMegaModule();
	setHellenCan();
	setHellenCan2();
}

void set8chanDefaultETBPins() {
	setupTLE9201IncludingStepper(/*controlPin*/Gpio::MM176_OUT_PWM9, Gpio::MM176_GP6, Gpio::MM176_GP7);
	setupTLE9201IncludingStepper(/*controlPin*/Gpio::MM176_OUT_PWM18, Gpio::MM176_GP10, Gpio::MM176_GP11, 1);
}

static void alphax_8chan_defaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();
	set8chanDefaultETBPins();
//	engineConfiguration->vvtPins[0] = Gpio::H144_OUT_PWM7;
//	engineConfiguration->vvtPins[1] = Gpio::H144_OUT_PWM8;

    config->boardUseTempPullUp = true;

    setHellenMMbaro();

	engineConfiguration->mainRelayPin = Gpio::MM176_GP1;
//	engineConfiguration->fanPin = Gpio::H144_OUT_IO11;
//	engineConfiguration->fuelPumpPin = Gpio::H144_OUT_IO12;
//    engineConfiguration->tachOutputPin = Gpio::H144_OUT_IO13;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();
  engineConfiguration->isSdCardEnabled = true;

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

static Gpio OUTPUTS_GM_GEN4[] = {
	Gpio::MM176_INJ1, // 1D - Injector 1
	Gpio::MM176_INJ2, // 2D - Injector 2
	Gpio::MM176_INJ3, // 3D - Injector 3
	Gpio::MM176_INJ4, // 4D - Injector 4

	Gpio::MM176_INJ5, // 5D - Injector 5
	Gpio::MM176_INJ6, // 6D - Injector 6
	Gpio::MM176_INJ7, // 7D - Injector 7
	Gpio::MM176_INJ8, // 13D - Injector 8

  Gpio::MM176_GP1, // 11D - Main Relay
	Gpio::MM176_GP2, // 10D - Fan
//	Gpio::MM176_OUT_PWM1, // 8D - VVT 1
};

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
    if (engineConfiguration->engineType == engine_type_e::GM_SBC_GEN4) {
        return efi::size(OUTPUTS_GM_GEN4);
    }
    return efi::size(OUTPUTS);
}

int getBoardMetaLowSideOutputsCount() {
    if (engineConfiguration->engineType == engine_type_e::GM_SBC_GEN4) {
      return getBoardMetaOutputsCount();
    }
    return getBoardMetaOutputsCount() - 6;
}

Gpio* getBoardMetaOutputs() {
    if (engineConfiguration->engineType == engine_type_e::GM_SBC_GEN4) {
      return OUTPUTS_GM_GEN4;
    }
    return OUTPUTS;
}

int getBoardMetaDcOutputsCount() {
    if (engineConfiguration->engineType == engine_type_e::GM_SBC_GEN4) {
      // STATIC_BOARD_ID_PLATINUM_GM_GEN4
        return 1;
    }
    return 2;
}

void setup_custom_board_overrides() {
	custom_board_InitHardware = alphax_8chan_boardInitHardware;
	custom_board_DefaultConfiguration = alphax_8chan_defaultConfiguration;
	custom_board_ConfigOverrides = alphax_8chan_boardConfigOverrides;

	custom_board_OnConfigurationChange = customBoardOnConfigurationChange;
}

int boardGetAnalogInputDiagnostic(adc_channel_e hwChannel, float voltage) {
	/* we do not check voltage for valid range yet */
	(void)voltage;

	switch (hwChannel) {
		/* inputs that may be affected by incorrect reference voltage */
		case MM176_IN_TPS_ANALOG:
		case MM176_IN_TPS2_ANALOG:
		case MM176_IN_PPS1_ANALOG:
		case MM176_IN_PPS2_ANALOG:
		case MM176_IN_IAT_ANALOG:
		case MM176_IN_AT1_ANALOG:
		case MM176_IN_CLT_ANALOG:
		case MM176_IN_AT2_ANALOG:
		//case MM176_IN_O2S_ANALOG:
		//case MM176_IN_O2S2_ANALOG:
		case MM176_IN_MAP1_ANALOG:
		case MM176_IN_MAP2_ANALOG:
		case MM176_IN_AUX1_ANALOG:
		case MM176_IN_AUX2_ANALOG:
		case MM176_IN_AUX3_ANALOG:
		case MM176_IN_AUX4_ANALOG:
			/* TODO: more? */
			return (boardGetAnalogDiagnostic() == ObdCode::None) ? 0 : -1;
		/* all other inputs should not rely on output 5V */
		default:
			return 0;
	}

	return 0;
}
