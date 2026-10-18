#include "pch.h"
#include "engine_state_machine.h"

void EngineStateMachine::setDefaultConfiguration() {
	engineConfiguration->useEngineStateMachine = false;
	engineConfiguration->stateBasedStft = false;
	engineConfiguration->smWotTpsThreshold = 90;
	engineConfiguration->smTransientHoldTimeMs = 200;
	engineConfiguration->smUpshiftClutchSwitch = sm_clutch_switch_e::None;
	engineConfiguration->smDownshiftClutchSwitch = sm_clutch_switch_e::None;
	engineConfiguration->smShiftDetectionMode = sm_shift_detection_mode_e::RpmRate;
	engineConfiguration->smSecondClutchSwitchAvailable = false;
	engineConfiguration->smRelatchOnClutchDown = false;
	engineConfiguration->smAccelRateThreshold = 0;
	engineConfiguration->smDecelRateThreshold = 0;
	engineConfiguration->smRpmRateWindowMs = 100;
	engineConfiguration->smShiftLatchTimeMs = 0;
	engineConfiguration->smShiftMinVss = 0;
	engineConfiguration->smAccumulatorUpshiftThreshold = 40;
	engineConfiguration->smAccumulatorDownshiftThreshold = 40;
	engineConfiguration->smAccelHoldMs = 0;
	engineConfiguration->smAccumulatorSnapTimeMs = 0;
	engineConfiguration->smAccumulatorGain = 20;
	engineConfiguration->smAccumulatorDecayRate = 10;
	engineConfiguration->smColdTempThreshold = 60;
	engineConfiguration->smHotTempThreshold = 100;
}

void EngineStateMachine::reset() {
	static_cast<engine_state_machine_state_s&>(*this) = {};
	engineSmEnabled = isEnabled();
	engineSmIsOff = true;
	m_currentState = EngineStateMachineState::Off;
	m_transientHoldTimer.init();
	m_accelHoldTimer.init();
	m_accelStateTimer.init();
	m_decelStateTimer.init();
	m_shiftWindowOpen = false;
	m_shiftIsUpshift = false;
	m_shiftWindowOpenMs = 0;
	m_prevClutchUp = false;
	m_prevClutchDown = false;
	m_clutchHasSample = false;
	m_shiftLatched = false;
	m_shiftLatchedIsUpshift = false;
	m_shiftLatchUntilMs = 0;
	m_shiftAccumulator = 0;
	m_accumulatorLastMs = 0;
	m_accumulatorLastRpm = 0;
	m_accumulatorLastVss = 0;
	m_accumulatorHasSample = false;
	m_lastRpmRate = 0;
	m_rpmRateAnchorMs = 0;
	m_rpmRateAnchorRpm = 0;
	m_rpmRateHasAnchor = false;
}

void EngineStateMachine::onEngineStop() {
	reset();
}

void EngineStateMachine::onConfigurationChange(engine_configuration_s const*) {
	reset();
}

bool EngineStateMachine::isEnabled() const {
	return engineConfiguration->useEngineStateMachine;
}

EngineStateMachineState EngineStateMachine::getCurrentState() const {
	return m_currentState;
}

void EngineStateMachine::onSlowCallback() {
	if (!isEnabled()) {
		reset();
		return;
	}

	// Do not carry timers, rate samples or shift windows through a stop/restart.
	bool running = engine->rpmCalculator.isRunning();
	if (!running) {
		reset();
	}
	engineSmEnabled = true;

	efitimems_t nowMs = getTimeNowMs();
	float rpm = engine->rpmCalculator.getCachedRpm();

	auto tpsSensor = Sensor::get(SensorType::DriverThrottleIntent);
	float tps = tpsSensor ? tpsSensor.Value : 0.0f;

	auto vssSensor = Sensor::get(SensorType::VehicleSpeed);
	float vss = vssSensor.value_or(0);
	bool vssValid = vssSensor.Valid;

	// Primary state determination (engine lifecycle, throttle position)
	EngineStateMachineState newState = determineState(rpm, tps, vss, vssValid);
	m_currentState = newState;

	uint8_t stateNum = static_cast<uint8_t>(newState);
	engineSmCurrentState = stateNum;

	engineSmIsOff          = (newState == EngineStateMachineState::Off);
	engineSmIsCranking     = (newState == EngineStateMachineState::Cranking);
	engineSmIsAfterStart   = (newState == EngineStateMachineState::Afterstart);
	engineSmIsIdle         = (newState == EngineStateMachineState::Idle);
	engineSmIsCoasting     = (newState == EngineStateMachineState::Coasting);
	engineSmIsTransient    = (newState == EngineStateMachineState::Transient);
	engineSmIsWot          = (newState == EngineStateMachineState::WOT);
	engineSmIsCruising     = (newState == EngineStateMachineState::Cruising);
	engineSmIsOverrun      = (newState == EngineStateMachineState::Overrun);
	engineSmIsAccelerating = (newState == EngineStateMachineState::Accelerating);
	engineSmIsDecelerating = (newState == EngineStateMachineState::Decelerating);

	if (!running) {
		updateTempOverlay();
		return;
	}

#if EFI_LAUNCH_CONTROL
	// Overlay: Launch Control — forward directly from LaunchControlBase
	engineSmIsLaunchControl = engine->launchController.isLaunchCondition;

	// Overlay: Torque Reduction + Shift direction
	// Flat-shift torque reduction is an authoritative upshift signal: the driver stayed WOT
	// throughout the shift, so direction is certain and no clutch analysis is needed.
	bool flatShiftActive = engineConfiguration->torqueReductionEnabled
		&& engine->shiftTorqueReductionController.isFlatShiftConditionSatisfied;
	engineSmIsTorqueReduction = flatShiftActive;
#else
	bool flatShiftActive = false;
#endif

	// RPM classification must continue without shift switches and during a flat shift.
	updateShiftAccumulator(rpm, vss, vssValid, nowMs);
	if (flatShiftActive) {
		engineSmIsUpshifting   = true;
		engineSmIsDownshifting = false;
		// Abort any open clutch-detection window/latch so it doesn't fight the flat-shift signal
		m_shiftWindowOpen = false;
		m_shiftLatched    = false;
		m_clutchHasSample = false;
	} else {
		updateShiftDetection(vss, vssValid, nowMs);
	}

	updateTempOverlay();
	// Display overlays do not change getCurrentState(), which returns the primary state.
	if (engineSmIsUpshifting) {
		engineSmCurrentState = static_cast<uint8_t>(EngineStateMachineState::Upshifting);
	} else if (engineSmIsDownshifting) {
		engineSmCurrentState = static_cast<uint8_t>(EngineStateMachineState::Downshifting);
	} else if (engineSmIsLaunchControl) {
		engineSmCurrentState = static_cast<uint8_t>(EngineStateMachineState::LaunchControl);
	}
}

void EngineStateMachine::updateTempOverlay() {
	auto clt = Sensor::get(SensorType::Clt);
	if (!clt) {
		// No valid CLT — treat as Operating so no false Cold/Hot trigger.
		engineSmIsCold      = false;
		engineSmIsOperating = true;
		engineSmIsHot       = false;
		return;
	}

	float temp = clt.Value;
	engineSmIsCold      = (temp < engineConfiguration->smColdTempThreshold);
	engineSmIsHot       = (temp > engineConfiguration->smHotTempThreshold);
	engineSmIsOperating = !engineSmIsCold && !engineSmIsHot;
}

EngineStateMachineState EngineStateMachine::determineState(float rpm, float tps, float vss, bool vssValid) {
	// Priority 1: engine not spinning
	if (engine->rpmCalculator.isStopped()) {
		return EngineStateMachineState::Off;
	}

	// Priority 2: engine below self-sustaining RPM
	if (engine->rpmCalculator.isCranking()) {
		return EngineStateMachineState::Cranking;
	}

	// Priority 3: wide open throttle
	if (tps > engineConfiguration->smWotTpsThreshold) {
		return EngineStateMachineState::WOT;
	}

	// Priority 4: AE-driven transient — active while AE threshold is met, then held
	//             for smTransientHoldTimeMs after it drops.
	{
		bool aeActive = engine->module<TpsAccelEnrichment>()->isAboveAccelThreshold ||
		                engine->module<TpsAccelEnrichment>()->isBelowDecelThreshold;
		if (aeActive) {
			m_transientHoldTimer.reset();
			return EngineStateMachineState::Transient;
		}
		if (!m_transientHoldTimer.hasElapsedMs(static_cast<float>(engineConfiguration->smTransientHoldTimeMs))) {
			return EngineStateMachineState::Transient;
		}
	}

#if EFI_IDLE_CONTROL
	auto idlePhase = engine->module<IdleController>()->getCurrentPhase();
#else
	auto idlePhase = IIdleController::Phase::Running;
#endif

	// Sustained acceleration/deceleration — based on the previous tick's RPM rate of change
	// (m_lastRpmRate, updated by updateShiftAccumulator later this callback; one slow-callback
	// lag is negligible at ~100 ms). Thresholds of 0 disable the respective state.
	// Skipped while the IdleController considers us in closed-loop idle territory: RPM rate
	// noise from idle hunting or AC/load compensation must not be misread as a driver-initiated
	// tip-in/tip-out.
	if (idlePhase != IIdleController::Phase::Idling) {
		int16_t accelThr = engineConfiguration->smAccelRateThreshold;
		int16_t decelThr = engineConfiguration->smDecelRateThreshold;

		if (accelThr > 0 && m_lastRpmRate > static_cast<float>(accelThr)) {
			m_accelHoldTimer.reset();
			return EngineStateMachineState::Accelerating;
		}

		// Hold Accelerating for smAccelHoldMs after the rate last crossed threshold, so a brief
		// dip below threshold doesn't cause chatter back to Cruising/Transient. A lift into
		// Coasting breaks the hold immediately -- a closed throttle physically contradicts a
		// stale "still accelerating" claim, and Overrun/DFCO must never be masked by it.
		uint16_t accelHoldMs = engineConfiguration->smAccelHoldMs;
		if (accelHoldMs > 0 && idlePhase != IIdleController::Phase::Coasting &&
				!m_accelHoldTimer.hasElapsedMs(static_cast<float>(accelHoldMs))) {
			return EngineStateMachineState::Accelerating;
		}

		// Decelerating: RPM dropping faster than threshold.
		// With VSS: only when transmission is disengaged (DetectedGear == 0).
		// Decelerating requires VSS to confirm the clutch is out (gear == 0).
		// Without VSS we cannot distinguish clutch-out decel from engine braking in gear,
		// so we fall through to let the idle-controller path classify it as Coasting/Overrun.
		if (decelThr > 0 && m_lastRpmRate < -static_cast<float>(decelThr)) {
			auto gearResult = Sensor::get(SensorType::DetectedGear);
			if (vssValid && gearResult.Valid && gearResult.Value == 0) {
				return EngineStateMachineState::Decelerating;
			}
		}
	}

	// Priorities 5–8: defer to IdleController — it is the single source of truth for the idle corner.
	// CrankToIdleTaper → Afterstart (taper table drives duration, CLT-indexed, same as idle control).
	// Idling → Idle; Coasting (elevated RPM) → Coasting or Overrun; Running → Coasting (still
	// rolling, off-throttle) or Cruising.
	{
		if (idlePhase == IIdleController::Phase::CrankToIdleTaper) {
			return EngineStateMachineState::Afterstart;
		}

		if (idlePhase == IIdleController::Phase::Idling) {
			return EngineStateMachineState::Idle;
		}

		if (idlePhase == IIdleController::Phase::Coasting) {
			bool tpsClosed = tps < engineConfiguration->coastingFuelCutTps;
			// Overrun means the transmission is actively spinning the engine (torque flowing
			// from the wheels) -- that requires the transmission engaged, per isTransmissionEngaged().
			if (tpsClosed && rpm > engineConfiguration->coastingFuelCutRpmHigh && isTransmissionEngaged()) {
				return EngineStateMachineState::Overrun;
			}
			return EngineStateMachineState::Coasting;
		}
	}

	// idlePhase is Running here. If the throttle is closed but the vehicle is still rolling
	// above maxIdleVss, the engine is unloaded (idling, producing no ground torque) even though
	// the car is moving -- that's Coasting, not Cruising, even though IdleController itself
	// bows out of closed-loop idle control at this speed.
	{
		uint8_t maxIdleVss = engineConfiguration->maxIdleVss;
		if (maxIdleVss != 0 && vss > static_cast<float>(maxIdleVss) &&
				tps < engineConfiguration->idlePidDeactivationTpsThreshold) {
			return EngineStateMachineState::Coasting;
		}
	}

	// Default: part-throttle cruising (IdleController phase is Running)
	return EngineStateMachineState::Cruising;
}

bool EngineStateMachine::isTransmissionEngaged() const {
	sm_clutch_switch_e upSw = engineConfiguration->smUpshiftClutchSwitch;
	sm_clutch_switch_e dnSw = engineConfiguration->smDownshiftClutchSwitch;
	bool secondSwitch = engineConfiguration->smSecondClutchSwitchAvailable
		&& (upSw != sm_clutch_switch_e::None || dnSw != sm_clutch_switch_e::None);
	// Physical inputs work independently of shift-direction selection. Selections
	// also describe virtual (Lua) switches that have no hardware pin assigned.
	bool hasClutchDown = isBrainPinValid(engineConfiguration->clutchDownPin) || secondSwitch
		|| upSw == sm_clutch_switch_e::ClutchDown || dnSw == sm_clutch_switch_e::ClutchDown;
	bool hasClutchUp = isBrainPinValid(engineConfiguration->clutchUpPin) || secondSwitch
		|| upSw == sm_clutch_switch_e::ClutchUp || dnSw == sm_clutch_switch_e::ClutchUp;

	// Clutch-Down reads true when the pedal is pressed (clutch disengaged).
	if (hasClutchDown && engine->engineState.clutchDownState) {
		return false;
	}
	// Clutch-Up reads true when the pedal is at rest (clutch engaged).
	if (hasClutchUp && engine->engineState.clutchUpState == 0) {
		return false;
	}

	return true;
}

float EngineStateMachine::recordRpmSampleAndComputeRate(float rpm, efitimems_t nowMs) {
	if (!m_rpmRateHasAnchor) {
		m_rpmRateAnchorMs  = nowMs;
		m_rpmRateAnchorRpm = rpm;
		m_rpmRateHasAnchor = true;
		return 0; // just started, no reference sample yet
	}

	// Floor the window to one slow-callback tick: a window shorter than the sampling cadence
	// carries no additional information, and this also makes a fresh/reset smRpmRateWindowMs
	// == 0 (e.g. from an older tune with no saved value) behave identically to the
	// pre-existing raw-tick calculation.
	uint16_t windowMs = engineConfiguration->smRpmRateWindowMs;
	if (windowMs < SLOW_CALLBACK_PERIOD_MS) {
		windowMs = SLOW_CALLBACK_PERIOD_MS;
	}

	efitimems_t elapsedMs = nowMs - m_rpmRateAnchorMs;
	if (elapsedMs < static_cast<efitimems_t>(windowMs)) {
		// Window hasn't elapsed yet -- hold the last computed rate rather than recomputing
		// against a partial window, so the value only actually changes at the cadence the user
		// configured.
		return m_lastRpmRate;
	}

	float rate = (rpm - m_rpmRateAnchorRpm) / (elapsedMs / 1000.0f);
	m_rpmRateAnchorMs  = nowMs;
	m_rpmRateAnchorRpm = rpm;
	return rate;
}

void EngineStateMachine::updateShiftAccumulator(float rpm, float vss, bool vssValid, efitimems_t nowMs) {
	efitimems_t dtMs = m_accumulatorHasSample ? nowMs - m_accumulatorLastMs : 0;
	m_accumulatorHasSample = true;
	float prevVss = m_accumulatorLastVss;
	m_accumulatorLastMs  = nowMs;
	m_accumulatorLastRpm = rpm;
	m_accumulatorLastVss = vss;

	// Always compute RPM rate for state detection (used by determineState next tick), over the
	// user-configurable window (smRpmRateWindowMs, "RPM/t") rather than a raw single tick.
	m_lastRpmRate = recordRpmSampleAndComputeRate(rpm, nowMs);
	engineSmRpmRate = m_lastRpmRate; // live-data visibility

	// Missing VSS is not a sudden stop. The first recovered sample starts a new interval.
	bool missingVss = engineConfiguration->smShiftDetectionMode == sm_shift_detection_mode_e::VssRate && !vssValid;
	bool noShiftSwitches = engineConfiguration->smUpshiftClutchSwitch == sm_clutch_switch_e::None
		&& engineConfiguration->smDownshiftClutchSwitch == sm_clutch_switch_e::None;
	if (missingVss || noShiftSwitches) {
		m_shiftAccumulator = 0;
		engineSmShiftAccumulator = 0;
		m_accumulatorHasSample = false;
		return;
	}

	// Accumulator is frozen while the shift window is open (clutch out).
	// The first sample establishes the anchor; same-time callbacks add no evidence.
	if (m_shiftWindowOpen || dtMs == 0) {
		engineSmShiftAccumulator = m_shiftAccumulator;
		return;
	}

	// Dwell tracking for the smAccumulatorSnapTimeMs override below: reset the instant we leave
	// the respective state(s) so the timer measures time since the most recent entry.
	if (m_currentState != EngineStateMachineState::Accelerating) {
		m_accelStateTimer.reset();
	}
	if (m_currentState != EngineStateMachineState::Decelerating &&
			m_currentState != EngineStateMachineState::Overrun) {
		m_decelStateTimer.reset();
	}

	float dt = dtMs / 1000.0f;
	float gain      = engineConfiguration->smAccumulatorGain;
	float decayRate = engineConfiguration->smAccumulatorDecayRate;

	auto decay = [&]() {
		float d = decayRate * dt;
		if (m_shiftAccumulator > 0) {
			m_shiftAccumulator -= d;
			if (m_shiftAccumulator < 0) { m_shiftAccumulator = 0; }
		} else if (m_shiftAccumulator < 0) {
			m_shiftAccumulator += d;
			if (m_shiftAccumulator > 0) { m_shiftAccumulator = 0; }
		}
	};

	if (m_currentState == EngineStateMachineState::Idle ||
	    engineSmIsLaunchControl || engineSmIsTorqueReduction) {
		// Idle oscillations and launch-RPM hold are not shift evidence — decay only.
		decay();
		engineSmShiftAccumulator = m_shiftAccumulator;
		return;
	}

	// Time-based override: a same-gear direction reversal (e.g. a long Overrun run immediately
	// followed by acceleration) would otherwise need an equally long run the other way just to
	// null out a large stale opposite-sign accumulator, risking a stale shift-direction call
	// (e.g. a spurious downshift blip on clutch-in shortly after tipping back into the
	// throttle). Once we've dwelled continuously in the state long enough, force the accumulator
	// to 0 so it starts accumulating fresh evidence for the new direction below instead of
	// waiting it out. smAccumulatorSnapTimeMs == 0 disables this (legacy decay/cross behavior).
	uint16_t snapTimeMs = engineConfiguration->smAccumulatorSnapTimeMs;
	if (snapTimeMs > 0) {
		float snapTimeMsF = static_cast<float>(snapTimeMs);
		if (m_currentState == EngineStateMachineState::Accelerating && m_shiftAccumulator < 0 &&
				m_accelStateTimer.hasElapsedMs(snapTimeMsF)) {
			m_shiftAccumulator = 0;
		} else if ((m_currentState == EngineStateMachineState::Decelerating ||
				m_currentState == EngineStateMachineState::Overrun) && m_shiftAccumulator > 0 &&
				m_decelStateTimer.hasElapsedMs(snapTimeMsF)) {
			m_shiftAccumulator = 0;
		}
	}

	// Accumulator integrates only in Accelerating (positive) and Overrun (negative).
	// The accumulator signal source for shift detection follows smShiftDetectionMode;
	// m_lastRpmRate above is always RPM-based regardless of mode.
	sm_shift_detection_mode_e mode = engineConfiguration->smShiftDetectionMode;
	float signal;
	if (mode == sm_shift_detection_mode_e::VssRate) {
		if (engineConfiguration->smShiftMinVss > 0 && vss < (float)engineConfiguration->smShiftMinVss) {
			decay();
			engineSmShiftAccumulator = m_shiftAccumulator;
			return;
		}
		signal = (vss - prevVss) / dt;
	} else {
		signal = m_lastRpmRate;
	}

	float accelThr = (float)engineConfiguration->smAccelRateThreshold;
	float decelThr = (float)engineConfiguration->smDecelRateThreshold;
	if (accelThr <= 0) { accelThr = 1.0f; }
	if (decelThr <= 0) { decelThr = 1.0f; }

	if (mode == sm_shift_detection_mode_e::VssRate) {
		// VSS-rate signal: integrate directly on threshold crossing — no RPM-based state gate,
		// since state (Accelerating/Overrun) is derived from RPM, not VSS.
		if (signal > 0) {
			m_shiftAccumulator += (signal / accelThr) * gain * dt;
		} else if (signal < 0) {
			m_shiftAccumulator += (signal / decelThr) * gain * dt;
		} else {
			decay();
		}
	} else {
		// RpmRate: upshift integrates only in Accelerating state (rate-gated, immune to idle
		// oscillations and launch hold). Downshift integrates whenever RPM is falling — Idle and
		// LaunchControl are already excluded above, so any other state with negative rate means
		// the driver is decelerating toward a downshift.
		if (m_currentState == EngineStateMachineState::Accelerating && signal > 0) {
			m_shiftAccumulator += (signal / accelThr) * gain * dt;
		} else if (signal < 0) {
			m_shiftAccumulator += (signal / decelThr) * gain * dt;
		} else {
			decay();
		}
	}

	m_shiftAccumulator = clampF(-100.0f, m_shiftAccumulator, 100.0f);
	engineSmShiftAccumulator = m_shiftAccumulator;
}

void EngineStateMachine::updateShiftDetection(float vss, bool vssValid, efitimems_t nowMs) {
	// VSS Rate mode requires a valid sample — disable shift detection
	// entirely in that mode when VSS is unavailable. RPM Rate mode can work without
	// VSS since it only needs engine RPM, so it falls through normally.
	if (!vssValid && engineConfiguration->smShiftDetectionMode == sm_shift_detection_mode_e::VssRate) {
		m_shiftWindowOpen = false;
		m_clutchHasSample = false;
		engineSmIsUpshifting   = false;
		engineSmIsDownshifting = false;
		m_shiftLatched         = false;
		return;
	}

	// Read clutch state maintained by the engine (clutchUpState via SwitchedState,
	// clutchDownState via engine.cpp getClutchDownState)
	bool clutchUpActive   = engine->engineState.clutchUpState  != 0;
	bool clutchDownActive = engine->engineState.clutchDownState;

	sm_clutch_switch_e upSw = engineConfiguration->smUpshiftClutchSwitch;
	sm_clutch_switch_e dnSw = engineConfiguration->smDownshiftClutchSwitch;
	bool secondSwitch = engineConfiguration->smSecondClutchSwitchAvailable;

	// No direction configured → disable shift detection
	if (upSw == sm_clutch_switch_e::None && dnSw == sm_clutch_switch_e::None) {
		m_shiftWindowOpen = false;
		m_clutchHasSample = false;
		engineSmIsUpshifting   = false;
		engineSmIsDownshifting = false;
		m_shiftLatched         = false;
		return;
	}

	// A reset or a gap in detection must not turn a held pedal into a new press.
	// Establish the baseline from the first usable sample, after switch inputs update.
	if (!m_clutchHasSample) {
		m_prevClutchUp = clutchUpActive;
		m_prevClutchDown = clutchDownActive;
		m_clutchHasSample = true;
	}

	// Raw physical edges, independent of which direction (if any) a switch is assigned to, so a
	// switch can be consulted even when it isn't the one a given direction is configured with.
	// Clutch-Up reads true with the pedal at rest, so its FALLING edge is the pedal leaving rest
	// (clutch starting to disengage, drivetrain still locked) and its RISING edge is the pedal
	// fully back at rest (clutch re-engaged). Clutch-Down is the inverse: RISING edge is full
	// press (clutch fully disengaged), FALLING edge is release starting.
	bool rawUpFell = !clutchUpActive   && m_prevClutchUp;
	bool rawUpRose =  clutchUpActive   && !m_prevClutchUp;
	bool rawDnFell = !clutchDownActive && m_prevClutchDown;
	bool rawDnRose =  clutchDownActive && !m_prevClutchDown;

	m_prevClutchUp   = clutchUpActive;
	m_prevClutchDown = clutchDownActive;

	// A switch type is usable for open/close timing if some direction is configured to use it,
	// or the operator has told us a second switch is wired alongside whichever one is selected.
	bool upWired = (upSw == sm_clutch_switch_e::ClutchUp)   || (dnSw == sm_clutch_switch_e::ClutchUp)   || secondSwitch;
	bool dnWired = (upSw == sm_clutch_switch_e::ClutchDown) || (dnSw == sm_clutch_switch_e::ClutchDown) || secondSwitch;

	auto openFired  = [&](sm_clutch_switch_e sw) {
		if (sw == sm_clutch_switch_e::ClutchUp)   { return rawUpFell; }
		if (sw == sm_clutch_switch_e::ClutchDown) { return rawDnRose; }
		return false;
	};
	auto closeFired = [&](sm_clutch_switch_e sw) {
		if (sw == sm_clutch_switch_e::ClutchUp)   { return rawUpRose; }
		if (sw == sm_clutch_switch_e::ClutchDown) { return rawDnFell; }
		return false;
	};

	// Open the shift detection window on the earliest available "clutch starting to disengage"
	// edge among the switch(es) actually wired.
	bool upTypeOpens = upWired && rawUpFell;
	bool dnTypeOpens = dnWired && rawDnRose;

	if (!m_shiftWindowOpen && (upTypeOpens || dnTypeOpens)) {
		m_shiftWindowOpen   = true;
		m_shiftWindowOpenMs = nowMs;

		if (upSw != sm_clutch_switch_e::None && dnSw != sm_clutch_switch_e::None) {
			if (upSw == dnSw) {
				// Both directions share the same physical switch. Read direction from the
				// pre-shift accumulator: positive = upshift context, negative = downshift context.
				// evaluateShiftDirection() will confirm or deny against the thresholds.
				m_shiftIsUpshift = (m_shiftAccumulator >= 0);
			} else {
				// Different switch types assigned per direction: whichever type's open edge
				// actually fired identifies the direction.
				m_shiftIsUpshift = openFired(upSw);
			}
		} else {
			// Only one direction configured at all.
			m_shiftIsUpshift = (upSw != sm_clutch_switch_e::None);
		}
	}

	// Relatch-on-clutch-down: while the latch is active, a fresh Clutch-Down full-press resets
	// it back to the full duration instead of letting it expire, independent of window state.
	// Requires a second switch to be wired (rawDnRose is meaningless without one).
	if (m_shiftLatched && secondSwitch && engineConfiguration->smRelatchOnClutchDown && rawDnRose) {
		m_shiftLatchUntilMs = nowMs + engineConfiguration->smShiftLatchTimeMs;
	}

	bool windowConfirmed = false;

	if (m_shiftWindowOpen) {
		// Close the window when the clutch re-engages, or on timeout. With a second switch wired,
		// Clutch-Down's falling edge (release starting) is an earlier, equally valid "re-engaging"
		// signal than waiting for Clutch-Up to fully return to rest.
		sm_clutch_switch_e dirSwitch = m_shiftIsUpshift ? upSw : dnSw;
		bool ownClose   = closeFired(dirSwitch);
		bool earlyClose = secondSwitch && dirSwitch == sm_clutch_switch_e::ClutchUp && rawDnFell;
		bool triggerFell    = ownClose || earlyClose;
		efitimems_t elapsed = nowMs - m_shiftWindowOpenMs;

		if (triggerFell || elapsed > SM_SHIFT_TIMEOUT_MS) {
			// Window closes; reset the accumulator for the next shift.
			// Any active latch keeps running independently (see below).
			m_shiftWindowOpen  = false;
			m_shiftAccumulator = 0;
			engineSmShiftAccumulator = 0;
			m_accumulatorLastMs = nowMs;
		} else {
			// Window active: evaluate direction from the (now frozen) accumulator.
			windowConfirmed = evaluateShiftDirection(m_shiftIsUpshift, vss);
		}
	}

	// Latch: arm fresh on the first confirmation, then hold for smShiftLatchTimeMs without
	// being extended by further confirmations (a true "minimum hold", not an extending one) --
	// the only thing allowed to push the deadline back out is the relatch-on-clutch-down logic
	// above. This masks rate-check flicker without making the overlay outlive a long latch
	// configured shorter than the actual shift.
	if (windowConfirmed) {
		if (!m_shiftLatched) {
			m_shiftLatchUntilMs = nowMs + engineConfiguration->smShiftLatchTimeMs;
		}
		m_shiftLatched          = true;
		m_shiftLatchedIsUpshift = m_shiftIsUpshift;
	} else if (m_shiftLatched && nowMs >= m_shiftLatchUntilMs) {
		m_shiftLatched = false;
	}

	bool active        = windowConfirmed || m_shiftLatched;
	bool activeIsUpshift = windowConfirmed ? m_shiftIsUpshift : m_shiftLatchedIsUpshift;
	engineSmIsUpshifting   = active &&  activeIsUpshift;
	engineSmIsDownshifting = active && !activeIsUpshift;

	// Clutch fully up (pedal at rest) unconditionally ends any shift overlay, overriding the
	// latch/relatch hold. The blipper and RPM-hold modules trust these flags with no clutch
	// check of their own, so this is the only place that can guarantee neither is ever active
	// while the pedal is at rest.
	if (clutchUpActive) {
		bool wasShifting = m_shiftWindowOpen || m_shiftLatched;
		engineSmIsUpshifting   = false;
		engineSmIsDownshifting = false;
		m_shiftWindowOpen      = false;
		m_shiftLatched         = false;
		// Only reset the accumulator when we're actually ending a shift, not on every
		// pedal-at-rest tick (which would prevent the accumulator from building up at all).
		if (wasShifting) {
			m_shiftAccumulator       = 0;
			engineSmShiftAccumulator = 0;
			m_accumulatorLastMs      = nowMs;
		}
	}
}

bool EngineStateMachine::evaluateShiftDirection(bool isUpshift, float currentVss) {
	sm_clutch_switch_e upSw = engineConfiguration->smUpshiftClutchSwitch;
	sm_clutch_switch_e dnSw = engineConfiguration->smDownshiftClutchSwitch;

	// Single direction configured — the switch is authoritative, no accumulator confirmation needed.
	if (upSw == sm_clutch_switch_e::None || dnSw == sm_clutch_switch_e::None) {
		return true;
	}

	// VSS mode: belt-and-suspenders speed gate (accumulator also decays below this threshold).
	if (engineConfiguration->smShiftDetectionMode == sm_shift_detection_mode_e::VssRate &&
	    engineConfiguration->smShiftMinVss > 0 &&
	    currentVss < (float)engineConfiguration->smShiftMinVss) {
		return false;
	}

	// Both directions configured: confirm against the pre-shift accumulator.
	float upThr = (float)engineConfiguration->smAccumulatorUpshiftThreshold;
	float dnThr = (float)engineConfiguration->smAccumulatorDownshiftThreshold;

	if (isUpshift) {
		return m_shiftAccumulator >= upThr;
	} else {
		return m_shiftAccumulator <= -dnThr;
	}
}
