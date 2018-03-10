/**
 * @file	main.h
 * @brief	Test version of main.h
 *
 *  Created on: Oct 17, 2013
 *      Author: Andrey Belomutskiy (C) 2012-2013
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <math.h>
#include "efitime.h"
#include "global.h"

#include "error_handling.h"
#include "efifeatures.h"

#include "efilib.h"
#include "efitime.h"

#include "boards.h"

#ifdef __cplusplus
#include "datalogging.h"

extern "C"
{
#endif

#define CH_FREQUENCY 1000

typedef int bool_t;
typedef uint32_t systime_t;

void chDbgAssert(int c, char *msg, void *arg);

void print(const char *fmt, ...);

#define TICKS_IN_MS 100

#define chDbgCheck(x, y) chDbgAssert(x, y, NULL)

#define systicks2ms(x) (0)

#ifdef __cplusplus
}
#endif


#endif /* MAIN_H_ */
