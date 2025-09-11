/**
 * @file	tunerstudio_calibration_channel.h
 * @brief	maintainConstantValue implementation header
 */

#pragma once

void tsCalibrationSetData(TsCalMode mode, float value, float value2 = 0, float timeoutMs = 300);
bool tsCalibrationIsIdle();
void tsCalibrationSetIdle();
