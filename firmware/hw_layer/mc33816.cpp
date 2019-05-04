/*
 * @file mc33816.cpp
 *
 * The NXP MC33816 is a programmable gate driver IC for precision solenoid control applications.
 *
 * @date May 3, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "global.h"

#if EFI_MC33816

#include "mc33816.h"
#include "engine_configuration.h"
#include "efi_gpio.h"
#include "hardware.h"

EXTERN_CONFIG;

static OutputPin chipSelect;

static SPIConfig spiCfg = { .circular = false,
		.end_cb = NULL,
		.ssport = NULL,
		.sspad = 0,
		.cr1 = SPI_CR1_MSTR |
//SPI_CR1_BR_1 // 5MHz
		SPI_CR1_CPHA | SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2,
		.cr2 = 0};

static SPIDriver *driver;


static void showStats() {

}

static void sentByte(int param) {
	spiSelect(driver);
	spiSend(driver, 1, &param);
	spiUnselect(driver);
}

void initMc33816() {
	// default spi3mosiPin PB5
	// default spi3misoPin PB4
	// default spi3sckPin  PB3
	// ideally disable isSdCardEnabled since it's on SPI3

	// uncomment thid to hard-code something
	//CONFIG(mc33816_cs) = GPIOD_7;

	if (CONFIG(mc33816_cs) == GPIO_UNASSIGNED)
		return;

	chipSelect.initPin("mc33 CS", engineConfiguration->mc33816_cs /*, &engineConfiguration->csPinMode*/);

	spiCfg.ssport = getHwPort("hip", CONFIG(mc33816_cs));
	spiCfg.sspad = getHwPin("hip", CONFIG(mc33816_cs));

	// hard-coded for now, just resolve the conflict with SD card!
	engineConfiguration->mc33816spiDevice = SPI_DEVICE_3;

	driver = getSpiDevice(engineConfiguration->mc33816spiDevice);
	if (driver == NULL) {
		// error already reported
		return;
	}

	spiStart(driver, &spiCfg);

	addConsoleAction("mc33_stats", showStats);

	addConsoleActionI("mc33_send", sentByte);

}

#endif /* EFI_MC33816 */
