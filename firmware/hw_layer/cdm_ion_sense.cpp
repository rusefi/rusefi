/*
 * @file cdm_ion_sense.cpp
 *
 * Saab Ion Sensing Module integration
 *
 * See https://github.com/rusefi/rusefi_documentation/tree/master/misc/Saab_Trionic_8_Combustion%20Detection%20Module_on_Mazda_Miata_running_rusEfi
 *
 *  Created on: Dec 31, 2018
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "cdm_ion_sense.h"
#include "engine.h"

CdmState::CdmState() {
	currentRevolution = 0;
	currentValue = 0;
	accumulator = 0;
}

int CdmState::getValue() {
	return currentValue;
}

void CdmState::onNewSignal(int currentRevolution) {
	if (this->currentRevolution == currentRevolution) {
		accumulator++;
	} else {
		this->currentRevolution = currentRevolution;
		currentValue = accumulator;
		accumulator = 1;
	}
}

// above logic compiles unconditionally so that unit tests are happy, but without an instance linker would have nothing to link
#if EFI_CDM_INTEGRATION

#include "digital_input_exti.h"

EXTERN_ENGINE;

static CdmState instance;

#if EFI_TUNER_STUDIO
void ionPostState(TunerStudioOutputChannels *tsOutputChannels) {
	tsOutputChannels->debugIntField1 = instance.totalCdmEvents;
}
#endif /* EFI_TUNER_STUDIO */

static void extIonCallback(void *arg) {
        UNUSED(arg);
        instance.totalCdmEvents++;

        int currentRevolution = engine->triggerCentral.triggerState.getTotalRevolutionCounter();
        instance.onNewSignal(currentRevolution);
}

void cdmIonInit(void) {
	// todo: allow 'GPIOA_0' once we migrate to new mandatory configuration
	if (CONFIGB(cdmInputPin) == GPIOA_0 || CONFIGB(cdmInputPin) == GPIO_UNASSIGNED) {
		return;
	}
	int pin = (int)CONFIGB(cdmInputPin);
	if (pin <= 0 || pin > (int)GPIO_UNASSIGNED) {
		// todo: remove this protection once we migrate to new mandatory configuration
		return;
	}

	efiExtiEnablePin("ion", CONFIGB(cdmInputPin), PAL_EVENT_MODE_RISING_EDGE, extIonCallback, NULL);
}

#endif /* EFI_CDM_INTEGRATION */
