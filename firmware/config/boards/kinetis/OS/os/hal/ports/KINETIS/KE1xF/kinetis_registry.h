/*
    ChibiOS - Copyright (C) 2014 Derek Mulcahy
                        (C) 2016 flabbergast <s3+flabbergast@sdfeu.org>
                        (C) 2019 andreika <prometheus.pcb@gmail.com>

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
 * @file    KE1xF/kinetis_registry.h
 * @brief   KE1xF capabilities registry.
 * @author  andreika <prometheus.pcb@gmail.com>
 *
 * @addtogroup HAL
 * @{
 */

#ifndef KINETIS_REGISTRY_H_
#define KINETIS_REGISTRY_H_

#if !defined(KE1xF) || defined(__DOXYGEN__)
#define KE1xF
#endif

/*===========================================================================*/
/* Platform capabilities.                                                    */
/*===========================================================================*/

/**
 * @name    KE1xF capabilities
 * @{
 */
/**
 * @brief   Maximum system and core clock (f_SYS) frequency.
 */
#define KINETIS_SYSCLK_MAX      168000000L

/**
 * @brief   Maximum bus clock (f_BUS) frequency.
 */
#define KINETIS_BUSCLK_MAX      50000000L

/**
 * @brief   Maximum flash clock (f_FLASH) frequency.
 */
#define KINETIS_FLASHCLK_MAX    25000000L

/**
 * @name    KE1xF attributes
 * @{
 */

/* DMA attributes.*/
#define KINETIS_DMA0_IRQ_VECTOR     Vector40
#define KINETIS_DMA1_IRQ_VECTOR     Vector44
#define KINETIS_DMA2_IRQ_VECTOR     Vector48
#define KINETIS_DMA3_IRQ_VECTOR     Vector4C
#define KINETIS_DMA4_IRQ_VECTOR     Vector50
#define KINETIS_DMA5_IRQ_VECTOR     Vector54
#define KINETIS_DMA6_IRQ_VECTOR     Vector58
#define KINETIS_DMA7_IRQ_VECTOR     Vector5C
#define KINETIS_DMA8_IRQ_VECTOR     Vector60
#define KINETIS_DMA9_IRQ_VECTOR     Vector64
#define KINETIS_DMA10_IRQ_VECTOR    Vector68
#define KINETIS_DMA11_IRQ_VECTOR    Vector6C
#define KINETIS_DMA12_IRQ_VECTOR    Vector70
#define KINETIS_DMA13_IRQ_VECTOR    Vector74
#define KINETIS_DMA14_IRQ_VECTOR    Vector78
#define KINETIS_DMA15_IRQ_VECTOR    Vector7C

//#define KINETIS_HAS_DMA_ERROR_IRQ   TRUE
#define KINETIS_DMA_ERROR_IRQ_VECTOR Vector80

/* I2C attributes.*/
#define KINETIS_HAS_I2C0            TRUE
#define KINETIS_I2C0_IRQ_VECTOR     VectorA0
#define KINETIS_HAS_I2C1            TRUE
#define KINETIS_I2C1_IRQ_VECTOR     VectorA4

/* SPI attributes.*/
#define KINETIS_HAS_SPI0            TRUE
#define KINETIS_SPI0_IRQ_VECTOR     VectorA8
#define KINETIS_HAS_SPI1            TRUE
#define KINETIS_SPI1_IRQ_VECTOR     VectorAC

/* Serial attributes.*/
#define KINETIS_HAS_UART0            TRUE
#define KINETIS_UART0_TX_IRQ_VECTOR  VectorBC
#define KINETIS_UART0_RX_IRQ_VECTOR  VectorC0
#define KINETIS_HAS_UART1            TRUE
#define KINETIS_UART1_TX_IRQ_VECTOR  VectorC4
#define KINETIS_UART1_RX_IRQ_VECTOR  VectorC8
#define KINETIS_HAS_UART2            TRUE
#define KINETIS_UART2_TX_IRQ_VECTOR  VectorCC
#define KINETIS_UART2_RX_IRQ_VECTOR  VectorD0

/* FlexTimer attributes.*/
#define KINETIS_FTM0_CHANNELS 8
#define KINETIS_FTM1_CHANNELS 2
#define KINETIS_FTM2_CHANNELS 2

/* ADC attributes.*/
#define KINETIS_HAS_ADC0            TRUE
#define KINETIS_ADC0_IRQ_VECTOR     VectorDC
#define KINETIS_HAS_ADC1            TRUE
#define KINETIS_ADC1_IRQ_VECTOR     Vector164
#define KINETIS_HAS_ADC2            TRUE
#define KINETIS_ADC2_IRQ_VECTOR     Vector168

/* CMP attributes.*/
#define KINETIS_HAS_COMP0           TRUE
#define KINETIS_COMP0_IRQ_VECTOR    VectorE0
#define KINETIS_HAS_COMP1           TRUE
#define KINETIS_COMP1_IRQ_VECTOR    VectorE4
#define KINETIS_HAS_COMP2           TRUE
#define KINETIS_COMP2_IRQ_VECTOR    Vector158

/* DAC attributes.*/
#define KINETIS_HAS_DAC0            TRUE
#define KINTEIS_DAC0_IRQ_VECTOR     Vector120

#define KINETIS_FTM0_IRQ_VECTOR     VectorE8
#define KINETIS_FTM1_IRQ_VECTOR     VectorEC
#define KINETIS_HAS_FTM2            TRUE
#define KINETIS_FTM2_IRQ_VECTOR     VectorF0
#define KINETIS_HAS_FTM3            TRUE
#define KINETIS_FTM3_IRQ_VECTOR     Vector15C

/* GPT attributes.*/
#define KINETIS_HAS_PIT0            TRUE
#define KINETIS_PIT0_IRQ_VECTOR     Vector100
#define KINETIS_HAS_PIT1            TRUE
#define KINETIS_PIT1_IRQ_VECTOR     Vector104
#define KINETIS_HAS_PIT2            TRUE
#define KINETIS_PIT2_IRQ_VECTOR     Vector108
#define KINETIS_HAS_PIT3            TRUE
#define KINETIS_PIT3_IRQ_VECTOR     Vector10C
#define KINETIS_HAS_PIT             TRUE
#define KINETIS_PIT_CHANNELS        4
#define KINETIS_HAS_PIT_COMMON_IRQ  FALSE

/* USB attributes.*/
#define KINETIS_HAS_USB             FALSE
#define KINETIS_USB0_IS_USBOTG      FALSE
#define KINETIS_HAS_USB_CLOCK_RECOVERY FALSE
 
/* EXT attributes.*/
#define KINETIS_PORTA_IRQ_VECTOR    Vector12C
#define KINETIS_PORTB_IRQ_VECTOR    Vector130
#define KINETIS_PORTC_IRQ_VECTOR    Vector134
#define KINETIS_PORTD_IRQ_VECTOR    Vector138
#define KINETIS_PORTE_IRQ_VECTOR    Vector13C
#define KINETIS_EXT_HAS_COMMON_CD_IRQ   FALSE
#define KINETIS_EXT_HAS_COMMON_BCDE_IRQ FALSE
#define KINETIS_GPIO_HAS_OPENDRAIN  FALSE

/* LPTMR attributes.*/
#define KINETIS_LPTMR0_IRQ_VECTOR   Vector194

/** @} */

#endif /* KINETIS_REGISTRY_H_ */

/** @} */
