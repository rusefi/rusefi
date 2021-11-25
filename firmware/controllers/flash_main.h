/**
 * @file    flash_main.h
 * @brief
 *
 * @date Sep 19, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

void readFromFlash();
void initFlash();

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
