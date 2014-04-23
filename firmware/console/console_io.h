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

#if EFI_SERIAL_OVER_USB
 #include "usbcfg.h"
 extern SerialUSBDriver SDU1;
 #define CONSOLE_CHANNEL (&SDU1)
#else
 #define CONSOLE_CHANNEL EFI_CONSOLE_UART_DEVICE
#endif

void consolePutChar(int x);
void consoleOutputBuffer(const int8_t *buf, int size);
void startChibiosConsole(void (*console_line_callback_p)(char *));
int is_serial_ready(void);

#endif /* CONSOLE_IO_H_ */
