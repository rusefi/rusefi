/**
 * @file    voltage.h
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef VOLTAGE_H_
#define VOLTAGE_H_

#include "main.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

float getVRef(void);
float getVBatt(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
