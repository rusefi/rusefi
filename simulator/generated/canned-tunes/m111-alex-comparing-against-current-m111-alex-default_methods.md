```
static void cannedknockNoiseRpmBins() {
	static const float hardCodedknockNoiseRpmBins[16] = {650.0, 720.0, 790.0, 1160.0, 1580.0, 2000.0, 2420.0, 2840.0, 3260.0, 3680.0, 4100.0, 4520.0, 4940.0, 5413.0, 6207.0, 7000.0};
	copyArray(config->knockNoiseRpmBins, hardCodedknockNoiseRpmBins);
}

static void cannedknockBaseNoise() {
	static const float hardCodedknockBaseNoise[16] = {-39.0, -39.0, -39.0, -34.0, -27.0, -24.0, -23.0, -22.0, -20.0, -16.0, -13.0, -12.0, -10.0, -8.0, -6.0, -2.0};
	copyArray(config->knockBaseNoise, hardCodedknockBaseNoise);
}

static void cannedscriptCurve5Bins() {
	static const float hardCodedscriptCurve5Bins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->scriptCurve5Bins, hardCodedscriptCurve5Bins);
}

static void cannedscriptCurve6Bins() {
	static const float hardCodedscriptCurve6Bins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->scriptCurve6Bins, hardCodedscriptCurve6Bins);
}

static void cannedsparkDwellValues() {
	static const float hardCodedsparkDwellValues[8] = {4.0, 3.0, 3.0, 2.6, 2.6, 2.6, 2.6, 2.6};
	copyArray(config->sparkDwellValues, hardCodedsparkDwellValues);
}

static void canneddwellVoltageCorrVoltBins() {
	static const float hardCodeddwellVoltageCorrVoltBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->dwellVoltageCorrVoltBins, hardCodeddwellVoltageCorrVoltBins);
}

static void canneddwellVoltageCorrValues() {
	static const float hardCodeddwellVoltageCorrValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->dwellVoltageCorrValues, hardCodeddwellVoltageCorrValues);
}

static void cannedprimeBins() {
	static const float hardCodedprimeBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->primeBins, hardCodedprimeBins);
}

static void cannedprimeValues() {
	static const float hardCodedprimeValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->primeValues, hardCodedprimeValues);
}

static void cannedmap_samplingAngleBins() {
	static const float hardCodedmap_samplingAngleBins[8] = {800.0, 1686.0, 2571.0, 3457.0, 4343.0, 5229.0, 6114.0, 7000.0};
	copyArray(engineConfiguration->map.map_samplingAngleBins, hardCodedmap_samplingAngleBins);
}

static void cannedmap_samplingWindowBins() {
	static const float hardCodedmap_samplingWindowBins[8] = {800.0, 1686.0, 2571.0, 3457.0, 4343.0, 5229.0, 6114.0, 7000.0};
	copyArray(engineConfiguration->map.map_samplingWindowBins, hardCodedmap_samplingWindowBins);
}

static void cannedinjector_battLagCorrBins() {
	static const float hardCodedinjector_battLagCorrBins[8] = {6.0, 8.0, 10.0, 11.0, 12.0, 14.0, 16.0, 18.0};
	copyArray(engineConfiguration->injectorSecondary.injector_battLagCorrBins, hardCodedinjector_battLagCorrBins);
}

static void cannedinjector_battLagCorr() {
	static const float hardCodedinjector_battLagCorr[8] = {3.3710022, 2.84, 2.0, 1.8050001, 1.61, 1.37, 1.18, 1.04};
	copyArray(engineConfiguration->injectorSecondary.injector_battLagCorr, hardCodedinjector_battLagCorr);
}

static void cannedvrThreshold1_rpmBins() {
	static const float hardCodedvrThreshold1_rpmBins[6] = {600.0, 1000.0, 3500.0, 4500.0, 5500.0, 7000.0};
	copyArray(engineConfiguration->vrThreshold2.vrThreshold1_rpmBins, hardCodedvrThreshold1_rpmBins);
}

static void cannedvrThreshold1_values() {
	static const float hardCodedvrThreshold1_values[6] = {0.3, 1.2, 2.4, 2.43, 2.47, 2.5};
	copyArray(engineConfiguration->vrThreshold2.vrThreshold1_values, hardCodedvrThreshold1_values);
}

static void cannedvrThreshold2_rpmBins() {
	static const float hardCodedvrThreshold2_rpmBins[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->vrThreshold2.vrThreshold2_rpmBins, hardCodedvrThreshold2_rpmBins);
}

static void cannedvrThreshold2_values() {
	static const float hardCodedvrThreshold2_values[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->vrThreshold2.vrThreshold2_values, hardCodedvrThreshold2_values);
}

static void cannedmafDecodingBins() {
	static const float hardCodedmafDecodingBins[32] = {0.0, 18.013283, 26.451612, 34.677418, 42.903225, 51.129032, 59.35484, 67.580635, 75.80645, 84.03226, 92.258064, 100.48387, 108.70968, 116.935486, 125.16129, 133.3871, 141.6129, 149.83871, 158.06451, 166.29033, 174.51613, 182.74193, 190.96774, 199.19353, 207.41936, 215.64516, 223.87097, 232.09677, 240.3226, 248.54839, 256.7742, 265.0};
	copyArray(config->mafDecodingBins, hardCodedmafDecodingBins);
}

static void cannedmafDecoding() {
	static const float hardCodedmafDecoding[32] = {-34.5, 195.01631, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0};
	copyArray(config->mafDecoding, hardCodedmafDecoding);
}

static void cannediatFuelCorr() {
	static const float hardCodediatFuelCorr[16] = {1.11, 1.07, 1.05, 1.02, 1.0, 0.98, 0.95, 0.93, 0.92, 0.9, 0.87, 0.85, 0.82, 0.8066667, 0.7933333, 0.78};
	copyArray(config->iatFuelCorr, hardCodediatFuelCorr);
}

static void cannedcltTimingBins() {
	static const float hardCodedcltTimingBins[8] = {-40.0, -15.0, 10.0, 20.0, 50.0, 80.0, 97.0, 120.0};
	copyArray(config->cltTimingBins, hardCodedcltTimingBins);
}

static void cannedcltTimingExtra() {
	static const float hardCodedcltTimingExtra[8] = {11.0, 9.0, 7.0, 5.0, 2.0, 0.0, 0.0, -2.0};
	copyArray(config->cltTimingExtra, hardCodedcltTimingExtra);
}

static void cannedcltFuelCorrBins() {
	static const float hardCodedcltFuelCorrBins[16] = {-40.0, -30.0, -20.0, -10.0, 0.0, 9.71, 21.46, 31.48, 41.17, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0, 110.0};
	copyArray(config->cltFuelCorrBins, hardCodedcltFuelCorrBins);
}

static void cannedcltFuelCorr() {
	static const float hardCodedcltFuelCorr[16] = {1.53, 1.53, 1.4499981, 1.3900006, 1.3099988, 1.25, 1.1900002, 1.1500002, 1.1000001, 1.06, 1.02, 1.0000021, 1.0, 1.0, 1.0, 1.0};
	copyArray(config->cltFuelCorr, hardCodedcltFuelCorr);
}

static void cannedcrankingFuelBins() {
	static const float hardCodedcrankingFuelBins[8] = {-27.15, -3.94, 4.77, 20.0, 35.0, 50.0, 65.0, 90.0};
	copyArray(config->crankingFuelBins, hardCodedcrankingFuelBins);
}

static void cannedcrankingFuelCoef() {
	static const float hardCodedcrankingFuelCoef[8] = {3.0, 2.64, 2.13, 1.550003, 1.300003, 1.0999985, 1.0, 1.0};
	copyArray(config->crankingFuelCoef, hardCodedcrankingFuelCoef);
}

static void cannedcrankingFuelCoefE100() {
	static const float hardCodedcrankingFuelCoefE100[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->crankingFuelCoefE100, hardCodedcrankingFuelCoefE100);
}

static void cannedcltRevLimitRpmBins() {
	static const float hardCodedcltRevLimitRpmBins[4] = {0.0, 0.0, 0.0, 0.0};
	copyArray(config->cltRevLimitRpmBins, hardCodedcltRevLimitRpmBins);
}

static void cannedetbBiasBins() {
	static const float hardCodedetbBiasBins[8] = {0.0, 2.0, 4.0, 10.0, 30.0, 50.0, 80.0, 100.0};
	copyArray(config->etbBiasBins, hardCodedetbBiasBins);
}

static void cannedetbBiasValues() {
	static const float hardCodedetbBiasValues[8] = {-18.0, -18.0, -18.0, 16.0, 17.0, 15.0, 16.0, 22.0};
	copyArray(config->etbBiasValues, hardCodedetbBiasValues);
}

static void cannedcltIdleCorrBins() {
	static const float hardCodedcltIdleCorrBins[16] = {-40.0, -30.410034, -21.25, -9.919983, 0.11000013, 10.0, 20.0, 30.0, 39.779785, 50.52, 60.0, 70.0, 80.0, 90.0, 100.0, 110.0};
	copyArray(config->cltIdleCorrBins, hardCodedcltIdleCorrBins);
}

static void cannedcltIdleCorr() {
	static const float hardCodedcltIdleCorr[16] = {1.6900029, 1.5400014, 1.4800038, 1.4199986, 1.3700032, 1.3200002, 1.196672, 1.0933361, 1.059998, 1.0300001, 1.0000031, 0.99999666, 1.0, 1.0, 1.0, 1.0};
	copyArray(config->cltIdleCorr, hardCodedcltIdleCorr);
}

static void cannedcltCrankingCorrBins() {
	static const float hardCodedcltCrankingCorrBins[8] = {-40.0, -20.0, 0.0, 19.83, 40.0, 60.0, 80.0, 100.0};
	copyArray(config->cltCrankingCorrBins, hardCodedcltCrankingCorrBins);
}

static void cannedcltCrankingCorr() {
	static const float hardCodedcltCrankingCorr[8] = {1.5, 1.3366667, 1.1733334, 1.05, 0.96000004, 0.8100002, 0.69000024, 0.6500002};
	copyArray(config->cltCrankingCorr, hardCodedcltCrankingCorr);
}

static void cannedcltIdleRpmBins() {
	static const float hardCodedcltIdleRpmBins[16] = {-40.0, -30.0, -20.0, 0.0, 22.0, 36.0, 46.0, 56.0, 60.0, 70.0, 80.0, 90.0, 100.0, 120.0, 130.0, 140.0};
	copyArray(config->cltIdleRpmBins, hardCodedcltIdleRpmBins);
}

static void cannedcltIdleRpm() {
	static const float hardCodedcltIdleRpm[16] = {1220.0, 1200.0, 1200.0, 1000.0, 1000.0, 960.0, 900.0, 860.0, 800.0, 800.0, 800.0, 800.0, 900.0, 480.0, 0.0, 0.0};
	copyArray(config->cltIdleRpm, hardCodedcltIdleRpm);
}

static void cannedfuelLevelBins() {
	static const float hardCodedfuelLevelBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->fuelLevelBins, hardCodedfuelLevelBins);
}

static void cannedhpfpLobeProfileQuantityBins() {
	static const float hardCodedhpfpLobeProfileQuantityBins[16] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->hpfpLobeProfileQuantityBins, hardCodedhpfpLobeProfileQuantityBins);
}

static void cannedhpfpLobeProfileAngle() {
	static const float hardCodedhpfpLobeProfileAngle[16] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->hpfpLobeProfileAngle, hardCodedhpfpLobeProfileAngle);
}

static void cannedhpfpDeadtimeVoltsBins() {
	static const float hardCodedhpfpDeadtimeVoltsBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->hpfpDeadtimeVoltsBins, hardCodedhpfpDeadtimeVoltsBins);
}

static void cannedwwTauCltValues() {
	static const float hardCodedwwTauCltValues[8] = {0.95, 0.8, 0.67, 0.55, 0.4, 0.32, 0.25, 0.2};
	copyArray(config->wwTauCltValues, hardCodedwwTauCltValues);
}

static void cannedwwMapBins() {
	static const float hardCodedwwMapBins[8] = {7.0, 18.0, 29.0, 40.0, 49.0, 60.0, 70.0, 88.0};
	copyArray(config->wwMapBins, hardCodedwwMapBins);
}

static void cannedwwTauMapValues() {
	static const float hardCodedwwTauMapValues[8] = {0.76, 0.83, 0.84, 0.86, 0.87, 0.92, 0.94, 0.95};
	copyArray(config->wwTauMapValues, hardCodedwwTauMapValues);
}

static void cannedwwBetaCltValues() {
	static const float hardCodedwwBetaCltValues[8] = {0.79, 0.72, 0.63, 0.52, 0.44, 0.37, 0.3, 0.25};
	copyArray(config->wwBetaCltValues, hardCodedwwBetaCltValues);
}

static void cannedwwBetaMapValues() {
	static const float hardCodedwwBetaMapValues[8] = {0.05, 0.16, 0.25, 0.7, 0.78, 0.96, 1.05, 1.18};
	copyArray(config->wwBetaMapValues, hardCodedwwBetaMapValues);
}

static void cannedignBlends1_blendBins() {
	static const float hardCodedignBlends1_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends1_blendBins, hardCodedignBlends1_blendBins);
}

static void cannedignBlends1_blendValues() {
	static const float hardCodedignBlends1_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends1_blendValues, hardCodedignBlends1_blendValues);
}

static void cannedignBlends2_blendBins() {
	static const float hardCodedignBlends2_blendBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends2_blendBins, hardCodedignBlends2_blendBins);
}

static void cannedignBlends2_blendValues() {
	static const float hardCodedignBlends2_blendValues[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
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

static void cannedtpsTpsAccelFromRpmBins() {
	static const float hardCodedtpsTpsAccelFromRpmBins[8] = {0.0, 5.0, 10.0, 20.0, 60.0, 70.0, 90.0, 100.0};
	copyArray(config->tpsTpsAccelFromRpmBins, hardCodedtpsTpsAccelFromRpmBins);
}

static void cannedtpsTpsAccelToRpmBins() {
	static const float hardCodedtpsTpsAccelToRpmBins[8] = {0.0, 5.0, 10.0, 20.0, 60.0, 70.0, 90.0, 100.0};
	copyArray(config->tpsTpsAccelToRpmBins, hardCodedtpsTpsAccelToRpmBins);
}

static void cannedtpsTpsAccelTable() {
	static const float hardCodedtpsTpsAccelTable[8][8] = {
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{7.240,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{7.242,	2.880,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{11.927,	4.896,	2.500,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{14.515,	6.624,	4.250,	2.500,	0.000,	0.000,	0.000,	0.000,	},
{16.589,	8.064,	5.750,	4.250,	2.500,	0.000,	0.000,	0.000,	},
{18.144,	9.216,	7.000,	5.750,	4.250,	5.000,	0.000,	0.000,	},
{19.181,	10.080,	8.000,	7.000,	5.750,	8.500,	5.000,	0.000,	},
};
	copyTable(config->tpsTpsAccelTable, hardCodedtpsTpsAccelTable);
}

static void cannedtractionControlSlipBins() {
	static const float hardCodedtractionControlSlipBins[6] = {0.8, 0.9, 0.95, 1.05, 1.1, 1.2};
	copyArray(engineConfiguration->tractionControlSlipBins, hardCodedtractionControlSlipBins);
}

static void cannedboostRpmBins() {
	static const float hardCodedboostRpmBins[8] = {0.0, 1200.0, 2300.0, 3500.0, 4600.0, 5700.0, 6900.0, 8000.0};
	copyArray(config->boostRpmBins, hardCodedboostRpmBins);
}

static void cannedboostTpsBins() {
	static const float hardCodedboostTpsBins[8] = {0.0, 14.0, 28.0, 42.0, 58.0, 72.0, 86.0, 100.0};
	copyArray(config->boostTpsBins, hardCodedboostTpsBins);
}

static void cannedboostTableOpenLoop() {
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

static void cannedboostTableClosedLoop() {
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

static void cannedscriptTable1RpmBins() {
	static const float hardCodedscriptTable1RpmBins[8] = {650.0, 800.0, 1100.0, 1500.0, 3500.0, 4400.0, 5300.0, 7000.0};
	copyArray(config->scriptTable1RpmBins, hardCodedscriptTable1RpmBins);
}

static void cannedscriptTable1LoadBins() {
	static const float hardCodedscriptTable1LoadBins[8] = {20.0, 25.0, 30.0, 40.0, 50.0, 70.0, 85.0, 100.0};
	copyArray(config->scriptTable1LoadBins, hardCodedscriptTable1LoadBins);
}

static void cannedscriptTable1() {
	static const float hardCodedscriptTable1[8][8] = {
{0.100,	0.100,	0.100,	0.100,	0.100,	0.100,	0.100,	0.100,	},
{0.100,	0.100,	0.100,	0.100,	0.100,	0.100,	0.100,	0.100,	},
{0.000,	0.200,	0.100,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.200,	0.050,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
};
	copyTable(config->scriptTable1, hardCodedscriptTable1);
}

static void cannedscriptTable4RpmBins() {
	static const float hardCodedscriptTable4RpmBins[10] = {650.0, 859.0, 1353.0, 2000.0, 2700.0, 3400.0, 4100.0, 4856.0, 5777.0, 7000.0};
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
	static const float hardCodedignitionRpmBins[16] = {300.0, 700.0, 800.0, 900.0, 1100.0, 1400.0, 1700.0, 2100.0, 2500.0, 3000.0, 3500.0, 4000.0, 4500.0, 5000.0, 6500.0, 7000.0};
	copyArray(config->ignitionRpmBins, hardCodedignitionRpmBins);
}

static void cannedignitionLoadBins() {
	static const float hardCodedignitionLoadBins[16] = {30.0, 35.0, 40.0, 45.0, 50.0, 55.0, 60.0, 65.0, 70.0, 75.0, 80.0, 85.0, 90.0, 95.0, 100.0, 110.0};
	copyArray(config->ignitionLoadBins, hardCodedignitionLoadBins);
}

static void cannedignitionTable() {
	static const float hardCodedignitionTable[16][16] = {
{0.000,	8.000,	8.000,	8.000,	8.000,	10.000,	18.400,	22.800,	23.700,	30.700,	30.700,	30.700,	30.700,	30.700,	30.700,	30.700,	},
{0.000,	8.000,	8.000,	8.000,	8.000,	11.500,	19.700,	24.000,	25.000,	30.500,	30.500,	30.500,	30.500,	30.500,	30.500,	30.500,	},
{0.000,	8.000,	8.000,	8.000,	8.000,	13.000,	21.000,	25.100,	26.300,	30.200,	30.200,	30.200,	30.200,	30.200,	30.200,	30.200,	},
{0.000,	8.000,	8.000,	8.000,	8.000,	14.500,	22.300,	26.300,	27.500,	30.000,	30.000,	30.000,	30.000,	30.000,	30.000,	30.000,	},
{0.000,	8.000,	8.000,	8.000,	8.000,	16.000,	23.600,	27.400,	28.800,	29.800,	29.800,	29.800,	29.800,	29.800,	29.800,	29.800,	},
{0.000,	8.000,	8.000,	8.000,	8.000,	16.000,	23.400,	27.200,	28.500,	29.500,	29.500,	29.500,	29.500,	29.500,	29.500,	29.500,	},
{0.000,	8.000,	8.000,	8.000,	8.000,	16.000,	23.200,	26.900,	28.200,	29.200,	29.200,	29.200,	29.200,	29.200,	29.200,	29.200,	},
{0.000,	10.000,	10.000,	10.000,	10.000,	16.000,	23.100,	26.600,	28.000,	28.900,	28.900,	28.900,	28.900,	28.900,	28.900,	28.900,	},
{0.000,	10.000,	10.000,	10.000,	10.000,	16.000,	23.000,	26.200,	27.600,	28.500,	28.500,	28.500,	28.500,	28.500,	28.500,	28.500,	},
{0.000,	10.000,	10.000,	10.000,	10.000,	16.000,	22.500,	25.700,	27.000,	28.000,	28.000,	28.000,	28.000,	28.000,	28.000,	28.000,	},
{0.000,	10.000,	10.000,	10.000,	10.000,	16.000,	22.000,	25.100,	26.500,	27.400,	27.400,	27.400,	27.400,	27.400,	27.400,	27.400,	},
{0.000,	10.000,	10.000,	10.000,	10.000,	15.900,	21.500,	24.600,	26.000,	27.000,	27.000,	27.000,	27.000,	27.000,	27.000,	27.000,	},
{0.000,	10.000,	10.000,	10.000,	10.000,	15.800,	21.000,	24.200,	25.500,	26.500,	26.500,	26.500,	26.500,	26.500,	26.500,	26.500,	},
{0.000,	10.000,	10.000,	10.000,	10.000,	15.100,	20.000,	23.100,	25.300,	25.800,	25.800,	25.800,	25.800,	25.800,	25.800,	25.800,	},
{0.000,	10.000,	10.000,	10.000,	10.000,	14.400,	19.000,	21.900,	25.100,	25.100,	25.100,	25.100,	25.100,	25.100,	25.100,	25.100,	},
{0.000,	10.000,	10.000,	10.000,	10.000,	13.200,	16.500,	19.100,	22.000,	23.700,	23.700,	23.700,	23.700,	23.700,	23.700,	23.700,	},
};
	copyTable(config->ignitionTable, hardCodedignitionTable);
}

static void cannedignBlends1_rpmBins() {
	static const float hardCodedignBlends1_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends1_rpmBins, hardCodedignBlends1_rpmBins);
}

static void cannedignBlends1_loadBins() {
	static const float hardCodedignBlends1_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends1_loadBins, hardCodedignBlends1_loadBins);
}

static void cannedignBlends2_rpmBins() {
	static const float hardCodedignBlends2_rpmBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends2_rpmBins, hardCodedignBlends2_rpmBins);
}

static void cannedignBlends2_loadBins() {
	static const float hardCodedignBlends2_loadBins[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(engineConfiguration->boostClosedLoopBlends2.ignBlends2_loadBins, hardCodedignBlends2_loadBins);
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
	static const float hardCodedignitionIatCorrLoadBins[8] = {0.0, 5.0, 10.0, 25.0, 45.0, 65.0, 90.0, 110.0};
	copyArray(config->ignitionIatCorrLoadBins, hardCodedignitionIatCorrLoadBins);
}

static void cannedignitionIatCorrTable() {
	static const float hardCodedignitionIatCorrTable[8][8] = {
{4.000,	4.000,	4.000,	4.000,	4.000,	4.000,	4.000,	4.000,	},
{2.900,	2.900,	2.900,	2.900,	2.900,	2.900,	2.900,	2.900,	},
{0.700,	0.700,	0.700,	0.700,	0.700,	0.700,	0.700,	0.700,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{-0.600,	-0.600,	-0.600,	-0.600,	-0.600,	-0.600,	-0.600,	-0.600,	},
{-3.400,	-3.400,	-3.400,	-3.400,	-3.400,	-3.400,	-3.400,	-3.400,	},
{-4.000,	-4.000,	-4.000,	-4.000,	-4.000,	-4.000,	-4.000,	-4.000,	},
{-6.000,	-6.000,	-6.000,	-6.000,	-6.000,	-6.000,	-6.000,	-6.000,	},
};
	copyTable(config->ignitionIatCorrTable, hardCodedignitionIatCorrTable);
}

static void cannedveRpmBins() {
	static const float hardCodedveRpmBins[16] = {650.0, 800.0, 1000.0, 1200.0, 1500.0, 2000.0, 2500.0, 3000.0, 3500.0, 4000.0, 4500.0, 5000.0, 5500.0, 6000.0, 6500.0, 7000.0};
	copyArray(config->veRpmBins, hardCodedveRpmBins);
}

static void cannedveLoadBins() {
	static const float hardCodedveLoadBins[16] = {15.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 55.0, 65.0, 70.0, 75.0, 80.0, 85.0, 90.0, 95.0, 100.0};
	copyArray(config->veLoadBins, hardCodedveLoadBins);
}

static void cannedveTable() {
	static const float hardCodedveTable[16][16] = {
{27.800,	27.500,	17.300,	7.800,	5.400,	6.500,	7.200,	10.300,	11.600,	13.700,	27.600,	30.000,	23.400,	23.800,	25.700,	34.400,	},
{27.800,	27.500,	17.300,	7.800,	5.400,	6.500,	7.200,	10.300,	11.600,	13.700,	27.600,	30.000,	23.400,	23.800,	25.700,	34.400,	},
{21.200,	22.400,	22.900,	12.800,	12.300,	12.600,	13.700,	13.800,	13.100,	28.800,	29.600,	32.400,	31.500,	32.300,	33.500,	34.400,	},
{27.400,	27.300,	28.100,	28.200,	29.500,	32.000,	37.800,	26.700,	24.000,	37.800,	39.000,	40.100,	45.000,	46.800,	48.000,	49.000,	},
{32.600,	33.900,	34.200,	39.900,	40.900,	43.200,	43.100,	39.600,	34.900,	40.600,	43.300,	47.300,	47.900,	49.700,	50.600,	51.500,	},
{38.000,	40.500,	46.600,	50.600,	50.400,	51.400,	50.800,	47.300,	44.400,	48.200,	51.800,	52.200,	49.900,	51.700,	52.400,	53.300,	},
{44.400,	47.900,	49.200,	60.200,	55.800,	58.600,	59.000,	53.700,	50.600,	49.400,	48.800,	50.600,	52.300,	54.000,	55.500,	57.100,	},
{41.400,	57.800,	66.200,	65.800,	65.500,	64.900,	63.200,	60.700,	59.100,	60.600,	61.300,	55.200,	55.500,	57.200,	58.900,	60.700,	},
{65.000,	68.700,	69.100,	70.000,	70.600,	71.600,	68.900,	66.900,	64.900,	69.600,	63.000,	64.400,	61.900,	63.500,	65.700,	67.900,	},
{69.000,	76.500,	74.000,	75.600,	73.600,	74.200,	72.500,	69.700,	68.600,	70.300,	65.100,	66.300,	63.300,	64.400,	66.400,	68.500,	},
{73.000,	84.200,	78.800,	81.200,	76.600,	76.800,	76.000,	72.500,	72.200,	70.900,	67.100,	68.100,	64.600,	65.300,	67.100,	69.100,	},
{74.200,	84.400,	78.800,	82.400,	77.900,	78.900,	78.100,	75.000,	75.600,	75.700,	75.200,	77.200,	73.500,	69.800,	70.500,	71.500,	},
{75.400,	84.500,	78.800,	83.600,	79.100,	81.000,	80.200,	77.500,	78.900,	80.500,	83.300,	86.300,	82.300,	74.200,	73.900,	73.900,	},
{76.900,	84.700,	78.800,	81.900,	80.000,	81.500,	81.600,	78.700,	80.700,	83.400,	86.500,	90.200,	89.500,	82.800,	82.700,	82.700,	},
{78.300,	84.800,	78.800,	80.200,	80.900,	82.000,	82.900,	79.900,	82.400,	86.200,	89.600,	94.000,	96.600,	91.400,	91.500,	91.500,	},
{80.000,	85.000,	84.900,	84.800,	84.700,	84.400,	84.200,	83.900,	83.700,	85.900,	89.100,	91.500,	91.600,	90.300,	92.000,	92.000,	},
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
	static const float hardCodedpedalToTpsRpmBins[8] = {0.0, 600.0, 800.0, 1000.0, 1500.0, 2000.0, 4000.0, 8000.0};
	copyArray(config->pedalToTpsRpmBins, hardCodedpedalToTpsRpmBins);
}

static void cannedpedalToTpsPedalBins() {
	static const float hardCodedpedalToTpsPedalBins[8] = {0.0, 10.0, 15.0, 20.0, 35.0, 71.0, 86.0, 100.0};
	copyArray(config->pedalToTpsPedalBins, hardCodedpedalToTpsPedalBins);
}

static void cannedpedalToTpsTable() {
	static const float hardCodedpedalToTpsTable[8][8] = {
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{2.000,	3.000,	2.000,	4.000,	4.000,	4.000,	4.000,	10.000,	},
{4.000,	4.000,	4.000,	5.000,	7.000,	14.000,	15.000,	15.000,	},
{8.000,	8.000,	8.000,	9.000,	12.000,	20.000,	20.000,	20.000,	},
{15.000,	15.000,	18.000,	20.000,	29.000,	33.000,	35.000,	35.000,	},
{40.000,	40.000,	40.000,	50.000,	60.000,	65.000,	71.000,	71.000,	},
{86.000,	86.000,	86.000,	86.000,	86.000,	86.000,	86.000,	86.000,	},
{100.000,	100.000,	100.000,	100.000,	100.000,	100.000,	100.000,	100.000,	},
};
	copyTable(config->pedalToTpsTable, hardCodedpedalToTpsTable);
}

static void cannedmaxKnockRetardRpmBins() {
	static const float hardCodedmaxKnockRetardRpmBins[6] = {500.0, 2000.0, 3000.0, 4000.0, 6000.0, 7000.0};
	copyArray(config->maxKnockRetardRpmBins, hardCodedmaxKnockRetardRpmBins);
}

static void cannedmaxKnockRetardLoadBins() {
	static const float hardCodedmaxKnockRetardLoadBins[6] = {0.0, 20.0, 50.0, 70.0, 90.0, 100.0};
	copyArray(config->maxKnockRetardLoadBins, hardCodedmaxKnockRetardLoadBins);
}

static void cannedmaxKnockRetardTable() {
	static const float hardCodedmaxKnockRetardTable[6][6] = {
{2.000,	2.000,	2.000,	2.000,	2.000,	2.000,	},
{2.000,	2.000,	2.000,	2.000,	2.000,	2.000,	},
{3.250,	3.250,	3.250,	3.250,	3.250,	3.250,	},
{4.250,	4.250,	4.250,	4.250,	4.250,	4.250,	},
{5.000,	5.000,	5.000,	5.000,	5.000,	5.000,	},
{5.000,	5.000,	5.000,	5.000,	5.000,	5.000,	},
};
	copyTable(config->maxKnockRetardTable, hardCodedmaxKnockRetardTable);
}

static void cannedlambdaLoadBins() {
	static const float hardCodedlambdaLoadBins[16] = {10.0, 20.0, 25.0, 30.0, 32.0, 40.0, 45.0, 50.0, 55.0, 60.0, 65.0, 70.0, 80.0, 85.0, 90.0, 100.0};
	copyArray(config->lambdaLoadBins, hardCodedlambdaLoadBins);
}

static void cannedlambdaTable() {
	static const float hardCodedlambdaTable[16][16] = {
{14.000,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.200,	14.200,	14.200,	14.200,	},
{14.000,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.200,	14.200,	14.200,	14.200,	},
{14.000,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.200,	14.200,	14.200,	14.200,	},
{14.000,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.200,	14.200,	14.200,	14.200,	},
{14.000,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.200,	14.200,	14.200,	14.200,	},
{14.000,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.200,	14.200,	14.000,	14.000,	},
{14.000,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.200,	14.000,	14.000,	},
{14.000,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.200,	14.000,	14.000,	},
{14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.200,	14.000,	14.000,	},
{14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.200,	14.000,	14.000,	},
{14.500,	14.500,	14.500,	14.500,	14.500,	14.500,	14.500,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.000,	14.000,	14.000,	},
{14.500,	14.500,	14.500,	14.500,	14.500,	14.500,	14.500,	14.700,	14.700,	14.700,	14.700,	14.700,	14.700,	14.000,	14.000,	14.000,	},
{14.200,	14.200,	14.200,	14.300,	14.300,	14.300,	14.300,	14.300,	14.700,	14.700,	14.700,	14.700,	14.700,	14.300,	14.300,	14.300,	},
{14.200,	14.200,	14.200,	14.200,	14.200,	14.200,	14.200,	14.200,	14.400,	14.400,	14.400,	14.400,	14.400,	14.200,	14.200,	14.200,	},
{14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	14.000,	},
{13.900,	13.900,	13.900,	13.900,	13.900,	13.900,	13.900,	13.900,	13.900,	13.900,	13.900,	13.900,	13.900,	13.900,	13.900,	13.900,	},
};
	copyTable(config->lambdaTable, hardCodedlambdaTable, 1.0 / 14.7);
}

static void cannediacPidMultRpmBins() {
	static const float hardCodediacPidMultRpmBins[8] = {600.0, 800.0, 900.0, 1000.0, 1100.0, 1200.0, 1300.0, 1500.0};
	copyArray(config->iacPidMultRpmBins, hardCodediacPidMultRpmBins);
}

static void cannediacPidMultLoadBins() {
	static const float hardCodediacPidMultLoadBins[8] = {10.0, 20.0, 30.0, 35.0, 40.0, 50.0, 70.0, 100.0};
	copyArray(config->iacPidMultLoadBins, hardCodediacPidMultLoadBins);
}

static void cannediacPidMultTable() {
	static const float hardCodediacPidMultTable[8][8] = {
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{6.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{8.000,	6.000,	4.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{9.000,	8.000,	4.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{10.000,	10.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
};
	copyTable(config->iacPidMultTable, hardCodediacPidMultTable);
}

static void cannedgppwm1_loadBins() {
	static const float hardCodedgppwm1_loadBins[8] = {0.0, 14.0, 70.0, 80.0, 82.0, 90.0, 92.0, 100.0};
	copyArray(engineConfiguration->gppwm4.gppwm1_loadBins, hardCodedgppwm1_loadBins);
}

static void cannedgppwm1_table() {
	static const float hardCodedgppwm1_table[8][8] = {
{90.000,	90.000,	90.000,	90.000,	90.000,	90.000,	90.000,	90.000,	},
{90.000,	90.000,	90.000,	90.000,	90.000,	90.000,	90.000,	90.000,	},
{85.000,	40.000,	40.000,	60.000,	85.000,	85.000,	85.000,	85.000,	},
{50.000,	40.000,	40.000,	50.000,	80.000,	80.000,	80.000,	80.000,	},
{40.000,	40.000,	40.000,	40.000,	60.000,	60.000,	60.000,	60.000,	},
{40.000,	40.000,	40.000,	40.000,	40.000,	40.000,	40.000,	40.000,	},
{30.000,	30.000,	30.000,	30.000,	30.000,	30.000,	30.000,	30.000,	},
{30.000,	30.000,	30.000,	30.000,	30.000,	30.000,	30.000,	30.000,	},
};
	copyTable(engineConfiguration->gppwm4.gppwm1_table, hardCodedgppwm1_table);
}

static void cannedgppwm2_rpmBins() {
	static const float hardCodedgppwm2_rpmBins[8] = {1000.0, 1900.0, 2100.0, 3000.0, 4000.0, 4900.0, 5100.0, 7000.0};
	copyArray(engineConfiguration->gppwm4.gppwm2_rpmBins, hardCodedgppwm2_rpmBins);
}

static void cannedgppwm2_table() {
	static const float hardCodedgppwm2_table[8][8] = {
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	100.000,	100.000,	100.000,	100.000,	0.000,	0.000,	},
{0.000,	0.000,	100.000,	100.000,	100.000,	100.000,	0.000,	0.000,	},
{0.000,	0.000,	100.000,	100.000,	100.000,	100.000,	0.000,	0.000,	},
{0.000,	0.000,	100.000,	100.000,	100.000,	100.000,	0.000,	0.000,	},
{0.000,	0.000,	100.000,	100.000,	100.000,	100.000,	0.000,	0.000,	},
};
	copyTable(engineConfiguration->gppwm4.gppwm2_table, hardCodedgppwm2_table);
}

static void cannedgppwm3_table() {
	static const float hardCodedgppwm3_table[8][8] = {
{5.000,	5.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{5.000,	5.000,	0.000,	0.000,	0.000,	0.000,	14.000,	14.000,	},
{70.000,	70.000,	0.000,	0.000,	0.000,	0.000,	28.000,	28.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	42.000,	42.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	57.000,	57.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	71.000,	71.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	85.000,	85.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	100.000,	100.000,	},
};
	copyTable(engineConfiguration->gppwm4.gppwm3_table, hardCodedgppwm3_table);
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
	static const float hardCodedhpfpTargetRpmBins[10] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->hpfpTargetRpmBins, hardCodedhpfpTargetRpmBins);
}

static void cannedhpfpTargetLoadBins() {
	static const float hardCodedhpfpTargetLoadBins[10] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->hpfpTargetLoadBins, hardCodedhpfpTargetLoadBins);
}

static void cannedhpfpCompensationRpmBins() {
	static const float hardCodedhpfpCompensationRpmBins[10] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->hpfpCompensationRpmBins, hardCodedhpfpCompensationRpmBins);
}

static void cannedhpfpCompensationLoadBins() {
	static const float hardCodedhpfpCompensationLoadBins[10] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	copyArray(config->hpfpCompensationLoadBins, hardCodedhpfpCompensationLoadBins);
}

static void cannedpostCrankingFactor() {
	static const float hardCodedpostCrankingFactor[6][6] = {
{1.200,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
{0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	},
};
	copyTable(engineConfiguration->postCrankingFactor, hardCodedpostCrankingFactor);
}

```
