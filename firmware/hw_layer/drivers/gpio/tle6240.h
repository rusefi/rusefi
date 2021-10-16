/*
 * tle6240.h
 *
 * TLE6240GP Smart 16-Channel Low-Side Switch
 *
 * @date Dec 29, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "efifeatures.h"
#include <hal.h>

#define TLE6240_OUTPUTS				16
#define TLE6240_DIRECT_OUTPUTS		8

/* DOTO: add irq support */
#define TLE6240_POLL_INTERVAL_MS	100

struct tle6240_config {
#if HAL_USE_SPI
	SPIDriver		*spi_bus;
	SPIConfig	spi_config;
#endif
	struct {
		ioportid_t		port;
		uint_fast8_t	pad;
	} direct_io[TLE6240_DIRECT_OUTPUTS];
	struct {
		ioportid_t		port;
		uint_fast8_t	pad;
	} reset;
};

int tle6240_add(brain_pin_e base, unsigned int index, const struct tle6240_config *cfg);
