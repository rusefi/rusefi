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
#define adcRawValueToScaledVoltage(adc) (5.0f / 4095 * (adc))

#define adcGetRawValue(channel) getMcp3208adc(channel)
/* NOTE: no call to getAnalogInputDividerCoefficient() */
#define adcGetScaledVoltage(msg, channel) (isAdcChannelValid(channel) ? adcRawValueToScaledVoltage(adcGetRawValue(msg, channel), channel) : 66.66)
