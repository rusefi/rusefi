#include "pch.h"

#include "defaults.h"
#include "table_helper.h"

#if EFI_ENGINE_CONTROL
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
	setLinearCurve(config->ignitionIatCorrLoadBins, /*from=*/ 0, /*to*/ 140, 1);

	copyArrayInterpolated(config->ignitionIatCorrTempBins, { -40, 0, 10, 20, 30, 40, 50, 60});

	// top rows are the same
	for (size_t i = 0; i < efi::size(config->ignitionIatCorrTable) - 1; i++) {
		//																						40  50  60 deg C
		copyArrayInterpolated(config->ignitionIatCorrTable[i], {0.0, 0.0, 0.0, 0.0, 0.0, -1.0, -2.0, -3.0});
	}

	// last row tapers out
	//																											40  50  60 deg C
	copyArrayInterpolated(config->ignitionIatCorrTable[efi::size(config->ignitionIatCorrTable) - 1], {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, -2.0});
}

static void setDefaultCltTimingCorrection() {
	setLinearCurve(config->ignitionCltCorrLoadBins, /*from=*/ 0, /*to*/ 140, 1);
  setLinearCurve(config->ignitionCltCorrTempBins, -20, 60, 1);

#if CLT_TIMING_TEMP_AXIS_SIZE == 5
	for (size_t i = 0; i < CLT_TIMING_TEMP_AXIS_SIZE; i++) {
	  // huh? use setArrayValues? and we probably get all zeros by default anyway?
		copyArray(config->ignitionCltCorrTable[i], {0.0, 0.0, 0.0, 0.0, 0.0});
	}
#endif
}

static void setDefaultTrailingSparkTable() {
	setLinearCurve(config->trailingSparkLoadBins, 20, 100, 1);
	setRpmTableBin(config->trailingSparkRpmBins);

#if TRAILING_SPARK_SIZE == 4
	for (size_t i = 0; i < TRAILING_SPARK_SIZE; i++) {
		copyArray(config->trailingSparkTable[i], {7,9,10,12});
	}
#endif

}

static float getAdvanceForRpm(float rpm, float advanceMax) {
	if (rpm >= 3000) {
		return advanceMax;
	}

	if (rpm < 600) {
		return 10;
	}

	return interpolateMsg("advance", 600, 10, 3000, advanceMax, rpm);
}

#define round10(x) efiRound(x, 0.1)

float getInitialAdvance(float rpm, float map, float advanceMax) {
	map = std::min(map, 100.0f);
	float advance = getAdvanceForRpm(rpm, advanceMax);

	if (rpm >= 3000)
		return round10(advance + 0.1 * (100 - map));
	return round10(advance + 0.1 * (100 - map) * rpm / 3000);
}

/**
 * this method builds a good-enough base timing advance map bases on a number of heuristics
 */
static void buildTimingMap(float advanceMax) {
	if (engineConfiguration->fuelAlgorithm != engine_load_mode_e::LM_SPEED_DENSITY) {
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

	// Dwell table - a bit conservative but reasonable
	setConstantDwell(4);

	setLinearCurve(config->dwellVoltageCorrVoltBins, 8, 15, 0.1);
	setLinearCurve(config->dwellVoltageCorrValues, 1, 1, 1);

	// Multispark
	setDefaultMultisparkParameters();

	// Ignition advance table
	setLinearCurve(config->ignitionLoadBins, 20, 120, 3);
	setTimingRpmBin(800, 7000);
	buildTimingMap(35);

	setDefaultTrailingSparkTable();

	// CLT correction
	setDefaultCltTimingCorrection();

	// IAT correction
	setDefaultIatTimingCorrection();

	// Give default axes for cylinder trim tables
#if IGN_TRIM_SIZE == 4
	copyArray(config->ignTrimRpmBins, { 1000, 3000, 5000, 7000 });
	copyArray(config->ignTrimLoadBins, { 20, 50, 80, 100 });
#else
  setRpmTableBin(config->ignTrimRpmBins);
  setLinearCurve(config->ignTrimLoadBins, 20, 100);
#endif

	// Default axes for VE blends
	for (size_t i = 0; i < efi::size(config->ignBlends); i++) {
		auto& blend = config->ignBlends[i];
		setLinearCurve(blend.loadBins, 0, 100, 10);
		setLinearCurve(blend.rpmBins, 0, 7000);

		setLinearCurve(blend.blendBins, 0, 100);
		setLinearCurve(blend.blendValues, 0, 100);
	}
}
#endif // EFI_ENGINE_CONTROL
