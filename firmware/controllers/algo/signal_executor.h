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

/**
 * @brief   Asynchronous output signal data structure
 */
typedef struct OutputSignal_struct OutputSignal;
struct OutputSignal_struct {
	NamedOutputPin *output;

	/**
	 * We are alternating instances so that events which extend into next revolution are not reused while
	 * scheduling next revolution events
	 */
	scheduling_s signalTimerUp[2];
	scheduling_s signalTimerDown[2];
};

void scheduleOutput(OutputSignal *signal, efitimeus_t nowUs, float delayUs, float durationUs);
void initSignalExecutor(void);

void initSignalExecutorImpl(void);

#endif /* SPARKOUT_H_ */
