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

// 10 seconds
#define CONSOLE_WRITE_TIMEOUT 10000

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

#if (defined(EFI_CONSOLE_SERIAL_DEVICE) && ! EFI_SIMULATOR )
static SerialConfig serialConfig = { 0, 0, USART_CR2_STOP1_BITS | USART_CR2_LINEN, 0 };
#endif

#if (defined(EFI_CONSOLE_UART_DEVICE) && ! EFI_SIMULATOR )
/* Note: This structure is modified from the default ChibiOS layout! */
static UARTConfig uartConfig = { 
	.txend1_cb = NULL, .txend2_cb = NULL, .rxend_cb = NULL, .rxchar_cb = NULL, .rxerr_cb = NULL, 
	.speed = 0, .cr1 = 0, .cr2 = 0/*USART_CR2_STOP1_BITS*/ | USART_CR2_LINEN, .cr3 = 0,
	.timeout_cb = NULL, .rxhalf_cb = NULL
};

// To use UART driver instead of Serial, we need to imitate "BaseChannel" streaming functionality
static msg_t _putt(void *, uint8_t b, sysinterval_t timeout) {
	int n = 1;
	uartSendTimeout(EFI_CONSOLE_UART_DEVICE, (size_t *)&n, &b, timeout);
	return MSG_OK;
}
static size_t _writet(void *, const uint8_t *bp, size_t n, sysinterval_t timeout) {
	uartSendTimeout(EFI_CONSOLE_UART_DEVICE, (size_t *)&n, bp, timeout);
	return n;
}
static msg_t _put(void *ip, uint8_t b) {
#ifdef UART_USE_BLOCKING_SEND
	// this version can be called from the locked state (no interrupts)
	uart_lld_blocking_send(EFI_CONSOLE_UART_DEVICE, 1, (void *)&b);
#else
	// uartSendTimeout() needs interrupts to wait for the end of transfer, so we have to unlock them temporary
	bool wasLocked = isLocked();
	if (wasLocked)
		unlockAnyContext();
	_putt(ip, b, CONSOLE_WRITE_TIMEOUT);
	if (wasLocked)
		lockAnyContext();
#endif /* UART_USE_BLOCKING_WRITE */
	return MSG_OK;
}
static size_t _write(void *ip, const uint8_t *bp, size_t n) {
	return _writet(ip, bp, n, CONSOLE_WRITE_TIMEOUT);
}
static msg_t _gett(void *, sysinterval_t /*timeout*/) {
	return 0;
}
static size_t _readt(void *, uint8_t */*bp*/, size_t /*n*/, sysinterval_t /*timeout*/) {
	return 0;
}
static msg_t _get(void *) {
	return 0;
}
static size_t _read(void *, uint8_t */*bp*/, size_t /*n*/) {
	return 0;
}
static msg_t _ctl(void *, unsigned int /*operation*/, void */*arg*/) {
	return MSG_OK;
}

// This is a "fake" channel for getConsoleChannel() filled with our handlers
static const struct BaseChannelVMT uartChannelVmt = {
  .instance_offset = (size_t)0, .write = _write, .read = _read, .put = _put, .get = _get,
  .putt = _putt, .gett = _gett, .writet = _writet, .readt = _readt, .ctl = _ctl
};
static const BaseChannel uartChannel = { .vmt = &uartChannelVmt };
#endif /* EFI_CONSOLE_UART_DEVICE */

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
	return (BaseChannel *) &uartChannel;
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

static ts_channel_s binaryConsole;

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

		chEvtRegisterMask((event_source_t *) chnGetEventSource(EFI_CONSOLE_SERIAL_DEVICE), &consoleEventListener, 1);
#elif (defined(EFI_CONSOLE_UART_DEVICE) && ! EFI_SIMULATOR)
		uartConfig.speed = engineConfiguration->uartConsoleSerialSpeed;
		uartStart(EFI_CONSOLE_UART_DEVICE, &uartConfig);

		// cannot use pin repository here because pin repository prints to console
		palSetPadMode(EFI_CONSOLE_RX_PORT, EFI_CONSOLE_RX_PIN, PAL_MODE_ALTERNATE(EFI_CONSOLE_AF));
		palSetPadMode(EFI_CONSOLE_TX_PORT, EFI_CONSOLE_TX_PIN, PAL_MODE_ALTERNATE(EFI_CONSOLE_AF));

		isSerialConsoleStarted = true;
#endif /* EFI_CONSOLE_SERIAL_DEVICE || EFI_CONSOLE_UART_DEVICE */

#if !defined(EFI_CONSOLE_NO_THREAD)
	chThdCreateStatic(consoleThreadStack, sizeof(consoleThreadStack), NORMALPRIO, (tfunc_t)consoleThreadEntryPoint, NULL);
#endif /* EFI_CONSOLE_NO_THREAD */

}

