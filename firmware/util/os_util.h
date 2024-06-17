/*
 * @file    rfiutil.h
 * @brief   Number to string conversion header
 *
 * @date Nov 15, 2012
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"

#if !EFI_UNIT_TEST

#include "histogram.h"

#ifdef _CHIBIOS_RT_CONF_VER_6_1_
	#define ch0 ch
#endif

/**
 * Unfortunately ChibiOS has two versions of methods for different
 * contexts.
 */
#define isLocked() (ch0.dbg.lock_cnt > 0)
#define isIsrContext() (ch0.dbg.isr_cnt > 0)

#define assertIsrContext(code) efiAssertVoid(code, isIsrContext(), "NOT_ISR")

void chVTSetAny(virtual_timer_t *vtp, systime_t time, vtfunc_t vtfunc, void *par);

void printHistogram(Logging *logging, histogram_s *histogram);

#endif /* EFI_UNIT_TEST */
