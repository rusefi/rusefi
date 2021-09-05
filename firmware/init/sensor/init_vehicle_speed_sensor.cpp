#include "pch.h"

#include "init.h"
#include "frequency_sensor.h"
#include "vehicle_speed_converter.h"

static FrequencySensor vehicleSpeedSensor(SensorType::VehicleSpeed, MS2NT(500));
static VehicleSpeedConverter vehicleSpeedConverter;


void initVehicleSpeedSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	INJECT_ENGINE_REFERENCE(&vehicleSpeedConverter);

	auto pin = CONFIG(vehicleSpeedSensorInputPin);

	// Nothing to do if no sensor configured
	if (!isBrainPinValid(pin)) {

#if EFI_CAN_SUPPORT
		//check if we do have can vss
		if (CONFIG(enableCanVss))
		{
			//can init
			vehicleSpeedSensor.setFunction(vehicleSpeedConverter);
			vehicleSpeedSensor.Register();
		} 
		else 
#endif		
		{
			return;
		}
	} else {

		vehicleSpeedSensor.setFunction(vehicleSpeedConverter);
		vehicleSpeedSensor.init(pin, "vss");
		vehicleSpeedSensor.Register();
	}
}

void deInitVehicleSpeedSensor() {
	vehicleSpeedSensor.deInit();
}