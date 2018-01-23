/*
 * @file	digital_input_hw.cpp
 * @brief	Helper methods related to Input Capture Unit (ICU)
 *
 * @date Jun 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
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
	firmwareError(CUSTOM_ERR_ICU, "reader not found");
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
		firmwareError(CUSTOM_ERR_ICU_AF, "getAlternateFunctions(NULL)");
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
	firmwareError(CUSTOM_ERR_ICU_DRIVER, "No such driver");
	return 0xffffffff;
}

icuchannel_t getInputCaptureChannel(brain_pin_e hwPin) {
	switch (hwPin) {
	case GPIOA_2: // TIM9
	case GPIOA_5: // TIM2 stm32f4discovery/Frankenso default
	case GPIOA_6: // TIM3
	case GPIOA_8: // TIM1
	case GPIOA_15: // TIM2
	case GPIOC_6: // TIM3 or TIM8 stm32f4discovery/Frankenso default
	case GPIOE_5: // TIM9
	case GPIOE_9: // TIM1
		return ICU_CHANNEL_1;

	case GPIOA_1: // TIM2
	case GPIOA_3: // TIM9
	case GPIOA_7: // TIM3
	case GPIOA_9: // TIM1
	case GPIOB_3: // TIM2
	case GPIOB_5: // TIM2
	case GPIOC_7: // TIM3 or TIM8
	case GPIOE_6: // TIM9
	case GPIOE_11: // TIM1
		return ICU_CHANNEL_2;
	default:
		firmwareError(CUSTOM_ERR_ICU_PIN, "Unexpected hw pin in getInputCaptureChannel %s", hwPortname(hwPin));
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
	    hwPin == GPIOA_9 ||
		hwPin == GPIOE_9 ||
		hwPin == GPIOE_11) {
		return &ICUD1;
	}
#endif
#if STM32_ICU_USE_TIM2
	if (hwPin == GPIOA_1 ||
		hwPin == GPIOA_5 ||
		hwPin == GPIOA_15 ||
		hwPin == GPIOB_3) {
		return &ICUD2;
	}
#endif
#if STM32_ICU_USE_TIM3
	if (hwPin == GPIOA_6 ||
		hwPin == GPIOA_7 ||
		hwPin == GPIOB_4 ||
		hwPin == GPIOB_5 ||
		hwPin == GPIOC_6 ||
		hwPin == GPIOC_7) {
		return &ICUD3;
	}
#endif
#if STM32_ICU_USE_TIM8
	if (hwPin == GPIOC_6 ||
		hwPin == GPIOC_7) {
		return &ICUD8;
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
	ICUDriver *driver = getInputCaptureDriver(msg, brainPin);
	if (driver != NULL) {
		iomode_t mode = (iomode_t) PAL_MODE_ALTERNATE(getAlternateFunctions(driver));
		efiSetPadMode(msg, brainPin, mode);
	}
}

/**
 * takes next digital_input_s from the registeredIcus pool
 */
digital_input_s * addWaveAnalyzerDriver(const char *msg, brain_pin_e brainPin) {
	ICUDriver *driver = getInputCaptureDriver(msg, brainPin);

	digital_input_s *hw = registeredIcus.add();
	hw->widthListeners.clear();
	hw->periodListeners.clear();
	hw->started = false;
	hw->brainPin = brainPin;
	hw->driver = driver;
	turnOnCapturePin(msg, brainPin);
	return hw;
}

/**
 * turns pin off and returns digital_input_s back into registeredIcus pool
 */
void removeWaveAnalyzerDriver(const char *msg, brain_pin_e brainPin) {
	if (brainPin == GPIO_UNASSIGNED) {
		return;
	}
	unmarkPin(brainPin);

	ICUDriver *driver = getInputCaptureDriver(msg, brainPin);
	if (driver == NULL) {
		return;
	}
	int regSize = registeredIcus.size;
	for (int i = 0; i < regSize; i++) {
		if (registeredIcus.elements[i].driver == driver) {
			// removing from driver from the list of used drivers
			memcpy(&registeredIcus.elements[i], &registeredIcus.elements[regSize - 1],
				sizeof(digital_input_s));
			registeredIcus.size--;
			icuDisableNotificationsI(driver);
			icuStopCapture(driver);
			icuStop(driver);
			return;
		}
	}
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
			icuStopCapture(driver);
			icuStop(driver);
		}
		wave_icucfg.channel = getInputCaptureChannel(hw->brainPin);
		efiIcuStart(driver, &wave_icucfg);
		efiAssertVoid(driver != NULL, "di: driver is NULL");
		efiAssertVoid(driver->state == ICU_READY, "di: driver not ready");
        icuStartCapture(driver); // this would change state from READY to WAITING
		icuEnableNotifications(driver);
	}
	hw->started = true;
}

#endif
