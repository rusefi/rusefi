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

/* 12 bits, 5V reference */
#define adcRawToScaledVoltage(adc) (5.0f / 4095 * (adc))

#define adcGetRawValue(channel) getMcp3208adc(channel)
#define adcGetScaledVoltage(msg, channel) (isAdcChannelValid(channel) ? adcToVoltsDivided(adcGetRawValue(msg, channel), channel) : 66.66)
