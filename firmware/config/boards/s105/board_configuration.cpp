#include "pch.h"

/**
 * @brief   Board-specific configuration defaults.
 * @todo    Add your board-specific code, if any.
 */
void setBoardDefaultConfiguration() {
    /* No LEDs on this board */
    engineConfiguration->communicationLedPin = Gpio::Unassigned;
    engineConfiguration->runningLedPin = Gpio::Unassigned;
    engineConfiguration->warningLedPin = Gpio::Unassigned;
}
