/**
 * @file boards/hellen/hellen-nb1/board_configuration.cpp
 *
 *
 * @brief Configuration defaults for the hellen-nb1 board
 *
 * See http://rusefi.com/s/hellenNB1
 *
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "custom_engine.h"
#include "hellen_meta.h"

static OutputPin alphaEn;
static OutputPin alphaTachPullUp;
static OutputPin alphaTempPullUp;
static OutputPin alphaCrankPPullUp;
static OutputPin alphaCrankNPullUp;

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = H144_LS_1;
	engineConfiguration->injectionPins[1] = H144_LS_2;
	engineConfiguration->injectionPins[2] = Gpio::Unassigned;
	engineConfiguration->injectionPins[3] = Gpio::Unassigned;

	// Disable remainder
	for (int i = 4; i < MAX_CYLINDER_COUNT;i++) {
		engineConfiguration->injectionPins[i] = Gpio::Unassigned;
	}

	engineConfiguration->injectionPinMode = OM_DEFAULT;

	engineConfiguration->clutchDownPin = Gpio::Unassigned;
	engineConfiguration->clutchDownPinMode = PI_PULLDOWN;
	engineConfiguration->launchActivationMode = CLUTCH_INPUT_LAUNCH;
	engineConfiguration->malfunctionIndicatorPin = Gpio::Unassigned;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = H144_IGN_1;
	engineConfiguration->ignitionPins[1] = H144_IGN_2;
	engineConfiguration->ignitionPins[2] = Gpio::Unassigned;
	engineConfiguration->ignitionPins[3] = Gpio::Unassigned;

	// disable remainder
	for (int i = 4; i < MAX_CYLINDER_COUNT; i++) {
		engineConfiguration->ignitionPins[i] = Gpio::Unassigned;
	}

	engineConfiguration->ignitionPinMode = OM_DEFAULT;
}

static void setupEtb() {
	// TLE9201 driver
	// This chip has three control pins:
	// DIR - sets direction of the motor
	// PWM - pwm control (enable high, coast low)
	// DIS - disables motor (enable low)

	// PWM pin
	engineConfiguration->etbIo[0].controlPin = H144_OUT_PWM2;
	// DIR pin
	engineConfiguration->etbIo[0].directionPin1 = H144_OUT_PWM1;
	// Disable pin
	engineConfiguration->etbIo[0].disablePin = H144_OUT_IO1;
	// Unused
	engineConfiguration->etbIo[0].directionPin2 = Gpio::Unassigned;

	// we only have pwm/dir, no dira/dirb
	engineConfiguration->etb_use_two_wires = false;
}

static void setupVbatt() {
	// 4.7k high side/4.7k low side = 2.0 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 2.0f;

	// set vbatt_divider 5.835
	// 33k / 6.8k
	engineConfiguration->vbattDividerCoeff = (33 + 6.8) / 6.8; // 5.835

	engineConfiguration->vbattAdcChannel = H144_IN_VBATT;

	engineConfiguration->adcVcc = 3.29f;
}

static void setupDefaultSensorInputs() {
	// trigger inputs, hall
	engineConfiguration->triggerInputPins[0] = H144_IN_SENS4;
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
	engineConfiguration->camInputs[0] = Gpio::Unassigned;

	engineConfiguration->vehicleSpeedSensorInputPin = H144_IN_VSS;

	engineConfiguration->tps1_1AdcChannel = H144_IN_TPS;
	engineConfiguration->tps1_2AdcChannel = H144_IN_O2S2;
	engineConfiguration->tps2_1AdcChannel = EFI_ADC_NONE;

	engineConfiguration->throttlePedalPositionAdcChannel = H144_IN_PPS;
	engineConfiguration->throttlePedalPositionSecondAdcChannel = H144_IN_AUX1;

	engineConfiguration->mafAdcChannel = H144_IN_MAP1;
	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP2;
	engineConfiguration->baroSensor.type = MT_MPXH6400;
	engineConfiguration->baroSensor.hwChannel = H144_IN_MAP3;

	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;

	engineConfiguration->iat.adcChannel = H144_IN_IAT;

	engineConfiguration->auxTempSensor1.adcChannel = EFI_ADC_NONE;
	engineConfiguration->auxTempSensor2.adcChannel = EFI_ADC_NONE;
}

void boardInitHardware() {
	alphaEn.initPin("a-EN", H144_OUT_IO3);
	alphaEn.setValue(1);

//	alphaTachPullUp.initPin("a-tach", H144_OUT_IO1);
//	alphaTempPullUp.initPin("a-temp", H144_OUT_IO4);
//	alphaCrankPPullUp.initPin("a-crank-p", H144_OUT_IO2);
//	alphaCrankNPullUp.initPin("a-crank-n", H144_OUT_IO5);
	boardOnConfigurationChange(nullptr);
}

void boardOnConfigurationChange(engine_configuration_s * /*previousConfiguration*/) {
	alphaTachPullUp.setValue(engineConfiguration->boardUseTachPullUp);
	alphaTempPullUp.setValue(engineConfiguration->boardUseTempPullUp);
	alphaCrankPPullUp.setValue(engineConfiguration->boardUseCrankPullUp);
	alphaCrankNPullUp.setValue(engineConfiguration->boardUseCrankPullUp);
}

void setBoardConfigOverrides() {
	setHellen144LedPins();
	setupVbatt();

	setHellenSdCardSpi2();

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
	setupEtb();
	engineConfiguration->acSwitch = Gpio::Unassigned;
//	engineConfiguration->fuelPumpPin = Gpio::Unassigned;//H144_OUT_PWM2;
//	engineConfiguration->fanPin = Gpio::Unassigned;//H144_OUT_PWM4;
	engineConfiguration->mainRelayPin = Gpio::Unassigned;
    engineConfiguration->tachOutputPin = H144_OUT_PWM3;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	engineConfiguration->specs.cylindersCount = 2;
	engineConfiguration->specs.firingOrder = FO_1_2;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;//IM_BATCH;// IM_SEQUENTIAL;

	// random values to have valid config
	engineConfiguration->tps1SecondaryMin = 1000;
	engineConfiguration->tps1SecondaryMax = 0;
	// random values to have valid config
	engineConfiguration->throttlePedalSecondaryUpVoltage = 5.0;
	engineConfiguration->throttlePedalSecondaryWOTVoltage = 0.0;


	engineConfiguration->clutchDownPin = H144_IN_D_2;
	engineConfiguration->clutchDownPinMode = PI_PULLDOWN;
	engineConfiguration->launchActivationMode = CLUTCH_INPUT_LAUNCH;
// ?	engineConfiguration->malfunctionIndicatorPin = Gpio::G4; //1E - Check Engine Light
	engineConfiguration->vrThreshold[0].pin = H144_OUT_PWM4;
}

void boardPrepareForStop() {
	// Wake on the CAN RX pin
	palEnableLineEvent(PAL_LINE(GPIOD, 0), PAL_EVENT_MODE_RISING_EDGE);
}
