/**
 * @file	signal_executor.h
 * @brief   Asynchronous output signal header
 *
 * @date Feb 10, 2013
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#ifndef SPARKOUT_H_
#define SPARKOUT_H_

#include "rusefi_enums.h"
#include "global.h"
#include "efifeatures.h"
#include "io_pins.h"
#include "scheduler.h"
#include "efiGpio.h"

#if EFI_PROD_CODE
#include "datalogging.h"
#endif /* EFI_PROD_CODE */

#if EFI_SIGNAL_EXECUTOR_SLEEP
#include "signal_executor_sleep.h"
#endif /* EFI_SIGNAL_EXECUTOR_SLEEP */

class OutputSignalPair {
public:
	OutputSignalPair();
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
	InjectorOutputPin *output;
};

/**
 * @brief   Asynchronous output signal data structure
 */
typedef struct OutputSignal_struct OutputSignal;
struct OutputSignal_struct {

	/**
	 * We are alternating instances so that events which extend into next revolution are not reused while
	 * scheduling next revolution events
	 */
	scheduling_s signalTimerUp[2];
	scheduling_s signalTimerDown[2];
};

void seScheduleByTime(const char *prefix, scheduling_s *scheduling, efitimeus_t time, schfunc_t callback, NamedOutputPin *param);
void initSignalExecutor(void);
void initEnginePinsNames(void);

void initSignalExecutorImpl(void);

#endif /* SPARKOUT_H_ */
