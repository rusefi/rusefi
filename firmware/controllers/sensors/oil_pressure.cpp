/**
 *  @author Matthew Kennedy, (c) 2017
 */

#include "oil_pressure.h"

#include "interpolation.h"
#include "analog_input.h"
#include "engine.h"

EXTERN_ENGINE;

float getOilPressureLegacy(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// If there's no sensor, return 0 pressure.
	if (engineConfiguration->oilPressure.hwChannel == EFI_ADC_NONE) {
		return 0.0f;
	}

	oil_pressure_config_s* sensor = &CONFIG(oilPressure);

	float volts = getVoltageDivided("oilp", sensor->hwChannel);

	return interpolateMsg("oil", sensor->v1, sensor->value1, sensor->v2, sensor->value2, volts);
}
