/*
 * @file    adc_external.h
 *
 * external ADC is probably broken, at least there is a major mess
 *
 * @date Aug 18, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "mcp3208.h"

#define adcGetRawValue(channel) getMcp3208adc(channel)
#define adcToVoltsDivided(adc) (5.0f / 4095 * (adc))
#define getVoltageDivided(msg, channel) (isAdcChannelValid(channel) ? adcToVoltsDivided(adcGetRawValue(msg, channel), channel) : 66.66)
