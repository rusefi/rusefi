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

char * getPinNameByAdcChannel(const char *msg, adc_channel_e hwChannel, char *buffer);
void initPeriodicEvents(DECLARE_ENGINE_PARAMETER_F);
void initEngineContoller(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_S);
void commonInitEngineController(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_S);

void setVBattVoltage(float voltage);
void setMapVoltage(float voltage);
void setTpsVoltage(float voltage);
void setAfrVoltage(float voltage);
void setMafVoltage(float voltage);
void setIatVoltage(float voltage);
void setCltVoltage(float voltage);

#endif /* ENGINE_STATUS_H_ */
