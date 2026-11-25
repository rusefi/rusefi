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
#if EFI_UNIT_TEST
void resetConfigurationWriteRequestCountForUnitTest();
int getConfigurationWriteRequestCountForUnitTest();
#endif
void suspendLinearTimeWatcher();

bool settingsLtftRequestWriteToFlash();

bool flashAllowWriteID(uint32_t id);

// Level state consumed by checkSettingsWriteFailure(): writeToFlashNowImpl() reports
// every settings-write attempt outcome here; a success clears the failing state.
void trackSettingsWriteResult(bool success);
// The pending settings write was given up, so no attempt will report an outcome: forget the
// failing streak. [tag:settings_write_keeps_failing]
void resetSettingsWriteFailure();
// Core config-error producer (see refreshConfigErrorState()): reports when retried settings
// writes keep failing: an attempt failed longer than the alert threshold after the first
// failure of the streak. About saving the tune only, not about what the tune contains.
bool checkSettingsWriteFailure();
