/**
 * @file	EfiWave.cpp
 *
 * @date May 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "EfiWave.h"
#include "trigger_structure.h"

single_wave_s::single_wave_s() {
	init(NULL);
}

single_wave_s::single_wave_s(int *ps) {
	init(ps);
}

void single_wave_s::init(int *pinStates) {
	this->pinStates = pinStates;
}

multi_wave_s::multi_wave_s() {
}

multi_wave_s::multi_wave_s(float *switchTimes, single_wave_s *waves) {
	init(switchTimes, waves);
}

void multi_wave_s::init(float *switchTimes, single_wave_s *waves) {
	this->switchTimes = switchTimes;
	this->waves = waves;
}

void multi_wave_s::reset(void) {
	waveCount = 0;
}

float multi_wave_s::getAngle(int index, engine_configuration_s const *engineConfiguration, trigger_shape_s * s) const {
	if (getOperationMode(engineConfiguration) == FOUR_STROKE_CAM_SENSOR)
		return getSwitchTime(index) * 720.0;
	/**
	 * FOUR_STROKE_CRANK_SENSOR magic:
	 * We have two crank shaft revolutions for each engine cycle
	 * See also trigger_central.cpp
	 * See also getEngineCycleEventCount()
	 */
	int triggerEventCounter = s->getSize();

	if (index < triggerEventCounter) {
		return getSwitchTime(index) * 360.0;
	} else {
		return 360 + getSwitchTime(index - triggerEventCounter) * 360.0;
	}
}

float multi_wave_s::getSwitchTime(int index) const {
	return switchTimes[index];
}

void checkSwitchTimes2(int size, float *switchTimes) {
	for (int i = 0; i < size - 1; i++) {
		if (switchTimes[i] >= switchTimes[i + 1]) {
			firmwareError("invalid switchTimes");
		}
	}
}
