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
 * @file    ADCv1/hal_adc_lld.h
 * @brief   Cypress S6E2x ADC subsystem low level driver header.
 * @author  andreika <prometheus.pcb@gmail.com>
 *
 * @addtogroup ADC
 * @{
 */

#ifndef HAL_ADC_LLD_H_
#define HAL_ADC_LLD_H_

#if HAL_USE_ADC || defined(__DOXYGEN__)

#include "pdl_header.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Absolute Maximum Ratings
 * @{
 */
/**
 * @brief   Minimum ADC clock frequency.
 */
#define CYPRESS_ADCCLK_MIN        600000

/**
 * @brief   Maximum ADC clock frequency.
 */
#define CYPRESS_ADCCLK_MAX        36000000

#define CYPRESS_ADC_FIFO_SIZE     16

#define ADCx_SC1n_ADCH_DISABLED   31

#define ADC_CR2_SWSTART           (0x1U << 30U)                /*!<Start Conversion of regular channels =0x40000000 */

#define ADC_NUM_CHANNELS          32


/**
 * @name    Sampling rate settings helper macros
 * @{
 */
#define ADC_SMPR2_SMP_AN0(n)    ((n) << 0)  /**< @brief AN0 sampling time.  */
#define ADC_SMPR2_SMP_AN1(n)    ((n) << 3)  /**< @brief AN1 sampling time.  */
#define ADC_SMPR2_SMP_AN2(n)    ((n) << 6)  /**< @brief AN2 sampling time.  */
#define ADC_SMPR2_SMP_AN3(n)    ((n) << 9)  /**< @brief AN3 sampling time.  */
#define ADC_SMPR2_SMP_AN4(n)    ((n) << 12) /**< @brief AN4 sampling time.  */
#define ADC_SMPR2_SMP_AN5(n)    ((n) << 15) /**< @brief AN5 sampling time.  */
#define ADC_SMPR2_SMP_AN6(n)    ((n) << 18) /**< @brief AN6 sampling time.  */
#define ADC_SMPR2_SMP_AN7(n)    ((n) << 21) /**< @brief AN7 sampling time.  */
#define ADC_SMPR2_SMP_AN8(n)    ((n) << 24) /**< @brief AN8 sampling time.  */
#define ADC_SMPR2_SMP_AN9(n)    ((n) << 27) /**< @brief AN9 sampling time.  */

#define ADC_SMPR1_SMP_AN10(n)   ((n) << 0)  /**< @brief AN10 sampling time. */
#define ADC_SMPR1_SMP_AN11(n)   ((n) << 3)  /**< @brief AN11 sampling time. */
#define ADC_SMPR1_SMP_AN12(n)   ((n) << 6)  /**< @brief AN12 sampling time. */
#define ADC_SMPR1_SMP_AN13(n)   ((n) << 9)  /**< @brief AN13 sampling time. */
#define ADC_SMPR1_SMP_AN14(n)   ((n) << 12) /**< @brief AN14 sampling time. */
#define ADC_SMPR1_SMP_AN15(n)   ((n) << 15) /**< @brief AN15 sampling time. */
#define ADC_SMPR1_SMP_SENSOR(n) ((n) << 18) /**< @brief Temperature Sensor
                                                 sampling time.             */

/**
 * @name    Available analog channels
 * @{
 */
#define ADC_CHANNEL_IN0         0   /**< @brief External analog input 0.    */
#define ADC_CHANNEL_IN1         1   /**< @brief External analog input 1.    */
#define ADC_CHANNEL_IN2         2   /**< @brief External analog input 2.    */
#define ADC_CHANNEL_IN3         3   /**< @brief External analog input 3.    */
#define ADC_CHANNEL_IN4         4   /**< @brief External analog input 4.    */
#define ADC_CHANNEL_IN5         5   /**< @brief External analog input 5.    */
#define ADC_CHANNEL_IN6         6   /**< @brief External analog input 6.    */
#define ADC_CHANNEL_IN7         7   /**< @brief External analog input 7.    */
#define ADC_CHANNEL_IN8         8   /**< @brief External analog input 8.    */
#define ADC_CHANNEL_IN9         9   /**< @brief External analog input 9.    */
#define ADC_CHANNEL_IN10        10  /**< @brief External analog input 10.   */
#define ADC_CHANNEL_IN11        11  /**< @brief External analog input 11.   */
#define ADC_CHANNEL_IN12        12  /**< @brief External analog input 12.   */
#define ADC_CHANNEL_IN13        13  /**< @brief External analog input 13.   */
#define ADC_CHANNEL_IN14        14  /**< @brief External analog input 14.   */
#define ADC_CHANNEL_IN15        15  /**< @brief External analog input 15.   */
#if 0
#define ADC_CHANNEL_SENSOR      16  /**< @brief Internal temperature sensor.
                                         @note Available onADC1 only.       */
#define ADC_CHANNEL_VREFINT     17  /**< @brief Internal reference.
                                         @note Available onADC1 only.       */
#define ADC_CHANNEL_VBAT        18  /**< @brief VBAT.
                                         @note Available onADC1 only.       */
#endif

/**
 * @name    Sampling rates
 * @{
 */
#define ADC_SAMPLE_3            0   /**< @brief 3 cycles sampling time.     */
#define ADC_SAMPLE_15           1   /**< @brief 15 cycles sampling time.    */
#define ADC_SAMPLE_28           2   /**< @brief 28 cycles sampling time.    */
#define ADC_SAMPLE_56           3   /**< @brief 56 cycles sampling time.    */
#define ADC_SAMPLE_84           4   /**< @brief 84 cycles sampling time.    */
#define ADC_SAMPLE_112          5   /**< @brief 112 cycles sampling time.   */
#define ADC_SAMPLE_144          6   /**< @brief 144 cycles sampling time.   */
#define ADC_SAMPLE_480          7   /**< @brief 480 cycles sampling time.   */


/** @} */

// STM32-specific defines
#ifndef ADC_TwoSamplingDelay_20Cycles
#define ADC_TwoSamplingDelay_20Cycles ((uint32_t)0x00000F00)
#endif

#define ADC_SQR1_NUM_CH(n)     0

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */

/**
 * @brief   ADC1 driver enable switch.
 * @details If set to @p TRUE the support for ADC1 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(CYPRESS_ADC_USE_ADC0) || defined(__DOXYGEN__)
#define CYPRESS_ADC_USE_ADC0                FALSE
#endif

#if !defined(CYPRESS_ADC_USE_ADC1) || defined(__DOXYGEN__)
#define CYPRESS_ADC_USE_ADC1                FALSE
#endif

#if !defined(CYPRESS_ADC_USE_ADC2) || defined(__DOXYGEN__)
#define CYPRESS_ADC_USE_ADC2                FALSE
#endif

/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !CYPRESS_ADC_USE_ADC0 && !CYPRESS_ADC_USE_ADC1 && !CYPRESS_ADC_USE_ADC2
#error "ADC driver activated but no ADC peripheral assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   ADC sample data type.
 */
typedef uint16_t adcsample_t;

/**
 * @brief   Channels number in a conversion group.
 */
typedef uint16_t adc_channels_num_t;

/**
 * @brief   Possible ADC failure causes.
 * @note    Error codes are architecture dependent and should not relied
 *          upon.
 */
typedef enum {
  ADC_ERR_DMAFAILURE = 0,                   /**< DMA operations failure.    */
  ADC_ERR_OVERFLOW = 1                      /**< ADC overflow condition.    */
} adcerror_t;

/**
 * @brief   ADC error callback type.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object triggering the
 *                      callback
 * @param[in] err       ADC error code
 */
typedef void (*adcerrorcallback_t)(ADCDriver *adcp, adcerror_t err);


/**
 * @brief   Low level fields of the ADC configuration structure.
 */
#define adc_lld_configuration_group_fields                                  \
  /* [dron_gus]: is there any reason to keep comatibility with stm32 drivers? */ \
  /* [andreika]: these are STM32-compatible fields, we'll convert them into Cypress ones */ \
  /* ADC CR1 register initialization data.                                  \
     NOTE: All the required bits must be defined into this field except     \
           @p ADC_CR1_SCAN that is enforced inside the driver.*/            \
  uint32_t                  cr1;                                            \
  /* ADC CR2 register initialization data.                                  \
     NOTE: All the required bits must be defined into this field except     \
           @p ADC_CR2_DMA, @p ADC_CR2_CONT and @p ADC_CR2_ADON that are     \
           enforced inside the driver.*/                                    \
  uint32_t                  cr2;                                            \
  /* ADC SMPR1 register initialization data.                                \
     NOTE: In this field must be specified the sample times for channels    \
           10...18.*/                                                       \
  uint32_t                  smpr1;                                          \
  /* ADC SMPR2 register initialization data.                                \
     NOTE: In this field must be specified the sample times for channels    \
           0...9.*/                                                         \
  uint32_t                  smpr2;                                          \
  /* ADC watchdog high threshold register.                                  \
     NOTE: This field defines the high threshold of the analog watchdog.*/  \
  uint16_t                  htr;                                            \
  /* ADC watchdog low threshold register.                                   \
     NOTE: This field defines the low threshold of the analog watchdog.*/   \
  uint16_t                  ltr;                                            \
  /* ADC SQR1 register initialization data.                                 \
     NOTE: Conversion group sequence 13...16 + sequence length.*/           \
  uint32_t                  sqr1;                                           \
  /* ADC SQR2 register initialization data. */                              \
  uint32_t                  sqr2;                                           \
  /* ADC SQR3 register initialization data. */                              \
  uint32_t                  sqr3;                                           \
  /* ADC SQR4 register initialization data. */                              \
  uint32_t                  sqr4;                                           \
  /* ADC SQR5 register initialization data. */                              \
  uint32_t                  sqr5

/**
 * @brief   Low level fields of the ADC configuration structure.
 */
#define adc_lld_config_fields                                               \
  /* Perform first time calibration */                                      \
  bool                      calibrate;                                      \
  /* Dummy configuration, it is not needed.*/                               \
  uint32_t                  dummy

/**
 * @brief   Low level fields of the ADC driver structure.
 */
#define adc_lld_driver_fields                                                \
	/* Pointer to the PDL driver structure. */                                 \
	stc_adcn_t              *adc;                                              \
	/* Pointer to the PDL driver config structures. */                         \
  stc_adc_config_t         stcAdcConfig;                                     \
  stc_adc_scan_t           stcScanCfg;                                       \
	/* Pointer to IRQ callback structures. */                                  \
  stc_adc_irq_en_t         stcIrqEn;                                         \
  stc_adc_irq_cb_t         stcIrqCb;                                         \
	/* Number of samples expected. */                                          \
	size_t                   number_of_samples;                                \
	/* Current position in the buffer. */                                      \
	volatile size_t          current_index;                                    \
	/* Channel group index used to select software or hardware conversion triggering. */ \
	uint32_t                 channelGroup;                                     \
	/* Bitmask of channels for ADC conversion. */                              \
	int8_t                   channelHwIndices[ADC_NUM_CHANNELS];               \
	/* Reciprocal to channelHwIndices. */                                      \
	int8_t                   channelLogicIndices[ADC_NUM_CHANNELS];            \
	bool                     wasInit;                                          \
	/* We don't want to init ADC driver every time, so we check if settings are changed */ \
	int8_t                   oldChannelHwIndices[ADC_NUM_CHANNELS];            \
	adc_channels_num_t       oldNumChannels;                                   \
	size_t                   oldDepth;                                         \
	/* The size of ADC FIFO buffer */                                          \
	int                      fifoSize

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/


/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if CYPRESS_ADC_USE_ADC0 && !defined(__DOXYGEN__)
extern ADCDriver ADCD1;
#endif

#if CYPRESS_ADC_USE_ADC1 && !defined(__DOXYGEN__)
extern ADCDriver ADCD2;
#endif

#if CYPRESS_ADC_USE_ADC2 && !defined(__DOXYGEN__)
extern ADCDriver ADCD3;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void adc_lld_init(void);
  void adc_lld_start(ADCDriver *adcp);
  void adc_lld_stop(ADCDriver *adcp);
  void adc_lld_start_conversion(ADCDriver *adcp);
  void adc_lld_stop_conversion(ADCDriver *adcp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_ADC */

#endif /* HAL_ADC_LLD_H_ */

/** @} */
