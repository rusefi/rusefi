#include "pch.h"

#include "ac_control.h"
#include "deadband.h"

// Deadbands to prevent rapid switching on/off of AC
static Deadband<200> maxRpmDeadband;
static Deadband<5> maxCltDeadband;
static Deadband<5> maxTpsDeadband;

bool AcState::getAcState(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	latest_usage_ac_control = getTimeNowSeconds();
	auto rpm = Sensor::getOrZero(SensorType::Rpm);

	ANNOTATE_IF(engineTooSlow, rpm < 500) {
		return false;
	}

	auto maxRpm = CONFIG(maxAcRpm);
	ANNOTATE_IF(engineTooFast, maxRpm != 0 && maxRpmDeadband.gt(rpm, maxRpm)) {
		return false;
	}

	auto clt = Sensor::get(SensorType::Clt);

	// No AC with failed CLT
	ANNOTATE_IF(noClt, !clt) {
		return false;
	}

	// Engine too hot, disable
	auto maxClt = CONFIG(maxAcClt);
	ANNOTATE_IF(engineTooHot, (maxClt != 0) && maxCltDeadband.gt(clt.Value, maxClt)) {
		return false;
	}

	// TPS too high, disable
	auto maxTps = CONFIG(maxAcTps);
	ANNOTATE_IF(tpsTooHigh, maxTps != 0 && maxTpsDeadband.gt(Sensor::getOrZero(SensorType::Tps1), maxTps)) {
		return false;
	}

	// All conditions allow AC, simply pass thru switch
	ANNOTATE_RETURN(acButtonState, ENGINE(acSwitchState));
}

bool AcState::updateAc(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	bool isEnabled = getAcState(PASS_ENGINE_PARAMETER_SIGNATURE);

	enginePins.acRelay.setValue(isEnabled);

#if EFI_TUNER_STUDIO
	tsOutputChannels.acSwitchState = engine->acSwitchState;
	tsOutputChannels.acState = isEnabled;
#endif // EFI_TUNER_STUDIO

	return isEnabled;
}
