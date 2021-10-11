/**
 * @file	mcuconf_common_f4_f7.cpp
 * @brief	Common MCU definitions for both the STM32F4 and STM32F7
 *
 * @date Feb 18, 2021
 * @author Matthew Kennedy
 */

#pragma once

#include "efifeatures.h"

/**
 * This is about SingleTimerExecutor - rusEfi 12MHz precise scheduling timer
 * this is not about ChibiOS virtual timer which rusEfi uses for
 * periodicFastCallback and periodicSlowCallback
 * See STM32_ST_IRQ_PRIORITY
 *
 * http://www.chibios.org/dokuwiki/doku.php?id=chibios:kb:priority
 */
#define PRECISE_SCHEDULING_TIMER_PRIORITY 4
/**
 * Input Capture Unit is how we capture shaft position sensors output
 */
#define ICU_PRIORITY 3

/**
 * UART DMA-mode byte capture (low latency)
 */
#define UART_DMA_IRQ_PRIORITY (PRECISE_SCHEDULING_TIMER_PRIORITY + 2)

/*
 * SysTick driver system settings.
 */
#define STM32_ST_IRQ_PRIORITY               8
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
#define STM32_ADC_ADC1_DMA_PRIORITY         2
#define STM32_ADC_ADC2_DMA_PRIORITY         2
#define STM32_ADC_ADC3_DMA_PRIORITY         2
#define STM32_ADC_IRQ_PRIORITY              PRECISE_SCHEDULING_TIMER_PRIORITY + 2
#define STM32_ADC_ADC1_DMA_IRQ_PRIORITY     PRECISE_SCHEDULING_TIMER_PRIORITY + 2
#define STM32_ADC_ADC2_DMA_IRQ_PRIORITY     PRECISE_SCHEDULING_TIMER_PRIORITY + 2
#define STM32_ADC_ADC3_DMA_IRQ_PRIORITY     PRECISE_SCHEDULING_TIMER_PRIORITY + 2

/*
 * CAN driver system settings.
 */
#define STM32_CAN_USE_CAN1                  TRUE
#define STM32_CAN_USE_CAN2                  TRUE
#define STM32_CAN_USE_CAN3                  FALSE
#define STM32_CAN_CAN1_IRQ_PRIORITY         PRECISE_SCHEDULING_TIMER_PRIORITY + 7
#define STM32_CAN_CAN2_IRQ_PRIORITY         PRECISE_SCHEDULING_TIMER_PRIORITY + 7
#define STM32_CAN_CAN3_IRQ_PRIORITY         PRECISE_SCHEDULING_TIMER_PRIORITY + 7

/*
 * DAC driver system settings.
 */
#define STM32_DAC_DUAL_MODE                 FALSE
#define STM32_DAC_USE_DAC1_CH1              FALSE
#define STM32_DAC_USE_DAC1_CH2              FALSE
#define STM32_DAC_DAC1_CH1_IRQ_PRIORITY     PRECISE_SCHEDULING_TIMER_PRIORITY + 6
#define STM32_DAC_DAC1_CH2_IRQ_PRIORITY     PRECISE_SCHEDULING_TIMER_PRIORITY + 6
#define STM32_DAC_DAC1_CH1_DMA_PRIORITY     2
#define STM32_DAC_DAC1_CH2_DMA_PRIORITY     2
#define STM32_DAC_DAC1_CH1_DMA_STREAM       STM32_DMA_STREAM_ID(1, 5)
#define STM32_DAC_DAC1_CH2_DMA_STREAM       STM32_DMA_STREAM_ID(1, 6)

/*
 * EXT driver system settings.
 */
#define STM32_IRQ_EXTI_PRIORITY             ICU_PRIORITY
#define STM32_IRQ_EXTI0_PRIORITY            STM32_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI1_PRIORITY            STM32_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI2_PRIORITY            STM32_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI3_PRIORITY            STM32_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI4_PRIORITY            STM32_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI5_9_PRIORITY          STM32_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI10_15_PRIORITY        STM32_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI16_PRIORITY           STM32_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI17_PRIORITY           15  /* why? */
#define STM32_IRQ_EXTI18_PRIORITY           STM32_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI19_PRIORITY           STM32_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI20_PRIORITY           STM32_IRQ_EXTI_PRIORITY
#define STM32_IRQ_EXTI21_PRIORITY           15  /* why? */
#define STM32_IRQ_EXTI22_PRIORITY           15  /* why? */
#define STM32_IRQ_EXTI23_PRIORITY           STM32_IRQ_EXTI_PRIORITY

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
#define STM32_I2C_USE_I2C1                  FALSE
#define STM32_I2C_USE_I2C2                  FALSE
#define STM32_I2C_USE_I2C3                  FALSE
#define STM32_I2C_USE_I2C4                  FALSE
#define STM32_I2C_BUSY_TIMEOUT              50
#define STM32_I2C_I2C1_RX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 0)
#define STM32_I2C_I2C1_TX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 6)
#define STM32_I2C_I2C2_RX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 2)
#define STM32_I2C_I2C2_TX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 7)
#define STM32_I2C_I2C3_RX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 2)
#define STM32_I2C_I2C3_TX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 4)
#define STM32_I2C_I2C4_RX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 2)
#define STM32_I2C_I2C4_TX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 5)
#define STM32_I2C_I2C1_IRQ_PRIORITY         5
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
#define STM32_ICU_USE_TIM1                  TRUE
#define STM32_ICU_USE_TIM2                  TRUE
#define STM32_ICU_USE_TIM3                  TRUE
#define STM32_ICU_USE_TIM4                  FALSE
#define STM32_ICU_USE_TIM5                  FALSE
#define STM32_ICU_USE_TIM8                  FALSE
#define STM32_ICU_USE_TIM9                  FALSE
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
#define STM32_MAC_BUFFERS_SIZE              1536
#define STM32_MAC_PHY_TIMEOUT               100
#define STM32_MAC_ETH1_CHANGE_PHY_STATE     TRUE
#define STM32_MAC_ETH1_IRQ_PRIORITY         13
#define STM32_MAC_IP_CHECKSUM_OFFLOAD       3

/*
 * PWM driver system settings.
 */
#define STM32_PWM_USE_ADVANCED              FALSE
#define STM32_PWM_USE_TIM1                  FALSE
#define STM32_PWM_USE_TIM2                  FALSE
#define STM32_PWM_USE_TIM3                  FALSE
#define STM32_PWM_USE_TIM4                  TRUE
#define STM32_PWM_USE_TIM5                  TRUE
#define STM32_PWM_USE_TIM8                  TRUE
#define STM32_PWM_USE_TIM9                  FALSE
#define STM32_PWM_TIM1_IRQ_PRIORITY         7
#define STM32_PWM_TIM2_IRQ_PRIORITY         7
#define STM32_PWM_TIM3_IRQ_PRIORITY         7
#define STM32_PWM_TIM4_IRQ_PRIORITY         7
#define STM32_PWM_TIM5_IRQ_PRIORITY         PRECISE_SCHEDULING_TIMER_PRIORITY
#define STM32_PWM_TIM8_IRQ_PRIORITY         7
#define STM32_PWM_TIM9_IRQ_PRIORITY         7

/*
 * SERIAL driver system settings.
 */
#define STM32_SERIAL_USE_USART1             TRUE
#define STM32_SERIAL_USE_USART2             FALSE
#define STM32_SERIAL_USE_USART3             FALSE
#define STM32_SERIAL_USE_UART4              FALSE
#define STM32_SERIAL_USE_UART5              FALSE
#define STM32_SERIAL_USE_USART6             TRUE
#define STM32_SERIAL_USE_UART7              FALSE
#define STM32_SERIAL_USE_UART8              FALSE
#define STM32_IRQ_USART1_PRIORITY           (PRECISE_SCHEDULING_TIMER_PRIORITY + 2)
#define STM32_IRQ_USART2_PRIORITY           (PRECISE_SCHEDULING_TIMER_PRIORITY + 2)
#define STM32_IRQ_USART3_PRIORITY           (PRECISE_SCHEDULING_TIMER_PRIORITY + 2)
#define STM32_IRQ_UART4_PRIORITY            (PRECISE_SCHEDULING_TIMER_PRIORITY + 2)
#define STM32_IRQ_UART5_PRIORITY            (PRECISE_SCHEDULING_TIMER_PRIORITY + 2)
#define STM32_IRQ_USART6_PRIORITY           (PRECISE_SCHEDULING_TIMER_PRIORITY + 2)
#define STM32_IRQ_UART7_PRIORITY            (PRECISE_SCHEDULING_TIMER_PRIORITY + 2)
#define STM32_IRQ_UART8_PRIORITY            (PRECISE_SCHEDULING_TIMER_PRIORITY + 2)

/*
 * SPI driver system settings.
 */
#define STM32_SPI_USE_SPI1                  TRUE
#define STM32_SPI_USE_SPI2                  TRUE
#define STM32_SPI_USE_SPI3                  TRUE
#define STM32_SPI_USE_SPI4                  FALSE
#define STM32_SPI_USE_SPI5                  FALSE
#define STM32_SPI_USE_SPI6                  FALSE
#define STM32_SPI_SPI1_RX_DMA_STREAM        STM32_DMA_STREAM_ID(2, 0)
#define STM32_SPI_SPI1_TX_DMA_STREAM        STM32_DMA_STREAM_ID(2, 3)
#define STM32_SPI_SPI2_RX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 3)
#define STM32_SPI_SPI2_TX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 4)
#define STM32_SPI_SPI3_RX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 0)
#define STM32_SPI_SPI3_TX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 7)
#define STM32_SPI_SPI4_RX_DMA_STREAM        STM32_DMA_STREAM_ID(2, 0)
#define STM32_SPI_SPI4_TX_DMA_STREAM        STM32_DMA_STREAM_ID(2, 1)
#define STM32_SPI_SPI5_RX_DMA_STREAM        STM32_DMA_STREAM_ID(2, 3)
#define STM32_SPI_SPI5_TX_DMA_STREAM        STM32_DMA_STREAM_ID(2, 4)
#define STM32_SPI_SPI6_RX_DMA_STREAM        STM32_DMA_STREAM_ID(2, 6)
#define STM32_SPI_SPI6_TX_DMA_STREAM        STM32_DMA_STREAM_ID(2, 5)
#define STM32_SPI_SPI1_DMA_PRIORITY         1
#define STM32_SPI_SPI2_DMA_PRIORITY         1
#define STM32_SPI_SPI3_DMA_PRIORITY         1
#define STM32_SPI_SPI4_DMA_PRIORITY         1
#define STM32_SPI_SPI5_DMA_PRIORITY         1
#define STM32_SPI_SPI6_DMA_PRIORITY         1
#define STM32_SPI_SPI1_IRQ_PRIORITY         10
#define STM32_SPI_SPI2_IRQ_PRIORITY         10
#define STM32_SPI_SPI3_IRQ_PRIORITY         10
#define STM32_SPI_SPI4_IRQ_PRIORITY         10
#define STM32_SPI_SPI5_IRQ_PRIORITY         10
#define STM32_SPI_SPI6_IRQ_PRIORITY         10
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
#define STM32_UART_USART1_IRQ_PRIORITY      UART_DMA_IRQ_PRIORITY
#define STM32_UART_USART2_IRQ_PRIORITY      UART_DMA_IRQ_PRIORITY
#define STM32_UART_USART3_IRQ_PRIORITY      UART_DMA_IRQ_PRIORITY
#define STM32_UART_UART4_IRQ_PRIORITY       UART_DMA_IRQ_PRIORITY
#define STM32_UART_UART5_IRQ_PRIORITY       UART_DMA_IRQ_PRIORITY
#define STM32_UART_USART6_IRQ_PRIORITY      UART_DMA_IRQ_PRIORITY
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
#define STM32_USB_USE_OTG1                  TRUE
#define STM32_USB_USE_OTG2                  FALSE
#define STM32_USB_OTG1_IRQ_PRIORITY         14
#define STM32_USB_OTG2_IRQ_PRIORITY         14
#define STM32_USB_OTG1_RX_FIFO_SIZE         512
#define STM32_USB_OTG2_RX_FIFO_SIZE         1024
#define STM32_USB_OTG_THREAD_STACK_SIZE     1024
#define STM32_USB_OTGFIFO_FILL_BASEPRI      0

/*
 * WDG driver system settings.
 */
#define STM32_WDG_USE_IWDG                  FALSE

// Pretend we have a 25MHz external crystal.  This value isn't actually used since we
// configure the PLL to start on the HSI oscillator, then compute HSE's speed at runtime
// and reconfigure the PLL appropriately.
#define STM32_HSECLK 25000000

// After boot, we will detect the real frequency, and adjust the PLL M value to suit

#define ENABLE_AUTO_DETECT_HSE

