/**
 * @file	idle_thread.h
 * @brief	Idle Valve Control thread
 *
 * @date May 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine.h"
#include "periodic_task.h"

class IdleController : public PeriodicTimerController {
public:
	DECLARE_ENGINE_PTR;

	int getPeriodMs() override;
	void PeriodicTask() override;
};

percent_t getIdlePosition(void);
void setIdleValvePosition(int positionPercent);
void startIdleThread(Logging*sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);
void setDefaultIdleParameters(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void startIdleBench(void);
void setIdleDT(int value);
void setIdleOffset(float value);
void setIdlePFactor(float value);
void setIdleIFactor(float value);
void setIdleDFactor(float value);
void setIdleMode(idle_mode_e value);
void setTargetIdleRpm(int value);
void setIdleDT(int value);
void stopIdleHardware(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void initIdleHardware(DECLARE_ENGINE_PARAMETER_SIGNATURE);
bool isIdleHardwareRestartNeeded();
void onConfigurationChangeIdleCallback(engine_configuration_s *previousConfiguration);

