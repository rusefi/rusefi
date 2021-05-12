/*
    ChibiOS - Copyright (C) 2014 Derek Mulcahy

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    PITv1/hal_gpt_lld.c
 * @brief   CYPRESS GPT subsystem low level driver source.
 * @author  andreika <prometheus.pcb@gmail.com>
 *
 * @addtogroup GPT
 * @{
 */

#include <string.h>
#include "hal.h"

#if HAL_USE_GPT || defined(__DOXYGEN__)

#include "pdl.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

// todo: fix 32-bit timer
#define USE_16BIT_TIMER
#define BT_16BIT_MAX_COUNT_VALUE 60000	// should be < 0xffff to fit the 16-bit register

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   GPTD1 driver identifier.
 * @note    The driver GPTD1 allocates the complex timer PIT0 when enabled.
 */
#if CYPRESS_GPT_USE_BT0_BT1 || defined(__DOXYGEN__)
GPTDriver GPTD1;
#endif

/**
 * @brief   GPTD2 driver identifier.
 * @note    The driver GPTD2 allocates the timer PIT1 when enabled.
 */
#if CYPRESS_GPT_USE_BT2_BT3 || defined(__DOXYGEN__)
GPTDriver GPTD2;
#endif

/**
 * @brief   GPTD3 driver identifier.
 * @note    The driver GPTD3 allocates the timer PIT2 when enabled.
 */
#if CYPRESS_GPT_USE_BT4_BT5 || defined(__DOXYGEN__)
GPTDriver GPTD3;
#endif

/**
 * @brief   GPTD4 driver identifier.
 * @note    The driver GPTD4 allocates the timer PIT3 when enabled.
 */
#if CYPRESS_GPT_USE_BT6_BT7 || defined(__DOXYGEN__)
GPTDriver GPTD4;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

// [andreika]: IRQ handlers declared inside interrupts_fm4_type_b.c 

/**
 * @brief   Shared IRQ handler.
 *
 * @param[in] gptp      pointer to a @p GPTDriver object
 */
 
static void gpt_lld_serve_interrupt(GPTDriver *gptp) {
#ifdef USE_16BIT_TIMER
	gptp->counter -= BT_16BIT_MAX_COUNT_VALUE;
	if (gptp->counter > 0) {
		// this is the last iteration, set the leftover
		if (gptp->counter < BT_16BIT_MAX_COUNT_VALUE) {
			// a little accuracy loss here, but it's ok 
			// (measured ~1ms drift on 1 minute interval, it's < 0.002%)
			// todo: use Bt_Rt_ReadCurCnt()?
			Bt_Rt_WriteCycleVal(gptp->bt, (uint16_t)gptp->counter);
			// restart the timer
			Bt_Rt_EnableSwTrig(gptp->bt);
		}
		return;
	} else {
		gptp->counter = 0;
	}
#endif /* USE_16BIT_TIMER */

  if (gptp->state == GPT_ONESHOT) {
    gptp->state = GPT_READY;                /* Back in GPT_READY state.     */
    gpt_lld_stop_timer(gptp);               /* Timer automatically stopped. */
  }
  gptp->config->callback(gptp);
}

#if CYPRESS_GPT_USE_BT0_BT1
static void gpt_lld_serve_interrupt0(void) {
	gpt_lld_serve_interrupt(&GPTD1);
}
#endif /* CYPRESS_GPT_USE_BT0 */

#if CYPRESS_GPT_USE_BT2_BT3
static void gpt_lld_serve_interrupt1(void) {
	gpt_lld_serve_interrupt(&GPTD2);
}
#endif /* CYPRESS_GPT_USE_BT1 */

#if CYPRESS_GPT_USE_BT4_BT5
static void gpt_lld_serve_interrupt2(void) {
	gpt_lld_serve_interrupt(&GPTD3);
}
#endif /* CYPRESS_GPT_USE_BT2 */

#if CYPRESS_GPT_USE_BT6_BT7
static void gpt_lld_serve_interrupt3(void) {
	gpt_lld_serve_interrupt(&GPTD4);
}
#endif /* CYPRESS_GPT_USE_BT3 */


/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level GPT driver initialization.
 *
 * @notapi
 */
void gpt_lld_init(void) {

#if CYPRESS_GPT_USE_BT0_BT1
  /* Driver initialization.*/
  gptObjectInit(&GPTD1);
  GPTD1.bt = &BT0;
  GPTD1.bt2 = &BT1;	// needed for 32-bit mode
  GPTD1.serve_interrupt = gpt_lld_serve_interrupt0;
#endif

#if CYPRESS_GPT_USE_BT2_BT3
  /* Driver initialization.*/
  gptObjectInit(&GPTD2);
  GPTD2.bt = &BT2;
  GPTD1.bt2 = &BT3;	// needed for 32-bit mode
  GPTD2.serve_interrupt = gpt_lld_serve_interrupt1;
#endif

#if CYPRESS_GPT_USE_BT4_BT5
  /* Driver initialization.*/
  gptObjectInit(&GPTD3);
  GPTD3.bt = &BT4;
  GPTD1.bt2 = &BT5;	// needed for 32-bit mode
  GPTD3.serve_interrupt = gpt_lld_serve_interrupt2;
#endif

#if CYPRESS_GPT_USE_BT6_BT7
  /* Driver initialization.*/
  gptObjectInit(&GPTD4);
  GPTD4.bt = &BT6;
  GPTD1.bt2 = &BT7;	// needed for 32-bit mode
  GPTD4.serve_interrupt = gpt_lld_serve_interrupt3;
#endif
}

/**
 * @brief   Configures and activates the GPT peripheral.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 *
 * @notapi
 */
void gpt_lld_start(GPTDriver *gptp) {
	en_result_t res;
  
	// [andreika]: these two should match:
	en_rt_clock_pres_t psc = RtPres1Div16;
	uint32_t prescaler = 16;

	if (gptp->state != GPT_STOP)
		return;
	gptp->clock = CYPRESS_PCLK1_FREQENCY / prescaler;

	// Check if the prescaler is correct and frequency is supported
	osalDbgAssert((uint32_t)(gptp->clock / gptp->config->frequency) * gptp->config->frequency == gptp->clock, "BT: invalid frequency or prescaler");

    // Set BT IO mode  
    Bt_ConfigIOMode(gptp->bt, BtIoMode0);
    
    // Initialize BT
    memset(&gptp->stcRtConfig, 0, sizeof(gptp->stcRtConfig));
    gptp->stcRtConfig.enPres = psc;
#ifdef USE_16BIT_TIMER
    gptp->stcRtConfig.enSize = RtSize16Bit;
#else
    gptp->stcRtConfig.enSize = RtSize32Bit;	// we use dual 16-bit timers (odd+even) for 32-bit mode
#endif

    // We don't know what kind of timer would be needed.
    // So we always use a continuous 'Reload' mode, and emulate 'RtOneshot' mode by stopping it manually inside the IRQ handler
    gptp->stcRtConfig.enMode = RtReload;
    
    gptp->stcRtConfig.enOutputPolarity = RtPolarityLow;
    gptp->stcRtConfig.enExtTrig = RtExtTiggerDisable;
    gptp->stcRtConfig.bTouchNvic = TRUE;
    
    // BT timer is a count-down timer so we use "Underflow" interrupts (when the counter reaches 0)
    memset(&gptp->stcRtIrqEn, 0, sizeof(gptp->stcRtIrqEn));
    gptp->stcRtIrqEn.bRtUnderflowIrq = TRUE;
    memset(&gptp->stcRtIrqCb, 0, sizeof(gptp->stcRtIrqCb));
    gptp->stcRtIrqCb.pfnRtUnderflowIrqCb = gptp->serve_interrupt;

    gptp->stcRtConfig.pstcRtIrqEn = &gptp->stcRtIrqEn;
    gptp->stcRtConfig.pstcRtIrqCb = &gptp->stcRtIrqCb;
    
    res = Bt_Rt_Init(gptp->bt, &gptp->stcRtConfig);
    osalDbgAssert(res == Ok, "Bt_Rt_Init failed");
}

/**
 * @brief   Deactivates the GPT peripheral.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 *
 * @notapi
 */
void gpt_lld_stop(GPTDriver *gptp) {

  if (gptp->state == GPT_READY) {
  	Bt_Rt_DeInit(gptp->bt, TRUE);
  }
}

/**
 * @brief   Starts the timer in continuous mode.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 * @param[in] interval  period in ticks
 *
 * @notapi
 */
void gpt_lld_start_timer(GPTDriver *gptp, gptcnt_t interval) {
	gpt_lld_change_interval(gptp, interval);
	Bt_Rt_EnableCount(gptp->bt);
    Bt_Rt_EnableSwTrig(gptp->bt);
}

/**
 * @brief   Stops the timer.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 *
 * @notapi
 */
void gpt_lld_stop_timer(GPTDriver *gptp) {
	Bt_Rt_DisableCount(gptp->bt);
	gptp->counter = 0;
}

/**
 * @brief   Starts the timer in one shot mode and waits for completion.
 * @details This function specifically polls the timer waiting for completion
 *          in order to not have extra delays caused by interrupt servicing,
 *          this function is only recommended for short delays.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 * @param[in] interval  time interval in ticks
 *
 * @notapi
 */
void gpt_lld_polled_delay(GPTDriver *gptp, gptcnt_t interval) {
	// todo: is it correct? not tested!
	gpt_lld_start_timer(gptp, interval);
	// wait
	while (Bt_Rt_GetIrqFlag(gptp->bt, RtUnderflowIrq) != PdlSet) {
    	Bt_Rt_ClrIrqFlag(gptp->bt, RtUnderflowIrq);
    	break;
    }
    gpt_lld_stop_timer(gptp);
}

void gpt_lld_change_interval(GPTDriver *gptp, gptcnt_t interval) {
	if (interval == 0)
		return;
	// we multiply the interval because the timer clock is N times faster than the user-set frequency
	int64_t val = (int32_t)((gptp->clock / gptp->config->frequency) * interval - 1);
#ifdef USE_16BIT_TIMER
	gptp->counter = val;
	if (val > BT_16BIT_MAX_COUNT_VALUE) {
		val = BT_16BIT_MAX_COUNT_VALUE;
	} 
#else
	// first, set HIWORD(val) to the odd channel
	// todo: it seems it doesn't work? :(
	Bt_Rt_WriteCycleVal(gptp->bt2, (uint16_t)(val >> 16));
#endif
	// then, set LOWORD(val) to the even channel
	Bt_Rt_WriteCycleVal(gptp->bt, (uint16_t)(val & 0xffff));
}

gptcnt_t gpt_lld_get_interval(GPTDriver *gptp) {
	// todo: test if it works
	uint32_t loword = Bt_Rt_ReadCurCnt(gptp->bt);
	uint32_t hiword = Bt_Rt_ReadCurCnt(gptp->bt2);
	uint64_t val = (hiword << 16) | loword;
	return (uint32_t)((val * gptp->config->frequency) / ((uint32_t)gptp->clock) + 1);
} 

#endif /* HAL_USE_GPT */

/** @} */
