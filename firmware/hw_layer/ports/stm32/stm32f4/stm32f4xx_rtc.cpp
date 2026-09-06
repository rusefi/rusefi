/**
 * @file stm32f4xx_rtc.cpp
 * @brief Restore the STM32F4 RTC to LSE after an earlier fallback to LSI.
 */

#include "pch.h"

#if HAL_USE_RTC

// Changing the RTC clock source requires a backup-domain reset. Preserve the
// current time across that reset; BKPSRAM is not affected.
void hal_lld_rtc_fixup(void) {
#if (STM32_RTCSEL == STM32_RTCSEL_LSE)
	if ((RCC->BDCR & STM32_RTCSEL_MASK) == STM32_RTCSEL) {
		return;
	}
	if ((RCC->BDCR & RCC_BDCR_LSERDY) == 0) {
		efiPrintf("LSE is not ready");
		return;
	}

	efiPrintf("Switching RTC to LSE clock");

	RTCDateTime timespec;
	rtcGetTime(&RTCD1, &timespec);

	// This also stops LSE, so it must be restarted before selecting the clock.
	RCC->BDCR |= RCC_BDCR_BDRST;
	RCC->BDCR &= ~RCC_BDCR_BDRST;

#if defined(STM32_LSE_BYPASS)
	RCC->BDCR |= RCC_BDCR_LSEON | RCC_BDCR_LSEBYP;
#else
	RCC->BDCR |= RCC_BDCR_LSEON;
#endif

	// Allow more startup time than RUSEFI_STM32_LSE_WAIT_MAX, as on F7.
	// LSE was confirmed running before resetting the backup domain.
	int timeout = 1000000000;
	while (((RCC->BDCR & RCC_BDCR_LSERDY) == 0) && (timeout--)) {
	}

	if (RCC->BDCR & RCC_BDCR_LSERDY) {
		RCC->BDCR |= STM32_RTCSEL;
	} else {
		efiPrintf("LSE is not ready after restart attempt");
		RCC->BDCR |= RUSEFI_STM32_LSE_WAIT_MAX_RTCSEL;
	}

	RCC->BDCR |= RCC_BDCR_RTCEN;
	rtcInit();
	rtcSetTime(&RTCD1, &timespec);
#endif
}

#endif // HAL_USE_RTC
