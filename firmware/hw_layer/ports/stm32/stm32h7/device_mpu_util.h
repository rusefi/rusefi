/**
 * @file	device_mpu_util.h
 *
 * @date Feb 5, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#pragma once

#include "stm32h7xx_hal_flash_ex.h"

#define MCU_SERIAL_NUMBER_LOCATION (uint8_t*)(0x1FF1E800)

// todo SPI! #2284

#define SPI_CFG1_8BIT_MODE (SPI_CFG1_DSIZE_2 | SPI_CFG1_DSIZE_1 | SPI_CFG1_DSIZE_0)
#define SPI_CFG2_8BIT_MODE 0

#define SPI_CFG1_16BIT_MODE (SPI_CFG1_DSISE_3 | SPI_CFG1_DSIZE_2 | SPI_CFG1_DSIZE_1 | SPI_CFG1_DSIZE_0)
#define SPI_CFG2_16BIT_MODE 0

/* 3 x 8-bit transfer */
#define SPI_CFG1_24BIT_MODE (SPI_CFG1_DSIZE_2 | SPI_CFG1_DSIZE_1 | SPI_CFG1_DSIZE_0)
#define SPI_CFG2_24BIT_MODE 0

#define ADC_MAX_VALUE 65535
