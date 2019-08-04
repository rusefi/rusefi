/*
 * @file	mpu_util.h
 *
 * @date Jan 8, 2015
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef CONFIG_STM32F1EGT_MPU_UTIL_H_
#define CONFIG_STM32F1EGT_MPU_UTIL_H_

#include "global.h"

#define baseMCUInit() {}

void initSpiCs(SPIConfig *spiConfig, brain_pin_e csPin);

#endif /* CONFIG_STM32F1EGT_MPU_UTIL_H_ */
