/**
 * @file vehicle_speed.cpp
 *
 * @date Dec 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "vehicle_speed.h"
#include "pch.h"

#if EFI_VEHICLE_SPEED

#include "digital_input_icu.h"
#include "digital_input_exti.h"
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

// todo: make this method public and invoke this method from test
void vsCallback(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engine->engineState.vssEventCounter++;
	efitick_t nowNt = getTimeNowNt();
	engine->vssDiff = nowNt - engine->vssLastSignalTimeNt;
	engine->vssLastSignalTimeNt = nowNt;
}

#if ! EFI_UNIT_TEST

static void vsAnaWidthCallback() {
	vsCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
}

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

bool hasVehicleSpeedSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return (isBrainPinValid(CONFIG(vehicleSpeedSensorInputPin)));
}

#if HAL_VSS_USE_PAL
static void vsExtiCallback(void *) {
	vsAnaWidthCallback();
}
#endif /* HAL_VSS_USE_PAL */

void stopVSSPins(void) {
#if HAL_VSS_USE_PAL
	efiExtiDisablePin(activeConfiguration.vehicleSpeedSensorInputPin);
#elif HAL_USE_ICU
	stopDigitalCapture("VSS", activeConfiguration.vehicleSpeedSensorInputPin);
#endif /* HAL_VSS_USE_PAL, HAL_USE_ICU */
}

#if ! EFI_UNIT_TEST

void startVSSPins(void) {
	if (!hasVehicleSpeedSensor()) {
		return;
	}

// todo: refactor https://github.com/rusefi/rusefi/issues/2123
#if HAL_VSS_USE_PAL
	efiExtiEnablePin("VSS", CONFIG(vehicleSpeedSensorInputPin), PAL_EVENT_MODE_BOTH_EDGES, vsExtiCallback, nullptr);
#elif HAL_USE_ICU
	digital_input_s* vehicleSpeedInput = startDigitalCapture("VSS", CONFIG(vehicleSpeedSensorInputPin));

	vehicleSpeedInput->widthListeners.registerCallback((VoidInt) vsAnaWidthCallback, nullptr);
#else
	#error "HAL_USE_ICU or HAL_VSS_USE_PAL should be enabled to use EFI_VEHICLE_SPEED"
#endif /* HAL_VSS_USE_PAL, HAL_USE_ICU */
}
#endif

void initVehicleSpeed() {
#if ! EFI_UNIT_TEST
	addConsoleAction("speedinfo", speedInfo);
	startVSSPins();
#endif // EFI_UNIT_TEST
}
#else  /* EFI_VEHICLE_SPEED */

#if EFI_UNIT_TEST

float getVehicleSpeed(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	
	// Mock return to be used in unit tests
	return mockVehicleSpeed;	
}
#else
float getVehicleSpeed(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	
	// no VSS support
	return 0;	
}
#endif /* EFI_UNIT_TEST */
#endif /* EFI_VEHICLE_SPEED */




void setMockVehicleSpeed(float speedKPH DECLARE_ENGINE_PARAMETER_SUFFIX) {
	engine->mockVehicleSpeed = speedKPH;
}

