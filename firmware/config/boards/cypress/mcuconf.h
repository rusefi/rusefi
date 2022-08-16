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

/*
 * HAL driver system settings.
 */

/* Select the MCU clocking mode below by enabling the appropriate block. */
#define CYPRESS_PLL_FREQ	                192000000UL	// 192MHz
#define CYPRESS_CLK			                  8000000U	// 8MHz

#define CYPRESS_SYSCLK_FREQUENCY            CYPRESS_PLL_FREQ
#define CYPRESS_PCLK1_FREQENCY              (CYPRESS_PLL_FREQ / (1 << (APBC1_PSR_Val & 3)))	// 48 MHz

/*
 * GPIO driver system settings.
 */
// Use this with PAL_MODE_ALTERNATE(x)
#define PAL_MODE_ALTERNATIVE_GPIO 	0x0
#define PAL_MODE_ALTERNATIVE_ANALOG	0x1
#define PAL_MODE_ALTERNATIVE_SPI	0x2
#define PAL_MODE_ALTERNATIVE_I2C	0x3
#define PAL_MODE_ALTERNATIVE_UART	0x4
#define PAL_MODE_ALTERNATIVE_CAN	0x5
#define PAL_MODE_ALTERNATIVE_EXTINT 0x6

/*
 * UART driver system settings.
 */
#define CYPRESS_UART_USE_UART1              FALSE/*TRUE*/
#define CYPRESS_UART_USE_UART2              FALSE/*TRUE*/

/*
 * GPT/PIT driver system settings.
 */
#define CYPRESS_GPT_USE_BT0_BT1             TRUE
#define CYPRESS_GPT_USE_BT2_BT3             TRUE
#define CYPRESS_GPT_USE_BT4_BT5             FALSE
#define CYPRESS_GPT_USE_BT6_BT7             FALSE

/*
 * PWM/FTM driver system settings.
 */
#define CYPRESS_PWM_USE_FTM0                FALSE
#define CYPRESS_PWM_USE_FTM1                FALSE
#define CYPRESS_PWM_USE_FTM2                FALSE
#define CYPRESS_PWM_USE_FTM3                FALSE

/*
 * SPI driver system settings.
 */
#define CYPRESS_SPI_USE_SPI0                TRUE
#define CYPRESS_SPI_SPI0_CHANNEL            CSIO6
#define CYPRESS_SPI_USE_SPI1                TRUE
#define CYPRESS_SPI_SPI1_CHANNEL            CSIO7

/*
 * ADC driver system settings.
 */
#define CYPRESS_ADC_USE_ADC0				TRUE
#define CYPRESS_ADC_USE_ADC1				TRUE
#define CYPRESS_ADC_USE_ADC2				TRUE

/*
 * CAN driver system settings.
 */
#define CYPRESS_CAN_USE_CAN0				TRUE
#define CYPRESS_CAN_USE_CAN1				FALSE

#endif /* _MCUCONF_H_ */
