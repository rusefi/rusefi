#include "engine.h"
#include "Lps25Sensor.h"
#include "pin_repository.h"
#include "rusefi_hw_enums.h"

EXTERN_ENGINE;

static Lps25 device;
static Lps25Sensor sensor(device);

void initBaro(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	// If there's already an external (analog) baro sensor configured,
	// don't configure the internal one.
	if (Sensor::hasSensor(SensorType::BarometricPressure)) {
		return;
	}

	if (device.init(CONFIG(lps25BaroSensorScl), CONFIG(lps25BaroSensorScl))) {
		sensor.Register();
	}
}

void baroUpdate() {
#if EFI_PROD_CODE
	sensor.update();
#endif
}
