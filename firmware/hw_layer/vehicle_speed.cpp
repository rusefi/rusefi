/**
 * @file vehicle_speed.cpp
 *
 * @date Dec 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "vehicle_speed.h"

#if EFI_VEHICLE_SPEED || defined(__DOXYGEN__)

#include "engine.h"
#include "wave_analyzer_hw.h"
#include "pin_repository.h"

EXTERN_ENGINE
;

static Logging *logger;

static WaveReaderHw vehicleSpeedInput;

static uint64_t lastSignalTimeNt = 0;
static uint64_t vssDiff = 0;
static int vssCounter = 0;

/**
 * @return vehicle speed, in kilometers per hour
 */
float getVehicleSpeed(void) {
	uint64_t nowNt = getTimeNowNt();
	if (nowNt - lastSignalTimeNt > US2NT(US_PER_SECOND_LL))
		return 0; // previous signal time is too long ago - we are stopped

	return engineConfiguration->vehicleSpeedCoef * US2NT(US_PER_SECOND_LL) / vssDiff;
}

static void vsAnaWidthCallback(void) {
	vssCounter++;
	uint64_t nowNt = getTimeNowNt();
	vssDiff = nowNt - lastSignalTimeNt;
	lastSignalTimeNt = nowNt;
}

static void speedInfo(void) {
	scheduleMsg(logger, "VSS@%s c=%f eventCounter=%d speed=%f",
			hwPortname(boardConfiguration->vehicleSpeedSensorInputPin),
			engineConfiguration->vehicleSpeedCoef,
			vssCounter,
			getVehicleSpeed());
}

void initVehicleSpeed(Logging *l) {
	logger = l;
	if (boardConfiguration->vehicleSpeedSensorInputPin == GPIO_UNASSIGNED)
		return;
	initWaveAnalyzerDriver(&vehicleSpeedInput, boardConfiguration->vehicleSpeedSensorInputPin);
	startInputDriver(&vehicleSpeedInput, true);

	vehicleSpeedInput.widthListeners.registerCallback((VoidInt) vsAnaWidthCallback, NULL);
	addConsoleAction("speedinfo", speedInfo);
}

#endif /* EFI_VEHICLE_SPEED */
