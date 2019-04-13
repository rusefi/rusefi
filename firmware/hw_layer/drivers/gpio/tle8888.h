/*
 * tle8888.h
 *
 *  Created on: Mar 25, 2019
 *      Author: garage
 */

#ifndef HW_LAYER_DRIVERS_GPIO_TLE8888_H_
#define HW_LAYER_DRIVERS_GPIO_TLE8888_H_

#include "global.h"

#include <hal.h>
#include "efifeatures.h"

#define TLE8888_OUTPUTS				24
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
};

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* cleanup !!!! */
void initTle8888(DECLARE_ENGINE_PARAMETER_SIGNATURE);

/**
 * @return return gpio chip base
 */
int tle8888_add(unsigned int index, const struct tle8888_config *cfg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HW_LAYER_DRIVERS_GPIO_TLE8888_H_ */
