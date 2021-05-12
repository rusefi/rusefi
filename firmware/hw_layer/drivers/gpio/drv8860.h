/*
 * drv8860.h
 *
 * DRV8860 Smart 8/16-Channel Low-Side Switch
 *
 * @date Apr 6, 2020
 *
 * @author andreika, (c) 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#ifndef HW_LAYER_DRV8860_H_
#define HW_LAYER_DRV8860_H_

#include "efifeatures.h"
#include <hal.h>

#define DRV8860_OUTPUTS				16

/* TODO: add irq support */
#define DRV8860_POLL_INTERVAL_MS	500

struct drv8860_config {
#if HAL_USE_SPI
	SPIDriver		*spi_bus;
	SPIConfig	spi_config;
#endif
	struct {
		ioportid_t		port;
		uint_fast8_t	pad;
	} reset;
};

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

int drv8860_add(brain_pin_e base, unsigned int index, const struct drv8860_config *cfg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HW_LAYER_DRV8860_H_ */
