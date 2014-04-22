/**
 * @file	signal_executor_single_timer_algo.h
 *
 * @date Nov 28, 2013
 * @author Andrey Belomutskiy, (c) 2012-2013
 */

#ifndef SIGNAL_EXECUTOR_SINGLE_TIMER_ALGO_H_
#define SIGNAL_EXECUTOR_SINGLE_TIMER_ALGO_H_

#include "signal_executor.h"

#define GET_DURATION(o) ((o)->status ? (o)->signalTimerDown.moment : (o)->signalTimerUp.moment)

inline void registerSignal(OutputSignal *signal);
inline time_t toggleSignalIfNeeded(OutputSignal *out, time_t now);

#endif /* SIGNAL_EXECUTOR_SINGLE_TIMER_ALGO_H_ */
