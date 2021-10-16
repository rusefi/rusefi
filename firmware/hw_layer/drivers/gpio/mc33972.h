/*
 * mc33972.h
 *
 * Multiple Switch Detection Interface with Suppressed Wake-up
 *
 * @date Apr 07, 2019
 * @author Andrey Gusakov <dron0gus@gmail.com>, (c) 2019
 */

#pragma once

#include <hal.h>
#include "efifeatures.h"

#define MC33972_INPUTS				22

/* DOTO: add irq support */
#define MC33972_POLL_INTERVAL_MS	100

struct mc33972_config {
#if HAL_USE_SPI
	SPIDriver		*spi_bus;
	SPIConfig	spi_config;
#endif
};

int mc33972_add(brain_pin_e base, unsigned int index, const struct mc33972_config *cfg);
