/*
 * @file tle8888.h
 *
 * Mar 25, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#ifndef HW_LAYER_DRIVERS_GPIO_TLE8888_H_
#define HW_LAYER_DRIVERS_GPIO_TLE8888_H_

#include "global.h"

#include <hal.h>
#include "efifeatures.h"

#define TLE8888_OUTPUTS				28
/* 4 misc channels */
#define TLE8888_DIRECT_MISC			4
/* 4 IGN channels - INJ1..4 - IN1..4
 * 4 INJ channels - OUT1..4 - IN5..8 */
#define TLE8888_DIRECT_OUTPUTS		(4 + 4 + TLE8888_DIRECT_MISC)

/* DOTO: add irq support */
#define TLE8888_POLL_INTERVAL_MS	100

/* note that spi transfer should be LSB first */
struct tle8888_config {
	SPIDriver		*spi_bus;
	SPIConfig	spi_config;
	/* bidirectional, check DS */
	struct {
		ioportid_t		port;
		uint_fast8_t	pad;
	} reset;
	struct {
		/* MCU port-pin routed to IN9..12 */
		ioportid_t		port;
		uint_fast8_t	pad;
		/* ...used to drive output (starts from 1, as in DS, coders gonna hate) */
		int 			output;
	} direct_io[TLE8888_DIRECT_MISC];
	bool hallMode;
};

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @return return gpio chip base
 */
int tle8888_add(unsigned int index, const struct tle8888_config *cfg);
void requestTLE8888initialization(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HW_LAYER_DRIVERS_GPIO_TLE8888_H_ */
