#include "pch.h"
#include "speed_density_airmass.h"

AirmassResult SpeedDensityAirmass::getAirmass(int rpm) {
	ScopePerf perf(PE::GetSpeedDensityFuel);

	/**
	 * most of the values are pre-calculated for performance reasons
	 */
	float tChargeK = ENGINE(engineState.sd.tChargeK);
	if (cisnan(tChargeK)) {
		warning(CUSTOM_ERR_TCHARGE_NOT_READY2, "tChargeK not ready"); // this would happen before we have CLT reading for example
		return {};
	}

	auto map = getMap(rpm);

	float adjustedMap = map;
	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(adjustedMap), "NaN adjustedMap", {});

	float ve = getVe(rpm, adjustedMap);

	float airMass = getAirmassImpl(ve, adjustedMap, tChargeK PASS_ENGINE_PARAMETER_SUFFIX);
	if (cisnan(airMass)) {
		warning(CUSTOM_ERR_6685, "NaN airMass");
		return {};
	}
#if EFI_PRINTF_FUEL_DETAILS
	printf("getSpeedDensityAirmass map=%.2f\n",
			map);
#endif /*EFI_PRINTF_FUEL_DETAILS */

	return {
		airMass,
		map,	// AFR/VE table Y axis
	};
}

float SpeedDensityAirmass::getMap(int rpm) const {
	float fallbackMap;
	if (CONFIG(enableMapEstimationTableFallback)) {
		// if the map estimation table is enabled, estimate map based on the TPS and RPM
		fallbackMap = m_mapEstimationTable->getValue(rpm, TPS_2_BYTE_PACKING_MULT * Sensor::getOrZero(SensorType::Tps1));
	} else {
		fallbackMap = CONFIG(failedMapFallback);
	}

#if EFI_TUNER_STUDIO
	if (CONFIG(debugMode) == DBG_MAP) {
		tsOutputChannels.debugFloatField4 = fallbackMap;
	}
#endif // EFI_TUNER_STUDIO

	return Sensor::get(SensorType::Map).value_or(fallbackMap);
}
