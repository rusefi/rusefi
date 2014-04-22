/*
 * wave_math.c
 *
 * @date Dec 25, 2012
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "wave_math.h"

//#define LOOPS_TIME(value, divider) (((int)((value) / (divider))) * (divider))

float floatmod(float value, float divider) {
	// number of full cycles
	int loopsNumber = (int)(value / divider);
	// total time of full cycles
	float loopsTime = loopsNumber * divider;

	return value - loopsTime;
}

/**
 * @param waveLen length of each wave, in ticks
 */
int waveState(time_t start, time_t now, float waveLen, float leftSide,
		float rightSide) {
	/**
	 * number of ticks since start of thread
	 */
	float time = now - start;
	float mod = floatmod(time, waveLen);
	return mod >= waveLen * leftSide && mod < waveLen * rightSide;
}
