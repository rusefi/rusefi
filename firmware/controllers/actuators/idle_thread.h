/**
 * @file	idle_thread.h
 * @brief	Idle Valve Control thread
 *
 * @date May 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_ptr.h"
#include "rusefi_types.h"
#include "periodic_task.h"

class Logging;
class Pid;

class IdleController {
public:
	DECLARE_ENGINE_PTR;

	float getIdlePosition();
	void update();
};

void updateIdleControl();
percent_t getIdlePosition();

void applyIACposition(percent_t position DECLARE_ENGINE_PARAMETER_SUFFIX);
void setManualIdleValvePosition(int positionPercent);

void startIdleThread(Logging* sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);
void setDefaultIdleParameters(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void startIdleBench(void);
void setIdleOffset(float value);
void setIdlePFactor(float value);
void setIdleIFactor(float value);
void setIdleDFactor(float value);
void setIdleMode(idle_mode_e value DECLARE_ENGINE_PARAMETER_SUFFIX);
void setTargetIdleRpm(int value);
void onConfigurationChangeIdleCallback(engine_configuration_s *previousConfiguration);
float getIdlePidOffset(DECLARE_ENGINE_PARAMETER_SIGNATURE);
Pid * getIdlePid(DECLARE_ENGINE_PARAMETER_SIGNATURE);
float getIdlePidMinValue(DECLARE_ENGINE_PARAMETER_SIGNATURE);
