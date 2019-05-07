/*
 * @file spark_logic.h
 *
 * @date Sep 15, 2016
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef CONTROLLERS_TRIGGER_SPARK_LOGIC_H_
#define CONTROLLERS_TRIGGER_SPARK_LOGIC_H_

#include "engine.h"

int isInjectionEnabled(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void onTriggerEventSparkLogic(bool limitedSpark, uint32_t trgEventIndex, int rpm DECLARE_ENGINE_PARAMETER_SUFFIX);
void initSparkLogic(Logging *sharedLogger);
void turnSparkPinHigh(IgnitionEvent *event);
void turnSparkPinLow(IgnitionEvent *event);
int getNumberOfSparks(ignition_mode_e mode DECLARE_ENGINE_PARAMETER_SUFFIX);
percent_t getCoilDutyCycle(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX);

#endif /* CONTROLLERS_TRIGGER_SPARK_LOGIC_H_ */
