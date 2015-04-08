/*
 * @file	wave_analyzer_hw.cpp
 * @brief	Helper methods related to Input Capture Unit (ICU)
 *
 * @date Jun 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "wave_analyzer_hw.h"
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

static ArrayList<WaveReaderHw, 8> registeredIcus;

static WaveReaderHw * findWaveReaderHw(ICUDriver *driver) {
	for (int i = 0; i < registeredIcus.size; i++) {
		if (registeredIcus.elements[i].driver == driver) {
			return &registeredIcus.elements[i];
		}
	}
	firmwareError("reader not found");
	return (WaveReaderHw *) NULL;
}

static void icuWidthCallback(ICUDriver *driver) {
	/*
	 * see comment in icuPeriordCallBack
	 int rowWidth = icuGetWidth(driver);
	 */
	WaveReaderHw * hw = findWaveReaderHw(driver);
	hw->widthListeners.invokeJustArgCallbacks();
}

static void icuPeriordCallBack(ICUDriver *driver) {
	/*
	 * 	we do not use timer period at all - we just need the event. For all time characteristics,
	 * 	we use system time
	 * 	int period = icuGetPeriod(driver);
	 */

	WaveReaderHw * hw = findWaveReaderHw(driver);
	hw->periodListeners.invokeJustArgCallbacks();
}

static uint32_t getAlternateFunctions(ICUDriver *driver) {
	if (driver == NULL) {
		firmwareError("getAlternateFunctions(NULL)");
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
	firmwareError("No such driver");
	return 0xffffffff;
}

icuchannel_t getInputCaptureChannel(brain_pin_e hwPin) {
	switch (hwPin) {
	case GPIOA_5:
	case GPIOA_8:
	case GPIOC_6:
	case GPIOE_5:
	case GPIOE_9:
		return ICU_CHANNEL_1;

	case GPIOE_6:
	case GPIOE_11:
		return ICU_CHANNEL_2;
	default:
		firmwareError("Unexpected hw pin in getInputCaptureChannel %d", hwPin);
		return ICU_CHANNEL_1;
	}
}

ICUDriver * getInputCaptureDriver(brain_pin_e hwPin) {
#if STM32_ICU_USE_TIM1
	if (hwPin == GPIOA_8) {
		return &ICUD1;
	}
	if (hwPin == GPIOE_9) {
		return &ICUD1;
	}
	if (hwPin == GPIOE_11) {
		return &ICUD1;
	}
#endif
#if STM32_ICU_USE_TIM2
	if (hwPin == GPIOA_5) {
		return &ICUD2;
	}
#endif
#if STM32_ICU_USE_TIM3
	if (hwPin == GPIOC_6) {
		return &ICUD3;
	}
#endif
#if STM32_ICU_USE_TIM9
	if (hwPin == GPIOE_5) {
		return &ICUD9;
	}
#endif
	return (ICUDriver *) NULL;
}

void turnOnCapturePin(brain_pin_e brainPin) {
	ioportid_t port = getHwPort(brainPin);
	ioportmask_t pin = getHwPin(brainPin);

	ICUDriver *driver = getInputCaptureDriver(brainPin);
	if (driver != NULL) {
		iomode_t mode = (iomode_t) PAL_MODE_ALTERNATE(getAlternateFunctions(driver));
		mySetPadMode("wave input", port, pin, mode);
	}
}

WaveReaderHw * initWaveAnalyzerDriver(brain_pin_e brainPin) {
	ICUDriver *driver = getInputCaptureDriver(brainPin);

	WaveReaderHw *hw = registeredIcus.add();

	hw->driver = driver;
	turnOnCapturePin(brainPin);
	return hw;
}

void startInputDriver(WaveReaderHw *hw, bool isActiveHigh) {
	hw->isActiveHigh = isActiveHigh;
	if (hw->isActiveHigh) {
		wave_icucfg.mode = ICU_INPUT_ACTIVE_HIGH;
	} else {
		wave_icucfg.mode = ICU_INPUT_ACTIVE_LOW;
	}
	ICUDriver *driver = hw->driver;

	if (driver != NULL) {
		if (hw->started) {
			icuDisable(driver);
			icuStop(driver);
		}
		efiIcuStart(driver, &wave_icucfg);
		icuEnable(driver);
	}
	hw->started = true;
}

#endif
