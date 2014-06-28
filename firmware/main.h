/**
 * @file	main.h
 *
 * @date Nov 29, 2012
 * @author Andrey Belomutskiy, (c) 2012-2014
 */
#pragma once
#ifndef MAIN_H_
#define MAIN_H_

#include <math.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "global.h"

#include "eficonsole.h"
#include "cli_registry.h"
#include "efilib.h"
#include "rusefi.h"

#include "efifeatures.h"
#include "efitime.h"
#include "boards.h"
#include "rusefi_enums.h"

#include "datalogging.h"
#include "chprintf.h"

#include "stm32f4xx_specific.h"

#ifdef __cplusplus
}
#endif /* __cplusplus */

// todo: access some existing configuration field
#define CORE_CLOCK 168000000

/**
 * number of SysClock ticks in one ms
 */
#define TICKS_IN_MS  (CH_FREQUENCY / 1000)

#define Delay(ms) chThdSleepMilliseconds(ms)
#ifdef __cplusplus
extern "C"
{
#endif
void updateHD44780lcd(void);
int systicks2ms(int systicks);

bool lockAnyContext(void);
void unlockAnyContext(void);

#ifdef __cplusplus
}
#endif


#endif /* MAIN_H_ */
