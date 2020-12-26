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

struct IIdleController {
	enum class Phase : uint8_t {
		Cranking,	// Below cranking threshold
		Idling,		// Below idle RPM, off throttle
		Coasting,	// Off throttle but above idle RPM
		Running,	// On throttle
	};

	virtual Phase determinePhase(int rpm, int targetRpm, SensorResult tps) const;
	virtual int getTargetRpm(float clt) const = 0;
};

class Logging;
class Pid;


class IdleController : public IIdleController {
public:
	DECLARE_ENGINE_PTR;

	float getIdlePosition();
	void update();

	// TARGET DETERMINATION
	int getTargetRpm(float clt) const override;

	// PHASE DETERMINATION: what is the driver trying to do right now?
	Phase determinePhase(int rpm, int targetRpm, SensorResult tps) const override;
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
