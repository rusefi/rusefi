/**
 * @file boards/core8/board_configuration.cpp
 *
 * @brief Configuration defaults for the core8 board
 *
 * @author Ben Brazdziunas, (c) 2021
 */

#include "pch.h"

void setSdCardConfigurationOverrides(void) {
}

void setBoardConfigOverrides(void) {
}

void setPinConfigurationOverrides(void) {
}

void setSerialConfigurationOverrides(void) {
}


/**
 * @brief   Board-specific configuration defaults.
 *
 * See also setDefaultEngineConfiguration
 *
 * @todo    Add your board-specific code, if any.
 */
static void setupVbatt() {
	// 5.6k high side/10k low side = 1.56 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 1.47f;

	// 6.34k high side/1k low side = 9.2
	engineConfiguration->vbattDividerCoeff = (6.34f / 1.0f);

	// Battery sense on PA7
	engineConfiguration->vbattAdcChannel = EFI_ADC_0;

	engineConfiguration->adcVcc = 3.3f;
}

static void setupDefaultSensorInputs() {

}

void setBoardDefaultConfiguration(void) {
	
	setupVbatt();
	
	engineConfiguration->sdCardPeriodMs = 50;
	engineConfiguration->isSdCardEnabled = true;


	engineConfiguration->canWriteEnabled = true;
	engineConfiguration->canReadEnabled = true;
	engineConfiguration->canSleepPeriodMs = 50;
	
	engineConfiguration->canTxPin = GPIOD_0;
	engineConfiguration->canRxPin = GPIOD_1;
	engineConfiguration->canBaudRate = B500KBPS;

	
	engineConfiguration->can2RxPin = GPIOB_5;
	engineConfiguration->can2TxPin = GPIOB_6;
	engineConfiguration->can2BaudRate = B500KBPS;

	
	engineConfiguration->clt.config.bias_resistor = 2490;
	engineConfiguration->iat.config.bias_resistor = 2490;

	
	engineConfiguration->lps25BaroSensorScl = GPIOB_10;
	engineConfiguration->lps25BaroSensorSda = GPIOB_11;

}
