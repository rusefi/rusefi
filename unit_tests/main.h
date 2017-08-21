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

void chDbgAssert(int c, char *msg, void *arg);

void print(const char *fmt, ...);

#define TICKS_IN_MS 100

#define DEBUG_INTERPOLATION 1

#define chDbgCheck(x, y) chDbgAssert(x, y, NULL)

void assertEqualsM2(const char *msg, float expected, float actual, float EPS);
void assertEqualsM(const char *msg, float expected, float actual);
void assertEqualsLM(const char *msg, long expected, long actual);
void assertEqualsM4(const char *prefix, const char *msg, float expected, float actual);
void assertEqualsM5(const char *prefix, const char *message, float expected, float actual, float EPS);
void assertEquals(float expected, float actual);
void assertTrue(float actual);
void assertTrueM(const char *msg, float actual);
void assertFalse(float actual);
void assertFalseM(const char *msg, float actual);


#define systicks2ms(x) (0)

#ifdef __cplusplus
}
#endif


#endif /* MAIN_H_ */
