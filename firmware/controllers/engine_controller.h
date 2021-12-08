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
void initPeriodicEvents();
void initEngineContoller();
void commonInitEngineController();
void initStartStopButton();

void initDataStructures();

void slowStartStopButtonCallback();

#if EFI_ENABLE_MOCK_ADC
void setMockVoltage(int hwChannel, float voltage);
#endif

void setMockMapVoltage(float voltage);
void setMockAfrVoltage(float voltage);
void setMockMafVoltage(float voltage);

void printCurrentState(Logging *logging, int seconds, const char *engineTypeName, const char *firmwareBuildId);
