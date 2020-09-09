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
	engineConfiguration->useSerialPort = true;
	engineConfiguration->binarySerialTxPin = GPIOD_8;
	engineConfiguration->binarySerialRxPin = GPIOD_9;
//	engineConfiguration->consoleSerialTxPin = GPIOD_8;
//	engineConfiguration->consoleSerialRxPin = GPIOD_9;
	engineConfiguration->tunerStudioSerialSpeed = SERIAL_SPEED;
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
	engineConfiguration->externalKnockSenseAdc = EFI_ADC_NONE;
	engineConfiguration->displayMode = DM_NONE;
	engineConfiguration->HD44780_rs = GPIO_UNASSIGNED;
	engineConfiguration->HD44780_e = GPIO_UNASSIGNED;
	engineConfiguration->HD44780_db4 = GPIO_UNASSIGNED;
	engineConfiguration->HD44780_db5 = GPIO_UNASSIGNED;
	engineConfiguration->HD44780_db6 = GPIO_UNASSIGNED;
	engineConfiguration->HD44780_db7 = GPIO_UNASSIGNED;
	for (int i = 0; i < DIGIPOT_COUNT ; i++) {
		engineConfiguration->digitalPotentiometerChipSelect[i] = GPIO_UNASSIGNED;
	}
	engineConfiguration->triggerSimulatorPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->triggerSimulatorPins[2] = GPIO_UNASSIGNED;
	engineConfiguration->triggerSimulatorPinModes[1] = OM_DEFAULT;
	engineConfiguration->triggerSimulatorPinModes[2] = OM_DEFAULT;
	engineConfiguration->vehicleSpeedSensorInputPin = GPIO_UNASSIGNED;

	engineConfiguration->digitalPotentiometerSpiDevice = SPI_NONE;
	engineConfiguration->max31855spiDevice = SPI_NONE;

	/////////////////////////////////////////////////////////
	engineConfiguration->is_enabled_spi_1 = false;
	engineConfiguration->is_enabled_spi_2 = false;
	engineConfiguration->is_enabled_spi_3 = false;
}

void setAdcChannelOverrides(void) {
}
