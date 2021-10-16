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

	engineTooSlow = rpm < 500;

	if (engineTooSlow) {
		return false;
	}

	auto maxRpm = CONFIG(maxAcRpm);
	engineTooFast = maxRpm != 0 && maxRpmDeadband.gt(rpm, maxRpm);
	if (engineTooFast) {
		return false;
	}

	auto clt = Sensor::get(SensorType::Clt);

	noClt = !clt;
	// No AC with failed CLT
	if (noClt) {
		return false;
	}

	// Engine too hot, disable
	auto maxClt = CONFIG(maxAcClt);
	engineTooHot = (maxClt != 0) && maxCltDeadband.gt(maxClt, clt.Value);
	if (engineTooHot) {
		return false;
	}

	// TPS too high, disable
	auto maxTps = CONFIG(maxAcTps);
	tpsTooHigh = maxTps != 0 && maxTpsDeadband.gt(maxTps, Sensor::getOrZero(SensorType::Tps1));
	if (tpsTooHigh) {
			return false;
	}

	acButtonState = ENGINE(acSwitchState);
	// All conditions allow AC, simply pass thru switch
	return acButtonState;
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
