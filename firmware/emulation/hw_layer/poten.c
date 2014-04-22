/**
 * @file	poten.c
 * @brief	MCP42010 digital potentiometer driver
 *
 * @date Mar 16, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "poten.h"
#include "eficonsole.h"
#include "pin_repository.h"

/**
 * MCP42010 digital potentiometer driver
 *
 *
 * 1	CS		pin select						PB12
 * 2	SCK		serial clock					PA5			PC10
 * 3	SI		serial input		(MOSI)		PA7			PC12
 * 4	Vss 	ground
 * 5	PB1
 * 6	PW1
 * 7	PA1
 * 8	PA0
 * 9	PW0
 * 10	PB0
 * 11	RS		Reset
 *
 * 14	Vdd 	V input
 *
 * Rwa = 10000 * (256 - d) / 256 + 52
 * d = 256 - (Rwa - 52) * 256 / 10000
 *
 */

//#define POTEN_CS_PORT GPIOB
//#define POTEN_CS_PIN 12


//#define POT_SPI &SPID1

// PA13 & PA14 are system pins

// chip select
#define POTEN_CS_PORT GPIOE
#define POTEN_CS_PIN 15

#define _POT_SPI &SPID2

//// chip select
//#define POTEN_CS_PORT GPIOA
//#define POTEN_CS_PIN 10
//#define POT_SPI &SPID3


/* Low speed SPI configuration (281.250kHz, CPHA=0, CPOL=0, MSb first).*/
#define SPI_POT_CONFIG SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_DFF

static Logging logger;


#if EFI_POTENTIOMETER
Mcp42010Driver config0;

void initPotentiometer(Mcp42010Driver *driver, SPIDriver *spi, ioportid_t port, ioportmask_t pin) {
	driver->spiConfig.end_cb = NULL;
	driver->spiConfig.ssport = port;
	driver->spiConfig.sspad = pin;
	driver->spiConfig.cr1 = SPI_POT_CONFIG;
	driver->spi = spi;
	mySetPadMode("pot chip select", port, pin, PAL_STM32_MODE_OUTPUT);
}

static int getPotStep(int resistanceWA) {
	return 256 - (int) ((resistanceWA - 52) * 256 / 10000);
}


static void sendToPot(Mcp42010Driver *driver, int channel, int value) {
	spiStart(driver->spi, &driver->spiConfig);
	spiSelect(driver->spi);
	int word = (17 + channel) * 256 + value;
	spiSend(driver->spi, 1, &word);
	spiUnselect(driver->spi);
	spiStop(driver->spi);
}

void setPotResistance(Mcp42010Driver *driver, int channel, int resistance) {
	int value = getPotStep(resistance);

	Logging *logging = &logger;
	resetLogging(logging);
	appendPrintf(logging, "msg");
	appendPrintf(logging, DELIMETER);
	appendPrintf(logging, "Sending to potentiometer%d", channel);
	appendPrintf(&logger, ": ");
	appendPrintf(&logger, "%d for R=%d", value, resistance);
	appendMsgPostfix(logging);


	scheduleLogging(logging);

	sendToPot(driver, channel, value);
}


static void setPotResistance0(int value) {
	setPotResistance(&config0, 0, value);
}

static void setPotResistance1(int value) {
	setPotResistance(&config0, 1, value);
}

static void setPotValue1(int value) {
	sendToPot(&config0, 1, value);
}

#endif /* EFI_POTENTIOMETER */

void initPotentiometers() {
#if EFI_POTENTIOMETER
	initLogging(&logger, "potentiometer");

	initPotentiometer(&config0, _POT_SPI, POTEN_CS_PORT, POTEN_CS_PIN);

	addConsoleActionI("pot0", setPotResistance0);
	addConsoleActionI("pot1", setPotResistance1);

	addConsoleActionI("potd1", setPotValue1);

	setPotResistance0(3000);
	setPotResistance1(7000);
#else
	print("potentiometer disabled\r\n");
#endif
}
