/**
 * @file boards/ME7_PNP/board_configuration.cpp
 *
 * @brief In this file we can override engine_configuration.cpp.
 *
 * @author Andrey Belomutskiy, (c) 2012-2020
 */
#include "global.h"
#include "engine.h"
#include "engine_math.h"
#include "allsensors.h"
#include "fsio_impl.h"
#include "engine_configuration.h"
#include "smart_gpio.h"

EXTERN_ENGINE;

static void vag_18_Turbo(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
}
void setSerialConfigurationOverrides(void) {
	engineConfiguration->useSerialPort = true;

	//UART

	engineConfiguration->binarySerialTxPin = GPIOB_10;
	engineConfiguration->binarySerialRxPin = GPIOB_11;
//	engineConfiguration->consoleSerialTxPin = GPIOB_10;
//	engineConfiguration->consoleSerialRxPin = GPIOB_11;

}

void setPinConfigurationOverrides(void) {

	//NOT USED

	engineConfiguration->ignitionPins[8] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[9] = GPIO_UNASSIGNED;
	engineConfiguration->mainRelayPin = GPIO_UNASSIGNED;
	engineConfiguration->idle.solenoidPin = GPIO_UNASSIGNED;
	engineConfiguration->fanPin = GPIO_UNASSIGNED;

}
void setBoardConfigurationOverrides(void) {

	// NOT USED
	engineConfiguration->externalKnockSenseAdc = EFI_ADC_NONE;
	for (int i = 0; i < DIGIPOT_COUNT; i++) {
		engineConfiguration->digitalPotentiometerChipSelect[i] = GPIO_UNASSIGNED;
	}
	engineConfiguration->warningLedPin = GPIO_UNASSIGNED;
	engineConfiguration->runningLedPin = GPIO_UNASSIGNED;
	engineConfiguration->useStepperIdle = false;
	engineConfiguration->idle.stepperDirectionPin = GPIO_UNASSIGNED;
	engineConfiguration->idle.stepperStepPin = GPIO_UNASSIGNED;
	engineConfiguration->stepperEnablePin = GPIO_UNASSIGNED;
	engineConfiguration->stepperEnablePinMode = OM_DEFAULT;
	engineConfiguration->isHip9011Enabled = false;

}

void setSdCardConfigurationOverrides(void) {
}
