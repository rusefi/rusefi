#include "pch.h"
#include "sensor_converter_func.h"

class VehicleSpeedConverter : public SensorConverter {
public:
	DECLARE_ENGINE_PTR;

	SensorResult convert(float frequency) const override {
		auto vssRevPerKm = CONFIG(driveWheelRevPerKm) * CONFIG(vssGearRatio);

		auto pulsePerKm = (vssRevPerKm * CONFIG(vssToothCount));

		if (pulsePerKm == 0) {
			// avoid div by 0
			return 0;
		}

		auto kmPerPulse = 1 / pulsePerKm;

		//     1 pulse       3600 sec      1 km       km
		//    ---------  *  ---------- * --------- = -----
		//       sec           1 hr       1 pulse     sec
		return frequency *     3600    * kmPerPulse;
	}
};
