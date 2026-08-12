#pragma once

#include "engine_module.h"
#include "obd_error_codes.h"
#include "rusefi/timer.h"

class CheckEngineLight : public EngineModule {
public:
	void setDefaultConfiguration() override;
	void onSlowCallback() override;

private:
	struct RangeState {
		ObdCode activeCode = ObdCode::None;
		ObdCode pendingCode = ObdCode::None;
		Timer pendingTimer;
	};

	void updateRange(RangeState& state, bool available, float value, float minimum, float maximum,
		ObdCode lowCode, ObdCode highCode);
	void clearCurrentFaults();

	RangeState m_battery;
	RangeState m_map;
	RangeState m_iat;
	RangeState m_tps;
};
