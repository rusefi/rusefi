#include "pch.h"

#include "defaults.h"
#include "table_helper.h"

static void setDefaultMultisparkParameters(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	// 1ms spark + 2ms dwell
	engineConfiguration->multisparkSparkDuration = 1000;
	engineConfiguration->multisparkDwell = 2000;

	// Conservative defaults - probably won't blow up coils
	engineConfiguration->multisparkMaxRpm = 1500;
	engineConfiguration->multisparkMaxExtraSparkCount = 2;
	engineConfiguration->multisparkMaxSparkingAngle = 30;
}

static constexpr float iatTimingRpmBins[] = { 880, 1260, 1640, 2020, 2400, 2780, 3000, 3380, 3760, 4140, 4520, 5000, 5700, 6500, 7200, 8000 };

static void setDefaultIatTimingCorrection(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
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
		setArrayValues(config->ignitionIatCorrTable[i], (float)defaultIatCorr[i]);
	}
#else
	setLinearCurve(config->ignitionIatCorrLoadBins, /*from*/0, 6000, 1);
#endif /* IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT */
}

void setDefaultIgnition(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	// Ignition base settings
	CONFIG(isIgnitionEnabled) = true;

	engineConfiguration->timingMode = TM_DYNAMIC;
	engineConfiguration->fixedModeTiming = 50;

	// Dwell table
	setConstantDwell(4 PASS_CONFIG_PARAMETER_SUFFIX);

	// Multispark
	setDefaultMultisparkParameters(PASS_CONFIG_PARAMETER_SIGNATURE);

	// Ignition advance table
	// TODO: populate some values that aren't all 0?
	setTimingLoadBin(1.2, 4.4 PASS_CONFIG_PARAMETER_SUFFIX);
	setTimingRpmBin(800, 7000 PASS_CONFIG_PARAMETER_SUFFIX);

	engineConfiguration->trailingSparkAngle = 10;

	// CLT correction
	setLinearCurve(engineConfiguration->cltTimingBins, CLT_CURVE_RANGE_FROM, 120, 1);
	setArrayValues(engineConfiguration->cltTimingExtra, 0.0f);

	// IAT correction
	setDefaultIatTimingCorrection(PASS_CONFIG_PARAMETER_SIGNATURE);
}
