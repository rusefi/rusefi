/**
 * @file	serial_sensor.cpp
 *
 *
 * @date Mar 19, 2020
 * @author Konstantin Smola, (c) 2020
 */

#include "global.h"
#if EFI_AUX_SERIAL
#include "serial.h"
#include "serial_sensor.h"
#include "engine.h"

EXTERN_ENGINE;

void SerialSendTest()
{
	char data[] = "Hello world!\n\r";
	sdWrite(&SD6, (uint8_t *)data, strlen(data));
}

#endif

