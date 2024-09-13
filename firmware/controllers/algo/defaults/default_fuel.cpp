#include "pch.h"

#include "defaults.h"
#include "table_helper.h"
#include "mazda_miata_vvt.h"

static void setBosch02880155868(injector_s& cfg) {
	// http://www.boschdealer.com/specsheets/0280155868cs.jpg
	cfg.battLagCorrBins[0] = 6;
	cfg.battLagCorr[0] = 3.371;

	cfg.battLagCorrBins[1] = 8;
	cfg.battLagCorr[1] = 1.974;

	cfg.battLagCorrBins[2] = 10;
	cfg.battLagCorr[2] = 1.383;

	cfg.battLagCorrBins[3] = 11;
	cfg.battLagCorr[3] = 1.194;

	cfg.battLagCorrBins[4] = 12;
	cfg.battLagCorr[4] = 1.04;

	cfg.battLagCorrBins[5] = 13;
	cfg.battLagCorr[5] = 0.914;

	cfg.battLagCorrBins[6] = 14;
	cfg.battLagCorr[6] = 0.797;

	cfg.battLagCorrBins[7] = 15;
	cfg.battLagCorr[7] = 0.726;
}

static void setDefaultWarmupFuelEnrichment() {
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

static void setDefaultVETable() {
	setRpmTableBin(config->veRpmBins);
	setTable(config->veTable, 80);

	setRpmTableBin(config->baroCorrRpmBins);
	setLinearCurve(config->baroCorrPressureBins, 75, 105, 1);

	// Default baro table is all 1.0, we can't recommend a reasonable default here
	setTable(config->baroCorrTable, 1);

	// Give default axes for cylinder trim tables
	copyArray(config->fuelTrimRpmBins, { 1000, 3000, 5000, 7000 });
	copyArray(config->fuelTrimLoadBins, { 20, 50, 80, 100 });

	// Default axes for VE blends
	for (size_t i = 0; i < efi::size(config->veBlends); i++) {
		auto& blend = config->veBlends[i];
		setLinearCurve(blend.loadBins, 0, 100, 10);
		setLinearCurve(blend.rpmBins, 0, 7000);

		setLinearCurve(blend.blendBins, 0, 100);
		setLinearCurve(blend.blendValues, 0, 100);
	}
}

static void setDefaultFuelCutParameters() {
	engineConfiguration->coastingFuelCutEnabled = false;
	engineConfiguration->coastingFuelCutRpmLow = 1300;
	engineConfiguration->coastingFuelCutRpmHigh = 1500;
	engineConfiguration->coastingFuelCutTps = 2;
	engineConfiguration->coastingFuelCutMap = 30;
	engineConfiguration->coastingFuelCutClt = 60;

	copyArray(config->dfcoMapRpmValuesBins, { 1500, 2000, 3500, 5000 });
	copyArray(config->dfcoMapRpmValues, { 30, 25, 20, 18 });
}

static void setDefaultStftSettings() {
	auto& cfg = engineConfiguration->stft;

	// Default to disabled
	engineConfiguration->fuelClosedLoopCorrectionEnabled = false;

	// Default to proportional mode (for wideband sensors)
	engineConfiguration->stftIgnoreErrorMagnitude = false;

	// 60 second startup delay - some O2 sensors are slow to warm up.
	cfg.startupDelay = 60;

	// Only correct in [12.0, 17.0]
	cfg.minAfr = 12;
	cfg.maxAfr = 17;

	// Above 60 deg C
	cfg.minClt = 60;

	// 0.5% deadband
	cfg.deadband = 0.5f;

	// Sensible region defaults
	cfg.maxIdleRegionRpm = 1000;
	cfg.maxOverrunLoad = 35;
	cfg.minPowerLoad = 85;

	// Sensible cell defaults
	for (size_t i = 0; i < efi::size(cfg.cellCfgs); i++) {
		// 30 second time constant - nice and slow
		cfg.cellCfgs[i].timeConstant = 30;

		/// Allow +-5%
		cfg.cellCfgs[i].maxAdd = 5;
		cfg.cellCfgs[i].maxRemove = -5;
	}
}

static const uint8_t tpsTpsTable[TPS_TPS_ACCEL_TABLE][TPS_TPS_ACCEL_TABLE] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 2, 0, 0, 0, 0, 0, 0, 0 },
	{ 3, 2, 0, 0, 0, 0, 0, 0 },
	{ 4, 3, 2, 0, 0, 0, 0, 0 },
	{ 5, 4, 3, 2, 0, 0, 0, 0 },
	{ 5, 5, 4, 3, 2, 0, 0, 0 },
	{ 6, 5, 5, 4, 3, 2, 0, 0 },
	{ 7, 6, 5, 5, 4, 3, 2, 0 },
};

static void setMazdaMiataNbTpsTps() {
	setLinearCurve(config->tpsTpsAccelFromRpmBins, 0, 100, 10);
	setLinearCurve(config->tpsTpsAccelToRpmBins, 0, 100, 10);
	copyTable(config->tpsTpsAccelTable, tpsTpsTable);
}

static void setDefaultLambdaTable() {
	static constexpr float mapBins[] = {
		30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 150, 175, 200, 225, 250
	};
	copyArray(config->lambdaLoadBins, mapBins);

	setRpmTableBin(config->lambdaRpmBins);

	static constexpr float rowValues[] = {
		1,		1,		1,		1,		// 30, 40, 50, 60 kpa
		1,		0.95,	0.92,	0.90,	// 70, 80, 90, 100 kpa
		0.89,	0.88,	0.86,	0.84,	// 110, 120, 130, 150 kpa
		0.8,	0.77,	0.75,	0.73,	// 175, 200, 225, 250 kpa
	};

	// Set each row to the corresponding value from rowValues
	for (size_t i = 0; i < efi::size(config->lambdaTable); i++) {
		for (size_t j = 0; j < efi::size(config->lambdaTable[i]); j++) {
			config->lambdaTable[i][j] = rowValues[i];
		}
	}
}

void setDefaultWallWetting() {
#if !EFI_UNIT_TEST
	// todo: this is a reasonable default for what kinds of engines exactly?
	engineConfiguration->wwaeTau = 0.3;
	engineConfiguration->wwaeBeta = 0.3;
#endif // EFI_UNIT_TEST

	// linear reasonable bins
	setLinearCurve(config->wwCltBins, -40, 100, 1);
	setLinearCurve(config->wwMapBins, 10, 80, 1);

	// These values are derived from the GM factory tune for a gen3 LS engine
	// Who knows if they're good for anything else, but at least they look nice?
	static constexpr float tauClt[] = {
		1.45, 1.30, 1.17, 1.05, 0.90, 0.82, 0.75, 0.70
	};
	copyArray(config->wwTauCltValues, tauClt);

	static constexpr float tauMap[] = {
		0.38, 0.55, 0.69, 0.86, 0.90, 0.95, 0.97, 1.00
	};
	copyArray(config->wwTauMapValues, tauMap);

	static constexpr float betaClt[] = {
		0.73, 0.66, 0.57, 0.46, 0.38, 0.31, 0.24, 0.19
	};
	copyArray(config->wwBetaCltValues, betaClt);

	static constexpr float betaMap[] = {
		0.21, 0.40, 0.60, 0.79, 0.85, 0.90, 0.95, 1.00
	};
	copyArray(config->wwBetaMapValues, betaMap);
}

static void setDefaultLambdaProtection() {
	engineConfiguration->lambdaProtectionEnable = false;

	engineConfiguration->lambdaProtectionMinLoad = 60;
	engineConfiguration->lambdaProtectionMinRpm = 2500;
	engineConfiguration->lambdaProtectionMinTps = 50;
	engineConfiguration->lambdaProtectionTimeout = 0.5f;

	engineConfiguration->lambdaProtectionRestoreLoad = 30;
	engineConfiguration->lambdaProtectionRestoreRpm = 2000;
	engineConfiguration->lambdaProtectionRestoreTps = 20;
}

static void setDefaultPriming() {
	// These defaults are reasonable for ~500cc cylinders
	static constexpr int8_t primeBins[]     = { -40, -20,   0,  20, 40, 60, 80, 100 };
	static constexpr uint16_t primeValues[] = { 755, 605, 265, 140, 75, 50, 45,  40 };

	copyArray(engineConfiguration->primeBins, primeBins);
	copyArray(engineConfiguration->primeValues, primeValues);
}

void setDefaultFuel() {
	// Base injection configuration
	engineConfiguration->isInjectionEnabled = true;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	/**
	 * By the way http://users.erols.com/srweiss/tableifc.htm has a LOT of data
	 */
	engineConfiguration->injector.flow = 200;
	engineConfiguration->injectorSecondary.flow = 200;
	engineConfiguration->stoichRatioPrimary = STOICH_RATIO;

	// 9.0 = E100 pure ethanol
	engineConfiguration->stoichRatioSecondary = 9.0f;

	// Injector deadtime
	setBosch02880155868(engineConfiguration->injector);
	setBosch02880155868(engineConfiguration->injectorSecondary);

	// Tables
	setFuelTablesLoadBin(10, 160);
	setRpmTableBin(config->injPhaseRpmBins);

	setRpmTableBin(config->tpsTspCorrValuesBins);
	setLinearCurve(config->tpsTspCorrValues, 1, 1);

	setDefaultVETable();
	setDefaultLambdaTable();

	setLinearCurve(config->injectorStagingLoadBins, 0, 100, 10);
	setRpmTableBin(config->injectorStagingRpmBins);

	setRpmTableBin(config->mapEstimateRpmBins);
	setLinearCurve(config->mapEstimateTpsBins, 0, 100);
	setTable(config->mapEstimateTable, 60);

	// -400 will close the injector just before TDC at the end of the exhaust stroke,
	// around the time the intake valve opens.
	setTable(config->injectionPhase, -400.0f);

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
	setDefaultWarmupFuelEnrichment();

	// IAT correction table
	// TODO

	// Closed loop fuel correction
	setDefaultStftSettings();

	// Decel fuel cut
	setDefaultFuelCutParameters();

	engineConfiguration->tpsAccelEnrichmentThreshold = 40; // TPS % change, per engine cycle

	setDefaultWallWetting();

	// TPS/TPS AE curve
	setMazdaMiataNbTpsTps();

	// Some reasonable reference pressure that many vehicles use
	engineConfiguration->fuelReferencePressure = 300;

	// Lambda protection defaults
	setDefaultLambdaProtection();

	setDefaultPriming();

	// Cut at 110% instantly
	engineConfiguration->maxInjectorDutyInstant = 110;
	// Cut at 96% after 0.5 second
	engineConfiguration->maxInjectorDutySustained = 96;
	engineConfiguration->maxInjectorDutySustainedTimeout = 0.5f;
}
