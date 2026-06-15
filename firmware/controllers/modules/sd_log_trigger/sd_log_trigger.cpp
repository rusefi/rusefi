/*
 * @file sd_log_trigger.cpp
 *
 * See sd_log_trigger.h and test_sd_log_trigger.cpp
 */

#include "pch.h"
#include "sd_log_trigger.h"

// button debounce window
#define SD_LOG_BUTTON_DEBOUNCE_MS 100

void SdLogTrigger::onConfigurationChange(engine_configuration_s const* /*previousConfig*/) {
	// Register the toggle button so its pin is configured through efiSetPadMode by
	// ButtonDebounce::startConfigurationList(), which reports a pin conflict
	// (criticalError "Pin X required by ... but is used by Y") if the pin is
	// already used by another feature - same protection as the start/stop button.
	if (!m_buttonInited) {
		m_button.init(SD_LOG_BUTTON_DEBOUNCE_MS, engineConfiguration->sdLogTriggerPin,
			engineConfiguration->sdLogTriggerPinMode);
		m_buttonInited = true;
	}
}

void SdLogTrigger::onSlowCallback() {
	// Conditional logging disabled -> always log (current/default behavior)
	if (!engineConfiguration->sdCardConditionalLogging) {
		m_logging = true;
		m_state = SD_LOG_UNCONDITIONAL;
		return;
	}

	// A configured toggle button takes precedence over the sensor conditions
	if (m_buttonInited && isBrainPinValid(engineConfiguration->sdLogTriggerPin)) {
		bool level = m_button.readPinEvent();
		// toggle on the rising edge (press): on->off->on...
		if (level && !m_lastButtonLevel) {
			m_logging = !m_logging;
		}
		m_lastButtonLevel = level;

		m_state = m_logging ? SD_LOG_ACTIVE : SD_LOG_BUTTON_OFF;
		return;
	}

	// Sensor based trigger with RPM hysteresis
	float rpm = Sensor::getOrZero(SensorType::Rpm);

	uint16_t startRpm = engineConfiguration->sdLogStartRpm;
	uint16_t stopRpm = engineConfiguration->sdLogStopRpm;
	// guard against an invalid (non-hysteresis) config: fall back to a single threshold
	if (stopRpm >= startRpm) {
		stopRpm = startRpm;
	}

	// Secondary conditions gate the START only (0 = ignore that condition)
	bool secondaryOk =
		(engineConfiguration->sdLogMinTps == 0 || Sensor::getOrZero(SensorType::Tps1)         >= engineConfiguration->sdLogMinTps) &&
		(engineConfiguration->sdLogMinMap == 0 || Sensor::getOrZero(SensorType::Map)          >= engineConfiguration->sdLogMinMap) &&
		(engineConfiguration->sdLogMinVss == 0 || Sensor::getOrZero(SensorType::VehicleSpeed) >= engineConfiguration->sdLogMinVss);

	// Start once RPM crosses the start threshold (and secondary conditions are met)
	if (rpm >= startRpm && secondaryOk) {
		m_logging = true;
	}

	// The stop delay starts counting once RPM drops below the stop threshold
	if (rpm >= stopRpm) {
		m_stopTimer.reset();
	} else if (m_stopTimer.hasElapsedSec(engineConfiguration->sdLogStopDelay)) {
		m_logging = false;
	}

	// Reason for the indicator
	if (m_logging) {
		m_state = (rpm < stopRpm) ? SD_LOG_STOP_DELAY : SD_LOG_ACTIVE;
	} else {
		m_state = (rpm >= startRpm && !secondaryOk) ? SD_LOG_WAIT_COND : SD_LOG_WAIT_RPM;
	}
}
