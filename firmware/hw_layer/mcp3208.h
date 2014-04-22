/*
 * @file    mcp3208.h
 *
 * @date Aug 12, 2013
 * @author Andrey Belomutskiy, (c) 2012-2013
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


// Peripherial Clock 84MHz SPI1                                SPI1        SPI2/3
// Peripherial Clock 42MHz SPI2 SPI3
#define SPI_BaudRatePrescaler_2         ((uint16_t)0x0000) //  42 MHz      21 MHZ
#define SPI_BaudRatePrescaler_4         ((uint16_t)0x0008) //  21 MHz      10.5 MHz
#define SPI_BaudRatePrescaler_8         ((uint16_t)0x0010) //  10.5 MHz    5.25 MHz
#define SPI_BaudRatePrescaler_16        ((uint16_t)0x0018) //  5.25 MHz    2.626 MHz
#define SPI_BaudRatePrescaler_32        ((uint16_t)0x0020) //  2.626 MHz   1.3125 MHz
#define SPI_BaudRatePrescaler_64        ((uint16_t)0x0028) //  1.3125 MHz  656.25 KHz
#define SPI_BaudRatePrescaler_128       ((uint16_t)0x0030) //  656.25 KHz  328.125 KHz
#define SPI_BaudRatePrescaler_256       ((uint16_t)0x0038) //  328.125 KHz 164.06 KHz

#endif /* MCP3208_H_ */
