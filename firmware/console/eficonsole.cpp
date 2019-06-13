/**
 * @file    eficonsole.cpp
 * @brief   Console package entry point code
 *
 *
 * @date Nov 15, 2012
 * @author Andrey Belomutskiy, (c) 2012-2018
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

#include <stdarg.h>
#include "global.h"
#include <chprintf.h>
#include "eficonsole.h"
#include "console_io.h"
#include "svnversion.h"

static LoggingWithStorage logger("console");

static void myfatal(void) {
	chDbgCheck(0);
}

static void myerror(void) {
	firmwareError(CUSTOM_ERR_TEST_ERROR, "firmwareError: %d", getRusEfiVersion());
}

static void sayNothing(void) {
	/**
	 * @see EngineState#TS_PROTOCOL_TAG
	 * this empty response is part of protocol check
	 * todo: make this logic smarter?
	 */
}

static void sayHello(void) {
	scheduleMsg(&logger, "*** rusEFI (c) Andrey Belomutskiy 2012-2019. All rights reserved.");
	scheduleMsg(&logger, "rusEFI v%d@%s", getRusEfiVersion(), VCS_VERSION);
	scheduleMsg(&logger, "*** Chibios Kernel:       %s", CH_KERNEL_VERSION);
	scheduleMsg(&logger, "*** Compiled:     " __DATE__ " - " __TIME__ "");
	scheduleMsg(&logger, "COMPILER=%s", __VERSION__);
#ifdef CH_FREQUENCY
	scheduleMsg(&logger, "CH_FREQUENCY=%d", CH_FREQUENCY);
#endif

#ifdef CORTEX_MAX_KERNEL_PRIORITY
	scheduleMsg(&logger, "CORTEX_MAX_KERNEL_PRIORITY=%d", CORTEX_MAX_KERNEL_PRIORITY);
#endif

#ifdef STM32_ADCCLK
	scheduleMsg(&logger, "STM32_ADCCLK=%d", STM32_ADCCLK);
	scheduleMsg(&logger, "STM32_TIMCLK1=%d", STM32_TIMCLK1);
	scheduleMsg(&logger, "STM32_TIMCLK2=%d", STM32_TIMCLK2);
#endif
#ifdef STM32_PCLK1
	scheduleMsg(&logger, "STM32_PCLK1=%d", STM32_PCLK1);
	scheduleMsg(&logger, "STM32_PCLK2=%d", STM32_PCLK2);
#endif

	scheduleMsg(&logger, "PORT_IDLE_THREAD_STACK_SIZE=%d", PORT_IDLE_THREAD_STACK_SIZE);

	scheduleMsg(&logger, "CH_DBG_ENABLE_ASSERTS=%d", CH_DBG_ENABLE_ASSERTS);
#ifdef CH_DBG_ENABLED
	scheduleMsg(&logger, "CH_DBG_ENABLED=%d", CH_DBG_ENABLED);
#endif
	scheduleMsg(&logger, "CH_DBG_SYSTEM_STATE_CHECK=%d", CH_DBG_SYSTEM_STATE_CHECK);
	scheduleMsg(&logger, "CH_DBG_ENABLE_STACK_CHECK=%d", CH_DBG_ENABLE_STACK_CHECK);

#ifdef EFI_WAVE_ANALYZER
	scheduleMsg(&logger, "EFI_WAVE_ANALYZER=%d", EFI_WAVE_ANALYZER);
#endif
#ifdef EFI_TUNER_STUDIO
	scheduleMsg(&logger, "EFI_TUNER_STUDIO=%d", EFI_TUNER_STUDIO);
#else
	scheduleMsg(&logger, "EFI_TUNER_STUDIO=%d", 0);
#endif

#ifdef EFI_SIGNAL_EXECUTOR_SLEEP
	scheduleMsg(&logger, "EFI_SIGNAL_EXECUTOR_SLEEP=%d", EFI_SIGNAL_EXECUTOR_SLEEP);
#endif

#ifdef EFI_SIGNAL_EXECUTOR_HW_TIMER
	scheduleMsg(&logger, "EFI_SIGNAL_EXECUTOR_HW_TIMER=%d", EFI_SIGNAL_EXECUTOR_HW_TIMER);
#endif

#if defined(EFI_SHAFT_POSITION_INPUT)
	scheduleMsg(&logger, "EFI_SHAFT_POSITION_INPUT=%d", EFI_SHAFT_POSITION_INPUT);
#endif
#ifdef EFI_INTERNAL_ADC
	scheduleMsg(&logger, "EFI_INTERNAL_ADC=%d", EFI_INTERNAL_ADC);
#endif

//	printSimpleMsg(&logger, "", );
//	printSimpleMsg(&logger, "", );

	/**
	 * Time to finish output. This is needed to avoid mix-up of this methods output and console command confirmation
	 */
	chThdSleepMilliseconds(5);
}

static uintptr_t CountFreeStackSpace(const void* wabase)
{
	const uint8_t* stackBase = reinterpret_cast<const uint8_t*>(wabase);
	const uint8_t* stackUsage = stackBase;

	// thread stacks are filled with 0x55
	// find out where that ends - that's the last thing we needed on the stack
	while(*stackUsage == 0x55) {
		stackUsage++;
	}

	return stackUsage - stackBase;
}

/**
 * This methods prints all threads, their stack usage, and their total times
 */
static void cmd_threads(void) {
#if CH_DBG_THREADS_PROFILING && CH_DBG_FILL_THREADS

	thread_t* tp = chRegFirstThread();

	scheduleMsg(&logger, "name\twabase\ttime\tfree stack");

	while(tp) {
		uintptr_t freeBytes = CountFreeStackSpace(tp->wabase);
		scheduleMsg(&logger, "%s\t%08x\t%lu\t%lu", tp->name, tp->wabase, tp->time, freeBytes);

		tp = chRegNextThread(tp);
	}

	uintptr_t isrSpace = CountFreeStackSpace(reinterpret_cast<void*>(0x20000000));
	scheduleMsg(&logger, "isr\t0\t0\t%lu", isrSpace);

#else // CH_DBG_THREADS_PROFILING && CH_DBG_FILL_THREADS

  scheduleMsg(&logger, "CH_DBG_THREADS_PROFILING && CH_DBG_FILL_THREADS is not enabled");

#endif
}

/**
 * This methods prints the message to whatever is configured as our primary console
 */
void print(const char *format, ...) {
#if !EFI_UART_ECHO_TEST_MODE
	if (!isCommandLineConsoleReady()) {
		return;
	}
	va_list ap;
	va_start(ap, format);
	chvprintf((BaseSequentialStream*) getConsoleChannel(), format, ap);
	va_end(ap);
#endif /* EFI_UART_ECHO_TEST_MODE */
}

void initializeConsole(Logging *sharedLogger) {
	initConsoleLogic(sharedLogger);

	startConsole(sharedLogger, &handleConsoleLine);

	sayHello();
	addConsoleAction("test", sayNothing);
	addConsoleAction("hello", sayHello);
#if EFI_HAS_RESET
	addConsoleAction("reset", scheduleReset);
#endif

	addConsoleAction("fatal", myfatal);
	addConsoleAction("error", myerror);
	addConsoleAction("threadsinfo", cmd_threads);
}
