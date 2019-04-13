/*
 * tle6240.h
 *
 * TLE6240GP Smart 16-Channel Low-Side Switch
 *
 * @date Dec 29, 2018
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#ifndef HW_LAYER_TLE6240_H_
#define HW_LAYER_TLE6240_H_

#include "efifeatures.h"
#include <hal.h>

#define TLE6240_OUTPUTS				16
#define TLE6240_DIRECT_OUTPUTS		8

/* DOTO: add irq support */
#define TLE6240_POLL_INTERVAL_MS	100

struct tle6240_config {
	SPIDriver		*spi_bus;
	const SPIConfig	spi_config;
	struct {
		ioportid_t		port;
		uint_fast8_t	pad;
	} direct_io[TLE6240_DIRECT_OUTPUTS];
	struct {
		ioportid_t		port;
		uint_fast8_t	pad;
	} reset;
};

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

int tle6240_add(unsigned int index, const struct tle6240_config *cfg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HW_LAYER_TLE6240_H_ */
