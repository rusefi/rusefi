/**
 * @file injection_close_hw.cpp
 *
 * TIM5 CC2 mini-queue for injection close events.
 * See injection_close_hw.h for architecture notes.
 */

#include "pch.h"

#if EFI_PROD_CODE && HAL_USE_PWM

#include "injection_close_hw.h"
#include "port_microsecond_timer.h"

// CC2G bit in EGR: force CC2 compare event (same pattern as CC1G in the executor).
// ChibiOS defines CC1G but not CC2G; define it here.
#ifndef STM32_TIM_EGR_CC2G
#  define STM32_TIM_EGR_CC2G   (1U << 2)
#endif

// Minimum ticks to arm CCR2 into the future (same margin as CC1 in the executor).
static constexpr uint32_t INJ_CC2_MARGIN_TICKS = US2NT(4);

struct InjCloseSlot {
    efitick_t  fireAt;          // absolute NT time for the LATEST pending close
    action_s   action;          // turnInjectionPinLow(event)
    uint8_t    pendingCount = 0; // 0=free, 1=one close pending, 2+=overlap
};

static InjCloseSlot s_injClose[4];
static uint32_t s_cc2FiredCount = 0;
static uint32_t s_cc2ScheduledCount = 0;

uint32_t getInjectionCC2FiredCount()     { return s_cc2FiredCount; }
uint32_t getInjectionCC2ScheduledCount() { return s_cc2ScheduledCount; }

// Re-arm CC2 to the earliest pending slot.
// Must be called from prio-3 ISR context (no lock needed: all callers are prio 3).
static void rearmCC2() {
    bool found = false;
    uint32_t earliest = 0;

    for (const auto& slot : s_injClose) {
        if (!slot.pendingCount) continue;
        const uint32_t t = static_cast<uint32_t>(slot.fireAt);
        if (!found || static_cast<int32_t>(t - earliest) < 0) {
            earliest = t;
            found = true;
        }
    }

    if (found) {
        // Disable CC2IE before writing CCR2 to avoid spurious fire mid-update.
        pwmDisableChannelNotificationI(&SCHEDULER_PWM_DEVICE, 1);
        // Clear any stale CC2IF from the previous fire.
        SCHEDULER_TIMER_DEVICE->SR = ~STM32_TIM_SR_CC2IF;
        // Set compare value.  NT == TIM5->CNT so no offset conversion needed.
        SCHEDULER_TIMER_DEVICE->CCR2 = earliest;
        // Re-enable CC2IE.
        pwmEnableChannelNotificationI(&SCHEDULER_PWM_DEVICE, 1);
        // If the target already passed during the update window, force the
        // interrupt via EGR (same pattern as the executor uses for CC1).
        if (static_cast<int32_t>(SCHEDULER_TIMER_DEVICE->CNT - earliest) >= 0) {
            SCHEDULER_TIMER_DEVICE->EGR = STM32_TIM_EGR_CC2G;
        }
    } else {
        pwmDisableChannelNotificationI(&SCHEDULER_PWM_DEVICE, 1);
    }
}

// Dispatch all slots whose fireAt has arrived, then re-arm for the next.
static void dispatchAndRearm() {
    const uint32_t now = SCHEDULER_TIMER_DEVICE->CNT;

    for (auto& slot : s_injClose) {
        if (!slot.pendingCount) continue;
        // Signed comparison so past-due events (CNT > fireAt) also fire.
        if (static_cast<int32_t>(now - static_cast<uint32_t>(slot.fireAt)) >= 0) {
            action_s act = slot.action;
            slot.pendingCount--;

            if (slot.pendingCount == 0) {
                // Last pending close: release slot.
                slot.action = {};
            } else {
                // Overlap: more closes pending (overlappingCounter > 1).
                // Re-arm CC2 immediately so the next close fires right away.
                // turnInjectionPinLow decrements overlappingCounter each call;
                // when it reaches 0 setLow() fires and the injector closes.
                slot.fireAt = static_cast<efitick_t>(SCHEDULER_TIMER_DEVICE->CNT)
                              + static_cast<efitick_t>(INJ_CC2_MARGIN_TICKS);
            }

            if (act) {
                act.execute();
            }
        }
    }

    rearmCC2();
}

void hwInjectionCloseCallback(PWMDriver* /*pwmp*/) {
    // CC2IE was NOT auto-disabled by the LLD (unlike CC1 which the executor
    // manages manually).  Disable it first to prevent re-fires on the same CCR.
    pwmDisableChannelNotificationI(&SCHEDULER_PWM_DEVICE, 1);
    SCHEDULER_TIMER_DEVICE->SR = ~STM32_TIM_SR_CC2IF;
    s_cc2FiredCount++;

    dispatchAndRearm();

    assertInterruptPriority(__func__, EFI_IRQ_SCHEDULING_TIMER_PRIORITY);
}

void scheduleInjectionCloseHW(int cyl, efitick_t nowNt,
                                uint32_t delayNt, action_s action) {
    if (cyl < 0 || cyl >= 4 || !delayNt) {
        return;
    }

    const efitick_t fireAt = sumTickAndFloat(nowNt, static_cast<float>(delayNt));
    s_cc2ScheduledCount++;

    if (s_injClose[cyl].pendingCount == 0) {
        // Normal path: slot free.
        s_injClose[cyl].fireAt  = fireAt;
        s_injClose[cyl].action  = action;
        s_injClose[cyl].pendingCount = 1;
    } else {
        // Overlap: a previous injection hasn't closed yet (overlappingCounter > 1).
        // Increment the counter so dispatchAndRearm fires turnInjectionPinLow
        // an extra time (each call decrements overlappingCounter by 1; when it
        // reaches 0 the pin goes low).
        // Use the LATER fireAt so both closes fire in sequence.
        if (static_cast<int32_t>(
                static_cast<uint32_t>(fireAt) -
                static_cast<uint32_t>(s_injClose[cyl].fireAt)) > 0) {
            s_injClose[cyl].fireAt = fireAt;
        }
        s_injClose[cyl].pendingCount++;
    }

    rearmCC2();
}

void cancelInjectionCloseHW(int cyl) {
    if (cyl < 0 || cyl >= 4) {
        return;
    }
    s_injClose[cyl].pendingCount = 0;
    s_injClose[cyl].action  = {};
    rearmCC2();
}

#endif // EFI_PROD_CODE && HAL_USE_PWM
