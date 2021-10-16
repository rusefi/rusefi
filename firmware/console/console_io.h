/**
 * @file	console_io.h
 *
 * @date Dec 29, 2012
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
    
#ifdef __cplusplus
}
#endif /* __cplusplus */

typedef void (*CommandHandler)(char *);

#include "efifeatures.h"

void consoleOutputBuffer(const uint8_t *buf, int size);
void startConsole(CommandHandler console_line_callback_p);
void onDataArrived(bool valid);
