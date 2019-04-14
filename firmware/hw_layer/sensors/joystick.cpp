/**
 * @file	joystick.cpp
 *
 * See lcd_controller.cpp for more information
 * See void onJoystick(joystick_button_e button)
 *
 * TODO: separate EXTI layer from joystick logic
 * You cannot use two pins with same index for EXTI (for instance PA5 and PE5) since these would
 * be using same EXTI line. See https://stm32f4-discovery.net/2014/08/stm32f4-external-interrupts-tutorial/
 * See also comments in digital_input_hw.cpp
 *
 *
 * TODO: In release 18.2.0 the EXT driver is no more provided for STM32, there is a PAL demo under /testhal/STM32/multi/PAL with examples of callbacks and synchronous API.
 *
 * @date Jan 2, 2015
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "engine.h"

#if (HAL_USE_PAL && EFI_JOYSTICK)
#include "joystick.h"
#include "pin_repository.h"
#include "digital_input_exti.h"

EXTERN_ENGINE
;

static int joyTotal = 0;
static int joyCenter;
static int joyA = 0;
static int joyB = 0;
static int joyC = 0;
static int joyD = 0;

// 50ms
#define NT_EVENT_GAP US2NT(50 *1000)

static Logging *sharedLogger;
static efitick_t lastEventTime = 0;

static bool isJitter() {
	efitick_t now = getTimeNowNt();
	if (now - lastEventTime < NT_EVENT_GAP)
		return true; // two consecutive events are probably just jitter
	lastEventTime = now;
	return false;
}

static void extCallback(ioportmask_t channel) {
    if (isJitter())
    	 return;
	joyTotal++;
	joystick_button_e button;
	// todo: I guess it's time to reduce code duplication and start working with an array
	if (channel == getHwPin("joy", CONFIGB(joystickCenterPin))) {
		joyCenter++;
		button = JB_CENTER;
	} else if (channel == getHwPin("joy", CONFIGB(joystickAPin))) {
		joyA++;
		button = JB_BUTTON_A;
/* not used so far
	} else if (channel == getHwPin("joy", CONFIGB(joystickBPin))) {
		joyB++;
		button = JB_BUTTON_B;
	} else if (channel == getHwPin("joy", CONFIGB(joystickCPin))) {
		joyC++;
		button = JB_BUTTON_C;
*/
	} else if (channel == getHwPin("joy", CONFIGB(joystickDPin))) {
		joyD++;
		button = JB_BUTTON_D;
	} else {
		// unexpected channel
		return;
	}
#if EFI_HD44780_LCD
	onJoystick(button);
#else
	UNUSED(button);
#endif
}

static void joystickInfo(void) {
	scheduleMsg(sharedLogger, "total %d center=%d@%s", joyTotal, joyCenter,
			hwPortname(CONFIGB(joystickCenterPin)));
	scheduleMsg(sharedLogger, "a=%d@%s", joyA, hwPortname(CONFIGB(joystickAPin)));
	scheduleMsg(sharedLogger, "b=%d@%s", joyB, hwPortname(CONFIGB(joystickBPin)));
	scheduleMsg(sharedLogger, "c=%d@%s", joyC, hwPortname(CONFIGB(joystickCPin)));
	scheduleMsg(sharedLogger, "d=%d@%s", joyD, hwPortname(CONFIGB(joystickDPin)));
}

static bool isJoystickEnabled() {
	return CONFIGB(joystickCenterPin) != GPIO_UNASSIGNED ||
			CONFIGB(joystickAPin) != GPIO_UNASSIGNED ||
			// not used so far			CONFIGB(joystickBPin) != GPIO_UNASSIGNED ||
			// not used so far	CONFIGB(joystickCPin) != GPIO_UNASSIGNED ||
			CONFIGB(joystickDPin) != GPIO_UNASSIGNED;
}

void initJoystick(Logging *shared) {
	addConsoleAction("joystickinfo", joystickInfo);
	if (!isJoystickEnabled())
		return;
	sharedLogger = shared;

	enableExti(CONFIGB(joystickCenterPin), PAL_EVENT_MODE_RISING_EDGE, (palcallback_t)(void *)extCallback);
	enableExti(CONFIGB(joystickAPin), PAL_EVENT_MODE_RISING_EDGE, (palcallback_t)(void *)extCallback);
// not used so far	applyPin(CONFIGB(joystickBPin));
// not used so far	applyPin(CONFIGB(joystickCPin));
	enableExti(CONFIGB(joystickDPin), PAL_EVENT_MODE_RISING_EDGE, (palcallback_t)(void *)extCallback);

	efiSetPadMode("joy center", CONFIGB(joystickCenterPin), PAL_MODE_INPUT_PULLUP);
	efiSetPadMode("joy A", CONFIGB(joystickAPin), PAL_MODE_INPUT_PULLUP);
	// not used so far	efiSetPadMode("joy B", CONFIGB(joystickBPin), PAL_MODE_INPUT_PULLUP);
	// not used so far	efiSetPadMode("joy C", CONFIGB(joystickCPin), PAL_MODE_INPUT_PULLUP);
	efiSetPadMode("joy D", CONFIGB(joystickDPin), PAL_MODE_INPUT_PULLUP);
}

#endif /* HAL_USE_PAL && EFI_JOYSTICK */
