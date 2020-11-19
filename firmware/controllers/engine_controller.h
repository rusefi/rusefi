/**
 * @file    engine_controller.h
 * @brief   Controllers package entry point header
 *
 * @date Feb 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine.h"

char * getPinNameByAdcChannel(const char *msg, adc_channel_e hwChannel, char *buffer);
void initPeriodicEvents(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void initEngineContoller(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);
void commonInitEngineController(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);
void initStartStopButton(DECLARE_ENGINE_PARAMETER_SIGNATURE);

void initDataStructures(DECLARE_ENGINE_PARAMETER_SIGNATURE);

void slowStartStopButtonCallback(DECLARE_ENGINE_PARAMETER_SIGNATURE);

#if EFI_ENABLE_MOCK_ADC
void setMockVoltage(int hwChannel, float voltage DECLARE_ENGINE_PARAMETER_SUFFIX);
#endif

void setMockVBattVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX);
void setMockMapVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX);
void setMockAfrVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX);
void setMockMafVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX);
void setMockIatVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX);
void setMockCltVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX);
void setMockState(brain_pin_e pin, bool state);

void printCurrentState(Logging *logging, int seconds, const char *engineTypeName, const char *firmwareBuildId);

