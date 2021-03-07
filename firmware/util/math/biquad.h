/*
 * @file biquad.h
 *
 * @date Sep 10, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

class Biquad {
public:
	Biquad();

	float filter(float input);
	void reset();
	void cookSteadyState(float steadyStateInput);

	void configureBandpass(float samplingFrequency, float centerFrequency, float Q);

	// Default Q = 0.54, which is the maximum quality factor without time domain overshoot
	// note that it is less than the maximally flat (frequency domain) Q=0.707, which gives some overshoot
	void configureLowpass(float samplingFrequency, float cutoffFrequency, float Q = 0.54f);

private:
	float a0, a1, a2, b1, b2;
	float z1, z2;
};
