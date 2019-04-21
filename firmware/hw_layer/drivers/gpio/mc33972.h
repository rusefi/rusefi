/*
 * mc33972.h
 *
 * Multiple Switch Detection Interface with Suppressed Wake-up
 *
 * @date Apr 07, 2019
 * @author Andrey Gusakov <dron0gus@gmail.com>, (c) 2019
 */

#ifndef HW_LAYER_MC33972_H_
#define HW_LAYER_MC33972_H_

#include <hal.h>
#include "efifeatures.h"

#define MC33972_INPUTS				22

/* DOTO: add irq support */
#define MC33972_POLL_INTERVAL_MS	100

struct mc33972_config {
	SPIDriver		*spi_bus;
	SPIConfig	spi_config;
};

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

int mc33972_add(unsigned int index, const struct mc33972_config *cfg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HW_LAYER_MC33972_H_ */
