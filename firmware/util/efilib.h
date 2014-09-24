/**
 * @file	efilib.h
 *
 * @date Feb 21, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef EFILIB_H_
#define EFILIB_H_

#include <stdint.h>
#include "global.h"

// number of milliseconds in one period of given frequency (per second)
#define frequency2periodMs(freq) ((1000.0f) / (freq))

// number of microseconds in one period of given frequency (per second)
#define frequency2periodUs(freq) ((1000000.0f) / (freq))

#ifndef FALSE
#define FALSE       0
#endif
#ifndef TRUE
#define TRUE        (!FALSE)
#endif

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define ERROR_CODE -11223344

const char * boolToString(bool value);

uint32_t efiStrlen(const char *param);
int indexOf(const char *string, char ch);
float atoff(const char *string);
int atoi(const char *string);
bool cisnan(float f);

int absI(int32_t value);
float absF(float value);
float efiRound(float value, float precision);
int maxI(int i1, int i2);
int minI(int i1, int i2);
float maxF(float i1, float i2);
char* itoa10(char *p, int num);
bool isSameF(float v1, float v2);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EFILIB_H_ */
