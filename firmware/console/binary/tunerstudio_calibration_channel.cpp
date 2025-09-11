/**
 * @file	tunerstudio_calibration_channel.cpp
 * @brief	maintainConstantValue implementation
 */

// See TS ini file specification
// See output_channels.txt for calibrationValue and calibrationMode
// See rusefi_enum.h for TsCalMode

#include "pch.h"

#include "tunerstudio_calibration_channel.h"

/*
 * TODO:
 * extract to livedata module
 * avoid TsCalMode casts
 */

static Timer tsCalibrationTimer;
static float tsCalibrationTimeout;

void tsCalibrationSetData(TsCalMode mode, float value, float value2, float timeoutMs) {
	// TODO: do under lock!
	engine->outputChannels.calibrationMode = (uint8_t)mode;
	engine->outputChannels.calibrationValue = value;
	engine->outputChannels.calibrationValue2 = value2;

	tsCalibrationTimeout = timeoutMs;
	tsCalibrationTimer.reset();
}

bool tsCalibrationIsIdle() {
	if ((engine->outputChannels.calibrationMode != (uint8_t)TsCalMode::None) &&
		(tsCalibrationTimer.hasElapsedMs(tsCalibrationTimeout))) {
		engine->outputChannels.calibrationMode = (uint8_t)TsCalMode::None;
	}

	return (engine->outputChannels.calibrationMode == (uint8_t)TsCalMode::None);
}

void tsCalibrationSetIdle()
{
	engine->outputChannels.calibrationMode = (uint8_t)TsCalMode::None;
}
