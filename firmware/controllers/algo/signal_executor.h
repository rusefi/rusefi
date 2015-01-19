/**
 * @file	signal_executor.h
 * @brief   Asynchronous output signal header
 *
 * @date Feb 10, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
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

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void scheduleOutput(OutputSignal *signal, float delayMs, float durationMs);

void initSignalExecutor(void);
void scheduleByAngle(int rpm, scheduling_s *timer, angle_t angle, schfunc_t callback, void *param);

#ifdef __cplusplus
}
#endif /* __cplusplus */

void initSignalExecutorImpl(void);

#endif /* SPARKOUT_H_ */
