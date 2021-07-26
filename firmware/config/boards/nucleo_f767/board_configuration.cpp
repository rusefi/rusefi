/**
 * @file boards/NUCLEO_F767/board_configuration.cpp
 *
 * @brief In this file we can override engine_configuration.cpp.
 *
 * @author andreika, (c) 2018
 */

#include "pch.h"
#include "fsio_impl.h"

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
 * @brief   Board-specific configuration defaults.
 * @todo    Add your board-specific code, if any.
 */
void setBoardDefaultConfiguration(void) {
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
	for (int i = 0; i < DIGIPOT_COUNT ; i++) {
		engineConfiguration->digitalPotentiometerChipSelect[i] = GPIO_UNASSIGNED;
	}
	engineConfiguration->triggerSimulatorPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->triggerSimulatorPins[2] = GPIO_UNASSIGNED;
	engineConfiguration->triggerSimulatorPinModes[1] = OM_DEFAULT;
	engineConfiguration->triggerSimulatorPinModes[2] = OM_DEFAULT;
	engineConfiguration->vehicleSpeedSensorInputPin = GPIO_UNASSIGNED;

	/////////////////////////////////////////////////////////
	engineConfiguration->is_enabled_spi_1 = false;
	engineConfiguration->is_enabled_spi_2 = false;
	engineConfiguration->is_enabled_spi_3 = false;
}
