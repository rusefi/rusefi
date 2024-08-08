/*
 * mc33810.h
 *
 * Automotive Engine Control IC
 *
 * @date Dec 29, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 * @date Jan 03, 2020
 * @author Andrey Gusakov <dron0gus@gmail.com>, (c) 2020
 */

#pragma once

#include "efifeatures.h"
#include <hal.h>
#include "rusefi_types.h"

#define MC33810_OUTPUTS				8
#define MC33810_DIRECT_OUTPUTS		8

/* TODO: add irq support */
#define MC33810_POLL_INTERVAL_MS	100

struct mc33810_config {
#if HAL_USE_SPI
	SPIDriver	*spi_bus;
	SPIConfig	spi_config;
#endif
	/* First 4 is injector drivers, then 4 ignition pre-drivers */
	struct {
		ioportid_t		port;
		uint_fast8_t	pad;
	} direct_io[MC33810_DIRECT_OUTPUTS];
	struct {
		ioportid_t		port;
		uint_fast8_t	pad;
	} en;
	/* See datasheet:
	 * "To avoid any spurious data, it is essential the high-to-low and low-to-high
	 * transitions of the CS signal occur only when SCLK is in a logic low state."
	 * If sck.port != null, driver will wait until SCLK goes low before releasing CS
	 * at the end of each transaction.
	 * STM32 driver/hw is known to affected by this issue */
	struct {
		ioportid_t		port;
		uint_fast8_t	pad;
	} sck;
};

int mc33810_add(brain_pin_e base, unsigned int index, const mc33810_config *cfg);

/* debug */
void mc33810_req_init();
