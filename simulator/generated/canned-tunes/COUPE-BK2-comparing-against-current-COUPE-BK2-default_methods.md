```
static void coupleBK2cannedtpsTspCorrValuesBins() {
	static const float hardCodedtpsTspCorrValuesBins[4] = {1000.0, 3000.0, 5000.0, 7000.0};
	copyArray(config->tpsTspCorrValuesBins, hardCodedtpsTspCorrValuesBins);
}

static void coupleBK2cannedtpsTspCorrValues() {
	static const float hardCodedtpsTspCorrValues[4] = {1.0, 0.7, 0.3, 0.1};
	copyArray(config->tpsTspCorrValues, hardCodedtpsTspCorrValues);
}

static void coupleBK2cannedprimeBins() {
	static const float hardCodedprimeBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->primeBins, hardCodedprimeBins);
}

static void coupleBK2cannedprimeValues() {
	static const float hardCodedprimeValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->primeValues, hardCodedprimeValues);
}

static void coupleBK2cannedmafDecodingBins() {
	static const float hardCodedmafDecodingBins[32] = {0.0, 0.78125, 1.3867188, 2.9101562, 4.2578125, 4.9804688, 2016.0, 2017.0, 2018.0, 2019.0, 2020.0, 2021.0, 2022.0, 2023.0, 2024.0, 2025.0, 2026.0, 2027.0, 2028.0, 2029.0, 2030.0, 2031.0, 2032.0, 2033.0, 2034.0, 2035.0, 2036.0, 2037.0, 2038.0, 2039.0, 2040.0, 2041.0};
	copyArray(config->mafDecodingBins, hardCodedmafDecodingBins);
}

static void coupleBK2cannedcltRevLimitRpmBins() {
	static const float hardCodedcltRevLimitRpmBins[4] = {0.0, 0.0, 0.0, 0.0};
	copyArray(config->cltRevLimitRpmBins, hardCodedcltRevLimitRpmBins);
}

static void coupleBK2cannedcltIdleCorrBins() {
	static const float hardCodedcltIdleCorrBins[16] = {-40.13, -30.13, -20.13, -10.13, -0.13, 9.87, 19.87, 29.87, 39.87, 49.87, 59.87, 69.87, 79.87, 89.87, 99.87, 109.87};
	copyArray(config->cltIdleCorrBins, hardCodedcltIdleCorrBins);
}

static void coupleBK2cannedcltIdleCorr() {
	static const float hardCodedcltIdleCorr[16] = {0.87, 0.87, 0.7033, 0.7033, 0.7033, 0.7033, 0.7033, 0.7033, 0.7033, 0.6033, 0.5367, 0.5, 0.5, 0.5, 0.5, 0.6};
	copyArray(config->cltIdleCorr, hardCodedcltIdleCorr);
}

static void coupleBK2cannedcltCrankingCorr() {
	static const float hardCodedcltCrankingCorr[8] = {0.8, 0.8, 0.8, 0.8, 0.7, 0.64, 0.6, 0.6};
	copyArray(config->cltCrankingCorr, hardCodedcltCrankingCorr);
}

static void coupleBK2cannedcltIdleRpm() {
	static const float hardCodedcltIdleRpm[16] = {1360.0, 1360.0, 1300.0, 1200.0, 1160.0, 1100.0, 1060.0, 1000.0, 1000.0, 960.0, 960.0, 940.0, 900.0, 800.0, 860.0, 1100.0};
	copyArray(config->cltIdleRpm, hardCodedcltIdleRpm);
}

static void coupleBK2cannedidleAdvanceBins() {
	static const float hardCodedidleAdvanceBins[8] = {500.0, 650.0, 800.0, 950.0, 1050.0, 1200.0, 1350.0, 1500.0};
	copyArray(config->idleAdvanceBins, hardCodedidleAdvanceBins);
}

static void coupleBK2cannedidleAdvance() {
	static const float hardCodedidleAdvance[8] = {10.0, 10.285714, 10.571428, 10.857142, 11.142858, 11.428572, 11.714286, 12.0};
	copyArray(config->idleAdvance, hardCodedidleAdvance);
}

static void coupleBK2cannedtpsTpsAccelTable() {
	static const float hardCodedtpsTpsAccelTable[8][8] = {
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{5.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{8.500,	5.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{11.500,	8.500,	5.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{14.000,	11.500,	8.500,	5.000,	0.000,	0.000,	0.000,	0.000,	},
{16.000,	14.000,	11.500,	8.500,	5.000,	0.000,	0.000,	0.000,	},
{17.500,	16.000,	14.000,	11.500,	8.500,	5.000,	0.000,	0.000,	},
{18.500,	17.500,	16.000,	14.000,	11.500,	8.500,	5.000,	0.000,	},
};
	copyTable(config->tpsTpsAccelTable, hardCodedtpsTpsAccelTable);
}

static void coupleBK2cannedboostRpmBins() {
	static const float hardCodedboostRpmBins[8] = {0.0, 1100.0, 2300.0, 3400.0, 4600.0, 5700.0, 6900.0, 8000.0};
	copyArray(config->boostRpmBins, hardCodedboostRpmBins);
}

static void coupleBK2cannedboostTableOpenLoop() {
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

static void coupleBK2cannedscriptTable4RpmBins() {
	static const float hardCodedscriptTable4RpmBins[10] = {650.0, 800.0, 1700.0, 2600.0, 3500.0, 4400.0, 5300.0, 7000.0, 0.0, 0.0};
	copyArray(config->scriptTable4RpmBins, hardCodedscriptTable4RpmBins);
}

static void coupleBK2cannedscriptTable4() {
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

static void coupleBK2cannedignitionRpmBins() {
	static const float hardCodedignitionRpmBins[16] = {650.0, 800.0, 1100.0, 1500.0, 2000.0, 2500.0, 3000.0, 3500.0, 4000.0, 4500.0, 5000.0, 5500.0, 6000.0, 6500.0, 7000.0, 8000.0};
	copyArray(config->ignitionRpmBins, hardCodedignitionRpmBins);
}

static void coupleBK2cannedignitionLoadBins() {
	static const float hardCodedignitionLoadBins[16] = {30.0, 35.0, 45.0, 55.0, 60.0, 75.0, 90.0, 100.0, 125.0, 150.0, 175.0, 200.0, 225.0, 250.0, 275.0, 300.0};
	copyArray(config->ignitionLoadBins, hardCodedignitionLoadBins);
}

static void coupleBK2cannedveRpmBins() {
	static const float hardCodedveRpmBins[16] = {600.0, 900.0, 1100.0, 1500.0, 1900.0, 2400.0, 3000.0, 3700.0, 4200.0, 4700.0, 5200.0, 5800.0, 6300.0, 6800.0, 7000.0, 7600.0};
	copyArray(config->veRpmBins, hardCodedveRpmBins);
}

static void coupleBK2cannedveLoadBins() {
	static const float hardCodedveLoadBins[16] = {30.0, 35.0, 45.0, 55.0, 60.0, 75.0, 90.0, 100.0, 125.0, 150.0, 175.0, 200.0, 225.0, 250.0, 275.0, 300.0};
	copyArray(config->veLoadBins, hardCodedveLoadBins);
}

static void coupleBK2cannedfuelTrimRpmBins() {
	static const float hardCodedfuelTrimRpmBins[4] = {0.0, 0.0, 0.0, 0.0};
	copyArray(config->fuelTrimRpmBins, hardCodedfuelTrimRpmBins);
}

static void coupleBK2cannedfuelTrimLoadBins() {
	static const float hardCodedfuelTrimLoadBins[4] = {0.0, 0.0, 0.0, 0.0};
	copyArray(config->fuelTrimLoadBins, hardCodedfuelTrimLoadBins);
}

static void coupleBK2cannedignTrimRpmBins() {
	static const float hardCodedignTrimRpmBins[4] = {0.0, 0.0, 0.0, 0.0};
	copyArray(config->ignTrimRpmBins, hardCodedignTrimRpmBins);
}

static void coupleBK2cannedignTrimLoadBins() {
	static const float hardCodedignTrimLoadBins[4] = {0.0, 0.0, 0.0, 0.0};
	copyArray(config->ignTrimLoadBins, hardCodedignTrimLoadBins);
}

static void coupleBK2cannedpedalToTpsRpmBins() {
	static const float hardCodedpedalToTpsRpmBins[8] = {0.0, 1100.0, 2300.0, 3400.0, 4600.0, 5700.0, 6900.0, 8000.0};
	copyArray(config->pedalToTpsRpmBins, hardCodedpedalToTpsRpmBins);
}

static void coupleBK2cannedpedalToTpsTable() {
	static const float hardCodedpedalToTpsTable[8][8] = {
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{8.000,	8.000,	8.000,	8.000,	8.000,	8.000,	8.000,	8.000,	},
{17.000,	17.000,	17.000,	17.000,	17.000,	17.000,	17.000,	17.000,	},
{26.000,	26.000,	26.000,	26.000,	26.000,	26.000,	26.000,	26.000,	},
{34.000,	34.000,	34.000,	34.000,	34.000,	34.000,	34.000,	34.000,	},
{43.000,	43.000,	43.000,	43.000,	43.000,	43.000,	43.000,	43.000,	},
{86.000,	86.000,	86.000,	86.000,	86.000,	86.000,	86.000,	86.000,	},
{100.000,	100.000,	100.000,	100.000,	100.000,	100.000,	100.000,	100.000,	},
};
	copyTable(config->pedalToTpsTable, hardCodedpedalToTpsTable);
}

static void coupleBK2cannedlambdaTable() {
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

static void coupleBK2cannedtcuSolenoidTable() {
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

static void coupleBK2cannedhpfpTargetRpmBins() {
	static const float hardCodedhpfpTargetRpmBins[10] = {0.0, 900.0, 1800.0, 2650.0, 3550.0, 4450.0, 5350.0, 6200.0, 7100.0, 8000.0};
	copyArray(config->hpfpTargetRpmBins, hardCodedhpfpTargetRpmBins);
}

static void coupleBK2cannedhpfpCompensationRpmBins() {
	static const float hardCodedhpfpCompensationRpmBins[10] = {0.0, 900.0, 1800.0, 2650.0, 3550.0, 4450.0, 5350.0, 6200.0, 7100.0, 8000.0};
	copyArray(config->hpfpCompensationRpmBins, hardCodedhpfpCompensationRpmBins);
}

static void coupleBK2cannedpostCrankingFactor() {
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
