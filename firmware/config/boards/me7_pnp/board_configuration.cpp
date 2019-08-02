/**
 * @file boards/ME7_PNP/board_configuration.cpp
 *
 * @brief In this file we can override engine_configuration.cpp.
 *
 * @author Andrey Belomutskiy, (c) 2012-2019
 */
#include "global.h"
#include "engine.h"
#include "engine_math.h"
#include "allsensors.h"
#include "fsio_impl.h"
#include "engine_configuration.h"
#include "smart_gpio.h"

EXTERN_ENGINE
;

static void vag_18_Turbo(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
}
void setSerialConfigurationOverrides(void) {
	boardConfiguration->useSerialPort = true;

	//UART

	engineConfiguration->binarySerialTxPin = GPIOB_10;
	engineConfiguration->binarySerialRxPin = GPIOB_11;
	engineConfiguration->consoleSerialTxPin = GPIOB_10;
	engineConfiguration->consoleSerialRxPin = GPIOB_11;

}

void setPinConfigurationOverrides(void) {

	//NOT USED

	engineConfiguration->baroSensor.hwChannel = EFI_ADC_NONE;
	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;
	boardConfiguration->ignitionPins[8] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[9] = GPIO_UNASSIGNED;
	boardConfiguration->mainRelayPin = GPIO_UNASSIGNED;
	boardConfiguration->idle.solenoidPin = GPIO_UNASSIGNED;
	boardConfiguration->fanPin = GPIO_UNASSIGNED;

}
void setBoardConfigurationOverrides(void) {

	// NOT USED
	engineConfiguration->dizzySparkOutputPin = GPIO_UNASSIGNED;
	engineConfiguration->externalKnockSenseAdc = EFI_ADC_NONE;
	engineConfiguration->displayMode = DM_NONE;
	boardConfiguration->HD44780_rs = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_e = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_db4 = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_db5 = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_db6 = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_db7 = GPIO_UNASSIGNED;
	for (int i = 0; i < DIGIPOT_COUNT; i++) {
		boardConfiguration->digitalPotentiometerChipSelect[i] = GPIO_UNASSIGNED;
	}
	boardConfiguration->triggerSimulatorPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->triggerSimulatorPins[2] = GPIO_UNASSIGNED;
	boardConfiguration->triggerSimulatorPinModes[1] = OM_DEFAULT;
	boardConfiguration->triggerSimulatorPinModes[2] = OM_DEFAULT;
	boardConfiguration->digitalPotentiometerSpiDevice = SPI_NONE;
	boardConfiguration->max31855spiDevice = SPI_NONE;
	engineConfiguration->warningLedPin = GPIO_UNASSIGNED;
	engineConfiguration->runningLedPin = GPIO_UNASSIGNED;
	boardConfiguration->useStepperIdle = false;
	boardConfiguration->idle.stepperDirectionPin = GPIO_UNASSIGNED;
	boardConfiguration->idle.stepperStepPin = GPIO_UNASSIGNED;
	engineConfiguration->stepperEnablePin = GPIO_UNASSIGNED;
	engineConfiguration->stepperEnablePinMode = OM_DEFAULT;
	boardConfiguration->injectionPins[8] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[9] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[10] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[11] = GPIO_UNASSIGNED;
	boardConfiguration->isHip9011Enabled = false;

}

void setSdCardConfigurationOverrides(void) {
}

void setAdcChannelOverrides(void) {
}

