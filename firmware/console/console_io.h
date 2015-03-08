/**
 * @file	console_io.h
 *
 * @date Dec 29, 2012
 * @author Andrey Belomutskiy, (c) 2012-2015
 */
#pragma once
#ifndef CONSOLE_IO_H_
#define CONSOLE_IO_H_

#include <ch.h>
#include <hal.h>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#include "efifeatures.h"
#include "datalogging.h"

#define GET_CONSOLE_MODE_VALUE() palReadPad(CONSOLE_MODE_SWITCH_PORT, CONSOLE_MODE_SWITCH_PIN)
#define SHOULD_INGORE_FLASH() (palReadPad(CONFIG_RESET_SWITCH_PORT, CONFIG_RESET_SWITCH_PIN) == 0)

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

SerialDriver * getConsoleChannel(void);

void consolePutChar(int x);
void consoleOutputBuffer(const uint8_t *buf, int size);
void startConsole(Logging *sharedLogger, void (*console_line_callback_p)(char *));
bool isSerialOverUart(void);

#if EFI_PROD_CODE || EFI_SIMULATOR || EFI_EGT
bool isConsoleReady(void);
#else
#define isConsoleReady() true
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CONSOLE_IO_H_ */
