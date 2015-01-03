/**
 * @file	joystick.cpp
 *
 * @date Jan 2, 2015
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "engine_configuration.h"
#include "joystick.h"
#include "engine.h"
#include "pin_repository.h"

#if HAL_USE_EXT || defined(__DOXYGEN__)

EXTERN_ENGINE;

static int joyTotal = 0;
static int joyCenter;
static int joyA = 0;
static int joyB = 0;
static int joyC = 0;
static int joyD = 0;;

static Logging *sharedLogger;

//     {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA, extcb1},

static void extCallback(EXTDriver *extp, expchannel_t channel) {
	joyTotal++;
	if (channel == getHwPin(boardConfiguration->joystickAPin)) {
		joyA++;
	} else if (channel == 9) {
		joyB++;
	} else if (channel == 10) {
		joyC++;
	}
}

static void joystickInfo(void) {
	scheduleMsg(sharedLogger, "total %d center=%d@%s", joyTotal,
			joyCenter, hwPortname(boardConfiguration->joystickCenterPin));
	scheduleMsg(sharedLogger, "a=%d@%s", joyA, hwPortname(boardConfiguration->joystickAPin));
	scheduleMsg(sharedLogger, "b=%d@%s", joyB, hwPortname(boardConfiguration->joystickBPin));
	scheduleMsg(sharedLogger, "c=%d@%s", joyC, hwPortname(boardConfiguration->joystickCPin));
	scheduleMsg(sharedLogger, "d=%d@%s", joyD, hwPortname(boardConfiguration->joystickDPin));
}

/**
 * EXTI is a funny thing: you can only use same pin on one port. For example, you can use
 * PA0 PB5 PE2 PD7
 * but you cannot use
 * PA0 PB0 PE2 PD7
 * because pin '0' would be used on two different ports
 */

static EXTConfig extcfg = { {
/* CH#00 */{ EXT_CH_MODE_DISABLED, NULL },
/* CH#01 */{ EXT_CH_MODE_DISABLED, NULL },
/* CH#02 */{ EXT_CH_MODE_DISABLED, NULL },
/* CH#03 */{ EXT_CH_MODE_DISABLED, NULL },
/* CH#04 */{ EXT_CH_MODE_DISABLED, NULL },
/* CH#05 */{ EXT_CH_MODE_DISABLED, NULL },
/* CH#06 */{ EXT_CH_MODE_DISABLED, NULL },
/* CH#07 */{ EXT_CH_MODE_DISABLED, NULL },
/* CH#08 */{ EXT_CH_MODE_DISABLED, NULL },
/* CH#09 */{ EXT_CH_MODE_DISABLED, NULL },
/* CH#10 */{ EXT_CH_MODE_DISABLED, NULL },
/* CH#11 */{ EXT_CH_MODE_DISABLED, NULL },
/* CH#12 */{ EXT_CH_MODE_DISABLED, NULL },
/* CH#13 */{ EXT_CH_MODE_DISABLED, NULL },
/* CH#14 */{ EXT_CH_MODE_DISABLED, NULL },
/* CH#15 */{ EXT_CH_MODE_DISABLED, NULL },
/* CH#16 */{ EXT_CH_MODE_DISABLED, NULL },
/* CH#17 */{ EXT_CH_MODE_DISABLED, NULL },
/* CH#18 */{ EXT_CH_MODE_DISABLED, NULL },
/* CH#19 */{ EXT_CH_MODE_DISABLED, NULL },
/* CH#20 */{ EXT_CH_MODE_DISABLED, NULL },
/* CH#21 */{ EXT_CH_MODE_DISABLED, NULL },
/* CH#22 */{ EXT_CH_MODE_DISABLED, NULL } } };

void initJoystick(Logging *shared) {
	if (!engineConfiguration->isJoystickEnabled)
		return;
	sharedLogger = shared;

	extcfg.channels[8].mode = EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART
			| EXT_MODE_GPIOC; // PC8
	extcfg.channels[8].cb = extCallback;

	extcfg.channels[10].mode = EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART
			| EXT_MODE_GPIOD; // PD10
	extcfg.channels[10].cb = extCallback;

	extcfg.channels[11].mode = EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART
			| EXT_MODE_GPIOD; // PD11
	extcfg.channels[11].cb = extCallback;

	mySetPadMode2("joy center", boardConfiguration->joystickCenterPin, PAL_MODE_INPUT_PULLUP);
	mySetPadMode("joy B", GPIOC, 8, PAL_MODE_INPUT_PULLUP);
	mySetPadMode("joy D", GPIOD, 11, PAL_MODE_INPUT_PULLUP);

	addConsoleAction("joystickinfo", joystickInfo);

	extStart(&EXTD1, &extcfg);

}

#endif
