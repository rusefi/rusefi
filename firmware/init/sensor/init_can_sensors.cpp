
#include "can_sensor.h"
#include "can.h"

CanSensor<int16_t, PACK_MULT_PERCENT> canPedalSensor(
	CAN_DEFAULT_BASE + CAN_PEDAL_TPS_OFFSET, /*offset =*/ 0,
	SensorType::AcceleratorPedal, MS2NT(100)
);

void registerCanSensor(CanSensorBase& sensor);

void initCanSensors() {
	registerCanSensor(canPedalSensor);
	(void)canPedalSensor.Register();
}
