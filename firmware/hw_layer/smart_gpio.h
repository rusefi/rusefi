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
 #define BOARD_EXT_PINREPOPINS (BOARD_TLE6240_COUNT * TLE6240_OUTPUTS + BOARD_MC33972_COUNT * MC33972_INPUTS + BOARD_TLE8888_COUNT * TLE8888_OUTPUTS)

#else /* EFI_PROD_CODE */
 #define BOARD_EXT_PINREPOPINS 0
#endif /* EFI_PROD_CODE */

#if EFI_UNIT_TEST
 #define BOARD_EXT_GPIOCHIPS 3
#else
 #define BOARD_EXT_GPIOCHIPS			(BOARD_TLE6240_COUNT + BOARD_MC33972_COUNT + BOARD_TLE8888_COUNT)
#endif

void initSmartGpio(void);
void startSmartCsPins(void);
void stopSmartCsPins(void);

