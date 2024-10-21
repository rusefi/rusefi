/*
 * @file knock_logic.c
 *
 * @date Apr 04, 2021
 * @author Andrey Gusakov
 */

#include "pch.h"
#include "knock_logic.h"

#include "hip9011.h"

int getCylinderKnockBank(uint8_t cylinderNumber) {
	// C/C++ can't index in to bit fields, we have to provide lookup ourselves
	switch (cylinderNumber) {
#if EFI_PROD_CODE
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
#endif
		default:
			return 0;
	}
}

void KnockControllerBase::onKnockSenseCompleted(uint8_t cylinderNumber, float dbv, efitick_t lastKnockTime) {
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
		auto distToMinimum = baseTiming - (-20);

		// percent -> ratio = divide by 100
		auto retardFraction = engineConfiguration->knockRetardAggression * 0.01f;
		auto retardAmount = distToMinimum * retardFraction;

		auto  trimFuelFraction = engineConfiguration->knockFuelTrimAggression * 0.01f;
		float trimFuelPersent = clampF(0.0, (float)engineConfiguration->knockFuelTrim, 30.0);
		float trimFuelCoeff = (trimFuelPersent / 100.f);
		float trimFuelAmount = trimFuelCoeff * trimFuelFraction;

		{
			// Adjust knock retard under lock
			chibios_rt::CriticalSectionLocker csl;

			auto newRetard = m_knockRetard + retardAmount;
			m_knockRetard = clampF(0, newRetard, m_maximumRetard);

			auto newFuelTrim = m_knockFuelTrimMultiplier + trimFuelAmount;
			m_knockFuelTrimMultiplier = clampF(0.0, newFuelTrim, 0.3); // remove magic 30%
		}
	}
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

void KnockControllerBase::onFastCallback() {
	m_knockThreshold = getKnockThreshold();
	m_maximumRetard = getMaximumRetard();

	constexpr auto callbackPeriodSeconds = FAST_CALLBACK_PERIOD_MS / 1000.0f;

	auto applyRetardAmount = engineConfiguration->knockRetardReapplyRate * callbackPeriodSeconds;
	auto applyFuelAmount = engineConfiguration->knockFuelTrimReapplyRate * callbackPeriodSeconds;

	// disable knock supression then deacceleraction
	//auto TPSValue = Sensor::getOrZero(SensorType::Tps1);

	{
		// Adjust knock retard under lock
		chibios_rt::CriticalSectionLocker csl;


		// if(TPSValue <= engineConfiguration->knockSupressMinTps) {
		// 	m_knockRetard = 0.0;
		// 	m_knockFuelTrimMultiplier = 0.0;
		// 	return;
		// }

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

		// don't allow retard to go negative
		if (newTrim < 0) {
			m_knockFuelTrimMultiplier = 0;
		} else {
			m_knockFuelTrimMultiplier = newTrim;
		}
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

#if EFI_HIP_9011 || EFI_SOFTWARE_KNOCK
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

#endif // EFI_HIP_9011 || EFI_SOFTWARE_KNOCK

void Engine::onSparkFireKnockSense(uint8_t cylinderNumber, efitick_t nowNt) {
#if EFI_HIP_9011 || EFI_SOFTWARE_KNOCK
	cylinderNumberCopy = cylinderNumber;
	scheduleByAngle(nullptr, nowNt,
			/*angle*/engineConfiguration->knockDetectionWindowStart, { startKnockSampling, engine });
#else
	UNUSED(cylinderNumber);
	UNUSED(nowNt);
#endif

#if EFI_HIP_9011
	hip9011_onFireEvent(cylinderNumber, nowNt);
#endif
}
