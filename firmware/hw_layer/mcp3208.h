/*
 * @file    mcp3208.h
 *
 * @date Aug 12, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 *
 *
 * MCP3208 pins:
 * 	pin10	CS		Chip Select
 * 	pin11	Din		Data In (MOSI)
 * 	pin12	Dout	Data Out (MISO)
 * 	pin13	CLK		spi CLoCk
 *
 */

#ifndef MCP3208_H_
#define MCP3208_H_

#include "global.h"

#if EFI_MCP_3208

#define MCP3208_CS_PORT GPIOD
#define MCP3208_CS_PIN 11

/**
 * @brief   MCP3208 ADC chip driver structure
 */
typedef struct {
	SPIDriver *driver;
	unsigned char tx_buff[3];
	unsigned char rx_buff[3];
	int requestedChannel;
	int results[8];
} McpAdcState;

void init_adc_mcp3208(McpAdcState *state, SPIDriver *driver);
void requestAdcValue(McpAdcState *state, int channel);
void requestAdcValueI(McpAdcState *state, int channel);
void adc_in_out(McpAdcState *state);

int getMcp3208adc(int channel);

#endif /* EFI_MCP_3208 */

#endif /* MCP3208_H_ */
