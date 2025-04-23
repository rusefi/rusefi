//
// Created by FDSoftware on 23/04/25.
// Note: keep this controller separated from main_loop.h,
// otherwise a circular dependency is generated between pch.h / mpu_watchdog.h / mpu_util.h
//
#pragma once
#include "pch.h"
#include "periodic_thread_controller.h"
#include "main_loop.h"

template <LoopPeriod flag>
static constexpr int loopCounts() {
    constexpr auto hz = hzForPeriod(flag);

    // check that this cleanly divides
    static_assert(MAIN_LOOP_RATE % hz == 0);

    return MAIN_LOOP_RATE / hz;
}

class MainLoop : public PeriodicController<1024> {
public:
    MainLoop();
    void PeriodicTask(efitick_t nowNt) override;

#if ! EFI_UNIT_TEST
private:
#endif

    template <LoopPeriod TFlag>
    LoopPeriod makePeriodFlag() const;

    LoopPeriod makePeriodFlags();

    int m_cycleCounter = 0;
};

static MainLoop mainLoop CCM_OPTIONAL;