```
static void cannedprimeBins() {
	static const float hardCodedprimeBins[8] = {0.0, 14.0, 29.0, 43.0, 57.0, 71.0, 86.0, 100.0};
	copyArray(engineConfiguration->primeBins, hardCodedprimeBins);
}

static void cannedprimeValues() {
	static const float hardCodedprimeValues[8] = {10.0, 10.0, 10.0, 10.0, 10.0, 5.0, 5.0, 5.0};
	copyArray(engineConfiguration->primeValues, hardCodedprimeValues);
}

static void cannedmap_samplingAngleBins() {
	static const float hardCodedmap_samplingAngleBins[8] = {800.0, 1686.0, 2571.0, 3457.0, 4343.0, 5229.0, 6114.0, 7000.0};
	copyArray(engineConfiguration->map.map_samplingAngleBins, hardCodedmap_samplingAngleBins);
}

static void cannedmap_samplingAngle() {
	static const float hardCodedmap_samplingAngle[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->map.map_samplingAngle, hardCodedmap_samplingAngle);
}

static void cannedmap_samplingWindowBins() {
	static const float hardCodedmap_samplingWindowBins[8] = {800.0, 1686.0, 2571.0, 3457.0, 4343.0, 5229.0, 6114.0, 7000.0};
	copyArray(engineConfiguration->map.map_samplingWindowBins, hardCodedmap_samplingWindowBins);
}

static void cannedmap_samplingWindow() {
	static const float hardCodedmap_samplingWindow[8] = {100.0, 100.0, 100.0, 400.0, 500.0, 500.0, 500.0, 500.0};
	copyArray(engineConfiguration->map.map_samplingWindow, hardCodedmap_samplingWindow);
}

static void cannedinjector_battLagCorrBins() {
	static const float hardCodedinjector_battLagCorrBins[8] = {6.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0};
	copyArray(engineConfiguration->injectorSecondary.injector_battLagCorrBins, hardCodedinjector_battLagCorrBins);
}

static void cannedinjector_battLagCorr() {
	static const float hardCodedinjector_battLagCorr[8] = {3.171, 1.28, 1.09, 0.94, 0.81, 0.7, 0.5, 0.3};
	copyArray(engineConfiguration->injectorSecondary.injector_battLagCorr, hardCodedinjector_battLagCorr);
}

static void cannedvrThreshold1_rpmBins() {
	static const float hardCodedvrThreshold1_rpmBins[6] = {600.0, 1900.0, 3200.0, 4400.0, 5700.0, 7000.0};
	copyArray(engineConfiguration->vrThreshold2.vrThreshold1_rpmBins, hardCodedvrThreshold1_rpmBins);
}

static void cannedvrThreshold2_rpmBins() {
	static const float hardCodedvrThreshold2_rpmBins[6] = {600.0, 1900.0, 3200.0, 4400.0, 5700.0, 7000.0};
	copyArray(engineConfiguration->vrThreshold2.vrThreshold2_rpmBins, hardCodedvrThreshold2_rpmBins);
}

static void cannedmafDecodingBins() {
	static const float hardCodedmafDecodingBins[32] = {0.0, 18.225807, 26.451612, 34.677418, 42.903225, 51.129032, 59.35484, 67.58064, 75.80645, 84.03226, 92.258064, 100.48387, 108.70968, 116.935486, 125.16129, 133.3871, 141.6129, 149.83871, 158.06451, 166.29033, 174.51613, 182.74193, 190.96774, 199.19354, 207.41936, 215.64516, 223.87097, 232.09677, 240.32259, 248.54839, 256.7742, 265.0};
	copyArray(config->mafDecodingBins, hardCodedmafDecodingBins);
}

static void cannedmafDecoding() {
	static const float hardCodedmafDecoding[32] = {-34.5, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0};
	copyArray(config->mafDecoding, hardCodedmafDecoding);
}

static void cannedcltFuelCorr() {
	static const float hardCodedcltFuelCorr[16] = {1.5, 1.5, 1.42, 1.36, 1.28, 1.19, 1.12, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
	copyArray(config->cltFuelCorr, hardCodedcltFuelCorr);
}

static void cannedcrankingFuelCoef() {
	static const float hardCodedcrankingFuelCoef[8] = {2.8, 2.2, 1.8, 1.8, 1.6, 1.5, 1.0, 1.0};
	copyArray(config->crankingFuelCoef, hardCodedcrankingFuelCoef);
}

static void cannedcltRevLimitRpmBins() {
	static const float hardCodedcltRevLimitRpmBins[4] = {0.0, 0.0, 0.0, 0.0};
	copyArray(config->cltRevLimitRpmBins, hardCodedcltRevLimitRpmBins);
}

static void cannedcltIdleCorr() {
	static const float hardCodedcltIdleCorr[16] = {2.0, 2.0, 2.0, 2.0, 1.6, 1.6, 1.6, 1.2, 1.2, 0.8, 0.5, 0.45, 0.45, 0.45, 0.45, 0.76};
	copyArray(config->cltIdleCorr, hardCodedcltIdleCorr);
}

static void cannedcltCrankingCorr() {
	static const float hardCodedcltCrankingCorr[8] = {1.2, 1.2, 1.0, 0.85, 0.8, 0.35, 0.35, 0.35};
	copyArray(config->cltCrankingCorr, hardCodedcltCrankingCorr);
}

static void cannedcltIdleRpm() {
	static const float hardCodedcltIdleRpm[16] = {1360.0, 1360.0, 1300.0, 1200.0, 1160.0, 1100.0, 1060.0, 1000.0, 1000.0, 860.0, 860.0, 800.0, 800.0, 800.0, 1000.0, 1100.0};
	copyArray(config->cltIdleRpm, hardCodedcltIdleRpm);
}

static void cannedidleAdvanceBins() {
	static const float hardCodedidleAdvanceBins[8] = {500.0, 650.0, 750.0, 900.0, 1000.0, 1150.0, 1250.0, 1400.0};
	copyArray(config->idleAdvanceBins, hardCodedidleAdvanceBins);
}

static void cannedidleAdvance() {
	static const float hardCodedidleAdvance[8] = {8.0, 10.0, 12.0, 14.0, 15.0, 16.0, 18.0, 20.0};
	copyArray(config->idleAdvance, hardCodedidleAdvance);
}

static void cannedfuelLevelBins() {
	static const float hardCodedfuelLevelBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->fuelLevelBins, hardCodedfuelLevelBins);
}

static void cannedignBlends1_blendBins() {
	static const float hardCodedignBlends1_blendBins[8] = {0.0, 14.3, 28.6, 40.0, 60.0, 71.4, 85.7, 100.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends1_blendBins, hardCodedignBlends1_blendBins);
}

static void cannedignBlends1_blendValues() {
	static const float hardCodedignBlends1_blendValues[8] = {100.0, 100.0, 100.0, 100.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends1_blendValues, hardCodedignBlends1_blendValues);
}

static void cannedignBlends2_blendValues() {
	static const float hardCodedignBlends2_blendValues[8] = {100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends2_blendValues, hardCodedignBlends2_blendValues);
}

static void cannedignBlends3_blendBins() {
	static const float hardCodedignBlends3_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends3_blendBins, hardCodedignBlends3_blendBins);
}

static void cannedignBlends3_blendValues() {
	static const float hardCodedignBlends3_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends3_blendValues, hardCodedignBlends3_blendValues);
}

static void cannedignBlends4_blendBins() {
	static const float hardCodedignBlends4_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends4_blendBins, hardCodedignBlends4_blendBins);
}

static void cannedignBlends4_blendValues() {
	static const float hardCodedignBlends4_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends4_blendValues, hardCodedignBlends4_blendValues);
}

static void cannedveBlends1_blendBins() {
	static const float hardCodedveBlends1_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends1_blendBins, hardCodedveBlends1_blendBins);
}

static void cannedveBlends1_blendValues() {
	static const float hardCodedveBlends1_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends1_blendValues, hardCodedveBlends1_blendValues);
}

static void cannedveBlends2_blendBins() {
	static const float hardCodedveBlends2_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends2_blendBins, hardCodedveBlends2_blendBins);
}

static void cannedveBlends2_blendValues() {
	static const float hardCodedveBlends2_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends2_blendValues, hardCodedveBlends2_blendValues);
}

static void cannedveBlends3_blendBins() {
	static const float hardCodedveBlends3_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends3_blendBins, hardCodedveBlends3_blendBins);
}

static void cannedveBlends3_blendValues() {
	static const float hardCodedveBlends3_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends3_blendValues, hardCodedveBlends3_blendValues);
}

static void cannedveBlends4_blendBins() {
	static const float hardCodedveBlends4_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends4_blendBins, hardCodedveBlends4_blendBins);
}

static void cannedveBlends4_blendValues() {
	static const float hardCodedveBlends4_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends4_blendValues, hardCodedveBlends4_blendValues);
}

static void cannedtpsTpsAccelTable() {
	static const float hardCodedtpsTpsAccelTable[8][8] = {
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{1.200,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{2.040,	1.200,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{2.760,	2.040,	1.200,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{3.360,	2.760,	2.040,	1.200,	0.000,	0.000,	0.000,	0.000,	},
{3.840,	3.360,	2.760,	2.040,	1.200,	0.000,	0.000,	0.000,	},
{4.200,	3.840,	3.360,	2.760,	2.040,	1.200,	0.000,	0.000,	},
{4.440,	4.200,	3.840,	3.360,	2.760,	2.040,	1.200,	0.000,	},
};
	copyTable(config->tpsTpsAccelTable, hardCodedtpsTpsAccelTable);
}

static void cannedtractionControlSlipBins() {
	static const float hardCodedtractionControlSlipBins[6] = {0.8, 0.9, 0.95, 1.05, 1.1, 1.2};
	copyArray(engineConfiguration->tractionControlSlipBins, hardCodedtractionControlSlipBins);
}

static void cannedboostRpmBins() {
	static const float hardCodedboostRpmBins[8] = {0.0, 1100.0, 2300.0, 3400.0, 4600.0, 5700.0, 6900.0, 8000.0};
	copyArray(config->boostRpmBins, hardCodedboostRpmBins);
}

static void cannedboostTableOpenLoop() {
	static const float hardCodedboostTableOpenLoop[8][8] = {
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	},
{29.000,	29.000,	29.000,	29.000,	29.000,	29.000,	29.000,	29.000,	},
{43.000,	43.000,	43.000,	43.000,	43.000,	43.000,	43.000,	43.000,	},
{57.000,	57.000,	57.000,	57.000,	57.000,	57.000,	57.000,	57.000,	},
{71.000,	71.000,	71.000,	71.000,	71.000,	71.000,	71.000,	71.000,	},
{86.000,	86.000,	86.000,	86.000,	86.000,	86.000,	86.000,	86.000,	},
{100.000,	100.000,	100.000,	100.000,	100.000,	100.000,	100.000,	100.000,	},
};
	copyTable(config->boostTableOpenLoop, hardCodedboostTableOpenLoop);
}

static void cannedvvtTable1RpmBins() {
	static const float hardCodedvvtTable1RpmBins[8] = {1000.0, 1700.0, 2000.0, 4400.0, 4500.0, 5000.0, 6000.0, 7000.0};
	copyArray(config->vvtTable1RpmBins, hardCodedvvtTable1RpmBins);
}

static void cannedvvtTable1() {
	static const float hardCodedvvtTable1[8][8] = {
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{2.000,	5.000,	16.000,	16.000,	8.000,	5.000,	0.000,	0.000,	},
{3.000,	7.000,	24.000,	24.000,	12.000,	8.000,	0.000,	0.000,	},
{3.000,	10.000,	40.000,	40.000,	20.000,	13.000,	0.000,	0.000,	},
{3.000,	10.000,	40.000,	40.000,	20.000,	13.000,	0.000,	0.000,	},
{3.000,	10.000,	40.000,	40.000,	20.000,	13.000,	0.000,	0.000,	},
{3.000,	10.000,	40.000,	40.000,	20.000,	13.000,	0.000,	0.000,	},
};
	copyTable(config->vvtTable1, hardCodedvvtTable1);
}

static void cannedscriptTable4RpmBins() {
	static const float hardCodedscriptTable4RpmBins[10] = {650.0, 767.0, 1300.0, 2000.0, 2700.0, 3400.0, 4100.0, 4800.0, 5678.0, 7000.0};
	copyArray(config->scriptTable4RpmBins, hardCodedscriptTable4RpmBins);
}

static void cannedscriptTable4() {
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

static void cannedignitionRpmBins() {
	static const float hardCodedignitionRpmBins[16] = {650.0, 800.0, 1100.0, 1500.0, 2000.0, 2500.0, 3000.0, 3500.0, 4000.0, 4500.0, 5000.0, 5500.0, 6000.0, 6500.0, 7000.0, 8000.0};
	copyArray(config->ignitionRpmBins, hardCodedignitionRpmBins);
}

static void cannedignitionLoadBins() {
	static const float hardCodedignitionLoadBins[16] = {0.0, 2.0, 4.0, 6.0, 8.0, 11.0, 12.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0};
	copyArray(config->ignitionLoadBins, hardCodedignitionLoadBins);
}

static void cannedignitionTable() {
	static const float hardCodedignitionTable[16][16] = {
{12.100,	10.000,	10.000,	10.000,	29.300,	16.600,	30.000,	40.000,	40.000,	40.000,	41.000,	42.000,	42.000,	42.000,	42.000,	42.000,	},
{15.000,	15.000,	15.000,	19.200,	24.500,	29.700,	35.000,	38.000,	0.000,	0.000,	0.000,	0.000,	40.000,	40.000,	40.000,	42.000,	},
{15.000,	15.000,	10.000,	10.000,	10.000,	10.000,	35.000,	38.000,	0.000,	0.000,	0.000,	0.000,	40.000,	40.000,	40.000,	42.000,	},
{15.000,	15.000,	10.000,	10.000,	10.000,	10.000,	35.000,	38.000,	-5.000,	-5.000,	-5.000,	-5.000,	42.000,	42.000,	42.000,	42.000,	},
{15.000,	15.000,	15.000,	19.200,	24.500,	30.000,	35.000,	38.000,	-5.000,	-5.000,	-5.000,	-5.000,	42.000,	42.000,	42.000,	42.000,	},
{15.000,	15.000,	15.000,	19.200,	24.500,	30.000,	35.000,	38.000,	-5.000,	-5.000,	-5.000,	42.000,	42.000,	42.000,	42.000,	42.000,	},
{15.000,	16.100,	18.200,	21.000,	24.500,	29.700,	30.000,	40.000,	40.000,	40.000,	41.000,	42.000,	42.000,	42.000,	42.000,	42.000,	},
{15.000,	16.100,	18.200,	21.000,	24.500,	29.700,	30.000,	40.000,	40.000,	40.000,	41.000,	42.000,	42.000,	42.000,	42.000,	42.000,	},
{15.000,	16.100,	18.200,	21.000,	24.500,	29.700,	30.000,	40.000,	40.000,	40.000,	41.000,	42.000,	42.000,	42.000,	42.000,	42.000,	},
{15.000,	16.000,	17.900,	20.500,	23.700,	27.500,	29.400,	30.800,	31.100,	31.500,	31.800,	32.200,	32.600,	32.900,	33.300,	34.000,	},
{15.000,	15.900,	17.700,	20.200,	23.200,	27.000,	28.900,	30.300,	30.600,	31.000,	31.300,	31.700,	32.100,	32.400,	32.800,	33.500,	},
{15.000,	15.900,	17.600,	19.800,	22.700,	26.500,	28.400,	29.800,	30.100,	30.500,	30.800,	31.200,	31.600,	31.900,	32.300,	33.000,	},
{15.000,	15.800,	17.400,	19.500,	22.200,	26.000,	27.900,	29.300,	29.600,	30.000,	30.300,	30.700,	31.100,	31.400,	31.800,	32.500,	},
{15.000,	15.700,	17.200,	19.200,	21.700,	25.500,	27.400,	28.800,	29.100,	29.500,	29.800,	30.200,	30.600,	30.900,	31.300,	32.000,	},
{15.000,	15.700,	17.100,	18.900,	21.200,	25.000,	26.900,	28.300,	28.600,	29.000,	29.300,	29.700,	30.100,	30.400,	30.800,	31.500,	},
{15.000,	15.600,	16.900,	18.600,	20.700,	24.500,	26.400,	27.800,	28.100,	28.500,	28.800,	29.200,	29.600,	29.900,	30.300,	31.000,	},
};
	copyTable(config->ignitionTable, hardCodedignitionTable);
}

static void cannedignBlends1_rpmBins() {
	static const float hardCodedignBlends1_rpmBins[8] = {0.0, 400.0, 500.0, 1000.0, 2000.0, 3000.0, 5000.0, 7000.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends1_rpmBins, hardCodedignBlends1_rpmBins);
}

static void cannedignBlends1_loadBins() {
	static const float hardCodedignBlends1_loadBins[8] = {10.0, 15.0, 20.0, 30.0, 60.0, 90.0, 100.0, 120.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends1_loadBins, hardCodedignBlends1_loadBins);
}

static void cannedignBlends1_table() {
	static const float hardCodedignBlends1_table[8][8] = {
{0.000,	0.000,	-20.000,	-20.000,	-20.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
};
	copyTable(engineConfiguration->boostClosedLoopBlends2.ignBlends1_table, hardCodedignBlends1_table);
}

static void cannedignBlends2_rpmBins() {
	static const float hardCodedignBlends2_rpmBins[8] = {0.0, 500.0, 2500.0, 3000.0, 5000.0, 6000.0, 7000.0, 8000.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends2_rpmBins, hardCodedignBlends2_rpmBins);
}

static void cannedignBlends2_loadBins() {
	static const float hardCodedignBlends2_loadBins[8] = {0.0, 5.0, 10.0, 20.0, 40.0, 60.0, 80.0, 100.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends2_loadBins, hardCodedignBlends2_loadBins);
}

static void cannedignBlends2_table() {
	static const float hardCodedignBlends2_table[8][8] = {
{0.000,	0.000,	0.000,	-20.000,	-20.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	-20.000,	-20.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
};
	copyTable(engineConfiguration->boostClosedLoopBlends2.ignBlends2_table, hardCodedignBlends2_table);
}

static void cannedignBlends3_rpmBins() {
	static const float hardCodedignBlends3_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends3_rpmBins, hardCodedignBlends3_rpmBins);
}

static void cannedignBlends3_loadBins() {
	static const float hardCodedignBlends3_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends3_loadBins, hardCodedignBlends3_loadBins);
}

static void cannedignBlends4_rpmBins() {
	static const float hardCodedignBlends4_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends4_rpmBins, hardCodedignBlends4_rpmBins);
}

static void cannedignBlends4_loadBins() {
	static const float hardCodedignBlends4_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends4_loadBins, hardCodedignBlends4_loadBins);
}

static void cannedveBlends1_rpmBins() {
	static const float hardCodedveBlends1_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends1_rpmBins, hardCodedveBlends1_rpmBins);
}

static void cannedveBlends1_loadBins() {
	static const float hardCodedveBlends1_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends1_loadBins, hardCodedveBlends1_loadBins);
}

static void cannedveBlends2_rpmBins() {
	static const float hardCodedveBlends2_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends2_rpmBins, hardCodedveBlends2_rpmBins);
}

static void cannedveBlends2_loadBins() {
	static const float hardCodedveBlends2_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends2_loadBins, hardCodedveBlends2_loadBins);
}

static void cannedveBlends3_rpmBins() {
	static const float hardCodedveBlends3_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends3_rpmBins, hardCodedveBlends3_rpmBins);
}

static void cannedveBlends3_loadBins() {
	static const float hardCodedveBlends3_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends3_loadBins, hardCodedveBlends3_loadBins);
}

static void cannedveBlends4_rpmBins() {
	static const float hardCodedveBlends4_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends4_rpmBins, hardCodedveBlends4_rpmBins);
}

static void cannedveBlends4_loadBins() {
	static const float hardCodedveBlends4_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends4_loadBins, hardCodedveBlends4_loadBins);
}

static void cannedignitionIatCorrLoadBins() {
	static const float hardCodedignitionIatCorrLoadBins[8] = {5.0, 15.0, 25.0, 35.0, 45.0, 65.0, 90.0, 110.0};
	copyArray(config->ignitionIatCorrLoadBins, hardCodedignitionIatCorrLoadBins);
}

static void cannedignitionIatCorrTable() {
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

static void cannedveRpmBins() {
	static const float hardCodedveRpmBins[16] = {600.0, 900.0, 1100.0, 1500.0, 1900.0, 2400.0, 3000.0, 3700.0, 4200.0, 4700.0, 5200.0, 5800.0, 6300.0, 6800.0, 7000.0, 7200.0};
	copyArray(config->veRpmBins, hardCodedveRpmBins);
}

static void cannedveLoadBins() {
	static const float hardCodedveLoadBins[16] = {0.0, 2.0, 4.0, 6.0, 8.0, 10.0, 15.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0};
	copyArray(config->veLoadBins, hardCodedveLoadBins);
}

static void cannedveTable() {
	static const float hardCodedveTable[16][16] = {
{15.400,	15.400,	21.000,	15.500,	13.100,	10.000,	10.000,	10.000,	10.000,	10.000,	10.000,	10.000,	10.000,	10.000,	10.000,	10.000,	},
{18.000,	18.000,	18.000,	18.000,	18.000,	10.000,	10.000,	5.000,	5.000,	5.000,	5.000,	5.000,	5.000,	10.000,	10.000,	10.000,	},
{21.400,	18.000,	18.000,	18.000,	18.000,	10.000,	10.000,	5.000,	5.000,	5.000,	5.000,	5.000,	5.000,	10.000,	10.000,	10.000,	},
{25.100,	22.900,	21.300,	25.000,	25.000,	10.000,	10.000,	5.000,	5.000,	5.000,	5.000,	5.000,	5.000,	10.000,	10.000,	10.000,	},
{25.900,	24.700,	40.000,	40.000,	33.300,	25.000,	15.000,	15.000,	5.000,	5.000,	5.000,	5.000,	5.000,	5.000,	5.000,	5.000,	},
{29.600,	50.000,	50.000,	40.000,	31.100,	29.400,	22.600,	21.000,	20.000,	20.000,	20.000,	20.000,	20.000,	20.000,	20.000,	20.000,	},
{41.800,	50.000,	50.000,	45.000,	43.000,	40.400,	41.600,	36.000,	32.500,	33.900,	37.100,	37.400,	37.400,	37.400,	37.400,	37.400,	},
{43.900,	49.200,	51.700,	47.300,	42.800,	58.500,	60.300,	55.300,	51.700,	53.800,	56.000,	51.000,	51.000,	51.000,	51.000,	51.000,	},
{54.300,	59.800,	65.000,	62.000,	58.500,	66.300,	67.300,	61.500,	57.400,	59.200,	61.000,	56.000,	56.000,	56.000,	56.000,	56.000,	},
{59.600,	62.800,	65.700,	64.800,	63.600,	68.300,	69.600,	72.700,	70.700,	71.600,	72.400,	69.900,	69.800,	69.700,	69.700,	69.700,	},
{64.900,	65.800,	66.400,	67.500,	68.700,	70.200,	71.900,	85.900,	85.900,	85.900,	85.800,	85.700,	85.600,	85.400,	85.300,	85.300,	},
{66.100,	67.000,	67.600,	68.700,	69.900,	71.400,	73.100,	85.800,	85.900,	85.800,	85.600,	85.300,	84.900,	84.400,	84.100,	83.900,	},
{67.400,	68.300,	68.900,	70.000,	71.200,	72.700,	74.400,	85.600,	90.800,	90.700,	90.400,	89.800,	89.200,	88.300,	87.900,	87.500,	},
{68.600,	69.500,	70.100,	71.200,	72.400,	73.900,	75.600,	87.600,	92.800,	92.700,	95.400,	94.800,	94.100,	93.200,	92.800,	92.400,	},
{69.900,	70.800,	71.400,	72.500,	73.700,	75.200,	76.900,	89.600,	94.700,	94.600,	97.300,	96.700,	96.100,	95.200,	94.800,	94.300,	},
{71.100,	72.000,	72.600,	73.700,	74.900,	76.400,	78.100,	92.900,	98.200,	98.100,	100.800,	100.200,	99.600,	98.700,	98.300,	97.900,	},
};
	copyTable(config->veTable, hardCodedveTable);
}

static void cannedfuelTrimRpmBins() {
	static const float hardCodedfuelTrimRpmBins[4] = {0.0, 0.0, 0.0, 0.0};
	copyArray(config->fuelTrimRpmBins, hardCodedfuelTrimRpmBins);
}

static void cannedfuelTrimLoadBins() {
	static const float hardCodedfuelTrimLoadBins[4] = {0.0, 0.0, 0.0, 0.0};
	copyArray(config->fuelTrimLoadBins, hardCodedfuelTrimLoadBins);
}

static void cannedignTrimRpmBins() {
	static const float hardCodedignTrimRpmBins[4] = {0.0, 0.0, 0.0, 0.0};
	copyArray(config->ignTrimRpmBins, hardCodedignTrimRpmBins);
}

static void cannedignTrimLoadBins() {
	static const float hardCodedignTrimLoadBins[4] = {0.0, 0.0, 0.0, 0.0};
	copyArray(config->ignTrimLoadBins, hardCodedignTrimLoadBins);
}

static void cannedmapEstimateRpmBins() {
	static const float hardCodedmapEstimateRpmBins[16] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->mapEstimateRpmBins, hardCodedmapEstimateRpmBins);
}

static void cannedmapEstimateTpsBins() {
	static const float hardCodedmapEstimateTpsBins[16] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->mapEstimateTpsBins, hardCodedmapEstimateTpsBins);
}

static void cannedmapEstimateTable() {
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

static void cannedinjectionPhase() {
	static const float hardCodedinjectionPhase[16][16] = {
{-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	},
{-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	},
{-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	},
{-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	},
{-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	},
{-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	},
{-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	},
{-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	},
{-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	},
{-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	},
{-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	},
{-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	},
{-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	},
{-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	},
{-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	},
{-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	-360.000,	},
};
	copyTable(config->injectionPhase, hardCodedinjectionPhase);
}

static void cannedpedalToTpsRpmBins() {
	static const float hardCodedpedalToTpsRpmBins[8] = {700.0, 900.0, 3000.0, 3200.0, 4000.0, 5700.0, 6900.0, 8000.0};
	copyArray(config->pedalToTpsRpmBins, hardCodedpedalToTpsRpmBins);
}

static void cannedpedalToTpsPedalBins() {
	static const float hardCodedpedalToTpsPedalBins[8] = {1.0, 5.0, 10.0, 20.0, 40.0, 50.0, 70.0, 90.0};
	copyArray(config->pedalToTpsPedalBins, hardCodedpedalToTpsPedalBins);
}

static void cannedpedalToTpsTable() {
	static const float hardCodedpedalToTpsTable[8][8] = {
{0.000,	0.000,	0.000,	0.000,	6.000,	6.000,	3.000,	1.000,	},
{5.000,	5.000,	5.000,	5.000,	5.000,	5.000,	5.000,	5.000,	},
{7.000,	7.000,	10.000,	10.000,	10.000,	10.000,	10.000,	10.000,	},
{15.000,	15.000,	20.000,	20.000,	20.000,	20.000,	20.000,	20.000,	},
{25.000,	25.000,	40.000,	40.000,	40.000,	40.000,	40.000,	40.000,	},
{30.000,	30.000,	50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	},
{40.000,	40.000,	70.000,	70.000,	70.000,	70.000,	70.000,	70.000,	},
{50.000,	90.000,	90.000,	90.000,	90.000,	90.000,	90.000,	90.000,	},
};
	copyTable(config->pedalToTpsTable, hardCodedpedalToTpsTable);
}

static void cannedlambdaTable() {
	static const float hardCodedlambdaTable[16][16] = {
{14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	},
{14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	},
{14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	},
{14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	},
{14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	},
{14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	},
{13.500,	13.500,	13.500,	13.500,	13.500,	13.500,	13.500,	13.500,	13.500,	13.500,	13.500,	13.500,	13.500,	13.500,	13.500,	13.500,	},
{13.200,	13.200,	13.200,	13.200,	13.200,	13.200,	13.200,	13.200,	13.200,	13.200,	13.200,	13.200,	13.200,	13.200,	13.200,	13.200,	},
{13.100,	13.100,	13.100,	13.100,	13.100,	13.100,	13.100,	13.100,	13.100,	13.100,	13.100,	13.100,	13.100,	13.100,	13.100,	13.100,	},
{12.900,	12.900,	12.900,	12.900,	12.900,	12.900,	12.900,	12.900,	12.900,	12.900,	12.900,	12.900,	12.900,	12.900,	12.900,	12.900,	},
{12.600,	12.600,	12.600,	12.600,	12.600,	12.600,	12.600,	12.600,	12.600,	12.600,	12.600,	12.600,	12.600,	12.600,	12.600,	12.600,	},
{12.300,	12.300,	12.300,	12.300,	12.300,	12.300,	12.300,	12.300,	12.300,	12.300,	12.300,	12.300,	12.300,	12.300,	12.300,	12.300,	},
{11.800,	11.800,	11.800,	11.800,	11.800,	11.800,	11.800,	11.800,	11.800,	11.800,	11.800,	11.800,	11.800,	11.800,	11.800,	11.800,	},
{11.300,	11.300,	11.300,	11.300,	11.300,	11.300,	11.300,	11.300,	11.300,	11.300,	11.300,	11.300,	11.300,	11.300,	11.300,	11.300,	},
{11.000,	11.000,	11.000,	11.000,	11.000,	11.000,	11.000,	11.000,	11.000,	11.000,	11.000,	11.000,	11.000,	11.000,	11.000,	11.000,	},
{10.700,	10.700,	10.700,	10.700,	10.700,	10.700,	10.700,	10.700,	10.700,	10.700,	10.700,	10.700,	10.700,	10.700,	10.700,	10.700,	},
};
	copyTable(config->lambdaTable, hardCodedlambdaTable);
}

static void cannedtcuSolenoidTable() {
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

static void cannedhpfpTargetRpmBins() {
	static const float hardCodedhpfpTargetRpmBins[10] = {0.0, 900.0, 1800.0, 2650.0, 3550.0, 4450.0, 5350.0, 6200.0, 7100.0, 8000.0};
	copyArray(config->hpfpTargetRpmBins, hardCodedhpfpTargetRpmBins);
}

static void cannedhpfpCompensationRpmBins() {
	static const float hardCodedhpfpCompensationRpmBins[10] = {0.0, 900.0, 1800.0, 2650.0, 3550.0, 4450.0, 5350.0, 6200.0, 7100.0, 8000.0};
	copyArray(config->hpfpCompensationRpmBins, hardCodedhpfpCompensationRpmBins);
}

static void cannedpostCrankingFactor() {
	static const float hardCodedpostCrankingFactor[6][6] = {
{1.200,	1.000,	1.000,	1.000,	1.000,	1.000,	},
{1.200,	1.000,	1.000,	1.000,	1.000,	1.000,	},
{1.200,	1.000,	1.000,	1.000,	1.000,	1.000,	},
{1.200,	1.000,	1.000,	1.000,	1.000,	1.000,	},
{1.200,	1.000,	1.000,	1.000,	1.000,	1.000,	},
{1.200,	1.000,	1.000,	1.000,	1.000,	1.000,	},
};
	copyTable(engineConfiguration->postCrankingFactor, hardCodedpostCrankingFactor);
}

```
