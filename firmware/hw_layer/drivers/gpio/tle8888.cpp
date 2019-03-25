/**
 * @date Mar 25, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#define TLE8888_CmdOE 0x1C

#include "engine.h"
#include "tle8888.h"

EXTERN_ENGINE;

static SPIDriver *driver;

static SPIConfig spiConfig;

void initTle8888(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (engineConfiguration->tle8888_cs == GPIO_UNASSIGNED) {
		return;
	}

}
