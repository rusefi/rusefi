/**
 * @file    engine_controller.h
 * @brief   Controllers package entry point header
 *
 * @date Feb 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#define FAST_CALLBACK_PERIOD_MS 5
#define SLOW_CALLBACK_PERIOD_MS 50

bool validateConfigOnStartUp();
char * getPinNameByAdcChannel(const char *msg, adc_channel_e hwChannel, char *buffer);
void initPeriodicEvents();
// see also applyNewHardwareSettings
void initRealHardwareEngineController();
void commonEarlyInit();
void commonInitEngineController();
void initStartStopButton();
void initWarningRunningPins();

void initDataStructures();

void slowStartStopButtonCallback();
