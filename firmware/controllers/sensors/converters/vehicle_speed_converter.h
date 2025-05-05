#include "pch.h"
#include "sensor_converter_func.h"

class VehicleSpeedConverter : public SensorConverter {
public:
	SensorResult convert(float frequency) const override {
          // check where the main VSS reference should be coming from
		if (engineConfiguration->trac_vss_source == VSS_SOURCE_FRONT) { // FRONT, we must be rear wheel drive, but not necessarily
			float ret = Sensor::getOrZero(SensorType::WheelSpeedFrontAvg);
			if (ret > 0) {
				return ret;
			}
		}

		if (engineConfiguration->trac_vss_source == VSS_SOURCE_REAR) { // rear, maybe we're front wheel drive ?
			float ret = Sensor::getOrZero(SensorType::WheelSpeedRearAvg);
			if (ret > 0) {
				return ret;
			}
		}

		if (engineConfiguration->trac_vss_source == VSS_SOURCE_FRONT_REAR_AVG) { // average the front and rear speeds
			float ret = (Sensor::getOrZero(SensorType::WheelSpeedRearAvg) + Sensor::getOrZero(SensorType::WheelSpeedFrontAvg)) / 2;
			if (ret > 0) {
				return ret;
			}
		}
        // do what we did before


        auto vssRevPerKm = engineConfiguration->driveWheelRevPerKm * engineConfiguration->vssGearRatio;

		auto pulsePerKm = (vssRevPerKm * engineConfiguration->vssToothCount);

		if (pulsePerKm == 0) {
			// avoid div by 0
			return 0;
		}

		auto kmPerPulse = 1 / pulsePerKm;

		//     1 pulse       3600 sec      1 km       km
		//    ---------  *  ---------- * --------- = ----
		//       sec           1 hr       1 pulse     hr
		return frequency *     3600    * kmPerPulse;
	}
};
