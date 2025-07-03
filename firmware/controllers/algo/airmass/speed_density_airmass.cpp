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

float SpeedDensityAirmass::getMap(float rpm, bool postState) {
	// If predictive MAP AE isn't enabled, just use the original logic.
	if (engineConfiguration->accelEnrichmentMode != AE_MODE_PREDICTIVE_MAP) {
		float fallbackMap = m_mapEstimationTable->getValue(rpm, Sensor::getOrZero(SensorType::Tps1));
		float realMap = Sensor::get(SensorType::Map).value_or(fallbackMap);

		#if EFI_TUNER_STUDIO
		if (postState) {
			engine->outputChannels.fallbackMap = fallbackMap / 0.1f;
			// Store the real MAP in our new channel, making sure to apply the scale
			engine->outputChannels.effectiveMap = realMap / 0.1f;
		}
		#endif // EFI_TUNER_STUDIO

		return realMap;
	}

	// --- Predictive MAP Acceleration Enrichment Logic ---

	auto realTimeMapResult = Sensor::get(SensorType::Map);
	auto tps = Sensor::getOrZero(SensorType::Tps1);
	float realTimeMap = realTimeMapResult.value_or(m_initialRealMap);

	float effectiveMap = 0;

	if (m_isMapPredictionActive) {
		float blendDuration = engineConfiguration->mapPredictionBlendDuration;
		float elapsedTime = m_predictionTimer.getElapsedSeconds();

		float blendFactor = (blendDuration > 0) ? (elapsedTime / blendDuration) : 1.0f;
		// Linearly interpolate between the initial predicted and real values
		effectiveMap = m_initialPredictedMap + (m_initialRealMap - m_initialPredictedMap) * blendFactor;

		if (elapsedTime >= blendDuration || realTimeMap >= effectiveMap || blendDuration <= 0 || !realTimeMapResult) {
			m_isMapPredictionActive = false;
		}
	} else {
		if (engine->module<TpsAccelEnrichment>()->isAccelEventTriggered()) {
			float predictedMap = m_mapEstimationTable->getValue(rpm, tps);

			if (realTimeMapResult && predictedMap > realTimeMap) {
				m_isMapPredictionActive = true;
				m_predictionTimer.reset();
				m_initialPredictedMap = predictedMap;
				m_initialRealMap = realTimeMap;
				effectiveMap = predictedMap;
			}
		}
	}

	if (!m_isMapPredictionActive) {
		float fallbackMap = m_mapEstimationTable->getValue(rpm, tps);
		effectiveMap = realTimeMapResult.value_or(fallbackMap);
	}

#if EFI_TUNER_STUDIO
	if (postState) {
		// FINAL FIX: Convert the float 'effectiveMap' to a scaled integer for logging
		engine->outputChannels.effectiveMap = effectiveMap / 0.1f;

		float fallbackMap = m_mapEstimationTable->getValue(rpm, tps);
		engine->outputChannels.fallbackMap = fallbackMap / 0.1f;
	}
#endif // EFI_TUNER_STUDIO

	return effectiveMap;
}
