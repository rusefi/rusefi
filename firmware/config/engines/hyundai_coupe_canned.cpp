#if HW_HELLEN_HYUNDAI
static void cannedprimeBins() {
	static const float hardCodedprimeBins[8] = {-40.0, -20.0, 0.0, 20.0, 40.0, 60.0, 80.0, 100.0};
	copyArray(engineConfiguration->primeBins, hardCodedprimeBins);
}

static void cannedprimeValues() {
	static const float hardCodedprimeValues[8] = {755.0, 605.0, 265.0, 140.0, 75.0, 50.0, 45.0, 40.0};
	copyArray(engineConfiguration->primeValues, hardCodedprimeValues);
}

static void cannedcltIdleCorrBins() {
	static const float hardCodedcltIdleCorrBins[16] = {-40.0, -30.0, -20.0, -10.0, 0.0, 10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0, 110.0};
	copyArray(config->cltIdleCorrBins, hardCodedcltIdleCorrBins);
}

static void cannedcltIdleCorr() {
	static const float hardCodedcltIdleCorr[16] = {1.5, 1.5, 1.333333, 1.333333, 1.333333, 1.333333, 1.333333, 1.333333, 1.333333, 1.233333, 1.166667, 1.1, 1.0, 1.0, 1.0, 1.0};
	copyArray(config->cltIdleCorr, hardCodedcltIdleCorr);
}
#endif // HW_HELLEN_HYUNDAI
