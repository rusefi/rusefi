#include "pch.h"

/**
 * @brief Board-specific initialization code.
 * @todo  Add your board-specific code, if any.
 */
void boardInit(void)
{
	/* NOP */
}

Gpio getRunningLedPin() {
	return Gpio::G13; /* LD3 - green */
}

/**
 * @brief   Board-specific configuration defaults.

 */
void setBoardDefaultConfiguration() {
}
