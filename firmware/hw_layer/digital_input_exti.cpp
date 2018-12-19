/*
 * digital_input_exti.cpp
 *
 *  Created on: Dec 18, 2018
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "digital_input_exti.h"
#include "efiGpio.h"

#if HAL_USE_EXT || defined(__DOXYGEN__)

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

static uint32_t getExtMode(ioportid_t port) {
	if (port == GPIOA) {
		return EXT_MODE_GPIOA;
	} else if (port == GPIOB) {
		return EXT_MODE_GPIOB;
	} else if (port == GPIOC) {
		return EXT_MODE_GPIOC;
	} else if (port == GPIOD) {
		return EXT_MODE_GPIOD;
	} else if (port == GPIOE) {
		return EXT_MODE_GPIOE;
	} else if (port == GPIOF) {
		return EXT_MODE_GPIOF;
	}
	firmwareError(CUSTOM_ERR_EXT_MODE, "Unsupported %d", port);
	return 0;
}

// EXT is not able to give you the front direction but you could read the pin in the callback.
void enableExti(brain_pin_e pin, uint32_t mode, extcallback_t cb) {
	if (pin == GPIO_UNASSIGNED)
		return;

	int index = getHwPin("joy", pin);
	ioportid_t port = getHwPort("joy", pin);

	extcfg.channels[index].mode = mode | EXT_CH_MODE_AUTOSTART | getExtMode(port);
	extcfg.channels[index].cb = cb;
}

void myExtStart(void) {
	extStart(&EXTD1, &extcfg);
}

#endif /* HAL_USE_EXT */
