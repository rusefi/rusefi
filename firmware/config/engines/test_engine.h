/**
 * @file	test_engine.h
 *
 * @date Nov 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */
#ifndef TEST_ENGINE_H_
#define TEST_ENGINE_H_

#include "engine_configuration.h"

void setTestEngineConfiguration(DECLARE_ENGINE_PARAMETER_F);
void setTestVVTEngineConfiguration(DECLARE_ENGINE_PARAMETER_F);

void setTestEngineIssue366both(DECLARE_ENGINE_PARAMETER_F);
void setTestEngineIssue366rise(DECLARE_ENGINE_PARAMETER_F);

#endif /* TEST_ENGINE_H_ */
