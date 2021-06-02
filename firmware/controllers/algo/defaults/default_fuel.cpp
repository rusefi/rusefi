#include "defaults.h"
#include "engine_configuration_generated_structures.h"
#include "table_helper.h"

static void setBosch02880155868(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	// http://www.boschdealer.com/specsheets/0280155868cs.jpg
	engineConfiguration->injector.battLagCorrBins[0] = 6;
	engineConfiguration->injector.battLagCorr[0] = 3.371;

	engineConfiguration->injector.battLagCorrBins[1] = 8;
	engineConfiguration->injector.battLagCorr[1] = 1.974;

	engineConfiguration->injector.battLagCorrBins[2] = 10;
	engineConfiguration->injector.battLagCorr[2] = 1.383;

	engineConfiguration->injector.battLagCorrBins[3] = 11;
	engineConfiguration->injector.battLagCorr[3] = 1.194;

	engineConfiguration->injector.battLagCorrBins[4] = 12;
	engineConfiguration->injector.battLagCorr[4] = 1.04;

	engineConfiguration->injector.battLagCorrBins[5] = 13;
	engineConfiguration->injector.battLagCorr[5] = 0.914;

	engineConfiguration->injector.battLagCorrBins[6] = 14;
	engineConfiguration->injector.battLagCorr[6] = 0.797;

	engineConfiguration->injector.battLagCorrBins[7] = 15;
	engineConfiguration->injector.battLagCorr[7] = 0.726;
}

static void setDefaultWarmupFuelEnrichment(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	static const float bins[] =
	{
		-40,
		-30,
		-20,
		-10,
		0,
		10,
		20,
		30,
		40,
		50,
		60,
		70,
		80,
		90,
		100,
		110
	};

	copyArray(config->cltFuelCorrBins, bins);

	static const float values[] =
	{
		1.50,
		1.50,
		1.42,
		1.36,
		1.28,
		1.19,
		1.12,
		1.10,
		1.06,
		1.06,
		1.03,
		1.01,
		1,
		1,
		1,
		1
	};

	copyArray(config->cltFuelCorr, values);
}

static void setDefaultFuelCutParameters(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->coastingFuelCutEnabled = false;
	engineConfiguration->coastingFuelCutRpmLow = 1300;
	engineConfiguration->coastingFuelCutRpmHigh = 1500;
	engineConfiguration->coastingFuelCutTps = 2;
	engineConfiguration->coastingFuelCutMap = 30;
	engineConfiguration->coastingFuelCutClt = 30;
}

static void setDefaultStftSettings(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	auto& cfg = CONFIG(stft);

	// Default to disabled
	CONFIG(fuelClosedLoopCorrectionEnabled) = false;

	// Default to proportional mode (for wideband sensors)
	CONFIG(stftIgnoreErrorMagnitude) = false;

	// 60 second startup delay - some O2 sensors are slow to warm up.
	cfg.startupDelay = 60;

	// Only correct in [12.0, 17.0]
	cfg.minAfr = 120;
	cfg.maxAfr = 170;

	// Above 60 deg C
	cfg.minClt = 60;

	// 0.5% deadband
	cfg.deadband = 5;

	// Sensible region defaults
	cfg.maxIdleRegionRpm = 1000 / RPM_1_BYTE_PACKING_MULT;
	cfg.maxOverrunLoad = 35;
	cfg.minPowerLoad = 85;

	// Sensible cell defaults
	for (size_t i = 0; i < efi::size(cfg.cellCfgs); i++) {
		// 30 second time constant - nice and slow
		cfg.cellCfgs[i].timeConstant = 30 * 10;

		/// Allow +-5%
		cfg.cellCfgs[i].maxAdd = 5;
		cfg.cellCfgs[i].maxRemove = -5;
	}
}

void setDefaultFuel(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	// Base injection configuration
	engineConfiguration->isInjectionEnabled = true;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	/**
	 * By the way http://users.erols.com/srweiss/tableifc.htm has a LOT of data
	 */
	engineConfiguration->injector.flow = 200;
	engineConfiguration->stoichRatioPrimary = STOICH_RATIO * PACK_MULT_AFR_CFG;

	// 9.0 = E100 pure ethanol
	engineConfiguration->stoichRatioSecondary = 9.0f * PACK_MULT_AFR_CFG;

	// Injector deadtime
	setBosch02880155868(PASS_CONFIG_PARAMETER_SIGNATURE);

	// Tables
	setFuelTablesLoadBin(10, 160 PASS_CONFIG_PARAMETER_SUFFIX);
	setRpmTableBin(config->injPhaseRpmBins, FUEL_RPM_COUNT);
	setDefaultVETable(PASS_CONFIG_PARAMETER_SIGNATURE);
	setTable(config->injectionPhase, -180.0f);

	// Charge temperature estimation
	engineConfiguration->tChargeMinRpmMinTps = 0.25;
	engineConfiguration->tChargeMinRpmMaxTps = 0.25;
	engineConfiguration->tChargeMaxRpmMinTps = 0.25;
	engineConfiguration->tChargeMaxRpmMaxTps = 0.9;
	engineConfiguration->tChargeMode = TCHARGE_MODE_RPM_TPS;
	engineConfiguration->tChargeAirCoefMin = 0.098f;
	engineConfiguration->tChargeAirCoefMax = 0.902f;
	engineConfiguration->tChargeAirFlowMax = 153.6f;
	engineConfiguration->tChargeAirIncrLimit = 1.0f;
	engineConfiguration->tChargeAirDecrLimit = 12.5f;

	// CLT correction table
	setDefaultWarmupFuelEnrichment(PASS_CONFIG_PARAMETER_SIGNATURE);

	// IAT correction table
	// TODO

	// Closed loop fuel correction
	setDefaultStftSettings(PASS_CONFIG_PARAMETER_SIGNATURE);

	// Decel fuel cut
	setDefaultFuelCutParameters(PASS_CONFIG_PARAMETER_SIGNATURE);

	// Accel Enrich
	engineConfiguration->engineLoadAccelLength = 6;
	engineConfiguration->engineLoadAccelEnrichmentThreshold = 5; // kPa
	engineConfiguration->engineLoadAccelEnrichmentMultiplier = 0; // todo: improve implementation and re-enable by default

	engineConfiguration->tpsAccelLength = 12;
	engineConfiguration->tpsAccelEnrichmentThreshold = 40; // TPS % change, per engine cycle

#if !EFI_UNIT_TEST
	// todo: this is a reasonable default for what kinds of engines exactly?
	engineConfiguration->wwaeTau = 0.3;
	engineConfiguration->wwaeBeta = 0.3;
#endif // EFI_UNIT_TEST

	// TPS/TPS AE curve
	setMazdaMiataNbTpsTps(PASS_CONFIG_PARAMETER_SIGNATURE);

	// AE load taper
	setLinearCurve(engineConfiguration->mapAccelTaperBins, 0, 32, 4);
	setArrayValues(engineConfiguration->mapAccelTaperMult, 1.0f);
}