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
 * Concurrency: the handoff ISR (priority 4) arms/refreshes/cancels channels,
 * the TMR2 ISR (priority 3) fires them. The TMR2 ISR can preempt the handoff
 * mid-arm, so the arm sequence is ordered so that no partial state can fire:
 *   1. clear stale CCxIF
 *   2. write CCR (a tick >= arm margin in the future - CNT cannot reach it
 *      within this ISR window)
 *   3. store the kind and the action
 *   4. enable CCxIE
 * A preempting TMR2 ISR sees no flag for a channel mid-arm (the flag cannot
 * set until CNT reaches the future CCR), and it never executes an action on
 * a channel whose action is empty. The priority-0 EXTI fast IRQ does not
 * touch the angle clock at all.
 *
 * The per-tooth refresh (angleClockRefresh, also handoff context) only ever
 * rewrites CCR of an ACTIVE channel to a FUTURE tick - never action/IE - so
 * a preempting ISR either fires at the old CCR (earlier than intended, still
 * a valid discharge) or sees the channel released. A passed CCR cannot be
 * "reached again", so a refresh can never cause a double fire.
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

// Absolute ceiling for the angle->time basis fed by angleClockOnTooth, see
// the comment there: NT ticks per degree, 5 ms/deg. Bounds the armed delay
// to MAX_LEAD_DEG x 5 ms = 150 ms so a garbage basis cannot stick a channel
// for the whole run.
static constexpr float MAX_TICKS_PER_DEGREE = US2NT(5000);

// Arming/refresh reject targets farther than this many degrees away. The
// early windows arm at most 2 teeth ahead (12.4 deg on 60-2); anything beyond
// this bound means the phase basis jumped (desync/re-sync) and the stored
// angle is no longer meaningful - drop instead of firing into the wrong
// phase. A lost CoilFire is covered by the overdwell rescue on TIM5.
static constexpr float MAX_LEAD_DEG = 30.0f;

// A Start event (dwell/injection start) that executes this late in the TMR2
// ISR is useless and dangerous: it charges a coil after its moment. Normal
// ISR lateness at priority 3 is microseconds; this threshold only trips on a
// gross stall (long chSysLock, flash write) during which the compare passed
// while the ISR could not run. CoilFire ignores it - the discharge must
// always happen.
static constexpr uint32_t DROP_LATE_TICKS = US2NT(1000);

static constexpr int ANGLE_CLOCK_CHANNELS = 4;

struct AngleClockChannel {
	action_s action;
	AngleClockKind kind = AngleClockKind::Start;
	// Absolute engine angle the event is armed for - the per-tooth refresh
	// re-anchors from it (same basis as the currentPhase fed by
	// angleClockOnTooth).
	float targetAngle = 0;
	uint32_t ccr = 0;
};

static AngleClockChannel s_channels[ANGLE_CLOCK_CHANNELS];

// TMR2->CNT - TIM5->CNT measured at init. Both counters free-run at 4 MHz
// from the same TIMCLK1, so the offset is constant and NT timestamps convert
// to angle-clock ticks by adding it.
static uint32_t s_ntOffset = 0;

// Per-tooth state, fed by angleClockOnTooth from the trigger handoff. Only
// the handoff (single context, priority 4) writes these; the TMR2 ISR never
// touches them.
static efitick_t s_edgeTimestamp = 0;
static float s_currentPhase = 0;
static float s_cycleDeg = 720;
static float s_ticksPerDegree = 0;	// NT ticks per degree of the LAST tooth

static uint32_t s_firedCount = 0;
static uint32_t s_armFailCount = 0;
static uint32_t s_lateArmCount = 0;
static uint32_t s_droppedCount = 0;
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
// which cannot be combined with the section attribute (GCC
// rejects attributes before the linkage specification) - spell it out so the
// dispatch path lives in zero-wait SRAM like the rest of the trigger code.
extern "C" {
void STM32_TIM2_HANDLER(void) {
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
		AngleClockKind kind = chState.kind;
		uint32_t ccr = chState.ccr;
		chState.action = {};

		if (action) {
			// Dispatch telemetry: the fixed entry latency of this ISR.
			uint32_t late = ANGLE_CLOCK_TIMER->CNT - ccr;
			s_firedCount++;
			if (late > s_maxLateTicks) {
				s_maxLateTicks = late;
			}

			// Stale-event policy, see angle_clock.h: a charge/injection start
			// that executes this late is dropped - its moment has passed and
			// firing it anyway is what piles all coils onto one instant.
			if (kind == AngleClockKind::CoilFire || late <= DROP_LATE_TICKS) {
				action.execute();
			} else {
				s_droppedCount++;
			}
		}
	}

	assertInterruptPriority(__func__, EFI_IRQ_ANGLE_CLOCK_PRIORITY);

	OSAL_IRQ_EPILOGUE();
}
} // extern "C"

void initAngleClock() {
	/* lp=true is LOAD-BEARING on the AT32 port: rccEnableTIMx(false) CLEARS
	 * the APB1LPENR bit (the fork's rccEnableAPB1 macro), which gates the
	 * timer clock OFF in sleep mode. TIM5 (the NT domain, enabled by the
	 * PWM LLD with lp=true) keeps running in sleep, so a sleep-gated TMR2
	 * would drift its init-measured NT<->angle-clock offset on every idle
	 * period (key-on engine-off, console idle) - every armed absolute tick
	 * would fire at the wrong time after wake. The same bug gated the WDA
	 * feed timer (see l9779.cpp rccEnableTIM7): the whole 2x-period saga
	 * was a sleep-gated counter, not a drifting clock. */
	rccEnableTIM2(true);

	// Freeze TMR2 together with TIM5 when the core halts (debugger): both are
	// free-running 4 MHz counters and the NT<->angle-clock offset must stay
	// constant across halts.
#if defined(STM32F4XX) || defined(STM32F7XX)
	DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_TIM2_STOP;
#endif

	// Free-running 32-bit up-counter, PROVISIONALLY at 4 MHz (PSC = 71 under
	// the fork's TIMCLK1 = PCLK1 * 2 = 288 MHz assumption). No auto-reload,
	// no per-tooth reset - events are armed as absolute counter values.
	ANGLE_CLOCK_TIMER->PSC = 71;
	ANGLE_CLOCK_TIMER->ARR = 0xFFFFFFFF;
	ANGLE_CLOCK_TIMER->CR1 = 0;

	// Output-compare mode 1 on all four channels, output disabled (CCxE = 0):
	// pure software comparators, CCxIF fires at CNT == CCR.
	ANGLE_CLOCK_TIMER->CCMR1 = STM32_TIM_CCMR1_OC1M(1) | STM32_TIM_CCMR1_OC2M(1);
	ANGLE_CLOCK_TIMER->CCMR2 = STM32_TIM_CCMR1_OC1M(1) | STM32_TIM_CCMR1_OC2M(1);
	ANGLE_CLOCK_TIMER->CCER = 0;
	ANGLE_CLOCK_TIMER->DIER = 0;

	// Latch PSC/ARR and clear any stale flags, then start the counter for
	// the rate measurement below.
	ANGLE_CLOCK_TIMER->EGR = STM32_TIM_EGR_UG;
	ANGLE_CLOCK_TIMER->SR = 0;
	ANGLE_CLOCK_TIMER->CR1 = STM32_TIM_CR1_CEN;

	// MEASURE the real counter rate against the NT timer (TIM5): the NT
	// 4 MHz domain is load-bearing and validated by tooth physics on the
	// car (rpm readings match reality), so it is the reference. The fork's
	// STM32_TIMCLK1 claims PCLK1 * 2 for all APB1 timers, but the AT32F435
	// does NOT necessarily double the APB timer clock - TMR10 on APB2
	// measured 144 MHz against the claimed 288. A 2x-slow angle clock makes
	// EVERY armed event fire 2x late, which doubles every dwell: the
	// 2026-08-30 catch overcharged all four coils ~8 ms (2x the nominal
	// cranking dwell) and blew the 15A fuse. Program the PSC from the
	// MEASURED rate instead, so the counter always ticks at exactly 4 MHz
	// == the NT domain, on any silicon.
	uint32_t ac0 = ANGLE_CLOCK_TIMER->CNT;
	uint32_t nt0 = getTimeNowLowerNt();
	do { } while (getTimeNowLowerNt() - nt0 < US2NT(10000));	/* 10 ms */
	uint32_t acDelta = ANGLE_CLOCK_TIMER->CNT - ac0;
	uint32_t ntDelta = getTimeNowLowerNt() - nt0;

	// newPsc = (PSC_provisional + 1) * acDelta / ntDelta - 1: scales the
	// measured tick rate to exactly the NT rate (acDelta == ntDelta).
	uint32_t newPsc = (72 * acDelta / ntDelta) - 1;
	if (newPsc > 0xFFFF)
		newPsc = 0xFFFF;

	ANGLE_CLOCK_TIMER->CR1 = 0;
	ANGLE_CLOCK_TIMER->PSC = newPsc;
	ANGLE_CLOCK_TIMER->CNT = 0;
	ANGLE_CLOCK_TIMER->EGR = STM32_TIM_EGR_UG;	/* latch the new PSC */
	ANGLE_CLOCK_TIMER->SR = 0;
	ANGLE_CLOCK_TIMER->CR1 = STM32_TIM_CR1_CEN;

	nvicEnableVector(STM32_TIM2_NUMBER, EFI_IRQ_ANGLE_CLOCK_PRIORITY);

	// The offset is only valid at the FINAL counter rate - measure it after
	// the PSC re-programming.
	s_ntOffset = ANGLE_CLOCK_TIMER->CNT - getTimeNowLowerNt();

	efiPrintf("angle clock: measured rate %lu/%lu ticks (PSC 71 -> %lu)",
		(unsigned long)acDelta, (unsigned long)ntDelta, (unsigned long)newPsc);
}

uint32_t angleClockTickForNt(efitick_t nt) {
	// Truncation to 32 bits is intentional: both counters wrap at 2^32, so
	// low-32-bit arithmetic is the wrap-correct representation.
	return static_cast<uint32_t>(nt) + s_ntOffset;
}

uint32_t angleClockNow() {
	return ANGLE_CLOCK_TIMER->CNT;
}

void angleClockOnTooth(efitick_t edgeTimestamp, float currentPhase, float cycleDeg, float ticksPerDegree) {
	s_edgeTimestamp = edgeTimestamp;
	s_currentPhase = currentPhase;
	s_cycleDeg = cycleDeg;

	// Absolute ceiling on the angle->time basis: the decoder clamps the stored
	// tooth duration to 10 s after a long pause, and the relative rpm-band
	// clamp cannot see it when the rpm average itself is stale (NaN at the
	// first teeth). 5 ms/deg (~32 rpm equivalent) is above any real cranking
	// tooth and far below the garbage - a capped basis bounds the armed delay
	// (MAX_LEAD_DEG x the cap) so a channel can never be stuck for the run.
	if (!(ticksPerDegree > 0)) {
		s_ticksPerDegree = 0;
	} else if (ticksPerDegree > MAX_TICKS_PER_DEGREE) {
		s_ticksPerDegree = MAX_TICKS_PER_DEGREE;
	} else {
		s_ticksPerDegree = ticksPerDegree;
	}
}

// Angle from the current phase to the target, wrapped into [0, cycleDeg).
static float remainingAngle(float targetAngle) {
	float remaining = targetAngle - s_currentPhase;
	if (remaining < 0) {
		remaining += s_cycleDeg;
	}
	return remaining;
}

static uint32_t tickForAngle(float targetAngle) {
	return angleClockTickForNt(s_edgeTimestamp) + angleClockDelayTicks(remainingAngle(targetAngle), s_ticksPerDegree);
}

static void cancelChannel(int ch) {
	auto& chState = s_channels[ch];
	chState.action = {};
	ANGLE_CLOCK_TIMER->DIER &= ~(STM32_TIM_DIER_CC1IE << ch);
}

bool angleClockArm(float targetAngle, action_s action, AngleClockKind kind) {
	float remaining = remainingAngle(targetAngle);

	// Refuse targets beyond the 1-2 tooth lookahead: the phase basis has
	// jumped (desync/re-sync) and the stored angle is not meaningful.
	if (remaining > MAX_LEAD_DEG) {
		s_armFailCount++;
		return false;
	}

	uint32_t atTick = tickForAngle(targetAngle);

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
		// clear, then the future CCR, then the kind/action, then the
		// interrupt.
		ANGLE_CLOCK_TIMER->SR = ~flag;
		*channelCcr(ch) = atTick;
		chState.ccr = atTick;
		chState.targetAngle = targetAngle;
		chState.kind = kind;
		chState.action = action;
		ANGLE_CLOCK_TIMER->DIER |= STM32_TIM_DIER_CC1IE << ch;

		return true;
	}

	s_armFailCount++;
	return false;
}

void angleClockRefresh() {
	for (int ch = 0; ch < ANGLE_CLOCK_CHANNELS; ch++) {
		auto& chState = s_channels[ch];
		if (!chState.action) {
			continue;
		}

		uint32_t flag = STM32_TIM_SR_CC1IF << ch;

		// The old CCR may already have matched while the TMR2 ISR was masked
		// (a chSysLock section of this handoff): the flag is pending and the
		// ISR will fire the channel with the OLD ccr as soon as it unmasks.
		// Skipping keeps the ccr the ISR measures consistent - re-anchoring
		// here made 'late = CNT - ccr' read the NEW far-future ccr and wrap
		// (~2^32 in the maxLateUs telemetry). The channel re-anchors next tooth.
		if (ANGLE_CLOCK_TIMER->SR & flag) {
			continue;
		}

		float remaining = remainingAngle(chState.targetAngle);

		// Stale phase basis (desync/re-sync): the stored angle is no longer
		// meaningful. Drop the event - a lost CoilFire is covered by the
		// overdwell rescue which is kept armed on TIM5.
		if (remaining > MAX_LEAD_DEG) {
			cancelChannel(ch);
			continue;
		}

		uint32_t newTick = tickForAngle(chState.targetAngle);

		if (static_cast<int32_t>(newTick - ANGLE_CLOCK_TIMER->CNT) < static_cast<int32_t>(ARM_MARGIN_TICKS)) {
			// The angle has arrived (or the handoff ran late enough to miss
			// it): fire now if it is a coil discharge, drop useless late
			// starts.
			if (chState.kind == AngleClockKind::CoilFire) {
				// Arm for immediate firing - the ISR (priority 3) executes it
				// right after this handoff.
				uint32_t nowTick = ANGLE_CLOCK_TIMER->CNT + ARM_MARGIN_TICKS;
				*channelCcr(ch) = nowTick;
				chState.ccr = nowTick;
			} else {
				cancelChannel(ch);
			}
			continue;
		}

		// Re-anchor: only the CCR moves (earlier OR later, both safe - the
		// compare can only hit a future tick once). Never touches action/IE,
		// see the file-header concurrency note.
		*channelCcr(ch) = newTick;
		chState.ccr = newTick;
	}
}

void angleClockCancelAll() {
	for (int ch = 0; ch < ANGLE_CLOCK_CHANNELS; ch++) {
		cancelChannel(ch);
	}
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

uint32_t angleClockDroppedCount() {
	return s_droppedCount;
}

uint32_t angleClockMaxLateTicks() {
	return s_maxLateTicks;
}

void angleClockResetStats() {
	s_firedCount = 0;
	s_armFailCount = 0;
	s_lateArmCount = 0;
	s_droppedCount = 0;
	s_maxLateTicks = 0;
}

#endif // EFI_ANGLE_CLOCK && EFI_PROD_CODE
