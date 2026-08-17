#pragma once

#include "gpio/gpio_ext.h"

int hbridgeGpio_add();

#if EFI_UNIT_TEST
GpioChip& getHbridgeGpioForUnitTest();
#endif
