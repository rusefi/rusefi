/*
 * cdm_ion_sense.h
 *
 *  Created on: Dec 31, 2018
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#ifndef HW_LAYER_CDM_ION_SENSE_H_
#define HW_LAYER_CDM_ION_SENSE_H_

#include "global.h"

class CdmState {
public:
	CdmState();
	int currentRevolution;
	/**
	 * accumulated value for engine cycle which is not over yet
	 */
	int accumulator;
	/**
	 * event counter for previous complete engine cycle
	 */
	int currentValue;
	void onNewSignal(int currentRevolution);
	int getValue();
};

#if EFI_TUNER_STUDIO
#include "tunerstudio_configuration.h"
void ionPostState(TunerStudioOutputChannels *tsOutputChannels);
#endif

void cdmIonInit(void);

#endif /* HW_LAYER_CDM_ION_SENSE_H_ */
