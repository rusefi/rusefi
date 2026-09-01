/**
 * @file angle_clock.h
 *
 * Hardware angle clock: three APB1 timers as pure software comparators,
 * one timer per event type, one channel per cylinder.
 *
 *   TMR2 (32-bit, VectorB0, prio 3) -> dwell starts   [cylinder 0..3 = ch 0..3]
 *   TMR4 (16-bit, VectorB8, prio 3) -> spark fires     [cylinder 0..3 = ch 0..3]
 *   TMR3 (16-bit, VectorB4, prio 3) -> injection starts[cylinder 0..3 = ch 0..3]
 *
 * FIXED CHANNEL ASSIGNMENT eliminates the "no channel available" arm failure
 * of the old single-pool design: each cylinder owns a dedicated channel in
 * each timer. A channel is always free at arm time because the previous event
 * on that channel fired within one engine cycle (~150 ms @ 800 rpm) and was
 * released by the ISR immediately. No search, no conflict possible.
 *
 * 16-bit timers (TMR3/TMR4): same APB1 4 MHz clock as TMR2/TIM5.
 * The CCR is a 16-bit value; the arm check uses signed 16-bit arithmetic so
 * delays > ~8 ms (half the 16.384 ms counter period) return false and the
 * caller falls back to TIM5 - this degrades gracefully below ~200 rpm where
 * the 2-tooth scheduling lead exceeds 8 ms. The angle clock is not needed
 * below ~200 rpm anyway.
 *
 * Timing identity with the FALSE build: at steady-state rpm the tick computed
 * from oneDegreeUs on tooth T-1 plus the per-tooth refresh on tooth T gives
 * the same absolute compare moment as TIM5 would schedule on tooth T. The
 * advantage is that the TMR ISR fires independently of the handoff latency
 * at tooth T - a 1 ms handoff tail no longer delays the event.
 */

#pragma once

#include "scheduler.h"
#include "efitime.h"

// Pure tick math, host-testable.
static inline uint32_t angleClockDelayTicks(float angleFromNow, float ticksPerDegree) {
    return static_cast<uint32_t>(angleFromNow * ticksPerDegree);
}

#if EFI_ANGLE_CLOCK

// Configure TMR2/TMR3/TMR4, measure rate, enable NVIC. Call once at boot,
// after the TIM5 executor is running.
void initAngleClock();

// Convert an NT timestamp to an absolute 32-bit angle-clock tick (TMR2 domain).
uint32_t angleClockTickForNt(efitick_t nt);

// Current TMR2 free-running counter value (for diagnostics).
uint32_t angleClockNow();

// Feed the per-tooth state used by all three timers. Call once per tooth
// (including rpm==0 storm-flap teeth - the refresh must track true speed).
// A not-positive ticksPerDegree (NaN on rpm==0 flap) keeps the last good
// basis without touching the phase or timestamp.
void angleClockOnTooth(efitick_t edgeTimestamp, float currentPhase,
                        float cycleDeg, float ticksPerDegree);

// Arm the dwell-start channel for `cylinderIndex` on TMR2.
// Returns false when the delay would exceed the scheduler lead window or when
// the target tick is already in the past - caller falls back to TIM5.
bool angleClockArmDwell(int cylinderIndex, float targetAngle, action_s action,
                         float callerPhase, float callerNextPhase);

// Arm the spark-fire channel for `cylinderIndex` on TMR4 (16-bit).
// Falls back at delays > ~8 ms (< ~200 rpm on 60-2, 2-tooth lead).
bool angleClockArmSpark(int cylinderIndex, float targetAngle, action_s action,
                         float callerPhase, float callerNextPhase);

// Arm the spark-fire channel for `cylinderIndex` on TMR4 directly from an
// NT timestamp + delay. Used by turnSparkPinHighStartCharging to guarantee
// the spark fires AFTER the coil is charged, regardless of the rpm basis
// used when the dwell was armed. The targetAngle sentinel (780 deg) makes
// the per-tooth refresh skip this channel (780 > cycleDeg + MAX_LEAD_DEG).
// Returns false (caller falls back to TIM5) when the delay is too small.
bool angleClockArmSparkFromNow(int cylinderIndex, efitick_t nowNt,
                                uint32_t delayNt, action_s action);

// Arm the injection-start channel for `cylinderIndex` on TMR3 (16-bit).
bool angleClockArmInjection(int cylinderIndex, float targetAngle, action_s action,
                              float callerPhase, float callerNextPhase);

// Re-anchor all armed channels from the freshest tooth data. Due events
// (angle already passed) are armed for immediate ISR firing - both dwell
// and spark/injection, matching the time-based build's due-tooth behavior.
// Call at the end of every trigger handoff (including rpm==0 flap teeth,
// before the rpm gate return).
void angleClockRefresh();

// Cancel one channel by cylinder index. Safe when the channel is free.
void angleClockCancelDwell(int cylinderIndex);
void angleClockCancelSpark(int cylinderIndex);
void angleClockCancelInjection(int cylinderIndex);

// Cancel every channel on every timer. Not called from hot paths.
void angleClockCancelAll();

// --- Telemetry ---
uint32_t angleClockFiredDwell();
uint32_t angleClockFiredSpark();
uint32_t angleClockFiredInj();
uint32_t angleClockLateArmDwell();
uint32_t angleClockLateArmSpark();
uint32_t angleClockLateArmInj();
uint32_t angleClockMaxLateTicks();
uint32_t angleClockImmediateFireCount();

// Init-time measurement (printed in lockstats for fuse-incident detection).
uint32_t angleClockInitAcDelta();
uint32_t angleClockInitNtDelta();
uint32_t angleClockInitPsc();

void angleClockResetStats();

#endif // EFI_ANGLE_CLOCK
