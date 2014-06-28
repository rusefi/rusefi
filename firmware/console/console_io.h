/**
 * @file	console_io.h
 *
 * @date Dec 29, 2012
 * @author Andrey Belomutskiy, (c) 2012-2014
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
#include "boards.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

SerialDriver * getConsoleChannel(void);

void consolePutChar(int x);
void consoleOutputBuffer(const int8_t *buf, int size);
void startConsole(void (*console_line_callback_p)(char *));
int isConsoleReady(void);
bool isSerialOverUart(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CONSOLE_IO_H_ */
