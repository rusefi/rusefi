/**
 * @file	joystick.cpp
 *
 * See lcd_controller.cpp for more information
 * See void onJoystick(joystick_button_e button)
 *
 * TODO: separate EXTI layer from joystick logic
 * You cannot use two pins with same index for EXTI (for instance PA5 and PE5) since these would
 * be using same EXTI line. See https://stm32f4-discovery.net/2014/08/stm32f4-external-interrupts-tutorial/
 * See also comments in digital_input_icu.cpp
 *
 * @date Jan 2, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */


#if (HAL_USE_PAL && EFI_JOYSTICK)
#include "joystick.h"
#include "pin_repository.h"
#include "digital_input_exti.h"

static int joyTotal = 0;
static int joyCenter;
static int joyA = 0;
static int joyB = 0;
static int joyC = 0;
static int joyD = 0;

// 50ms
#define NT_EVENT_GAP US2NT(50 *1000)

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
	if (channel == getHwPin("joy", CONFIG(joystickCenterPin))) {
		joyCenter++;
		button = JB_CENTER;
	} else if (channel == getHwPin("joy", CONFIG(joystickAPin))) {
		joyA++;
		button = JB_BUTTON_A;
/* not used so far
	} else if (channel == getHwPin("joy", CONFIG(joystickBPin))) {
		joyB++;
		button = JB_BUTTON_B;
	} else if (channel == getHwPin("joy", CONFIG(joystickCPin))) {
		joyC++;
		button = JB_BUTTON_C;
*/
	} else if (channel == getHwPin("joy", CONFIG(joystickDPin))) {
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
	efiPrintf("total %d center=%d@%s", joyTotal, joyCenter,
			hwPortname(CONFIG(joystickCenterPin)));
	efiPrintf("a=%d@%s", joyA, hwPortname(CONFIG(joystickAPin)));
	efiPrintf("b=%d@%s", joyB, hwPortname(CONFIG(joystickBPin)));
	efiPrintf("c=%d@%s", joyC, hwPortname(CONFIG(joystickCPin)));
	efiPrintf("d=%d@%s", joyD, hwPortname(CONFIG(joystickDPin)));
}

static bool isJoystickEnabled() {
	return (isBrainPinValid(CONFIG(joystickCenterPin)) &&
			isBrainPinValid(CONFIG(joystickAPin)) &&
			// not used so far	isBrainPinValid(CONFIG(joystickBPin)) &&
			// not used so far	isBrainPinValid(CONFIG(joystickCPin)) &&
			isBrainPinValid(CONFIG(joystickDPin)));
}

void stopJoystickPins() {
	// todo: should be 'efiExtiDisablePin' or smth?
	efiSetPadUnused(activeConfiguration.joystickCenterPin);
	efiSetPadUnused(activeConfiguration.joystickAPin);
	efiSetPadUnused(activeConfiguration.joystickDPin);
}

void startJoystickPins() {
	// todo: extract 'configurePalInputPin() method?
	// input capture driver would claim pin ownership so we are not using 'efiSetPadMode' here
	efiSetPadModeWithoutOwnershipAcquisition("joy center", CONFIG(joystickCenterPin), PAL_MODE_INPUT_PULLUP);
	efiSetPadModeWithoutOwnershipAcquisition("joy A", CONFIG(joystickAPin), PAL_MODE_INPUT_PULLUP);
	// not used so far	efiSetPadModeWithoutOwnershipAcquisition("joy B", CONFIG(joystickBPin), PAL_MODE_INPUT_PULLUP);
	// not used so far	efiSetPadModeWithoutOwnershipAcquisition("joy C", CONFIG(joystickCPin), PAL_MODE_INPUT_PULLUP);
	efiSetPadModeWithoutOwnershipAcquisition("joy D", CONFIG(joystickDPin), PAL_MODE_INPUT_PULLUP);
}

void initJoystick() {
	int channel;
	addConsoleAction("joystickinfo", joystickInfo);
	if (!isJoystickEnabled())
		return;

	channel = getHwPin("joy", CONFIG(joystickCenterPin));
	efiExtiEnablePin("joy", CONFIG(joystickCenterPin), PAL_EVENT_MODE_RISING_EDGE, (palcallback_t)(void *)extCallback, (void *)channel);
	channel = getHwPin("joy", CONFIG(joystickAPin));
	efiExtiEnablePin("joy", CONFIG(joystickAPin), PAL_EVENT_MODE_RISING_EDGE, (palcallback_t)(void *)extCallback, (void *)channel);
// not used so far	applyPin(CONFIG(joystickBPin));
// not used so far	applyPin(CONFIG(joystickCPin));
	channel = getHwPin("joy", CONFIG(joystickDPin));
	efiExtiEnablePin("joy", CONFIG(joystickDPin), PAL_EVENT_MODE_RISING_EDGE, (palcallback_t)(void *)extCallback, (void *)channel);
}

#endif /* HAL_USE_PAL && EFI_JOYSTICK */
