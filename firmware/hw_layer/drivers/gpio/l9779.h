/*
 * l9779.h
 * ST L9779WD-SPI
 *
 *
 *  Created on: Jan 10, 2022
 */

#pragma once

#include "global.h"

#include <hal.h>
#include "efifeatures.h"

struct l9779_config {
#if HAL_USE_SPI
	SPIDriver		*spi_bus;
	SPIConfig		spi_config;
#endif
};

#define L9779_OUTPUTS_IGN   (4)
#define L9779_OUTPUTS       (L9779_OUTPUTS_IGN + 28 + 1)
#define L9779_INPUTS        (1)

#define L9779_SIGNALS       (L9779_OUTPUTS + L9779_INPUTS)

int l9779_add(brain_pin_e base, unsigned int index, const l9779_config *cfg);
