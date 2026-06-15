/*
 * @file sd_log_trigger.h
 *
 * Conditional SD logging (phase 1): start/stop SD card logging based on trigger
 * conditions (RPM with hysteresis + optional TPS/MAP/VSS, or a toggle button),
 * so we avoid huge idle/cruise logs and capture only the interesting events.
 *
 * See also test_sd_log_trigger.cpp
 */

#pragma once

#include "debounce.h"

class SdLogTrigger : public EngineModule {
public:
	void onSlowCallback() override;
	// registers the toggle button pin so it takes part in pin-conflict detection
	void onConfigurationChange(engine_configuration_s const* previousConfig) override;

	// Should the SD logger be writing right now?
	bool shouldLog() const {
		return m_state == SD_LOG_UNCONDITIONAL || m_state == SD_LOG_ACTIVE || m_state == SD_LOG_STOP_DELAY;
	}

	// Why it is or is not logging - drives the "SD: Logging state" indicator
	sd_log_state_e getState() const {
		return m_state;
	}

private:
	bool m_logging = false;
	sd_log_state_e m_state = SD_LOG_DISABLED;

	// counts time since RPM dropped below the stop threshold
	Timer m_stopTimer;

	// toggle button (press to start, press again to stop)
	ButtonDebounce m_button{"sd_log"};
	bool m_buttonInited = false;
	bool m_lastButtonLevel = false;
};
