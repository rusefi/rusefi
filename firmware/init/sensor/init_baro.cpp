#include "pch.h"
#include "lps25.h"

static Lps25 device;

#if EFI_PROD_CODE
static Lps25Thread driver(device);
#endif

void initBaro() {
	// If there's already an external (analog) baro sensor configured,
	// don't configure the internal one.
	if (Sensor::hasSensor(SensorType::BarometricPressure)) {
		return;
	}

	if (device.init(engineConfiguration->lps25BaroSensorScl, engineConfiguration->lps25BaroSensorSda)) {
#if EFI_PROD_CODE
		driver.init();
#endif
		return;
	}
}
