/*
 * @file mc33816.cpp
 *
 * The NXP MC33816 is a programmable gate driver IC for precision solenoid control applications.
 *
 *
 * For MC33816 vs PT2000 differences see
 * https://www.nxp.com/docs/en/application-note/AN5203.pdf
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
#include "mc33816_data.h"
#include "mpu_util.h"

EXTERN_CONFIG;

static OutputPin chipSelect;

static SPIConfig spiCfg = { .circular = false,
		.end_cb = NULL,
		.ssport = NULL,
		.sspad = 0,
		.cr1 =
				SPI_CR1_16BIT_MODE |
				SPI_CR1_MSTR |
//SPI_CR1_BR_1 // 5MHz
		SPI_CR1_CPHA | SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2,
		.cr2 = 0};

static SPIDriver *driver;


static void showStats() {

}

static void send_16bit_SPI(short param) {
	spiSelect(driver);
	// we are in 16 bit mode so '1' means 16 bits word hopefully?
	spiSend(driver, 1, &param);
	spiUnselect(driver);
}

static void sentWord(int word) {
	send_16bit_SPI(word);
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

	addConsoleActionI("mc33_send", sentWord);

	int size = 105;

	unsigned short * RAM_ptr = MC33816_code_RAM1;

    for (int k = 0; k < size; k++)            // downloads RAM
    {
        short data = *RAM_ptr;                  // retrieves data to be sent
        send_16bit_SPI(data);             // sends data
        RAM_ptr++;
    }


}

#endif /* EFI_MC33816 */
