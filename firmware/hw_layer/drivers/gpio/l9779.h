/*
 * l9779.h
 * ST L9779WD-SPI
 *
 *
 *  Created on: Jan 10, 2022
 */

#pragma once

#include "efifeatures.h"
#include <hal.h>

struct l9779_config {
#if HAL_USE_SPI
	SPIDriver		*spi_bus;
#endif
};
