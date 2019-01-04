/*
 * cdm_ion_sense.h
 *
 *  Created on: Dec 31, 2018
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#ifndef HW_LAYER_CDM_ION_SENSE_H_
#define HW_LAYER_CDM_ION_SENSE_H_

#include "global.h"

#if EFI_TUNER_STUDIO
#include "tunerstudio_configuration.h"
void ionPostState(TunerStudioOutputChannels *tsOutputChannels);
#endif

#endif /* HW_LAYER_CDM_ION_SENSE_H_ */
