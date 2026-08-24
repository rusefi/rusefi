/**
 * @file knock_controller.cpp
 * @brief High-level knock control logic.
 *
 * This module handles the response to detected knock events.
 * Key responsibilities:
 * 1. Tracking per-cylinder knock levels and peak values.
 * 2. Comparing current noise levels against an RPM-based threshold.
 * 3. Calculating ignition timing retard and fuel trim enrichment when knock is detected.
 * 4. Gradually decaying (re-applying) timing and fuel trims back to base values when no knock is present.
 *
 * @date Apr 04, 2021
 * @author Andrey Gusakov
 */

#include "pch.h"
#include "knock_logic.h"

int getCylinderKnockBank(uint8_t cylinderNumber) {
	// C/C++ can't index in to bit fields, we have to provide lookup ourselves
	switch (cylinderNumber) {
		case 0:
			return engineConfiguration->knockBankCyl1;
		case 1:
			return engineConfiguration->knockBankCyl2;
		case 2:
			return engineConfiguration->knockBankCyl3;
		case 3:
			return engineConfiguration->knockBankCyl4;
		case 4:
			return engineConfiguration->knockBankCyl5;
		case 5:
			return engineConfiguration->knockBankCyl6;
		case 6:
			return engineConfiguration->knockBankCyl7;
		case 7:
			return engineConfiguration->knockBankCyl8;
		case 8:
			return engineConfiguration->knockBankCyl9;
		case 9:
			return engineConfiguration->knockBankCyl10;
		case 10:
			return engineConfiguration->knockBankCyl11;
		case 11:
			return engineConfiguration->knockBankCyl12;
		default:
			return 0;
	}
}

	/**
	 * Main entry point for a processed knock event.
	 * 1. Checks if the detected level (dBv) exceeds the threshold.
	 * 2. Updates per-cylinder and global peak detectors.
	 * 3. If knock is confirmed:
	 *    - Increments the total knock count.
	 *    - Calculates timing retard based on 'knockRetardAggression'.
	 *    - Calculates fuel trim enrichment based on 'knockFuelTrimAggression'.
	 *    - Clamps adjustments to configured maximums.
	 */
bool KnockControllerBase::onKnockSenseCompleted(uint8_t cylinderNumber, float dbv, efitick_t lastKnockTime) {
	// Adjust by the user-configured gain for this cylinder
	dbv += m_gain[cylinderNumber];

	bool isKnock = dbv > m_knockThreshold;

	// Per-cylinder peak detector
	float cylPeak = peakDetectors[cylinderNumber].detect(dbv, lastKnockTime);
	m_knockCyl[cylinderNumber] = roundf(cylPeak);

	// All-cylinders peak detector
	m_knockLevel = allCylinderPeakDetector.detect(dbv, lastKnockTime);

	if (isKnock) {
		m_knockCount++;

		auto baseTiming = engine->engineState.timingAdvance[cylinderNumber];

		// TODO: 20 configurable? Better explanation why 20?
		// Calculation order:
		// 1. Calculate the distance from current timing to the maximum allowed retard.
		// 2. Multiply by aggression to get this event's retard amount.
		// 3. Add to total knock retard and CLAMP to maximum.
		// This order ensures we don't retard more than the maximum even with high aggression.
		auto distToMinimum = baseTiming - (-20);

		// percent -> ratio = divide by 100
		auto retardFraction = engineConfiguration->knockRetardAggression * 0.01f;
		auto retardAmount = distToMinimum * retardFraction;

    // TODO: remove magic 30% m_maximumFuelTrim?
    auto maximumFuelTrim = 0.3f;

		// Calculation order:
		// 1. Clamp the fuel trim to the maximum configured or hardcoded limit.
		// 2. Multiply by the aggression factor.
		// 3. Convert percentage to multiplier ratio.
		// 4. Update the multiplier and clamp the final value.
		auto  trimFuelFraction = engineConfiguration->knockFuelTrimAggression * 0.01f;
		float trimFuelPercent = clampF(0.f, (float)engineConfiguration->knockFuelTrim, maximumFuelTrim * 100.f);
		float trimFuelAmountPercent = trimFuelPercent * trimFuelFraction;
		float trimFuelAmount = trimFuelAmountPercent / 100.f;

		{
			// Adjust knock retard under lock
			chibios_rt::CriticalSectionLocker csl;

			auto newRetard = m_knockRetard + retardAmount;
			m_knockRetard = clampF(0.f, newRetard, m_maximumRetard);

			auto newFuelTrim = m_knockFuelTrimMultiplier + trimFuelAmount;
			m_knockFuelTrimMultiplier = clampF(0.f, newFuelTrim, maximumFuelTrim);
		}
	}

	return isKnock;
}

float KnockControllerBase::getKnockRetard() const {
	return m_knockRetard;
}

uint32_t KnockControllerBase::getKnockCount() const {
	return m_knockCount;
}

float KnockControllerBase::getFuelTrimMultiplier() const {
	return 1.0 + m_knockFuelTrimMultiplier;
}

	/**
	 * Periodic callback (typically every 10ms via FAST_CALLBACK_PERIOD_MS).
	 * 1. Updates RPM-dependent thresholds and limits.
	 * 2. Checks for TPS-based suppression (disables knock control at low TPS).
	 * 3. Decays the current knock retard and fuel trim back towards zero
	 *    based on configured 'reapplyRate' parameters.
	 */
void KnockControllerBase::onFastCallback() {
	constexpr auto callbackPeriodSeconds = FAST_CALLBACK_PERIOD_MS / 1000.0f;

	auto applyRetardAmount = engineConfiguration->knockRetardReapplyRate * callbackPeriodSeconds;
	auto applyFuelAmount = engineConfiguration->knockFuelTrimReapplyRate * 0.01f * callbackPeriodSeconds;

	// disable knock suppression then deceleration
	auto TPSValue = Sensor::getOrZero(SensorType::Tps1);

	{
		// Adjust knock retard under lock
		chibios_rt::CriticalSectionLocker csl;


		 if(TPSValue < engineConfiguration->knockSuppressMinTps) {
		 	m_knockRetard = 0.0;
		 	m_knockFuelTrimMultiplier = 0.0;
		 	return;
		 }

		// Reduce knock retard at the requested rate
		float newRetard = m_knockRetard - applyRetardAmount;

		// don't allow retard to go negative
		if (newRetard < 0) {
			m_knockRetard = 0;
		} else {
			m_knockRetard = newRetard;
		}

		// Reduce fuel trim at the requested rate
		float newTrim = m_knockFuelTrimMultiplier - applyFuelAmount;

		// don't allow trim to go negative
		if (newTrim < 0) {
			m_knockFuelTrimMultiplier = 0;
		} else {
			m_knockFuelTrimMultiplier = newTrim;
		}
	}

	hasKnockRecently = !m_lastKnockTimer.hasElapsedSec(0.5f);
	hasKnockRetardNow = m_knockRetard > 0;

	m_knockThreshold = getKnockThreshold();
	m_maximumRetard = getMaximumRetard();

	auto rpm = Sensor::getOrZero(SensorType::Rpm);
	auto load = getIgnitionLoad();

	for (size_t i = 0; i < engineConfiguration->cylindersCount; i++) {
		m_gain[i] = interpolate3d(
			config->knockGains[i].table,
			config->knockGainLoadBins, load,
			config->knockGainRpmBins, rpm
		);
	}
}

float KnockController::getKnockThreshold() const {
	return interpolate2d(
		Sensor::getOrZero(SensorType::Rpm),
		config->knockNoiseRpmBins,
		config->knockBaseNoise
	);
}

float KnockController::getMaximumRetard() const {
	return
		interpolate3d(
			config->maxKnockRetardTable,
			config->maxKnockRetardLoadBins, getIgnitionLoad(),
			config->maxKnockRetardRpmBins, Sensor::getOrZero(SensorType::Rpm)
		);
}

// This callback is to be implemented by the knock sense driver
__attribute__((weak)) void onStartKnockSampling(uint8_t cylinderNumber, float samplingTimeSeconds, uint8_t channelIdx) {
	UNUSED(cylinderNumber);
	UNUSED(samplingTimeSeconds);
	UNUSED(channelIdx);
}

#if EFI_SOFTWARE_KNOCK
static uint8_t cylinderNumberCopy;

// Called when its time to start listening for knock
// Does some math, then hands off to the driver to start any sampling hardware
static void startKnockSampling(Engine* p_engine) {
	// todo: why do we pass engine as parameter? is that for unit tests?
	if (!p_engine->rpmCalculator.isRunning()) {
		return;
	}

	// Convert sampling angle to time
	float samplingSeconds = engine->rpmCalculator.oneDegreeUs * engineConfiguration->knockSamplingDuration / US_PER_SECOND_F;

	// Look up which channel this cylinder uses
	auto channel = getCylinderKnockBank(cylinderNumberCopy);

	// Call the driver to begin sampling
	onStartKnockSampling(cylinderNumberCopy, samplingSeconds, channel);
}

#endif // EFI_SOFTWARE_KNOCK

/**
 * Triggered when a spark plug fires to schedule the knock detection window.
 * rusEFI uses a crank-angle based window for knock detection.
 */
void Engine::onSparkFireKnockSense(uint8_t cylinderNumber, efitick_t nowNt) {
#if EFI_SOFTWARE_KNOCK
	cylinderNumberCopy = cylinderNumber;
	scheduleByAngle(nullptr, nowNt,
			/*angle*/engineConfiguration->knockDetectionWindowStart, action_s::make<startKnockSampling>(static_cast<Engine*>(engine)));
#else
	UNUSED(cylinderNumber);
	UNUSED(nowNt);
#endif
}
