#include "pch.h"

#include "ac_control.h"
#include "deadband.h"
#include "max_limit_with_hysteresis.h"

namespace {
	// Deadbands to prevent rapid switching on/off of AC
	Deadband<200> maxRpmDeadband;
	Deadband<5> maxCltDeadband;
	Deadband<5> maxTpsDeadband;
	Deadband<AcController::PRESSURE_DEADBAND_WIDTH> minPressureDeadband;
	MaxLimitWithHysteresis acPressureEnableHysteresis;
}

bool AcController::getAcState() {
	auto rpm = Sensor::getOrZero(SensorType::Rpm);

	engineTooSlow = rpm < 500;

	if (engineTooSlow) {
		return false;
	}

	auto maxRpm = engineConfiguration->maxAcRpm;
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
	auto maxClt = engineConfiguration->maxAcClt;
	engineTooHot = (maxClt != 0) && maxCltDeadband.gt(clt.Value, maxClt);
	if (engineTooHot) {
		return false;
	}

	// TPS too high, disable
	auto maxTps = engineConfiguration->maxAcTps;
	tpsTooHigh = maxTps != 0 && maxTpsDeadband.gt(Sensor::getOrZero(SensorType::Tps1), maxTps);
	if (tpsTooHigh) {
			return false;
	}

    const auto acPressure= Sensor::get(SensorType::AcPressure);
    if (acPressure.Valid) {
        const auto minAcPressure = static_cast<float>(engineConfiguration->minAcPressure);
        acPressureTooLow = minPressureDeadband.lt(acPressure.Value, minAcPressure);
        if (acPressureTooLow) {
            return false;
        }

        const auto maxAcPressure = static_cast<float>(engineConfiguration->maxAcPressure);
        acPressureTooHigh = acPressureEnableHysteresis.checkIfLimitIsExceeded(
        	acPressure.Value,
        	maxAcPressure,
        	engineConfiguration->acPressureEnableHyst
        );
        if (acPressureTooHigh) {
            return false;
        }
    }

	if (isDisabledByLua) {
		return false;
	}

	// All conditions allow AC, simply pass thru switch
	return acButtonState;
}

void AcController::onSlowCallback() {
	bool isEnabled = getAcState();

	m_acEnabled = isEnabled;

	if (!isEnabled) {
		// reset the timer if AC is off
		m_timeSinceNoAc.reset();
	}

	float acDelay = engineConfiguration->acDelay;
	if (acDelay == 0) {
		// Without delay configured, enable immediately
		acCompressorState = isEnabled;
	} else {
		acCompressorState = isEnabled && m_timeSinceNoAc.hasElapsedSec(acDelay);
	}

	enginePins.acRelay.setValue(acCompressorState);
}

bool AcController::isAcEnabled() const {
	return m_acEnabled;
}
