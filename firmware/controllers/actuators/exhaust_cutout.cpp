#include "pch.h"

#if EFI_EXHAUST_CUTOUT

#include "exhaust_cutout.h"

static constexpr float VBATT_KEY_ON_TEST_THRESHOLD = 10.0f;

void ExhaustCutoutController::deEnergize() {
	m_pwmOutput.setSimplePwmDutyCycle(0);
	m_hbridgePwm.setSimplePwmDutyCycle(0);
}

void ExhaustCutoutController::initPins() {
	m_outputPin.deInit();
	m_hbridgePin2.deInit();
	m_ledPin.deInit();

	auto& cfg = *engineConfiguration;

	if (!cfg.exhaustCutoutEnabled) {
		return;
	}

	bool isHBridge = cfg.exhaustCutoutOutputMode == EXHAUST_CUTOUT_OUTPUT_HBRIDGE;
	bool isPwm = cfg.exhaustCutoutOutputMode == EXHAUST_CUTOUT_OUTPUT_PWM;

	if (isHBridge) {
		float freq = cfg.exhaustCutoutHBridgePwmFrequency > 0 ? cfg.exhaustCutoutHBridgePwmFrequency : 1000.0f;
		if (isBrainPinValid(cfg.exhaustCutoutOutputPin)) {
			m_outputPin.initPin("cutout-in1", cfg.exhaustCutoutOutputPin, cfg.exhaustCutoutOutputPinMode);
			startSimplePwm(&m_pwmOutput, "cutout-in1", &engine->scheduler, &m_outputPin, freq, 0);
		}
		if (isBrainPinValid(cfg.exhaustCutoutHBridgePin2)) {
			m_hbridgePin2.initPin("cutout-in2", cfg.exhaustCutoutHBridgePin2, cfg.exhaustCutoutHBridgePin2Mode);
			startSimplePwm(&m_hbridgePwm, "cutout-in2", &engine->scheduler, &m_hbridgePin2, freq, 0);
		}
	} else if (isBrainPinValid(cfg.exhaustCutoutOutputPin)) {
		m_outputPin.initPin("cutout", cfg.exhaustCutoutOutputPin, cfg.exhaustCutoutOutputPinMode);

		if (isPwm) {
			float freq = cfg.exhaustCutoutPwmFrequency > 0 ? cfg.exhaustCutoutPwmFrequency : 100.0f;
			startSimplePwm(&m_pwmOutput, "cutout",
				&engine->scheduler,
				&m_outputPin,
				freq,
				PERCENT_TO_DUTY(cfg.exhaustCutoutPwmClosedDuty));
		}
	}

	if (isBrainPinValid(cfg.exhaustCutoutLedPin)) {
		m_ledPin.initPin("cutout led", cfg.exhaustCutoutLedPin, cfg.exhaustCutoutLedPinMode);
	}

	m_tpsHoldTimer.reset();
	m_state = ExhaustCutoutState::CLOSED;

	// Safe initial output state
	if (isHBridge) {
		deEnergize();
	} else if (isPwm) {
		m_pwmOutput.setSimplePwmDutyCycle(PERCENT_TO_DUTY(cfg.exhaustCutoutPwmClosedDuty));
	} else {
		m_outputPin.setValue(cfg.exhaustCutoutInvertedOutput ? 1 : 0);
	}

	m_ledPin.setValue(0);

	// Arm key-on test; actual start is deferred until 12V is present (onSlowCallback)
	if (cfg.exhaustCutoutKeyOnTestEnabled && !m_bootTestDone) {
		m_keyOnTestPending = true;
	}
}

void ExhaustCutoutController::onConfigurationChange(engine_configuration_s const* /*previousConfig*/) {
	initPins();
}

bool ExhaustCutoutController::getInputHigh() const {
	auto& cfg = *engineConfiguration;

	if (cfg.exhaustCutoutActivationMode == EXHAUST_CUTOUT_SWITCH) {
#if !EFI_SIMULATOR
		if (isBrainPinValid(cfg.exhaustCutoutSwitchPin)) {
			return efiReadPin(cfg.exhaustCutoutSwitchPin, cfg.exhaustCutoutSwitchPinMode);
		}
#endif
		return false;
	}

	if (cfg.exhaustCutoutActivationMode == EXHAUST_CUTOUT_LUA_GAUGE) {
		SensorType gauge;
		switch (cfg.exhaustCutoutLuaGauge) {
			case LUA_GAUGE_2: gauge = SensorType::LuaGauge2; break;
			case LUA_GAUGE_3: gauge = SensorType::LuaGauge3; break;
			case LUA_GAUGE_4: gauge = SensorType::LuaGauge4; break;
			case LUA_GAUGE_5: gauge = SensorType::LuaGauge5; break;
			case LUA_GAUGE_6: gauge = SensorType::LuaGauge6; break;
			case LUA_GAUGE_7: gauge = SensorType::LuaGauge7; break;
			case LUA_GAUGE_8: gauge = SensorType::LuaGauge8; break;
			default:          gauge = SensorType::LuaGauge1; break;
		}
		auto result = Sensor::get(gauge);
		if (!result.Valid) {
			return false;
		}
		if (cfg.exhaustCutoutLuaGaugeMeaning == LUA_GAUGE_LOWER_BOUND) {
			return result.Value >= cfg.exhaustCutoutLuaGaugeThreshold;
		} else {
			return result.Value <= cfg.exhaustCutoutLuaGaugeThreshold;
		}
	}

	return false;
}

bool ExhaustCutoutController::evaluateAutoTrigger() {
	auto& cfg = *engineConfiguration;

	float rpm = Sensor::getOrZero(SensorType::Rpm);
	isTriggerRpm = (cfg.exhaustCutoutOpenRpm > 0) && (rpm > cfg.exhaustCutoutOpenRpm);

	float tps = Sensor::getOrZero(SensorType::Tps1);
	if (cfg.exhaustCutoutOpenTps > 0 && tps > cfg.exhaustCutoutOpenTps) {
		isTriggerTps = m_tpsHoldTimer.hasElapsedSec(cfg.exhaustCutoutTpsDelayS);
	} else {
		m_tpsHoldTimer.reset();
		isTriggerTps = false;
	}

	float map = Sensor::getOrZero(SensorType::MapSlow);
	isTriggerMap = (cfg.exhaustCutoutOpenMapKpa > 0) && (map > cfg.exhaustCutoutOpenMapKpa);

	bool shouldBeOpen = isTriggerRpm || isTriggerTps || isTriggerMap;

	if (shouldBeOpen) {
		m_closeDelayTimer.reset();
		return true;
	} else {
		return !m_closeDelayTimer.hasElapsedSec(cfg.exhaustCutoutClosingDelayS);
	}
}

void ExhaustCutoutController::setState(ExhaustCutoutState newState) {
	if (m_state == newState) {
		return;
	}

	m_state = newState;
	m_moveTimer.reset();
	m_ledTimer.reset();

	auto& cfg = *engineConfiguration;
	bool isHBridge = cfg.exhaustCutoutOutputMode == EXHAUST_CUTOUT_OUTPUT_HBRIDGE;
	bool isPwm = cfg.exhaustCutoutOutputMode == EXHAUST_CUTOUT_OUTPUT_PWM;

	switch (newState) {
		case ExhaustCutoutState::CLOSED:
			if (isHBridge) {
				deEnergize();
			} else if (isPwm) {
				m_pwmOutput.setSimplePwmDutyCycle(PERCENT_TO_DUTY(cfg.exhaustCutoutPwmClosedDuty));
			} else {
				m_outputPin.setValue(cfg.exhaustCutoutInvertedOutput ? 1 : 0);
			}
			m_ledPin.setValue(0);
			break;
		case ExhaustCutoutState::OPENING:
			if (isHBridge) {
				// IN1 = drive duty, IN2 = 0 (open direction)
				m_hbridgePwm.setSimplePwmDutyCycle(0);
				m_pwmOutput.setSimplePwmDutyCycle(PERCENT_TO_DUTY(cfg.exhaustCutoutHBridgeDutyCycle));
			} else if (isPwm) {
				m_pwmOutput.setSimplePwmDutyCycle(PERCENT_TO_DUTY(cfg.exhaustCutoutPwmOpenDuty));
			} else {
				m_outputPin.setValue(cfg.exhaustCutoutInvertedOutput ? 0 : 1);
			}
			break;
		case ExhaustCutoutState::OPEN:
			if (isHBridge) {
				deEnergize();
			}
			// PWM: duty stays at open level set during OPENING
			// Digital: pin stays at open logic level set during OPENING
			break;
		case ExhaustCutoutState::CLOSING:
			if (isHBridge) {
				// IN1 = 0, IN2 = drive duty (close direction)
				m_pwmOutput.setSimplePwmDutyCycle(0);
				m_hbridgePwm.setSimplePwmDutyCycle(PERCENT_TO_DUTY(cfg.exhaustCutoutHBridgeDutyCycle));
			} else if (isPwm) {
				m_pwmOutput.setSimplePwmDutyCycle(PERCENT_TO_DUTY(cfg.exhaustCutoutPwmClosedDuty));
			} else {
				m_outputPin.setValue(cfg.exhaustCutoutInvertedOutput ? 1 : 0);
			}
			break;
	}
}

void ExhaustCutoutController::updateLed() {
	switch (m_state) {
		case ExhaustCutoutState::CLOSED:
			m_ledPin.setValue(0);
			break;
		case ExhaustCutoutState::OPENING: {
			float phase = fmod(m_ledTimer.getElapsedSeconds() * 3.0f, 1.0f);
			m_ledPin.setValue(phase < 0.5f ? 1 : 0);
			break;
		}
		case ExhaustCutoutState::OPEN:
			m_ledPin.setValue(engineConfiguration->exhaustCutoutShowOpenState ? 1 : 0);
			break;
		case ExhaustCutoutState::CLOSING: {
			float phase = fmod(m_ledTimer.getElapsedSeconds() * 1.0f, 1.0f);
			m_ledPin.setValue(phase < 0.5f ? 1 : 0);
			break;
		}
	}
}

// Returns the desired open state for the current test step, advancing phase when timer expires.
bool ExhaustCutoutController::runTestSequence() {
	auto& cfg = *engineConfiguration;
	float stepDuration = cfg.exhaustCutoutMoveDurationS;
	bool stepDone = m_testStepTimer.hasElapsedSec(stepDuration);

	auto advance = [&](ExhaustCutoutTestPhase next) {
		m_testPhase = next;
		m_testStepTimer.reset();
	};

	switch (m_testPhase) {
		case ExhaustCutoutTestPhase::KEY_ON_OPEN_1:
			if (stepDone) advance(ExhaustCutoutTestPhase::KEY_ON_CLOSE_1);
			return true;
		case ExhaustCutoutTestPhase::KEY_ON_CLOSE_1:
			if (stepDone) advance(ExhaustCutoutTestPhase::KEY_ON_OPEN_2);
			return false;
		case ExhaustCutoutTestPhase::KEY_ON_OPEN_2:
			if (stepDone) advance(ExhaustCutoutTestPhase::KEY_ON_CLOSE_2);
			return true;
		case ExhaustCutoutTestPhase::KEY_ON_CLOSE_2:
			if (stepDone) {
				m_testPhase = ExhaustCutoutTestPhase::IDLE;
				m_bootTestDone = true;
				m_closeDelayTimer.reset();
				if (cfg.exhaustCutoutEngineOnTestEnabled) {
					m_engineOnTestArmed = true;
				}
			}
			return false;
		case ExhaustCutoutTestPhase::ENGINE_ON_OPEN:
			if (stepDone) advance(ExhaustCutoutTestPhase::ENGINE_ON_CLOSE);
			return true;
		case ExhaustCutoutTestPhase::ENGINE_ON_CLOSE:
			if (stepDone) {
				m_testPhase = ExhaustCutoutTestPhase::IDLE;
				m_closeDelayTimer.reset();
			}
			return false;
		default:
			return false;
	}
}

void ExhaustCutoutController::onSlowCallback() {
	auto& cfg = *engineConfiguration;

	if (!cfg.exhaustCutoutEnabled || cfg.exhaustCutoutActivationMode == EXHAUST_CUTOUT_OFF) {
		if (m_state != ExhaustCutoutState::CLOSED) {
			if (cfg.exhaustCutoutOutputMode == EXHAUST_CUTOUT_OUTPUT_HBRIDGE) {
				deEnergize();
			} else if (cfg.exhaustCutoutOutputMode == EXHAUST_CUTOUT_OUTPUT_PWM) {
				m_pwmOutput.setSimplePwmDutyCycle(PERCENT_TO_DUTY(cfg.exhaustCutoutPwmClosedDuty));
			} else {
				m_outputPin.setValue(cfg.exhaustCutoutInvertedOutput ? 1 : 0);
			}
			m_ledPin.setValue(0);
			m_state = ExhaustCutoutState::CLOSED;
		}
		isTriggerRpm = false;
		isTriggerTps = false;
		isTriggerMap = false;
		isInputHigh = false;
		targetOpen = false;
		isCutoutOpen = false;
		isCutoutMoving = false;
		isTestActive = false;
		m_keyOnTestPending = false;
		return;
	}

	float vbatt = Sensor::getOrZero(SensorType::BatteryVoltage);

	// Start key-on test once 12V is present (deferred from initPins)
	if (m_keyOnTestPending && vbatt >= VBATT_KEY_ON_TEST_THRESHOLD) {
		m_keyOnTestPending = false;
		m_testPhase = ExhaustCutoutTestPhase::KEY_ON_OPEN_1;
		m_testStepTimer.reset();
	}

	// Engine-on test: fire on first transition from cranking to running
	bool engineRunning = engine->rpmCalculator.isRunning();
	if (engineRunning && !m_lastEngineRunning && m_engineOnTestArmed) {
		m_testPhase = ExhaustCutoutTestPhase::ENGINE_ON_OPEN;
		m_testStepTimer.reset();
		m_engineOnTestArmed = false;
	}
	m_lastEngineRunning = engineRunning;

	isTestActive = (m_testPhase != ExhaustCutoutTestPhase::IDLE);
	bool desiredOpen;

	if (isTestActive) {
		desiredOpen = runTestSequence();
		isTriggerRpm = false;
		isTriggerTps = false;
		isTriggerMap = false;
		isInputHigh = false;
	} else {
		// Normal activation logic
		isInputHigh = getInputHigh();

		exhaust_cutout_behavior_e behavior = isInputHigh ? cfg.exhaustCutoutBehaviorHigh
		                                                  : cfg.exhaustCutoutBehaviorLow;

		if (behavior == EXHAUST_CUTOUT_ALWAYS_CLOSED) {
			desiredOpen = false;
			isTriggerRpm = false;
			isTriggerTps = false;
			isTriggerMap = false;
			m_tpsHoldTimer.reset();
		} else if (behavior == EXHAUST_CUTOUT_ALWAYS_OPEN) {
			desiredOpen = true;
			isTriggerRpm = false;
			isTriggerTps = false;
			isTriggerMap = false;
		} else {
			desiredOpen = evaluateAutoTrigger();
		}
	}

	targetOpen = desiredOpen;

	// H-Bridge: advance from OPENING to OPEN / CLOSING to CLOSED after motor duration
	float moveDuration = cfg.exhaustCutoutMoveDurationS;
	bool moveComplete = m_moveTimer.hasElapsedSec(moveDuration);

	if (m_state == ExhaustCutoutState::OPENING && moveComplete) {
		setState(ExhaustCutoutState::OPEN);
	} else if (m_state == ExhaustCutoutState::CLOSING && moveComplete) {
		setState(ExhaustCutoutState::CLOSED);
	}

	// State machine transitions
	if (desiredOpen) {
		if (m_state == ExhaustCutoutState::CLOSED || m_state == ExhaustCutoutState::CLOSING) {
			setState(ExhaustCutoutState::OPENING);
		}
	} else {
		if (m_state == ExhaustCutoutState::OPEN || m_state == ExhaustCutoutState::OPENING) {
			setState(ExhaustCutoutState::CLOSING);
		}
	}

	updateLed();

	isCutoutOpen = (m_state == ExhaustCutoutState::OPEN || m_state == ExhaustCutoutState::OPENING);
	isCutoutMoving = (m_state == ExhaustCutoutState::OPENING || m_state == ExhaustCutoutState::CLOSING);
}

void initExhaustCutout() {
	engine->module<ExhaustCutoutController>()->initPins();
}

#endif // EFI_EXHAUST_CUTOUT
