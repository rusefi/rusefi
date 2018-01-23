/*
 * @file    mcp3208.c
 * @brief   MCP3208 external ADC chip implementation. Not really used right now.
 *
 * @date Aug 12, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"

#if EFI_MCP_3208 || defined(__DOXYGEN__)
#include "mcp3208.h"
#include "pin_repository.h"

McpAdcState *hack;

static int adcEventCounter = 0;
static int adcErrorCounter = 0;

static int getValue(McpAdcState *state, int channel) {
	return state->results[channel] & 4095;
}

int getMcp3208adc(int channel) {
	return getValue(hack, channel);
}

static int getNextChannel(void) {
	return adcEventCounter % 2;
}

static void spiCallback(SPIDriver *spip) {
	spiUnselectI(spip);
	adcEventCounter++;

	McpAdcState *state = hack;

	int withError = 0;

	if (state->rx_buff[0] != 255) {
		withError = 1;
		//fatal("ADC: first byte");
	}

	if ((state->rx_buff[1] & 0xE0) != 0xE0) {
		withError = 1;
		//fatal("ADC: second byte");
	}

	if (!withError) {
//		unsigned char upperByte = state->rx_buff[1] & 0b00001111;
		int result = (state->rx_buff[0] << 16) + (state->rx_buff[1] << 8) + state->rx_buff[2];
		state->results[state->requestedChannel] = result;
	} else {
		adcErrorCounter++;
	}

	requestAdcValueI(state, getNextChannel());
}

static const SPIConfig spicfg = { spiCallback,
/* HW dependent part.*/
MCP3208_CS_PORT,
MCP3208_CS_PIN,
//SPI_CR1_MSTR |
		SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2
//		SPI_CR1_BR_1 | SPI_CR1_BR_2
		};

static void createRequest(McpAdcState *state, int channel) {
	efiAssertVoid(channel < 8, "Invalid ADC channel");

	state->requestedChannel = channel;

	state->tx_buff[0] = 0x06 + (channel >> 2);
	state->tx_buff[1] = (channel & 3) << 6;
}

void requestAdcValue(McpAdcState *state, int channel) {
	createRequest(state, channel);

	spiSelect(state->driver);
	spiStartExchange(state->driver, 3, state->tx_buff, state->rx_buff);
	// SPI unselect is in the callback
}

void requestAdcValueI(McpAdcState *state, int channel) {
	createRequest(state, channel);

	spiSelectI(state->driver);
	spiStartExchangeI(state->driver, 3, state->tx_buff, state->rx_buff);
	// SPI unselect is in the callback
}

void adc_in_out(McpAdcState *state) {

//	chThdSleepMilliseconds(10);

	int result = state->results[0];

//	int v = result;
	unsigned r = 0;
//
//	while (v >>= 1)
//		r++;
//
//	unsigned int f;
//	if (r < 12) {
//		f = -1;
//	} else {
//		f = result >> (r - 12);
//		f = f & 4095;
//	}

	int errRatio = 1000 * adcErrorCounter / adcEventCounter;

	print("c/e %7d/%7d/%4d  result %5d r=%d ", adcEventCounter, adcErrorCounter, errRatio, result, r);

	unsigned int f0 = getValue(state, 0);
	print("ch0=%d adj %d    ", f0, f0 * 5000 / 4096);
	unsigned int f1 = getValue(state, 1);
	print("ch1=%d adj %d\r\n", f1, f1 * 5000 / 4096);
}

void init_adc_mcp3208(McpAdcState *state, SPIDriver *driver) {

//	initSpiModules();

	state->driver = driver;
	state->tx_buff[2] = 0;

	hack = state;

	todo: convert to new API, todo: array of CS
	mySetPadMod("ext adc chip select", MCP3208_CS_PORT, MCP3208_CS_PIN, PAL_STM32_MODE_OUTPUT);


	spiStart(driver, &spicfg);
}

#endif /* EFI_MCP_3208 */
