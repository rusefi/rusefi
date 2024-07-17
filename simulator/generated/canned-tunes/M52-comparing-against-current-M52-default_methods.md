```
static void cannedsparkDwellRpmBins() {
	static const float hardCodedsparkDwellRpmBins[8] = {0.0, 500.0, 2000.0, 3000.0, 4000.0, 5000.0, 6000.0, 7000.0};
	copyArray(config->sparkDwellRpmBins, hardCodedsparkDwellRpmBins);
}

static void cannedsparkDwellValues() {
	static const float hardCodedsparkDwellValues[8] = {5.4, 3.1, 3.05, 3.0, 2.95, 2.9, 2.85, 2.8};
	copyArray(config->sparkDwellValues, hardCodedsparkDwellValues);
}

static void canneddwellVoltageCorrVoltBins() {
	static const float hardCodeddwellVoltageCorrVoltBins[8] = {8.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0};
	copyArray(config->dwellVoltageCorrVoltBins, hardCodeddwellVoltageCorrVoltBins);
}

static void canneddwellVoltageCorrValues() {
	static const float hardCodeddwellVoltageCorrValues[8] = {1.4, 1.32, 1.24, 1.12, 1.08, 1.02, 0.92, 0.84};
	copyArray(config->dwellVoltageCorrValues, hardCodeddwellVoltageCorrValues);
}

static void cannedtpsTspCorrValuesBins() {
	static const float hardCodedtpsTspCorrValuesBins[4] = {3000.0, 5000.0, 7000.0, 9000.0};
	copyArray(config->tpsTspCorrValuesBins, hardCodedtpsTspCorrValuesBins);
}

static void cannedtpsTspCorrValues() {
	static const float hardCodedtpsTspCorrValues[4] = {1.0, 0.4, 0.2, 0.02};
	copyArray(config->tpsTspCorrValues, hardCodedtpsTspCorrValues);
}

static void cannedprimeBins() {
	static const float hardCodedprimeBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->primeBins, hardCodedprimeBins);
}

static void cannedprimeValues() {
	static const float hardCodedprimeValues[8] = {50.0, 50.0, 50.0, 50.0, 50.0, 50.0, 50.0, 50.0};
	copyArray(engineConfiguration->primeValues, hardCodedprimeValues);
}

static void cannedmap_samplingAngleBins() {
	static const float hardCodedmap_samplingAngleBins[8] = {800.0, 1686.0, 2571.0, 3457.0, 4343.0, 5229.0, 6114.0, 7000.0};
	copyArray(engineConfiguration->map.map.samplingAngleBins, hardCodedmap_samplingAngleBins);
}

static void cannedmap_samplingAngle() {
	static const float hardCodedmap_samplingAngle[8] = {400.0, 400.0, 400.0, 400.0, 400.0, 400.0, 400.0, 400.0};
	copyArray(engineConfiguration->map.map.samplingAngle, hardCodedmap_samplingAngle);
}

static void cannedmap_samplingWindowBins() {
	static const float hardCodedmap_samplingWindowBins[8] = {800.0, 1686.0, 2571.0, 3457.0, 4343.0, 5229.0, 6114.0, 7000.0};
	copyArray(engineConfiguration->map.map.samplingWindowBins, hardCodedmap_samplingWindowBins);
}

static void cannedmap_samplingWindow() {
	static const float hardCodedmap_samplingWindow[8] = {150.0, 150.0, 150.0, 50.0, 50.0, 50.0, 50.0, 50.0};
	copyArray(engineConfiguration->map.map.samplingWindow, hardCodedmap_samplingWindow);
}

static void cannedvrThreshold1_rpmBins() {
	static const float hardCodedvrThreshold1_rpmBins[6] = {600.0, 1900.0, 3200.0, 4400.0, 5700.0, 7000.0};
	copyArray(engineConfiguration->vrThreshold[1].vrThreshold[0].rpmBins, hardCodedvrThreshold1_rpmBins);
}

static void cannedvrThreshold2_rpmBins() {
	static const float hardCodedvrThreshold2_rpmBins[6] = {600.0, 1900.0, 3200.0, 4400.0, 5700.0, 7000.0};
	copyArray(engineConfiguration->vrThreshold[1].vrThreshold[1].rpmBins, hardCodedvrThreshold2_rpmBins);
}

static void cannedcltFuelCorr() {
	static const float hardCodedcltFuelCorr[16] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
	copyArray(config->cltFuelCorr, hardCodedcltFuelCorr);
}

static void cannedcrankingFuelCoef() {
	static const float hardCodedcrankingFuelCoef[8] = {1.2, 1.2, 1.2, 1.2, 1.1333333, 1.0666667, 1.0, 1.0};
	copyArray(config->crankingFuelCoef, hardCodedcrankingFuelCoef);
}

static void cannedcltRevLimitRpmBins() {
	static const float hardCodedcltRevLimitRpmBins[4] = {0.0, 0.0, 0.0, 0.0};
	copyArray(config->cltRevLimitRpmBins, hardCodedcltRevLimitRpmBins);
}

static void cannedcltIdleCorr() {
	static const float hardCodedcltIdleCorr[16] = {1.2, 1.2, 1.2, 2.0, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 1.2, 1.2, 1.0, 1.0, 1.2};
	copyArray(config->cltIdleCorr, hardCodedcltIdleCorr);
}

static void cannedcltCrankingCorr() {
	static const float hardCodedcltCrankingCorr[8] = {1.2, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2};
	copyArray(config->cltCrankingCorr, hardCodedcltCrankingCorr);
}

static void cannedcltIdleRpm() {
	static const float hardCodedcltIdleRpm[16] = {2200.0, 2200.0, 2200.0, 2200.0, 2200.0, 2200.0, 2200.0, 2200.0, 2200.0, 2200.0, 2200.0, 2200.0, 2200.0, 2200.0, 2200.0, 2200.0};
	copyArray(config->cltIdleRpm, hardCodedcltIdleRpm);
}

static void cannedidleAdvanceBins() {
	static const float hardCodedidleAdvanceBins[8] = {1000.0, 1200.0, 1450.0, 1650.0, 1850.0, 2050.0, 2300.0, 2500.0};
	copyArray(config->idleAdvanceBins, hardCodedidleAdvanceBins);
}

static void cannedidleAdvance() {
	static const float hardCodedidleAdvance[8] = {0.0, 0.2857, 0.5714, 0.8571, 1.1429, 1.4286, 1.7143, 2.0};
	copyArray(config->idleAdvance, hardCodedidleAdvance);
}

static void cannedfuelLevelBins() {
	static const float hardCodedfuelLevelBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->fuelLevelBins, hardCodedfuelLevelBins);
}

static void cannedignBlends1_blendBins() {
	static const float hardCodedignBlends1_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].ignBlends[0].blendBins, hardCodedignBlends1_blendBins);
}

static void cannedignBlends1_blendValues() {
	static const float hardCodedignBlends1_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].ignBlends[0].blendValues, hardCodedignBlends1_blendValues);
}

static void cannedignBlends2_blendBins() {
	static const float hardCodedignBlends2_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].ignBlends[1].blendBins, hardCodedignBlends2_blendBins);
}

static void cannedignBlends2_blendValues() {
	static const float hardCodedignBlends2_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].ignBlends[1].blendValues, hardCodedignBlends2_blendValues);
}

static void cannedignBlends3_blendBins() {
	static const float hardCodedignBlends3_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].ignBlends[2].blendBins, hardCodedignBlends3_blendBins);
}

static void cannedignBlends3_blendValues() {
	static const float hardCodedignBlends3_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].ignBlends[2].blendValues, hardCodedignBlends3_blendValues);
}

static void cannedignBlends4_blendBins() {
	static const float hardCodedignBlends4_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].ignBlends[3].blendBins, hardCodedignBlends4_blendBins);
}

static void cannedignBlends4_blendValues() {
	static const float hardCodedignBlends4_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].ignBlends[3].blendValues, hardCodedignBlends4_blendValues);
}

static void cannedveBlends1_blendBins() {
	static const float hardCodedveBlends1_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].veBlends[0].blendBins, hardCodedveBlends1_blendBins);
}

static void cannedveBlends1_blendValues() {
	static const float hardCodedveBlends1_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].veBlends[0].blendValues, hardCodedveBlends1_blendValues);
}

static void cannedveBlends2_blendBins() {
	static const float hardCodedveBlends2_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].veBlends[1].blendBins, hardCodedveBlends2_blendBins);
}

static void cannedveBlends2_blendValues() {
	static const float hardCodedveBlends2_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].veBlends[1].blendValues, hardCodedveBlends2_blendValues);
}

static void cannedveBlends3_blendBins() {
	static const float hardCodedveBlends3_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].veBlends[2].blendBins, hardCodedveBlends3_blendBins);
}

static void cannedveBlends3_blendValues() {
	static const float hardCodedveBlends3_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].veBlends[2].blendValues, hardCodedveBlends3_blendValues);
}

static void cannedveBlends4_blendBins() {
	static const float hardCodedveBlends4_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].veBlends[3].blendBins, hardCodedveBlends4_blendBins);
}

static void cannedveBlends4_blendValues() {
	static const float hardCodedveBlends4_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].veBlends[3].blendValues, hardCodedveBlends4_blendValues);
}

static void cannedtpsTpsAccelTable() {
	static const float hardCodedtpsTpsAccelTable[8][8] = {
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{4.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{6.800,	4.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{9.200,	6.800,	4.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{11.200,	9.200,	6.800,	4.000,	0.000,	0.000,	0.000,	0.000,	},
{12.800,	11.200,	9.200,	6.800,	4.000,	0.000,	0.000,	0.000,	},
{14.000,	12.800,	11.200,	9.200,	6.800,	4.000,	0.000,	0.000,	},
{14.800,	14.000,	12.800,	11.200,	9.200,	6.800,	4.000,	0.000,	},
};
	copyTable(config->tpsTpsAccelTable, hardCodedtpsTpsAccelTable);
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
	static const float hardCodedignitionRpmBins[16] = {1200.0, 1500.0, 2000.0, 3000.0, 4000.0, 4500.0, 5000.0, 5500.0, 6000.0, 6500.0, 7000.0, 7500.0, 8000.0, 8500.0, 9000.0, 10000.0};
	copyArray(config->ignitionRpmBins, hardCodedignitionRpmBins);
}

static void cannedignitionLoadBins() {
	static const float hardCodedignitionLoadBins[16] = {0.0, 2.0, 4.0, 6.0, 8.0, 10.0, 20.0, 25.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0};
	copyArray(config->ignitionLoadBins, hardCodedignitionLoadBins);
}

static void cannedignitionTable() {
	static const float hardCodedignitionTable[16][16] = {
{10.000,	5.000,	-5.000,	0.000,	2.100,	2.900,	-20.000,	-20.000,	-20.000,	-20.000,	-20.000,	-20.000,	0.200,	20.600,	41.000,	41.000,	},
{10.000,	5.000,	-5.000,	0.000,	2.100,	2.900,	-20.000,	-20.000,	-20.000,	-20.000,	-20.000,	-20.000,	0.200,	20.600,	41.000,	41.000,	},
{8.000,	14.000,	22.100,	36.200,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	},
{8.000,	14.000,	22.100,	36.200,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	40.000,	40.000,	},
{8.000,	14.000,	22.100,	36.200,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	40.000,	40.000,	},
{8.000,	13.900,	22.000,	36.200,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	40.000,	40.000,	},
{8.000,	13.800,	21.900,	36.200,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	40.000,	40.000,	},
{8.000,	13.800,	21.900,	36.200,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	40.000,	40.000,	},
{8.000,	13.700,	21.900,	36.200,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	40.000,	40.000,	},
{8.000,	13.600,	21.800,	36.200,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	40.000,	40.000,	},
{10.700,	13.500,	21.700,	36.200,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	37.000,	39.000,	39.000,	},
{15.900,	13.400,	21.700,	27.200,	28.000,	28.000,	28.000,	28.000,	28.000,	28.000,	28.000,	28.000,	28.000,	28.000,	29.000,	29.000,	},
{15.400,	12.800,	21.300,	27.200,	28.000,	28.000,	28.000,	28.000,	28.000,	28.000,	28.000,	28.000,	28.000,	28.000,	28.000,	28.000,	},
{15.000,	12.300,	20.300,	25.300,	26.000,	26.000,	26.000,	26.000,	26.000,	26.000,	26.400,	26.800,	27.000,	27.000,	27.000,	27.000,	},
{14.700,	11.900,	20.000,	25.300,	26.000,	26.000,	26.000,	26.000,	26.000,	26.000,	26.400,	26.800,	27.000,	27.000,	27.000,	27.000,	},
{14.300,	11.400,	19.700,	25.300,	26.000,	26.000,	26.000,	26.000,	26.000,	26.000,	26.400,	26.800,	27.000,	27.000,	27.000,	27.000,	},
};
	copyTable(config->ignitionTable, hardCodedignitionTable);
}

static void cannedignBlends1_rpmBins() {
	static const float hardCodedignBlends1_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].ignBlends[0].rpmBins, hardCodedignBlends1_rpmBins);
}

static void cannedignBlends1_loadBins() {
	static const float hardCodedignBlends1_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].ignBlends[0].loadBins, hardCodedignBlends1_loadBins);
}

static void cannedignBlends2_rpmBins() {
	static const float hardCodedignBlends2_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].ignBlends[1].rpmBins, hardCodedignBlends2_rpmBins);
}

static void cannedignBlends2_loadBins() {
	static const float hardCodedignBlends2_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].ignBlends[1].loadBins, hardCodedignBlends2_loadBins);
}

static void cannedignBlends3_rpmBins() {
	static const float hardCodedignBlends3_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].ignBlends[2].rpmBins, hardCodedignBlends3_rpmBins);
}

static void cannedignBlends3_loadBins() {
	static const float hardCodedignBlends3_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].ignBlends[2].loadBins, hardCodedignBlends3_loadBins);
}

static void cannedignBlends4_rpmBins() {
	static const float hardCodedignBlends4_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].ignBlends[3].rpmBins, hardCodedignBlends4_rpmBins);
}

static void cannedignBlends4_loadBins() {
	static const float hardCodedignBlends4_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].ignBlends[3].loadBins, hardCodedignBlends4_loadBins);
}

static void cannedveBlends1_rpmBins() {
	static const float hardCodedveBlends1_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].veBlends[0].rpmBins, hardCodedveBlends1_rpmBins);
}

static void cannedveBlends1_loadBins() {
	static const float hardCodedveBlends1_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].veBlends[0].loadBins, hardCodedveBlends1_loadBins);
}

static void cannedveBlends2_rpmBins() {
	static const float hardCodedveBlends2_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].veBlends[1].rpmBins, hardCodedveBlends2_rpmBins);
}

static void cannedveBlends2_loadBins() {
	static const float hardCodedveBlends2_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].veBlends[1].loadBins, hardCodedveBlends2_loadBins);
}

static void cannedveBlends3_rpmBins() {
	static const float hardCodedveBlends3_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].veBlends[2].rpmBins, hardCodedveBlends3_rpmBins);
}

static void cannedveBlends3_loadBins() {
	static const float hardCodedveBlends3_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].veBlends[2].loadBins, hardCodedveBlends3_loadBins);
}

static void cannedveBlends4_rpmBins() {
	static const float hardCodedveBlends4_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].veBlends[3].rpmBins, hardCodedveBlends4_rpmBins);
}

static void cannedveBlends4_loadBins() {
	static const float hardCodedveBlends4_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->boostClosedLoopBlends[1].veBlends[3].loadBins, hardCodedveBlends4_loadBins);
}

static void cannedveRpmBins() {
	static const float hardCodedveRpmBins[16] = {1200.0, 1500.0, 2000.0, 3000.0, 4000.0, 4500.0, 5000.0, 5500.0, 6000.0, 6500.0, 7000.0, 7500.0, 8000.0, 8500.0, 9000.0, 10000.0};
	copyArray(config->veRpmBins, hardCodedveRpmBins);
}

static void cannedveLoadBins() {
	static const float hardCodedveLoadBins[16] = {0.0, 2.0, 4.0, 6.0, 8.0, 10.0, 20.0, 25.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0};
	copyArray(config->veLoadBins, hardCodedveLoadBins);
}

static void cannedveTable() {
	static const float hardCodedveTable[16][16] = {
{20.000,	20.000,	20.000,	20.000,	25.000,	25.000,	10.000,	10.000,	10.000,	10.000,	10.000,	10.000,	10.000,	15.000,	15.000,	15.000,	},
{20.000,	20.000,	20.000,	20.000,	25.000,	25.000,	10.000,	10.000,	10.000,	10.000,	10.000,	10.000,	10.000,	15.000,	15.000,	15.000,	},
{20.000,	20.000,	20.000,	20.000,	20.000,	20.000,	15.000,	15.000,	15.000,	15.000,	15.000,	15.000,	15.000,	15.000,	15.000,	15.000,	},
{20.000,	20.000,	20.000,	20.000,	20.000,	20.000,	15.000,	15.000,	15.000,	15.000,	15.000,	15.000,	15.000,	15.000,	15.000,	15.000,	},
{20.000,	20.000,	20.000,	20.000,	20.900,	21.000,	16.100,	16.100,	16.100,	16.100,	16.100,	16.100,	16.100,	16.100,	16.100,	16.100,	},
{20.000,	20.000,	20.000,	20.000,	21.900,	21.900,	17.100,	17.100,	17.100,	17.100,	17.100,	17.100,	17.100,	17.100,	17.100,	17.100,	},
{19.500,	19.500,	19.500,	19.500,	22.500,	22.500,	22.500,	22.500,	22.500,	22.500,	22.500,	22.500,	22.500,	22.500,	22.500,	22.500,	},
{22.100,	22.100,	22.100,	22.100,	23.800,	23.800,	23.800,	23.800,	23.800,	23.800,	23.800,	23.800,	23.800,	23.800,	23.800,	23.800,	},
{44.700,	44.700,	44.700,	44.700,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	25.000,	},
{50.000,	50.000,	50.000,	50.000,	17.500,	27.500,	27.500,	27.500,	27.500,	27.500,	27.500,	27.500,	27.500,	27.500,	27.500,	27.500,	},
{50.000,	50.000,	50.000,	50.000,	20.000,	30.000,	30.000,	30.000,	30.000,	30.000,	30.000,	30.000,	30.000,	30.000,	30.000,	30.000,	},
{50.000,	50.000,	50.000,	50.000,	22.500,	32.500,	32.500,	32.500,	32.500,	32.500,	32.500,	32.500,	32.500,	32.500,	32.500,	32.500,	},
{50.000,	50.000,	50.000,	50.000,	25.000,	35.000,	35.000,	35.000,	35.000,	35.000,	35.000,	35.000,	35.000,	35.000,	35.000,	35.000,	},
{50.000,	50.000,	50.000,	50.000,	44.000,	44.000,	44.000,	44.000,	44.000,	44.000,	44.000,	44.000,	44.000,	44.000,	44.000,	44.000,	},
{65.000,	65.000,	65.000,	65.000,	44.000,	44.000,	44.000,	44.000,	44.000,	44.000,	44.000,	44.000,	44.000,	44.000,	44.000,	44.000,	},
{65.000,	65.000,	65.000,	65.000,	44.000,	44.000,	44.000,	44.000,	44.000,	44.000,	44.000,	44.000,	44.000,	44.000,	44.000,	44.000,	},
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

static void cannedpedalToTpsRpmBins() {
	static const float hardCodedpedalToTpsRpmBins[8] = {0.0, 1100.0, 2300.0, 3400.0, 4600.0, 5700.0, 6900.0, 8000.0};
	copyArray(config->pedalToTpsRpmBins, hardCodedpedalToTpsRpmBins);
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
	copyTable(config->lambdaTable, hardCodedlambdaTable, 1.0 / 14.7);
}

static void cannedgppwm4_table() {
	static const float hardCodedgppwm4_table[8][8] = {
{50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	},
{50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	},
{50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	},
{50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	},
{50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	},
{50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	},
{50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	},
{50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	50.000,	},
};
	copyTable(engineConfiguration->gppwm[3].gppwm4_table, hardCodedgppwm4_table);
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
