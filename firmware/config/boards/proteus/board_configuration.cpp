/**
 * @file boards/proteus/board_configuration.cpp
 *
 * @brief Configuration defaults for the Proteus board
 *
 * @author Matthew Kennedy, (c) 2019
 */

#include "allsensors.h"
#include "fsio_impl.h"

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
	// PE3 is error LED, configured in board.mk
	CONFIG(communicationLedPin) = GPIOE_4;
	CONFIG(runningLedPin) = GPIOE_5;
	CONFIG(warningLedPin) = GPIOE_6;
}

static void setupVbatt() {
	// 5.6k high side/10k low side = 1.56 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 1.56f;

	// 82k high side/10k low side = 9.2
	engineConfiguration->vbattDividerCoeff = (92.0f / 10.0f);

	// Battery sense on PA7
	engineConfiguration->vbattAdcChannel = EFI_ADC_7;

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
	engineConfiguration->etbIo[0].controlPin = GPIOD_12;
	// DIR pin
	engineConfiguration->etbIo[0].directionPin1 = GPIOD_10;
	// Disable pin
	engineConfiguration->etbIo[0].disablePin = GPIOD_11;
	// Unused
	engineConfiguration->etbIo[0].directionPin2 = GPIO_UNASSIGNED;

	// Throttle #2
	// PWM pin
	engineConfiguration->etbIo[1].controlPin = GPIOD_13;
	// DIR pin
	engineConfiguration->etbIo[1].directionPin1 = GPIOD_9;
	// Disable pin
	engineConfiguration->etbIo[1].disablePin = GPIOD_8;
	// Unused
	engineConfiguration->etbIo[1].directionPin2 = GPIO_UNASSIGNED;

	// we only have pwm/dir, no dira/dirb
	engineConfiguration->etb_use_two_wires = false;
}

static void setupDefaultSensorInputs() {
	// trigger inputs
#if VR_HW_CHECK_MODE
	// set_trigger_input_pin 0 PE7
	// GPIOE_7:  "VR 1"
	engineConfiguration->triggerInputPins[0] = GPIOE_7;
	// GPIOE_8:  "VR 2"
	engineConfiguration->camInputs[0] = GPIOE_8;
#else
	// Digital channel 1 as default - others not set
	engineConfiguration->triggerInputPins[0] = GPIOC_6;
	engineConfiguration->camInputs[0] = GPIO_UNASSIGNED;
#endif

	engineConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->triggerInputPins[2] = GPIO_UNASSIGNED;

	// CLT = Analog Temp 3 = PB0
	engineConfiguration->clt.adcChannel = EFI_ADC_8;

	// IAT = Analog Temp 2 = PC5
	engineConfiguration->iat.adcChannel = EFI_ADC_15;

	// TPS = Analog volt 2 = PC1
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_11;

	// MAP = Analog volt 1 = PC0
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_10;

	// pin #28 WBO AFR "Analog Volt 10"
	engineConfiguration->afr.hwChannel = EFI_ADC_5;
}

static void setupSdCard() {

	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_3;
	engineConfiguration->sdCardCsPin = GPIOD_2;

	engineConfiguration->is_enabled_spi_3 = true;
	engineConfiguration->spi3sckPin = GPIOC_10;
	engineConfiguration->spi3misoPin = GPIOC_11;
	engineConfiguration->spi3mosiPin = GPIOC_12;
}

void setBoardConfigOverrides(void) {
	setupSdCard();
	setupEtb();
	setLedPins();
	setupVbatt();

	engineConfiguration->clt.config.bias_resistor = 2700;
	engineConfiguration->iat.config.bias_resistor = 2700;

	engineConfiguration->canTxPin = GPIOD_1;
	engineConfiguration->canRxPin = GPIOD_0;

	engineConfiguration->lps25BaroSensorScl = GPIOB_10;
	engineConfiguration->lps25BaroSensorSda = GPIOB_11;
}

void setPinConfigurationOverrides(void) {
}

void setSerialConfigurationOverrides(void) {
	engineConfiguration->useSerialPort = false;
	engineConfiguration->binarySerialTxPin = GPIO_UNASSIGNED;
	engineConfiguration->binarySerialRxPin = GPIO_UNASSIGNED;
//	engineConfiguration->consoleSerialTxPin = GPIO_UNASSIGNED;
//	engineConfiguration->consoleSerialRxPin = GPIO_UNASSIGNED;
}


/**
 * @brief   Board-specific configuration defaults.
 *
 * See also setDefaultEngineConfiguration
 *
 * @todo    Add your board-specific code, if any.
 */
void setBoardDefaultConfiguration(void) {
	setInjectorPins();
	setIgnitionPins();

	engineConfiguration->isSdCardEnabled = true;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	// Some sensible defaults for other options
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	setAlgorithm(LM_SPEED_DENSITY PASS_CONFIG_PARAMETER_SUFFIX);

	engineConfiguration->specs.cylindersCount = 8;
	engineConfiguration->specs.firingOrder = FO_1_8_7_2_6_5_4_3;

	CONFIG(enableSoftwareKnock) = true;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;

	CONFIG(mainRelayPin) = GPIOB_9;//  "Lowside 13"    # pin 10/black35
	CONFIG(fanPin) = GPIOE_1;//  "Lowside 15"    # pin 12/black35
	CONFIG(fuelPumpPin) = GPIOE_2;//  "Lowside 16"    # pin 23/black35

	// If we're running as hardware CI, borrow a few extra pins for that
#ifdef HARDWARE_CI
	engineConfiguration->triggerSimulatorPins[0] = GPIOG_3;
	engineConfiguration->triggerSimulatorPins[1] = GPIOG_2;
#endif
}
