/**
 * @file	main.h
 *
 * @date Nov 29, 2012
 * @author Andrey Belomutskiy, (c) 2012-2017
 */
#pragma once
#ifndef MAIN_H_
#define MAIN_H_

#include <math.h>
#include <float.h>

#include "efilib.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "global.h"

#include "rusefi.h"

#include "efifeatures.h"
#include "efitime.h"
#include "rusefi_enums.h"

#ifdef __cplusplus
#include "cli_registry.h"
#include "datalogging.h"
#include "loggingcentral.h"
#include "eficonsole.h"
#endif /* __cplusplus */

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
#define US2NT(us) (((efitime_t)(us))*US_TO_NT_MULTIPLIER)

/**
 * converts efitick_t to efitimeus_t
 */
#define NT2US(nt) ((nt) / US_TO_NT_MULTIPLIER)

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
