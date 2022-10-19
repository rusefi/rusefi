/**
 * @file boards/NUCLEO_F767/board_configuration.cpp
 *
 * @brief In this file we can override engine_configuration.cpp.
 *
 * @author andreika, (c) 2018
 */

#include "pch.h"

// Warning! This is a test config!

#undef SERIAL_SPEED
#define SERIAL_SPEED 115200

/**
 * @brief   Board-specific configuration defaults.
 * @todo    Add your board-specific code, if any.
 */
void setBoardDefaultConfiguration() {
	engineConfiguration->binarySerialTxPin = Gpio::D8;
	engineConfiguration->binarySerialRxPin = Gpio::D9;
	engineConfiguration->tunerStudioSerialSpeed = SERIAL_SPEED;
	engineConfiguration->uartConsoleSerialSpeed = SERIAL_SPEED;

	engineConfiguration->runningLedPin = Gpio::B0; //green LED
	engineConfiguration->warningLedPin = Gpio::Unassigned;
#if 0
	engineConfiguration->vbattAdcChannel = EFI_ADC_13;
	engineConfiguration->adcVcc = ADC_VCC;
#endif
	engineConfiguration->baroSensor.hwChannel = EFI_ADC_NONE;
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_NONE;

	// not used
	for (int i = 0; i < DIGIPOT_COUNT ; i++) {
		engineConfiguration->digitalPotentiometerChipSelect[i] = Gpio::Unassigned;
	}
	engineConfiguration->triggerSimulatorPins[1] = Gpio::Unassigned;
	engineConfiguration->triggerSimulatorPinModes[1] = OM_DEFAULT;
	engineConfiguration->vehicleSpeedSensorInputPin = Gpio::Unassigned;

	/////////////////////////////////////////////////////////
	engineConfiguration->is_enabled_spi_1 = false;
	engineConfiguration->is_enabled_spi_2 = false;
	engineConfiguration->is_enabled_spi_3 = false;
}

void preHalInit() {
	efiSetPadMode("Ethernet",  Gpio::A1, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("Ethernet",  Gpio::A2, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("Ethernet",  Gpio::A7, PAL_MODE_ALTERNATE(0xb));

	efiSetPadMode("Ethernet",  Gpio::C1, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("Ethernet",  Gpio::C4, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("Ethernet",  Gpio::C5, PAL_MODE_ALTERNATE(0xb));

	efiSetPadMode("Ethernet",  Gpio::D5, PAL_MODE_ALTERNATE(0xb));

	efiSetPadMode("Ethernet", Gpio::G11, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("Ethernet", Gpio::G13, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("Ethernet", Gpio::G14, PAL_MODE_ALTERNATE(0xb));
}
