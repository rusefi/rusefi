/**
 * @file angle_clock.cpp
 *
 * TMR2 hardware angle clock: 4 OC channels as pure software comparators.
 * See angle_clock.h for the design rationale.
 *
 * Register-level notes (same TIM peripheral on STM32F4/F7 and AT32F435):
 *  - OCxM = 1 ("set channel to active level on match") with CCxE = 0: no pin
 *    is driven, but CCxIF still sets on the CNT == CCR match. This is the
 *    same trick the TIM5 executor uses (microsecond_timer_stm32.cpp).
 *  - ARPE is left 0: ARR writes take effect immediately. Not load-bearing
 *    here (ARR stays 0xFFFFFFFF, the counter only wraps every ~1073 s).
 *  - SR flags are rc_w0: write ~flag to clear.
 *
 * Concurrency: the handoff ISR (priority 4) arms/cancels channels, the TMR2
 * ISR (priority 3) fires them. The TMR2 ISR can preempt the handoff mid-arm,
 * so the arm sequence is ordered so that no partial state can fire:
 *   1. clear stale CCxIF
 *   2. write CCR (a tick >= arm margin in the future - CNT cannot reach it
 *      within this ISR window)
 *   3. store the action
 *   4. enable CCxIE
 * A preempting TMR2 ISR sees no flag for a channel mid-arm (the flag cannot
 * set until CNT reaches the future CCR), and it never executes an action on
 * a channel whose action is empty. The priority-0 EXTI fast IRQ does not
 * touch the angle clock at all.
 */

#include "pch.h"

#if EFI_ANGLE_CLOCK && EFI_PROD_CODE

#include "angle_clock.h"

#define ANGLE_CLOCK_TIMER TIM2

// CC1..CC4 interrupt flag bits in SR / enable bits in DIER share positions.
static constexpr uint32_t CC_IF_MASK =
	STM32_TIM_SR_CC1IF | STM32_TIM_SR_CC2IF | STM32_TIM_SR_CC3IF | STM32_TIM_SR_CC4IF;

// The armed tick must be at least this far in the future: the CCR write plus
// the IE enable take a few cycles, and a compare exactly at the write moment
// is racy. 4 us mirrors the TIM5 compare clamp.
static constexpr uint32_t ARM_MARGIN_TICKS = US2NT(4);

static constexpr int ANGLE_CLOCK_CHANNELS = 4;

struct AngleClockChannel {
	action_s action;
	uint32_t ccr = 0;
};

static AngleClockChannel s_channels[ANGLE_CLOCK_CHANNELS];

// TMR2->CNT - TIM5->CNT measured at init. Both counters free-run at 4 MHz
// from the same TIMCLK1, so the offset is constant and NT timestamps convert
// to angle-clock ticks by adding it.
static uint32_t s_ntOffset = 0;

static uint32_t s_firedCount = 0;
static uint32_t s_armFailCount = 0;
static uint32_t s_lateArmCount = 0;
static uint32_t s_maxLateTicks = 0;

// CCR1..CCR4 are separate fields in the AT32 TIM_TypeDef (not an array).
static volatile uint32_t* channelCcr(int ch) {
	switch (ch) {
	case 0:
		return &ANGLE_CLOCK_TIMER->CCR1;
	case 1:
		return &ANGLE_CLOCK_TIMER->CCR2;
	case 2:
		return &ANGLE_CLOCK_TIMER->CCR3;
	default:
		return &ANGLE_CLOCK_TIMER->CCR4;
	}
}

// CH_IRQ_HANDLER expands to 'extern "C" void VectorB0(void)' in C++ mode,
// which cannot be combined with the TRIGGER_RAM_CODE section attribute (GCC
// rejects attributes before the linkage specification) - spell it out so the
// dispatch path lives in zero-wait SRAM like the rest of the trigger code.
extern "C" {
TRIGGER_RAM_CODE void STM32_TIM2_HANDLER(void) {
	OSAL_IRQ_PROLOGUE();

	uint32_t sr = ANGLE_CLOCK_TIMER->SR & CC_IF_MASK;
	for (int ch = 0; ch < ANGLE_CLOCK_CHANNELS && sr; ch++) {
		uint32_t flag = STM32_TIM_SR_CC1IF << ch;
		if (!(sr & flag)) {
			continue;
		}
		sr &= ~flag;

		// Release the channel first (clear flag, disable IE, take the action
		// out) so a re-arm from inside the action cannot clobber it, then
		// execute outside the register-critical section.
		ANGLE_CLOCK_TIMER->SR = ~flag;
		ANGLE_CLOCK_TIMER->DIER &= ~(STM32_TIM_DIER_CC1IE << ch);

		auto& chState = s_channels[ch];
		action_s action = chState.action;
		uint32_t ccr = chState.ccr;
		chState.action = {};

		if (action) {
			// Dispatch telemetry: the fixed entry latency of this ISR.
			uint32_t late = ANGLE_CLOCK_TIMER->CNT - ccr;
			s_firedCount++;
			if (late > s_maxLateTicks) {
				s_maxLateTicks = late;
			}
			action.execute();
		}
	}

	assertInterruptPriority(__func__, EFI_IRQ_ANGLE_CLOCK_PRIORITY);

	OSAL_IRQ_EPILOGUE();
}
} // extern "C"

void initAngleClock() {
	rccEnableTIM2(false);

	// Freeze TMR2 together with TIM5 when the core halts (debugger): both are
	// free-running 4 MHz counters and the NT<->angle-clock offset must stay
	// constant across halts.
#if defined(STM32F4XX) || defined(STM32F7XX)
	DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_TIM2_STOP;
#endif

	// Free-running 32-bit up-counter at 4 MHz (TIMCLK1 = 288 MHz, PSC = 71),
	// the same tick rate as the NT domain (TIM5). No auto-reload, no
	// per-tooth reset - events are armed as absolute counter values.
	ANGLE_CLOCK_TIMER->PSC = 71;
	ANGLE_CLOCK_TIMER->ARR = 0xFFFFFFFF;
	ANGLE_CLOCK_TIMER->CR1 = 0;

	// Output-compare mode 1 on all four channels, output disabled (CCxE = 0):
	// pure software comparators, CCxIF fires at CNT == CCR.
	ANGLE_CLOCK_TIMER->CCMR1 = STM32_TIM_CCMR1_OC1M(1) | STM32_TIM_CCMR1_OC2M(1);
	ANGLE_CLOCK_TIMER->CCMR2 = STM32_TIM_CCMR1_OC1M(1) | STM32_TIM_CCMR1_OC2M(1);
	ANGLE_CLOCK_TIMER->CCER = 0;
	ANGLE_CLOCK_TIMER->DIER = 0;

	// Latch PSC/ARR and clear any stale flags.
	ANGLE_CLOCK_TIMER->EGR = STM32_TIM_EGR_UG;

	nvicEnableVector(STM32_TIM2_NUMBER, EFI_IRQ_ANGLE_CLOCK_PRIORITY);

	s_ntOffset = ANGLE_CLOCK_TIMER->CNT - getTimeNowLowerNt();

	ANGLE_CLOCK_TIMER->CR1 = STM32_TIM_CR1_CEN;
}

TRIGGER_RAM_CODE uint32_t angleClockTickForNt(efitick_t nt) {
	// Truncation to 32 bits is intentional: both counters wrap at 2^32, so
	// low-32-bit arithmetic is the wrap-correct representation.
	return static_cast<uint32_t>(nt) + s_ntOffset;
}

uint32_t angleClockNow() {
	return ANGLE_CLOCK_TIMER->CNT;
}

TRIGGER_RAM_CODE bool angleClockArm(uint32_t atTick, action_s action) {
	// Armed in the past = the compare equality was missed and the ISR will
	// never fire (until the 2^32 wrap) - a silently lost event. The caller
	// falls back to the time-based executor instead.
	if (static_cast<int32_t>(atTick - ANGLE_CLOCK_TIMER->CNT) < static_cast<int32_t>(ARM_MARGIN_TICKS)) {
		s_lateArmCount++;
		return false;
	}

	for (int ch = 0; ch < ANGLE_CLOCK_CHANNELS; ch++) {
		auto& chState = s_channels[ch];
		if (chState.action) {
			continue;
		}

		uint32_t flag = STM32_TIM_SR_CC1IF << ch;

		// Order matters, see the file-header concurrency note: stale flag
		// clear, then the future CCR, then the action, then the interrupt.
		ANGLE_CLOCK_TIMER->SR = ~flag;
		*channelCcr(ch) = atTick;
		chState.ccr = atTick;
		chState.action = action;
		ANGLE_CLOCK_TIMER->DIER |= STM32_TIM_DIER_CC1IE << ch;

		return true;
	}

	s_armFailCount++;
	return false;
}

void angleClockCancel(action_s action) {
	if (!action) {
		return;
	}

	for (int ch = 0; ch < ANGLE_CLOCK_CHANNELS; ch++) {
		auto& chState = s_channels[ch];
		if (chState.action == action) {
			chState.action = {};
			ANGLE_CLOCK_TIMER->DIER &= ~(STM32_TIM_DIER_CC1IE << ch);
		}
	}
}

uint32_t angleClockFiredCount() {
	return s_firedCount;
}

uint32_t angleClockArmFailCount() {
	return s_armFailCount;
}

uint32_t angleClockProgrammedLateCount() {
	return s_lateArmCount;
}

uint32_t angleClockMaxLateTicks() {
	return s_maxLateTicks;
}

void angleClockResetStats() {
	s_firedCount = 0;
	s_armFailCount = 0;
	s_lateArmCount = 0;
	s_maxLateTicks = 0;
}

#endif // EFI_ANGLE_CLOCK && EFI_PROD_CODE
