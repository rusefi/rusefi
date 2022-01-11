#include "pch.h"

void setPinConfigurationOverrides(void) {
}

void setSerialConfigurationOverrides(void) {
}

void setSdCardConfigurationOverrides(void) {
}

/**
 * @brief Board-specific initialization code.
 * @todo  Add your board-specific code, if any.
 */
void boardInit(void)
{
	/* NOP */
}

/**
 * @brief   Board-specific configuration defaults.
 * @todo    Add your board-specific code, if any.
 */
void setBoardDefaultConfiguration(void) {
    engineConfiguration->communicationLedPin = GPIO_UNASSIGNED;
    engineConfiguration->runningLedPin = GPIOG_13; /* LD3 - green */
    engineConfiguration->warningLedPin = GPIO_UNASSIGNED;
}