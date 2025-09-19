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
			// prediction phase is over
			m_isMapPredictionActive = false;
	}

	float effectiveMap = 0;
	if (m_isMapPredictionActive) {
		float blendFactor = elapsedTime / blendDuration;
		// Linearly interpolate between the initial predicted and real values
		effectiveMap = m_initialPredictedMap + (m_initialRealMap - m_initialPredictedMap) * blendFactor;

		if (mapSensor >= effectiveMap) {
			m_isMapPredictionActive = false;
		}
	} else {
		if (engine->module<TpsAccelEnrichment>()->isAccelEventTriggered()) {
			float predictedMap = logAndGetFallback(rpm, postState);

			if (predictedMap > mapSensor) {
				m_isMapPredictionActive = true;
			  engine->module<TpsAccelEnrichment>()->m_timeSinceAccel.reset();
				m_predictionTimer.reset();
				m_initialPredictedMap = predictedMap;
				m_initialRealMap = mapSensor;
				effectiveMap = predictedMap;
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
