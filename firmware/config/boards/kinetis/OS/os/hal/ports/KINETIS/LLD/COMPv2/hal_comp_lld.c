/*
    ChibiOS - Copyright (C) 2006..2017 Giovanni Di Sirio
              Copyright (C) 2017 Fabien Poussin (fabien.poussin (at) google's mail)

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
 * @file    Kinetis/hal_comp_lld.c
 * @brief   Kinetis KE1xF Comp subsystem low level driver header.
 * @author  andreika <prometheus.pcb@gmail.com>
 *
 * @addtogroup COMP
 * @{
 */

#include "hal.h"

#if HAL_USE_COMP || defined(__DOXYGEN__)

#include "hal_comp.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   COMPD1 driver identifier.
 * @note    The driver COMPD1 allocates the comparator COMP1 when enabled.
 */
#if KINETIS_COMP_USE_COMP0 || defined(__DOXYGEN__)
COMPDriver COMPD1;
#endif

/**
 * @brief   COMPD2 driver identifier.
 * @note    The driver COMPD2 allocates the comparator COMP2 when enabled.
 */
#if KINETIS_COMP_USE_COMP1 || defined(__DOXYGEN__)
COMPDriver COMPD2;
#endif

/**
 * @brief   COMPD3 driver identifier.
 * @note    The driver COMPD3 allocates the comparator COMP3 when enabled.
 */
#if KINETIS_COMP_USE_COMP2 || defined(__DOXYGEN__)
COMPDriver COMPD3;
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


/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level COMP driver initialization.
 *
 * @notapi
 */
void comp_lld_init(void) {
#if KINETIS_COMP_USE_COMP0
  /* Driver initialization.*/
  compObjectInit(&COMPD1);
  COMPD1.reg = CMP0;
  nvicEnableVector(CMP0_IRQn, KINETIS_COMP_IRQ_PRIORITY);
#endif

#if KINETIS_COMP_USE_COMP1
  /* Driver initialization.*/
  compObjectInit(&COMPD2);
  COMPD2.reg = CMP1;
  nvicEnableVector(CMP1_IRQn, KINETIS_COMP_IRQ_PRIORITY);
#endif

#if KINETIS_COMP_USE_COMP2
  /* Driver initialization.*/
  compObjectInit(&COMPD3);
  COMPD3.reg = CMP2;
  nvicEnableVector(CMP2_IRQn, KINETIS_COMP_IRQ_PRIORITY);
#endif
}

static void comp_lld_irq_handler(COMPDriver *compp) {
  if (compp->config->cb != NULL)
    compp->config->cb(compp);
  // clear flags to continue
  ACMP_ClearStatusFlags(compp->reg, kACMP_OutputRisingEventFlag | kACMP_OutputFallingEventFlag);
}

/**
 * @brief  COMP1, COMP2, COMP3 interrupt handler.
 *
 * @isr
 */
#if KINETIS_COMP_USE_COMP0 || defined(__DOXYGEN__)
OSAL_IRQ_HANDLER(KINETIS_COMP0_IRQ_VECTOR) {
  OSAL_IRQ_PROLOGUE();
  comp_lld_irq_handler(&COMPD1);
  OSAL_IRQ_EPILOGUE();
}
#endif

#if KINETIS_COMP_USE_COMP1 || defined(__DOXYGEN__)
OSAL_IRQ_HANDLER(KINETIS_COMP1_IRQ_VECTOR) {
  OSAL_IRQ_PROLOGUE();
  comp_lld_irq_handler(&COMPD2);
  OSAL_IRQ_EPILOGUE();
}
#endif

#if KINETIS_COMP_USE_COMP2 || defined(__DOXYGEN__)
OSAL_IRQ_HANDLER(KINETIS_COMP2_IRQ_VECTOR) {
  OSAL_IRQ_PROLOGUE();
  comp_lld_irq_handler(&COMPD3);
  OSAL_IRQ_EPILOGUE();
}
#endif

/**
 * @brief   Configures and activates an EXT channel (used by comp)
 *
 * @param[in] compp      pointer to the @p COMPDriver object
 * @param[in] channel    EXT channel (0..7)
 *
 * @notapi
 */
void comp_lld_channel_enable(COMPDriver *compp, uint32_t channel) {
  osalDbgAssert(channel <= 7, "invalid channel number");
  memset(&compp->acmpChannelCfg, 0, sizeof(compp->acmpChannelCfg));
  compp->acmpChannelCfg.positivePortInput = kACMP_PortInputFromDAC;
  compp->acmpChannelCfg.negativePortInput = kACMP_PortInputFromMux;
  compp->acmpChannelCfg.minusMuxInput = channel;
  ACMP_SetChannelConfig(compp->reg, &compp->acmpChannelCfg);
}

/**
 * @brief   Deactivate an EXT channel (used by comp)
 *
 * @param[in] compp      pointer to the @p COMPDriver object
 * @param[in] channel    EXT channel
 *
 * @notapi
 */
void comp_lld_channel_disable(COMPDriver *compp, uint32_t channel) {
  // todo: implement
}

/**
 * @brief   Set the DAC value used by comp
 *
 * @param[in] compp      pointer to the @p COMPDriver object
 * @param[in] value      DAC value (0..255?)
 *
 * @notapi
 */
void comp_lld_set_dac_value(COMPDriver *compp, uint32_t value) {
  memset(&compp->acmpDacCfg, 0, sizeof(compp->acmpDacCfg));
  compp->acmpDacCfg.referenceVoltageSource = kACMP_VrefSourceVin1;	// connected to VDDA on KE1xF
  compp->acmpDacCfg.DACValue = value;
  ACMP_SetDACConfig(compp->reg, &compp->acmpDacCfg);
}

/**
 * @brief   Configures and activates the COMP peripheral.
 *
 * @param[in] compp      pointer to the @p COMPDriver object
 *
 * @notapi
 */
void comp_lld_start(COMPDriver *compp) {
  ACMP_GetDefaultConfig(&compp->acmpCfg);
  if (compp->config->output_mode == COMP_OUTPUT_INVERTED)
  	compp->acmpCfg.enableInvertOutput = true;
  ACMP_Init(compp->reg, &compp->acmpCfg);
  compp->irq_mask = 0;
}

/**
 * @brief   Deactivates the comp peripheral.
 *
 * @param[in] compp      pointer to the @p COMPDriver object
 *
 * @notapi
 */
void comp_lld_stop(COMPDriver *compp) {
  ACMP_Deinit(compp->reg);
}

/**
 * @brief   Enables the output.
 *
 * @param[in] compp      pointer to the @p COMPDriver object
 *
 * @notapi
 */
void comp_lld_enable(COMPDriver *compp) {
  compp->irq_mask = 0;
  
  /* Programming edge registers.*/
  if (compp->config->irq_mode == COMP_IRQ_RISING || compp->config->irq_mode == COMP_IRQ_BOTH)
  	compp->irq_mask |= kACMP_OutputRisingInterruptEnable;
  if (compp->config->irq_mode == COMP_IRQ_FALLING || compp->config->irq_mode == COMP_IRQ_BOTH)
    compp->irq_mask |= kACMP_OutputFallingInterruptEnable;
    
  ACMP_EnableInterrupts(compp->reg, compp->irq_mask);
  ACMP_Enable(compp->reg, true);
}

/**
 * @brief   Disables the output.
 *
 * @param[in] compp      pointer to the @p COMPDriver object
 *
 * @notapi
 */
void comp_lld_disable(COMPDriver *compp) {
  ACMP_Enable(compp->reg, false);
  ACMP_DisableInterrupts(compp->reg, compp->irq_mask);
  compp->irq_mask = 0;
}

uint32_t comp_lld_get_status(COMPDriver *compp) {
  return ACMP_GetStatusFlags(compp->reg);
}

#endif /* HAL_USE_COMP */

/** @} */
