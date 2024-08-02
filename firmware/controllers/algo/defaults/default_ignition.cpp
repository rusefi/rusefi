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
#if (IGN_LOAD_COUNT != DEFAULT_IGN_LOAD_COUNT) || (IGN_RPM_COUNT != DEFAULT_IGN_RPM_COUNT)
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
#endif

static void cannedignitionTable() {
#if (IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT) && (IGN_RPM_COUNT == DEFAULT_IGN_RPM_COUNT)
	static const float hardCodedignitionTable[16][16] = {
{12.100,	15.300,	15.000,	17.200,	20.000,	27.000,	30.000,	35.000,	35.000,	35.000,	35.000,	35.000,	35.000,	35.000,	35.000,	35.000,	},
{11.800,	14.900,	15.000,	17.200,	20.000,	27.000,	30.000,	35.000,	35.000,	35.000,	35.000,	35.000,	35.000,	35.000,	35.000,	35.000,	},
{11.600,	14.600,	15.000,	17.200,	20.000,	27.000,	30.000,	35.000,	35.000,	35.000,	35.000,	35.000,	35.000,	35.000,	35.000,	35.000,	},
{10.500,	13.100,	15.200,	17.300,	20.000,	21.000,	30.000,	35.000,	35.000,	35.000,	35.000,	35.000,	35.000,	35.000,	35.000,	35.000,	},
{11.500,	14.100,	16.200,	18.300,	21.000,	22.000,	27.000,	29.900,	30.200,	30.500,	30.800,	31.100,	31.400,	31.700,	31.900,	32.000,	},
{12.500,	15.100,	17.200,	19.300,	22.000,	23.000,	24.000,	24.700,	25.400,	26.000,	26.600,	27.200,	27.700,	28.300,	28.700,	29.000,	},
{11.500,	14.100,	16.200,	18.300,	21.000,	22.000,	23.000,	23.700,	24.400,	25.000,	25.600,	26.200,	26.700,	27.300,	27.700,	28.000,	},
{10.500,	13.100,	15.200,	17.300,	20.000,	21.000,	22.000,	22.700,	23.400,	24.000,	24.600,	25.200,	25.400,	26.300,	26.700,	27.000,	},
{8.400,	11.000,	13.100,	15.200,	17.900,	18.900,	19.900,	20.600,	21.300,	21.900,	22.500,	23.100,	23.600,	24.200,	24.600,	24.900,	},
{6.300,	8.900,	11.000,	13.100,	15.800,	16.800,	17.800,	18.500,	19.200,	19.800,	20.400,	21.000,	21.500,	22.100,	22.500,	22.800,	},
{4.100,	6.700,	8.800,	10.900,	13.600,	14.600,	15.600,	16.300,	17.000,	17.600,	18.200,	18.800,	19.300,	19.900,	20.300,	20.600,	},
{2.000,	4.600,	6.700,	8.800,	11.500,	12.500,	13.500,	14.200,	14.900,	15.500,	16.100,	16.700,	17.200,	17.800,	18.200,	18.500,	},
{-0.100,	2.500,	4.600,	6.700,	9.400,	10.400,	11.400,	12.100,	12.800,	13.400,	14.000,	14.600,	15.100,	15.700,	16.100,	16.400,	},
{-2.200,	0.400,	2.500,	4.600,	7.300,	8.300,	9.300,	10.000,	10.700,	11.300,	11.900,	12.500,	13.000,	13.600,	14.000,	14.300,	},
{-4.400,	-1.800,	0.300,	2.400,	5.100,	6.100,	7.100,	7.800,	8.500,	9.100,	9.700,	10.300,	10.800,	11.400,	11.800,	12.100,	},
{-6.500,	-3.900,	-1.800,	0.300,	3.000,	4.000,	5.000,	5.700,	6.400,	7.000,	7.600,	8.200,	8.700,	9.300,	9.700,	10.000,	},
};
	copyTable(config->ignitionTable, hardCodedignitionTable);
#else
  buildTimingMap(35);
#endif
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
	cannedignitionTable();

	engineConfiguration->trailingSparkAngle = 10;

	// CLT correction
	setLinearCurve(config->cltTimingBins, CLT_CURVE_RANGE_FROM, 120, 1);
	setArrayValues(config->cltTimingExtra, 0.0f);

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
