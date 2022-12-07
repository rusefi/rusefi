/**
 * @file	port_mpu_util.h
 *
 * @date Jul 27, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 * @author andreika <prometheus.pcb@gmail.com>
 */

#pragma once

// 168 ticks in microsecond in case of 168MHz 407
#define US_TO_NT_MULTIPLIER (CORE_CLOCK / 1000000)

// Scheduler queue GPT device
#define GPTDEVICE GPTD1

typedef enum {
	BOR_Level_None = 0,
	BOR_Level_1 = 1,
	BOR_Level_2 = 2,
	BOR_Level_3 = 3
} BOR_Level_t;

// we are lucky - all CAN pins use the same AF
#define EFI_CAN_RX_AF 9
#define EFI_CAN_TX_AF 9

#ifndef ADC_TwoSamplingDelay_5Cycles
#define ADC_TwoSamplingDelay_5Cycles ((uint32_t)0x00000000)
#endif

#ifndef ADC_TwoSamplingDelay_20Cycles
#define ADC_TwoSamplingDelay_20Cycles ((uint32_t)0x00000F00)
#endif

#ifndef ADC_CR2_SWSTART
#define ADC_CR2_SWSTART ((uint32_t)0x40000000)
#endif

#define SPI_CR1_8BIT_MODE 0
#define SPI_CR2_8BIT_MODE 0

#define SPI_CR1_16BIT_MODE SPI_CR1_DFF
#define SPI_CR2_16BIT_MODE 0

// TODO
#define SPI_CR1_24BIT_MODE 0
#define SPI_CR2_24BIT_MODE 0

#define ADC_MAX_VALUE 4095
