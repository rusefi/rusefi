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
#include "rusefi_enums.h"
#include "enum_strings.h"

#include "datalogging.h"
#include "chprintf.h"

#ifdef __cplusplus
}
#endif /* __cplusplus */

// todo: access some existing configuration field
#define CORE_CLOCK 168000000

// 168 ticks in microsecond
#define US_TO_NT_MULTIPLIER 168

/**
 * converts efitimeus_t to efitick_t
 */
#define US2NT(us) (((uint64_t)(us))*US_TO_NT_MULTIPLIER)

/**
 * converts efitick_t to efitimeus_t
 */
#define NT2US(nt) ((nt) / US_TO_NT_MULTIPLIER)


/**
 * number of SysClock ticks in one ms
 */
#define TICKS_IN_MS  (CH_FREQUENCY / 1000)

#define Delay(ms) chThdSleepMilliseconds(ms)
#ifdef __cplusplus
extern "C"
{
#endif
int systicks2ms(int systicks);

bool lockAnyContext(void);
void unlockAnyContext(void);

#ifdef __cplusplus
}
#endif


#endif /* MAIN_H_ */
