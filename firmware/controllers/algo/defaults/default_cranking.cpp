#include "pch.h"

#include "defaults.h"
#include "table_helper.h"

void setDefaultCranking() {
	engineConfiguration->cranking.rpm = 550;

	// Fuel
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;

	// Ignition
	engineConfiguration->ignitionDwellForCrankingMs = DEFAULT_CRANKING_DWELL_MS;
	engineConfiguration->crankingTimingAngle = DEFAULT_CRANKING_ANGLE;

	// IAC
	setArrayValues(config->cltCrankingCorr, 50);
	// should be 100 once tune is better
	setArrayValues(config->afterCrankingIACtaperDuration, 200);
	setLinearCurve(config->afterCrankingIACtaperDurationBins, CLT_CURVE_RANGE_FROM, 100, 1);

	engineConfiguration->isFasterEngineSpinUpEnabled = true;

	// After start enrichment
#if !EFI_UNIT_TEST
	// don't set this for unit tests, as it makes things more complicated to test
	setLinearCurve(config->postCrankingCLTBins, /*from*/-20, /*to*/80, 20);
	setLinearCurve(config->postCrankingDurationBins, /*from*/0, /*to*/150, 40);
	setTable(config->postCrankingFactor, 1.2f);
#endif

	setLinearCurve(config->crankingTpsCoef, /*from*/1, /*to*/1, 1);
	setLinearCurve(config->crankingTpsBins, 0, 100, 1);

	setLinearCurve(config->cltCrankingCorrBins, CLT_CURVE_RANGE_FROM, 100, 1);
	setLinearCurve(config->cltCrankingCorr, 50, 50, 1); // now as % of idle valve/etb

#if CRANKING_CURVE_SIZE == 8
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
#endif
	// Cranking cycle compensation

	setTable(config->crankingCycleBaseFuel, 27.0f);
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
