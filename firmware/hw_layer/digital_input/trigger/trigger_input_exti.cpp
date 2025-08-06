/**
 * @file	trigger_input_exti.cpp
 * @brief	Position sensor hardware layer - PAL version
 *
 * todo: VVT implementation is a nasty copy-paste :(
 *
 * see digital_input_icu.cp
 *
 * @date Dec 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"

#if EFI_SHAFT_POSITION_INPUT && (HAL_TRIGGER_USE_PAL == TRUE)

#include "trigger_input.h"
#include "digital_input_exti.h"

#if (PAL_USE_CALLBACKS == FALSE)
	#error "PAL_USE_CALLBACKS should be enabled to use HAL_TRIGGER_USE_PAL"
#endif

static ioline_t shaftLines[TRIGGER_INPUT_PIN_COUNT];
static ioline_t camLines[CAM_INPUTS_COUNT];

static void shaft_callback(void *arg, efitick_t stamp) {
	// do the time sensitive things as early as possible!
	int index = (int)arg;
	ioline_t pal_line = shaftLines[index];
	bool rise = (palReadLine(pal_line) == PAL_HIGH);

	// todo: support for 3rd trigger input channel
	// todo: start using real event time from HW event, not just software timer?

	hwHandleShaftSignal(index, rise, stamp);
}

static void cam_callback(void *arg, efitick_t stamp) {
	int index = (int)arg;
	ioline_t pal_line = camLines[index];

	bool rise = (palReadLine(pal_line) == PAL_HIGH);

	hwHandleVvtCamSignal(rise, stamp, index);
}

/*==========================================================================*/
/* Exported functions.														*/
/*==========================================================================*/

int extiTriggerTurnOnInputPin(const char *msg, int index, bool isTriggerShaft) {
	brain_pin_e brainPin = isTriggerShaft ? engineConfiguration->triggerInputPins[index] : engineConfiguration->camInputs[index];

	efiPrintf("extiTriggerTurnOnInputPin %s %s", msg, hwPortname(brainPin));

	/* TODO:
	 * * do not set to both edges if we need only one
	 * * simplify callback in case of one edge */
	if (efiExtiEnablePin(msg, brainPin, PAL_EVENT_MODE_BOTH_EDGES,
		isTriggerShaft ? shaft_callback : cam_callback, (void *)index) < 0) {
		return -1;
	}

	ioline_t pal_line = PAL_LINE(getHwPort("trg", brainPin), getHwPin("trg", brainPin));
	if (isTriggerShaft) {
		shaftLines[index] = pal_line;
	} else {
		camLines[index] = pal_line;
	}

	return 0;
}

void extiTriggerTurnOffInputPin(brain_pin_e brainPin) {
	efiExtiDisablePin(brainPin);
}

#endif /* (EFI_SHAFT_POSITION_INPUT && (HAL_TRIGGER_USE_PAL == TRUE)) */
