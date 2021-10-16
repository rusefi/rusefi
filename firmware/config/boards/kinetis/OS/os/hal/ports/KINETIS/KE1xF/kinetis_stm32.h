/*
    ChibiOS - Copyright (C) 2014-2015 Fabio Utzig

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
 * @file    kinetis_stm32.h
 * @brief   This is needed for better compatibility with STM32 or other CPUs of Kinetis family.
 * @author  andreika <prometheus.pcb@gmail.com>
 *
 */

#ifndef KINETIS_STM32_H_
#define KINETIS_STM32_H_

#ifdef KE1xF

// Kinetis family compatibility macros
#define GPIO_TypeDef GPIO_Type
#define PORT_TypeDef PORT_Type
#define UART_TypeDef LPUART_Type
#define FTM_TypeDef FTM_Type
#define ADC_TypeDef ADC_Type
#define COMP_TypeDef CMP_Type

// More Kinetis family compatibility macros
#define PORTx_PCRn_MUX 				PORT_PCR_MUX
#define PORTx_PCRn_PE 				PORT_PCR_PE(1)
#define PORTx_PCRn_PS 				PORT_PCR_PS(1)
#define PAL_STM32_PUPDR_PULLUP 		(PORTx_PCRn_PE | PORTx_PCRn_PS)
#define PAL_STM32_PUPDR_PULLDOWN    (PORTx_PCRn_PE)
#define PAL_STM32_OSPEED_HIGHEST    0
#define PAL_STM32_MODE_INPUT        (0U << 0U)

#define FTM_SC_TOFx 				FTM_SC_TOF(1)
#define FTM_SC_TOIEx 				FTM_SC_TOIE(1)
#define FTM_CnSC_CHFx 				FTM_CnSC_CHF(1)
#define FTM_CnSC_CHIEx 				FTM_CnSC_CHIE(1)
#define FTM_CnSC_MSBx				FTM_CnSC_MSB(1)
#define FTM_CnSC_ELSAx				FTM_CnSC_ELSA(1)
#define FTM_CnSC_ELSBx				FTM_CnSC_ELSB(1)

#define PIT 						LPIT0
#define PITChannel0_IRQn 			LPIT0_Ch0_IRQn
#define PITChannel1_IRQn 			LPIT0_Ch1_IRQn
#define PITChannel2_IRQn 			LPIT0_Ch2_IRQn
#define PITChannel3_IRQn 			LPIT0_Ch3_IRQn

#else	// other CPUs of Kinetis family

#define FTM_SC_TOFx 				FTM_SC_TOF
#define FTM_SC_TOIEx 				FTM_SC_TOIE
#define FTM_CnSC_CHFx 				FTM_CnSC_CHF
#define FTM_CnSC_CHIEx 				FTM_CnSC_CHIE
#define FTM_CnSC_MSBx				FTM_CnSC_MSB
#define FTM_CnSC_ELSAx				FTM_CnSC_ELSA
#define FTM_CnSC_ELSBx				FTM_CnSC_ELSB

#endif /* KE1xF */

// SPI STM32 compatibility layer
#define SPI_CR1_DFF             	(0x1U << 11U)                /*!< 0x00000800 */
#define SPI_CR1_CPOL            	(0x1U << 1U)                 /*!< 0x00000002 */
#define SPI_CR1_CPHA            	(0x1U << 0U)                 /*!< 0x00000001 */
#define SPI_CR1_MSTR                (0x1U << 2U)                 /*!< 0x00000004 */
#define SPI_CR1_BR_Pos              (3U)                                       
#define SPI_CR1_BR_Msk              (0x7U << SPI_CR1_BR_Pos)     /*!< 0x00000038 */
#define SPI_CR1_BR                  SPI_CR1_BR_Msk               /*!<BR[2:0] bits (Baud Rate Control) */
#define SPI_CR1_BR_0            	(0x1U << 3U)                 /*!< 0x00000008 */
#define SPI_CR1_BR_1            	(0x2U << 3U)                 /*!< 0x00000010 */
#define SPI_CR1_BR_2            	(0x4U << 3U)                 /*!< 0x00000020 */
#define SPI_CR1_LSBFIRST            (0x1U << 7U)                 /*!< 0x00000080 */
#define SPI_CR1_SSI                 (0x1U << 8U)                 /*!< 0x00000100 */
#define SPI_CR1_SSM                 (0x1U << 9U)                 /*!< 0x00000200 */
#define STM32_SPI_USE_SPI1          KINETIS_SPI_USE_SPI0
#define STM32_SPI_USE_SPI2          KINETIS_SPI_USE_SPI1
#define STM32_SPI_USE_SPI3          FALSE

#define USART_CR2_LINEN             (0x1U << 14U)                /*!<LIN mode enable */ /*!< 0x00004000 */

#define STM32_HAS_GPIOA             KINETIS_HAS_GPIOA
#define STM32_HAS_GPIOB             KINETIS_HAS_GPIOB
#define STM32_HAS_GPIOC             KINETIS_HAS_GPIOC
#define STM32_HAS_GPIOD             KINETIS_HAS_GPIOD
#define STM32_HAS_GPIOE             KINETIS_HAS_GPIOE
#define STM32_HAS_GPIOF             KINETIS_HAS_GPIOF
#define STM32_HAS_GPIOG             KINETIS_HAS_GPIOG
#define STM32_HAS_GPIOH             KINETIS_HAS_GPIOH

#endif /* KINETIS_STM32_H_ */
