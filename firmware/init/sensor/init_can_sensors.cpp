/**
 * @file	init_can_sensors.cpp
 *
 * @date March 31, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#include "pin_repository.h"
#include "engine_configuration.h"

#if EFI_CAN_SUPPORT
#include "can_sensor.h"
#include "can.h"

CanSensor<int16_t, PACK_MULT_PERCENT> canPedalSensor(
	CAN_DEFAULT_BASE + CAN_PEDAL_TPS_OFFSET, /*offset =*/ 0,
	SensorType::AcceleratorPedal, CAN_TIMEOUT
);

ObdCanSensor<2, 0> obdRpmSensor(
		PID_RPM, ODB_RPM_MULT,
	SensorType::Rpm
);

ObdCanSensor<1, ODB_TEMP_EXTRA> obdCltSensor(
		PID_COOLANT_TEMP, 1,
	SensorType::Clt
);

ObdCanSensor<1, ODB_TEMP_EXTRA> obdIatSensor(
		PID_INTAKE_TEMP, 1,
	SensorType::Iat
);

ObdCanSensor<1, 0> obdTpsSensor(
		PID_INTAKE_TEMP, ODB_TPS_BYTE_PERCENT,
	SensorType::Tps1
);

//ObdCanSensor<1, ODB_TPS_BYTE_PERCENT> obdTpsSensor(
//		PID_ENGINE_LOAD,
//	SensorType::Tps, TIMEOUT
//);

void initCanSensors() {
	if (CONFIG(consumeObdSensors)) {
//		registerCanSensor(canPedalSensor);
		registerCanSensor(obdRpmSensor);
		registerCanSensor(obdCltSensor);
		registerCanSensor(obdIatSensor);
		registerCanSensor(obdTpsSensor);
	}
}
#endif // EFI_CAN_SUPPORT
