/**
 * @file boards/microrusefi/board_configuration.cpp
 *
 * @brief In this file we can override engine_configuration.cpp.
 *
 * @author Matthew Kennedy, (c) 2019
 */

#include "global.h"
#include "engine.h"
#include "engine_math.h"
#include "allsensors.h"
#include "fsio_impl.h"
#include "engine_configuration.h"

EXTERN_ENGINE;

static void setInjectorPins()
{
	boardConfiguration->injectionPins[0] = GPIOE_14;
	boardConfiguration->injectionPins[1] = GPIOE_13;
	boardConfiguration->injectionPins[2] = GPIOE_12;
	boardConfiguration->injectionPins[3] = GPIOE_11;

	// Disable remainder
	for (int i = 4; i < INJECTION_PIN_COUNT;i++) {
		boardConfiguration->injectionPins[i] = GPIO_UNASSIGNED;
	}

	boardConfiguration->injectionPinMode = OM_DEFAULT;
}

static void setIgnitionPins()
{
	boardConfiguration->ignitionPins[0] = GPIOD_4;
	boardConfiguration->ignitionPins[1] = GPIOD_3;
	boardConfiguration->ignitionPins[2] = GPIOD_2;
	boardConfiguration->ignitionPins[3] = GPIOD_1;

	// disable remainder
	for (int i = 4; i < IGNITION_PIN_COUNT; i++) {
		boardConfiguration->ignitionPins[i] = GPIO_UNASSIGNED;
	}

	boardConfiguration->ignitionPinMode = OM_DEFAULT;
}

static void setLedPins()
{
	engineConfiguration->fatalErrorPin = GPIOE_3;		// d21 = red
	engineConfiguration->communicationLedPin = GPIOE_2; // d23 = blue
	engineConfiguration->runningLedPin = GPIOE_4;		// d22 = green
	boardConfiguration->triggerErrorPin = GPIOE_1;		// d27 = orange
}

static void setupVbatt()
{
	engineConfiguration->vbattDividerCoeff = 8.166666f;
	engineConfiguration->vbattAdcChannel = EFI_ADC_11;

	// 1k high side/1.5k low side = 1.6667 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 2.5f / 1.5f;

	engineConfiguration->adcVcc = 3.29f;
}

static void setupTle8888()
{
	// Enable spi3
	boardConfiguration->is_enabled_spi_3 = true;

	// Wire up spi3
	boardConfiguration->spi3mosiPin = GPIOB_5;
	boardConfiguration->spi3misoPin = GPIOB_4;
	boardConfiguration->spi3sckPin = GPIOB_3;

	// Chip select
	engineConfiguration->tle8888_cs = GPIOD_5;

	// SPI device
	engineConfiguration->tle8888spiDevice = SPI_DEVICE_3;
}

static void setupEtb()
{
	// TLE9201 driver
	// This chip has three control pins:
	// DIR - sets direction of the motor
	// PWM - pwm control (enable high, coast low)
	// DIS - disables motor (enable low)

	// PWM pin
	boardConfiguration->etb1.controlPin1 = GPIOC_7;
	// DIR pin
	boardConfiguration->etb1.directionPin1 = GPIOA_8;

	// Unused
	boardConfiguration->etb1.controlPin2 = GPIO_UNASSIGNED;
	boardConfiguration->etb1.directionPin2 = GPIO_UNASSIGNED;

	// we only have pwm/dir, no dira/dirb
	engineConfiguration->etb1_use_two_wires = false;

	engineConfiguration->etbFreq = 800;
}

static void setupDefaultSensorInputs()
{
	// trigger inputs
	// tle8888 VR conditioner
	boardConfiguration->triggerInputPins[0] = GPIOC_6;
	boardConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->triggerInputPins[2] = GPIO_UNASSIGNED;
	// Direct hall-only cam input
	engineConfiguration->camInputs[0] = GPIOA_5;

	// tps
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_13;
	engineConfiguration->tps2_1AdcChannel = EFI_ADC_NONE;

	// clt = AN temp 1
	engineConfiguration->clt.adcChannel = EFI_ADC_0;
	engineConfiguration->clt.config.bias_resistor = 2700;

	// iat = AN temp 2
	engineConfiguration->iat.adcChannel = EFI_ADC_1;
	engineConfiguration->iat.config.bias_resistor = 2700;
}

void setPinConfigurationOverrides(void) {
}

void setSerialConfigurationOverrides(void) {
	boardConfiguration->useSerialPort = false;
	engineConfiguration->binarySerialTxPin = GPIO_UNASSIGNED;
	engineConfiguration->binarySerialRxPin = GPIO_UNASSIGNED;
	engineConfiguration->consoleSerialTxPin = GPIO_UNASSIGNED;
	engineConfiguration->consoleSerialRxPin = GPIO_UNASSIGNED;
}


/**
 * @brief   Board-specific configuration code overrides.
 * @todo    Add your board-specific code, if any.
 */
void setBoardConfigurationOverrides(void) {
	engineConfiguration->baroSensor.hwChannel = EFI_ADC_NONE;
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_NONE;

	// not used
	engineConfiguration->dizzySparkOutputPin = GPIO_UNASSIGNED;
	engineConfiguration->externalKnockSenseAdc = EFI_ADC_NONE;
	engineConfiguration->displayMode = DM_NONE;
	boardConfiguration->HD44780_rs = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_e = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_db4 = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_db5 = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_db6 = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_db7 = GPIO_UNASSIGNED;
	for (int i = 0; i < DIGIPOT_COUNT ; i++) {
		boardConfiguration->digitalPotentiometerChipSelect[i] = GPIO_UNASSIGNED;
	}
	boardConfiguration->triggerSimulatorPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->triggerSimulatorPins[2] = GPIO_UNASSIGNED;
	boardConfiguration->triggerSimulatorPinModes[1] = OM_DEFAULT;
	boardConfiguration->triggerSimulatorPinModes[2] = OM_DEFAULT;
	boardConfiguration->vehicleSpeedSensorInputPin = GPIO_UNASSIGNED;
	boardConfiguration->boardTestModeJumperPin = GPIO_UNASSIGNED;
	boardConfiguration->acRelayPin = GPIO_UNASSIGNED;
	boardConfiguration->digitalPotentiometerSpiDevice = SPI_NONE;
	boardConfiguration->max31855spiDevice = SPI_NONE;

	/////////////////////////////////////////////////////////
	boardConfiguration->is_enabled_spi_1 = false;
	boardConfiguration->is_enabled_spi_2 = false;
	boardConfiguration->is_enabled_spi_3 = false;

	setInjectorPins();
	setIgnitionPins();
	setLedPins();
	setupVbatt();
	setupTle8888();
	setupEtb();

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	// Some sensible defaults for other options
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR/*FOUR_STROKE_CAM_SENSOR*/);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	//engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	setAlgorithm(LM_SPEED_DENSITY PASS_CONFIG_PARAMETER_SUFFIX);

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;//IM_BATCH;// IM_SEQUENTIAL;
}

void setAdcChannelOverrides(void) {
}
