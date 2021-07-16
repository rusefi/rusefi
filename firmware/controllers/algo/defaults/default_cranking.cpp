#include "defaults.h"
#include "engine_configuration.h"
#include "table_helper.h"

void setDefaultCranking(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->cranking.rpm = 550;

	// Fuel
	CONFIG(crankingInjectionMode) = IM_SIMULTANEOUS;
	engineConfiguration->cranking.baseFuel = 27;

	// Ignition
	engineConfiguration->ignitionDwellForCrankingMs = 6;
	CONFIG(crankingTimingAngle) = 6;

	// IAC
	engineConfiguration->crankingIACposition = 50;
	engineConfiguration->afterCrankingIACtaperDuration = 35;

	// After start enrichment
#if !EFI_UNIT_TEST
	// don't set this for unit tests, as it makes things more complicated to test
	engineConfiguration->postCrankingFactor = 1.2;
#endif

	engineConfiguration->postCrankingDurationSec = 10;

	setLinearCurve(engineConfiguration->crankingTpsCoef, /*from*/1, /*to*/1, 1);
	setLinearCurve(engineConfiguration->crankingTpsBins, 0, 100, 1);

	setLinearCurve(config->cltCrankingCorrBins, CLT_CURVE_RANGE_FROM, 100, 1);
	setLinearCurve(config->cltCrankingCorr, 1.0, 1.0, 1);

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
	copyArray(config->crankingFuelCoef, crankingCoef);

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

	// Whole table is 1.0, except first two values are steeper
	setArrayValues(config->crankingCycleCoef, 1.0f);
	config->crankingCycleCoef[0] = 2.0f;
	config->crankingCycleCoef[1] = 1.3f;

	// X values are simply counting up cycle number starting at 1
	for (size_t i = 0; i < efi::size(config->crankingCycleBins); i++) {
		config->crankingCycleBins[i] = i + 1;
	}

	// Cranking ignition timing
	static const float advanceValues[] = { 0, 0, 0, 0 };
	copyArray(engineConfiguration->crankingAdvance, advanceValues);

	static const float advanceBins[] = { 0, 200, 400, 1000 };
	copyArray(engineConfiguration->crankingAdvanceBins, advanceBins);

	CONFIG(useTLE8888_cranking_hack) = true;
}
