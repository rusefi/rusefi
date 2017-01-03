/*
 * @file signal_filtering.h
 *
 * @date Aug 5, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef SIGNAL_FILTERING_H_
#define SIGNAL_FILTERING_H_

#define FILTER_SIZE 5

typedef struct {
	float values[FILTER_SIZE];
	float sorted[FILTER_SIZE];
	int pointer;
	float K, Vacc, Vout;
} SignalFiltering;

void sfInit(SignalFiltering *fs, float K, float initialValue);
void sfAddValue(SignalFiltering *fs, float value);
void sfAddValue2(SignalFiltering *fs, float value);
float sfGetValue(SignalFiltering *fs);

#endif /* SIGNAL_FILTERING_H_ */
