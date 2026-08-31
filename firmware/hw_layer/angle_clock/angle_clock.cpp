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
// the comment there: NT ticks per degree, 2 ms/deg (~120 rpm floor). Bounds
// the armed delay to MAX_LEAD_DEG x 2 ms so a garbage basis cannot stick a
// channel for the whole run - and cannot stretch the armed tick into the
// catch-storm lateness that made the car fire 4.5-8 ms late.
static constexpr float MAX_TICKS_PER_DEGREE = US2NT(2000);

// Arming/refresh reject targets farther than this many degrees away. The
// early windows arm at most 2 teeth ahead (12.4 deg on 60-2); anything beyond
// this bound means the phase basis jumped (desync/re-sync) and the stored
// angle is no longer meaningful - drop instead of firing into the wrong
// phase. A lost CoilFire is covered by the overdwell rescue on TIM5.
static constexpr float MAX_LEAD_DEG = 30.0f;

// Every armed event executes when its compare fires, whatever the dispatch
// lateness: the angle-domain arming + per-tooth refresh bound the armed
// delay (a few ms), and the charge-anchored overdwell rescue bounds any
// charge to 1.5x dwell. There is no late-start drop anymore - dropping turned
// executor load into a complete misfire (no charge at all) and a wrapped
// unsigned late misread the refresh race as a drop.

// A desynced channel's armed tick may be stretched by a storm-garbage basis
// (the phase jump left it armed - see the refresh branch below). Clamp it to
// this horizon so it cannot fire wildly late: the event fires within the
// bound, the charge-anchored overdwell rescue stays the backstop.
static constexpr uint32_t MAX_STALE_TICKS = US2NT(4000);

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

// Arm-failure breakdown (diagnostic, 2026-08-31): the combined armFail
// counter cannot tell a stale-phase refusal (remaining > MAX_LEAD_DEG) from
// all-four-channels-busy - the two have opposite root causes. Split them and
// snapshot the last refusal's inputs so a single lockstats settles which one
// dominates without guessing.
static uint32_t s_armAttempts = 0;
static uint32_t s_refuseCount = 0;		// remaining > MAX_LEAD_DEG
static uint32_t s_noChannelCount = 0;	// all four channels busy
static float s_lastRefuseTarget = 0;	// targetAngle of the last refusal
static float s_lastRefusePhase = 0;		// s_currentPhase of the last refusal
static float s_lastRefuseCallerPhase = 0;	// the caller's currentPhase at the last refusal
static float s_lastRefuseCallerNext = 0;	// the caller's nextPhase at the last refusal
static float s_lastRefuseBasis = 0;		// s_ticksPerDegree of the last refusal
static float s_lastRefuseRemaining = 0;	// the computed remaining (wrapped) that was refused
static uint32_t s_lastRefuseCallback = 0;	// callback address of the refused action - names the caller
static uint32_t s_maxBusyDeltaTicks = 0;	// max |ccr - CNT| of a busy channel at arm time

// TMR2 rate measurement result from initAngleClock - printed in lockstats so
// a wrong PSC (the fuse-incident class) is visible without scrolling to boot.
static uint32_t s_initAcDelta = 0;
static uint32_t s_initNtDelta = 0;
static uint32_t s_initPsc = 0;

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
			// Dispatch telemetry: SIGNED so a re-anchored ccr in the future
			// (the refresh race) reads as negative (early), not a ~2^32 wrap
			// that the old unsigned read misclassified as grossly late and
			// DROPPED (2026-08-31 800 rpm self-stim: drop=fired=642, which
			// killed every dwell so sched dwell=0 spark=8).
			int32_t late = static_cast<int32_t>(ANGLE_CLOCK_TIMER->CNT - ccr);
			s_firedCount++;
			if (late > 0 && static_cast<uint32_t>(late) > s_maxLateTicks) {
				s_maxLateTicks = static_cast<uint32_t>(late);
			}

			// Every armed event executes when its compare fires. The
			// angle-domain arming + per-tooth refresh bound the delay and
			// the charge-anchored overdwell rescue bounds any charge, so a
			// late event cannot pile up (the pre-redesign 14:13 fuse
			// incident). Dropping a late start instead converted executor
			// load into a lost spark (no charge at all), and a wrapped late
			// turned the refresh race into a spurious drop.
			action.execute();
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

	s_initAcDelta = acDelta;
	s_initNtDelta = ntDelta;
	s_initPsc = newPsc;

	efiPrintf("angle clock: measured rate %lu/%lu ticks (PSC 71 -> %lu)",
		(unsigned long)acDelta, (unsigned long)ntDelta, (unsigned long)newPsc);
}

TRIGGER_RAM_CODE uint32_t angleClockTickForNt(efitick_t nt) {
	// Truncation to 32 bits is intentional: both counters wrap at 2^32, so
	// low-32-bit arithmetic is the wrap-correct representation.
	return static_cast<uint32_t>(nt) + s_ntOffset;
}

uint32_t angleClockNow() {
	return ANGLE_CLOCK_TIMER->CNT;
}

TRIGGER_RAM_CODE void angleClockOnTooth(efitick_t edgeTimestamp, float currentPhase, float cycleDeg, float ticksPerDegree) {
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
TRIGGER_RAM_CODE static float remainingAngle(float targetAngle) {
	float remaining = targetAngle - s_currentPhase;
	if (remaining < 0) {
		remaining += s_cycleDeg;
	}
	return remaining;
}

TRIGGER_RAM_CODE static uint32_t tickForAngle(float targetAngle) {
	return angleClockTickForNt(s_edgeTimestamp) + angleClockDelayTicks(remainingAngle(targetAngle), s_ticksPerDegree);
}

TRIGGER_RAM_CODE static void cancelChannel(int ch) {
	auto& chState = s_channels[ch];
	chState.action = {};
	ANGLE_CLOCK_TIMER->DIER &= ~(STM32_TIM_DIER_CC1IE << ch);
}

TRIGGER_RAM_CODE bool angleClockArm(float targetAngle, action_s action, AngleClockKind kind, float callerPhase, float callerNextPhase) {
	s_armAttempts++;

	float remaining = remainingAngle(targetAngle);

	// Refuse targets beyond the 1-2 tooth lookahead: the phase basis has
	// jumped (desync/re-sync) and the stored angle is not meaningful.
	if (remaining > MAX_LEAD_DEG) {
		s_armFailCount++;
		s_refuseCount++;
		s_lastRefuseTarget = targetAngle;
		s_lastRefusePhase = s_currentPhase;
		s_lastRefuseCallerPhase = callerPhase;
		s_lastRefuseCallerNext = callerNextPhase;
		s_lastRefuseBasis = s_ticksPerDegree;
		s_lastRefuseRemaining = remaining;
		s_lastRefuseCallback = reinterpret_cast<uint32_t>(action.getCallback());
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
			// Busy: record how far its tick is in the FUTURE (a genuinely stuck
			// channel). A passed tick (negative) is a stale ccr, not a stuck
			// channel - the unsigned wrap would report ~2^32.
			int32_t delta = static_cast<int32_t>(chState.ccr - ANGLE_CLOCK_TIMER->CNT);
			if (delta > 0 && static_cast<uint32_t>(delta) > s_maxBusyDeltaTicks) {
				s_maxBusyDeltaTicks = static_cast<uint32_t>(delta);
			}
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
	s_noChannelCount++;
	return false;
}

TRIGGER_RAM_CODE void angleClockRefresh() {
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

		// Phase basis jumped (desync/re-sync): the stored angle is no longer
		// meaningful for RE-ANCHORING, but the armed tick is an absolute time
		// and does not jump - leave the channel armed so it fires, exactly
		// like the time-based build (which fires by time regardless of sync).
		// The tick may however be stretched by a storm-garbage basis from its
		// arming tooth - clamp it to a bounded horizon when it is farther out,
		// so it cannot fire 4-16x late (the car catch instability: the rescue
		// discharged at 1.5x dwell before the stretched fire arrived).
		if (remaining > MAX_LEAD_DEG) {
			uint32_t oldCcr = *channelCcr(ch);
			if (static_cast<int32_t>(oldCcr - ANGLE_CLOCK_TIMER->CNT) > static_cast<int32_t>(MAX_STALE_TICKS)) {
				uint32_t boundedTick = ANGLE_CLOCK_TIMER->CNT + MAX_STALE_TICKS;
				*channelCcr(ch) = boundedTick;
				// Same match-in-the-window race guard as the re-anchor below:
				// the old CCR can match between the SR check above and this
				// write. Restore it when it did - the pending ISR then measures
				// its true latency and fires the event.
				if (ANGLE_CLOCK_TIMER->SR & flag) {
					*channelCcr(ch) = oldCcr;
				} else {
					chState.ccr = boundedTick;
				}
			}
			continue;
		}

		uint32_t oldCcr = *channelCcr(ch);
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
				// The old CCR can match between the SR check above and this
				// write (the flag sets, the prio-3 ISR preempts this handoff a
				// few cycles later). The pending ISR fires the channel either
				// way - restore the old CCR so its lateness telemetry is the
				// true dispatch latency, not a ~2^32 wrap against the new one.
				if (ANGLE_CLOCK_TIMER->SR & flag) {
					*channelCcr(ch) = oldCcr;
				} else {
					chState.ccr = nowTick;
				}
			} else {
				cancelChannel(ch);
			}
			continue;
		}

		// Re-anchor: only the CCR moves (earlier OR later, both safe - the
		// compare can only hit a future tick once). Never touches action/IE,
		// see the file-header concurrency note. The write is not atomic with
		// the SR check above: the old CCR can match in the few cycles between
		// them (the flag sets before the prio-3 ISR runs). Writing the new
		// far-future CCR would make that pending ISR measure
		// late = CNT - newCcr (the ~2^32 maxLateUs wrap) - so re-read SR after
		// the write and restore the old CCR when the match happened in the
		// window. The channel then re-anchors next tooth.
		*channelCcr(ch) = newTick;
		if (ANGLE_CLOCK_TIMER->SR & flag) {
			*channelCcr(ch) = oldCcr;
		} else {
			chState.ccr = newTick;
		}
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

uint32_t angleClockArmAttempts() {
	return s_armAttempts;
}

uint32_t angleClockRefuseCount() {
	return s_refuseCount;
}

uint32_t angleClockNoChannelCount() {
	return s_noChannelCount;
}

float angleClockLastRefuseTarget() {
	return s_lastRefuseTarget;
}

float angleClockLastRefusePhase() {
	return s_lastRefusePhase;
}

float angleClockLastRefuseCallerPhase() {
	return s_lastRefuseCallerPhase;
}

float angleClockLastRefuseCallerNext() {
	return s_lastRefuseCallerNext;
}

float angleClockLastRefuseBasis() {
	return s_lastRefuseBasis;
}

float angleClockLastRefuseRemaining() {
	return s_lastRefuseRemaining;
}

uint32_t angleClockLastRefuseCallback() {
	return s_lastRefuseCallback;
}

uint32_t angleClockMaxBusyDeltaTicks() {
	return s_maxBusyDeltaTicks;
}

uint32_t angleClockInitAcDelta() {
	return s_initAcDelta;
}

uint32_t angleClockInitNtDelta() {
	return s_initNtDelta;
}

uint32_t angleClockInitPsc() {
	return s_initPsc;
}

void angleClockResetStats() {
	s_firedCount = 0;
	s_armFailCount = 0;
	s_lateArmCount = 0;
	s_droppedCount = 0;
	s_maxLateTicks = 0;
	s_armAttempts = 0;
	s_refuseCount = 0;
	s_noChannelCount = 0;
	s_lastRefuseTarget = 0;
	s_lastRefusePhase = 0;
	s_lastRefuseCallerPhase = 0;
	s_lastRefuseCallerNext = 0;
	s_lastRefuseBasis = 0;
	s_lastRefuseRemaining = 0;
	s_lastRefuseCallback = 0;
	s_maxBusyDeltaTicks = 0;
}

#endif // EFI_ANGLE_CLOCK && EFI_PROD_CODE
