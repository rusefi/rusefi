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

float multi_wave_s::getSwitchTime(int index) const {
	return switchTimes[index];
}

void checkSwitchTimes2(int size, float *switchTimes) {
	for (int i = 0; i < size - 1; i++) {
		if (switchTimes[i] >= switchTimes[i + 1]) {
			firmwareError("invalid switchTimes @%d: %f/%f", i, switchTimes[i], switchTimes[i + 1]);
		}
	}
}
