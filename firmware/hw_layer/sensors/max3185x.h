/**
 * @file max3185x.h
 *
 * @date Sep 17, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"
#include "engine_configuration.h"

#if HAL_USE_SPI
void initMax3185x(spi_device_e device, egt_cs_array_t max31855_cs);
void stopMax3185x();
void startMax3185x(spi_device_e device, egt_cs_array_t max31855_cs);
#endif /* HAL_USE_SPI */
