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

/*
 * STM32F7xx drivers configuration.
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

#define STM32F7xx_MCUCONF
#define STM32F765_MCUCONF
#define STM32F767_MCUCONF
#define STM32F777_MCUCONF
#define STM32F769_MCUCONF
#define STM32F779_MCUCONF

/* for stm32f746_nucleo */
#define STM32F746_MCUCONF

#include "efifeatures.h"

/*
 * STM32F7xx drivers configuration.
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

#define STM32F7xx_MCUCONF

/*
 * HAL driver system settings.
 */
#define STM32_NO_INIT                       FALSE
#define STM32_PVD_ENABLE                    FALSE
#define STM32_PLS                           STM32_PLS_LEV0
#define STM32_BKPRAM_ENABLE                 FALSE
#define STM32_HSI_ENABLED                   TRUE
#define STM32_LSI_ENABLED                   TRUE
#define STM32_HSE_ENABLED                   TRUE
#define STM32_LSE_ENABLED                   FALSE
#define STM32_CLOCK48_REQUIRED              TRUE
#define STM32_SW                            STM32_SW_PLL
#define STM32_PLLSRC                        STM32_PLLSRC_HSI
#define STM32_PLLM_VALUE                    16
#define STM32_PLLN_VALUE                    432
#define STM32_PLLP_VALUE                    2
#define STM32_PLLQ_VALUE                    9
#define STM32_HPRE                          STM32_HPRE_DIV1
#define STM32_PPRE1                         STM32_PPRE1_DIV4
#define STM32_PPRE2                         STM32_PPRE2_DIV2
#if STM32_LSE_ENABLED
 #define STM32_RTCSEL                        STM32_RTCSEL_LSE
#else
 #define STM32_RTCSEL                        STM32_RTCSEL_LSI
#endif
#define STM32_RTCPRE_VALUE                  8
#define STM32_MCO1SEL                       STM32_MCO1SEL_HSI
#define STM32_MCO1PRE                       STM32_MCO1PRE_DIV1
#define STM32_MCO2SEL                       STM32_MCO2SEL_SYSCLK
#define STM32_MCO2PRE                       STM32_MCO2PRE_DIV5
#define STM32_I2SSRC                        STM32_I2SSRC_CKIN
#define STM32_PLLI2SN_VALUE                 192
#define STM32_PLLI2SP_VALUE                 4
#define STM32_PLLI2SQ_VALUE                 4
#define STM32_PLLI2SR_VALUE                 4
#define STM32_PLLI2SDIVQ_VALUE              2
#define STM32_PLLSAIN_VALUE                 192
#define STM32_PLLSAIP_VALUE                 4
#define STM32_PLLSAIQ_VALUE                 4
#define STM32_PLLSAIR_VALUE                 4
#define STM32_PLLSAIDIVQ_VALUE              2
#define STM32_PLLSAIDIVR_VALUE              2
#define STM32_SAI1SEL                       STM32_SAI1SEL_OFF
#define STM32_SAI2SEL                       STM32_SAI2SEL_OFF
#define STM32_LCDTFT_REQUIRED               FALSE
#define STM32_USART1SEL                     STM32_USART1SEL_PCLK2
#define STM32_USART2SEL                     STM32_USART2SEL_PCLK1
#define STM32_USART3SEL                     STM32_USART3SEL_PCLK1
#define STM32_UART4SEL                      STM32_UART4SEL_PCLK1
#define STM32_UART5SEL                      STM32_UART5SEL_PCLK1
#define STM32_USART6SEL                     STM32_USART6SEL_PCLK2
#define STM32_UART7SEL                      STM32_UART7SEL_PCLK1
#define STM32_UART8SEL                      STM32_UART8SEL_PCLK1
#define STM32_I2C1SEL                       STM32_I2C1SEL_PCLK1
#define STM32_I2C2SEL                       STM32_I2C2SEL_PCLK1
#define STM32_I2C3SEL                       STM32_I2C3SEL_PCLK1
#define STM32_I2C4SEL                       STM32_I2C4SEL_PCLK1
#define STM32_LPTIM1SEL                     STM32_LPTIM1SEL_PCLK1
#define STM32_CECSEL                        STM32_CECSEL_LSE
#define STM32_CK48MSEL                      STM32_CK48MSEL_PLL
#define STM32_SDMMCSEL                      STM32_SDMMCSEL_PLL48CLK
#define STM32_SRAM2_NOCACHE                 TRUE

/*
 * GPT driver system settings.
 */
#define STM32_IRQ_TIM1_BRK_TIM9_PRIORITY    7
#define STM32_IRQ_TIM1_UP_TIM10_PRIORITY    7
#define STM32_IRQ_TIM1_TRGCO_TIM11_PRIORITY 7
#define STM32_IRQ_TIM1_CC_PRIORITY          7
#define STM32_IRQ_TIM2_PRIORITY             7
#define STM32_IRQ_TIM3_PRIORITY             7
#define STM32_IRQ_TIM4_PRIORITY             7
#define STM32_IRQ_TIM5_PRIORITY             7
#define STM32_IRQ_TIM6_PRIORITY             7
#define STM32_IRQ_TIM7_PRIORITY             7
#define STM32_IRQ_TIM8_BRK_TIM12_PRIORITY   7
#define STM32_IRQ_TIM8_UP_TIM13_PRIORITY    7
#define STM32_IRQ_TIM8_TRGCO_TIM14_PRIORITY 7
#define STM32_IRQ_TIM8_CC_PRIORITY          7

/*
 * SDC driver system settings.
 */
#define STM32_SDC_USE_SDMMC1                FALSE
#define STM32_SDC_USE_SDMMC2                TRUE
#define STM32_SDC_SDMMC_UNALIGNED_SUPPORT   TRUE
#define STM32_SDC_SDMMC_WRITE_TIMEOUT       1000
#define STM32_SDC_SDMMC_READ_TIMEOUT        1000
#define STM32_SDC_SDMMC_CLOCK_DELAY         10
#define STM32_SDC_SDMMC1_DMA_STREAM         STM32_DMA_STREAM_ID(2, 3)
#define STM32_SDC_SDMMC2_DMA_STREAM         STM32_DMA_STREAM_ID(2, 0)
#define STM32_SDC_SDMMC1_DMA_PRIORITY       3
#define STM32_SDC_SDMMC2_DMA_PRIORITY       3
#define STM32_SDC_SDMMC1_IRQ_PRIORITY       9
#define STM32_SDC_SDMMC2_IRQ_PRIORITY       9

#include "mcuconf_common_f4_f7.h"

/* common for all STM32 */
#include "../../cfg/mcuconf_community.h"

#endif /* MCUCONF_H */
