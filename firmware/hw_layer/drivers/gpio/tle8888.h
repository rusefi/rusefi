/*
 * @file tle8888.h
 *
 * Mar 25, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"

#include <hal.h>
#include "efifeatures.h"

#define TLE8888_OUTPUTS				28
/* 4 misc channels */
#define TLE8888_DIRECT_MISC			4
/* 4 IGN channels - INJ1..4 - IN1..4
 * 4 INJ channels - OUT1..4 - IN5..8 */
#define TLE8888_DIRECT_OUTPUTS		(4 + 4 + TLE8888_DIRECT_MISC)

#define getRegisterFromResponse(x) (((x) >> 1) & 0x7f)
#define getDataFromResponse(x) (((x) >> 8) & 0xff)


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
		/* MCU port-pin routed to IN1..12 */
		ioportid_t		port;
		uint_fast8_t	pad;
	} direct_gpio[TLE8888_DIRECT_OUTPUTS];
	/* IN9..IN12 to output mapping */
	struct {
		/* ...used to drive output (starts from 1, as in DS, coders gonna hate) */
		int 			output;
	} direct_maps[TLE8888_DIRECT_MISC];
	struct {
		ioportid_t		port;
		uint_fast8_t	pad;
	} ign_en;
	struct {
		ioportid_t		port;
		uint_fast8_t	pad;
	} inj_en;
	tle8888_mode_e mode;
};

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @return return gpio chip base
 */
int tle8888_add(unsigned int index, const struct tle8888_config *cfg);

/* debug */
void tle8888_read_reg(uint16_t reg, uint16_t *val);
void tle8888_req_init(void);

#if EFI_TUNER_STUDIO
#include "tunerstudio_debug_struct.h"
void tle8888PostState(TsDebugChannels *tsDebugChannels);
#endif /* EFI_TUNER_STUDIO */

#ifdef __cplusplus
}
#endif /* __cplusplus */

