/*
 * @file tachometer.cpp
 * @brief This is about driving external analog tachometers
 *
 * This implementation produces one pulse per engine cycle
 *
 * todo: these is a bit of duplication with dizzySparkOutputPin
 *
 * @date Aug 18, 2015
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "tachometer.h"
#include "trigger_central.h"

EXTERN_ENGINE;

static NamedOutputPin tachOut(TACH_NAME);
static scheduling_s tachTurnSignalOff;

static void turnTachPinLow(void) {
	turnPinLow(&tachOut);
}

static void tachSignalCallback(trigger_event_e ckpSignalType,
		uint32_t index DECLARE_ENGINE_PARAMETER_S) {
	if (index != engineConfiguration->tachPulseTriggerIndex) {
		return;
	}
	turnPinHigh(&tachOut);
	scheduleTask("tach off", &tachTurnSignalOff, (int)MS2US(engineConfiguration->tachPulseDuractionMs), (schfunc_t) &turnTachPinLow, NULL);
}

void initTachometer(void) {
	if (boardConfiguration->tachOutputPin == GPIO_UNASSIGNED) {
		return;
	}

	outputPinRegisterExt2("analog tach output", &tachOut, boardConfiguration->tachOutputPin, &boardConfiguration->tachOutputPinMode);

	addTriggerEventListener(tachSignalCallback, "tach", engine);


}
