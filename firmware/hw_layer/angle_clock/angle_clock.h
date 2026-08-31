/**
 * @file angle_clock.h
 *
 * @author (c) 2026 rusEFI LLC
 *
 * Hardware angle clock (TMR2 on STM32F4/F7 and AT32F435) for one-tooth-ahead
 * firing of engine commands.
 *
 * The problem it solves: every engine command (dwell start, spark fire,
 * injection start) is converted angle->time at the trigger tooth IMMEDIATELY
 * before the event angle, so the software chain (EXTI timestamp -> handoff
 * decode -> scheduling) must complete within that one tooth. The handoff
 * averages ~44 us and tails to ~1 ms, so events due in a tooth whose handoff
 * runs late fire late by the whole tail (up to 36 deg at 6000 rpm on 60-2).
 *
 * The angle clock decouples the firing from the handoff: the handoff of tooth
 * N arms tooth N+1's queued angle events as absolute timer ticks on one of
 * four OC channels used as PURE SOFTWARE COMPARATORS (CCxE=0 - no output pin,
 * CCxIF fires at CNT == CCR). The TMR2 ISR (priority 3, the same level as the
 * TIM5 executor) then just executes the action. A late handoff only misses
 * the arming of the tooth it spans - those events fall back to the time-based
 * TIM5 path.
 *
 * TMR2 runs free at 4 MHz (TIMCLK1 = 288 MHz, PSC = 71) - the same tick as
 * the NT domain (TIM5), so NT timestamps convert to angle-clock ticks with a
 * constant offset measured at init. The counter is never reset per tooth:
 * events are armed as ABSOLUTE counter values, which needs no writes from the
 * priority-0 EXTI fast IRQ and cannot fire spuriously in the current tooth.
 * The 32-bit wrap (~1073 s) is absorbed by unsigned tick arithmetic.
 *
 * TIMING ACCURACY AT ANY RPM (the 2026-08-31 redesign): the armed tick must
 * not be computed from the 90-degree rpm average (InstantRpmCalculator's
 * oneDegreeUs) - that value lags by revolutions at the catch and made every
 * armed event fire ms-late (the fuse incident). Instead the handoff feeds
 * this module the freshly measured per-degree duration of the LAST completed
 * tooth (angleClockOnTooth), and every tooth re-anchors all armed channels
 * from that freshest data (angleClockRefresh). The prediction error is then
 * always bounded by one tooth of acceleration - a few percent even at the
 * 250->1500 rpm catch - instead of the full catch transient.
 *
 * STALE EVENT POLICY: an event whose moment has passed must NOT be executed
 * late. Per kind:
 *  - AngleClockKind::CoilFire (coil off) always executes - discharging the
 *    coil is the safety action; the redundant overdwell rescue on TIM5 is the
 *    second writer of the same coil-off.
 *  - AngleClockKind::Start (dwell start / injection start) is dropped when
 *    late - a late charge start is useless and piles all coils onto one
 *    moment, which is what blows fuses.
 *
 * The fallback contract: if the armed tick is already in the past (handoff
 * ran late) or all four channels are busy, angleClockArm() returns false and
 * the caller schedules the event on the time-based executor as before.
 */

#pragma once

#include "scheduler.h"
#include "efitime.h"

// Pure tick math, host-testable: convert "angleFromNow degrees until the
// event" and "ticksPerDegree NT ticks per degree" (fresh last-tooth basis,
// see angleClockOnTooth) into a delay in angle-clock ticks (4 MHz, == NT
// ticks on the target).
static inline uint32_t angleClockDelayTicks(float angleFromNow, float ticksPerDegree) {
	return static_cast<uint32_t>(angleFromNow * ticksPerDegree);
}

#if EFI_ANGLE_CLOCK

// What the armed action does - decides the stale-event policy, see the file
// header comment.
enum class AngleClockKind : uint8_t {
	CoilFire,	// spark fire / coil off: execute even when late (coil safety)
	Start,		// dwell start / injection start: drop when late
};

// Configure TMR2 (RCC, PSC, compare channels, NVIC) and measure the
// NT<->angle-clock offset. Call once at boot, after the TIM5 executor is
// running (both are free-running 4 MHz counters on the same TIMCLK1).
void initAngleClock();

// Convert an NT-domain timestamp into an absolute angle-clock tick.
uint32_t angleClockTickForNt(efitick_t nt);

// Current free-running angle-clock counter value.
uint32_t angleClockNow();

// Called by the trigger handoff ONCE PER TOOTH, before any arming in that
// tooth: stores the edge timestamp, the scheduling phase and the freshest
// angle->time basis (NT ticks per degree of the LAST completed tooth,
// measured from the decoder's toothDurations[0] and the real tooth span).
// cycleDeg is the engine cycle (720 four-stroke / 360 two-stroke) used for
// angle wrap. All arming and the refresh use this stored data - never the
// 90-degree rpm average.
void angleClockOnTooth(efitick_t edgeTimestamp, float currentPhase, float cycleDeg, float ticksPerDegree);

// Arm `action` to fire at the absolute engine angle `targetAngle` (same
// basis as the currentPhase passed to angleClockOnTooth). `callerPhase` is
// the caller's currentPhase (diagnostic: must equal the stored phase in a
// single handoff). The delay is computed from the freshest tooth data.
// Returns false (without arming) when the target is not a plausible 1-2 tooth
// lookahead, the tick is not far enough in the future, or all four channels
// are busy - the caller must fall back to the TIM5 path.
bool angleClockArm(float targetAngle, action_s action, AngleClockKind kind, float callerPhase, float callerNextPhase);

// Re-anchor every armed channel from the freshest tooth data: rewrite the
// compare tick from the current phase and last-tooth duration, so the
// prediction error never exceeds one tooth of acceleration. Events whose
// angle has already passed are handled per their kind: CoilFire is armed for
// immediate firing, Start is cancelled. Channels whose stored angle is no
// longer plausible (phase basis jumped, e.g. desync/re-sync) are LEFT ARMED:
// their tick is an absolute time and fires the event within 1-2 teeth just
// like the time-based build - a lost CoilFire would only be covered by the
// overdwell rescue, which is exactly the late-fire signature cancelling
// produced on the car. Call at the end of the trigger handoff, after all
// arming of that tooth.
void angleClockRefresh();

// Cancel every armed channel. Kept as the driver API, but NO LONGER called
// from the rpm==0 / firmwareError handoff paths: during the catch trigger
// storm the rpm sensor flaps 0/nonzero at ~1 kHz and each flap cancelled the
// armed fires (charged coils discharged by 4.5 ms rescues - the C935x
// cluster) and armed dwell starts (fires on uncharged coils - C9012). The
// armed ticks are bounded and the charge-anchored overdwell rescue bounds
// any charge, so armed events fire like the time-based build - by time,
// regardless of the rpm flap.
void angleClockCancelAll();

// Cancel every armed channel whose action matches (callback + argument).
// Safe to call when nothing matching is armed.
void angleClockCancel(action_s action);

// Telemetry for lockstats.
uint32_t angleClockFiredCount();
uint32_t angleClockArmFailCount();
uint32_t angleClockProgrammedLateCount();
uint32_t angleClockDroppedCount();
uint32_t angleClockMaxLateTicks();

// Arm-failure breakdown (diagnostic): attempts, refusals (remaining >
// MAX_LEAD_DEG), all-channels-busy, the last refusal's inputs (target angle,
// stored phase, basis) and the max |ccr - CNT| of a busy channel at arm time.
uint32_t angleClockArmAttempts();
uint32_t angleClockRefuseCount();
uint32_t angleClockNoChannelCount();
float angleClockLastRefuseTarget();
float angleClockLastRefusePhase();
float angleClockLastRefuseCallerPhase();
float angleClockLastRefuseCallerNext();
float angleClockLastRefuseBasis();
float angleClockLastRefuseRemaining();
uint32_t angleClockLastRefuseCallback();
uint32_t angleClockMaxBusyDeltaTicks();

// TMR2 rate measurement from initAngleClock.
uint32_t angleClockInitAcDelta();
uint32_t angleClockInitNtDelta();
uint32_t angleClockInitPsc();

void angleClockResetStats();

#endif // EFI_ANGLE_CLOCK
