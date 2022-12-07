#include "pch.h"
#include "port_microsecond_timer.h"

#if EFI_PROD_CODE && HAL_USE_GPT

void portSetHardwareSchedulerTimer(efitick_t nowNt, efitick_t setTimeNt) {
	int32_t deltaTimeUs = NT2US((int32_t)setTimeNt - (int32_t)nowNt);

	// If already set, reset the timer
	if (GPTDEVICE.state == GPT_ONESHOT) {
		gptStopTimerI(&GPTDEVICE);
	}

	if (GPTDEVICE.state != GPT_READY) {
		firmwareError(CUSTOM_HW_TIMER, "HW timer state %d", GPTDEVICE.state);
		return;
	}

	// Start the timer
	gptStartOneShotI(&GPTDEVICE, deltaTimeUs);
}

static void hwTimerCallback(GPTDriver*) {
	portMicrosecondTimerCallback();
}

/*
 * The specific 1MHz frequency is important here since 'setHardwareUsTimer' method takes microsecond parameter
 * For any arbitrary frequency to work we would need an additional layer of conversion.
 */
static constexpr GPTConfig gpt5cfg = { 1000000, /* 1 MHz timer clock.*/
		hwTimerCallback, /* Timer callback.*/
0, 0 };

void portInitMicrosecondTimer() {
	gptStart(&GPTDEVICE, &gpt5cfg);
	efiAssertVoid(CUSTOM_ERR_TIMER_STATE, GPTDEVICE.state == GPT_READY, "hw state");
}

#endif // EFI_PROD_CODE

// This implementation just uses the generic port counter - this usually returns a count of CPU cycles since start
uint32_t getTimeNowLowerNt() {
	return port_rt_get_counter_value();
}
