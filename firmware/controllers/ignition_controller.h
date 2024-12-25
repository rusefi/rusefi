// file ignition_controller.h

#pragma once

#include "engine_module.h"

#include <rusefi/timer.h>

class IgnitionController : public EngineModule {
public:
	using interface_t = IgnitionController;

	void onSlowCallback() override;

	virtual bool getIgnState() const {
		return m_lastState;
	}

	float secondsSinceIgnVoltage() {
	   return m_timeSinceIgnVoltage.getElapsedSeconds();
	}

	bool getPendingSleep() {
		return m_pendingSleep;
	}

	void resetPendingSleep() {
		m_pendingSleep = 0;
	}

private:
	Timer m_timeSinceIgnVoltage;
	bool m_lastState = false;
	bool m_pendingSleep = false;
};

// USB power or vehicle power?
bool isIgnVoltage();
