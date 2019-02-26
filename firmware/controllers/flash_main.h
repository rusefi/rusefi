/**
 * @file    flash_main.h
 * @brief
 *
 * @date Sep 19, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef FLASH_MAIN_H_
#define FLASH_MAIN_H_

#include "engine.h"

#define FLASH_DATA_VERSION 10001

typedef enum {
	PC_OK = 0,
	CRC_FAILED = 1,
	INCOMPATIBLE_VERSION = 2,
	RESET_REQUESTED = 3,
	PC_ERROR = 4
} persisted_configuration_state_e;

persisted_configuration_state_e readConfiguration(Logging * logger);
void readFromFlash(void);
void initFlash(Logging *sharedLogger);

/**
 * Because of hardware-related issues, stm32f4 chip is totally
 * frozen while we are writing to internal flash. Writing the configuration takes
 * about 1-2 seconds, we cannot afford to do that while the engine is
 * running so we postpone the write until the engine is stopped.
 */
void writeToFlashNow(void);
void setNeedToWriteConfiguration(void);
/**
 * @return true if an flash write is pending
 */
bool getNeedToWriteConfiguration(void);
void writeToFlashIfPending(void);

#endif /* FLASH_MAIN_H_ */
