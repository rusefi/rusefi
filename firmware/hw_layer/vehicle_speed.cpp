/**
 * @file vehicle_speed.cpp
 *
 * @date Dec 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#if EFI_VEHICLE_SPEED

#include "can_vss.h"

/**
 * @return vehicle speed, in kilometers per hour
 */
float getVehicleSpeed(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (engine->mockVehicleSpeed != DEFAULT_MOCK_SPEED)
		return engine->mockVehicleSpeed;
#if EFI_CAN_SUPPORT
	if (CONFIG(enableCanVss)) {
		return getVehicleCanSpeed();
	}
#endif	/* EFI_CAN_SUPPORT */	
	if (!hasVehicleSpeedSensor(PASS_ENGINE_PARAMETER_SIGNATURE))
		return 0;
	efitick_t nowNt = getTimeNowNt();
	if (nowNt - engine->vssLastSignalTimeNt > NT_PER_SECOND)
		return 0; // previous signal time is too long ago - we are stopped

	return engineConfiguration->vehicleSpeedCoef * NT_PER_SECOND / engine->vssDiff;
}

#if ! EFI_UNIT_TEST

static void speedInfo(void) {
	efiPrintf("VSS input at %s",
			hwPortname(CONFIG(vehicleSpeedSensorInputPin)));

	efiPrintf("c=%.2f eventCounter=%d speed=%.2f",
			engineConfiguration->vehicleSpeedCoef,
			engine->engineState.vssEventCounter,
			getVehicleSpeed(PASS_ENGINE_PARAMETER_SIGNATURE));
	efiPrintf("vss diff %d", engine->vssDiff);
}
#endif // EFI_UNIT_TEST
#endif /* EFI_VEHICLE_SPEED */
