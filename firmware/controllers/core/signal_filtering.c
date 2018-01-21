/*
 * @file signal_filtering.c
 *
 * @date Aug 5, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#ifndef SIGNAL_FILTERING_C_
#define SIGNAL_FILTERING_C_

#include "signal_filtering.h"

void sfInit(SignalFiltering *fs, float K, float initialValue) {
	fs->pointer = 0;
	fs->K = K;
	fs->Vout = initialValue;
	fs->Vacc = initialValue * K;
}

static void addCopyAndSort(SignalFiltering *fs, float value) {
	fs->values[fs->pointer] = value;
	fs->pointer = ++fs->pointer == FILTER_SIZE ? 0 : fs->pointer;

	for (int i = 0; i < FILTER_SIZE; i++)
		fs->sorted[i] = fs->values[i];

	for (int i = 0; i < FILTER_SIZE; i++)
		for (int j = i + 1; j < FILTER_SIZE; j++)
			if (fs->sorted[i] < fs->sorted[j]) {
				float temp = fs->sorted[i];
				fs->sorted[i] = fs->sorted[j];
				fs->sorted[j] = temp;
			}
}

void sfAddValue(SignalFiltering *fs, float value) {
	addCopyAndSort(fs, value);
	float Vin = fs->sorted[FILTER_SIZE / 2];

	fs->Vacc += Vin - fs->Vout;
	fs->Vout = fs->Vacc / fs->K;
}

void sfAddValue2(SignalFiltering *fs, float value) {
	addCopyAndSort(fs, value);

	int fromIndex = FILTER_SIZE / 4;
	int toIndex = FILTER_SIZE / 4 + FILTER_SIZE / 2;

	/**
	 * this implementation takes the average of the middle hald of the sorted values
	 */
	float result = 0;
	for (int i = fromIndex; i < toIndex; i++)
		result += fs->sorted[i];

	fs->Vout = result / (FILTER_SIZE / 2);
}

float sfGetValue(SignalFiltering *fs) {
	return fs->Vout;
}

#endif /* SIGNAL_FILTERING_C_ */
