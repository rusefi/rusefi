/**
 * @file    eficonsole.cpp
 * @brief   Console package entry point code
 *
 *
 * @date Nov 15, 2012
 * @author Andrey Belomutskiy, (c) 2012-2015
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
#include <stdbool.h>
#include "main.h"
#include <chprintf.h>
#include "eficonsole.h"
#include "console_io.h"
#include "svnversion.h"

static LoggingWithStorage logger("console");

static char fatalErrorMessage[200];

void fatal3(char *msg, char *file, int line) {
	strcpy(fatalErrorMessage, msg);
#if EFI_CUSTOM_PANIC_METHOD
	chDbgPanic3(fatalErrorMessage, file, line);
#else
	chDbgPanic(fatalErrorMessage);
#endif
}

static void myfatal(void) {
	chDbgCheck(0, "my fatal");
}

static void myerror(void) {
	firmwareError("firmwareError: %d", getRusEfiVersion());
}

static void sayNothing(void) {
	/**
	 * @see EngineState#TS_PROTOCOL_TAG
	 * this empty response is part of protocol check
	 * todo: make this logic smarter?
	 */
}

static void sayHello(void) {
	printMsg(&logger, "*** rusEFI (c) Andrey Belomutskiy, 2012-2015. All rights reserved.");
	printMsg(&logger, "rusEFI v%d@%s", getRusEfiVersion(), VCS_VERSION);
	printMsg(&logger, "*** Chibios Kernel:       %s", CH_KERNEL_VERSION);
	printMsg(&logger, "*** Compiled:     " __DATE__ " - " __TIME__ "");
	printMsg(&logger, "COMPILER=%s", __VERSION__);
	printMsg(&logger, "CH_FREQUENCY=%d", CH_FREQUENCY);
#ifdef SERIAL_SPEED
	printMsg(&logger, "SERIAL_SPEED=%d", SERIAL_SPEED);
#endif

#ifdef CORTEX_MAX_KERNEL_PRIORITY
	printMsg(&logger, "CORTEX_MAX_KERNEL_PRIORITY=%d", CORTEX_MAX_KERNEL_PRIORITY);
#endif

#ifdef STM32_ADCCLK
	printMsg(&logger, "STM32_ADCCLK=%d", STM32_ADCCLK);
	printMsg(&logger, "STM32_TIMCLK1=%d", STM32_TIMCLK1);
	printMsg(&logger, "STM32_TIMCLK2=%d", STM32_TIMCLK2);
#endif
#ifdef STM32_PCLK1
	printMsg(&logger, "STM32_PCLK1=%d", STM32_PCLK1);
	printMsg(&logger, "STM32_PCLK2=%d", STM32_PCLK2);
#endif

	printMsg(&logger, "PORT_IDLE_THREAD_STACK_SIZE=%d", PORT_IDLE_THREAD_STACK_SIZE);

	printMsg(&logger, "CH_DBG_ENABLE_ASSERTS=%d", CH_DBG_ENABLE_ASSERTS);
	printMsg(&logger, "CH_DBG_ENABLED=%d", CH_DBG_ENABLED);
	printMsg(&logger, "CH_DBG_SYSTEM_STATE_CHECK=%d", CH_DBG_SYSTEM_STATE_CHECK);
	printMsg(&logger, "CH_DBG_ENABLE_STACK_CHECK=%d", CH_DBG_ENABLE_STACK_CHECK);

#ifdef EFI_WAVE_ANALYZER
	printMsg(&logger, "EFI_WAVE_ANALYZER=%d", EFI_WAVE_ANALYZER);
#endif
#ifdef EFI_TUNER_STUDIO
	printMsg(&logger, "EFI_TUNER_STUDIO=%d", EFI_TUNER_STUDIO);
#else
	printMsg(&logger, "EFI_TUNER_STUDIO=%d", 0);
#endif

#ifdef EFI_SIGNAL_EXECUTOR_SLEEP
	printMsg(&logger, "EFI_SIGNAL_EXECUTOR_SLEEP=%d", EFI_SIGNAL_EXECUTOR_SLEEP);
#endif

#ifdef EFI_SIGNAL_EXECUTOR_HW_TIMER
	printMsg(&logger, "EFI_SIGNAL_EXECUTOR_HW_TIMER=%d", EFI_SIGNAL_EXECUTOR_HW_TIMER);
#endif

#if defined(EFI_SHAFT_POSITION_INPUT) || defined(__DOXYGEN__)
	printMsg(&logger, "EFI_SHAFT_POSITION_INPUT=%d", EFI_SHAFT_POSITION_INPUT);
#endif
#ifdef EFI_INTERNAL_ADC
	printMsg(&logger, "EFI_INTERNAL_ADC=%d", EFI_INTERNAL_ADC);
#endif

//	printSimpleMsg(&logger, "", );
//	printSimpleMsg(&logger, "", );

	/**
	 * Time to finish output. This is needed to avoid mix-up of this methods output and console command confirmation
	 */
	chThdSleepMilliseconds(5);
}

/**
 * This methods prints all threads and their total times
 */
static void cmd_threads(void) {
#if CH_DBG_THREADS_PROFILING || defined(__DOXYGEN__)
	static const char *states[] = { THD_STATE_NAMES };
	Thread *tp;

	print("    addr    stack prio refs     state time\r\n");
	tp = chRegFirstThread();
	do {
		print("%.8lx [%.8lx] %4lu %4lu %9s %lu %s\r\n", (uint32_t) tp, 0, (uint32_t) tp->p_prio,
				(uint32_t) (tp->p_refs - 1), states[tp->p_state], (uint32_t) tp->p_time, tp->p_name);
		tp = chRegNextThread(tp);
	} while (tp != NULL);
#endif
}

/**
 * This methods prints the message to whatever is configured as our primary console
 */
void print(const char *format, ...) {
#if !EFI_UART_ECHO_TEST_MODE
	if (!isConsoleReady()) {
		return;
	}
	va_list ap;
	va_start(ap, format);
	chvprintf((BaseSequentialStream*) getConsoleChannel(), format, ap);
	va_end(ap);
#endif /* EFI_UART_ECHO_TEST_MODE */
}

void initializeConsole(Logging *sharedLogger) {
	initIntermediateLoggingBuffer();
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
