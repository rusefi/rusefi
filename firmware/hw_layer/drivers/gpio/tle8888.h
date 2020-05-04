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

// Looks like reset value is 113.6ms? 1.6ms * 0x47
#define Functional_Watchdog_PERIOD_MS 20

// we can change this value on TLE8888QK but we probably do not have a reason to change
#define Window_watchdog_close_window_time_ms 100.8

#define getRegisterFromResponse(x) (((x) >> 1) & 0x7f)

// unchangeable value for TLE8888QK
// unused for now
//#define Window_watchdog_open_window_time_ms 12.8

/* DOTO: add irq support */
#define TLE8888_POLL_INTERVAL_MS	7

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
void requestTLE8888initialization(void);

#if EFI_TUNER_STUDIO
#include "tunerstudio_debug_struct.h"
void tle8888PostState(TsDebugChannels *tsDebugChannels);
#endif /* EFI_TUNER_STUDIO */

#ifdef __cplusplus
}
#endif /* __cplusplus */

