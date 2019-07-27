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
 * @brief   KINETIS GPT subsystem low level driver source.
 * @author  andreika <prometheus.pcb@gmail.com>
 *
 * @addtogroup GPT
 * @{
 */

#include "hal.h"

#if HAL_USE_GPT || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   GPTD1 driver identifier.
 * @note    The driver GPTD1 allocates the complex timer PIT0 when enabled.
 */
#if KINETIS_GPT_USE_PIT0 || defined(__DOXYGEN__)
GPTDriver GPTD1;
#endif

/**
 * @brief   GPTD2 driver identifier.
 * @note    The driver GPTD2 allocates the timer PIT1 when enabled.
 */
#if KINETIS_GPT_USE_PIT1 || defined(__DOXYGEN__)
GPTDriver GPTD2;
#endif

/**
 * @brief   GPTD3 driver identifier.
 * @note    The driver GPTD3 allocates the timer PIT2 when enabled.
 */
#if KINETIS_GPT_USE_PIT2 || defined(__DOXYGEN__)
GPTDriver GPTD3;
#endif

/**
 * @brief   GPTD4 driver identifier.
 * @note    The driver GPTD4 allocates the timer PIT3 when enabled.
 */
#if KINETIS_GPT_USE_PIT3 || defined(__DOXYGEN__)
GPTDriver GPTD4;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

#if KINETIS_HAS_PIT_COMMON_IRQ
static uint8_t active_channels = 0;
#endif /* KINETIS_HAS_PIT_COMMON_IRQ */

#ifdef KE1xF
static const uint32_t tif_flags[] = { LPIT_MSR_TIF0(1), LPIT_MSR_TIF1(1), LPIT_MSR_TIF2(1), LPIT_MSR_TIF3(1) };
static const uint32_t mier_flags[] = { LPIT_MIER_TIE0(1), LPIT_MIER_TIE1(1), LPIT_MIER_TIE2(1), LPIT_MIER_TIE3(1) };
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Shared IRQ handler.
 *
 * @param[in] gptp      pointer to a @p GPTDriver object
 */
static void gpt_lld_serve_interrupt(GPTDriver *gptp) {

  /* Clear the interrupt */
#ifdef KE1xF
  PIT->MSR |= tif_flags[gptp->channelIndex];
#else
  gptp->channel->TFLG |= PIT_TFLGn_TIF;
#endif

  if (gptp->state == GPT_ONESHOT) {
    gptp->state = GPT_READY;                /* Back in GPT_READY state.     */
    gpt_lld_stop_timer(gptp);               /* Timer automatically stopped. */
  }
  gptp->config->callback(gptp);
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if !KINETIS_HAS_PIT_COMMON_IRQ

#if KINETIS_GPT_USE_PIT0
/**
 * @brief   PIT1 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(KINETIS_PIT0_IRQ_VECTOR) {
  OSAL_IRQ_PROLOGUE();
  gpt_lld_serve_interrupt(&GPTD1);
  OSAL_IRQ_EPILOGUE();
}
#endif /* KINETIS_GPT_USE_PIT0 */

#if KINETIS_GPT_USE_PIT1
/**
 * @brief   PIT1 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(KINETIS_PIT1_IRQ_VECTOR) {
  OSAL_IRQ_PROLOGUE();
  gpt_lld_serve_interrupt(&GPTD2);
  OSAL_IRQ_EPILOGUE();
}
#endif /* KINETIS_GPT_USE_PIT1 */

#if KINETIS_GPT_USE_PIT2
/**
 * @brief   PIT2 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(KINETIS_PIT2_IRQ_VECTOR) {
  OSAL_IRQ_PROLOGUE();
  gpt_lld_serve_interrupt(&GPTD3);
  OSAL_IRQ_EPILOGUE();
}
#endif /* KINETIS_GPT_USE_PIT2 */

#if KINETIS_GPT_USE_PIT3
/**
 * @brief   PIT3 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(KINETIS_PIT3_IRQ_VECTOR) {
  OSAL_IRQ_PROLOGUE();
  gpt_lld_serve_interrupt(&GPTD4);
  OSAL_IRQ_EPILOGUE();
}
#endif /* KINETIS_GPT_USE_PIT3 */

#else /* !KINETIS_HAS_PIT_COMMON_IRQ */
/**
 * @brief   Common PIT interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(KINETIS_PIT_IRQ_VECTOR) {
  OSAL_IRQ_PROLOGUE();
#if KINETIS_GPT_USE_PIT0
#ifdef KE1xF
  if(PIT->MSR & LPIT_MSR_TIF0(1))
#else
  if(GPTD1.channel->TFLG & PIT_TFLGn_TIF)
#endif
    gpt_lld_serve_interrupt(&GPTD1);
#endif /* KINETIS_GPT_USE_PIT0 */
#if KINETIS_GPT_USE_PIT1
#ifdef KE1xF
  if(PIT->MSR & LPIT_MSR_TIF1(1))
#else
  if(GPTD2.channel->TFLG & PIT_TFLGn_TIF)
#endif
    gpt_lld_serve_interrupt(&GPTD2);
#endif /* KINETIS_GPT_USE_PIT1 */
#if KINETIS_GPT_USE_PIT2
#ifdef KE1xF
  if(PIT->MSR & LPIT_MSR_TIF2(1))
#else
  if(GPTD3.channel->TFLG & PIT_TFLGn_TIF)
#endif
    gpt_lld_serve_interrupt(&GPTD3);
#endif /* KINETIS_GPT_USE_PIT2 */
#if KINETIS_GPT_USE_PIT3
#ifdef KE1xF
  if(PIT->MSR & LPIT_MSR_TIF3(1))
#else
  if(GPTD4.channel->TFLG & PIT_TFLGn_TIF)
#endif
    gpt_lld_serve_interrupt(&GPTD4);
#endif /* KINETIS_GPT_USE_PIT3 */
  OSAL_IRQ_EPILOGUE();
}

#endif /* !KINETIS_HAS_PIT_COMMON_IRQ */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level GPT driver initialization.
 *
 * @notapi
 */
void gpt_lld_init(void) {

#if KINETIS_GPT_USE_PIT0
  /* Driver initialization.*/
#ifdef KE1xF
  GPTD1.channelIndex = 0;
#else
  GPTD1.channel = &PIT->CHANNEL[0];
#endif
  gptObjectInit(&GPTD1);
#endif

#if KINETIS_GPT_USE_PIT1
  /* Driver initialization.*/
#ifdef KE1xF
  GPTD2.channelIndex = 1;
#else
  GPTD2.channel = &PIT->CHANNEL[1];
#endif
  gptObjectInit(&GPTD2);
#endif

#if KINETIS_GPT_USE_PIT2
  /* Driver initialization.*/
#ifdef KE1xF
  GPTD3.channelIndex = 2;
#else
  GPTD3.channel =  &PIT->CHANNEL[2];
#endif
  gptObjectInit(&GPTD3);
#endif

#if KINETIS_GPT_USE_PIT3
  /* Driver initialization.*/
#ifdef KE1xF
  GPTD4.channelIndex = 3;
#else
  GPTD4.channel =  &PIT->CHANNEL[3];
#endif
  gptObjectInit(&GPTD4);
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
  uint16_t psc;

  if (gptp->state == GPT_STOP) {
#ifdef KE1xF
    /* Clock activation.*/
    PCC->CLKCFG[PCC_LPIT0_INDEX] |= PCC_CLKCFG_CGC(1);
    //SCG->FIRCDIV = SCG_FIRCDIV_FIRCDIV2(1) | SCG_FIRCDIV_FIRCDIV1(1);
    PCC->CLKCFG[PCC_LPIT0_INDEX] = (PCC->CLKCFG[PCC_LPIT0_INDEX] & ~PCC_CLKCFG_PCS_MASK) | PCC_CLKCFG_PCS(6/*SCGPCLK System PLL clock*/);
#else
    /* Clock activation.*/
    SIM->SCGC6 |= SIM_SCGC6_PIT;
#endif
    gptp->clock = KINETIS_SYSCLK_FREQUENCY;

#if !KINETIS_HAS_PIT_COMMON_IRQ

#if KINETIS_GPT_USE_PIT0
    if (&GPTD1 == gptp) {
      nvicEnableVector(PITChannel0_IRQn, KINETIS_GPT_PIT0_IRQ_PRIORITY);
    }
#endif
#if KINETIS_GPT_USE_PIT1
    if (&GPTD2 == gptp) {
      nvicEnableVector(PITChannel1_IRQn, KINETIS_GPT_PIT1_IRQ_PRIORITY);
    }
#endif
#if KINETIS_GPT_USE_PIT2
    if (&GPTD3 == gptp) {
      nvicEnableVector(PITChannel2_IRQn, KINETIS_GPT_PIT2_IRQ_PRIORITY);
    }
#endif
#if KINETIS_GPT_USE_PIT3
    if (&GPTD4 == gptp) {
      nvicEnableVector(PITChannel3_IRQn, KINETIS_GPT_PIT3_IRQ_PRIORITY);
    }
#endif

#else /* !KINETIS_HAS_PIT_COMMON_IRQ */
    nvicEnableVector(PIT_IRQn, KINETIS_GPT_PIT_IRQ_PRIORITY);
    active_channels++;
#endif /* !KINETIS_HAS_PIT_COMMON_IRQ */
  }

  /* Prescaler value calculation.*/
  psc = (uint16_t)((gptp->clock / gptp->config->frequency) - 1);
  osalDbgAssert(((uint32_t)(psc + 1) * gptp->config->frequency) == gptp->clock,
                "invalid frequency");

#ifdef KE1xF
  /* Enable the PIT */
  PIT->MCR |= LPIT_MCR_M_CEN(1);
#else
  /* Enable the PIT */
  PIT->MCR = 0;
#endif
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
#ifdef KE1xF
	PCC->CLKCFG[PCC_LPIT0_INDEX] &= ~PCC_CLKCFG_CGC(1);

    /* Disable the channel */
    PIT->CHANNEL[gptp->channelIndex].TCTRL = 0;
    
    /* Disable interrupts */
    PIT->MIER &= ~mier_flags[gptp->channelIndex];

    /* Clear pending interrupts */
    PIT->MSR |= tif_flags[gptp->channelIndex];
#else
    SIM->SCGC6 &= ~SIM_SCGC6_PIT;

    /* Disable the channel */
    gptp->channel->TCTRL = 0;

    /* Clear pending interrupts */
    gptp->channel->TFLG |= PIT_TFLGn_TIF;
#endif

#if !KINETIS_HAS_PIT_COMMON_IRQ

#if KINETIS_GPT_USE_PIT0
    if (&GPTD1 == gptp) {
      nvicDisableVector(PITChannel0_IRQn);
    }
#endif
#if KINETIS_GPT_USE_PIT1
    if (&GPTD2 == gptp) {
      nvicDisableVector(PITChannel1_IRQn);
    }
#endif
#if KINETIS_GPT_USE_PIT2
    if (&GPTD3 == gptp) {
      nvicDisableVector(PITChannel2_IRQn);
    }
#endif
#if KINETIS_GPT_USE_PIT3
    if (&GPTD4 == gptp) {
      nvicDisableVector(PITChannel3_IRQn);
    }
#endif

#else /* !KINETIS_HAS_PIT_COMMON_IRQ */
    if(--active_channels == 0)
      nvicDisableVector(PIT_IRQn);
#endif /* !KINETIS_HAS_PIT_COMMON_IRQ */
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
#ifdef KE1xF
  /* Clear pending interrupts */
  PIT->MSR |= tif_flags[gptp->channelIndex];

  /* Set the interval */
  gpt_lld_change_interval(gptp, interval);
  
  /* Enable interrupts */
  PIT->MIER |= mier_flags[gptp->channelIndex];
  /* Start the timer */
  PIT->CHANNEL[gptp->channelIndex].TCTRL |= LPIT_TCTRL_T_EN(1);
#else
  /* Clear pending interrupts */
  gptp->channel->TFLG |= PIT_TFLGn_TIF;

  /* Set the interval */
  gpt_lld_change_interval(gptp, interval);
  
  /* Start the timer */
  gptp->channel->TCTRL |= PIT_TCTRLn_TIE | PIT_TCTRLn_TEN;
#endif
}

/**
 * @brief   Stops the timer.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 *
 * @notapi
 */
void gpt_lld_stop_timer(GPTDriver *gptp) {
#ifdef KE1xF
  /* Stop the timer */
  PIT->CHANNEL[gptp->channelIndex].TCTRL = 0;
#else
  /* Stop the timer */
  gptp->channel->TCTRL = 0;
#endif
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
#ifdef KE1xF
  /* Disable timer and disable interrupts */
  PIT->CHANNEL[gptp->channelIndex].TCTRL = 0;

  PIT->MSR |= tif_flags[gptp->channelIndex];

  /* Set the interval */
  PIT->CHANNEL[gptp->channelIndex].TVAL = (gptp->clock / gptp->config->frequency) * interval;

  PIT->CHANNEL[gptp->channelIndex].TCTRL = LPIT_TCTRL_T_EN(1);

  while (!(PIT->MSR & tif_flags[gptp->channelIndex]))
    ;
  /* Disable timer and disable interrupts */
  PIT->CHANNEL[gptp->channelIndex].TCTRL = 0;
#else
  struct PIT_CHANNEL *channel = gptp->channel;

  /* Disable timer and disable interrupts */
  channel->TCTRL = 0;

  /* Clear the interrupt flag */
  channel->TFLG |= PIT_TFLGn_TIF;

  /* Set the interval */
  channel->LDVAL = (gptp->clock / gptp->config->frequency) * interval;

  /* Enable Timer but keep interrupts disabled */
  channel->TCTRL = PIT_TCTRLn_TEN;

  /* Wait for the interrupt flag to be set */
  while (!(channel->TFLG & PIT_TFLGn_TIF))
    ;
  /* Disable timer and disable interrupts */
  channel->TCTRL = 0;
#endif
}

#endif /* HAL_USE_GPT */

/** @} */
