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

static void shaft_callback(void *arg, efitick_t stamp, bool level) {
	// do the time sensitive things as early as possible!
	int index = (int)arg;
	// The pin level was captured in the EXTI fast IRQ (~us after the edge) and
	// travelled here through the queue - never re-read it in the handoff: the
	// handoff runs up to ~1 ms late and would reconstruct the wrong edge
	// direction at running rpm.
	bool rise = level;

	// todo: support for 3rd trigger input channel
	// todo: start using real event time from HW event, not just software timer?

	hwHandleShaftSignal(index, rise, stamp);
}

static void cam_callback(void *arg, efitick_t stamp, bool level) {
	int index = (int)arg;

	hwHandleVvtCamSignal(level, stamp, index);
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

	return 0;
}

void extiTriggerTurnOffInputPin(brain_pin_e brainPin) {
	efiExtiDisablePin(brainPin);
}

#endif /* (EFI_SHAFT_POSITION_INPUT && (HAL_TRIGGER_USE_PAL == TRUE)) */
