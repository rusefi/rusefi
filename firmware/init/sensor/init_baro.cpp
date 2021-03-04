#include "engine.h"
#include "Lps25Sensor.h"

#include "rusefi_hw_enums.h"

static Lps25 device;
static Lps25Sensor sensor(device);

void initBaro() {
	// If there's already an external (analog) baro sensor configured,
	// don't configure the internal one.
	if (Sensor::hasSensor(SensorType::BarometricPressure)) {
		return;
	}

#if HW_PROTEUS
	device.init(GPIOB_10, GPIOB_11);
	sensor.Register();
#endif
}

void baroUpdate() {
	sensor.update();
}
