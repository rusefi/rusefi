/**
 * @file stm32f4xx_rtc.cpp
 * @brief Restore the STM32F4 RTC to LSE after an earlier fallback to LSI.
 */

#include "pch.h"

#if HAL_USE_RTC

// Changing the RTC clock source requires a backup-domain reset. Preserve the
// current time and the RTC backup registers used by backup_ram.cpp across that
// reset. The separate BKPSRAM is not affected.
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
	const uint32_t backup0 = RTC->BKP0R;
	const uint32_t backup1 = RTC->BKP1R;
	const uint32_t backup2 = RTC->BKP2R;
	const uint32_t backup3 = RTC->BKP3R;

	// This also stops LSE, so it must be restarted before selecting the clock.
	RCC->BDCR |= RCC_BDCR_BDRST;
	RCC->BDCR &= ~RCC_BDCR_BDRST;

#if defined(STM32_LSE_BYPASS)
	RCC->BDCR |= RCC_BDCR_LSEON | RCC_BDCR_LSEBYP;
#else
	RCC->BDCR |= RCC_BDCR_LSEON;
#endif

	// initRtc() runs with the RTOS and watchdog active, before periodic watchdog
	// servicing and USB startup. Give LSE at most one second to restart, then
	// fall back to LSI so a bad crystal cannot prevent the ECU from booting.
	const systime_t start = chVTGetSystemTimeX();
	while (((RCC->BDCR & RCC_BDCR_LSERDY) == 0) && (chTimeDiffX(start, chVTGetSystemTimeX()) < TIME_MS2I(1000))) {
#if HAL_USE_WDG
		wdgReset(&WDGD1);
#endif
		chThdSleepMilliseconds(10);
	}

#if HAL_USE_WDG
	// Leave the remaining startup code a fresh watchdog interval.
	wdgReset(&WDGD1);
#endif

	if (RCC->BDCR & RCC_BDCR_LSERDY) {
		RCC->BDCR |= STM32_RTCSEL;
	} else {
		efiPrintf("LSE is not ready after restart attempt");
		RCC->BDCR |= RUSEFI_STM32_LSE_WAIT_MAX_RTCSEL;
	}

	RCC->BDCR |= RCC_BDCR_RTCEN;
	rtcInit();
	rtcSetTime(&RTCD1, &timespec);
	RTC->BKP0R = backup0;
	RTC->BKP1R = backup1;
	RTC->BKP2R = backup2;
	RTC->BKP3R = backup3;
#endif
}

#endif // HAL_USE_RTC
