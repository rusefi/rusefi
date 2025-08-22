//
// Created by FDSoftware on 21/04/25.
//

#include "pch.h"
#include "thread_priority.h"
#include "main_loop.h"
#include "main_loop_controller.h"

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
		adcInputsUpdateSubscribers(nowNt);
	}
#endif // HAL_USE_ADC

#if EFI_ELECTRONIC_THROTTLE_BODY
	if (currentLoopPeriod & ETB_UPDATE_RATE) {
		// TODO: main_loop etb callback
	}
#endif // EFI_ELECTRONIC_THROTTLE_BODY

	if (currentLoopPeriod & SLOW_CALLBACK_RATE) {
		//TODO: main_loop slow callback
	}

	if (currentLoopPeriod & FAST_CALLBACK_RATE) {
	   //TODO: main_loop fast callback
	}
}

void initMainLoop() {
	// see test_periodic_thread_controller.cpp for explanation
#if ! EFI_UNIT_TEST
	mainLoop.start();
#endif
}
