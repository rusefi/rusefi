/**
 * @file	console_io.h
 *
 * @date Dec 29, 2012
 * @author Andrey Belomutskiy, (c) 2012-2017
 */
#ifndef CONSOLE_IO_H_
#define CONSOLE_IO_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
    
#ifdef __cplusplus
}
#endif /* __cplusplus */

typedef void (*CommandHandler)(char *);

#include "efifeatures.h"
#include "datalogging.h"

#ifdef CONFIG_RESET_SWITCH_PORT
// this pin is not configurable at runtime so that we have a reliable way to reset configuration
#define SHOULD_INGORE_FLASH() (palReadPad(CONFIG_RESET_SWITCH_PORT, CONFIG_RESET_SWITCH_PIN) == 0)
#else
#define SHOULD_INGORE_FLASH() (false)
#endif

BaseChannel * getConsoleChannel(void);

void consolePutChar(int x);
void consoleOutputBuffer(const uint8_t *buf, int size);
void startConsole(Logging *sharedLogger, CommandHandler console_line_callback_p);
void onDataArrived(void);
bool isUsbSerial(BaseChannel * channel);

#if EFI_PROD_CODE || EFI_SIMULATOR || EFI_EGT
bool isCommandLineConsoleReady(void);
#else
#define isCommandLineConsoleReady() true
#endif

#endif /* CONSOLE_IO_H_ */
