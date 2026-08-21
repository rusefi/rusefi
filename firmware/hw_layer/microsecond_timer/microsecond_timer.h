/**
 * @file	microsecond_timer.h
 *
 * @date Apr 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

void initMicrosecondTimer();
void setHardwareSchedulerTimer(efitick_t nowNt, efitick_t setTimeNt);

// scheduler ISR duration histogram diagnostics (isr_duration_histogram.h)
void resetSchedulerIsrHistogram();
void printSchedulerIsrHistogram();

#define TOO_FAR_INTO_FUTURE_MS (10 * MS_PER_SECOND)
#define TOO_FAR_INTO_FUTURE_US MS2US(TOO_FAR_INTO_FUTURE_MS)
#define TOO_FAR_INTO_FUTURE_NT US2NT(TOO_FAR_INTO_FUTURE_US)
