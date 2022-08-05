/**
 * @file	idle_thread.h
 * @brief	Idle Valve Control thread
 *
 * @date May 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_module.h"
#include "rusefi_types.h"
#include "periodic_task.h"
#include "efi_pid.h"

struct IIdleController {
	enum class Phase : uint8_t {
		Cranking,	// Below cranking threshold
		Idling,		// Below idle RPM, off throttle
		Coasting,	// Off throttle but above idle RPM
		CrankToIdleTaper, // Taper between cranking and idling
		Running,	// On throttle
	};

	virtual Phase determinePhase(int rpm, int targetRpm, SensorResult tps, float vss, float crankingTaperFraction) = 0;
	virtual int getTargetRpm(float clt) = 0;
	virtual float getCrankingOpenLoop(float clt) const = 0;
	virtual float getRunningOpenLoop(float clt, SensorResult tps) = 0;
	virtual float getOpenLoop(Phase phase, float clt, SensorResult tps, float crankingTaperFraction) = 0;
	virtual float getClosedLoop(Phase phase, float tps, int rpm, int target) = 0;
	virtual float getCrankingTaperFraction() const = 0;
	virtual bool isIdlingOrTaper() const = 0;
	virtual float getIdleTimingAdjustment(int rpm) = 0;
};

class IdleController : public IIdleController, public EngineModule, public idle_state_s {
public:
	typedef IIdleController interface_t;

	void init();

	float getIdlePosition();

	// TARGET DETERMINATION
	int getTargetRpm(float clt) override;

	// PHASE DETERMINATION: what is the driver trying to do right now?
	Phase determinePhase(int rpm, int targetRpm, SensorResult tps, float vss, float crankingTaperFraction) override;
	float getCrankingTaperFraction() const override;

	// OPEN LOOP CORRECTIONS
	percent_t getCrankingOpenLoop(float clt) const override;
	percent_t getRunningOpenLoop(float clt, SensorResult tps) override;
	percent_t getOpenLoop(Phase phase, float clt, SensorResult tps, float crankingTaperFraction) override;

	float getIdleTimingAdjustment(int rpm) override;
	float getIdleTimingAdjustment(int rpm, int targetRpm, Phase phase);

	// CLOSED LOOP CORRECTION
	float getClosedLoop(IIdleController::Phase phase, float tpsPos, int rpm, int targetRpm) override;

	void onConfigurationChange(engine_configuration_s const * previousConfig) final;
	void onSlowCallback() final;

	// Allow querying state from outside
	bool isIdlingOrTaper() const override {
		return m_lastPhase == Phase::Idling || (engineConfiguration->useSeparateIdleTablesForCrankingTaper && m_lastPhase == Phase::CrankToIdleTaper);
	}

	PidIndustrial industrialWithOverrideIdlePid;

	#if EFI_IDLE_PID_CIC
	// Use PID with CIC integrator
		PidCic idleCicPid;
	#endif //EFI_IDLE_PID_CIC

	Pid * getIdlePid() {
	#if EFI_IDLE_PID_CIC
		if (engineConfiguration->useCicPidForIdle) {
			return &idleCicPid;
		}
	#endif /* EFI_IDLE_PID_CIC */
		return &industrialWithOverrideIdlePid;
	}


private:

	// These are stored by getIdlePosition() and used by getIdleTimingAdjustment()
	Phase m_lastPhase = Phase::Cranking;
	int m_lastTargetRpm = 0;
	efitimeus_t restoreAfterPidResetTimeUs = 0;

	// This is stored by getClosedLoop and used in case we want to "do nothing"
	float m_lastAutomaticPosition = 0;

	Pid m_timingPid;
};

percent_t getIdlePosition();

void applyIACposition(percent_t position);
void setManualIdleValvePosition(int positionPercent);

void startIdleThread();
void setDefaultIdleParameters();
void startIdleBench(void);
void setIdlePFactor(float value);
void setIdleIFactor(float value);
void setIdleDFactor(float value);
void setIdleMode(idle_mode_e value);
void setTargetIdleRpm(int value);
void startPedalPins();
void stopPedalPins();
