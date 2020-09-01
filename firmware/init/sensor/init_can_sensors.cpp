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

#define TIMEOUT MS2NT(100)

CanSensor<int16_t, PACK_MULT_PERCENT> canPedalSensor(
	CAN_DEFAULT_BASE + CAN_PEDAL_TPS_OFFSET, /*offset =*/ 0,
	SensorType::AcceleratorPedal, TIMEOUT
);

ObdCanSensor<int16_t, ODB_RPM_MULT> obdCltSensor(
	PID_RPM,
	SensorType::Rpm, TIMEOUT
);

void initCanSensors() {
#if EFI_CANBUS_SLAVE
	registerCanSensor(canPedalSensor);
	registerCanSensor(obdCltSensor);

#endif // EFI_CANBUS_SLAVE
}
#endif // EFI_CAN_SUPPORT
