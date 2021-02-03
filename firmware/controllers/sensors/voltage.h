/**
 * @file    voltage.h
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"
#define LOW_VBATT 7

#ifdef __cplusplus
#include "engine_configuration.h"

float getVBatt(DECLARE_ENGINE_PARAMETER_SIGNATURE);

#endif /* __cplusplus */
