/**
 * @file    engine_controller.h
 * @brief   Controllers package entry point header
 *
 * @date Feb 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef ENGINE_STATUS_H_
#define ENGINE_STATUS_H_

#include "global.h"
#include "signal_executor.h"
#include "engine_configuration.h"
#include "engine.h"

/**
 * I am not sure if this needs to be configurable.
 *
 * Also technically the whole feature might be implemented as cranking fuel coefficient curve by TPS.
 */
#define CLEANUP_MODE_TPS 90
#define STEPPER_PARKING_TPS CLEANUP_MODE_TPS

char * getPinNameByAdcChannel(const char *msg, adc_channel_e hwChannel, char *buffer);
void initPeriodicEvents(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void initEngineContoller(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);
void commonInitEngineController(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);

void setMockVBattVoltage(float voltage);
void setMockMapVoltage(float voltage);
void setMockTpsVoltage(float voltage);
void setMockAfrVoltage(float voltage);
void setMockMafVoltage(float voltage);
void setMockIatVoltage(float voltage);
void setMockCltVoltage(float voltage);

#endif /* ENGINE_STATUS_H_ */
