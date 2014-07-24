/*
 * hardware.h
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "main.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void turnOnSpi(spi_device_e device);
void lockSpi(spi_device_e device);
void unlockSpi(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#define GET_BOARD_TEST_MODE_VALUE() (!palReadPad(getHwPort(boardConfiguration->boardTestModeJumperPin), getHwPin(boardConfiguration->boardTestModeJumperPin)))

void initHardware(Logging *logging);

#endif /* HARDWARE_H_ */
