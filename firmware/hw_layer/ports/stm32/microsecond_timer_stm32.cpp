/**
 * @file	microsecond_timer_stm32.cpp
 *
 * A single upcounting timer (currently TIM5) is used as a single timebase both for time
 * measurement and event scheduling.  This helps reduce jitter by not making another time
 * measurement at the time of scheduling.
 * 
 * This implementation only works on stm32 because it sets hardware registers directly.
 * ChibiOS doesn't support using timers in output compare mode, only PMW, so we have to 
 * manually configure the timer in outupt compare mode.
 * 
 * @date Dec 1, 2020
 * @author Matthew Kennedy, (c) 2012-2020
 */

#include "global.h"

#if EFI_PROD_CODE && HAL_USE_PWM

#include "port_microsecond_timer.h"

void portSetHardwareSchedulerTimer(efitick_t nowNt, efitick_t setTimeNt) {
	// This implementation doesn't need the current time, only the target time
	UNUSED(nowNt);

	pwm_lld_enable_channel(&SCHEDULER_PWM_DEVICE, 0, setTimeNt);
	pwmEnableChannelNotificationI(&SCHEDULER_PWM_DEVICE, 0);
}

static void hwTimerCallback(PWMDriver*) {
	pwmDisableChannelNotificationI(&SCHEDULER_PWM_DEVICE, 0);
	portMicrosecondTimerCallback();
}

static constexpr PWMConfig timerConfig = {
	.frequency = SCHEDULER_TIMER_FREQ,
	/* wanted timer period = 2^32 counts,
	 * but driver set (period - 1) value to register
	 * also period is uint32_t
	 * So set it to zero so it will overlap to 0xffffffff when writen to register */
	.period = 0,
	.callback = nullptr,		// No update callback
	.channels = {
		{PWM_OUTPUT_DISABLED, hwTimerCallback},	// Channel 0 = timer callback, others unused
		{PWM_OUTPUT_DISABLED, nullptr},
		{PWM_OUTPUT_DISABLED, nullptr},
		{PWM_OUTPUT_DISABLED, nullptr}
	},
	.cr2 = 0,
	.bdtr = 0,
	.dier = 0
};

void portInitMicrosecondTimer() {
	pwmStart(&SCHEDULER_PWM_DEVICE, &timerConfig);

	// ChibiOS doesn't let you configure timers in output compare mode, only PWM mode.
	// We want to be able to set the compare register without waiting for an update event
	// (which would take 358 seconds at 12mhz timer speed), so we have to use normal upcounting
	// output compare mode instead.
	SCHEDULER_TIMER_DEVICE->CCMR1 = STM32_TIM_CCMR1_OC1M(1);

	/* TODO: implement for all possible TIMs */
	if (SCHEDULER_TIMER_DEVICE == TIM5) {
		/* stop timers clock when core is halted */
	#if defined(STM32F4XX) || defined (STM32F7XX)
		DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_TIM5_STOP;
	#endif
		/* TODO: stm32h7? */
	}
}

uint32_t getTimeNowLowerNt() {
	// Using the same timer for measurement and scheduling improves
	// precision and reduces jitter.
	return SCHEDULER_TIMER_DEVICE->CNT;
}

#endif // EFI_PROD_CODE
