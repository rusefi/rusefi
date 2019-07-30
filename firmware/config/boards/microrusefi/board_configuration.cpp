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

	engineConfiguration->adcVcc = 3.3f;
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


#undef SERIAL_SPEED
#define SERIAL_SPEED 115200

void setPinConfigurationOverrides(void) {
}

void setSerialConfigurationOverrides(void) {
	boardConfiguration->useSerialPort = true;
	engineConfiguration->binarySerialTxPin = GPIOD_8;
	engineConfiguration->binarySerialRxPin = GPIOD_9;
	engineConfiguration->consoleSerialTxPin = GPIOD_8;
	engineConfiguration->consoleSerialRxPin = GPIOD_9;
	boardConfiguration->tunerStudioSerialSpeed = SERIAL_SPEED;
	engineConfiguration->uartConsoleSerialSpeed = SERIAL_SPEED;
}

void setSdCardConfigurationOverrides(void) {
}

/**
 * @brief   Board-specific configuration code overrides.
 * @todo    Add your board-specific code, if any.
 */
void setBoardConfigurationOverrides(void) {
	setSerialConfigurationOverrides();

	engineConfiguration->runningLedPin = GPIOB_0; //green LED
	engineConfiguration->warningLedPin = GPIO_UNASSIGNED;
#if 0
	engineConfiguration->vbattAdcChannel = EFI_ADC_13;
	engineConfiguration->adcVcc = ADC_VCC;
#endif
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

	engineConfiguration->tpsMin = 143;
	engineConfiguration->tpsMin = 816;
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_12;
}

void setAdcChannelOverrides(void) {
}
