#include "main.h"
#include "engine_configuration.h"
#include "interpolation.h"
#include "adc_inputs.h"
#include "engine.h"

EXTERN_ENGINE;

float getAfr(DECLARE_ENGINE_PARAMETER_F) {
	afr_sensor_s * sensor = &engineConfiguration->afr;

	float volts = getVoltageDivided(sensor->hwChannel);

	return interpolate(sensor->v1, sensor->value1, sensor->v2, sensor->value2, volts);
}

void initEgoSensor(afr_sensor_s *sensor, ego_sensor_e type) {

	switch (type) {
	case ES_BPSX_D1:
		/**
		 * This decodes BPSX D1 Wideband Controller analog signal
		 */
		sensor->v1 = 0;
		sensor->value1 = 9;
		sensor->v2 = 5;
		sensor->value2 = 19;
		break;

	case ES_Innovate_MTX_L:
		sensor->v1 = 0;
		sensor->value1 = 7.35;
		sensor->v2 = 5;
		sensor->value2 = 22.39;
		break;
	case ES_14Point7_Free:
		sensor->v1 = 0;
		sensor->value1 = 10;
		sensor->v2 = 5;
		sensor->value2 = 20;
		break;
	default:
		firmwareError("Unexpected EGO %d", type);
		break;
	}
}
