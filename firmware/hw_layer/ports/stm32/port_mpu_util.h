/**
 * @file	stm32_common_mpu_util.h
 * @brief	Low level common STM32 header
 *
 * @date Aug 3, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "device_mpu_util.h"

#define MCU_SERIAL_NUMBER_BYTES 12

// 4mhz was chosen because it's the GCD of (84, 108, 200), the three speeds of STM32 TIM5 clock currently supported
// https://www.wolframalpha.com/input/?i=common+factors+of+168+180+216
#define US_TO_NT_MULTIPLIER (4)

// Scheduler queue timer - use TIM5
#define SCHEDULER_PWM_DEVICE PWMD5
#define SCHEDULER_TIMER_DEVICE TIM5
#define SCHEDULER_TIMER_FREQ (US_TO_NT_MULTIPLIER * 1'000'000)

typedef enum {
	BOR_Level_None = OB_BOR_OFF, // 0x0C=12  Supply voltage ranges from 1.62 to 2.10 V
	BOR_Level_1 = OB_BOR_LEVEL1, // 0x08     Supply voltage ranges from 2.10 to 2.40 V
	BOR_Level_2 = OB_BOR_LEVEL2, // 0x04     Supply voltage ranges from 2.40 to 2.70 V
	BOR_Level_3 = OB_BOR_LEVEL3  // 0x00     Supply voltage ranges from 2.70 to 3.60 V
} BOR_Level_t;

// we are lucky - all CAN pins use the same AF
#define EFI_CAN_RX_AF 9
#define EFI_CAN_TX_AF 9

#ifndef GPIO_AF_TIM1
#define GPIO_AF_TIM1 1
#endif

#ifndef GPIO_AF_TIM2
#define GPIO_AF_TIM2 1
#endif

#ifndef GPIO_AF_TIM3
#define GPIO_AF_TIM3 2
#endif

#ifndef GPIO_AF_TIM4
#define GPIO_AF_TIM4 2
#endif

#ifndef GPIO_AF_TIM5
#define GPIO_AF_TIM5 2
#endif

#ifndef GPIO_AF_TIM8
#define GPIO_AF_TIM8 3
#endif

#ifndef GPIO_AF_TIM9
#define GPIO_AF_TIM9 3
#endif

#ifndef ADC_CR2_SWSTART
#define ADC_CR2_SWSTART ((uint32_t)0x40000000)
#endif
