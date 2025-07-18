/**
 * @file	mpu_watchdog.h
 * @brief	Hardware Independent Watchdog (IWDG) high-level API
 *
 * @date Nov 28, 2023
 * @author	Andrey Belomutskiy, (c) 2012-2023
 * @author	andreika <prometheus.pcb@gmail.com>
 */

#pragma once

#include "main_loop.h"
#include "engine_controller.h"

// 100 ms is our empiric choice based on 2 * SLOW_CALLBACK_PERIOD_MS
#define WATCHDOG_RESET_MS (2 * SLOW_CALLBACK_PERIOD_MS)
// 500 ms is our default timeout
#define WATCHDOG_TIMEOUT_MS (5 * WATCHDOG_RESET_MS)
// 5 secs should be enough to wait until 
#define WATCHDOG_FLASH_TIMEOUT_MS 5000
// MFS startup time in case of garbage collection can take a loooot of time
#define WATCHDOG_MFS_START_TIMEOUT_MS 7000

// we use 'int' for compatibility with addConsoleActionI()
// can be called multiple times to change the timeout
void startWatchdog(int timeoutMs = WATCHDOG_TIMEOUT_MS);

// Can be called for debug reasons to test the watchdog
void setWatchdogResetPeriod(int resetMs);

// A reset is done only if enough time has passed since the last reset.
void tryResetWatchdog();
