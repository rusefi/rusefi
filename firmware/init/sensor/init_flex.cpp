#include "init.h"

EXTERN_ENGINE;

// https://rusefi.com/forum/viewtopic.php?p=37452&sid=829804c90d5b2e1fecd1b900cf1b1811#p37452

void initFlexSensor(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	// Nothing to do if no sensor configured
	if (CONFIG(flexSensorPin) == GPIO_UNASSIGNED) {
		return;
	}

	// TODO: init sensor
}
