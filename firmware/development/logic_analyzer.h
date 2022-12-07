/**
 * @file	logic_analyzer.h
 *
 * @date Jan 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"

#if EFI_LOGIC_ANALYZER

#include "engine_sniffer.h"

void initWaveAnalyzer();
void startLogicAnalyzerPins();
void stopLogicAnalyzerPins();
void printWave(Logging *logging);
void showWaveInfo(void);
void reportLogicAnalyzerToTS(void); 

void waTriggerEventListener(trigger_event_e ckpSignalType, uint32_t index, efitick_t edgeTimestamp);

#endif /* EFI_LOGIC_ANALYZER */

