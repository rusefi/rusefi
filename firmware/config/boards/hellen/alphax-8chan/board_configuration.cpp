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

//	// PWM pin
//	engineConfiguration->etbIo[0].controlPin = Gpio::H144_OUT_PWM2;
//	// DIR pin
//	engineConfiguration->etbIo[0].directionPin1 = H144_GP1;
//	// Disable pin
//	engineConfiguration->etbIo[0].disablePin = H144_GP5;
//	// Unused
//	engineConfiguration->etbIo[0].directionPin2 = Gpio::Unassigned;
//
//	// PWM pin
//	engineConfiguration->etbIo[1].controlPin = H144_GP4;
//	// DIR pin
//	engineConfiguration->etbIo[1].directionPin1 = Gpio::H144_GP3;
//	// Disable pin
//	engineConfiguration->etbIo[1].disablePin = Gpio::Unassigned;
//	// Unused
//	engineConfiguration->etbIo[1].directionPin2 = Gpio::Unassigned;
	// we only have pwm/dir, no dira/dirb
	engineConfiguration->etb_use_two_wires = false;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::MM176_IGN1;
	engineConfiguration->ignitionPins[1] = Gpio::MM176_IGN2;
	engineConfiguration->ignitionPins[2] = Gpio::MM176_IGN3;
	engineConfiguration->ignitionPins[3] = Gpio::MM176_IGN4;
}

static void setupVbatt() {
	// 4.7k high side/4.7k low side = 2.0 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 2.0f;

	// set vbatt_divider 5.835
	// 33k / 6.8k
	engineConfiguration->vbattDividerCoeff = (33 + 6.8) / 6.8; // 5.835

	engineConfiguration->vbattAdcChannel = MM176_IN_VBATT;

	engineConfiguration->adcVcc = 3.29f;
}

static void setupDefaultSensorInputs() {
	// trigger inputs, hall
//	engineConfiguration->triggerInputPins[0] = H144_USB1ID;
//	engineConfiguration->triggerInputPins[1] = Gpio::H144_IN_CAM;
	engineConfiguration->camInputs[0] = Gpio::Unassigned;

	setTPS1Inputs(MM176_IN_TPS_ANALOG, MM176_IN_TPS2_ANALOG);

	setPPSInputs(MM176_IN_PPS1_ANALOG, MM176_IN_PPS2_ANALOG);

	// random values to have valid config
	engineConfiguration->tps1SecondaryMin = 1000;
	engineConfiguration->tps1SecondaryMax = 0;

	engineConfiguration->mafAdcChannel = EFI_ADC_NONE;
//	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP2;
	engineConfiguration->baroSensor.type = MT_MPXH6400;
//	engineConfiguration->baroSensor.hwChannel = H144_IN_MAP3;

	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;

	engineConfiguration->clt.adcChannel = MM176_IN_CLT_ANALOG;

	engineConfiguration->iat.adcChannel = MM176_IN_IAT_ANALOG;
}

void boardInitHardware() {
	setHellenEnPin(Gpio::MM176_EN_PIN);

//	alphaTempPullUp.initPin("a-temp", Gpio::H144_OUT_IO4);
//	alphaCrankPPullUp.initPin("a-crank-p", Gpio::H144_OUT_IO5);
//	alphaTachPullUp.initPin("a-tach", Gpio::H144_OUT_IO6);
//	alpha2stepPullDown.initPin("a-2step", Gpio::H144_OUT_IO7);
//	alphaCamPullDown.initPin("a-cam", Gpio::H144_OUT_IO8);
//	//alphaCamVrPullUp.initPin("a-cam-vr", Gpio::H144_OUT_IO9);
//	alphaD2PullDown.initPin("a-d2", H_SPI3_SCK);
//	alphaD3PullDown.initPin("a-d3", H_SPI3_MISO);
//	alphaD4PullDown.initPin("a-d4", H_SPI3_MOSI);
//	//alphaD5PullDown.initPin("a-d5", Gpio::H144_LS_8);
	boardOnConfigurationChange(nullptr);
}

void boardOnConfigurationChange(engine_configuration_s * /*previousConfiguration*/) {
//	alphaTachPullUp.setValue(engineConfiguration->boardUseTachPullUp);
//	alphaTempPullUp.setValue(engineConfiguration->boardUseTempPullUp);
//	alphaCrankPPullUp.setValue(engineConfiguration->boardUseCrankPullUp);
//	alpha2stepPullDown.setValue(engineConfiguration->boardUse2stepPullDown);
//	alphaCamPullDown.setValue(engineConfiguration->boardUseCamPullDown);
//	//alphaCamVrPullUp.setValue(engineConfiguration->boardUseCamVrPullUp);
//
//	alphaD2PullDown.setValue(engineConfiguration->boardUseD2PullDown);
//	alphaD3PullDown.setValue(engineConfiguration->boardUseD3PullDown);
//	alphaD4PullDown.setValue(engineConfiguration->boardUseD4PullDown);
	//alphaD5PullDown.setValue(engineConfiguration->boardUseD5PullDown);
}

#include "hellen_leds_144.cpp"

void setBoardConfigOverrides() {
	setupVbatt();

	setHellenSdCardSpi1();

    setDefaultHellenAtPullUps();

	setHellenCan();
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
	setupEtb();
//	engineConfiguration->vvtPins[0] = Gpio::H144_OUT_PWM7;
//	engineConfiguration->vvtPins[1] = Gpio::H144_OUT_PWM8;

    engineConfiguration->boardUseTempPullUp = true;

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
// ?	engineConfiguration->malfunctionIndicatorPin = Gpio::G4; //1E - Check Engine Light
	engineConfiguration->vrThreshold[0].pin = Gpio::H144_OUT_PWM6;
	engineConfiguration->vrThreshold[1].pin = Gpio::H144_OUT_PWM4;

	hellenWbo();
}

void boardPrepareForStop() {
	// Wake on the CAN RX pin
	palEnableLineEvent(PAL_LINE(GPIOD, 0), PAL_EVENT_MODE_RISING_EDGE);
}

//static Gpio OUTPUTS[] = {
//		Gpio::MM176_INJ1,
//		Gpio::H144_LS_2,
//		Gpio::H144_LS_3,
//		Gpio::H144_LS_4,
//};
//
//int getBoardMetaOutputsCount() {
//    return efi::size(OUTPUTS);
//}
//
//Gpio* getBoardMetaOutputs() {
//    return OUTPUTS;
//}
//
//int getBoardMetaDcOutputsCount() {
//    return 1;
//}
