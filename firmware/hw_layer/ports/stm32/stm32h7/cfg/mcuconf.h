/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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

#ifndef MCUCONF_H
#define MCUCONF_H

#include "interrupt_priority.h"

/*
 * STM32H7xx drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the whole
 * driver is enabled in halconf.h.
 *
 * IRQ priorities:
 * 15...0       Lowest...Highest.
 *
 * DMA priorities:
 * 0...3        Lowest...Highest.
 */

#define STM32H7xx_MCUCONF

// See hw_ports.mk
#ifdef STM32H743xx
    #define STM32H742_MCUCONF
    #define STM32H743_MCUCONF
    #define STM32H753_MCUCONF
    #define STM32H745_MCUCONF
    #define STM32H755_MCUCONF
    #define STM32H747_MCUCONF
    #define STM32H757_MCUCONF

    #include "mcuconf_stm32h743.h"
#endif
#ifdef STM32H723xx
    #define STM32H723_MCUCONF
    #define STM32H723_MCUCONF
    #define STM32H733_MCUCONF
    #define STM32H725_MCUCONF
    #define STM32H735_MCUCONF

    #include "mcuconf_stm32h723.h"
#endif

// Allows LSE init to timeout and configure fallback RTC clock source in case
#define RUSEFI_STM32_LSE_WAIT_MAX           1000000
#define RUSEFI_STM32_LSE_WAIT_MAX_RTCSEL    STM32_RTCSEL_LSI_CK

/*
 * General settings.
 */
#define STM32_NO_INIT                       FALSE
#define STM32_TARGET_CORE                   1

/*
 * IRQ system settings.
 */

#define STM32_DISABLE_EXTI0_HANDLER
#define STM32_DISABLE_EXTI1_HANDLER
#define STM32_DISABLE_EXTI2_HANDLER
#define STM32_DISABLE_EXTI3_HANDLER
#define STM32_DISABLE_EXTI4_HANDLER
#define STM32_DISABLE_EXTI5_9_HANDLER
#define STM32_DISABLE_EXTI10_15_HANDLER

// we hijack this interrupt handler as the EXTI chained handler, see digital_input_exti.cpp
#define STM32_I2C_I2C1_IRQ_PRIORITY         6

#define STM32_IRQ_EXTI_PRIORITY             EFI_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI0_PRIORITY            EFI_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI1_PRIORITY            EFI_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI2_PRIORITY            EFI_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI3_PRIORITY            EFI_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI4_PRIORITY            EFI_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI5_9_PRIORITY          EFI_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI10_15_PRIORITY        EFI_IRQ_EXTI_PRIORITY

#define STM32_IRQ_EXTI16_PRIORITY           6
#define STM32_IRQ_EXTI17_PRIORITY           6
#define STM32_IRQ_EXTI18_PRIORITY           6
#define STM32_IRQ_EXTI19_PRIORITY           6
#define STM32_IRQ_EXTI20_21_PRIORITY        6

#define STM32_IRQ_FDCAN1_PRIORITY           EFI_IRQ_CAN_PRIORITY
#define STM32_IRQ_FDCAN2_PRIORITY           EFI_IRQ_CAN_PRIORITY
#define STM32_IRQ_FDCAN3_PRIORITY           EFI_IRQ_CAN_PRIORITY

#define STM32_IRQ_MDMA_PRIORITY             9

#define STM32_IRQ_OCTOSPI1_PRIORITY         10
#define STM32_IRQ_OCTOSPI2_PRIORITY         10

#define STM32_IRQ_QUADSPI1_PRIORITY         10

#define STM32_IRQ_SDMMC1_PRIORITY           EFI_IRQ_SDMMC_PRIORITY
#define STM32_IRQ_SDMMC2_PRIORITY           EFI_IRQ_SDMMC_PRIORITY

#define STM32_IRQ_TIM1_UP_PRIORITY          7
#define STM32_IRQ_TIM1_CC_PRIORITY          7
#define STM32_IRQ_TIM2_PRIORITY             7
#define STM32_IRQ_TIM3_PRIORITY             7
#define STM32_IRQ_TIM4_PRIORITY             7
#define STM32_IRQ_TIM5_PRIORITY             EFI_IRQ_SCHEDULING_TIMER_PRIORITY
#define STM32_IRQ_TIM6_PRIORITY             7
#define STM32_IRQ_TIM7_PRIORITY             7
#define STM32_IRQ_TIM8_BRK_TIM12_PRIORITY   7
#define STM32_IRQ_TIM8_UP_TIM13_PRIORITY    7
#define STM32_IRQ_TIM8_TRGCO_TIM14_PRIORITY 7
#define STM32_IRQ_TIM8_CC_PRIORITY          7
#define STM32_IRQ_TIM15_PRIORITY            7
#define STM32_IRQ_TIM16_PRIORITY            7
#define STM32_IRQ_TIM17_PRIORITY            7

#define STM32_IRQ_USART1_PRIORITY           EFI_IRQ_UART_PRIORITY
#define STM32_IRQ_USART2_PRIORITY           EFI_IRQ_UART_PRIORITY
#define STM32_IRQ_USART3_PRIORITY           EFI_IRQ_UART_PRIORITY
#define STM32_IRQ_UART4_PRIORITY            EFI_IRQ_UART_PRIORITY
#define STM32_IRQ_UART5_PRIORITY            EFI_IRQ_UART_PRIORITY
#define STM32_IRQ_USART6_PRIORITY           EFI_IRQ_UART_PRIORITY
#define STM32_IRQ_UART7_PRIORITY            EFI_IRQ_UART_PRIORITY
#define STM32_IRQ_UART8_PRIORITY            EFI_IRQ_UART_PRIORITY
#define STM32_IRQ_UART9_PRIORITY            EFI_IRQ_UART_PRIORITY
#define STM32_IRQ_USART10_PRIORITY          EFI_IRQ_UART_PRIORITY
#define STM32_IRQ_LPUART1_PRIORITY          EFI_IRQ_UART_PRIORITY

/*
 * ADC driver system settings.
 */
#define STM32_ADC_DUAL_MODE                 FALSE
#define STM32_ADC_COMPACT_SAMPLES           FALSE
#define STM32_ADC_USE_ADC12                 TRUE

#ifndef STM32_ADC_USE_ADC3
#define STM32_ADC_USE_ADC3                  FALSE
#endif /* STM32_ADC_USE_ADC3 */

#define STM32_ADC_ADC12_DMA_STREAM          STM32_DMA_STREAM_ID_ANY
#define STM32_ADC_ADC3_BDMA_STREAM          STM32_BDMA_STREAM_ID_ANY
#define STM32_ADC_ADC12_DMA_PRIORITY        2
#define STM32_ADC_ADC3_DMA_PRIORITY         2
#define STM32_ADC_ADC12_IRQ_PRIORITY        EFI_IRQ_ADC_PRIORITY
#define STM32_ADC_ADC3_IRQ_PRIORITY         EFI_IRQ_ADC_PRIORITY
#define STM32_ADC_ADC12_CLOCK_MODE          ADC_CCR_CKMODE_AHB_DIV4
#define STM32_ADC_ADC3_CLOCK_MODE           ADC_CCR_CKMODE_AHB_DIV4

/*
 * CAN driver system settings.
 */
#define STM32_CAN_USE_FDCAN1                TRUE
#define STM32_CAN_USE_FDCAN2                TRUE

/*
 * DAC driver system settings.
 */
#define STM32_DAC_DUAL_MODE                 FALSE
#define STM32_DAC_USE_DAC1_CH1              FALSE
#define STM32_DAC_USE_DAC1_CH2              FALSE
#define STM32_DAC_DAC1_CH1_IRQ_PRIORITY     10
#define STM32_DAC_DAC1_CH2_IRQ_PRIORITY     10
#define STM32_DAC_DAC1_CH1_DMA_PRIORITY     2
#define STM32_DAC_DAC1_CH2_DMA_PRIORITY     2
#define STM32_DAC_DAC1_CH1_DMA_STREAM       STM32_DMA_STREAM_ID_ANY
#define STM32_DAC_DAC1_CH2_DMA_STREAM       STM32_DMA_STREAM_ID_ANY

/*
 * GPT driver system settings.
 */
#define STM32_GPT_USE_TIM1                  FALSE
#define STM32_GPT_USE_TIM2                  FALSE
#define STM32_GPT_USE_TIM3                  TRUE
#define STM32_GPT_USE_TIM4                  FALSE
#define STM32_GPT_USE_TIM5                  FALSE
#define STM32_GPT_USE_TIM6                  TRUE
#define STM32_GPT_USE_TIM7                  FALSE
#define STM32_GPT_USE_TIM8                  FALSE
#define STM32_GPT_USE_TIM12                 FALSE
#define STM32_GPT_USE_TIM13                 FALSE
#define STM32_GPT_USE_TIM14                 FALSE
#define STM32_GPT_USE_TIM15                 FALSE
#define STM32_GPT_USE_TIM16                 FALSE
#define STM32_GPT_USE_TIM17                 FALSE

/*
 * I2C driver system settings.
 */
#define STM32_I2C_USE_I2C1                  FALSE
#define STM32_I2C_USE_I2C2                  FALSE
#define STM32_I2C_USE_I2C3                  FALSE
#define STM32_I2C_USE_I2C4                  FALSE
#define STM32_I2C_BUSY_TIMEOUT              50
#define STM32_I2C_I2C1_RX_DMA_STREAM        STM32_DMA_STREAM_ID_ANY
#define STM32_I2C_I2C1_TX_DMA_STREAM        STM32_DMA_STREAM_ID_ANY
#define STM32_I2C_I2C2_RX_DMA_STREAM        STM32_DMA_STREAM_ID_ANY
#define STM32_I2C_I2C2_TX_DMA_STREAM        STM32_DMA_STREAM_ID_ANY
#define STM32_I2C_I2C3_RX_DMA_STREAM        STM32_DMA_STREAM_ID_ANY
#define STM32_I2C_I2C3_TX_DMA_STREAM        STM32_DMA_STREAM_ID_ANY
#define STM32_I2C_I2C4_RX_BDMA_STREAM       STM32_BDMA_STREAM_ID_ANY
#define STM32_I2C_I2C4_TX_BDMA_STREAM       STM32_BDMA_STREAM_ID_ANY
// STM32_I2C_I2C1_IRQ_PRIORITY is defined above, reused for fast EXTI handoff
#define STM32_I2C_I2C2_IRQ_PRIORITY         5
#define STM32_I2C_I2C3_IRQ_PRIORITY         5
#define STM32_I2C_I2C4_IRQ_PRIORITY         5
#define STM32_I2C_I2C1_DMA_PRIORITY         3
#define STM32_I2C_I2C2_DMA_PRIORITY         3
#define STM32_I2C_I2C3_DMA_PRIORITY         3
#define STM32_I2C_I2C4_DMA_PRIORITY         3
#define STM32_I2C_DMA_ERROR_HOOK(i2cp)      osalSysHalt("DMA failure")

/*
 * ICU driver system settings.
 */
#ifndef STM32_ICU_USE_TIM1
#define STM32_ICU_USE_TIM1                  FALSE
#endif

#ifndef STM32_ICU_USE_TIM2
#define STM32_ICU_USE_TIM2                  TRUE
#endif

#ifndef STM32_ICU_USE_TIM3
#define STM32_ICU_USE_TIM3                  FALSE
#endif

#ifndef STM32_ICU_USE_TIM4
#define STM32_ICU_USE_TIM4                  FALSE
#endif

#ifndef STM32_ICU_USE_TIM5
#define STM32_ICU_USE_TIM5                  FALSE
#endif

#ifndef STM32_ICU_USE_TIM8
#define STM32_ICU_USE_TIM8                  FALSE
#endif

#ifndef STM32_ICU_USE_TIM9
#define STM32_ICU_USE_TIM9                  FALSE
#endif

#define STM32_ICU_USE_TIM12                 FALSE
#define STM32_ICU_USE_TIM13                 FALSE
#define STM32_ICU_USE_TIM14                 FALSE
#define STM32_ICU_USE_TIM15                 FALSE
#define STM32_ICU_USE_TIM16                 FALSE
#define STM32_ICU_USE_TIM17                 FALSE

/*
 * MAC driver system settings.
 */
#define STM32_MAC_TRANSMIT_BUFFERS          4
#define STM32_MAC_RECEIVE_BUFFERS           4
#define STM32_MAC_BUFFERS_SIZE              1522
#define STM32_MAC_PHY_TIMEOUT               100
#define STM32_MAC_ETH1_CHANGE_PHY_STATE     TRUE
#define STM32_MAC_ETH1_IRQ_PRIORITY         13
#define STM32_MAC_IP_CHECKSUM_OFFLOAD       0

/*
 * PWM driver system settings.
 */
#define STM32_PWM_USE_TIM1                  FALSE
#define STM32_PWM_USE_TIM2                  FALSE

#ifndef STM32_PWM_USE_TIM3
#define STM32_PWM_USE_TIM3                  FALSE
#endif

#ifndef STM32_PWM_USE_TIM4
#define STM32_PWM_USE_TIM4                  TRUE
#endif

#ifndef STM32_PWM_USE_TIM5
#define STM32_PWM_USE_TIM5                  TRUE
#endif

#ifndef STM32_PWM_USE_TIM8
#define STM32_PWM_USE_TIM8                  TRUE
#endif

#define STM32_PWM_USE_TIM12                 FALSE
#define STM32_PWM_USE_TIM13                 FALSE
#define STM32_PWM_USE_TIM14                 FALSE
#define STM32_PWM_USE_TIM15                 FALSE
#define STM32_PWM_USE_TIM16                 FALSE
#define STM32_PWM_USE_TIM17                 FALSE

/*
 * RTC driver system settings.
 */
#define STM32_RTC_PRESA_VALUE               32
#define STM32_RTC_PRESS_VALUE               1024
#define STM32_RTC_CR_INIT                   0
#define STM32_RTC_TAMPCR_INIT               0

/*
 * SDC driver system settings.
 */
#define STM32_SDC_USE_SDMMC1                TRUE
#define STM32_SDC_USE_SDMMC2                FALSE
#define STM32_SDC_SDMMC_UNALIGNED_SUPPORT   TRUE
#define STM32_SDC_SDMMC_WRITE_TIMEOUT       1000000
#define STM32_SDC_SDMMC_READ_TIMEOUT        1000000
#define STM32_SDC_SDMMC_CLOCK_DELAY         10
#define STM32_SDC_SDMMC_PWRSAV              TRUE

/*
 * SERIAL driver system settings.
 */
#ifndef STM32_SERIAL_USE_USART1
#define STM32_SERIAL_USE_USART1             TRUE
#endif

#ifndef STM32_SERIAL_USE_USART2
#define STM32_SERIAL_USE_USART2             FALSE
#endif

#ifndef STM32_SERIAL_USE_USART3
#define STM32_SERIAL_USE_USART3             FALSE
#endif

#ifndef STM32_SERIAL_USE_UART4
#define STM32_SERIAL_USE_UART4              FALSE
#endif

#ifndef STM32_SERIAL_USE_UART5
#define STM32_SERIAL_USE_UART5              FALSE
#endif

#ifndef STM32_SERIAL_USE_USART6
#define STM32_SERIAL_USE_USART6             TRUE
#endif

#define STM32_SERIAL_USE_UART7              FALSE
#define STM32_SERIAL_USE_UART8              FALSE
#define STM32_SERIAL_USE_LPUART1            FALSE

/*
 * SPI driver system settings.
 */
#ifndef STM32_SPI_USE_SPI1
#define STM32_SPI_USE_SPI1                  TRUE
#endif
#ifndef STM32_SPI_USE_SPI2
#define STM32_SPI_USE_SPI2                  TRUE
#endif
#ifndef STM32_SPI_USE_SPI3
#define STM32_SPI_USE_SPI3                  TRUE
#endif
#ifndef STM32_SPI_USE_SPI4
#define STM32_SPI_USE_SPI4                  FALSE
#endif
#ifndef STM32_SPI_USE_SPI5
#define STM32_SPI_USE_SPI5                  FALSE
#endif
#ifndef STM32_SPI_USE_SPI6
#define STM32_SPI_USE_SPI6                  FALSE
#endif

#define STM32_SPI_SPI1_RX_DMA_STREAM        STM32_DMA_STREAM_ID_ANY
#define STM32_SPI_SPI1_TX_DMA_STREAM        STM32_DMA_STREAM_ID_ANY
#define STM32_SPI_SPI2_RX_DMA_STREAM        STM32_DMA_STREAM_ID_ANY
#define STM32_SPI_SPI2_TX_DMA_STREAM        STM32_DMA_STREAM_ID_ANY
#define STM32_SPI_SPI3_RX_DMA_STREAM        STM32_DMA_STREAM_ID_ANY
#define STM32_SPI_SPI3_TX_DMA_STREAM        STM32_DMA_STREAM_ID_ANY
#define STM32_SPI_SPI4_RX_DMA_STREAM        STM32_DMA_STREAM_ID_ANY
#define STM32_SPI_SPI4_TX_DMA_STREAM        STM32_DMA_STREAM_ID_ANY
#define STM32_SPI_SPI5_RX_DMA_STREAM        STM32_DMA_STREAM_ID_ANY
#define STM32_SPI_SPI5_TX_DMA_STREAM        STM32_DMA_STREAM_ID_ANY
#define STM32_SPI_SPI6_RX_BDMA_STREAM       STM32_BDMA_STREAM_ID_ANY
#define STM32_SPI_SPI6_TX_BDMA_STREAM       STM32_BDMA_STREAM_ID_ANY
#define STM32_SPI_SPI1_DMA_PRIORITY         1
#define STM32_SPI_SPI2_DMA_PRIORITY         1
#define STM32_SPI_SPI3_DMA_PRIORITY         1
#define STM32_SPI_SPI4_DMA_PRIORITY         1
#define STM32_SPI_SPI5_DMA_PRIORITY         1
#define STM32_SPI_SPI6_DMA_PRIORITY         1
#define STM32_SPI_SPI1_IRQ_PRIORITY         EFI_IRQ_SPI_PRIORITY
#define STM32_SPI_SPI2_IRQ_PRIORITY         EFI_IRQ_SPI_PRIORITY
#define STM32_SPI_SPI3_IRQ_PRIORITY         EFI_IRQ_SPI_PRIORITY
#define STM32_SPI_SPI4_IRQ_PRIORITY         EFI_IRQ_SPI_PRIORITY
#define STM32_SPI_SPI5_IRQ_PRIORITY         EFI_IRQ_SPI_PRIORITY
#define STM32_SPI_SPI6_IRQ_PRIORITY         EFI_IRQ_SPI_PRIORITY
#define STM32_SPI_DMA_ERROR_HOOK(spip)      osalSysHalt("DMA failure")

/*
 * ST driver system settings.
 */
#define STM32_ST_IRQ_PRIORITY               EFI_IRQ_SYSTICK_PRIORITY
#define STM32_ST_USE_TIMER                  2

/*
 * TRNG driver system settings.
 */
#define STM32_TRNG_USE_RNG1                 FALSE

/*
 * UART driver system settings.
 */
#define STM32_UART_USE_USART1               FALSE

#ifndef STM32_UART_USE_USART2
#define STM32_UART_USE_USART2               FALSE
#endif

#define STM32_UART_USE_USART3               TRUE
#define STM32_UART_USE_UART4                FALSE
#define STM32_UART_USE_UART5                FALSE
#define STM32_UART_USE_USART6               FALSE
#define STM32_UART_USE_UART7                FALSE
#define STM32_UART_USE_UART8                FALSE
#define STM32_UART_USART1_RX_DMA_STREAM     STM32_DMA_STREAM_ID_ANY
#define STM32_UART_USART1_TX_DMA_STREAM     STM32_DMA_STREAM_ID_ANY
#define STM32_UART_USART2_RX_DMA_STREAM     STM32_DMA_STREAM_ID_ANY
#define STM32_UART_USART2_TX_DMA_STREAM     STM32_DMA_STREAM_ID_ANY
#define STM32_UART_USART3_RX_DMA_STREAM     STM32_DMA_STREAM_ID_ANY
#define STM32_UART_USART3_TX_DMA_STREAM     STM32_DMA_STREAM_ID_ANY
#define STM32_UART_UART4_RX_DMA_STREAM      STM32_DMA_STREAM_ID_ANY
#define STM32_UART_UART4_TX_DMA_STREAM      STM32_DMA_STREAM_ID_ANY
#define STM32_UART_UART5_RX_DMA_STREAM      STM32_DMA_STREAM_ID_ANY
#define STM32_UART_UART5_TX_DMA_STREAM      STM32_DMA_STREAM_ID_ANY
#define STM32_UART_USART6_RX_DMA_STREAM     STM32_DMA_STREAM_ID_ANY
#define STM32_UART_USART6_TX_DMA_STREAM     STM32_DMA_STREAM_ID_ANY
#define STM32_UART_UART7_RX_DMA_STREAM      STM32_DMA_STREAM_ID_ANY
#define STM32_UART_UART7_TX_DMA_STREAM      STM32_DMA_STREAM_ID_ANY
#define STM32_UART_UART8_RX_DMA_STREAM      STM32_DMA_STREAM_ID_ANY
#define STM32_UART_UART8_TX_DMA_STREAM      STM32_DMA_STREAM_ID_ANY
#define STM32_UART_USART1_DMA_PRIORITY      0
#define STM32_UART_USART2_DMA_PRIORITY      0
#define STM32_UART_USART3_DMA_PRIORITY      0
#define STM32_UART_UART4_DMA_PRIORITY       0
#define STM32_UART_UART5_DMA_PRIORITY       0
#define STM32_UART_USART6_DMA_PRIORITY      0
#define STM32_UART_UART7_DMA_PRIORITY       0
#define STM32_UART_UART8_DMA_PRIORITY       0
#define STM32_UART_DMA_ERROR_HOOK(uartp)    osalSysHalt("DMA failure")

/*
 * USB driver system settings.
 */
#define STM32_USB_OTG1_IRQ_PRIORITY         EFI_IRQ_USB_PRIORITY
#define STM32_USB_OTG2_IRQ_PRIORITY         EFI_IRQ_USB_PRIORITY

/*
 * WDG driver system settings.
 */
#define STM32_WDG_USE_IWDG                  TRUE

/*
 * WSPI driver system settings.
 */
#define STM32_WSPI_USE_QUADSPI1             FALSE
#define STM32_WSPI_QUADSPI1_PRESCALER_VALUE 1
#define STM32_WSPI_QUADSPI1_MDMA_CHANNEL    STM32_MDMA_CHANNEL_ID_ANY
#define STM32_WSPI_QUADSPI1_MDMA_PRIORITY   1
#define STM32_WSPI_MDMA_ERROR_HOOK(qspip)   osalSysHalt("MDMA failure")

#define STM32_SYSCLK STM32_SYS_CK

#ifndef ENABLE_AUTO_DETECT_HSE
    #define ENABLE_AUTO_DETECT_HSE          TRUE
#endif

#endif /* MCUCONF_H */
