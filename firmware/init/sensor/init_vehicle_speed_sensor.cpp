#include "pch.h"

#include "init.h"
#include "frequency_sensor.h"

// 0.05 filter parameter means averaging over ~20 sensor teeth
static FrequencySensor vehicleSpeedSensor(SensorType::VehicleSpeed, MS2NT(500), 0.05f);

void initVehicleSpeedSensor() {

	auto pin = engineConfiguration->vehicleSpeedSensorInputPin;

	// Nothing to do if no sensor configured
	if (!isBrainPinValid(pin)) {
		return;
	}

	class : public SensorConverter {
	public:
		SensorResult convert(float frequency) const override {
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
	} converter;


	vehicleSpeedSensor.setFunction(converter);
	vehicleSpeedSensor.init(pin);
	vehicleSpeedSensor.Register();
}

void deInitVehicleSpeedSensor() {
	vehicleSpeedSensor.deInit();
}
