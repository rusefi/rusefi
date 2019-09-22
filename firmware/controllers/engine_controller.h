/**
 * @file    engine_controller.h
 * @brief   Controllers package entry point header
 *
 * @date Feb 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#pragma once

#include "global.h"
#include "signal_executor.h"
#include "engine_configuration.h"
#include "engine.h"

char * getPinNameByAdcChannel(const char *msg, adc_channel_e hwChannel, char *buffer);
void initPeriodicEvents(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void initEngineContoller(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);
void commonInitEngineController(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);
void mostCommonInitEngineController(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);

#if EFI_ENABLE_MOCK_ADC
void setMockVoltage(int hwChannel, float voltage DECLARE_ENGINE_PARAMETER_SUFFIX);
#endif

void setMockVBattVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX);
void setMockMapVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX);
// throttle body sensor
void setMockThrottlePositionSensorVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX);
void setMockThrottlePedalSensorVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX);
void setMockAfrVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX);
void setMockMafVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX);
void setMockIatVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX);
void setMockCltVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX);

void printCurrentState(Logging *logging, int seconds, const char *engineTypeName, const char *firmwareBuildId);

