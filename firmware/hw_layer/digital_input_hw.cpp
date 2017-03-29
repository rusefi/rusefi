/*
 * @file	digital_input_hw.cpp
 * @brief	Helper methods related to Input Capture Unit (ICU)
 *
 * @date Jun 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "digital_input_hw.h"
#include "mpu_util.h"
#include "fl_stack.h"

#if EFI_WAVE_ANALYZER || defined(__DOXYGEN__)

#include "eficonsole.h"
#include "pin_repository.h"

static void icuWidthCallback(ICUDriver *driver);
static void icuPeriordCallBack(ICUDriver *driver);

/*
 * 30ms seems like width maximum, at 16bit precision that means
 * CORE_CLOCK / 33.33333 = TICKS * 65536
 * 168000000 / 33.333333 / 65536 = 76.90
 */
static ICUConfig wave_icucfg = { ICU_INPUT_ACTIVE_LOW, CORE_CLOCK / 100, icuWidthCallback, icuPeriordCallBack, 0,
		ICU_CHANNEL_1, 0 };

static ArrayList<digital_input_s, 8> registeredIcus;

static digital_input_s * finddigital_input_s(ICUDriver *driver) {
	for (int i = 0; i < registeredIcus.size; i++) {
		if (registeredIcus.elements[i].driver == driver) {
			return &registeredIcus.elements[i];
		}
	}
	firmwareError(CUSTOM_ERR_6520, "reader not found");
	return (digital_input_s *) NULL;
}

static void icuWidthCallback(ICUDriver *driver) {
	/*
	 * see comment in icuPeriordCallBack
	 int rowWidth = icuGetWidth(driver);
	 */
	digital_input_s * hw = finddigital_input_s(driver);
	hw->widthListeners.invokeJustArgCallbacks();
}

static void icuPeriordCallBack(ICUDriver *driver) {
	/*
	 * 	we do not use timer period at all - we just need the event. For all time characteristics,
	 * 	we use system time
	 * 	int period = icuGetPeriod(driver);
	 */

	digital_input_s * hw = finddigital_input_s(driver);
	hw->periodListeners.invokeJustArgCallbacks();
}

static uint32_t getAlternateFunctions(ICUDriver *driver) {
	if (driver == NULL) {
		firmwareError(CUSTOM_ERR_6521, "getAlternateFunctions(NULL)");
		return 0xffffffff;
	}
#if STM32_ICU_USE_TIM1
	if (driver == &ICUD1) {
		return GPIO_AF_TIM1;
	}
#endif
#if STM32_ICU_USE_TIM2
	if (driver == &ICUD2) {
		return GPIO_AF_TIM2;
	}
#endif
#if STM32_ICU_USE_TIM3
	if (driver == &ICUD3) {
		return GPIO_AF_TIM3;
	}
#endif
#if STM32_ICU_USE_TIM4
	if (driver == &ICUD4) {
		return GPIO_AF_TIM4;
	}
#endif
#if STM32_ICU_USE_TIM9
	if (driver == &ICUD9) {
		return GPIO_AF_TIM9;
	}
#endif
	firmwareError(CUSTOM_ERR_6522, "No such driver");
	return 0xffffffff;
}

icuchannel_t getInputCaptureChannel(brain_pin_e hwPin) {
	switch (hwPin) {
	case GPIOA_2:
	case GPIOA_5:
	case GPIOA_6:
	case GPIOA_8:
	case GPIOC_6:
	case GPIOE_5:
	case GPIOE_9:
		return ICU_CHANNEL_1;

	case GPIOA_3:
	case GPIOA_7:
	case GPIOE_6:
	case GPIOE_11:
		return ICU_CHANNEL_2;
	default:
		firmwareError(CUSTOM_ERR_6523, "Unexpected hw pin in getInputCaptureChannel %s", hwPortname(hwPin));
		return ICU_CHANNEL_1;
	}
}

/**
 * as of Feb 2016, TIM1, TIM2, TIM3 and TIM9 are used for input capture
 * (that's the kind of event you need for shaft position sensor)
 * ChibiOS limitation is that only channels #1 and #2 could be used for input capture
 *
 * TODO: migrate slow ADC to software timer so that TIM8 is also available for input capture
 */
ICUDriver * getInputCaptureDriver(const char *msg, brain_pin_e hwPin) {
	if (hwPin == GPIO_UNASSIGNED || hwPin == GPIO_INVALID) {
		return NULL;
	}
#if STM32_ICU_USE_TIM1
	if (hwPin == GPIOA_8 ||
		hwPin == GPIOE_9 ||
		hwPin == GPIOE_11) {
		return &ICUD1;
	}
#endif
#if STM32_ICU_USE_TIM2
	if (hwPin == GPIOA_1 ||
		hwPin == GPIOA_5 ||
		hwPin == GPIOB_3) {
		return &ICUD2;
	}
#endif
#if STM32_ICU_USE_TIM3
	if (hwPin == GPIOA_6 ||
		hwPin == GPIOA_7 ||
		hwPin == GPIOC_6 ||
		hwPin == GPIOC_7) {
		return &ICUD3;
	}
#endif
#if STM32_ICU_USE_TIM8
	if (hwPin == GPIOC_6 ||
		hwPin == GPIOC_7) {
		return &ICUD9;
	}
#endif
#if STM32_ICU_USE_TIM9
	if (hwPin == GPIOA_2 ||
		hwPin == GPIOA_3 ||
		hwPin == GPIOE_5 ||
		hwPin == GPIOE_6) {
		return &ICUD9;
	}
#endif
	firmwareError(CUSTOM_ERR_NOT_INPUT_PIN, "%s: Not input pin %s", msg, hwPortname(hwPin));
	return (ICUDriver *) NULL;
}

void turnOnCapturePin(const char *msg, brain_pin_e brainPin) {
	ioportid_t port = getHwPort(brainPin);
	ioportmask_t pin = getHwPin(brainPin);

	ICUDriver *driver = getInputCaptureDriver(msg, brainPin);
	if (driver != NULL) {
		iomode_t mode = (iomode_t) PAL_MODE_ALTERNATE(getAlternateFunctions(driver));
		mySetPadMode(msg, port, pin, mode);
	}
}

digital_input_s * initWaveAnalyzerDriver(const char *msg, brain_pin_e brainPin) {
	ICUDriver *driver = getInputCaptureDriver(msg, brainPin);

	digital_input_s *hw = registeredIcus.add();
	hw->brainPin = brainPin;
	hw->driver = driver;
	turnOnCapturePin(msg, brainPin);
	return hw;
}

void startInputDriver(digital_input_s *hw, bool isActiveHigh) {
	hw->isActiveHigh = isActiveHigh;
	if (hw->isActiveHigh) {
		wave_icucfg.mode = ICU_INPUT_ACTIVE_HIGH;
	} else {
		wave_icucfg.mode = ICU_INPUT_ACTIVE_LOW;
	}
	ICUDriver *driver = hw->driver;

	if (driver != NULL) {
		if (hw->started) {
			icuDisableNotificationsI(driver);
			icuStop(driver);
		}
		wave_icucfg.channel = getInputCaptureChannel(hw->brainPin);
		efiIcuStart(driver, &wave_icucfg);
		icuEnableNotifications(driver);
	}
	hw->started = true;
}

#endif
