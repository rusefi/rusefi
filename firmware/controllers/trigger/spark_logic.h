/*
 * @file spark_logic.h
 *
 * @date Sep 15, 2016
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#ifndef CONTROLLERS_TRIGGER_SPARK_LOGIC_H_
#define CONTROLLERS_TRIGGER_SPARK_LOGIC_H_

#include "engine.h"

int isInjectionEnabled(engine_configuration_s *engineConfiguration);
void handleSpark(int revolutionIndex, bool limitedSpark, uint32_t trgEventIndex, int rpm,
		IgnitionEventList *list DECLARE_ENGINE_PARAMETER_S);
void initSparkLogic(Logging *sharedLogger);
void turnSparkPinHigh(IgnitionOutputPin *output);
void turnSparkPinLow(IgnitionOutputPin *output);

#endif /* CONTROLLERS_TRIGGER_SPARK_LOGIC_H_ */
