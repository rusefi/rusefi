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
 * Because of hardware-related issues, some stm32f4/7 chips are totally
 * frozen while we are writing to internal flash. Writing the configuration takes
 * about 1-2 seconds, we cannot afford to do that while the engine is
 * running so we postpone the write until the engine is stopped.
 */
void writeToFlashNow();
bool writeToFlashNowImpl();
void setNeedToWriteConfiguration();
void suspendLinearTimeWatcher();

bool settingsLtftRequestWriteToFlash();

bool flashAllowWriteID(uint32_t id);

// Level state consumed by checkSettingsWriteFailure(): writeToFlashNowImpl() reports
// every settings-write attempt outcome here; a success clears the failing state.
void trackSettingsWriteResult(bool success);
// Core config-error producer (see refreshConfigErrorState()): reports when settings
// write attempts have been failing continuously for longer than the alert threshold.
bool checkSettingsWriteFailure();
