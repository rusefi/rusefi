/**
 * @file	console_io.cpp
 *
 * @date Dec 29, 2012
 * @author Andrey Belomutskiy, (c) 2012-2018
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

#include "global.h"
#include "engine.h"
#include "console_io.h"
#include "rfiutil.h"
#include "tunerstudio.h"

#if EFI_SIMULATOR
#include "rusEfiFunctionalTest.h"
#endif /*EFI_SIMULATOR */

EXTERN_ENGINE;

#if HAL_USE_SERIAL_USB
#include "usbcfg.h"
#include "usbconsole.h"
extern SerialUSBDriver SDU1;
#endif /* HAL_USE_SERIAL_USB */

int lastWriteSize;
int lastWriteActual;

static bool isSerialConsoleStarted = false;

static event_listener_t consoleEventListener;

bool consoleByteArrived = false;

void onDataArrived(void) {
	consoleByteArrived = true;
}


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
/* let's keep this dead code for a bit
static bool getConsoleLine(BaseSequentialStream *chp, char *line, unsigned size) {
	char *p = line;

	while (true) {
		if (!isCommandLineConsoleReady()) {
			// we better do not read from serial before it is ready
			chThdSleepMilliseconds(10);
			continue;
		}

		short c = (short) streamGet(chp);
		onDataArrived();

#if defined(EFI_CONSOLE_SERIAL_DEVICE)

			uint32_t flags;
			chSysLock()
			;

			flags = chEvtGetAndClearFlagsI(&consoleEventListener);
			chSysUnlock()
			;
			if (flags & SD_OVERRUN_ERROR) {
//				firmwareError(OBD_PCM_Processor_Fault, "serial overrun");
			}

#endif

#if EFI_UART_ECHO_TEST_MODE
		// That's test code - let's test connectivity
		consolePutChar((uint8_t) c);
		continue;
#endif

		if (c < 0 || c == 4) {
			return true;
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
		if (c == '\n') {
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
*/
CommandHandler console_line_callback;

#if (defined(EFI_CONSOLE_UART_DEVICE) && ! EFI_SIMULATOR )
static SerialConfig serialConfig = { 0, 0, USART_CR2_STOP1_BITS | USART_CR2_LINEN, 0 };
#endif

#if EFI_PROD_CODE || EFI_EGT

bool isUsbSerial(BaseChannel * channel) {
#if HAL_USE_SERIAL_USB
	return channel == (BaseChannel *) &CONSOLE_USB_DEVICE;
#else
	return false;
#endif
}

BaseChannel * getConsoleChannel(void) {
#if defined(EFI_CONSOLE_SERIAL_DEVICE)
	return (BaseChannel *) EFI_CONSOLE_SERIAL_DEVICE;
#endif /* EFI_CONSOLE_SERIAL_DEVICE */

#if defined(EFI_CONSOLE_UART_DEVICE)
	return (BaseChannel *) EFI_CONSOLE_UART_DEVICE;
#endif /* EFI_CONSOLE_UART_DEVICE */

#if HAL_USE_SERIAL_USB
	return (BaseChannel *) &CONSOLE_USB_DEVICE;
#else
	return NULL;
#endif /* HAL_USE_SERIAL_USB */
}

bool isCommandLineConsoleReady(void) {
	return isSerialConsoleStarted;
}
#endif /* EFI_PROD_CODE || EFI_EGT */

#if !defined(EFI_CONSOLE_NO_THREAD)

ts_channel_s binaryConsole;

static THD_WORKING_AREA(consoleThreadStack, 3 * UTILITY_THREAD_STACK_SIZE);
static THD_FUNCTION(consoleThreadEntryPoint, arg) {
	(void) arg;
	chRegSetThreadName("console thread");

	binaryConsole.channel = (BaseChannel *) getConsoleChannel();
	if (binaryConsole.channel != NULL) {
#if EFI_TUNER_STUDIO
		runBinaryProtocolLoop(&binaryConsole);
#endif /* EFI_TUNER_STUDIO */
	}
}

#endif /* EFI_CONSOLE_NO_THREAD */

// 10 seconds
#define CONSOLE_WRITE_TIMEOUT 10000

void consolePutChar(int x) {
	chnWriteTimeout(getConsoleChannel(), (const uint8_t *)&x, 1, CONSOLE_WRITE_TIMEOUT);
}

void consoleOutputBuffer(const uint8_t *buf, int size) {
	lastWriteSize = size;
#if !EFI_UART_ECHO_TEST_MODE
	lastWriteActual = chnWriteTimeout(getConsoleChannel(), buf, size, CONSOLE_WRITE_TIMEOUT);
//	if (r != size)
//		firmwareError(OBD_PCM_Processor_Fault, "Partial console write");
#endif /* EFI_UART_ECHO_TEST_MODE */
}

static Logging *logger;

void startConsole(Logging *sharedLogger, CommandHandler console_line_callback_p) {
	logger = sharedLogger;
	console_line_callback = console_line_callback_p;

#if (defined(EFI_CONSOLE_SERIAL_DEVICE) && ! EFI_SIMULATOR)
		/*
		 * Activates the serial
		 * it is important to set 'NONE' as flow control! in terminal application on the PC
		 */
		serialConfig.speed = engineConfiguration->uartConsoleSerialSpeed;
		sdStart(EFI_CONSOLE_SERIAL_DEVICE, &serialConfig);

// cannot use pin repository here because pin repository prints to console
		palSetPadMode(EFI_CONSOLE_RX_PORT, EFI_CONSOLE_RX_PIN, PAL_MODE_ALTERNATE(EFI_CONSOLE_AF));
		palSetPadMode(EFI_CONSOLE_TX_PORT, EFI_CONSOLE_TX_PIN, PAL_MODE_ALTERNATE(EFI_CONSOLE_AF));

		isSerialConsoleStarted = true;

		chEvtRegisterMask((event_source_t *) chnGetEventSource(EFI_CONSOLE_UART_DEVICE), &consoleEventListener, 1);
#endif /* EFI_PROD_CODE */

#if !defined(EFI_CONSOLE_NO_THREAD)
	chThdCreateStatic(consoleThreadStack, sizeof(consoleThreadStack), NORMALPRIO, (tfunc_t)consoleThreadEntryPoint, NULL);
#endif /* EFI_CONSOLE_NO_THREAD */

}

