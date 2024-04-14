/*
 * @file smart_gpio.h
 *
 * @date Apr 13, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#if EFI_PROD_CODE
#include "drivers/gpio/mc33810.h"
#include "drivers/gpio/tle6240.h"
#include "drivers/gpio/mc33972.h"
#include "drivers/gpio/tle8888.h"
#include "drivers/gpio/drv8860.h"
#include "drivers/gpio/can_gpio_msiobox.h"
// we seem OK without L9779 here do we need those includes at all?
#endif /* EFI_PROD_CODE */

#if EFI_SIMULATOR
#define BOARD_EXT_GPIOCHIPS 0
#elif EFI_UNIT_TEST || EFI_SIMULATOR
#define BOARD_EXT_GPIOCHIPS 3
#else
#define BOARD_EXT_GPIOCHIPS (\
BOARD_TLE6240_COUNT + \
BOARD_MC33972_COUNT + \
BOARD_TLE8888_COUNT + \
BOARD_DRV8860_COUNT + \
BOARD_MC33810_COUNT + \
BOARD_L9779_COUNT + \
BOARD_CAN_GPIO_COUNT + \
0)
#endif

void initSmartGpio(void);
void startSmartCsPins(void);
void stopSmartCsPins(void);

void tle8888startup(void);
