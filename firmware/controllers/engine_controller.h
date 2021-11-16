/**
 * @file    engine_controller.h
 * @brief   Controllers package entry point header
 *
 * @date Feb 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

bool validateConfig();
char * getPinNameByAdcChannel(const char *msg, adc_channel_e hwChannel, char *buffer);
void initPeriodicEvents(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void initEngineContoller(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void commonInitEngineController(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void initStartStopButton(DECLARE_ENGINE_PARAMETER_SIGNATURE);

void initDataStructures(DECLARE_ENGINE_PARAMETER_SIGNATURE);

void slowStartStopButtonCallback(DECLARE_ENGINE_PARAMETER_SIGNATURE);

#if EFI_ENABLE_MOCK_ADC
void setMockVoltage(int hwChannel, float voltage DECLARE_ENGINE_PARAMETER_SUFFIX);
#endif

void setMockMapVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX);
void setMockAfrVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX);
void setMockMafVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX);

void printCurrentState(Logging *logging, int seconds, const char *engineTypeName, const char *firmwareBuildId);
