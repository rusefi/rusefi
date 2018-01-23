/**
 * @file	EfiWave.cpp
 *
 * @date May 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"
#include "EfiWave.h"
#include "trigger_structure.h"

single_wave_s::single_wave_s() {
	init(NULL);
}

single_wave_s::single_wave_s(pin_state_t *ps) {
	init(ps);
}

void single_wave_s::init(pin_state_t *pinStates) {
	this->pinStates = pinStates;
}

void multi_wave_s::baseConstructor() {
	waves = NULL;
	switchTimes = NULL;
	reset();
}

multi_wave_s::multi_wave_s() {
	baseConstructor();
}

multi_wave_s::multi_wave_s(float *switchTimes, single_wave_s *waves) {
	baseConstructor();
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
	if (switchTimes[size - 1] != 1) {
		firmwareError(CUSTOM_ERR_WAVE_1, "last switch time has to be 1 not %.2f", switchTimes[size - 1]);
		return;
	}
	for (int i = 0; i < size - 1; i++) {
		if (switchTimes[i] >= switchTimes[i + 1]) {
			firmwareError(CUSTOM_ERR_WAVE_2, "invalid switchTimes @%d: %.2f/%.2f", i, switchTimes[i], switchTimes[i + 1]);
		}
	}
}
