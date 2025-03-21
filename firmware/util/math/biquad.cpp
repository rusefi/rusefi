/*
 * @file biquad.cpp
 *
 * @date Sep 10, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "biquad.h"

Biquad::Biquad() {
// Default to passthru
	a0 = 1;
	a1 = a2 = b1 = b2 = 0;

	reset();
}

void Biquad::reset() {
	z1 = z2 = 0;
}

static float getK(float samplingFrequency, float cutoff) {
	return tanf_taylor(CONST_PI * cutoff / samplingFrequency);
}

static float getNorm(float K, float Q) {
	return 1 / (1 + K / Q + K * K);
}

void Biquad::configureBandpass(float samplingFrequency, float centerFrequency, float Q) {
  if (samplingFrequency < 2.5f * centerFrequency) {
    criticalError("Invalid biquad parameters %f %f", samplingFrequency, centerFrequency);
    return;
  }

	float K = getK(samplingFrequency, centerFrequency);
	float norm = getNorm(K, Q);

	a0 = K / Q * norm;
	a1 = 0;
	a2 = -a0;
	b1 = 2 * (K * K - 1) * norm;
	b2 = (1 - K / Q + K * K) * norm;
}

void Biquad::configureLowpass(float samplingFrequency, float cutoffFrequency, float Q) {
	criticalAssertVoid(samplingFrequency >= 2.5f * cutoffFrequency, "Invalid biquad parameters");

	float K = getK(samplingFrequency, cutoffFrequency);
	float norm = getNorm(K, Q);

	a0 = K * K * norm;
	a1 = 2 * a0;
	a2 = a0;
	b1 = 2 * (K * K - 1) * norm;
	b2 = (1 - K / Q + K * K) * norm;
}

void Biquad::configureHighpass(float samplingFrequency, float cutoffFrequency, float Q) {
	criticalAssertVoid(samplingFrequency >= 2.5f * cutoffFrequency, "Invalid biquad parameters");

	float K = getK(samplingFrequency, cutoffFrequency);
	float norm = getNorm(K, Q);

	a0 = 1 * norm;
	a1 = -2 * a0;
	a2 = a0;
	b1 = 2 * (K * K - 1) * norm;
	b2 = (1 - K / Q + K * K) * norm;
}

float Biquad::filter(float input) {
	float result = input * a0 + z1;
	if (engineConfiguration->verboseQuad) {
		efiPrintf("input %f, a0 %f, z1 %f, result %f", input, a0, z1, result);
	}
	z1 = input * a1 + z2 - b1 * result;
	z2 = input * a2 - b2 * result;
	return result;
}

void Biquad::cookSteadyState(float steadyStateInput) {
    float Y = steadyStateInput * (a0 + a1 + a2) / (1 + b1 + b2);

    float steady_z2 = steadyStateInput * a2 - Y * b2;
    float steady_z1 = steady_z2 + steadyStateInput * a1 - Y * b1;

    this->z1 = steady_z1;
    this->z2 = steady_z2;
}
