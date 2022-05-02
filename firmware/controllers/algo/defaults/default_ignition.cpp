#include "pch.h"

#include "defaults.h"
#include "table_helper.h"

static void setDefaultMultisparkParameters() {
	// 1ms spark + 2ms dwell
	engineConfiguration->multisparkSparkDuration = 1;
	engineConfiguration->multisparkDwell = 2;

	// Conservative defaults - probably won't blow up coils
	engineConfiguration->multisparkMaxRpm = 1500;
	engineConfiguration->multisparkMaxExtraSparkCount = 2;
	engineConfiguration->multisparkMaxSparkingAngle = 30;
}

static constexpr float iatTimingRpmBins[] = { 880, 1260, 1640, 2020, 2400, 2780, 3000, 3380, 3760, 4140, 4520, 5000, 5700, 6500, 7200, 8000 };

static void setDefaultIatTimingCorrection() {
	setLinearCurve(config->ignitionIatCorrLoadBins, /*from*/CLT_CURVE_RANGE_FROM, 110, 1);
#if IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT
	copyArray(config->ignitionIatCorrRpmBins, iatTimingRpmBins);

	static constexpr int8_t defaultIatCorr[16] = {
		4,	// -40 deg
		4,
		3,
		2,
		0,	// 0 deg
		0,
		0,
		0,
		0,
		-1,	// 50 deg
		-2,
		-4,
		-4,
		-4,
		-4,
		-4,	// 110 deg
	};

	// Set each row of the table to the same value (no rpm dependence by default)
	for (size_t i = 0; i < efi::size(defaultIatCorr); i++) {
		setArrayValues(config->ignitionIatCorrTable[i], defaultIatCorr[i]);
	}
#else
	setLinearCurve(config->ignitionIatCorrLoadBins, /*from*/0, 6000, 1);
#endif /* IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT */
}

static float getAdvanceForRpm(int rpm, float advanceMax) {
        if (rpm >= 3000)
            return advanceMax;
        if (rpm < 600)
            return 10;
       return interpolateMsg("advance", 600, 10, 3000, advanceMax, rpm);
}

#define round10(x) efiRound(x, 0.1)

float getInitialAdvance(int rpm, float map, float advanceMax) {
	map = minF(map, 100);
	float advance = getAdvanceForRpm(rpm, advanceMax);

	if (rpm >= 3000)
		return round10(advance + 0.1 * (100 - map));
	return round10(advance + 0.1 * (100 - map) * rpm / 3000);
}

/**
 * this method builds a good-enough base timing advance map bases on a number of heuristics
 */
static void buildTimingMap(float advanceMax) {
	if (engineConfiguration->fuelAlgorithm != LM_SPEED_DENSITY) {
		warning(CUSTOM_WRONG_ALGORITHM, "wrong algorithm for MAP-based timing");
		return;
	}
	/**
	 * good enough (but do not trust us!) default timing map in case of MAP-based engine load
	 */
	for (int loadIndex = 0; loadIndex < IGN_LOAD_COUNT; loadIndex++) {
		float load = config->ignitionLoadBins[loadIndex];
		for (int rpmIndex = 0;rpmIndex<IGN_RPM_COUNT;rpmIndex++) {
			float rpm = config->ignitionRpmBins[rpmIndex];
			config->ignitionTable[loadIndex][rpmIndex] = getInitialAdvance(rpm, load, advanceMax);
		}
	}
}

void setDefaultIgnition() {
	// Ignition base settings
	engineConfiguration->isIgnitionEnabled = true;

	engineConfiguration->timingMode = TM_DYNAMIC;
	engineConfiguration->fixedModeTiming = 50;

	// Dwell table
	setConstantDwell(4);

	setLinearCurve(engineConfiguration->dwellVoltageCorrVoltBins, 8, 15, 0.1);
	setLinearCurve(engineConfiguration->dwellVoltageCorrValues, 1, 1, 1);

	// Multispark
	setDefaultMultisparkParameters();

	// Ignition advance table
	setLinearCurve(config->ignitionLoadBins, 20, 120, 3);
	setTimingRpmBin(800, 7000);
	buildTimingMap(35);

	engineConfiguration->trailingSparkAngle = 10;

	// CLT correction
	setLinearCurve(config->cltTimingBins, CLT_CURVE_RANGE_FROM, 120, 1);
	setArrayValues(config->cltTimingExtra, 0.0f);

	// IAT correction
	setDefaultIatTimingCorrection();
}
