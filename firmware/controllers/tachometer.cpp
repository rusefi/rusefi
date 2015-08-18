/*
 * @file tachometer.cpp
 * @brief This is about driving external analog tachometers
 *
 * @date Aug 18, 2015
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "tachometer.h"
#include "trigger_central.h"

EXTERN_ENGINE;

static OutputPin tachOut;

static void tachSignalCallback(trigger_event_e ckpSignalType,
		uint32_t index DECLARE_ENGINE_PARAMETER_S) {
}

void initTachometer(void) {
	if (boardConfiguration->tachOutputPin == GPIO_UNASSIGNED)
		return;

	outputPinRegisterExt2("tach", &tachOut, boardConfiguration->tachOutputPin, &boardConfiguration->tachOutputPinMode);

	addTriggerEventListener(tachSignalCallback, "tach", engine);


}
