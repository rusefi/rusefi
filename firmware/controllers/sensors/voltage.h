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
#include "engine_configuration.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

float getVRef(void);
float getVBatt(engine_configuration_s *engineConfiguration);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
