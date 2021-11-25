/*
 * @file cdm_ion_sense.h
 *
 * @date Dec 31, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"

class CdmState {
public:
	CdmState();
	int totalCdmEvents = 0;

	int accumilatingAtRevolution = -1;
	/**
	 * accumulated value for engine cycle which is not over yet
	 */
	int accumulatingCurrentValue;
	/**
	 * event counter for previous complete engine cycle
	 */
	int currentValue = 0;
	int currentValueAtIndex = -1;
	void onNewSignal(int currentRevolution);
	void applyAccumulatedData(int currentRevolution);
	int getValue(int currentRevolution);
};

#if EFI_TUNER_STUDIO
void ionPostState(TunerStudioOutputChannels *tsOutputChannels);
#endif

void cdmIonInit(void);
int getCurrentCdmValue(int currentRevolution);

