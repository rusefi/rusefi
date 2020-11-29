#include "global.h"
#include "engine.h"
#include "speed_density_airmass.h"
#include "map.h"
#include "perf_trace.h"

EXTERN_ENGINE;

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

	float map = getMap(PASS_ENGINE_PARAMETER_SIGNATURE);
	if (cisnan(map)) {
		warning(CUSTOM_ERR_TCHARGE_NOT_READY2, "map not ready"); // this could happen during HW CI during configuration reset
		return {};
	}

	engine->engineState.sd.manifoldAirPressureAccelerationAdjustment = engine->engineLoadAccelEnrichment.getEngineLoadEnrichment(PASS_ENGINE_PARAMETER_SIGNATURE);

	float adjustedMap = engine->engineState.sd.adjustedManifoldAirPressure = map + engine->engineState.sd.manifoldAirPressureAccelerationAdjustment;
	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(adjustedMap), "NaN adjustedMap", {});

	float ve = getVe(rpm, adjustedMap);

	float airMass = getAirmassImpl(ve, adjustedMap, tChargeK PASS_ENGINE_PARAMETER_SUFFIX);
	if (cisnan(airMass)) {
		warning(CUSTOM_ERR_6685, "NaN airMass");
		return {};
	}
#if EFI_PRINTF_FUEL_DETAILS
	printf("getSpeedDensityAirmass map=%.2f adjustedMap=%.2f airMass=%.2f\t\n",
			map, adjustedMap, engine->engineState.sd.adjustedManifoldAirPressure);
#endif /*EFI_PRINTF_FUEL_DETAILS */

	return {
		airMass,
		map,	// AFR/VE table Y axis
	};
}
