/**
 * @file	console_io.h
 *
 * @date Dec 29, 2012
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

typedef void (*CommandHandler)(char *);

void consoleOutputBuffer(const uint8_t *buf, int size);
void startConsole(CommandHandler console_line_callback_p);
void onDataArrived(bool valid);
