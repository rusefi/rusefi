//
// Created by FDSoftware on 21/04/25.
//

#include "pch.h"
#include "thread_priority.h"
#include "main_loop.h"
#include "main_loop_controller.h"
#include "electronic_throttle.h"

MainLoop::MainLoop()
	: PeriodicController("MainLoop", PRIO_MAIN_LOOP, MAIN_LOOP_RATE)
{
}

template <LoopPeriod TFlag>
LoopPeriod MainLoop::makePeriodFlag() const {
	if (m_cycleCounter % loopCounts<TFlag>() == 0) {
		return TFlag;
	} else {
		return LoopPeriod::None;
	}
}

LoopPeriod MainLoop::makePeriodFlags() {
	if (m_cycleCounter >= MAIN_LOOP_RATE) {
		m_cycleCounter = 0;
	}

	LoopPeriod lp = LoopPeriod::None;
	lp |= makePeriodFlag<LoopPeriod::Period1000hz>();
	lp |= makePeriodFlag<LoopPeriod::Period500hz>();
	lp |= makePeriodFlag<LoopPeriod::Period250hz>();
	lp |= makePeriodFlag<LoopPeriod::Period200hz>();
	lp |= makePeriodFlag<LoopPeriod::Period20hz>();

	m_cycleCounter++;

	return lp;
}

void MainLoop::PeriodicTask(efitick_t nowNt) {
	ScopePerf perf(PE::MainLoop);

	LoopPeriod currentLoopPeriod = makePeriodFlags();

#if HAL_USE_ADC
	if (currentLoopPeriod & ADC_UPDATE_RATE) {
		updateSlowAdc(nowNt);
	}
#endif // HAL_USE_ADC

#if EFI_ELECTRONIC_THROTTLE_BODY
	if (currentLoopPeriod & ETB_UPDATE_RATE) {
		for (int i = 0 ; i < ETB_COUNT; i++) {
			auto etb = engine->etbControllers[i];

			if (etb) {
				etb->update();
			}
		}
	}
#endif // EFI_ELECTRONIC_THROTTLE_BODY

	if (currentLoopPeriod & SLOW_CALLBACK_RATE) {
		doPeriodicSlowCallback();
	}

	if (currentLoopPeriod & FAST_CALLBACK_RATE) {
		engine->periodicFastCallback();
	}
}

void initMainLoop() {
	// see test_periodic_thread_controller.cpp for explanation
#if ! EFI_UNIT_TEST
	mainLoop.start();
#endif
}
