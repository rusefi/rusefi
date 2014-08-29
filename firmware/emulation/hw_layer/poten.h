/**
 * @file	poten.h
 * @brief	MCP42010 digital potentiometer driver
 *
 * @date Mar 16, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef POTEN_H_
#define POTEN_H_

#include "main.h"
#include "engine_configuration.h"

typedef struct {
	SPIDriver *spi;
	SPIConfig spiConfig;
} Mcp42010Driver;

void initPotentiometer(Mcp42010Driver *driver, SPIDriver *spi, ioportid_t port, ioportmask_t pin);
void initPotentiometers(board_configuration_s *boardConfiguration);
void setPotResistance(Mcp42010Driver *driver, int channel, int resistance);

#endif /* POTEN_H_ */
