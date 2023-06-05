#include "pch.h"

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
void setBoardDefaultConfiguration() {
    engineConfiguration->runningLedPin = Gpio::G13; /* LD3 - green */
}
