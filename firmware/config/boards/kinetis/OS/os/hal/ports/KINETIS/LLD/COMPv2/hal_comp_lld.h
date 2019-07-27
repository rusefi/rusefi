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
 * @file    KINETIS/comp_lld.h
 * @brief   KINETIS KE1xF Comparator subsystem low level driver header.
 * @author  andreika <prometheus.pcb@gmail.com>
 *
 * @addtogroup COMP
 * @{
 */

#ifndef HAL_COMP_LLD_H_
#define HAL_COMP_LLD_H_

#include "hal.h"

#if HAL_USE_COMP || defined(__DOXYGEN__)

#include "fsl_acmp.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/


/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */

/**
 * @brief   COMPD1 driver enable switch.
 * @details If set to @p TRUE the support for COMPD1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(KINETIS_COMP_USE_COMP1) || defined(__DOXYGEN__)
#define KINETIS_COMP_USE_COMP1                  FALSE
#endif

/**
 * @brief   COMPD2 driver enable switch.
 * @details If set to @p TRUE the support for COMPD2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(KINETIS_COMP_USE_COMP2) || defined(__DOXYGEN__)
#define KINETIS_COMP_USE_COMP2                  FALSE
#endif

/**
 * @brief   COMPD3 driver enable switch.
 * @details If set to @p TRUE the support for COMPD3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(KINETIS_COMP_USE_COMP3) || defined(__DOXYGEN__)
#define KINETIS_COMP_USE_COMP3                  FALSE
#endif

/**
 * @brief   COMPD4 driver enable switch.
 * @details If set to @p TRUE the support for COMPD4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(KINETIS_COMP_USE_COMP4) || defined(__DOXYGEN__)
#define KINETIS_COMP_USE_COMP4                  FALSE
#endif

/**
 * @brief   COMPD5 driver enable switch.
 * @details If set to @p TRUE the support for COMPD4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(KINETIS_COMP_USE_COMP5) || defined(__DOXYGEN__)
#define KINETIS_COMP_USE_COMP5                  FALSE
#endif

/**
 * @brief   COMPD6 driver enable switch.
 * @details If set to @p TRUE the support for COMPD4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(KINETIS_COMP_USE_COMP6) || defined(__DOXYGEN__)
#define KINETIS_COMP_USE_COMP6                  FALSE
#endif

/**
 * @brief   COMPD7 driver enable switch.
 * @details If set to @p TRUE the support for COMPD4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(KINETIS_COMP_USE_COMP7) || defined(__DOXYGEN__)
#define KINETIS_COMP_USE_COMP7                  FALSE
#endif

/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if KINETIS_COMP_USE_COMP0 && !KINETIS_HAS_COMP0
#error "COMP1 not present in the selected device"
#endif

#if KINETIS_COMP_USE_COMP1 && !KINETIS_HAS_COMP1
#error "COMP2 not present in the selected device"
#endif

#if KINETIS_COMP_USE_COMP2 && !KINETIS_HAS_COMP2
#error "COMP3 not present in the selected device"
#endif

#if !KINETIS_COMP_USE_COMP0 && !KINETIS_COMP_USE_COMP1 && !KINETIS_COMP_USE_COMP2
#error "COMP driver activated but no COMP peripheral assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   COMP output mode.
 */
typedef enum {
  COMP_OUTPUT_NORMAL = 0,
  COMP_OUTPUT_INVERTED = 1
} comp_output_mode_t;

/**
 * @brief   COMP interrupt mode.
 */
typedef enum {
  COMP_IRQ_RISING = kACMP_OutputRisingEventFlag,
  COMP_IRQ_FALLING = kACMP_OutputFallingEventFlag,
  COMP_IRQ_BOTH = (COMP_IRQ_RISING | COMP_IRQ_FALLING),
} comp_irq_mode_t;

/**
 * @brief   Driver configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct {
  /**
   * @brief   Ouput mode.
   */
  comp_output_mode_t        output_mode;

  /**
   * @brief   Ouput mode.
   */
  comp_irq_mode_t           irq_mode;

  /**
   * @brief   Callback.
   */
  compcallback_t             cb;

  /* End of the mandatory fields.*/
  /**
   * @brief COMP CSR register initialization data.
   * @note  The value of this field should normally be equal to zero.
   */
   /* [andreika]: STM32-compatible field */
  uint32_t                  csr;
} COMPConfig;

/**
 * @brief   Structure representing an COMP driver.
 */
struct COMPDriver {
  /**
   * @brief Driver state.
   */
  compstate_t                state;
  /**
   * @brief Current configuration data.
   */
  const COMPConfig           *config;
#if defined(COMP_DRIVER_EXT_FIELDS)
  COMP_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief Pointer to the COMPx registers block.
   */
  COMP_TypeDef               *reg;

#ifdef KE1xF
  /**
   * @brief ACMP driver config.
   */
  acmp_config_t acmpCfg;

  /**
   * @brief ACMP driver channel config.
   */
  acmp_channel_config_t acmpChannelCfg;

  /**
   * @brief ACMP driver DAC config.
   */
  acmp_dac_config_t acmpDacCfg;

  uint32_t irq_mask;
#endif /* KE1xF */
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if KINETIS_COMP_USE_COMP0 && !defined(__DOXYGEN__)
extern COMPDriver COMPD1;
#endif

#if KINETIS_COMP_USE_COMP1 && !defined(__DOXYGEN__)
extern COMPDriver COMPD2;
#endif

#if KINETIS_COMP_USE_COMP2 && !defined(__DOXYGEN__)
extern COMPDriver COMPD3;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void comp_lld_init(void);
  void comp_lld_start(COMPDriver *compp);
  void comp_lld_stop(COMPDriver *compp);
  void comp_lld_enable(COMPDriver *compp);
  void comp_lld_disable(COMPDriver *compp);
  
  void comp_lld_set_dac_value(COMPDriver *compp, uint32_t value);
  void comp_lld_channel_enable(COMPDriver *compp, uint32_t channel);
  void comp_lld_channel_disable(COMPDriver *compp, uint32_t channel);
  uint32_t comp_lld_get_status(COMPDriver *compp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_COMP */

#endif /* _comp_lld_H_ */

/** @} */
