#include "main.h"
#include "engine_configuration.h"
#include "interpolation.h"
#include "boards.h"
#include "adc_inputs.h"

extern engine_configuration_s *engineConfiguration;

float getAfr(void) {
	afr_sensor_s * sensor = &engineConfiguration->afrSensor;

	float volts = getVoltageDivided(sensor->afrAdcChannel);

	return interpolate(sensor->v1, sensor->value1, sensor->v2, sensor->value2, volts);
}
