#include "init.h"

EXTERN_ENGINE;

void initFlexSensor(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	// Nothing to do if no sensor configured
	if (CONFIG(flexSensorPin) == GPIO_UNASSIGNED) {
		return;
	}

	// TODO: init sensor
}
