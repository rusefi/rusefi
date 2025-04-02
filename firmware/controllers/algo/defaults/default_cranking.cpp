#include "pch.h"

#include "defaults.h"
#include "table_helper.h"

void setDefaultCranking() {
	engineConfiguration->cranking.rpm = 550;

	// Fuel
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->cranking.baseFuel = 27;

	// Ignition
	engineConfiguration->ignitionDwellForCrankingMs = DEFAULT_CRANKING_DWELL_MS;
	engineConfiguration->crankingTimingAngle = DEFAULT_CRANKING_ANGLE;

	// IAC
	setArrayValues(config->cltCrankingCorr, 50);
	// should be 100 once tune is better
	engineConfiguration->afterCrankingIACtaperDuration = 200;

	engineConfiguration->isFasterEngineSpinUpEnabled = true;

	// After start enrichment
#if !EFI_UNIT_TEST
	// don't set this for unit tests, as it makes things more complicated to test
	static const int16_t defaultPostCrankingCLTBins[] = {
		-20, 0, 20, 40, 60, 80
	};
	static const uint16_t defaultPostCrankingDurationBins[] = {
		0, 15, 35, 65, 100, 150
	};
	copyArray(config->postCrankingCLTBins, defaultPostCrankingCLTBins);
	copyArray(config->postCrankingDurationBins, defaultPostCrankingDurationBins);
	setTable(config->postCrankingFactor, 1.2f);
#endif

	setLinearCurve(config->crankingTpsCoef, /*from*/1, /*to*/1, 1);
	setLinearCurve(config->crankingTpsBins, 0, 100, 1);

	setLinearCurve(config->cltCrankingCorrBins, CLT_CURVE_RANGE_FROM, 100, 1);
	setLinearCurve(config->cltCrankingCorr, 50, 50, 1); // now as % of idle valve/etb

	// Cranking temperature compensation
	static const float crankingCoef[] = {
		2.8,
		2.2,
		1.8,
		1.55,
		1.3,
		1.1,
		1.0,
		1.0
	};
	copyArray(config->crankingFuelCoef,     crankingCoef);
	copyArray(config->crankingFuelCoefE100, crankingCoef);

	// Deg C
	static const float crankingBins[] = {
		-20,
		-10,
		5,
		20,
		35,
		50,
		65,
		90
	};
	copyArray(config->crankingFuelBins, crankingBins);

	// Cranking cycle compensation

	// Whole table is 1.0, except first two columns which are steeper
	setTable(config->crankingCycleFuelCoef, 1.0f);
	for (int cltIndex = 0;cltIndex<CRANKING_CYCLE_CLT_SIZE;cltIndex++) {
	  // kludge: we have a few unit tests which depend on these magic numbers
	  config->crankingCycleFuelCoef[cltIndex][0] = 2;
	  config->crankingCycleFuelCoef[cltIndex][/*x - cycles*/1] = 1.3f;
	}
	setLinearCurve(config->crankingCycleFuelCltBins, 0, 60, 1);

	// X values are simply counting up cycle number starting at 1
	for (size_t i = 0; i < efi::size(config->crankingCycleBins); i++) {
		config->crankingCycleBins[i] = i + 1;
	}

	// Cranking ignition timing
	setArrayValues(config->crankingAdvance, 0);

	static const float advanceBins[] = { 0, 200, 400, 1000 };
	copyArray(config->crankingAdvanceBins, advanceBins);

	engineConfiguration->useTLE8888_cranking_hack = true;
}
