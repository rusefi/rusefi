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
	engineConfiguration->analogInputDividerCoefficient = 1.56f;

	// 6.34k high side/1k low side = 9.2
	engineConfiguration->vbattDividerCoeff = (7.38f / 1.0f);

	// Battery sense on PA7
	engineConfiguration->vbattAdcChannel = EFI_ADC_0;

	engineConfiguration->adcVcc = 3.3f;
}

static void setupDefaultSensorInputs() {

}

void setBoardDefaultConfiguration(void) {
	
	setupVbatt();

}
