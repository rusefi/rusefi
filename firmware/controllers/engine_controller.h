/**
 * @file    engine_controller.h
 * @brief   Controllers package entry point header
 *
 * @date Feb 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once


bool validateConfigOnStartUpOrBurn();
bool validateConfigOnStartUpOrBurn(bool isRunningOnBurn);
char * getPinNameByAdcChannel(const char *msg, adc_channel_e hwChannel, char *buffer, size_t bufferSize);
void initPeriodicEvents();
// see also applyNewHardwareSettings
void initRealHardwareEngineController();
void commonEarlyInit();
void commonInitEngineController();
void initStartStopButton();
void initWarningRunningPins();

void initDataStructures();

void slowStartStopButtonCallback();
