/*
 * @file engine_parts.h
 *
 * @date May 27, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "static_vector.h"
#include <rusefi/timer.h>

#define MOCK_ADC_SIZE 26

struct Accelerometer {
	float lat = 0; // G value
	float lon = 0;
	float vert = 0;
	float yawRate = 0;
};

struct SensorsState {
	Accelerometer accelerometer;
};

class TransmissionState {
public:
	gear_e gearSelectorPosition;
};

struct multispark_state
{
	efidur_t delay = 0;
	efidur_t dwell = 0;
	uint8_t count = 0;
};
