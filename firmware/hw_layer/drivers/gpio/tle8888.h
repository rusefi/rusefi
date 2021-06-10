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

#define TLE8888_OUTPUTS_REGULAR		28

#define TLE8888_OUTPUT_MR			(TLE8888_OUTPUTS_REGULAR + 0)

/* regular outputs + MR output */
#define TLE8888_OUTPUTS				(TLE8888_OUTPUTS_REGULAR + 1)
/* 4 misc channels */
#define TLE8888_DIRECT_MISC			4
/* 4 IGN channels - INJ1..4 - IN1..4
 * 4 INJ channels - OUT1..4 - IN5..8 */
#define TLE8888_DIRECT_OUTPUTS		(4 + 4 + TLE8888_DIRECT_MISC)

/* Inputs */
#define TLE8888_INPUT_KEY			(TLE8888_OUTPUTS + 0)
#define TLE8888_INPUT_WAKE			(TLE8888_OUTPUTS + 1)

/* KEY and WAKE */
#define TLE8888_INPUTS				2

#define TLE8888_SIGNALS				(TLE8888_OUTPUTS + TLE8888_INPUTS)

#define getRegisterFromResponse(x) (((x) >> 1) & 0x7f)
#define getDataFromResponse(x) (((x) >> 8) & 0xff)


/* note that spi transfer should be LSB first */
struct tle8888_config {
#if HAL_USE_SPI
	SPIDriver		*spi_bus;
	SPIConfig	spi_config;
#endif
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
		uint8_t 			output;
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
	/* this is workaround to enable PP mode for OUT21..OUT24
	 * until users won't call setPinMode */
	bool			stepper;
};

/**
 * @return return gpio chip base
 */
int tle8888_add(brain_pin_e base, unsigned int index, const struct tle8888_config *cfg);

/* debug */
void tle8888_req_init();
void tle8888_dump_regs();

#if EFI_TUNER_STUDIO
#include "tunerstudio_debug_struct.h"
void tle8888PostState(TsDebugChannels *tsDebugChannels);
#endif /* EFI_TUNER_STUDIO */
