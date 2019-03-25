/**
 * @date Mar 25, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "engine.h"
#include "tle8888.h"

/**
 * 15.2 SPI Frame Definition
 *
 */
#define Cmd_write_access 1
#define CmdOE 0x1C
#define DATA_OE_ENABLE 00000010b

static unsigned char tx_buff[2];

EXTERN_ENGINE;

static SPIDriver *driver;

static SPIConfig spiConfig;

void initTle8888(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (engineConfiguration->tle8888_cs == GPIO_UNASSIGNED) {
		return;
	}

}
