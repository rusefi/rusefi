/**
 * @file stm32f4xx_rtc.cpp
 * @brief Switch the RTC from the internal clock (LSI) to the external clock (LSE).
 */

#include "pch.h"

#if HAL_USE_RTC

#if (STM32_RTCSEL == STM32_RTCSEL_LSE)
// Wait up to one second for LSE. The normal watchdog task has not started yet,
// so feed the watchdog here while waiting.
static bool waitForLseReady() {
	const systime_t start = chVTGetSystemTimeX();
	while (((RCC->BDCR & RCC_BDCR_LSERDY) == 0) && (chTimeDiffX(start, chVTGetSystemTimeX()) < TIME_MS2I(1000))) {
#if HAL_USE_WDG
		wdgReset(&WDGD1);
#endif
		chThdSleepMilliseconds(10);
	}

#if HAL_USE_WDG
	// Give the rest of startup a full watchdog timeout.
	wdgReset(&WDGD1);
#endif
	return (RCC->BDCR & RCC_BDCR_LSERDY) != 0;
}
#endif

// Changing clocks needs a reset, which clears the time and backup registers.
// Save and restore them. The separate backup SRAM is not reset.
void hal_lld_rtc_fixup(void) {
#if (STM32_RTCSEL == STM32_RTCSEL_LSE)
	if ((RCC->BDCR & STM32_RTCSEL_MASK) == STM32_RTCSEL) {
		return;
	}
	// LSE can take longer to start. Wait before resetting the RTC.
	if (!waitForLseReady()) {
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

	// Reset the RTC and backup registers. This also stops LSE.
	RCC->BDCR |= RCC_BDCR_BDRST;
	RCC->BDCR &= ~RCC_BDCR_BDRST;

#if defined(STM32_LSE_BYPASS)
	RCC->BDCR |= RCC_BDCR_LSEON | RCC_BDCR_LSEBYP;
#else
	RCC->BDCR |= RCC_BDCR_LSEON;
#endif

	// Wait for LSE to restart, or use LSI if it fails.
	if (waitForLseReady()) {
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
