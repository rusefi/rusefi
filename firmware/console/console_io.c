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
extern SerialUSBDriver SDU1;
#include "usbcfg.h"
#include "usbconsole.h"
#endif
#include "rfiutil.h"

int lastWriteSize;
int lastWriteActual;

static bool isSerialConsoleStarted = false;

static EventListener consoleEventListener;

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
static bool getConsoleLine(BaseSequentialStream *chp, char *line, unsigned size) {
	char *p = line;

	while (TRUE) {
		if (!isConsoleReady()) {
			// we better do not read from USB serial before it is ready
			chThdSleepMilliseconds(10);
			continue;
		}

		short c = (short) chSequentialStreamGet(chp);

		if (isSerialOverUart()) {
			uint32_t flags;
			chSysLock()
			;

			flags = chEvtGetAndClearFlagsI(&consoleEventListener);
			chSysUnlock()
			;

			if (flags & SD_OVERRUN_ERROR) {
//				firmwareError("serial overrun");
			}

		}

#if EFI_UART_ECHO_TEST_MODE
		/**
		 * That's test code - let's test connectivity
		 */
		consolePutChar((uint8_t) c);
		continue;
#endif

		if (c < 0 || c == 4) {
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
			return false;
		}
		if (c < 0x20) {
			continue;
                }
		if (p < line + size - 1) {
			consolePutChar((uint8_t) c);
			*p++ = (char) c;
		}
	}
}

// todo: this is ugly as hell!
static char consoleInput[] = "                                                                              ";

void (*console_line_callback)(char *);

static bool is_serial_over_uart;

bool isSerialOverUart(void) {
	return is_serial_over_uart;
}

static THD_WORKING_AREA(consoleThreadStack, 2 * UTILITY_THREAD_STACK_SIZE);
static msg_t consoleThreadThreadEntryPoint(void *arg) {
	(void) arg;
	chRegSetThreadName("console thread");

	while (TRUE) {
		efiAssert(getRemainingStack(chThdSelf()) > 32, "lowstck#9e", 0);
		bool end = getConsoleLine((BaseSequentialStream*) getConsoleChannel(), consoleInput, sizeof(consoleInput));
		if (end) {
			// firmware simulator is the only case when this happens
			continue;
		}

		(console_line_callback)(consoleInput);
	}
#if defined __GNUC__
	return false;
#endif        
}

#if EFI_PROD_CODE

static SerialConfig serialConfig = { SERIAL_SPEED, 0, USART_CR2_STOP1_BITS | USART_CR2_LINEN, 0 };

SerialDriver * getConsoleChannel(void) {
	if (isSerialOverUart()) {
		return (SerialDriver *) EFI_CONSOLE_UART_DEVICE;
	} else {
		return (SerialDriver *) &SDU1;
	}
}

bool isConsoleReady(void) {
	if (isSerialOverUart()) {
		return isSerialConsoleStarted;
	} else {
		return is_usb_serial_ready();
	}
}

#endif /* EFI_PROD_CODE */

void consolePutChar(int x) {
	chSequentialStreamPut(getConsoleChannel(), (uint8_t )(x));
}

// 10 seconds
#define CONSOLE_WRITE_TIMEOUT 10000

void consoleOutputBuffer(const uint8_t *buf, int size) {
	lastWriteSize = size;
#if !EFI_UART_ECHO_TEST_MODE
	lastWriteActual = chnWriteTimeout(getConsoleChannel(), buf, size, CONSOLE_WRITE_TIMEOUT);
//	if (r != size)
//		firmwareError("Partial console write");
#endif /* EFI_UART_ECHO_TEST_MODE */
}

void startConsole(void (*console_line_callback_p)(char *)) {
	console_line_callback = console_line_callback_p;

#if EFI_PROD_CODE

	palSetPadMode(CONSOLE_MODE_SWITCH_PORT, CONSOLE_MODE_SWITCH_PIN, PAL_MODE_INPUT_PULLUP);

	is_serial_over_uart = GET_CONSOLE_MODE_VALUE() == EFI_USE_UART_FOR_CONSOLE;

	if (isSerialOverUart()) {
		/*
		 * Activates the serial using the driver default configuration (that's 38400)
		 * it is important to set 'NONE' as flow control! in terminal application on the PC
		 */
		sdStart(EFI_CONSOLE_UART_DEVICE, &serialConfig);

// cannot use pin repository here because pin repository prints to console
		palSetPadMode(EFI_CONSOLE_RX_PORT, EFI_CONSOLE_RX_PIN, PAL_MODE_ALTERNATE(EFI_CONSOLE_AF));
		palSetPadMode(EFI_CONSOLE_TX_PORT, EFI_CONSOLE_TX_PIN, PAL_MODE_ALTERNATE(EFI_CONSOLE_AF));

		isSerialConsoleStarted = TRUE;

		chEvtRegisterMask((EventSource *) chnGetEventSource(EFI_CONSOLE_UART_DEVICE), &consoleEventListener, 1);
	} else {
		usb_serial_start();
	}
#endif /* EFI_PROD_CODE */
	chThdCreateStatic(consoleThreadStack, sizeof(consoleThreadStack), NORMALPRIO, consoleThreadThreadEntryPoint, NULL);
}

extern cnt_t dbg_isr_cnt;

/**
 * @return TRUE if already in locked context
 */
bool lockAnyContext(void) {
	int alreadyLocked = isLocked();
	if (alreadyLocked)
		return TRUE;
	if (isIsrContext()) {
		chSysLockFromIsr()
		;
	} else {
		chSysLock()
		;
	}
	return false;
}

bool lockOutputBuffer(void) {
	return lockAnyContext();
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
