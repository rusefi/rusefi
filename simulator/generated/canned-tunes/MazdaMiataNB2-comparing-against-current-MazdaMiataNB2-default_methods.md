```
static void nb2cannedknockNoiseRpmBins() {
	static const float hardCodedknockNoiseRpmBins[16] = {650.0, 700.0, 800.0, 1150.0, 1600.0, 2000.0, 2400.0, 2850.0, 3250.0, 3700.0, 4100.0, 4500.0, 4950.0, 5400.0, 6200.0, 7000.0};
	copyArray(config->knockNoiseRpmBins, hardCodedknockNoiseRpmBins);
}

static void nb2cannedknockBaseNoise() {
	static const float hardCodedknockBaseNoise[16] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->knockBaseNoise, hardCodedknockBaseNoise);
}

static void nb2cannedscriptCurve5Bins() {
	static const float hardCodedscriptCurve5Bins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->scriptCurve5Bins, hardCodedscriptCurve5Bins);
}

static void nb2cannedscriptCurve6Bins() {
	static const float hardCodedscriptCurve6Bins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->scriptCurve6Bins, hardCodedscriptCurve6Bins);
}

static void nb2cannedsparkDwellRpmBins() {
	static const float hardCodedsparkDwellRpmBins[8] = {500.0, 1200.0, 2000.0, 3000.0, 4000.0, 5000.0, 6000.0, 7200.0};
	copyArray(config->sparkDwellRpmBins, hardCodedsparkDwellRpmBins);
}

static void nb2cannedsparkDwellValues() {
	static const float hardCodedsparkDwellValues[8] = {2.5, 2.4, 2.3, 2.2, 2.1, 2.1, 2.0, 1.9};
	copyArray(config->sparkDwellValues, hardCodedsparkDwellValues);
}

static void nb2canneddwellVoltageCorrVoltBins() {
	static const float hardCodeddwellVoltageCorrVoltBins[8] = {8.0, 9.0, 10.0, 11.0, 12.0, 14.0, 16.0, 18.0};
	copyArray(config->dwellVoltageCorrVoltBins, hardCodeddwellVoltageCorrVoltBins);
}

static void nb2canneddwellVoltageCorrValues() {
	static const float hardCodeddwellVoltageCorrValues[8] = {2.28, 2.0, 1.7, 1.54, 1.36, 1.0, 0.86, 0.8};
	copyArray(config->dwellVoltageCorrValues, hardCodeddwellVoltageCorrValues);
}

static void nb2cannedprimeBins() {
	static const float hardCodedprimeBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->primeBins, hardCodedprimeBins);
}

static void nb2cannedprimeValues() {
	static const float hardCodedprimeValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->primeValues, hardCodedprimeValues);
}

static void nb2cannedmap_samplingAngleBins() {
	static const float hardCodedmap_samplingAngleBins[8] = {800.0, 1686.0, 2571.0, 3457.0, 4343.0, 5229.0, 6114.0, 7200.0};
	copyArray(engineConfiguration->map.map_samplingAngleBins, hardCodedmap_samplingAngleBins);
}

static void nb2cannedmap_samplingWindowBins() {
	static const float hardCodedmap_samplingWindowBins[8] = {800.0, 1686.0, 2571.0, 3457.0, 4343.0, 5229.0, 6114.0, 7200.0};
	copyArray(engineConfiguration->map.map_samplingWindowBins, hardCodedmap_samplingWindowBins);
}

static void nb2cannedinjector_battLagCorrBins() {
	static const float hardCodedinjector_battLagCorrBins[8] = {6.0, 8.0, 10.0, 11.0, 12.0, 13.0, 14.0, 16.0};
	copyArray(engineConfiguration->injectorSecondary.injector_battLagCorrBins, hardCodedinjector_battLagCorrBins);
}

static void nb2cannedinjector_battLagCorr() {
	static const float hardCodedinjector_battLagCorr[8] = {3.0, 2.5, 2.0, 1.699997, 1.5, 1.3499985, 1.25, 0.8499985};
	copyArray(engineConfiguration->injectorSecondary.injector_battLagCorr, hardCodedinjector_battLagCorr);
}

static void nb2cannedvrThreshold1_rpmBins() {
	static const float hardCodedvrThreshold1_rpmBins[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->vrThreshold2.vrThreshold1_rpmBins, hardCodedvrThreshold1_rpmBins);
}

static void nb2cannedvrThreshold1_values() {
	static const float hardCodedvrThreshold1_values[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->vrThreshold2.vrThreshold1_values, hardCodedvrThreshold1_values);
}

static void nb2cannedvrThreshold2_rpmBins() {
	static const float hardCodedvrThreshold2_rpmBins[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->vrThreshold2.vrThreshold2_rpmBins, hardCodedvrThreshold2_rpmBins);
}

static void nb2cannedvrThreshold2_values() {
	static const float hardCodedvrThreshold2_values[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->vrThreshold2.vrThreshold2_values, hardCodedvrThreshold2_values);
}

static void nb2cannedmafDecodingBins() {
	static const float hardCodedmafDecodingBins[32] = {0.5, 0.86999893, 1.0699997, 1.5299988, 1.8499985, 2.1099854, 2.4599915, 3.0, 3.5100098, 4.5, 4.5999756, 4.6099854, 4.619995, 4.630005, 4.6400146, 4.6500244, 4.659973, 4.669983, 4.6799927, 4.6900024, 4.700012, 4.710022, 4.7199707, 4.7299805, 4.73999, 4.75, 4.76001, 4.7700195, 4.7800293, 4.789978, 4.799988, 4.8099976};
	copyArray(config->mafDecodingBins, hardCodedmafDecodingBins);
}

static void nb2cannedcltFuelCorr() {
	static const float hardCodedcltFuelCorr[16] = {1.5, 1.5, 1.4199982, 1.3600006, 1.2799988, 1.199997, 1.1800003, 1.1500015, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1};
	copyArray(config->cltFuelCorr, hardCodedcltFuelCorr);
}

static void nb2cannedcrankingFuelCoef() {
	static const float hardCodedcrankingFuelCoef[8] = {2.7999878, 2.2000122, 1.800003, 1.550003, 1.199997, 1.1, 1.1, 1.1};
	copyArray(config->crankingFuelCoef, hardCodedcrankingFuelCoef);
}

static void nb2cannedcrankingFuelCoefE100() {
	static const float hardCodedcrankingFuelCoefE100[8] = {0.02, 0.02, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01};
	copyArray(config->crankingFuelCoefE100, hardCodedcrankingFuelCoefE100);
}

static void nb2cannedcltRevLimitRpmBins() {
	static const float hardCodedcltRevLimitRpmBins[4] = {0.0, 0.0, 0.0, 0.0};
	copyArray(config->cltRevLimitRpmBins, hardCodedcltRevLimitRpmBins);
}

static void nb2cannedcltIdleCorrBins() {
	static const float hardCodedcltIdleCorrBins[16] = {-40.0, -30.0, -20.0, -10.0, 0.0, 9.439941, 19.120117, 30.810059, 42.509766, 54.529785, 64.21973, 71.90039, 80.0, 90.0, 100.0, 110.0};
	copyArray(config->cltIdleCorrBins, hardCodedcltIdleCorrBins);
}

static void nb2cannedcltIdleCorr() {
	static const float hardCodedcltIdleCorr[16] = {1.300003, 1.25, 1.2200012, 1.1900024, 1.1399994, 1.1, 1.08, 1.08, 1.08, 1.08, 1.08, 1.08, 1.08, 1.08, 1.08, 1.08};
	copyArray(config->cltIdleCorr, hardCodedcltIdleCorr);
}

static void nb2cannedcltCrankingCorr() {
	static const float hardCodedcltCrankingCorr[8] = {1.4000015, 1.35, 1.28, 1.25, 1.2, 1.15, 1.1, 1.1};
	copyArray(config->cltCrankingCorr, hardCodedcltCrankingCorr);
}

static void nb2cannedcltIdleRpmBins() {
	static const float hardCodedcltIdleRpmBins[16] = {-40.0, -30.0, -20.0, 0.0, 10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0, 110.0, 130.0};
	copyArray(config->cltIdleRpmBins, hardCodedcltIdleRpmBins);
}

static void nb2cannedcltIdleRpm() {
	static const float hardCodedcltIdleRpm[16] = {1400.0, 1400.0, 1400.0, 1400.0, 1400.0, 1350.0, 1350.0, 1250.0, 1150.0, 1000.0, 950.0, 950.0, 950.0, 950.0, 950.0, 950.0};
	copyArray(config->cltIdleRpm, hardCodedcltIdleRpm);
}

static void nb2cannedidleAdvanceBins() {
	static const float hardCodedidleAdvanceBins[8] = {500.0, 700.0, 800.0, 900.0, 1000.0, 1100.0, 1200.0, 1300.0};
	copyArray(config->idleAdvanceBins, hardCodedidleAdvanceBins);
}

static void nb2cannedidleAdvance() {
	static const float hardCodedidleAdvance[8] = {12.0, 12.0, 11.0, 10.0, 10.0, 10.0, 10.0, 10.0};
	copyArray(config->idleAdvance, hardCodedidleAdvance);
}

static void nb2cannedfuelLevelBins() {
	static const float hardCodedfuelLevelBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->fuelLevelBins, hardCodedfuelLevelBins);
}

static void nb2cannedhpfpLobeProfileQuantityBins() {
	static const float hardCodedhpfpLobeProfileQuantityBins[16] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->hpfpLobeProfileQuantityBins, hardCodedhpfpLobeProfileQuantityBins);
}

static void nb2cannedhpfpLobeProfileAngle() {
	static const float hardCodedhpfpLobeProfileAngle[16] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->hpfpLobeProfileAngle, hardCodedhpfpLobeProfileAngle);
}

static void nb2cannedhpfpDeadtimeVoltsBins() {
	static const float hardCodedhpfpDeadtimeVoltsBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->hpfpDeadtimeVoltsBins, hardCodedhpfpDeadtimeVoltsBins);
}

static void nb2cannedtpsTpsAccelTable() {
	static const float hardCodedtpsTpsAccelTable[8][8] = {
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{1.484,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{1.822,	1.649,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{2.103,	2.024,	1.653,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{2.257,	2.337,	2.024,	1.649,	0.000,	0.000,	0.000,	0.000,	},
{2.380,	2.508,	2.337,	2.024,	1.649,	0.000,	0.000,	0.000,	},
{2.467,	2.645,	2.508,	2.337,	2.024,	1.649,	0.000,	0.000,	},
{2.586,	2.467,	2.380,	2.257,	2.103,	1.822,	1.484,	0.000,	},
};
	copyTable(config->tpsTpsAccelTable, hardCodedtpsTpsAccelTable);
}

static void nb2cannedboostRpmBins() {
	static const float hardCodedboostRpmBins[8] = {0.0, 1150.0, 2300.0, 3450.0, 4550.0, 5700.0, 6850.0, 8000.0};
	copyArray(config->boostRpmBins, hardCodedboostRpmBins);
}

static void nb2cannedboostTpsBins() {
	static const float hardCodedboostTpsBins[8] = {0.0, 14.0, 28.0, 42.0, 58.0, 72.0, 86.0, 100.0};
	copyArray(config->boostTpsBins, hardCodedboostTpsBins);
}

static void nb2cannedboostTableOpenLoop() {
	static const float hardCodedboostTableOpenLoop[8][8] = {
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	},
{28.000,	28.000,	28.000,	28.000,	28.000,	28.000,	28.000,	28.000,	},
{42.000,	42.000,	42.000,	42.000,	42.000,	42.000,	42.000,	42.000,	},
{58.000,	58.000,	58.000,	58.000,	58.000,	58.000,	58.000,	58.000,	},
{72.000,	72.000,	72.000,	72.000,	72.000,	72.000,	72.000,	72.000,	},
{86.000,	86.000,	86.000,	86.000,	86.000,	86.000,	86.000,	86.000,	},
{100.000,	100.000,	100.000,	100.000,	100.000,	100.000,	100.000,	100.000,	},
};
	copyTable(config->boostTableOpenLoop, hardCodedboostTableOpenLoop);
}

static void nb2cannedboostTableClosedLoop() {
	static const float hardCodedboostTableClosedLoop[8][8] = {
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	},
{28.000,	28.000,	28.000,	28.000,	28.000,	28.000,	28.000,	28.000,	},
{42.000,	42.000,	42.000,	42.000,	42.000,	42.000,	42.000,	42.000,	},
{58.000,	58.000,	58.000,	58.000,	58.000,	58.000,	58.000,	58.000,	},
{72.000,	72.000,	72.000,	72.000,	72.000,	72.000,	72.000,	72.000,	},
{86.000,	86.000,	86.000,	86.000,	86.000,	86.000,	86.000,	86.000,	},
{100.000,	100.000,	100.000,	100.000,	100.000,	100.000,	100.000,	100.000,	},
};
	copyTable(config->boostTableClosedLoop, hardCodedboostTableClosedLoop);
}

static void nb2cannedvvtTable1RpmBins() {
	static const float hardCodedvvtTable1RpmBins[8] = {950.0, 1800.0, 2500.0, 3300.0, 4200.0, 5000.0, 6000.0, 7200.0};
	copyArray(config->vvtTable1RpmBins, hardCodedvvtTable1RpmBins);
}

static void nb2cannedvvtTable1() {
	static const float hardCodedvvtTable1[8][8] = {
{0.000,	0.000,	10.000,	10.000,	10.000,	10.000,	0.000,	0.000,	},
{0.000,	0.000,	15.000,	15.000,	15.000,	15.000,	0.000,	0.000,	},
{0.000,	0.000,	20.000,	20.000,	20.000,	18.000,	12.000,	8.000,	},
{6.000,	12.000,	30.000,	25.000,	25.000,	15.000,	12.000,	8.000,	},
{6.000,	12.000,	30.000,	25.000,	25.000,	15.000,	12.000,	8.000,	},
{6.000,	12.000,	30.000,	25.000,	25.000,	15.000,	12.000,	8.000,	},
{6.000,	12.000,	30.000,	25.000,	25.000,	15.000,	12.000,	8.000,	},
{6.000,	12.000,	30.000,	25.000,	25.000,	15.000,	12.000,	8.000,	},
};
	copyTable(config->vvtTable1, hardCodedvvtTable1);
}

static void nb2cannedscriptTable4RpmBins() {
	static const float hardCodedscriptTable4RpmBins[10] = {650.0, 800.0, 1700.0, 2600.0, 3500.0, 4400.0, 5300.0, 7000.0, 0.0, 0.0};
	copyArray(config->scriptTable4RpmBins, hardCodedscriptTable4RpmBins);
}

static void nb2cannedscriptTable4() {
	static const float hardCodedscriptTable4[8][10] = {
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
};
	copyTable(config->scriptTable4, hardCodedscriptTable4);
}

static void nb2cannedignitionRpmBins() {
	static const float hardCodedignitionRpmBins[16] = {700.0, 900.0, 1100.0, 1500.0, 1800.0, 2300.0, 2800.0, 3200.0, 3700.0, 4200.0, 4700.0, 5000.0, 5250.0, 5800.0, 6200.0, 7200.0};
	copyArray(config->ignitionRpmBins, hardCodedignitionRpmBins);
}

static void nb2cannedignitionLoadBins() {
	static const float hardCodedignitionLoadBins[16] = {25.0, 35.0, 45.0, 55.0, 65.0, 75.0, 85.0, 95.0, 100.0, 110.0, 130.0, 150.0, 170.0, 190.0, 205.0, 220.0};
	copyArray(config->ignitionLoadBins, hardCodedignitionLoadBins);
}

static void nb2cannedignitionTable() {
	static const float hardCodedignitionTable[16][16] = {
{16.000,	16.000,	16.000,	16.000,	16.000,	16.000,	15.000,	6.100,	-3.000,	-3.000,	-3.000,	-3.000,	-2.000,	-2.000,	-2.000,	-2.000,	},
{16.000,	10.000,	10.000,	20.000,	24.000,	24.000,	24.000,	26.700,	30.000,	30.000,	30.000,	30.000,	30.000,	30.000,	30.000,	30.000,	},
{16.000,	10.000,	10.000,	20.000,	26.700,	26.400,	28.500,	31.200,	32.000,	32.700,	33.000,	33.300,	33.900,	34.100,	34.300,	35.700,	},
{16.800,	18.900,	20.100,	22.400,	27.000,	27.000,	29.100,	32.700,	32.000,	32.800,	33.700,	33.100,	34.200,	35.900,	37.300,	37.800,	},
{16.800,	18.900,	20.500,	23.700,	25.800,	28.500,	30.600,	31.500,	32.000,	32.800,	32.900,	33.400,	34.400,	35.600,	35.600,	35.900,	},
{15.800,	15.800,	17.700,	21.700,	26.200,	28.600,	30.800,	33.100,	31.200,	31.500,	31.900,	32.400,	32.800,	33.700,	33.800,	34.100,	},
{15.800,	15.800,	17.100,	19.700,	25.300,	26.800,	29.000,	31.400,	30.000,	31.000,	31.100,	31.200,	31.500,	32.100,	32.500,	33.000,	},
{15.800,	15.800,	16.500,	17.900,	23.800,	25.100,	26.700,	29.000,	27.900,	29.000,	29.000,	29.100,	29.400,	30.000,	30.400,	31.200,	},
{15.800,	15.800,	16.100,	16.800,	23.000,	24.200,	25.600,	26.000,	26.800,	27.900,	27.900,	28.100,	28.400,	29.000,	29.400,	30.300,	},
{15.000,	15.000,	15.400,	16.100,	16.600,	17.900,	19.000,	21.100,	22.400,	23.100,	23.400,	23.900,	24.200,	24.800,	25.200,	26.000,	},
{15.000,	15.000,	15.000,	12.700,	14.800,	15.800,	16.900,	18.800,	20.100,	20.800,	21.100,	21.600,	22.000,	22.700,	23.100,	24.000,	},
{6.500,	7.600,	9.000,	11.300,	12.900,	13.700,	14.700,	16.400,	17.800,	18.500,	18.900,	19.400,	19.900,	20.600,	21.000,	21.900,	},
{5.900,	6.700,	7.800,	9.800,	11.000,	11.700,	12.600,	14.000,	15.500,	16.200,	16.600,	17.200,	17.700,	18.600,	19.000,	19.900,	},
{5.300,	5.800,	6.700,	8.400,	9.200,	9.600,	10.400,	11.700,	13.200,	13.900,	14.300,	15.000,	15.500,	16.500,	16.900,	17.800,	},
{4.800,	5.100,	5.800,	7.300,	7.800,	8.100,	8.800,	9.900,	11.500,	12.200,	12.600,	13.300,	13.900,	15.000,	15.300,	16.300,	},
{4.400,	4.400,	4.900,	6.200,	6.400,	6.500,	7.200,	8.100,	9.700,	10.400,	10.800,	11.600,	12.300,	13.400,	13.800,	14.700,	},
};
	copyTable(config->ignitionTable, hardCodedignitionTable);
}

static void nb2cannedignitionIatCorrLoadBins() {
	static const float hardCodedignitionIatCorrLoadBins[8] = {-40.0, -30.0, -20.0, -10.0, 0.0, 10.0, 20.0, 30.0};
	copyArray(config->ignitionIatCorrLoadBins, hardCodedignitionIatCorrLoadBins);
}

static void nb2cannedignitionIatCorrTable() {
	static const float hardCodedignitionIatCorrTable[8][8] = {
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
};
	copyTable(config->ignitionIatCorrTable, hardCodedignitionIatCorrTable);
}

static void nb2cannedveRpmBins() {
	static const float hardCodedveRpmBins[16] = {700.0, 900.0, 1100.0, 1500.0, 1800.0, 2300.0, 2800.0, 3200.0, 3700.0, 4200.0, 4700.0, 5250.0, 5700.0, 6200.0, 6700.0, 7200.0};
	copyArray(config->veRpmBins, hardCodedveRpmBins);
}

static void nb2cannedveLoadBins() {
	static const float hardCodedveLoadBins[16] = {20.0, 25.0, 32.0, 36.0, 40.0, 45.0, 50.0, 55.0, 65.0, 75.0, 85.0, 90.0, 95.0, 100.0, 105.0, 110.0};
	copyArray(config->veLoadBins, hardCodedveLoadBins);
}

static void nb2cannedveTable() {
	static const float hardCodedveTable[16][16] = {
{13.500,	22.000,	22.000,	19.700,	19.000,	19.000,	19.000,	21.000,	24.000,	24.000,	24.000,	23.500,	22.500,	22.800,	22.800,	22.200,	},
{40.000,	46.200,	37.700,	38.000,	48.000,	45.200,	43.100,	42.500,	35.400,	40.900,	45.300,	45.800,	37.600,	38.500,	36.300,	38.100,	},
{42.000,	40.200,	44.800,	41.100,	48.700,	44.700,	49.200,	47.400,	48.500,	57.200,	64.600,	73.700,	68.200,	60.000,	58.300,	56.500,	},
{41.400,	42.200,	45.600,	45.900,	51.900,	47.100,	50.800,	46.300,	48.700,	54.900,	68.600,	72.500,	72.800,	63.000,	61.400,	59.800,	},
{43.200,	44.800,	50.400,	52.800,	52.700,	48.800,	52.000,	50.700,	48.700,	54.700,	72.300,	71.400,	68.400,	66.000,	63.100,	60.100,	},
{49.500,	49.900,	52.600,	56.700,	52.500,	50.400,	54.500,	50.100,	47.600,	58.000,	74.100,	72.700,	70.300,	66.100,	64.200,	62.300,	},
{55.500,	56.400,	56.000,	57.100,	54.100,	52.000,	56.600,	48.000,	47.300,	58.000,	72.200,	74.300,	73.900,	70.600,	67.200,	63.800,	},
{60.900,	63.500,	61.400,	52.900,	55.600,	56.700,	59.400,	50.700,	49.200,	61.400,	77.300,	80.000,	74.300,	72.200,	67.900,	63.500,	},
{72.200,	69.200,	62.800,	56.500,	59.100,	61.600,	60.300,	52.700,	51.600,	68.200,	79.900,	77.900,	72.300,	68.600,	65.700,	63.100,	},
{74.500,	78.300,	69.800,	62.200,	61.200,	64.700,	64.900,	55.100,	57.400,	74.400,	81.900,	79.200,	77.100,	75.400,	67.900,	65.900,	},
{83.500,	79.600,	76.100,	65.300,	65.300,	65.300,	65.200,	64.000,	62.500,	76.600,	81.800,	82.500,	76.700,	77.300,	70.800,	69.300,	},
{73.000,	82.900,	88.000,	63.300,	62.900,	62.700,	66.300,	61.200,	66.400,	78.800,	80.000,	78.900,	75.200,	76.400,	72.700,	67.400,	},
{73.400,	78.700,	87.800,	66.600,	64.700,	67.300,	66.600,	61.700,	67.800,	78.100,	82.300,	83.000,	82.000,	81.400,	80.500,	78.200,	},
{69.700,	83.700,	76.900,	62.400,	65.100,	68.100,	65.600,	62.400,	68.700,	76.400,	83.000,	84.500,	83.200,	82.500,	81.300,	77.700,	},
{75.400,	81.100,	78.300,	71.600,	71.800,	72.700,	70.000,	70.100,	70.700,	76.400,	89.300,	90.000,	89.300,	82.500,	77.700,	72.900,	},
{78.000,	78.200,	78.700,	74.000,	73.300,	71.000,	71.300,	70.700,	73.000,	86.400,	89.000,	86.900,	86.300,	83.600,	80.300,	78.400,	},
};
	copyTable(config->veTable, hardCodedveTable);
}

static void nb2cannedidleVeRpmBins() {
	static const float hardCodedidleVeRpmBins[4] = {800.0, 900.0, 1100.0, 1200.0};
	copyArray(config->idleVeRpmBins, hardCodedidleVeRpmBins);
}

static void nb2cannedidleVeLoadBins() {
	static const float hardCodedidleVeLoadBins[4] = {30.0, 35.0, 40.0, 45.0};
	copyArray(config->idleVeLoadBins, hardCodedidleVeLoadBins);
}

static void nb2cannedidleVeTable() {
	static const float hardCodedidleVeTable[4][4] = {
{41.000,	41.000,	41.000,	43.000,	},
{42.000,	40.000,	40.000,	43.300,	},
{42.000,	40.000,	45.000,	47.000,	},
{47.000,	45.000,	47.000,	47.000,	},
};
	copyTable(config->idleVeTable, hardCodedidleVeTable);
}

static void nb2cannedfuelTrimRpmBins() {
	static const float hardCodedfuelTrimRpmBins[4] = {0.0, 0.0, 0.0, 0.0};
	copyArray(config->fuelTrimRpmBins, hardCodedfuelTrimRpmBins);
}

static void nb2cannedfuelTrimLoadBins() {
	static const float hardCodedfuelTrimLoadBins[4] = {0.0, 0.0, 0.0, 0.0};
	copyArray(config->fuelTrimLoadBins, hardCodedfuelTrimLoadBins);
}

static void nb2cannedignTrimRpmBins() {
	static const float hardCodedignTrimRpmBins[4] = {0.0, 0.0, 0.0, 0.0};
	copyArray(config->ignTrimRpmBins, hardCodedignTrimRpmBins);
}

static void nb2cannedignTrimLoadBins() {
	static const float hardCodedignTrimLoadBins[4] = {0.0, 0.0, 0.0, 0.0};
	copyArray(config->ignTrimLoadBins, hardCodedignTrimLoadBins);
}

static void nb2cannedmapEstimateRpmBins() {
	static const float hardCodedmapEstimateRpmBins[16] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->mapEstimateRpmBins, hardCodedmapEstimateRpmBins);
}

static void nb2cannedmapEstimateTpsBins() {
	static const float hardCodedmapEstimateTpsBins[16] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->mapEstimateTpsBins, hardCodedmapEstimateTpsBins);
}

static void nb2cannedmapEstimateTable() {
	static const float hardCodedmapEstimateTable[16][16] = {
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
};
	copyTable(config->mapEstimateTable, hardCodedmapEstimateTable);
}

static void nb2cannedinjectionPhase() {
	static const float hardCodedinjectionPhase[16][16] = {
{-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	},
{-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	},
{-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	},
{-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	},
{-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	},
{-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	},
{-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	},
{-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	},
{-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	},
{-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	},
{-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	},
{-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	},
{-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	},
{-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	},
{-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	},
{-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	-180.000,	},
};
	copyTable(config->injectionPhase, hardCodedinjectionPhase);
}

static void nb2cannedpedalToTpsRpmBins() {
	static const float hardCodedpedalToTpsRpmBins[8] = {0.0, 1150.0, 2300.0, 3450.0, 4550.0, 5700.0, 6850.0, 8000.0};
	copyArray(config->pedalToTpsRpmBins, hardCodedpedalToTpsRpmBins);
}

static void nb2cannedlambdaRpmBins() {
	static const float hardCodedlambdaRpmBins[16] = {700.0, 900.0, 1100.0, 1500.0, 1700.0, 2300.0, 2800.0, 3200.0, 3700.0, 4200.0, 4700.0, 5250.0, 5700.0, 6200.0, 6700.0, 7200.0};
	copyArray(config->lambdaRpmBins, hardCodedlambdaRpmBins);
}

static void nb2cannedlambdaLoadBins() {
	static const float hardCodedlambdaLoadBins[16] = {20.0, 35.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0, 110.0, 120.0, 130.0, 150.0, 175.0, 200.0, 225.0, 250.0};
	copyArray(config->lambdaLoadBins, hardCodedlambdaLoadBins);
}

static void nb2cannedlambdaTable() {
	static const float hardCodedlambdaTable[16][16] = {
{14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	15.400,	15.400,	15.400,	15.400,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	},
{14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	15.400,	15.400,	15.400,	15.400,	14.500,	14.200,	14.200,	14.200,	14.200,	14.200,	},
{14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	15.400,	15.400,	15.400,	15.400,	14.500,	14.200,	14.200,	14.200,	14.200,	14.200,	},
{14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	15.000,	15.000,	15.000,	15.000,	14.500,	14.200,	14.200,	14.200,	14.200,	14.200,	},
{14.500,	14.500,	14.500,	14.500,	14.500,	14.500,	14.500,	14.500,	14.500,	14.500,	14.500,	14.000,	14.000,	14.000,	14.000,	14.000,	},
{14.200,	14.200,	14.200,	14.200,	14.200,	14.200,	14.200,	14.200,	14.200,	14.200,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	},
{14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	13.800,	13.800,	13.800,	13.800,	13.800,	13.800,	},
{13.800,	13.800,	13.800,	13.800,	13.800,	13.800,	13.800,	13.800,	13.800,	13.800,	13.600,	13.600,	13.600,	13.600,	13.600,	13.600,	},
{13.800,	13.800,	13.800,	13.800,	13.800,	13.800,	13.800,	13.800,	13.800,	13.800,	13.600,	13.600,	13.600,	13.600,	13.600,	13.600,	},
{13.700,	13.700,	13.700,	13.700,	13.700,	13.700,	13.700,	13.700,	13.700,	13.700,	13.400,	13.400,	13.400,	13.400,	13.400,	13.400,	},
{13.500,	13.500,	13.500,	13.500,	13.500,	13.500,	13.500,	13.500,	13.500,	13.500,	13.200,	13.200,	13.200,	13.200,	13.200,	13.200,	},
{13.100,	13.100,	13.100,	13.100,	13.100,	13.100,	13.100,	13.100,	13.100,	13.100,	13.100,	13.100,	13.100,	13.100,	13.100,	13.100,	},
{12.500,	12.500,	12.500,	12.500,	12.500,	12.500,	12.500,	12.500,	12.500,	12.500,	12.500,	12.500,	12.500,	12.500,	12.500,	12.500,	},
{12.000,	12.000,	12.000,	12.000,	12.000,	12.000,	12.000,	12.000,	12.000,	12.000,	12.000,	12.000,	12.000,	12.000,	12.000,	12.000,	},
{11.500,	11.500,	11.500,	11.500,	11.500,	11.500,	11.500,	11.500,	11.500,	11.500,	11.500,	11.500,	11.500,	11.500,	11.500,	11.500,	},
{11.000,	11.000,	11.000,	11.000,	11.000,	11.000,	11.000,	11.000,	11.000,	11.000,	11.000,	11.000,	11.000,	11.000,	11.000,	11.000,	},
};
	copyTable(config->lambdaTable, hardCodedlambdaTable, 1.0 / 14.7);
}

static void nb2cannedtcuSolenoidTable() {
	static const float hardCodedtcuSolenoidTable[6][10] = {
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
};
	copyTable(config->tcuSolenoidTable, hardCodedtcuSolenoidTable);
}

static void nb2cannedhpfpTargetRpmBins() {
	static const float hardCodedhpfpTargetRpmBins[10] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->hpfpTargetRpmBins, hardCodedhpfpTargetRpmBins);
}

static void nb2cannedhpfpTargetLoadBins() {
	static const float hardCodedhpfpTargetLoadBins[10] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->hpfpTargetLoadBins, hardCodedhpfpTargetLoadBins);
}

static void nb2cannedhpfpCompensationRpmBins() {
	static const float hardCodedhpfpCompensationRpmBins[10] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->hpfpCompensationRpmBins, hardCodedhpfpCompensationRpmBins);
}

static void nb2cannedhpfpCompensationLoadBins() {
	static const float hardCodedhpfpCompensationLoadBins[10] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->hpfpCompensationLoadBins, hardCodedhpfpCompensationLoadBins);
}

static void nb2cannedpostCrankingFactor() {
	static const float hardCodedpostCrankingFactor[6][6] = {
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
};
	copyTable(engineConfiguration->postCrankingFactor, hardCodedpostCrankingFactor);
}

```
