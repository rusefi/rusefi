/**
 * @file	hip9011_lookup.cpp
 * @brief	HIP9011-specific table lookups, the other part of implementation is in HIP9011.cpp
 *
 * @date Jan 4, 2015
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "hip9011_lookup.h"
#include "interpolation.h"

/**
 * These are HIP9011 magic values - integrator time constants in uS
 */
const int integratorValues[INT_LOOKUP_SIZE] = { 40, 45, 50, 55, 60, 65, 70, 75, 80, 90, 100, 110, 120, 130, 140, 150,
		160, 180, 200, 220, 240, 260, 280, 300, 320, 360, 400, 440, 480, 520, 560, 600 };

const float gainLookupInReverseOrder[GAIN_LOOKUP_SIZE] = {
/* 00 */0.111, 0.118, 0.125, 0.129, 0.133, 0.138, 0.143, 0.148,
/* 08 */0.154, 0.160, 0.167, 0.174, 0.182, 0.190, 0.200, 0.211,
/* 16 */0.222, 0.236, 0.250, 0.258, 0.267, 0.276, 0.286, 0.296,
/* 24 */0.308, 0.320, 0.333, 0.348, 0.364, 0.381, 0.400, 0.421,
/* 32 */0.444, 0.471, 0.500, 0.548, 0.567, 0.586, 0.607, 0.630,
/* 40 */0.654, 0.680, 0.708, 0.739, 0.773, 0.810, 0.850, 0.895,
/* 48 */0.944, 1.000, 1.063, 1.143, 1.185, 1.231, 1.280, 1.333,
/* 56 */1.391, 1.455, 1.523, 1.600, 1.684, 1.778, 1.882, 2.0 };

const float bandFreqLookup[BAND_LOOKUP_SIZE] = { 1.22, 1.26, 1.31, 1.35, 1.4, 1.45, 1.51, 1.57, 1.63, 1.71, 1.78,
		1.87, 1.96, 2.07, 2.18, 2.31, 2.46, 2.54, 2.62, 2.71, 2.81, 2.92, 3.03, 3.15, 3.28, 3.43, 3.59, 3.76, 3.95,
		4.16, 4.39, 4.66, 4.95, 5.12, 5.29, 5.48, 5.68, 5.9, 6.12, 6.37, 6.64, 6.94, 7.27, 7.63, 8.02, 8.46, 8.95, 9.5,
		10.12, 10.46, 10.83, 11.22, 11.65, 12.1, 12.6, 13.14, 13.72, 14.36, 15.07, 15.84, 16.71, 17.67, 18.76, 19.98 };


float rpmLookup[INT_LOOKUP_SIZE];

/**
 *
 * We know the set of possible integration times, we know the knock detection window width
 */
void prepareHip9011RpmLookup(float angleWindowWidth) {
	/**
	 * out binary search method needs increasing order thus the reverse order here
	 */
	for (int i = 0; i < INT_LOOKUP_SIZE; i++) {
		rpmLookup[i] = getRpmByAngleWindowAndTimeUs(integratorValues[INT_LOOKUP_SIZE - i - 1], angleWindowWidth);
	}
}


/**
 * 'TC is typically TINT/(2*Pi*VOUT)'
 * Knock Sensor Training TPIC8101, page 24
 */
float getRpmByAngleWindowAndTimeUs(int timeUs, float angleWindowWidth) {
	/**
	 * TINT = TC * 2 * PI * VOUT
	 */
	float integrationTimeUs = timeUs * 2 * PIF * DESIRED_OUTPUT_VALUE;
	/**
	 * rpm = 60 seconds / time
	 * '60000000' because revolutions per MINUTE in uS conversion
	 */
	float windowWidthMult = angleWindowWidth / 360.0f;
	return 60000000.0f / integrationTimeUs * windowWidthMult;
}

int getIntegrationIndexByRpm(float rpm) {
	int i = findIndexMsg("getIbR", rpmLookup, INT_LOOKUP_SIZE, (rpm));
	return i == -1 ? INT_LOOKUP_SIZE - 1 : INT_LOOKUP_SIZE - i - 1;
}

int getHip9011GainIndex(float gain) {
	int i = GAIN_INDEX(gain);
	// GAIN_LOOKUP_SIZE is returned for index which is too low
	return i == GAIN_LOOKUP_SIZE ? GAIN_LOOKUP_SIZE - 1 : i;
}

/**
 * @param frequency knock frequencey, in kHz
 */
int getHip9011BandIndex(float frequency) {
	return findIndexMsg("freq", bandFreqLookup, BAND_LOOKUP_SIZE, frequency);
}

EXTERN_ENGINE;

void initEngineNoiseTable(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setRpmTableBin(engineConfiguration->knockNoiseRpmBins, ENGINE_NOISE_CURVE_SIZE);

	engineConfiguration->knockNoise[0] = 2; // 800
	engineConfiguration->knockNoise[1] = 2; // 1700
	engineConfiguration->knockNoise[2] = 2; // 2600
	engineConfiguration->knockNoise[3] = 2; // 3400
	engineConfiguration->knockNoise[4] = 2; // 4300
	engineConfiguration->knockNoise[5] = 2; // 5200
	engineConfiguration->knockNoise[6] = 2; // 6100
	engineConfiguration->knockNoise[7] = 2; // 7000

}
