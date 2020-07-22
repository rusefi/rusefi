
#include "global.h"
#include "speed_density_base.h"

EXTERN_ENGINE;

/**
 * is J/g*K
 * aka 
 */
#define GAS_R 0.28705

float idealGasLaw(float volume, float pressure, float temperature) {
	return volume * pressure / (GAS_R * temperature);
}

float SpeedDensityBase::getAirmassImpl(float ve, float manifoldPressure, float temperature DECLARE_ENGINE_PARAMETER_SUFFIX) {
	float cycleAir = ve * idealGasLaw(CONFIG(specs.displacement), manifoldPressure, temperature);
	return cycleAir / CONFIG(specs.cylindersCount);
}
