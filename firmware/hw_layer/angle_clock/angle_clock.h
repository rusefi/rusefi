/**
 * @file angle_clock.h
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
 * N converts tooth N+1's queued angle events to absolute timer ticks and arms
 * one of four OC channels used as PURE SOFTWARE COMPARATORS (CCxE=0 - no
 * output pin, CCxIF fires at CNT == CCR). The TMR2 ISR (priority 3, the same
 * level as the TIM5 executor) then just executes the action. A late handoff
 * only misses the arming of the tooth it spans - those events fall back to
 * the time-based TIM5 path.
 *
 * TMR2 runs free at 4 MHz (TIMCLK1 = 288 MHz, PSC = 71) - the same tick as
 * the NT domain (TIM5), so NT timestamps convert to angle-clock ticks with a
 * constant offset measured at init. The counter is never reset per tooth:
 * events are armed as ABSOLUTE counter values (epoch of the next tooth
 * predicted from the current edge + the angle offset), which needs no writes
 * from the priority-0 EXTI fast IRQ and cannot fire spuriously in the
 * current tooth. The 32-bit wrap (~1073 s) is absorbed by unsigned tick
 * arithmetic.
 *
 * The fallback contract: if the armed tick is already in the past (handoff
 * ran late) or all four channels are busy, angleClockArm() returns false and
 * the caller schedules the event on the time-based executor as before.
 */

#pragma once

#include "scheduler.h"
#include "efitime.h"

// Pure tick math, host-testable: convert "angleFromNow degrees until the
// event" and "oneDegreeUs microseconds per degree" into a delay in
// angle-clock ticks (4 MHz, == NT ticks on the target).
static inline uint32_t angleClockDelayTicks(float angleFromNow, float oneDegreeUs) {
	float delayUs = angleFromNow * oneDegreeUs;
	return static_cast<uint32_t>(USF2NT(delayUs));
}

#if EFI_ANGLE_CLOCK

// Configure TMR2 (RCC, PSC, compare channels, NVIC) and measure the
// NT<->angle-clock offset. Call once at boot, after the TIM5 executor is
// running (both are free-running 4 MHz counters on the same TIMCLK1).
void initAngleClock();

// Convert an NT-domain timestamp into an absolute angle-clock tick.
uint32_t angleClockTickForNt(efitick_t nt);

// Current free-running angle-clock counter value.
uint32_t angleClockNow();

// Arm `action` to fire when the counter reaches `atTick`. Returns false
// (without arming) when `atTick` is not far enough in the future or all
// four channels are busy - the caller must fall back to the TIM5 path.
bool angleClockArm(uint32_t atTick, action_s action);

// Cancel every armed channel whose action matches (callback + argument).
// Safe to call when nothing matching is armed.
void angleClockCancel(action_s action);

// Telemetry for lockstats.
uint32_t angleClockFiredCount();
uint32_t angleClockArmFailCount();
uint32_t angleClockProgrammedLateCount();
uint32_t angleClockMaxLateTicks();
void angleClockResetStats();

#endif // EFI_ANGLE_CLOCK
