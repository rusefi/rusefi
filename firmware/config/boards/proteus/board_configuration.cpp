/**
 * @file boards/proteus/board_configuration.cpp
 *
 * @brief Configuration defaults for the Proteus board
 *
 * @author Matthew Kennedy, (c) 2019
 */

#include "engine_configuration.h"
#include "engine_math.h"
#include "allsensors.h"
#include "fsio_impl.h"

EXTERN_ENGINE;

static const brain_pin_e injPins[] = {
	GPIOD_7,
	GPIOG_9,
	GPIOG_10,
	GPIOG_11,
	GPIOG_12,
	GPIOG_13,
	GPIOG_14,
	GPIOB_4,
	GPIOB_5,
	GPIOB_6,
	GPIOB_7,
	GPIOB_8
};

static const brain_pin_e ignPins[] = {
	GPIOD_4,
	GPIOD_3,
	GPIOC_9,
	GPIOC_8,
	GPIOC_7,
	GPIOG_8,
	GPIOG_7,
	GPIOG_6,
	GPIOG_5,
	GPIOG_4,
	GPIOG_3,
	GPIOG_2,
};

static void setInjectorPins() {
	copyArray(engineConfiguration->injectionPins, injPins);
	engineConfiguration->injectionPinMode = OM_DEFAULT;
}

static void setIgnitionPins() {
	copyArray(engineConfiguration->ignitionPins, ignPins);
	engineConfiguration->ignitionPinMode = OM_DEFAULT;
}

void setSdCardConfigurationOverrides(void) {
}

static void setLedPins() {
	CONFIG(warningLedPin) = GPIOE_3;
	CONFIG(communicationLedPin) = GPIOE_4;
	engineConfiguration->runningLedPin = GPIOE_5;
	engineConfiguration->triggerErrorPin = GPIOE_6;
}

static void setupVbatt() {
	// 5.6k high side/10k low side = 1.56 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 1.56f;

	// 82k high side/10k low side = 9.2
	engineConfiguration->vbattDividerCoeff = (92.0f / 10.0f);
	//engineConfiguration->vbattAdcChannel = TODO;

	engineConfiguration->adcVcc = 3.3f;
}

static void setupEtb() {
	// TLE9201 driver
	// This chip has three control pins:
	// DIR - sets direction of the motor
	// PWM - pwm control (enable high, coast low)
	// DIS - disables motor (enable low)

	// Throttle #1
	// PWM pin
	engineConfiguration->etbIo[0].controlPin1 = GPIOD_12;
	// DIR pin
	engineConfiguration->etbIo[0].directionPin1 = GPIOD_10;
	// Disable pin
	engineConfiguration->etbIo[0].disablePin = GPIOD_11;
	// Unused
	engineConfiguration->etbIo[0].directionPin2 = GPIO_UNASSIGNED;

	// Throttle #2
	// PWM pin
	engineConfiguration->etbIo[1].controlPin1 = GPIOD_13;
	// DIR pin
	engineConfiguration->etbIo[1].directionPin1 = GPIOD_9;
	// Disable pin
	engineConfiguration->etbIo[1].disablePin = GPIOD_8;
	// Unused
	engineConfiguration->etbIo[1].directionPin2 = GPIO_UNASSIGNED;

	// we only have pwm/dir, no dira/dirb
	engineConfiguration->etb_use_two_wires = false;
	engineConfiguration->etbFreq = 800;
}

static void setupCanPins() {
	engineConfiguration->canTxPin = GPIOD_1;
	engineConfiguration->canRxPin = GPIOD_0;
}

static void setupDefaultSensorInputs() {
	// trigger inputs
	// Digital channel 1 as default - others not set
	engineConfiguration->triggerInputPins[0] = GPIOC_6;
	engineConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->triggerInputPins[2] = GPIO_UNASSIGNED;
	engineConfiguration->camInputs[0] = GPIO_UNASSIGNED;

	// CLT = Analog Temp 3 = PB0
	engineConfiguration->clt.adcChannel = EFI_ADC_8;
	engineConfiguration->clt.config.bias_resistor = 2700;

	// IAT = Analog Temp 2 = PC5
	engineConfiguration->iat.adcChannel = EFI_ADC_15;
	engineConfiguration->iat.config.bias_resistor = 2700;

	// TPS = Analog volt 2 = PC1
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_11;

	// MAP = Analog volt 1 = PC0
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_10;

	// No battery voltage setting - see adc_hack.cpp
	engineConfiguration->vbattAdcChannel = EFI_ADC_NONE;
}

void setPinConfigurationOverrides(void) {
}

void setSerialConfigurationOverrides(void) {
	engineConfiguration->useSerialPort = false;
	engineConfiguration->binarySerialTxPin = GPIO_UNASSIGNED;
	engineConfiguration->binarySerialRxPin = GPIO_UNASSIGNED;
	engineConfiguration->consoleSerialTxPin = GPIO_UNASSIGNED;
	engineConfiguration->consoleSerialRxPin = GPIO_UNASSIGNED;
}


/**
 * @brief   Board-specific configuration code overrides.
 *
 * See also setDefaultEngineConfiguration
 *
 * @todo    Add your board-specific code, if any.
 */
void setBoardConfigurationOverrides(void) {
	setInjectorPins();
	setIgnitionPins();
	setLedPins();
	setupVbatt();
	setupEtb();
	setupCanPins();

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	// Some sensible defaults for other options
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	setAlgorithm(LM_SPEED_DENSITY PASS_CONFIG_PARAMETER_SUFFIX);

	engineConfiguration->specs.cylindersCount = 8;
	engineConfiguration->specs.firingOrder = FO_1_8_7_2_6_5_4_3;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;

	// output 13
	CONFIG(mainRelayPin) = GPIOB_9;
	// output 15
	CONFIG(fanPin) = GPIOE_1;
	// output 16
	CONFIG(fuelPumpPin) = GPIOE_2;
}
