/**
 * @file stm32f7xx_rtc.cpp
 * @brief Real Time Clock STM32F7xx switched from LSE to LSI
 *
 * @date Jan 8, 2025
 * @author Andrey Gusakov
 */

#include "pch.h"

#if HAL_USE_RTC

/* switch to LSE clock if ECU previously falledback to LSI clock.
 * This will preserve current time and set it back to RTC after reinit
 * On STM32 change of RTC source clock can be done only through reset of whole backup domain
 * This reset does no affect backup ram */
void hal_lld_rtc_fixup(void)
{
#if (STM32_RTCSEL == STM32_RTCSEL_LSE)
	// we need some more time than defined in RUSEFI_STM32_LSE_WAIT_MAX
	// this is safe as we check that LSE is runnig before reseting BKP domain (stopping LSE)
	// After that we are starting LSE again and waiting for LSERDY.
	int timeout = 1000000000;
	if ((RCC->BDCR & STM32_RTCSEL_MASK) == STM32_RTCSEL) {
		// Backup domain is already driven by expected clock
		return;
	}
	if ((RCC->BDCR & RCC_BDCR_LSERDY) == 0) {
		// LSE is failed to start
		efiPrintf("LSE in not ready");
		return;
	}

	efiPrintf("Switching RTC to LSE clock");

	// Get current time
	RTCDateTime timespec;
	rtcGetTime(&RTCD1, &timespec);

	// Reset BKP domain
	// The BKPSRAM is not affected by this reset
	// This will also reset LSEON
	RCC->BDCR |= RCC_BDCR_BDRST;
	RCC->BDCR &= ~RCC_BDCR_BDRST;

#if defined(STM32_LSE_BYPASS)
	// LSE Bypass.
	RCC->BDCR |= RCC_BDCR_LSEON | RCC_BDCR_LSEBYP;
#else
	// No LSE Bypass.
	RCC->BDCR |= RCC_BDCR_LSEON;
#endif
	// Waits until LSE is stable or times out.
	while (((RCC->BDCR & RCC_BDCR_LSERDY) == 0) && (timeout--)) {
		//this is executed when RTOS is not ready
		//chThdSleepMilliseconds(1);
	}

	// Lets check again
	if (RCC->BDCR & RCC_BDCR_LSERDY) {
		RCC->BDCR |= STM32_RTCSEL;
	} else {
		// LSE is failed to start
		efiPrintf("LSE in not ready after restart attemp");
		// Keep initing
		RCC->BDCR |= RUSEFI_STM32_LSE_WAIT_MAX_RTCSEL;
	}

  /* RTC clock enabled.*/
  RCC->BDCR |= RCC_BDCR_RTCEN;

	// init RTC again
	rtcInit();
	// Set previously saved time
	rtcSetTime(&RTCD1, &timespec);
#endif
}

#endif //HAL_USE_RTC
