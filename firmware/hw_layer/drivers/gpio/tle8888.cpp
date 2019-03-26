/**
 *
 * TLE8888 driver
 *
 * at the moment only Enable Outputs is implemented - this command is needed to get parallel control pins
 *
 * @date Mar 25, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "engine.h"
#include "tle8888.h"
#include "hardware.h"

/**
 * 15.2 SPI Frame Definition
 *
 */
#define Cmd_write_access 1
#define CmdOE 0x1C
#define DATA_OE_ENABLE 0b00000010

static unsigned char tx_buff[2];

EXTERN_ENGINE;

static SPIDriver *driver;

static OutputPin tle8888Cs;

static SPIConfig spiConfig = { NULL,
	/* HW dependent part.*/
	NULL, 0, SPI_CR1_MSTR | SPI_CR1_CPHA | SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2 };

void initTle8888(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (engineConfiguration->tle8888_cs == GPIO_UNASSIGNED) {
		return;
	}
	tle8888Cs.initPin("tle8888 CS", engineConfiguration->tle8888_cs,
			&engineConfiguration->tle8888_csPinMode);

	//spiConfig.cr1 = SPI_BaudRatePrescaler_8;

	driver = getSpiDevice(engineConfiguration->tle8888spiDevice);
	spiConfig.ssport = getHwPort("tle8888", engineConfiguration->tle8888_cs);
	spiConfig.sspad = getHwPin("tle8888", engineConfiguration->tle8888_cs);

	spiStart(driver, &spiConfig);

	tx_buff[0] = Cmd_write_access + CmdOE;
	tx_buff[1] = DATA_OE_ENABLE;
	// todo: extract 'sendSync' method?
	spiSelect(driver);
	spiSend(driver, 2, tx_buff);
	spiUnselect(driver);
}
