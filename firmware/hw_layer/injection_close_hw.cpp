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

struct InjCloseSlot {
    efitick_t  fireAt;
    action_s   action;
    bool       pending = false;
};

static InjCloseSlot s_injClose[4];

// Re-arm CC2 to the earliest pending slot.
// Must be called from prio-3 ISR context (no lock needed: all callers are prio 3).
static void rearmCC2() {
    bool found = false;
    uint32_t earliest = 0;

    for (const auto& slot : s_injClose) {
        if (!slot.pending) continue;
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
        if (!slot.pending) continue;
        // Signed comparison so past-due events (CNT > fireAt) also fire.
        if (static_cast<int32_t>(now - static_cast<uint32_t>(slot.fireAt)) >= 0) {
            slot.pending = false;
            action_s act = slot.action;
            slot.action = {};
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

    dispatchAndRearm();

    assertInterruptPriority(__func__, EFI_IRQ_SCHEDULING_TIMER_PRIORITY);
}

void scheduleInjectionCloseHW(int cyl, efitick_t nowNt,
                                uint32_t delayNt, action_s action) {
    if (cyl < 0 || cyl >= 4 || !delayNt) {
        return;
    }

    s_injClose[cyl].fireAt  = sumTickAndFloat(nowNt, static_cast<float>(delayNt));
    s_injClose[cyl].action  = action;
    s_injClose[cyl].pending = true;

    rearmCC2();
}

void cancelInjectionCloseHW(int cyl) {
    if (cyl < 0 || cyl >= 4) {
        return;
    }
    s_injClose[cyl].pending = false;
    s_injClose[cyl].action  = {};
    rearmCC2();
}

#endif // EFI_PROD_CODE && HAL_USE_PWM
