/**
 * @file boards/NUCLEO_F767/board_configuration.cpp
 *
 * @brief In this file we can override engine_configuration.cpp.
 *
 * @author andreika, (c) 2018
 */

#include "global.h"
#include "engine.h"
#include "engine_math.h"
#include "allsensors.h"
#include "fsio_impl.h"
#include "engine_configuration.h"

EXTERN_ENGINE;

// Warning! This is a test config!

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
	engineConfiguration->warninigLedPin = GPIO_UNASSIGNED;
	engineConfiguration->configResetPin = GPIO_UNASSIGNED;
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
	boardConfiguration->digitalPotentiometerChipSelect[0] = GPIO_UNASSIGNED;
	boardConfiguration->digitalPotentiometerChipSelect[1] = GPIO_UNASSIGNED;
	boardConfiguration->digitalPotentiometerChipSelect[2] = GPIO_UNASSIGNED;
	boardConfiguration->digitalPotentiometerChipSelect[3] = GPIO_UNASSIGNED;
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
}

