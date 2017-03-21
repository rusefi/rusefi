/*
 * @file    rfiutil.h
 * @brief   Number to string conversion header
 *
 * @date Nov 15, 2012
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef RFIUTIL_H_
#define RFIUTIL_H_

#include "global.h"

#if !EFI_UNIT_TEST

#include "histogram.h"

extern cnt_t dbg_lock_cnt;
extern cnt_t dbg_isr_cnt;
#define isLocked() (dbg_lock_cnt > 0)

	/**
	 * Unfortunately ChibiOS has two versions of methods for different
	 * contexts.
	 */

#define isIsrContext() (dbg_isr_cnt > 0)

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

//char hexC(int v);
void chVTSetAny(virtual_timer_t *vtp, systime_t time, vtfunc_t vtfunc, void *par);

#ifdef __cplusplus
}
void printHistogram(Logging *logging, histogram_s *histogram);
#endif /* __cplusplus */

#endif /* EFI_UNIT_TEST */

#endif /* RFIUTIL_H_ */

