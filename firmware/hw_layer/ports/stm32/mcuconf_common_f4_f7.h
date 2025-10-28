/**
 * @file	mcuconf_common_f4_f7.cpp
 * @brief	Common MCU definitions for both the STM32F4 and STM32F7
 *
 * @date Feb 18, 2021
 * @author Matthew Kennedy
 */

#pragma once

#include "efifeatures.h"

#include "interrupt_priority.h"

/*
 * SysTick driver system settings.
 */
#define STM32_ST_IRQ_PRIORITY               EFI_IRQ_SYSTICK_PRIORITY
#define STM32_ST_USE_TIMER                  2

/*
 * ADC driver system settings.
 */
#define STM32_ADC_ADCPRE                    ADC_CCR_ADCPRE_DIV4

#ifndef STM32_ADC_USE_ADC1
#define STM32_ADC_USE_ADC1                  TRUE
#endif /* STM32_ADC_USE_ADC1 */

#ifndef STM32_ADC_USE_ADC2
#define STM32_ADC_USE_ADC2                  TRUE
#endif /* STM32_ADC_USE_ADC2 */

#ifndef STM32_ADC_USE_ADC3
#define STM32_ADC_USE_ADC3                  FALSE
#endif /* STM32_ADC_USE_ADC3 */

#define STM32_ADC_ADC1_DMA_STREAM           STM32_DMA_STREAM_ID(2, 4)
#define STM32_ADC_ADC2_DMA_STREAM           STM32_DMA_STREAM_ID(2, 2)
#define STM32_ADC_ADC3_DMA_STREAM           STM32_DMA_STREAM_ID(2, 1)
#define STM32_ADC_ADC1_DMA_PRIORITY         3
#define STM32_ADC_ADC2_DMA_PRIORITY         3
#define STM32_ADC_ADC3_DMA_PRIORITY         3
#define STM32_ADC_IRQ_PRIORITY              EFI_IRQ_ADC_PRIORITY
#define STM32_ADC_ADC1_DMA_IRQ_PRIORITY     EFI_IRQ_ADC_PRIORITY
#define STM32_ADC_ADC2_DMA_IRQ_PRIORITY     EFI_IRQ_ADC_PRIORITY
#define STM32_ADC_ADC3_DMA_IRQ_PRIORITY     EFI_IRQ_ADC_PRIORITY

/*
 * CAN driver system settings.
 */
#ifndef STM32_CAN_USE_CAN1
#define STM32_CAN_USE_CAN1                  TRUE
#endif

#ifndef STM32_CAN_USE_CAN2
#define STM32_CAN_USE_CAN2                  TRUE
#endif

#ifndef STM32_CAN_USE_CAN3
#define STM32_CAN_USE_CAN3                  FALSE
#endif

#define STM32_CAN_CAN1_IRQ_PRIORITY         EFI_IRQ_CAN_PRIORITY
#define STM32_CAN_CAN2_IRQ_PRIORITY         EFI_IRQ_CAN_PRIORITY
#define STM32_CAN_CAN3_IRQ_PRIORITY         EFI_IRQ_CAN_PRIORITY

#define STM32_IRQ_EXTI0_PRIORITY            EFI_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI1_PRIORITY            EFI_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI2_PRIORITY            EFI_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI3_PRIORITY            EFI_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI4_PRIORITY            EFI_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI5_9_PRIORITY          EFI_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI10_15_PRIORITY        EFI_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI16_PRIORITY           15
#define STM32_IRQ_EXTI17_PRIORITY           15  /* why? */
#define STM32_IRQ_EXTI18_PRIORITY           15
#define STM32_IRQ_EXTI19_PRIORITY           15
#define STM32_IRQ_EXTI20_PRIORITY           15
#define STM32_IRQ_EXTI21_PRIORITY           15  /* why? */
#define STM32_IRQ_EXTI22_PRIORITY           15  /* why? */
#define STM32_IRQ_EXTI23_PRIORITY           15

/*
 * GPT driver system settings.
 */
/* This actually defines TIM IRQ priorities independently of driver used */
#define STM32_IRQ_TIM1_BRK_TIM9_PRIORITY    7
#define STM32_IRQ_TIM1_UP_TIM10_PRIORITY    7
#define STM32_IRQ_TIM1_TRGCO_TIM11_PRIORITY 7
#define STM32_IRQ_TIM1_CC_PRIORITY          7
#define STM32_IRQ_TIM2_PRIORITY             7
#define STM32_IRQ_TIM3_PRIORITY             7
#define STM32_IRQ_TIM4_PRIORITY             7
#define STM32_IRQ_TIM7_PRIORITY             7
#define STM32_IRQ_TIM8_BRK_TIM12_PRIORITY   7
#define STM32_IRQ_TIM8_UP_TIM13_PRIORITY    7
#define STM32_IRQ_TIM8_TRGCO_TIM14_PRIORITY 7
#define STM32_IRQ_TIM8_CC_PRIORITY          7

/*
 * GPT driver system settings.
 */
#define STM32_GPT_USE_TIM1                  FALSE
#define STM32_GPT_USE_TIM2                  FALSE
#define STM32_GPT_USE_TIM3                  FALSE
#define STM32_GPT_USE_TIM4                  FALSE
#define STM32_GPT_USE_TIM5                  FALSE
#define STM32_GPT_USE_TIM6                  TRUE
#define STM32_GPT_USE_TIM7                  FALSE
#define STM32_GPT_USE_TIM8                  FALSE
#define STM32_GPT_USE_TIM9                  FALSE
#define STM32_GPT_USE_TIM11                 FALSE
#define STM32_GPT_USE_TIM12                 FALSE
#define STM32_GPT_USE_TIM14                 FALSE

/*
 * I2C driver system settings.
 */
#ifndef STM32_I2C_USE_I2C1
#define STM32_I2C_USE_I2C1                  FALSE
#endif

#ifndef STM32_I2C_USE_I2C2
/* PB10/PB11 like some Hellen likes AF4 */
#define STM32_I2C_USE_I2C2                  TRUE
#endif

#ifndef STM32_I2C_USE_I2C3
// PC9 I2C3_SDA AF4
// PA8 I2C3_SCL AF4
#define STM32_I2C_USE_I2C3                  FALSE
#endif

#ifndef STM32_I2C_USE_I2C4
#define STM32_I2C_USE_I2C4                  FALSE
#endif

#define STM32_I2C_BUSY_TIMEOUT              50
#define STM32_I2C_I2C1_RX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 0)
#define STM32_I2C_I2C1_TX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 6)
#define STM32_I2C_I2C2_RX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 2)
#define STM32_I2C_I2C2_TX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 7)
#define STM32_I2C_I2C3_RX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 2)
#define STM32_I2C_I2C3_TX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 4)
#define STM32_I2C_I2C4_RX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 2)
#define STM32_I2C_I2C4_TX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 5)
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

#define STM32_ICU_TIM1_IRQ_PRIORITY         ICU_PRIORITY
#define STM32_ICU_TIM2_IRQ_PRIORITY         ICU_PRIORITY
#define STM32_ICU_TIM3_IRQ_PRIORITY         ICU_PRIORITY
#define STM32_ICU_TIM4_IRQ_PRIORITY         ICU_PRIORITY
#define STM32_ICU_TIM5_IRQ_PRIORITY         ICU_PRIORITY
#define STM32_ICU_TIM8_IRQ_PRIORITY         ICU_PRIORITY
#define STM32_ICU_TIM9_IRQ_PRIORITY         ICU_PRIORITY

/*
 * MAC driver system settings.
 */
#define STM32_MAC_TRANSMIT_BUFFERS          2
#define STM32_MAC_RECEIVE_BUFFERS           4
#define STM32_MAC_BUFFERS_SIZE              1522
#define STM32_MAC_PHY_TIMEOUT               100
#define STM32_MAC_ETH1_CHANGE_PHY_STATE     TRUE
#define STM32_MAC_ETH1_IRQ_PRIORITY         13
#define STM32_MAC_IP_CHECKSUM_OFFLOAD       3

/*
 * PWM driver system settings.
 */
#ifndef STM32_PWM_USE_TIM1
#define STM32_PWM_USE_TIM1                  TRUE
#endif

#ifndef STM32_PWM_USE_TIM2
#define STM32_PWM_USE_TIM2                  FALSE
#endif

#ifndef STM32_PWM_USE_TIM3
// Hellen often uses TIM3 for ETB
#define STM32_PWM_USE_TIM3                  TRUE
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

#ifndef STM32_PWM_USE_TIM9
#define STM32_PWM_USE_TIM9                  FALSE
#endif

/* NOTE: following does not set TIM IRQ priority in PWM mode
 * F4/F7 share IRQ lines between few timers...
 * See:
 * - ChibiOS/os/hal/ports/STM32/STM32F4xx/stm32_isr.h
 * - ChibiOS/os/hal/ports/STM32/STM32F7xx/stm32_isr.h
 * STM32_TIMx_SUPPRESS_ISR is defined for all TIMs
 * Timers IRQ priority setup is done in stm32_tim*.inc
 * files. See also tim_irq_mapping.txt for irq collision map.
 */
#define STM32_PWM_TIM1_IRQ_PRIORITY         7
#define STM32_PWM_TIM2_IRQ_PRIORITY         7
#define STM32_PWM_TIM3_IRQ_PRIORITY         7
#define STM32_PWM_TIM4_IRQ_PRIORITY         7
#define STM32_PWM_TIM8_IRQ_PRIORITY         7
#define STM32_PWM_TIM9_IRQ_PRIORITY         7

// TIM5 is used for the precise scheduler queue
#define STM32_PWM_TIM5_IRQ_PRIORITY         EFI_IRQ_SCHEDULING_TIMER_PRIORITY
#define STM32_IRQ_TIM5_PRIORITY             EFI_IRQ_SCHEDULING_TIMER_PRIORITY

// TIM6 is used for to start the fast ADC
#define STM32_GPT_TIM6_IRQ_PRIORITY         EFI_IRQ_ADC_PRIORITY
#define STM32_IRQ_TIM6_PRIORITY             EFI_IRQ_ADC_PRIORITY

/*
 * SERIAL driver system settings.
 */
#ifndef STM32_SERIAL_USE_USART1
#define STM32_SERIAL_USE_USART1             FALSE
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
#define STM32_IRQ_USART1_PRIORITY           EFI_IRQ_UART_PRIORITY
#define STM32_IRQ_USART2_PRIORITY           EFI_IRQ_UART_PRIORITY
#define STM32_IRQ_USART3_PRIORITY           EFI_IRQ_UART_PRIORITY
#define STM32_IRQ_UART4_PRIORITY            EFI_IRQ_UART_PRIORITY
#define STM32_IRQ_UART5_PRIORITY            EFI_IRQ_UART_PRIORITY
#define STM32_IRQ_USART6_PRIORITY           EFI_IRQ_UART_PRIORITY
#define STM32_IRQ_UART7_PRIORITY            EFI_IRQ_UART_PRIORITY
#define STM32_IRQ_UART8_PRIORITY            EFI_IRQ_UART_PRIORITY

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

#define STM32_SPI_SPI1_RX_DMA_STREAM        STM32_DMA_STREAM_ID(2, 0)
#define STM32_SPI_SPI1_TX_DMA_STREAM        STM32_DMA_STREAM_ID(2, 3)
#define STM32_SPI_SPI2_RX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 3)
#define STM32_SPI_SPI2_TX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 4)
#define STM32_SPI_SPI3_RX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 0)
#define STM32_SPI_SPI3_TX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 7)
#define STM32_SPI_SPI4_RX_DMA_STREAM        STM32_DMA_STREAM_ID(2, 0)
#define STM32_SPI_SPI4_TX_DMA_STREAM        STM32_DMA_STREAM_ID(2, 1)
#define STM32_SPI_SPI5_RX_DMA_STREAM        STM32_DMA_STREAM_ID(2, 3)
/* STM32_DMA_STREAM_ID(2, 4) is used by ADC1 */
//#define STM32_SPI_SPI5_TX_DMA_STREAM        STM32_DMA_STREAM_ID(2, 4)
#define STM32_SPI_SPI5_TX_DMA_STREAM        STM32_DMA_STREAM_ID(2, 6)
#define STM32_SPI_SPI6_RX_DMA_STREAM        STM32_DMA_STREAM_ID(2, 6)
#define STM32_SPI_SPI6_TX_DMA_STREAM        STM32_DMA_STREAM_ID(2, 5)
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
 * UART driver system settings.
 */
#ifndef STM32_UART_USE_USART1
#define STM32_UART_USE_USART1               FALSE
#endif

#ifndef STM32_UART_USE_USART2
#define STM32_UART_USE_USART2               FALSE
#endif

#ifndef STM32_UART_USE_USART3
#define STM32_UART_USE_USART3               TRUE
#endif

#define STM32_UART_USE_UART4                FALSE
#define STM32_UART_USE_UART5                FALSE
#define STM32_UART_USE_USART6               FALSE
#define STM32_UART_USE_UART7                FALSE

#ifndef STM32_UART_USE_UART8
#define STM32_UART_USE_UART8                FALSE
#endif

#define STM32_UART_USART1_RX_DMA_STREAM     STM32_DMA_STREAM_ID(2, 5)
#define STM32_UART_USART1_TX_DMA_STREAM     STM32_DMA_STREAM_ID(2, 7)
#define STM32_UART_USART2_RX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 5)
#define STM32_UART_USART2_TX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 6)
#define STM32_UART_USART3_RX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 1)
#define STM32_UART_USART3_TX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 3)
#define STM32_UART_UART4_RX_DMA_STREAM      STM32_DMA_STREAM_ID(1, 2)
#define STM32_UART_UART4_TX_DMA_STREAM      STM32_DMA_STREAM_ID(1, 4)
#define STM32_UART_UART5_RX_DMA_STREAM      STM32_DMA_STREAM_ID(1, 0)
#define STM32_UART_UART5_TX_DMA_STREAM      STM32_DMA_STREAM_ID(1, 7)
#define STM32_UART_USART6_RX_DMA_STREAM     STM32_DMA_STREAM_ID(2, 2)
#define STM32_UART_USART6_TX_DMA_STREAM     STM32_DMA_STREAM_ID(2, 7)
#define STM32_UART_UART7_RX_DMA_STREAM      STM32_DMA_STREAM_ID(1, 3)
#define STM32_UART_UART7_TX_DMA_STREAM      STM32_DMA_STREAM_ID(1, 1)
#define STM32_UART_UART8_RX_DMA_STREAM      STM32_DMA_STREAM_ID(1, 6)
#define STM32_UART_UART8_TX_DMA_STREAM      STM32_DMA_STREAM_ID(1, 0)
#define STM32_UART_USART1_IRQ_PRIORITY      EFI_IRQ_UART_PRIORITY
#define STM32_UART_USART2_IRQ_PRIORITY      EFI_IRQ_UART_PRIORITY
#define STM32_UART_USART3_IRQ_PRIORITY      EFI_IRQ_UART_PRIORITY
#define STM32_UART_UART4_IRQ_PRIORITY       EFI_IRQ_UART_PRIORITY
#define STM32_UART_UART5_IRQ_PRIORITY       EFI_IRQ_UART_PRIORITY
#define STM32_UART_USART6_IRQ_PRIORITY      EFI_IRQ_UART_PRIORITY
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
#ifndef STM32_USB_USE_OTG1
#define STM32_USB_USE_OTG1                  TRUE
#endif

#ifndef STM32_USB_USE_OTG2
#define STM32_USB_USE_OTG2                  FALSE
#endif

#define STM32_USB_OTG1_IRQ_PRIORITY         EFI_IRQ_USB_PRIORITY
#define STM32_USB_OTG2_IRQ_PRIORITY         EFI_IRQ_USB_PRIORITY
#define STM32_USB_OTG1_RX_FIFO_SIZE         512
#define STM32_USB_OTG2_RX_FIFO_SIZE         1024
#define STM32_USB_OTG_THREAD_STACK_SIZE     1024
#define STM32_USB_OTGFIFO_FILL_BASEPRI      0

/*
 * WDG driver system settings.
 */
#ifndef STM32_WDG_USE_IWDG
#define STM32_WDG_USE_IWDG                  TRUE
#endif

/* Some boards need to know clock early on boot.
 * F429-Discovery board configures clock and then SDRAM early on boot */
#ifndef STM32_HSECLK
    // Some boards has no HSE oscillator at all and obviously disable HSE detections
    #ifndef ENABLE_AUTO_DETECT_HSE
        // Pretend we have a 25MHz external crystal.  This value isn't actually used since we
        // configure the PLL to start on the HSI oscillator, then compute HSE's speed at runtime
        // and reconfigure the PLL appropriately.
        #define STM32_HSECLK 25000000

        // After boot, we will detect the real frequency, and adjust the PLL M value to suit
        #define ENABLE_AUTO_DETECT_HSE TRUE
    #endif
#endif
