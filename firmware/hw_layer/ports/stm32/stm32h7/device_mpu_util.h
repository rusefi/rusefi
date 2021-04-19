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

#define ADC_MAX_VALUE 65535
