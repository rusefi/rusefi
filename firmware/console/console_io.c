/**
 * @file	console_io.c
 *
 * @date Dec 29, 2012
 * @author Andrey Belomutskiy, (c) 2012-2014
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

#include "main.h"
#include "console_io.h"

#if EFI_PROD_CODE
#include "usbconsole.h"
#endif
#include "rfiutil.h"

/**
 * @brief   Reads a whole line from the input channel.
 *
 * @param[in] chp       pointer to a @p BaseChannel object
 * @param[in] line      pointer to the line buffer
 * @param[in] size      buffer maximum length
 * @return              The operation status.
 * @retval TRUE         the channel was reset or CTRL-D pressed.
 * @retval FALSE        operation successful.
 */
static bool_t getConsoleLine(BaseSequentialStream *chp, char *line, unsigned size) {
	char *p = line;

	while (TRUE) {
		if (!is_serial_ready()) {
			// we better do not read from USB serial before it is ready
			chThdSleepMilliseconds(10);
			continue;
		}

		short c = (short) chSequentialStreamGet(chp);
		if (c < 0)
			return TRUE;
		if (c == 4) {
			return TRUE;
		}
		if (c == 8) {
			if (p != line) {
				// backspace
				consolePutChar((uint8_t) c);
				consolePutChar(0x20);
				consolePutChar((uint8_t) c);
				p--;
			}
			continue;
		}
		if (c == '\r') {
			consolePutChar('\r');
			consolePutChar('\n');
			*p = 0;
			return FALSE;
		}
		if (c < 0x20)
			continue;
		if (p < line + size - 1) {
			consolePutChar((uint8_t) c);
			*p++ = (char) c;
		}
	}
}

// todo: this is ugly as hell!
static char consoleInput[] = "                                                                              ";

void (*console_line_callback)(char *);

static WORKING_AREA(consoleThreadStack, 2 * UTILITY_THREAD_STACK_SIZE);
static msg_t consoleThreadThreadEntryPoint(void *arg) {
	(void) arg;
	chRegSetThreadName("console thread");

	while (TRUE) {
		bool_t end = getConsoleLine((BaseSequentialStream *) CONSOLE_CHANNEL, consoleInput, sizeof(consoleInput));
		if (end) {
			// firmware simulator is the only case when this happens
			continue;
		}

		(console_line_callback)(consoleInput);
	}
	return FALSE;
}

#if EFI_SERIAL_OVER_USB
#else
#if EFI_SERIAL_OVER_UART
static SerialConfig serialConfig = { SERIAL_SPEED, 0, USART_CR2_STOP1_BITS | USART_CR2_LINEN, 0 };
#endif /* EFI_SERIAL_OVER_UART */
#endif /* EFI_SERIAL_OVER_USB */

#if ! defined EFI_SERIAL_OVER_USB && ! EFI_SIMULATOR
int is_serial_ready(void) {
	return TRUE;
}
#endif

void consolePutChar(int x) {
	chSequentialStreamPut(CONSOLE_CHANNEL, (uint8_t )(x));
}

void consoleOutputBuffer(const int8_t *buf, int size) {
	chSequentialStreamWrite(CONSOLE_CHANNEL, buf, size);
}

void startChibiosConsole(void (*console_line_callback_p)(char *)) {
	console_line_callback = console_line_callback_p;
#if EFI_SERIAL_OVER_USB
	usb_serial_start();

#else
#if EFI_SERIAL_OVER_UART
	/*
	 * Activates the serial using the driver default configuration (that's 38400)
	 * it is important to set 'NONE' as flow control! in terminal application on the PC
	 */
	sdStart(CONSOLE_CHANNEL, &serialConfig);

	// cannot use pin repository here because pin repository prints to console
	palSetPadMode(EFI_CONSOLE_RX_PORT, EFI_CONSOLE_RX_PIN, PAL_MODE_ALTERNATE(EFI_CONSOLE_AF));
	palSetPadMode(EFI_CONSOLE_TX_PORT, EFI_CONSOLE_TX_PIN, PAL_MODE_ALTERNATE(EFI_CONSOLE_AF));
#endif /* EFI_SERIAL_OVER_UART */
#endif /* EFI_SERIAL_OVER_USB */
	chThdCreateStatic(consoleThreadStack, sizeof(consoleThreadStack), NORMALPRIO, consoleThreadThreadEntryPoint, NULL);
}

extern cnt_t dbg_isr_cnt;

void lockAnyContext(void) {
	if (isIsrContext()) {
		chSysLockFromIsr()
		;
	} else {
		chSysLock()
		;
	}
}

void lockOutputBuffer(void) {
	lockAnyContext();
}

void unlockAnyContext(void) {
	if (isIsrContext()) {
		chSysUnlockFromIsr()
		;
	} else {
		chSysUnlock()
		;
	}
}

void unlockOutputBuffer(void) {
	unlockAnyContext();
}
