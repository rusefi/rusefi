/*
 * @file engine_controller_misc.cpp
 *
 * @date Mar 22, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

static uint8_t nextThreadId = 0;
void threadInitHook(void* vtp) {
	// No lock required, this is already under lock
	auto tp = reinterpret_cast<thread_t*>(vtp);
	tp->threadId = ++nextThreadId;
}

#if ENABLE_PERF_TRACE
void irqEnterHook() {
	perfEventBegin(PE::ISR);
}

void irqExitHook() {
	perfEventEnd(PE::ISR);
}

void contextSwitchHook() {
	perfEventInstantGlobal(PE::ContextSwitch);
}

#else
void irqEnterHook() {}
void irqExitHook() {}
void contextSwitchHook() {}
#endif /* ENABLE_PERF_TRACE */
