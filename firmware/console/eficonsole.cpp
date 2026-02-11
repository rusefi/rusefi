/**
 * @file    eficonsole.cpp
 * @brief   Console package entry point code
 *
 *
 * @date Nov 15, 2012
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "pch.h"


#include "eficonsole.h"
#include "console_io.h"
#include "mpu_util.h"
#include "board_overrides.h"

std::optional<setup_custom_board_overrides_type> custom_board_boardSayHello;

#if defined(STM32F4) || defined(STM32F7) || defined(STM32H7)
static void printUid() {
	uint32_t *uid = ((uint32_t *)UID_BASE);
	engine->outputChannels.deviceUid = crc8((const uint8_t*)uid, 12);
	efiPrintf("********************** UID=%lx:%lx:%lx crc=%d ******************************", uid[0], uid[1], uid[2], engine->outputChannels.deviceUid);
	engineConfiguration->device_uid[0] = uid[0];
	engineConfiguration->device_uid[1] = uid[1];
	engineConfiguration->device_uid[2] = uid[2];
}
#endif

/*
 * I was a little bit surprised that we declare __attribute__((weak)) before returning type in a function definition.
 * In the most sources this declaration is placed either before function name or after function parentheses, see:
 * - https://gcc.gnu.org/onlinedocs/gcc-4.0.0/gcc/Function-Attributes.html#Function-Attributes
 * - https://en.wikipedia.org/wiki/Weak_symbol
 * But it looks like our manner of __attribute__((weak)) declaration works at well, and I hope it will not cause
 * problems in the future.
 */
void boardSayHello() {
  // please use custom_board_boardSayHello
}

static void sayHello() {
	efiPrintf(PROTOCOL_HELLO_PREFIX " rusEFI LLC (c) 2012-2026. All rights reserved.");
	efiPrintf(PROTOCOL_HELLO_PREFIX " rusEFI v%d@%u now=%d", getRusEfiVersion(), /*do we have a working way to print 64 bit values?!*/(int)SIGNATURE_HASH, (int)getTimeNowMs());
	efiPrintf(PROTOCOL_HELLO_PREFIX " Chibios Kernel:       %s", CH_KERNEL_VERSION);
	efiPrintf(PROTOCOL_HELLO_PREFIX " Compiled:     " __DATE__ " - " __TIME__ "");
	efiPrintf(PROTOCOL_HELLO_PREFIX " COMPILER=%s", __VERSION__);
#if EFI_USE_OPENBLT
	efiPrintf(PROTOCOL_HELLO_PREFIX " with OPENBLT");
#endif

  call_board_override(custom_board_boardSayHello);

#if EFI_PROD_CODE && ENABLE_AUTO_DETECT_HSE
	extern float hseFrequencyMhz;
	extern uint8_t autoDetectedRoundedMhz;
	efiPrintf(PROTOCOL_HELLO_PREFIX " detected HSE clock %.2f MHz PLLM = %d", hseFrequencyMhz, autoDetectedRoundedMhz);
#endif /* ENABLE_AUTO_DETECT_HSE */

	efiPrintf("hellenBoardId=%d", engine->engineState.hellenBoardId);

#if defined(STM32F4) || defined(STM32F7) || defined(STM32H7)
  printUid();

#if defined(STM32F4) && !defined(AT32F4XX)
//	efiPrintf("ramReadProbe 0x20000010 %d", ramReadProbe((const char *)0x20000010));
//	efiPrintf("ramReadProbe 0x20020010 %d", ramReadProbe((const char *)0x20020010));
//	efiPrintf("ramReadProbe 0x20070010 %d", ramReadProbe((const char *)0x20070010));

	efiPrintf("isStm32F42x %s", boolToString(isStm32F42x()));
#endif // STM32F4

#ifndef MIN_FLASH_SIZE
#define MIN_FLASH_SIZE 1024
#endif // MIN_FLASH_SIZE

	int flashSize = TM_ID_GetFlashSize();
	if (flashSize < MIN_FLASH_SIZE) {
		// todo: bug, at the moment we report 1MB on dual-bank F7
		criticalError("rusEFI expected at least %dK of flash", MIN_FLASH_SIZE);
	}

#ifdef AT32F4XX
	int mcuRevision = DBGMCU->SERID & 0x07;
	int mcuSerId = (DBGMCU->SERID >> 8) & 0xff;
	const char *partNumber, *package;
	uint32_t pnFlashSize;
	int ret = at32GetMcuType(DBGMCU->IDCODE, &partNumber, &package, &pnFlashSize);
	if (ret == 0) {
		efiPrintf("MCU IDCODE %s in %s with %ld KB flash",
			partNumber, package, pnFlashSize);
	} else {
		efiPrintf("MCU IDCODE unknown 0x%lx", DBGMCU->IDCODE);
	}
	efiPrintf("MCU SER_ID %s rev %c",
		(mcuSerId == 0x0d) ? "AT32F435" : ((mcuSerId == 0x0e) ? "AT32F437" : "UNKNOWN"),
		'A' + mcuRevision);
	efiPrintf("MCU F_SIZE %d KB", flashSize);
	efiPrintf("MCU RAM %d KB", at32GetRamSizeKb());
#else
#define MCU_REVISION_MASK  0xfff
	int mcuRevision = DBGMCU->IDCODE & MCU_REVISION_MASK;
	efiPrintf("MCU rev=%x flashSize=%d", mcuRevision, flashSize);
#endif
#endif

#ifdef CH_CFG_ST_FREQUENCY
	efiPrintf("CH_CFG_ST_FREQUENCY=%d", CH_CFG_ST_FREQUENCY);
#endif

#ifdef ENABLE_PERF_TRACE
	efiPrintf("ENABLE_PERF_TRACE=%d", ENABLE_PERF_TRACE);
#endif

#ifdef STM32_ADCCLK
	efiPrintf("STM32_ADCCLK=%d", STM32_ADCCLK);
	efiPrintf("STM32_TIMCLK1=%d", STM32_TIMCLK1);
	efiPrintf("STM32_TIMCLK2=%d", STM32_TIMCLK2);
#endif
#ifdef STM32_PCLK1
	efiPrintf("STM32_PCLK1=%d", STM32_PCLK1);
	efiPrintf("STM32_PCLK2=%d", STM32_PCLK2);
#endif
#ifdef STM32_RTCSEL
	if (1) {
		#define STM32_RTCSEL_SHIFT 8
		const char * rtcsel_names[4] = {"no clock", "LSE", "LSI", "HSE"};

		int rtcsel = (RCC->BDCR & STM32_RTCSEL_MASK) >> STM32_RTCSEL_SHIFT;
		efiPrintf("STM32_RTCSEL=%d %s actual=%d %s",
			STM32_RTCSEL >> STM32_RTCSEL_SHIFT, rtcsel_names[STM32_RTCSEL >> STM32_RTCSEL_SHIFT], rtcsel, rtcsel_names[rtcsel]);
#ifdef RUSEFI_STM32_LSE_WAIT_MAX_RTCSEL
		efiPrintf("RUSEFI_STM32_LSE_WAIT_MAX_RTCSEL=%d %s",
			RUSEFI_STM32_LSE_WAIT_MAX_RTCSEL >> STM32_RTCSEL_SHIFT, rtcsel_names[RUSEFI_STM32_LSE_WAIT_MAX_RTCSEL >> STM32_RTCSEL_SHIFT]);
#endif
	}
#endif

	efiPrintf("PORT_IDLE_THREAD_STACK_SIZE=%d", PORT_IDLE_THREAD_STACK_SIZE);

	efiPrintf("CH_DBG_ENABLE_ASSERTS=%d", CH_DBG_ENABLE_ASSERTS);
#ifdef CH_DBG_ENABLED
	efiPrintf("CH_DBG_ENABLED=%d", CH_DBG_ENABLED);
#endif
	efiPrintf("CH_DBG_SYSTEM_STATE_CHECK=%d", CH_DBG_SYSTEM_STATE_CHECK);
	efiPrintf("CH_DBG_ENABLE_STACK_CHECK=%d", CH_DBG_ENABLE_STACK_CHECK);

	/**
	 * Time to finish output. This is needed to avoid mix-up of this methods output and console command confirmation
	 * this code here dates back to 2015. today in 2024 I have no idea what it does :(
	 */
	chThdSleepMilliseconds(5);
}

#if CH_DBG_THREADS_PROFILING && CH_DBG_FILL_THREADS
int CountFreeStackSpace(const void* wabase) {
	const uint8_t* stackBase = reinterpret_cast<const uint8_t*>(wabase);
	const uint8_t* stackUsage = stackBase;

	// thread stacks are filled with CH_DBG_STACK_FILL_VALUE
	// find out where that ends - that's the last thing we needed on the stack
	while (*stackUsage == CH_DBG_STACK_FILL_VALUE) {
		stackUsage++;
	}

	return (int)(stackUsage - stackBase);
}
#endif

/**
 * This methods prints all threads, their stack usage, and their total times
 */
static void cmd_threads() {
#if CH_DBG_THREADS_PROFILING && CH_DBG_FILL_THREADS
	thread_t* tp = chRegFirstThread();

	efiPrintf("name\twabase\ttime\tfree stack");

	while (tp) {
		int freeBytes = CountFreeStackSpace(tp->wabase);
		efiPrintf("%s\t%08x\t%lu\t%d", tp->name, (unsigned int)tp->wabase, tp->time, freeBytes);

		if (freeBytes < 100) {
			criticalError("Ran out of stack on thread %s, %d bytes remain", tp->name, freeBytes);
		}

		tp = chRegNextThread(tp);
	}

#if EFI_PROD_CODE
	// isr stack base
	extern uint32_t __main_stack_base__;

	int isrSpace = CountFreeStackSpace(reinterpret_cast<void*>(&__main_stack_base__));
	efiPrintf("isr\t0\t0\t%d", isrSpace);
#endif // EFI_PROD_CODE

#else // CH_DBG_THREADS_PROFILING && CH_DBG_FILL_THREADS

  efiPrintf("CH_DBG_THREADS_PROFILING && CH_DBG_FILL_THREADS is not enabled");

#endif
}

/**
 * @brief This is just a test function
 */
static void echo(int value) {
	efiPrintf("got value: %d", value);
}

void checkStackAndHandleConsoleLine(char *line) {
	assertStackVoid("console", ObdCode::STACK_USAGE_MISC, EXPECTED_REMAINING_STACK);
    handleConsoleLine(line);
}

void onCliCaseError(const char *token) {
	firmwareError(ObdCode::CUSTOM_ERR_COMMAND_LOWER_CASE_EXPECTED, "lowerCase expected [%s]", token);
}

void onCliDuplicateError(const char *token) {
    firmwareError(ObdCode::CUSTOM_SAME_TWICE, "Same action twice [%s]", token);
}

void onCliOverflowError() {
    firmwareError(ObdCode::CUSTOM_CONSOLE_TOO_MANY, "Too many console actions");
}

void initializeConsole() {
	initConsoleLogic();

	startConsole(&handleConsoleLine);

#if defined(STM32F4) || defined(STM32F7) || defined(STM32H7)
	addConsoleAction("uid", printUid);
#endif

	sayHello();
	addConsoleAction("test", [](){ /* do nothing */});
	addConsoleActionI("echo", echo);
	addConsoleAction("hello", sayHello);
	#if EFI_USE_OPENBLT
	  addConsoleAction("show_blt_version", [](){
      	uint32_t bltBinVersion = getOpenBltVersion();
      	efiPrintf("********************** blt=%lx %s version", bltBinVersion, bltBinVersion == BLT_CURRENT_VERSION ? "CURRENT" : "UNEXPECTED");
	  });
	#endif
#if EFI_HAS_RESET
	addConsoleAction("reset", scheduleReset);
#endif

	addConsoleAction("threadsinfo", cmd_threads);

#if HAL_USE_WDG
	addConsoleActionI("set_watchdog_timeout", startWatchdog);
	addConsoleActionI("set_watchdog_reset", setWatchdogResetPeriod);
#endif
}
