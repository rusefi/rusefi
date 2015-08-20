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
static scheduling_s tachTurnSignalOff;

static void turnTachPinLow(void) {
	tachOut.setValue(false);
}

static void tachSignalCallback(trigger_event_e ckpSignalType,
		uint32_t index DECLARE_ENGINE_PARAMETER_S) {
	if (index != engineConfiguration->tachPulseTriggerIndex) {
		return;
	}
	tachOut.setValue(true);
	scheduleTask("tach off", &tachTurnSignalOff, (int)MS2US(engineConfiguration->tachPulseDuractionMs), (schfunc_t) &turnTachPinLow, NULL);
}

void initTachometer(void) {
	if (boardConfiguration->tachOutputPin == GPIO_UNASSIGNED) {
		return;
	}

	outputPinRegisterExt2("analog tach output", &tachOut, boardConfiguration->tachOutputPin, &boardConfiguration->tachOutputPinMode);

	addTriggerEventListener(tachSignalCallback, "tach", engine);


}
