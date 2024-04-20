```
static void coupleBK1cannedprimeBins() {
	static const float hardCodedprimeBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->primeBins, hardCodedprimeBins);
}

static void coupleBK1cannedprimeValues() {
	static const float hardCodedprimeValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->primeValues, hardCodedprimeValues);
}

static void coupleBK1cannedmap_samplingAngleBins() {
	static const float hardCodedmap_samplingAngleBins[8] = {800.0, 1686.0, 2571.0, 3457.0, 4343.0, 5229.0, 6114.0, 7000.0};
	copyArray(engineConfiguration->map.map_samplingAngleBins, hardCodedmap_samplingAngleBins);
}

static void coupleBK1cannedmap_samplingWindowBins() {
	static const float hardCodedmap_samplingWindowBins[8] = {800.0, 1686.0, 2571.0, 3457.0, 4343.0, 5229.0, 6114.0, 7000.0};
	copyArray(engineConfiguration->map.map_samplingWindowBins, hardCodedmap_samplingWindowBins);
}

static void coupleBK1cannedvrThreshold1_rpmBins() {
	static const float hardCodedvrThreshold1_rpmBins[6] = {600.0, 1900.0, 3200.0, 4400.0, 5700.0, 7000.0};
	copyArray(engineConfiguration->vrThreshold2.vrThreshold1_rpmBins, hardCodedvrThreshold1_rpmBins);
}

static void coupleBK1cannedvrThreshold2_rpmBins() {
	static const float hardCodedvrThreshold2_rpmBins[6] = {600.0, 1900.0, 3200.0, 4400.0, 5700.0, 7000.0};
	copyArray(engineConfiguration->vrThreshold2.vrThreshold2_rpmBins, hardCodedvrThreshold2_rpmBins);
}

static void coupleBK1cannedmafDecodingBins() {
	static const float hardCodedmafDecodingBins[32] = {0.0, 0.78125, 1.3867188, 2.9101562, 4.2578125, 4.9804688, 2016.0, 2017.0, 2018.0, 2019.0, 2020.0, 2021.0, 2022.0, 2023.0, 2024.0, 2025.0, 2026.0, 2027.0, 2028.0, 2029.0, 2030.0, 2031.0, 2032.0, 2033.0, 2034.0, 2035.0, 2036.0, 2037.0, 2038.0, 2039.0, 2040.0, 2041.0};
	copyArray(config->mafDecodingBins, hardCodedmafDecodingBins);
}

static void coupleBK1cannedcltRevLimitRpmBins() {
	static const float hardCodedcltRevLimitRpmBins[4] = {0.0, 0.0, 0.0, 0.0};
	copyArray(config->cltRevLimitRpmBins, hardCodedcltRevLimitRpmBins);
}

static void coupleBK1cannedcltIdleCorrBins() {
	static const float hardCodedcltIdleCorrBins[16] = {-40.45, -30.45, -20.45, -10.45, -0.45, 9.55, 19.55, 29.55, 39.55, 49.55, 59.55, 69.55, 79.55, 89.55, 99.55, 109.55};
	copyArray(config->cltIdleCorrBins, hardCodedcltIdleCorrBins);
}

static void coupleBK1cannedcltIdleCorr() {
	static const float hardCodedcltIdleCorr[16] = {1.05, 1.05, 0.8833, 0.8833, 0.8833, 0.8833, 0.8833, 0.8833, 0.8833, 0.7833, 0.7167, 0.65, 0.55, 0.55, 0.55, 0.55};
	copyArray(config->cltIdleCorr, hardCodedcltIdleCorr);
}

static void coupleBK1cannedcltIdleRpm() {
	static const float hardCodedcltIdleRpm[16] = {1260.0, 1260.0, 1200.0, 1100.0, 1060.0, 1000.0, 960.0, 900.0, 900.0, 860.0, 860.0, 840.0, 800.0, 800.0, 900.0, 1000.0};
	copyArray(config->cltIdleRpm, hardCodedcltIdleRpm);
}

static void coupleBK1cannedidleAdvanceBins() {
	static const float hardCodedidleAdvanceBins[8] = {500.0, 650.0, 800.0, 950.0, 1050.0, 1200.0, 1350.0, 1500.0};
	copyArray(config->idleAdvanceBins, hardCodedidleAdvanceBins);
}

static void coupleBK1cannedidleAdvance() {
	static const float hardCodedidleAdvance[8] = {10.0, 10.571428, 11.142858, 11.714286, 12.285714, 12.857142, 13.428572, 14.0};
	copyArray(config->idleAdvance, hardCodedidleAdvance);
}

static void coupleBK1cannedfuelLevelBins() {
	static const float hardCodedfuelLevelBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->fuelLevelBins, hardCodedfuelLevelBins);
}

static void coupleBK1cannedignBlends1_blendBins() {
	static const float hardCodedignBlends1_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends1_blendBins, hardCodedignBlends1_blendBins);
}

static void coupleBK1cannedignBlends1_blendValues() {
	static const float hardCodedignBlends1_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends1_blendValues, hardCodedignBlends1_blendValues);
}

static void coupleBK1cannedignBlends2_blendBins() {
	static const float hardCodedignBlends2_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends2_blendBins, hardCodedignBlends2_blendBins);
}

static void coupleBK1cannedignBlends2_blendValues() {
	static const float hardCodedignBlends2_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends2_blendValues, hardCodedignBlends2_blendValues);
}

static void coupleBK1cannedignBlends3_blendBins() {
	static const float hardCodedignBlends3_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends3_blendBins, hardCodedignBlends3_blendBins);
}

static void coupleBK1cannedignBlends3_blendValues() {
	static const float hardCodedignBlends3_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends3_blendValues, hardCodedignBlends3_blendValues);
}

static void coupleBK1cannedignBlends4_blendBins() {
	static const float hardCodedignBlends4_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends4_blendBins, hardCodedignBlends4_blendBins);
}

static void coupleBK1cannedignBlends4_blendValues() {
	static const float hardCodedignBlends4_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends4_blendValues, hardCodedignBlends4_blendValues);
}

static void coupleBK1cannedveBlends1_blendBins() {
	static const float hardCodedveBlends1_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends1_blendBins, hardCodedveBlends1_blendBins);
}

static void coupleBK1cannedveBlends1_blendValues() {
	static const float hardCodedveBlends1_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends1_blendValues, hardCodedveBlends1_blendValues);
}

static void coupleBK1cannedveBlends2_blendBins() {
	static const float hardCodedveBlends2_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends2_blendBins, hardCodedveBlends2_blendBins);
}

static void coupleBK1cannedveBlends2_blendValues() {
	static const float hardCodedveBlends2_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends2_blendValues, hardCodedveBlends2_blendValues);
}

static void coupleBK1cannedveBlends3_blendBins() {
	static const float hardCodedveBlends3_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends3_blendBins, hardCodedveBlends3_blendBins);
}

static void coupleBK1cannedveBlends3_blendValues() {
	static const float hardCodedveBlends3_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends3_blendValues, hardCodedveBlends3_blendValues);
}

static void coupleBK1cannedveBlends4_blendBins() {
	static const float hardCodedveBlends4_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends4_blendBins, hardCodedveBlends4_blendBins);
}

static void coupleBK1cannedveBlends4_blendValues() {
	static const float hardCodedveBlends4_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends4_blendValues, hardCodedveBlends4_blendValues);
}

static void coupleBK1cannedboostRpmBins() {
	static const float hardCodedboostRpmBins[8] = {0.0, 1100.0, 2300.0, 3400.0, 4600.0, 5700.0, 6900.0, 8000.0};
	copyArray(config->boostRpmBins, hardCodedboostRpmBins);
}

static void coupleBK1cannedboostTableOpenLoop() {
	static const float hardCodedboostTableOpenLoop[8][8] = {
{25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	},
{25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	},
{25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	},
{25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	},
{25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	},
{25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	},
{25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	},
{25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	},
};
	copyTable(config->boostTableOpenLoop, hardCodedboostTableOpenLoop);
}

static void coupleBK1cannedvvtTable1() {
	static const float hardCodedvvtTable1[8][8] = {
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	5.000,	10.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	10.000,	15.000,	15.000,	15.000,	10.000,	10.000,	10.000,	},
{0.000,	10.000,	15.000,	15.000,	15.000,	10.000,	10.000,	10.000,	},
{0.000,	10.000,	15.000,	15.000,	15.000,	10.000,	10.000,	10.000,	},
{0.000,	10.000,	15.000,	15.000,	15.000,	10.000,	10.000,	10.000,	},
{0.000,	30.000,	30.000,	30.000,	19.000,	9.000,	5.000,	5.000,	},
{0.000,	30.000,	30.000,	30.000,	19.000,	9.000,	5.000,	5.000,	},
};
	copyTable(config->vvtTable1, hardCodedvvtTable1);
}

static void coupleBK1cannedvvtTable2() {
	static const float hardCodedvvtTable2[8][8] = {
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	-25.000,	-25.000,	-25.000,	-25.000,	-25.000,	-25.000,	-25.000,	},
{0.000,	-25.000,	-25.000,	-25.000,	-25.000,	-25.000,	-25.000,	-25.000,	},
{0.000,	-25.000,	-25.000,	-25.000,	-25.000,	-25.000,	-25.000,	-25.000,	},
{0.000,	-25.000,	-25.000,	-25.000,	-25.000,	-25.000,	-25.000,	-25.000,	},
};
	copyTable(config->vvtTable2, hardCodedvvtTable2);
}

static void coupleBK1cannedscriptTable4RpmBins() {
	static const float hardCodedscriptTable4RpmBins[10] = {650.0, 800.0, 1700.0, 2600.0, 3500.0, 4400.0, 5300.0, 7000.0, 0.0, 0.0};
	copyArray(config->scriptTable4RpmBins, hardCodedscriptTable4RpmBins);
}

static void coupleBK1cannedscriptTable4() {
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

static void coupleBK1cannedignitionRpmBins() {
	static const float hardCodedignitionRpmBins[16] = {650.0, 800.0, 1100.0, 1500.0, 2000.0, 2500.0, 3000.0, 3500.0, 4000.0, 4500.0, 5000.0, 5500.0, 6000.0, 6500.0, 7000.0, 8000.0};
	copyArray(config->ignitionRpmBins, hardCodedignitionRpmBins);
}

static void coupleBK1cannedignitionLoadBins() {
	static const float hardCodedignitionLoadBins[16] = {30.0, 35.0, 45.0, 55.0, 60.0, 75.0, 90.0, 100.0, 125.0, 150.0, 175.0, 200.0, 225.0, 250.0, 275.0, 300.0};
	copyArray(config->ignitionLoadBins, hardCodedignitionLoadBins);
}

static void coupleBK1cannedignitionTable() {
	static const float hardCodedignitionTable[16][16] = {
{12.100,	14.000,	17.800,	22.900,	29.300,	33.600,	-20.000,	-20.000,	-20.000,	-20.000,	-20.000,	-20.000,	42.000,	42.000,	42.000,	42.000,	},
{11.800,	13.700,	10.000,	14.900,	23.600,	30.800,	-20.000,	-20.000,	-20.000,	-20.000,	-20.000,	-20.000,	41.000,	41.000,	41.000,	41.000,	},
{11.600,	13.400,	12.300,	17.100,	23.000,	29.900,	39.200,	40.000,	40.000,	40.000,	40.000,	40.000,	40.000,	40.000,	40.000,	40.000,	},
{11.400,	13.200,	16.700,	21.400,	22.300,	29.100,	38.200,	39.000,	39.000,	39.000,	39.000,	39.000,	39.000,	39.000,	39.000,	39.000,	},
{11.200,	12.900,	16.300,	20.800,	21.600,	28.300,	37.200,	38.000,	38.000,	38.000,	38.000,	38.000,	38.000,	38.000,	38.000,	38.000,	},
{10.900,	12.600,	16.600,	20.200,	23.400,	26.700,	27.000,	28.500,	30.000,	30.400,	30.800,	31.100,	31.500,	31.900,	32.300,	33.000,	},
{10.800,	12.400,	16.800,	19.600,	20.900,	24.200,	24.500,	26.000,	27.500,	27.900,	28.300,	28.600,	29.000,	29.400,	29.800,	30.500,	},
{14.300,	14.900,	16.500,	18.800,	20.700,	21.700,	22.000,	23.500,	25.000,	25.400,	25.800,	26.100,	26.500,	26.900,	27.300,	28.000,	},
{14.000,	14.600,	15.600,	17.100,	18.500,	19.200,	19.500,	21.000,	22.500,	22.900,	23.300,	23.600,	24.000,	24.400,	24.800,	25.500,	},
{12.900,	13.300,	14.000,	15.100,	16.100,	16.700,	17.000,	18.500,	20.000,	20.400,	20.800,	21.100,	21.500,	21.900,	22.300,	23.000,	},
{11.700,	12.000,	12.400,	13.100,	13.700,	14.200,	14.500,	16.000,	17.500,	17.900,	18.300,	18.600,	19.000,	19.400,	19.800,	20.500,	},
{10.400,	10.500,	10.800,	11.100,	11.500,	11.900,	12.000,	13.500,	15.000,	15.400,	15.800,	16.100,	16.500,	16.900,	17.300,	18.000,	},
{8.800,	9.000,	9.100,	9.400,	9.700,	10.000,	10.300,	11.800,	13.300,	13.700,	14.100,	14.400,	14.800,	15.200,	15.600,	16.300,	},
{6.700,	6.800,	7.500,	7.700,	8.000,	8.400,	8.700,	10.200,	11.700,	12.100,	12.500,	12.800,	13.200,	13.600,	14.000,	14.700,	},
{1.800,	5.600,	6.200,	6.500,	6.800,	7.100,	7.000,	8.500,	10.000,	10.400,	10.800,	11.100,	11.500,	11.900,	12.300,	13.000,	},
{-1.200,	2.600,	3.200,	3.600,	4.100,	5.000,	7.100,	7.800,	8.400,	9.000,	9.400,	9.500,	9.700,	9.800,	10.000,	10.000,	},
};
	copyTable(config->ignitionTable, hardCodedignitionTable);
}

static void coupleBK1cannedignBlends1_rpmBins() {
	static const float hardCodedignBlends1_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends1_rpmBins, hardCodedignBlends1_rpmBins);
}

static void coupleBK1cannedignBlends1_loadBins() {
	static const float hardCodedignBlends1_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends1_loadBins, hardCodedignBlends1_loadBins);
}

static void coupleBK1cannedignBlends2_rpmBins() {
	static const float hardCodedignBlends2_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends2_rpmBins, hardCodedignBlends2_rpmBins);
}

static void coupleBK1cannedignBlends2_loadBins() {
	static const float hardCodedignBlends2_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends2_loadBins, hardCodedignBlends2_loadBins);
}

static void coupleBK1cannedignBlends3_rpmBins() {
	static const float hardCodedignBlends3_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends3_rpmBins, hardCodedignBlends3_rpmBins);
}

static void coupleBK1cannedignBlends3_loadBins() {
	static const float hardCodedignBlends3_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends3_loadBins, hardCodedignBlends3_loadBins);
}

static void coupleBK1cannedignBlends4_rpmBins() {
	static const float hardCodedignBlends4_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends4_rpmBins, hardCodedignBlends4_rpmBins);
}

static void coupleBK1cannedignBlends4_loadBins() {
	static const float hardCodedignBlends4_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends4_loadBins, hardCodedignBlends4_loadBins);
}

static void coupleBK1cannedveBlends1_rpmBins() {
	static const float hardCodedveBlends1_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends1_rpmBins, hardCodedveBlends1_rpmBins);
}

static void coupleBK1cannedveBlends1_loadBins() {
	static const float hardCodedveBlends1_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends1_loadBins, hardCodedveBlends1_loadBins);
}

static void coupleBK1cannedveBlends2_rpmBins() {
	static const float hardCodedveBlends2_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends2_rpmBins, hardCodedveBlends2_rpmBins);
}

static void coupleBK1cannedveBlends2_loadBins() {
	static const float hardCodedveBlends2_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends2_loadBins, hardCodedveBlends2_loadBins);
}

static void coupleBK1cannedveBlends3_rpmBins() {
	static const float hardCodedveBlends3_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends3_rpmBins, hardCodedveBlends3_rpmBins);
}

static void coupleBK1cannedveBlends3_loadBins() {
	static const float hardCodedveBlends3_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends3_loadBins, hardCodedveBlends3_loadBins);
}

static void coupleBK1cannedveBlends4_rpmBins() {
	static const float hardCodedveBlends4_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends4_rpmBins, hardCodedveBlends4_rpmBins);
}

static void coupleBK1cannedveBlends4_loadBins() {
	static const float hardCodedveBlends4_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.veBlends4_loadBins, hardCodedveBlends4_loadBins);
}

static void coupleBK1cannedveTable() {
	static const float hardCodedveTable[16][16] = {
{52.000,	54.000,	54.000,	56.000,	61.000,	55.000,	55.000,	65.000,	65.000,	60.000,	60.000,	60.000,	60.000,	60.000,	60.000,	60.000,	},
{51.500,	54.500,	54.400,	56.000,	61.000,	68.100,	66.800,	67.600,	68.200,	63.400,	63.400,	63.400,	63.400,	63.400,	63.400,	63.400,	},
{52.500,	55.400,	57.700,	62.200,	62.800,	65.800,	69.600,	71.100,	72.200,	67.600,	67.600,	67.600,	67.600,	67.600,	67.600,	67.600,	},
{54.200,	56.300,	59.000,	64.300,	65.700,	68.400,	71.400,	72.900,	73.900,	70.500,	70.400,	70.400,	70.300,	70.200,	70.200,	70.200,	},
{54.400,	56.700,	59.500,	65.200,	71.800,	75.400,	75.300,	73.800,	74.700,	72.600,	72.500,	72.300,	72.200,	72.100,	72.000,	72.000,	},
{55.100,	58.100,	61.400,	67.900,	75.400,	77.300,	75.000,	76.300,	77.200,	77.500,	77.000,	76.500,	76.000,	75.600,	75.400,	75.400,	},
{55.800,	59.400,	63.000,	70.100,	73.200,	75.400,	77.800,	78.900,	79.800,	79.100,	78.400,	77.700,	77.000,	76.400,	76.100,	76.100,	},
{56.000,	66.200,	70.600,	74.200,	74.100,	76.000,	79.900,	84.100,	82.200,	79.600,	78.900,	78.200,	77.500,	76.900,	76.600,	76.600,	},
{58.500,	67.400,	69.200,	75.500,	78.300,	79.900,	81.700,	83.700,	84.700,	82.100,	81.400,	80.700,	80.000,	79.400,	79.100,	79.100,	},
{52.900,	70.000,	71.900,	75.900,	79.800,	81.900,	83.700,	85.600,	86.100,	83.600,	83.000,	82.200,	81.600,	81.000,	80.700,	83.700,	},
{50.700,	71.000,	73.000,	77.300,	81.600,	83.900,	85.700,	87.600,	87.900,	85.400,	84.800,	84.000,	83.400,	82.800,	82.500,	85.500,	},
{53.700,	74.000,	76.000,	80.300,	84.600,	86.900,	88.700,	90.600,	90.900,	88.400,	87.800,	87.000,	86.400,	85.800,	85.500,	88.500,	},
{56.700,	77.000,	79.000,	83.300,	87.600,	89.900,	91.700,	93.600,	93.900,	91.400,	90.800,	90.000,	89.400,	88.800,	88.500,	91.500,	},
{59.700,	80.000,	82.000,	86.300,	90.600,	92.900,	94.700,	96.600,	96.900,	94.400,	93.800,	93.000,	92.400,	91.800,	91.500,	94.500,	},
{62.700,	83.000,	85.000,	89.300,	93.600,	95.900,	97.700,	99.600,	99.900,	97.400,	96.800,	96.000,	95.400,	94.800,	94.500,	97.500,	},
{65.700,	86.000,	88.000,	92.300,	96.600,	98.900,	100.700,	102.600,	102.900,	100.400,	99.800,	99.000,	98.400,	97.800,	97.500,	100.500,	},
};
	copyTable(config->veTable, hardCodedveTable);
}

static void coupleBK1cannedfuelTrimRpmBins() {
	static const float hardCodedfuelTrimRpmBins[4] = {0.0, 0.0, 0.0, 0.0};
	copyArray(config->fuelTrimRpmBins, hardCodedfuelTrimRpmBins);
}

static void coupleBK1cannedfuelTrimLoadBins() {
	static const float hardCodedfuelTrimLoadBins[4] = {0.0, 0.0, 0.0, 0.0};
	copyArray(config->fuelTrimLoadBins, hardCodedfuelTrimLoadBins);
}

static void coupleBK1cannedignTrimRpmBins() {
	static const float hardCodedignTrimRpmBins[4] = {0.0, 0.0, 0.0, 0.0};
	copyArray(config->ignTrimRpmBins, hardCodedignTrimRpmBins);
}

static void coupleBK1cannedignTrimLoadBins() {
	static const float hardCodedignTrimLoadBins[4] = {0.0, 0.0, 0.0, 0.0};
	copyArray(config->ignTrimLoadBins, hardCodedignTrimLoadBins);
}

static void coupleBK1cannedpedalToTpsRpmBins() {
	static const float hardCodedpedalToTpsRpmBins[8] = {0.0, 1100.0, 2300.0, 3400.0, 4600.0, 5700.0, 6900.0, 8000.0};
	copyArray(config->pedalToTpsRpmBins, hardCodedpedalToTpsRpmBins);
}

static void coupleBK1cannedlambdaTable() {
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

static void coupleBK1cannedtcuSolenoidTable() {
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

static void coupleBK1cannedhpfpTargetRpmBins() {
	static const float hardCodedhpfpTargetRpmBins[10] = {0.0, 900.0, 1800.0, 2650.0, 3550.0, 4450.0, 5350.0, 6200.0, 7100.0, 8000.0};
	copyArray(config->hpfpTargetRpmBins, hardCodedhpfpTargetRpmBins);
}

static void coupleBK1cannedhpfpCompensationRpmBins() {
	static const float hardCodedhpfpCompensationRpmBins[10] = {0.0, 900.0, 1800.0, 2650.0, 3550.0, 4450.0, 5350.0, 6200.0, 7100.0, 8000.0};
	copyArray(config->hpfpCompensationRpmBins, hardCodedhpfpCompensationRpmBins);
}

static void coupleBK1cannedpostCrankingFactor() {
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
