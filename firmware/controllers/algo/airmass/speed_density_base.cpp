/**
 * @file	speed_density_base.cpp
 *
 * Base for speed density (ie, ideal gas law) math shared by multiple fueling modes.
 * 
 * @date July 22, 2020
 * @author Matthew Kennedy, (C) 2020
 */

#include "speed_density_base.h"
#include "engine_configuration.h"

/**
 * Derived via:
 * (8.31 J K mol^-1)  <- ideal gas constant R
 * /
 * (28.97g mol^-1)    <- molar mass of air
 * = 0.28705 J*K/g
 */
#define AIR_R 0.28705f

float idealGasLaw(float volume, float pressure, float temperature) {
	return volume * pressure / (AIR_R * temperature);
}

/*static*/ float SpeedDensityBase::getAirmassImpl(float ve, float manifoldPressure, float temperature DECLARE_ENGINE_PARAMETER_SUFFIX) {
	float cycleAir = ve * idealGasLaw(CONFIG(specs.displacement), manifoldPressure, temperature);
	return cycleAir / CONFIG(specs.cylindersCount);
}
