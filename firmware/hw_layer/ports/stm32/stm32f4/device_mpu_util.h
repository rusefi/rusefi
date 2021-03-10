/**
 * @file	device_mpu_util.h
 *
 * @date Jul 27, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "stm32f4xx_hal_flash_ex.h"

#define MCU_SERIAL_NUMBER_LOCATION (uint8_t*)(0x1FFF7A10)

#define SPI_CR1_8BIT_MODE 0
#define SPI_CR2_8BIT_MODE 0

#define SPI_CR1_16BIT_MODE SPI_CR1_DFF
#define SPI_CR2_16BIT_MODE 0

// TODO
#define SPI_CR1_24BIT_MODE 0
#define SPI_CR2_24BIT_MODE 0

#define ADC_MAX_VALUE 4095
