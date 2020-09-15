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
 * @file    PITv1/hal_gpt_lld.h
 * @brief   Cypress GPT subsystem low level driver header.
 * @author  andreika <prometheus.pcb@gmail.com>
 *
 * @addtogroup GPT
 * @{
 */
 
#ifndef HAL_GPT_LLD_H_
#define HAL_GPT_LLD_H_

#if HAL_USE_GPT || defined(__DOXYGEN__)

#include "pdl_header.h"

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
 * @brief   GPTD1 driver enable switch.
 * @details If set to @p TRUE the support for GPTD1 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(CYPRESS_GPT_USE_BT0_BT1) || defined(__DOXYGEN__)
#define CYPRESS_GPT_USE_BT0_BT1                  FALSE
#endif

/**
 * @brief   GPTD2 driver enable switch.
 * @details If set to @p TRUE the support for GPTD2 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(CYPRESS_GPT_USE_BT2_BT3) || defined(__DOXYGEN__)
#define CYPRESS_GPT_USE_BT2_BT3                  FALSE
#endif

/**
 * @brief   GPTD3 driver enable switch.
 * @details If set to @p TRUE the support for GPTD3 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(CYPRESS_GPT_USE_BT4_BT5) || defined(__DOXYGEN__)
#define CYPRESS_GPT_USE_BT4_BT5                  FALSE
#endif

/**
 * @brief   GPTD4 driver enable switch.
 * @details If set to @p TRUE the support for GPTD4 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(CYPRESS_GPT_USE_BT6_BT7) || defined(__DOXYGEN__)
#define CYPRESS_GPT_USE_BT6_BT7                  FALSE
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !CYPRESS_GPT_USE_BT0_BT1 && !CYPRESS_GPT_USE_BT2_BT3 &&                         \
    !CYPRESS_GPT_USE_BT4_BT5 && !CYPRESS_GPT_USE_BT6_BT7
#error "GPT driver activated but no BT peripheral assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   GPT frequency type.
 */
typedef uint32_t gptfreq_t;

/**
 * @brief   GPT counter type.
 */
typedef uint32_t gptcnt_t;

/**
 * @brief   Driver configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct {
  /**
   * @brief   Timer clock in Hz.
   * @note    The low level can use assertions in order to catch invalid
   *          frequency specifications.
   */
  gptfreq_t                 frequency;
  /**
   * @brief   Timer callback pointer.
   * @note    This callback is invoked on GPT counter events.
   * @note    This callback can be set to @p NULL but in that case the
   *          one-shot mode cannot be used.
   */
  gptcallback_t             callback;
  /* End of the mandatory fields.*/
  /* [andreika]: STM32-compatible fields */
  /**
   * @brief TIM CR2 register initialization data.
   * @note  The value of this field should normally be equal to zero.
   */
  uint32_t                  cr2;
  /**
   * @brief TIM DIER register initialization data.
   * @note  The value of this field should normally be equal to zero.
   * @note  Only the DMA-related bits can be specified in this field.
   */
  uint32_t                  dier;
} GPTConfig;

/**
 * @brief   Structure representing a GPT driver.
 */
struct GPTDriver {
  /**
   * @brief Driver state.
   */
  gptstate_t                state;
  /**
   * @brief Current configuration data.
   */
  const GPTConfig           *config;
#if defined(GPT_DRIVER_EXT_FIELDS)
  GPT_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
	/**
     * @brief Timer base clock.
	*/
	uint32_t                  clock;
	
	// PDL internals
	volatile stc_btn_t        *bt;
	volatile stc_btn_t        *bt2;
	stc_bt_rt_config_t        stcRtConfig;
    stc_rt_irq_en_t           stcRtIrqEn;
    stc_rt_irq_cb_t           stcRtIrqCb;


	func_ptr_t                serve_interrupt;

	/**
     * @brief Used for 16-bit Timer mode.
	*/
	volatile int32_t          counter;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Returns the counter value of GPT peripheral.
 * @pre     The GPT unit must be running in continuous mode.
 * @note    The nature of the counter is not defined, it may count upward
 *          or downward, it could be continuously running or not.
 *
 * @param[in] gptp      pointer to a @p GPTDriver object
 * @return              The current counter value.
 *
 * @notapi
 */
#define gpt_lld_get_counter(gptp) 0	// TODO: ? // ((gptcnt_t)PIT->CHANNEL[(gptp)->channelIndex].CVAL)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if CYPRESS_GPT_USE_BT0_BT1 && !defined(__DOXYGEN__)
extern GPTDriver GPTD1;
#endif

#if CYPRESS_GPT_USE_BT2_BT3 && !defined(__DOXYGEN__)
extern GPTDriver GPTD2;
#endif

#if CYPRESS_GPT_USE_BT4_BT5 && !defined(__DOXYGEN__)
extern GPTDriver GPTD3;
#endif

#if CYPRESS_GPT_USE_BT6_BT7 && !defined(__DOXYGEN__)
extern GPTDriver GPTD4;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void gpt_lld_init(void);
  void gpt_lld_start(GPTDriver *gptp);
  void gpt_lld_stop(GPTDriver *gptp);
  void gpt_lld_start_timer(GPTDriver *gptp, gptcnt_t period);
  void gpt_lld_stop_timer(GPTDriver *gptp);
  void gpt_lld_polled_delay(GPTDriver *gptp, gptcnt_t interval);

  /**
   * @brief   Changes the interval of GPT peripheral.
   * @details This function changes the interval of a running GPT unit.
   * @pre     The GPT unit must be running in continuous mode.
   * @post    The GPT unit interval is changed to the new value.
   * @note    The function has effect at the next cycle start.
   *
   * @param[in] gptp      pointer to a @p GPTDriver object
   * @param[in] interval  new cycle time in timer ticks
   */
  void gpt_lld_change_interval(GPTDriver *gptp, gptcnt_t interval);

  /**
   * @brief   Returns the interval of GPT peripheral.
   * @pre     The GPT unit must be running in continuous mode.
   *
   * @param[in] gptp      pointer to a @p GPTDriver object
   * @return              The current interval.
   */
  gptcnt_t gpt_lld_get_interval(GPTDriver *gptp);

#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_GPT */

#endif /* HAL_GPT_LLD_H_ */

/** @} */
