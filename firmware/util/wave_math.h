/*
 * @file wave_math.h
 * TODO: this file needs a better name, maybe the methods shoould be moved somewhere?
 *
 *
 * @date Dec 25, 2012
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef WAVE_MATH_H_
#define WAVE_MATH_H_

#include "global.h"

// number of milliseconds in one period of given frequency (per second)
#define frequency2period(freq) (((float)1000) / (freq))

float floatmod(float value, float divider);
int waveState(time_t start, time_t now, float waveLen, float leftSide,
		float rightSide);

#endif /* WAVE_MATH_H_ */
