/**
 * @file	init_can_sensors.cpp
 *
 * @date March 31, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#include "global.h"

#if EFI_CAN_SUPPORT
#include "can_sensor.h"
#include "can.h"

CanSensor<int16_t, PACK_MULT_PERCENT> canPedalSensor(
	CAN_DEFAULT_BASE + CAN_PEDAL_TPS_OFFSET, /*offset =*/ 0,
	SensorType::AcceleratorPedal, MS2NT(100)
);

void initCanSensors() {
#if EFI_CANBUS_SLAVE
	registerCanSensor(canPedalSensor);

	if (!canPedalSensor.Register()) {
		firmwareError(CUSTOM_INVALID_TPS_SETTING, "Duplicate registration for pedal sensor");
	}
#endif // EFI_CANBUS_SLAVE
}
#endif // EFI_CAN_SUPPORT
