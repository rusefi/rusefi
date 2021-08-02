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

#include "globalaccess.h"
#include "os_access.h"
#include "eficonsole.h"
#include "console_io.h"
#include "svnversion.h"

static void testCritical(void) {
	chDbgCheck(0);
}

static void myerror(void) {
	firmwareError(CUSTOM_ERR_TEST_ERROR, "firmwareError: %d", getRusEfiVersion());
}

static void sayHello(void) {
	efiPrintf(PROTOCOL_HELLO_PREFIX " rusEFI LLC (c) 2012-2021. All rights reserved.");
	efiPrintf(PROTOCOL_HELLO_PREFIX " rusEFI v%d@%s", getRusEfiVersion(), VCS_VERSION);
	efiPrintf(PROTOCOL_HELLO_PREFIX " Chibios Kernel:       %s", CH_KERNEL_VERSION);
	efiPrintf(PROTOCOL_HELLO_PREFIX " Compiled:     " __DATE__ " - " __TIME__ "");
	efiPrintf(PROTOCOL_HELLO_PREFIX " COMPILER=%s", __VERSION__);

#ifdef ENABLE_AUTO_DETECT_HSE
	extern float hseFrequencyMhz;
	extern uint8_t autoDetectedRoundedMhz;
	efiPrintf(PROTOCOL_HELLO_PREFIX " detected HSE clock %.2f MHz PLLM = %d", hseFrequencyMhz, autoDetectedRoundedMhz);
#endif /* ENABLE_AUTO_DETECT_HSE */

#if defined(STM32F4) || defined(STM32F7)
	uint32_t *uid = ((uint32_t *)UID_BASE);
	efiPrintf("UID=%x %x %x", uid[0], uid[1], uid[2]);

#define 	TM_ID_GetFlashSize()    (*(__IO uint16_t *) (FLASHSIZE_BASE))
#define MCU_REVISION_MASK  0xfff

	int mcuRevision = DBGMCU->IDCODE & MCU_REVISION_MASK;

#define MIN_FLASH_SIZE 1024

	int flashSize = TM_ID_GetFlashSize();
	if (flashSize < MIN_FLASH_SIZE) {
		firmwareError(OBD_PCM_Processor_Fault, "rusEFI expected at least %dK of flash", MIN_FLASH_SIZE);
	}

	efiPrintf("rev=%x size=%d", mcuRevision, flashSize);
#endif


#ifdef CH_FREQUENCY
	efiPrintf("CH_FREQUENCY=%d", CH_FREQUENCY);
#endif

#ifdef CORTEX_MAX_KERNEL_PRIORITY
	efiPrintf("CORTEX_MAX_KERNEL_PRIORITY=%d", CORTEX_MAX_KERNEL_PRIORITY);
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

	efiPrintf("PORT_IDLE_THREAD_STACK_SIZE=%d", PORT_IDLE_THREAD_STACK_SIZE);

	efiPrintf("CH_DBG_ENABLE_ASSERTS=%d", CH_DBG_ENABLE_ASSERTS);
#ifdef CH_DBG_ENABLED
	efiPrintf("CH_DBG_ENABLED=%d", CH_DBG_ENABLED);
#endif
	efiPrintf("CH_DBG_SYSTEM_STATE_CHECK=%d", CH_DBG_SYSTEM_STATE_CHECK);
	efiPrintf("CH_DBG_ENABLE_STACK_CHECK=%d", CH_DBG_ENABLE_STACK_CHECK);

#ifdef EFI_LOGIC_ANALYZER
	efiPrintf("EFI_LOGIC_ANALYZER=%d", EFI_LOGIC_ANALYZER);
#endif
#ifdef EFI_TUNER_STUDIO
	efiPrintf("EFI_TUNER_STUDIO=%d", EFI_TUNER_STUDIO);
#else
	efiPrintf("EFI_TUNER_STUDIO=%d", 0);
#endif

#ifdef EFI_SIGNAL_EXECUTOR_SLEEP
	efiPrintf("EFI_SIGNAL_EXECUTOR_SLEEP=%d", EFI_SIGNAL_EXECUTOR_SLEEP);
#endif

#ifdef EFI_SIGNAL_EXECUTOR_HW_TIMER
	efiPrintf("EFI_SIGNAL_EXECUTOR_HW_TIMER=%d", EFI_SIGNAL_EXECUTOR_HW_TIMER);
#endif

#if defined(EFI_SHAFT_POSITION_INPUT)
	efiPrintf("EFI_SHAFT_POSITION_INPUT=%d", EFI_SHAFT_POSITION_INPUT);
#endif
#ifdef EFI_INTERNAL_ADC
	efiPrintf("EFI_INTERNAL_ADC=%d", EFI_INTERNAL_ADC);
#endif

	/**
	 * Time to finish output. This is needed to avoid mix-up of this methods output and console command confirmation
	 */
	chThdSleepMilliseconds(5);
}

void validateStack(const char*msg, obd_code_e code, int desiredStackUnusedSize) {
#if CH_DBG_THREADS_PROFILING && CH_DBG_FILL_THREADS
	int unusedStack = CountFreeStackSpace(chThdGetSelfX()->wabase);
	if (unusedStack < desiredStackUnusedSize) {
		warning(code, "Stack low on %s: %d", msg, unusedStack);
	}
#else
	(void)msg; (void)code; (void)desiredStackUnusedSize;
#endif
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
static void cmd_threads(void) {
#if CH_DBG_THREADS_PROFILING && CH_DBG_FILL_THREADS

	thread_t* tp = chRegFirstThread();

	efiPrintf("name\twabase\ttime\tfree stack");

	while (tp) {
		int freeBytes = CountFreeStackSpace(tp->wabase);
		efiPrintf("%s\t%08x\t%lu\t%d", tp->name, tp->wabase, tp->time, freeBytes);

		tp = chRegNextThread(tp);
	}

	int isrSpace = CountFreeStackSpace(reinterpret_cast<void*>(0x20000000));
	efiPrintf("isr\t0\t0\t%d", isrSpace);

#else // CH_DBG_THREADS_PROFILING && CH_DBG_FILL_THREADS

  efiPrintf("CH_DBG_THREADS_PROFILING && CH_DBG_FILL_THREADS is not enabled");

#endif
}

void initializeConsole() {
	initConsoleLogic();

	startConsole(&handleConsoleLine);

	sayHello();
	addConsoleAction("test", [](){ /* do nothing */});
	addConsoleAction("hello", sayHello);
#if EFI_HAS_RESET
	addConsoleAction("reset", scheduleReset);
#endif

	addConsoleAction("critical", testCritical);
	addConsoleAction("error", myerror);
	addConsoleAction("threadsinfo", cmd_threads);
}
