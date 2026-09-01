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
#include "injection_close_hw.h"

void portSetHardwareSchedulerTimer(efitick_t nowNt, efitick_t setTimeNt) {
	// This implementation doesn't need the current time, only the target time
	UNUSED(nowNt);

	// The hardware counter is free-running and monotonic: if the compare value is
	// written in the past, the CNT == CCR1 equality has already been missed and no
	// further interrupts fire until the 32-bit counter wraps (~18 min at 4 MHz),
	// silently killing the whole event scheduler. The scheduler's setTimeNt can be
	// stale by the time this write lands, so re-check against a fresh counter read
	// and clamp the compare a small margin into the future.
	const uint32_t compareMinDelta = static_cast<uint32_t>(US2NT(4));
	uint32_t compare = static_cast<uint32_t>(setTimeNt);
	const uint32_t cnt = SCHEDULER_TIMER_DEVICE->CNT;
	if (static_cast<int32_t>(compare - cnt) < static_cast<int32_t>(compareMinDelta)) {
		compare = cnt + compareMinDelta;
	}

	pwm_lld_enable_channel(&SCHEDULER_PWM_DEVICE, 0, compare);
	pwmEnableChannelNotificationI(&SCHEDULER_PWM_DEVICE, 0);

	// ROOT-CAUSE fix for the silent scheduler hang (the 2026-08-23 20:17 lockup:
	// ccr1 stuck in the past, sr=0x0, dier=0x2, isr/setHw frozen - a lost compare
	// event, not a too-small re-arm slack).
	//
	// pwm_lld_enable_channel_notification() clears CC1IF (SR = ~CC1IF) right before
	// setting DIER CC1IE, and only when CC1IE was previously off - which is exactly
	// the re-arm path we are on, because hwTimerCallback() disables CC1IE before
	// invoking the callback. If the free-running counter crosses `compare` in the
	// window between the CCR write above and that SR-clear, the just-set CC1IF is
	// erased: the channel, now with CCR in the past, can never match again, the ISR
	// never fires and the whole event scheduler sleeps until the counter wraps or
	// the MCU resets. The 4 us clamp only narrows that window; over a long run under
	// heavy executor+trigger/fast-IRQ load the window occasionally exceeds it (e.g.
	// nested re-arms from schedule() inside executeAllPendingActions while CC1IE is
	// still off, or preemption by the level-0 fast tooth IRQ), so the hang is rare
	// but real.
	//
	// Fix the root cause (a lost compare, not the slack): if after arming the
	// counter has already reached/passed `compare`, force the missed event with a
	// software capture/compare generation (EGR CC1G). This guarantees the ISR
	// always fires and re-arms on a future trip, independent of how wide the arm
	// window was. In the normal case (compare still in the future) nothing happens.
	if (static_cast<int32_t>(SCHEDULER_TIMER_DEVICE->CNT - compare) >= 0) {
		SCHEDULER_TIMER_DEVICE->EGR = STM32_TIM_EGR_CC1G;
	}
}

static void hwTimerCallback(PWMDriver*) {
	pwmDisableChannelNotificationI(&SCHEDULER_PWM_DEVICE, 0);
	portMicrosecondTimerCallback();

	assertInterruptPriority(__func__, EFI_IRQ_SCHEDULING_TIMER_PRIORITY);
}

static const PWMConfig timerConfig = {
	.frequency = SCHEDULER_TIMER_FREQ,
	/* wanted timer period = 2^32 counts,
	 * but driver set (period - 1) value to register
	 * also period is uint32_t
	 * So set it to zero so it will overlap to 0xffffffff when writen to register */
	.period = 0,
	.callback = nullptr,		// No update callback
	.channels = {
		{PWM_OUTPUT_DISABLED, hwTimerCallback},		   // CC1: NT executor
		{PWM_OUTPUT_DISABLED, hwInjectionCloseCallback},  // CC2: injection close mini-queue
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
	// CC1: executor (OC1M=1 = set-on-match, software comparator, CCxE=0 no pin output)
	// CC2: injection close mini-queue (OC2M=1, same mode)
	SCHEDULER_TIMER_DEVICE->CCMR1 = STM32_TIM_CCMR1_OC1M(1)
	                               | STM32_TIM_CCMR1_OC2M(1);

	/* TODO: implement for all possible TIMs */
	if (SCHEDULER_TIMER_DEVICE == TIM5) {
		/* stop timers clock when core is halted */
	#if defined(STM32F4XX) || defined (STM32F7XX)
		DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_TIM5_STOP;
	#endif
	#if defined(STM32H7XX)
		DBGMCU->APB1LFZ1 |= DBGMCU_APB1LFZ1_DBG_TIM5;
	#endif
	}
}

uint32_t getTimeNowLowerNt() {
	// Using the same timer for measurement and scheduling improves
	// precision and reduces jitter.
	return SCHEDULER_TIMER_DEVICE->CNT;
}

#endif // EFI_PROD_CODE
