#include "pch.h"
#include "speed_density_airmass.h"
#include "accel_enrichment.h"


AirmassResult SpeedDensityAirmass::getAirmass(float rpm, bool postState) {
	ScopePerf perf(PE::GetSpeedDensityFuel);

	auto map = getMap(rpm, postState);

	return getAirmass(rpm, map, postState);
}

AirmassResult SpeedDensityAirmass::getAirmass(float rpm, float map, bool postState) {
	/**
	 * most of the values are pre-calculated for performance reasons
	 */
	float tChargeK = engine->engineState.sd.tChargeK;
	if (std::isnan(tChargeK)) {
		warning(ObdCode::CUSTOM_ERR_TCHARGE_NOT_READY2, "tChargeK not ready"); // this would happen before we have CLT reading for example
		return {};
	}

	float ve = getVe(rpm, map, postState);

	float airMass = getAirmassImpl(ve, map, tChargeK);
	if (std::isnan(airMass)) {
		warning(ObdCode::CUSTOM_ERR_6685, "NaN airMass");
		return {};
	}
#if EFI_PRINTF_FUEL_DETAILS
	printf("getSpeedDensityAirmass map=%.2f\n", map);
#endif /*EFI_PRINTF_FUEL_DETAILS */

	return {
		airMass,
		map,	// AFR/VE table Y axis
	};
}

float SpeedDensityAirmass::getAirflow(float rpm, float map, bool postState) {
	auto airmassResult = getAirmass(rpm, map, postState);

	float massPerCycle = airmassResult.CylinderAirmass * engineConfiguration->cylindersCount;

	if (!engineConfiguration->twoStroke) {
		// 4 stroke engines only do a half cycle per rev
		massPerCycle = massPerCycle / 2;
	}

	// g/s
	return massPerCycle * rpm / 60;
}

float SpeedDensityAirmass::getPredictiveMap(float rpm, bool postState, float mapSensor) {
	float blendDuration = interpolate2d(rpm, config->predictiveMapBlendDurationBins,
						config->predictiveMapBlendDurationValues);

	float elapsedTime = m_predictionTimer.getElapsedSeconds();

	if (m_isMapPredictionActive && elapsedTime >= blendDuration) {
			m_isMapPredictionActive = false;
			engine->outputChannels.mapPredEventOver++;
	}

	// Always compute table output for diagnostics, regardless of accel event state.
	float predictedMap = logAndGetFallback(rpm, postState);

	auto& ae = engine->module<TpsAccelEnrichment>();

	// Clear the re-trigger lock once the throttle returns below the accel threshold,
	// requiring a fresh pedal engagement before prediction can fire again.
	if (!ae->isAboveAccelThreshold) {
		m_awaitingThrottleRelease = false;
	}

	float effectiveMap = 0;
	if (m_isMapPredictionActive) {
		// Drain any accel events that occurred during prediction so they
		// cannot immediately re-trigger when the blend window ends.
		ae->isAccelEventTriggered();

		// Track the rising TPS: if the table now predicts a higher MAP than our
		// current base, adopt it and restart the blend so we don't under-fuel
		// a still-opening throttle.
		if (predictedMap > m_initialPredictedMap) {
			m_initialPredictedMap = predictedMap;
			m_predictionTimer.reset();
			elapsedTime = 0;
		}

		float blendFactor = elapsedTime / blendDuration;
		// Blend toward the current (rising) sensor value so the transition
		// tracks manifold fill-up rather than the stale pre-event pressure.
		effectiveMap = m_initialPredictedMap + (mapSensor - m_initialPredictedMap) * blendFactor;

		if (mapSensor >= effectiveMap) {
			m_isMapPredictionActive = false;
		}
	} else {
		if (!m_awaitingThrottleRelease && ae->isAccelEventTriggered()) {
			if (predictedMap > mapSensor) {
				m_isMapPredictionActive = true;
				ae->m_timeSinceAccel.reset();
				m_predictionTimer.reset();
				m_initialPredictedMap = predictedMap;
				engine->outputChannels.predTimerResetCnt++;
				effectiveMap = predictedMap;
				m_awaitingThrottleRelease = true;
			}
		}
	}
  engine->outputChannels.isMapPredictionActive = m_isMapPredictionActive;

	if (!m_isMapPredictionActive) {
		effectiveMap = mapSensor;
	}

#if EFI_TUNER_STUDIO
	if (postState) {
		engine->outputChannels.effectiveMap = effectiveMap;
	}
#endif // EFI_TUNER_STUDIO

	return effectiveMap;
}

float SpeedDensityAirmass::logAndGetFallback(float rpm, bool postState) const {
  float fallbackMap = m_mapEstimationTable->getValue(rpm, Sensor::getOrZero(SensorType::Tps1));
#if EFI_TUNER_STUDIO
	if (postState) {
		engine->outputChannels.fallbackMap = fallbackMap;
	}
#endif // EFI_TUNER_STUDIO
  return fallbackMap;
}

float SpeedDensityAirmass::getMap(float rpm, bool postState) {
  auto mapSensor = Sensor::get(SensorType::Map);
	if (mapSensor && engineConfiguration->accelEnrichmentMode == AE_MODE_PREDICTIVE_MAP) {
		return getPredictiveMap(rpm, postState, mapSensor.Value);
	}
	return mapSensor.value_or(logAndGetFallback(rpm, postState));
}
