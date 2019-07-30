/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

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

#ifndef _MCUCONF_H_
#define _MCUCONF_H_

#include "clock_config.h"

#define KE1xF_MCUCONF

/*
 * HAL driver system settings.
 */

/* Select the MCU clocking mode below by enabling the appropriate block. */
#define KINETIS_PLL_FREQ	                168000000UL	// 168MHz
#define KINETIS_CLK			                  8000000U	// 8MHz

#define KINETIS_DEFAULT_CLK                 kinetis_clock_int_osc_hs /* kinetis_clock_ext_osc_hs */

/* External OSC */
#define KINETIS_EXT_PLL_MUL	                26
#define KINETIS_EXT_PLL_DIV	                0

/* Internal OSC */
#define KINETIS_INT_PLL_MUL	                26
#define KINETIS_INT_PLL_DIV	                5

/* Used by drivers */
#define KINETIS_PLLCLK_FREQUENCY            KINETIS_PLL_FREQ
#define KINETIS_SYSCLK_FREQUENCY            KINETIS_PLL_FREQ
#define KINETIS_SPLL_DIV1_FREQENCY          2625000U
#define KINETIS_SPLL_DIV2_FREQENCY          21000000U
#define KINETIS_CLKDIV1_OUTDIV4             7 // FLASHCLK=24000000
#define KINETIS_CLKDIV1_OUTDIV2             8 // BUSCLK=21000000

#define KINETIS_UART_FREQUENCY              KINETIS_SPLL_DIV2_FREQENCY
#define KINETIS_SPI_FREQUENCY               KINETIS_SPLL_DIV2_FREQENCY
#define KINETIS_FTM_FREQUENCY               KINETIS_SPLL_DIV1_FREQENCY

/*
 * GPIO driver system settings.
 */
#define KINETIS_HAS_GPIOA                   TRUE
#define KINETIS_HAS_GPIOB                   TRUE
#define KINETIS_HAS_GPIOC                   TRUE
#define KINETIS_HAS_GPIOD                   TRUE
#define KINETIS_HAS_GPIOE                   TRUE
#define KINETIS_HAS_GPIOF                   FALSE
#define KINETIS_HAS_GPIOG                   FALSE
#define KINETIS_HAS_GPIOH                   FALSE
#define KINETIS_GPIO_IRQ_PRIORITY           3

/*
 * UART driver system settings.
 */
#define KINETIS_UART_USE_UART1              TRUE
#define KINETIS_UART_USE_UART2              TRUE

#define KINETIS_UART0_IRQ_PRIORITY          6
#define KINETIS_UART1_IRQ_PRIORITY          6
#define KINETIS_UART2_IRQ_PRIORITY          6
#define KINETIS_UART_DMA_IRQ_PRIORITY       3

/*
 * GPT/PIT driver system settings.
 */
#define KINETIS_GPT_USE_PIT0                TRUE
#define KINETIS_GPT_PIT0_IRQ_PRIORITY       4

/*
 * PWM/FTM driver system settings.
 */
#define KINETIS_PWM_USE_FTM0                TRUE
#define KINETIS_PWM_USE_FTM1                TRUE
//#define KINETIS_PWM_USE_FTM2                FALSE
//#define KINETIS_PWM_USE_FTM3                FALSE
#define KINETIS_PWM_FTM0_PRIORITY           7
#define KINETIS_PWM_FTM1_PRIORITY           7
#define KINETIS_PWM_FTM2_PRIORITY           7
#define KINETIS_PWM_FTM3_PRIORITY           7

/*
 * SPI driver system settings.
 */
#define KINETIS_SPI_USE_SPI0                TRUE
#define KINETIS_SPI_SPI0_IRQ_PRIORITY       10
#define KINETIS_SPI_SPI0_DMA_PRIORITY       1
//#define KINETIS_SPI_USE_SPI1                TRUE
//#define KINETIS_SPI_SPI1_IRQ_PRIORITY       10
//#define KINETIS_SPI_SPI1_DMA_PRIORITY       1

/*
 * ADC driver system settings.
 */
#define KINETIS_ADC_USE_ADC0				TRUE
#define KINETIS_ADC_USE_ADC1				TRUE
#define KINETIS_ADC_USE_ADC2				TRUE
#define KINETIS_ADC_IRQ_PRIORITY            6

/*
 * COMP driver system settings.
 */
#define KINETIS_COMP_USE_COMP0				TRUE
#define KINETIS_COMP_USE_COMP1				TRUE
#define KINETIS_COMP_USE_COMP2				TRUE
#define KINETIS_COMP_IRQ_PRIORITY           2

#endif /* _MCUCONF_H_ */
