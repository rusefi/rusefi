#include "pch.h"

static int addMafPoint(int index, float kgHrValue, float voltage) {
	config->mafDecoding[index] = kgHrValue;
	config->mafDecodingBins[index] = voltage;
	return index + 1;
}

static int addMafPointByVoltage(int index, float voltage, float kgHrValue) {
	return addMafPoint(index, kgHrValue, voltage);
}

static void fillTheRest(int i) {
	/**
	 * unrealistic values just to make binary search happy
	 */
	while (i < MAF_DECODING_COUNT) {
		i = addMafPoint(i, 3000, 2010 + i);
	}
}

/**
 * Hot-film air-mass meter, Type HFM 5
 */
void setBosch0280218037() {
	int i = 0;
	i = addMafPoint(i, -34.5, 0);
	i = addMafPoint(i, -6, 0.78125);

	config->mafDecoding[i] = 10.5;
	config->mafDecodingBins[i++] = 1.38671875;

	config->mafDecoding[i] = 105.3;
	config->mafDecodingBins[i++] = 2.91015625;

	config->mafDecoding[i] = 387.5;
	config->mafDecodingBins[i++] = 4.2578125;

	config->mafDecoding[i] = 738;
	config->mafDecodingBins[i++] = 4.98046875;

	fillTheRest(i);
}

void setBosch0280218004() {
	int i = 0;

	fillTheRest(i);
}

void setNissanMAF0031() {
	int i = 0;
	  // I am copy-pasting from a spreadsheet, it works better if voltage goes first
	i = addMafPointByVoltage(i, 0.29, 3.1768838175);
	i = addMafPointByVoltage(i, 0.49, 3.6987752861);
	i = addMafPointByVoltage(i, 0.72, 5.8013108424);
	i = addMafPointByVoltage(i, 1, 11.1849);
	i = addMafPointByVoltage(i, 1.37, 24.5646673361);
	i = addMafPointByVoltage(i, 1.66, 41.453048941);
	i = addMafPointByVoltage(i, 1.91, 61.5847903829);
	i = addMafPointByVoltage(i, 2.09, 79.7924502089);
	i = addMafPointByVoltage(i, 2.34, 110.961012317);
	i = addMafPointByVoltage(i, 2.89, 208.198652496);
	i = addMafPointByVoltage(i, 3.11, 260.1030585044);
	i = addMafPointByVoltage(i, 3.54, 387.150427974);
	i = addMafPointByVoltage(i, 3.81, 486.5363959026);
	i = addMafPointByVoltage(i, 4, 566.628);
	i = addMafPointByVoltage(i, 4.32, 722.3485684449);
	i = addMafPointByVoltage(i, 4.65, 913.0847954331);
	i = addMafPointByVoltage(i, 4.98, 1137.8746972553);
	fillTheRest(i);
}
