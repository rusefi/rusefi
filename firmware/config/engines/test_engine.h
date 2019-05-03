/**
 * @file	test_engine.h
 *
 * @date Nov 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */
#ifndef TEST_ENGINE_H_
#define TEST_ENGINE_H_

#include "engine_configuration.h"

void setTestEngineConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setTestVVTEngineConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);

void setTestEngineIssue366both(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setTestEngineIssue366rise(DECLARE_CONFIG_PARAMETER_SIGNATURE);

#endif /* TEST_ENGINE_H_ */
