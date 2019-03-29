/**
 * @file	signal_executor.h
 * @brief   Asynchronous output signal header
 *
 * @date Feb 10, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef SIGNAL_EXECUTOR_H_
#define SIGNAL_EXECUTOR_H_

#include "global.h"
#include "scheduler.h"
#include "efi_gpio.h"

#if EFI_SIGNAL_EXECUTOR_SLEEP
#include "signal_executor_sleep.h"
#endif /* EFI_SIGNAL_EXECUTOR_SLEEP */

#define MAX_WIRES_COUNT 2

class InjectionEvent;

class InjectionSignalPair {
public:
	InjectionSignalPair();
	scheduling_s signalTimerUp;
	scheduling_s signalTimerDown;

	/**
	 * we need atomic flag so that we do not schedule a new pair of up/down before previous down was executed.
	 *
	 * That's because we want to be sure that no 'down' side callback would be ignored since we are counting to see
	 * overlaps so we need the end counter to always have zero.
	 * TODO: make watchdog decrement relevant counter
	 */
	bool isScheduled;

	InjectorOutputPin *outputs[MAX_WIRES_COUNT];

	InjectionEvent *event;
};

void initSignalExecutor(void);

void initSignalExecutorImpl(void);

#endif /* SIGNAL_EXECUTOR_H_ */
