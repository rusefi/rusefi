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
#if CLT_CURVE_SIZE == 16
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
#endif // CLT_CURVE_SIZE
}

static void setDefaultVETable() {
	setRpmTableBin(config->veRpmBins);
#if (IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT) && (IGN_RPM_COUNT == DEFAULT_IGN_RPM_COUNT)
	static const float hardCodedveTable[16][16] = {
{49.300,	49.300,	49.400,	49.600,	50.200,	51.400,	52.600,	53.800,	54.400,	54.600,	54.400,	53.700,	52.800,	51.800,	50.900,	50.000,	},
{49.600,	50.500,	51.500,	54.100,	57.500,	60.700,	62.900,	64.400,	65.000,	65.000,	64.500,	63.500,	62.300,	61.100,	60.000,	58.800,	},
{50.300,	52.400,	54.300,	58.500,	62.700,	66.100,	68.200,	69.700,	70.200,	70.100,	69.500,	68.400,	67.100,	65.800,	64.500,	63.300,	},
{51.400,	55.200,	58.200,	63.500,	68.000,	71.400,	73.500,	74.900,	75.300,	75.200,	74.500,	73.300,	71.800,	70.400,	69.000,	67.700,	},
{54.300,	60.900,	64.900,	70.700,	75.100,	78.300,	80.300,	81.700,	82.100,	81.900,	81.100,	79.700,	78.100,	76.500,	75.000,	73.500,	},
{56.400,	64.400,	68.700,	74.500,	78.700,	81.800,	83.800,	85.100,	85.500,	85.300,	84.400,	83.000,	81.300,	79.600,	78.000,	76.400,	},
{60.700,	70.200,	74.500,	79.900,	83.800,	86.800,	88.900,	90.200,	90.600,	90.300,	89.400,	87.800,	86.000,	84.200,	82.500,	80.800,	},
{65.800,	76.000,	80.000,	85.000,	88.800,	91.800,	93.900,	95.200,	95.600,	95.300,	94.300,	92.600,	90.700,	88.800,	87.000,	85.200,	},
{69.400,	79.100,	82.900,	87.800,	91.700,	94.800,	97.000,	98.300,	98.800,	98.400,	97.400,	95.600,	93.500,	91.500,	89.600,	87.700,	},
{73.000,	82.200,	85.900,	90.700,	94.600,	97.800,	100.000,	101.500,	101.900,	101.600,	100.400,	98.500,	96.400,	94.200,	92.200,	90.200,	},
{76.600,	85.300,	88.800,	93.500,	97.400,	100.700,	103.100,	104.600,	105.100,	104.700,	103.500,	101.500,	99.200,	96.900,	94.800,	92.600,	},
{80.300,	88.500,	91.800,	96.400,	100.300,	103.700,	106.200,	107.800,	108.200,	107.800,	106.500,	104.400,	102.000,	99.700,	97.400,	95.100,	},
{83.900,	91.600,	94.700,	99.200,	103.200,	106.700,	109.200,	110.900,	111.400,	110.900,	109.600,	107.400,	104.800,	102.400,	99.900,	97.600,	},
{87.500,	94.700,	97.600,	102.000,	106.100,	109.700,	112.300,	114.000,	114.500,	114.100,	112.600,	110.300,	107.700,	105.100,	102.500,	100.100,	},
{91.100,	97.800,	100.600,	104.900,	108.900,	112.600,	115.300,	117.200,	117.700,	117.200,	115.700,	113.300,	110.500,	107.800,	105.100,	102.500,	},
{94.700,	100.900,	103.500,	107.700,	111.800,	115.600,	118.400,	120.300,	120.800,	120.300,	118.700,	116.200,	113.300,	110.500,	107.700,	105.000,	},
};
	copyTable(config->veTable, hardCodedveTable);
#else
	setTable(config->veTable, 80);
#endif

	setRpmTableBin(config->baroCorrRpmBins);
	setLinearCurve(config->baroCorrPressureBins, 75, 105, 1);

	// Default baro table is all 1.0, we can't recommend a reasonable default here
	setTable(config->baroCorrTable, 1);

	// Give default axes for cylinder trim tables
#if FUEL_TRIM_SIZE == 4
	copyArray(config->fuelTrimRpmBins, { 1000, 3000, 5000, 7000 });
	copyArray(config->fuelTrimLoadBins, { 20, 50, 80, 100 });
#else
  setRpmTableBin(config->fuelTrimRpmBins);
  setLinearCurve(config->fuelTrimLoadBins, 20, 100);
#endif

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
/* Generated by TS2C on Tue Apr 18 21:29:16 EDT 2017*/
{/* 0 0	*//* 0 0.0*/0,	/* 1 10.0*/0,	/* 2 20.0*/0,	/* 3 35.0*/0,	/* 4 50.0*/0,	/* 5 65.0*/0,	/* 6 80.0*/0,	/* 7 100.0*/0,	},
{/* 1 10	*//* 0 0.0*/10,	/* 1 10.0*/0,	/* 2 20.0*/0,	/* 3 35.0*/0,	/* 4 50.0*/0,	/* 5 65.0*/0,	/* 6 80.0*/0,	/* 7 100.0*/0,	},
{/* 2 20	*//* 0 0.0*/17,	/* 1 10.0*/10,	/* 2 20.0*/0,	/* 3 35.0*/0,	/* 4 50.0*/0,	/* 5 65.0*/0,	/* 6 80.0*/0,	/* 7 100.0*/0,	},
{/* 3 35	*//* 0 0.0*/23,	/* 1 10.0*/17,	/* 2 20.0*/10,	/* 3 35.0*/0,	/* 4 50.0*/0,	/* 5 65.0*/0,	/* 6 80.0*/0,	/* 7 100.0*/0,	},
{/* 4 50	*//* 0 0.0*/28,	/* 1 10.0*/23,	/* 2 20.0*/17,	/* 3 35.0*/10,	/* 4 50.0*/0,	/* 5 65.0*/0,	/* 6 80.0*/0,	/* 7 100.0*/0,	},
{/* 5 65	*//* 0 0.0*/32,	/* 1 10.0*/28,	/* 2 20.0*/23,	/* 3 35.0*/17,	/* 4 50.0*/10,	/* 5 65.0*/0,	/* 6 80.0*/0,	/* 7 100.0*/0,	},
{/* 6 80	*//* 0 0.0*/35,	/* 1 10.0*/32,	/* 2 20.0*/28,	/* 3 35.0*/23,	/* 4 50.0*/17,	/* 5 65.0*/10,	/* 6 80.0*/0,	/* 7 100.0*/0,	},
{/* 7 100	*//* 0 0.0*/37,	/* 1 10.0*/35,	/* 2 20.0*/32,	/* 3 35.0*/28,	/* 4 50.0*/23,	/* 5 65.0*/17,	/* 6 80.0*/10,	/* 7 100.0*/0,	},
};

static void setMazdaMiataNbTpsTps() {
	setLinearCurve(config->tpsTpsAccelFromRpmBins, 0, 100, 10);
	setLinearCurve(config->tpsTpsAccelToRpmBins, 0, 100, 10);
	copyTable(config->tpsTpsAccelTable, tpsTpsTable);
}

static void setDefaultLambdaTable() {
	setRpmTableBin(config->lambdaRpmBins);

#if (FUEL_LOAD_COUNT == DEFAULT_FUEL_LOAD_COUNT) && (FUEL_RPM_COUNT == DEFAULT_FUEL_LOAD_COUNT)
	static constexpr float mapBins[] = {
		30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 150, 175, 200, 225, 250
	};
	copyArray(config->lambdaLoadBins, mapBins);

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
#endif
}

void setGdiWallWetting() {
	engineConfiguration->wwaeTau = 0;
	engineConfiguration->wwaeBeta = 0;
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

	setTable(config->injectionPhase, PORT_INJECTION_OFFSET);

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
