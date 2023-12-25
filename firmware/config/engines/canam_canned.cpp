
static void cannedsparkDwellValues() {
	static const float hardCodedsparkDwellValues[8] = {4.0, 3.3, 3.2, 3.1, 3.05, 2.97, 2.88, 2.8};
	copyArray(config->sparkDwellValues, hardCodedsparkDwellValues);
}

static void canneddwellVoltageCorrValues() {
	static const float hardCodeddwellVoltageCorrValues[8] = {1.26, 1.2, 1.16, 1.1, 1.06, 1.0, 0.9, 0.8};
	copyArray(engineConfiguration->dwellVoltageCorrValues, hardCodeddwellVoltageCorrValues);
}

static void cannedidleAdvanceBins() {
	static const float hardCodedidleAdvanceBins[8] = {600.0, 800.0, 1000.0, 1200.0, 1400.0, 1600.0, 1800.0, 2000.0};
	copyArray(config->idleAdvanceBins, hardCodedidleAdvanceBins);
}

static void cannedidleAdvance() {
	static const float hardCodedidleAdvance[8] = {10.0, 10.571428, 11.142858, 11.714286, 12.285714, 12.857142, 13.428572, 14.0};
	copyArray(config->idleAdvance, hardCodedidleAdvance);
}

static void cannedignitionRpmBins() {
	static const float hardCodedignitionRpmBins[16] = {800.0, 1000.0, 1300.0, 1600.0, 2000.0, 2500.0, 3000.0, 3600.0, 4000.0, 4400.0, 4900.0, 5300.0, 5800.0, 6200.0, 7100.0, 8000.0};
	copyArray(config->ignitionRpmBins, hardCodedignitionRpmBins);
}

static void cannedignitionLoadBins() {
	static const float hardCodedignitionLoadBins[16] = {15.0, 27.0, 33.0, 45.0, 57.0, 69.0, 81.0, 93.0, 105.0, 129.0, 157.0, 186.0, 214.0, 243.0, 271.0, 300.0};
	copyArray(config->ignitionLoadBins, hardCodedignitionLoadBins);
}

static void cannedveRpmBins() {
	static const float hardCodedveRpmBins[16] = {800.0, 1000.0, 1300.0, 1600.0, 2000.0, 2500.0, 3000.0, 3600.0, 4000.0, 4400.0, 4900.0, 5300.0, 5800.0, 6200.0, 7100.0, 8000.0};
	copyArray(config->veRpmBins, hardCodedveRpmBins);
}

static void cannedveLoadBins() {
	static const float hardCodedveLoadBins[16] = {15.0, 27.0, 33.0, 45.0, 57.0, 69.0, 81.0, 93.0, 105.0, 129.0, 157.0, 186.0, 214.0, 243.0, 271.0, 300.0};
	copyArray(config->veLoadBins, hardCodedveLoadBins);
}
