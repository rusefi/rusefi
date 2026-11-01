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

	config->mafDecoding[i] = 105.3f;
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
	i = addMafPointByVoltage(i, 0.29f, 3.1768838175f);
	i = addMafPointByVoltage(i, 0.49f, 3.6987752861f);
	i = addMafPointByVoltage(i, 0.72f, 5.8013108424f);
	i = addMafPointByVoltage(i, 1, 11.1849f);
	i = addMafPointByVoltage(i, 1.37f, 24.5646673361f);
	i = addMafPointByVoltage(i, 1.66f, 41.453048941f);
	i = addMafPointByVoltage(i, 1.91f, 61.5847903829f);
	i = addMafPointByVoltage(i, 2.09f, 79.7924502089f);
	i = addMafPointByVoltage(i, 2.34f, 110.961012317f);
	i = addMafPointByVoltage(i, 2.89f, 208.198652496f);
	i = addMafPointByVoltage(i, 3.11f, 260.1030585044f);
	i = addMafPointByVoltage(i, 3.54f, 387.150427974f);
	i = addMafPointByVoltage(i, 3.81f, 486.5363959026f);
	i = addMafPointByVoltage(i, 4, 566.628f);
	i = addMafPointByVoltage(i, 4.32f, 722.3485684449f);
	i = addMafPointByVoltage(i, 4.65f, 913.0847954331f);
	i = addMafPointByVoltage(i, 4.98f, 1137.8746972553f);
	fillTheRest(i);
}
