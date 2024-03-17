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

static void setDefaultIatTimingCorrection() {
	copyArray(config->ignitionIatCorrTempBins, { -40, 0, 10, 20, 30, 40, 50, 60});
	setLinearCurve(config->ignitionIatCorrLoadBins, /*from=*/ 0, /*to*/ 140, 1);

	// top 5 rows are the same
	for (size_t i = 3; i < 8; i++) {
		//                                                         40  50  60 deg C
		copyArray(config->ignitionIatCorrTable[i], {0, 0, 0, 0, 0, -1, -2, -3});
	}

	// 6th row tapers out
	//                                                        40  50  60 deg C
	copyArray(config->ignitionIatCorrTable[2], {0, 0, 0, 0, 0, 0, -1, -2});
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
		warning(ObdCode::CUSTOM_WRONG_ALGORITHM, "wrong algorithm for MAP-based timing");
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

	engineConfiguration->minimumIgnitionTiming = -10;
	engineConfiguration->maximumIgnitionTiming = 60;

	// Dwell table
	setConstantDwell(4);

	setLinearCurve(config->dwellVoltageCorrVoltBins, 8, 15, 0.1);
	setLinearCurve(config->dwellVoltageCorrValues, 1, 1, 1);

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

	// Give default axes for cylinder trim tables
	copyArray(config->ignTrimRpmBins, { 1000, 3000, 5000, 7000 });
	copyArray(config->ignTrimLoadBins, { 20, 50, 80, 100 });

	// Default axes for VE blends
	for (size_t i = 0; i < efi::size(config->ignBlends); i++) {
		auto& blend = config->ignBlends[i];
		setLinearCurve(blend.loadBins, 0, 100, 10);
		setLinearCurve(blend.rpmBins, 0, 7000);

		setLinearCurve(blend.blendBins, 0, 100);
		setLinearCurve(blend.blendValues, 0, 100);
	}
}
