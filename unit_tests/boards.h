/*
 * @file boards.h
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_configuration.h"
#include "adc_math.h"

#define ADC_CHANNEL_VREF 0

int getAdcValue(const char *msg, adc_channel_e channel);
