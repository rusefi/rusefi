#include "pch.h"

#include "init.h"
#include "frequency_sensor.h"
#include "proxy_sensor.h"
#include "vehicle_speed_converter.h"

// 0.05 filter parameter means averaging over ~20 sensor teeth
FrequencySensor vehicleSpeedSensor(SensorType::VehicleSpeed, MS2NT(500));
ProxySensor passthroughSpeedSensor(SensorType::VehicleSpeed);

static VehicleSpeedConverter vehicleSpeedConverter;

void initVehicleSpeedSensor() {
	int parameter = engineConfiguration->vssFilterReciprocal;

	if (parameter < VSS_FILTER_MIN || parameter > VSS_FILTER_MAX) {
		parameter = VSS_FILTER_MIN;
	}

	float filterParameter = 1.0f / parameter;
	if (engineConfiguration->trac_vss_source == VSS_SOURCE_VSS_SENSOR) {
		vehicleSpeedSensor.initIfValid(engineConfiguration->vehicleSpeedSensorInputPin, vehicleSpeedConverter, filterParameter);
    }

	if (engineConfiguration->trac_vss_source == VSS_SOURCE_FRONT) { // FRONT, we must be rear wheel drive, but not necessarily
		passthroughSpeedSensor.setProxiedSensor(SensorType::WheelSpeedFrontAvg);
		passthroughSpeedSensor.Register();
	}

	if (engineConfiguration->trac_vss_source == VSS_SOURCE_REAR) { // rear, maybe we're front wheel drive ?
		passthroughSpeedSensor.setProxiedSensor(SensorType::WheelSpeedRearAvg);
		passthroughSpeedSensor.Register();
    }

	if (engineConfiguration->trac_vss_source == VSS_SOURCE_FRONT_REAR_AVG) { // average the front and rear speeds
		passthroughSpeedSensor.setProxiedSensor(SensorType::WheelSpeedFrontRearAvg);
		passthroughSpeedSensor.Register();
	}
}

void deInitVehicleSpeedSensor() {
	vehicleSpeedSensor.deInit();
}
