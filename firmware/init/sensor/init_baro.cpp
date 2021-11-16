#include "pch.h"
#include "Lps25Sensor.h"

static Lps25 device;
static Lps25Sensor sensor(device);

void initBaro() {
	// If there's already an external (analog) baro sensor configured,
	// don't configure the internal one.
	if (Sensor::hasSensor(SensorType::BarometricPressure)) {
		return;
	}

	if (device.init(CONFIG(lps25BaroSensorScl), CONFIG(lps25BaroSensorSda))) {
		sensor.Register();
	}
}

void baroLps25Update() {
#if EFI_PROD_CODE
	if (device.hasInit()) {
		sensor.update();
	}
#endif // EFI_PROD_CODE
}
