/*
 * @file engine_template.cpp
 *
 * @date
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "engine_template.h"
#include "custom_engine.h"

EXTERN_ENGINE;

void setEngineTemplateConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
}
