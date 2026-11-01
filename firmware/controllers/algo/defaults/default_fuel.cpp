#include "pch.h"

#include "defaults.h"
#include "table_helper.h"
#include "mazda_miata_vvt.h"
#include "engine_configuration_defaults.h"

static void setBosch02880155868(injector_s& cfg) {
	// http://www.boschdealer.com/specsheets/0280155868cs.jpg (use web.archive.org)
#if (VBAT_INJECTOR_CURVE_PRESSURE_SIZE == 2) && (VBAT_INJECTOR_CURVE_SIZE == 8)
    // see https://github.com/rusefi/rusefi/issues/7521 for adding more values
	copyTable(cfg.battLagCorrTable, engine_configuration_defaults::INJECTOR_BATT_LAG_CURR);
#endif

#if (VBAT_INJECTOR_CURVE_SIZE == 8)
    static const float vBattBins[8] = { 6.0, 8.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0 };
	copyArray(cfg.battLagCorrBattBins, vBattBins);
#else
  setLinearCurve(cfg.battLagCorrBattBins, 6, 15, 0.1);
#endif

#if (VBAT_INJECTOR_CURVE_PRESSURE_SIZE == 2)
	static const float pressureBins[2] = { 206.843f, 413.685f };
	copyArray(cfg.battLagCorrPressBins,pressureBins);
#else
  setLinearCurve(cfg.battLagCorrPressBins, 300, 400, 1);
#endif

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
		1.42f,
		1.36f,
		1.28f,
		1.19f,
		1.12f,
		1.10f,
		1.06f,
		1.06f,
		1.03f,
		1.01f,
		1,
		1,
		1,
		1
	};

	copyArray(config->cltFuelCorr, values);
#elif CLT_CURVE_SIZE == 8
	// Compressed version of the 16-point curve above (every other point), for boards that
	// override CLT_CURVE_SIZE down to 8 (e.g. fw-custom-paralela-master/prepend.txt) — without
	// this branch the #if above silently compiled to nothing and cltFuelCorrBins/cltFuelCorr
	// stayed at their zero-initialized default, i.e. no warmup enrichment axis at all.
	static const float bins[] =
	{
		-40,
		-20,
		0,
		20,
		40,
		60,
		80,
		100
	};

	copyArray(config->cltFuelCorrBins, bins);

	static const float values[] =
	{
		1.50,
		1.42,
		1.28,
		1.12,
		1.06,
		1.03,
		1,
		1
	};

	copyArray(config->cltFuelCorr, values);
#endif // CLT_CURVE_SIZE
}

static void setDefaultVETable() {
	setRpmTableBin(config->veRpmBins);

#if (VE_LOAD_COUNT == 16) && (VE_RPM_COUNT == 16)
	static const float hardCodedveTable[16][16] = {
{49.300f,	49.300f,	49.400f,	49.600f,	50.200f,	51.400f,	52.600f,	53.800f,	54.400f,	54.600f,	54.400f,	53.700f,	52.800f,	51.800f,	50.900f,	50.000f,	},
{49.600f,	50.500f,	51.500f,	54.100f,	57.500f,	60.700f,	62.900f,	64.400f,	65.000f,	65.000f,	64.500f,	63.500f,	62.300f,	61.100f,	60.000f,	58.800f,	},
{50.300f,	52.400f,	54.300f,	58.500f,	62.700f,	66.100f,	68.200f,	69.700f,	70.200f,	70.100f,	69.500f,	68.400f,	67.100f,	65.800f,	64.500f,	63.300f,	},
{51.400f,	55.200f,	58.200f,	63.500f,	68.000f,	71.400f,	73.500f,	74.900f,	75.300f,	75.200f,	74.500f,	73.300f,	71.800f,	70.400f,	69.000f,	67.700f,	},
{54.300f,	60.900f,	64.900f,	70.700f,	75.100f,	78.300f,	80.300f,	81.700f,	82.100f,	81.900f,	81.100f,	79.700f,	78.100f,	76.500f,	75.000f,	73.500f,	},
{56.400f,	64.400f,	68.700f,	74.500f,	78.700f,	81.800f,	83.800f,	85.100f,	85.500f,	85.300f,	84.400f,	83.000f,	81.300f,	79.600f,	78.000f,	76.400f,	},
{60.700f,	70.200f,	74.500f,	79.900f,	83.800f,	86.800f,	88.900f,	90.200f,	90.600f,	90.300f,	89.400f,	87.800f,	86.000f,	84.200f,	82.500f,	80.800f,	},
{65.800f,	76.000f,	80.000f,	85.000f,	88.800f,	91.800f,	93.900f,	95.200f,	95.600f,	95.300f,	94.300f,	92.600f,	90.700f,	88.800f,	87.000f,	85.200f,	},
{69.400f,	79.100f,	82.900f,	87.800f,	91.700f,	94.800f,	97.000f,	98.300f,	98.800f,	98.400f,	97.400f,	95.600f,	93.500f,	91.500f,	89.600f,	87.700f,	},
{73.000f,	82.200f,	85.900f,	90.700f,	94.600f,	97.800f,	100.000f,	101.500f,	101.900f,	101.600f,	100.400f,	98.500f,	96.400f,	94.200f,	92.200f,	90.200f,	},
{76.600f,	85.300f,	88.800f,	93.500f,	97.400f,	100.700f,	103.100f,	104.600f,	105.100f,	104.700f,	103.500f,	101.500f,	99.200f,	96.900f,	94.800f,	92.600f,	},
{80.300f,	88.500f,	91.800f,	96.400f,	100.300f,	103.700f,	106.200f,	107.800f,	108.200f,	107.800f,	106.500f,	104.400f,	102.000f,	99.700f,	97.400f,	95.100f,	},
{83.900f,	91.600f,	94.700f,	99.200f,	103.200f,	106.700f,	109.200f,	110.900f,	111.400f,	110.900f,	109.600f,	107.400f,	104.800f,	102.400f,	99.900f,	97.600f,	},
{87.500f,	94.700f,	97.600f,	102.000f,	106.100f,	109.700f,	112.300f,	114.000f,	114.500f,	114.100f,	112.600f,	110.300f,	107.700f,	105.100f,	102.500f,	100.100f,	},
{91.100f,	97.800f,	100.600f,	104.900f,	108.900f,	112.600f,	115.300f,	117.200f,	117.700f,	117.200f,	115.700f,	113.300f,	110.500f,	107.800f,	105.100f,	102.500f,	},
{94.700f,	100.900f,	103.500f,	107.700f,	111.800f,	115.600f,	118.400f,	120.300f,	120.800f,	120.300f,	118.700f,	116.200f,	113.300f,	110.500f,	107.700f,	105.000f,	},
};
	copyTable(config->veTable, hardCodedveTable);
#else
	setTable(config->veTable, 80);
#endif

	setRpmTableBin(config->baroCorrRpmBins);
	setLinearCurve(config->baroCorrPressureBins, 75, 105, 1);

	setLinearCurve(config->tmfRatioBins, 0.5f, 1.5f, 0.1f);

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

	for (size_t i = 0; i < efi::size(config->targetAfrBlends); i++) {
		auto& blend = config->targetAfrBlends[i];
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

	// Also used in lambda monitor
	engineConfiguration->noFuelTrimAfterDfcoTime = 5;

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
		cfg.cellCfgs[i].maxRemove = 5;
	}
}

static void setDefaultLtftSettings() {
	auto& cfg = engineConfiguration->ltft;

	// Default to allow learning, but do not apply learned corrections
	cfg.enabled = true;
	cfg.correctionEnabled = false;

	// Default to slow learning
	cfg.timeConstant[ftRegionIdle] = 3000;
	cfg.timeConstant[ftRegionOverrun] = 1500;
	cfg.timeConstant[ftRegionPower] = 30;
	cfg.timeConstant[ftRegionCruise] = 300;

	// 0.5% deadband
	cfg.deadband = 0.5f;

	// Allow +-12.5%
	cfg.maxAdd = 12.5;
	cfg.maxRemove = 12.5;
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

#if (FUEL_LOAD_COUNT == 16) && (FUEL_RPM_COUNT == 16)
	copyArray(config->lambdaLoadBins, engine_configuration_defaults::DEFAULT_LAMBDA_LOAD_BINS);

	// Set each row to the corresponding value from rowValues
	for (size_t i = 0; i < efi::size(config->lambdaTable); i++) {
		for (size_t j = 0; j < efi::size(config->lambdaTable[i]); j++) {
			config->lambdaTable[i][j] = engine_configuration_defaults::DEFAULT_LAMBDA_TABLE_ROW[i];
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
	engineConfiguration->wwaeTau = 0.3f;
	engineConfiguration->wwaeBeta = 0.3f;
#endif // EFI_UNIT_TEST

	// linear reasonable bins
	setLinearCurve(config->wwCltBins, -40, 100, 1);
	setLinearCurve(config->wwMapBins, 10, 80, 1);

	// These values are derived from the GM factory tune for a gen3 LS engine
	// Who knows if they're good for anything else, but at least they look nice?
	static constexpr float tauClt[] = {
		1.45f, 1.30f, 1.17f, 1.05f, 0.90f, 0.82f, 0.75f, 0.70f
	};
	copyArray(config->wwTauCltValues, tauClt);

	static constexpr float tauMap[] = {
		0.38f, 0.55f, 0.69f, 0.86f, 0.90f, 0.95f, 0.97f, 1.00f
	};
	copyArray(config->wwTauMapValues, tauMap);

	static constexpr float betaClt[] = {
		0.73f, 0.66f, 0.57f, 0.46f, 0.38f, 0.31f, 0.24f, 0.19f
	};
	copyArray(config->wwBetaCltValues, betaClt);

	static constexpr float betaMap[] = {
		0.21f, 0.40f, 0.60f, 0.79f, 0.85f, 0.90f, 0.95f, 1.00f
	};
	copyArray(config->wwBetaMapValues, betaMap);
}

static void setDefaultFlexTransientComp() {
	setLinearCurve(config->flexTransientCltBins, -40, 100, 1);
	setLinearCurve(config->flexTransientEthanolBins, 0, 100, 1);

	// Default to neutral (1.0) so transient fueling is unchanged until the user tunes these
	setTable(config->flexAeMult, 1.0f);
	setTable(config->flexWwTauMult, 1.0f);
	setTable(config->flexWwBetaMult, 1.0f);
}

static void setDefaultWboSettings() {
	for (size_t i = 0; i < CAN_WBO_COUNT; i++) {
		engineConfiguration->canWbo[i].type = RUSEFI;
		engineConfiguration->canWbo[i].reId = static_cast<can_wbo_re_id_e>(i);
		engineConfiguration->canWbo[i].aemId = static_cast<can_wbo_aem_id_e>(i);
	}
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

	// Opt-in 2D priming flex table: seed every ethanol row with the same coolant curve so it starts
	// neutral with respect to ethanol (identical to the non-flex curve) until the user calibrates it.
	static constexpr uint8_t primeFlexEthanolBins[] = { 0, 35, 65, 100 };
	copyArray(engineConfiguration->primeFlexBins, primeFlexEthanolBins);

	for (size_t ethanolIdx = 0; ethanolIdx < efi::size(engineConfiguration->primeFlexTable); ethanolIdx++) {
		for (size_t cltIdx = 0; cltIdx < PRIME_CURVE_COUNT; cltIdx++) {
			engineConfiguration->primeFlexTable[ethanolIdx][cltIdx] = primeValues[cltIdx];
		}
	}
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

	setRpmTableBin(config->predictiveMapBlendDurationBins);
	setLinearCurve(config->predictiveMapBlendDurationValues, 1, 1);

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
	engineConfiguration->tChargeMaxRpmMaxTps = 0.9f;
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
	setDefaultLtftSettings();

	// Decel fuel cut
	setDefaultFuelCutParameters();

	engineConfiguration->tpsAccelEnrichmentThreshold = 40; // TPS % change, per engine cycle

	setDefaultWallWetting();

	// Flex fuel transient compensation (CLT x ethanol) - neutral by default
	setDefaultFlexTransientComp();

	// TPS/TPS AE curve
	setMazdaMiataNbTpsTps();

	// Some reasonable reference pressure that many vehicles use
	engineConfiguration->fuelReferencePressure = 300;

	setDefaultWboSettings();

	// Lambda protection defaults
	setDefaultLambdaProtection();

	setDefaultPriming();

	// Cut at 110% instantly
	engineConfiguration->maxInjectorDutyInstant = 110;
	// Cut at 96% after 0.5 second
	engineConfiguration->maxInjectorDutySustained = 96;
	engineConfiguration->maxInjectorDutySustainedTimeout = 0.5f;
}
