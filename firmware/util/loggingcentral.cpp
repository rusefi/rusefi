/**
 * @file	loggingcentral.cpp
 *
 *
 * As of May 2019 we have given up on text-based 'push' terminal mode. At the moment binary protocol
 * is the consumen of this logging buffer.
 *
 * @date Mar 8, 2015
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"
#include "efilib.h"

#if ! EFI_UNIT_TEST

typedef char log_buf_t[DL_OUTPUT_BUFFER];

/**
 * we need to leave a byte for zero terminator, also two bytes for the \r\n in
 * printWithLength, also couple of bytes just in case
 */
#define MAX_DL_CAPACITY (DL_OUTPUT_BUFFER - 5)

/**
 * This is the buffer into which all the data providers write
 */
static char *accumulationBuffer;

static log_buf_t pendingBuffers0;
static log_buf_t pendingBuffers1;

/**
 * amount of data accumulated so far
 */
static uint32_t accumulatedSize;

/**
 * We copy all the pending data into this buffer once we are ready to push it out
 */
static char * outputBuffer;

/**
 * This method appends the content of specified thread-local logger into the global buffer
 * of logging content.
 */
void scheduleLogging(Logging *logging) {
#if EFI_TEXT_LOGGING
#ifdef EFI_PRINT_MESSAGES_TO_TERMINAL
	print(logging->buffer);
	print("\r\n");
#endif /* EFI_PRINT_MESSAGES_TO_TERMINAL */
	// this could be done without locking
	int newLength = efiStrlen(logging->buffer);

	bool alreadyLocked = lockOutputBuffer();
	if (accumulatedSize + newLength >= MAX_DL_CAPACITY) {
		/**
		 * if no one is consuming the data we have to drop it
		 * this happens in case of serial-over-USB, todo: find a better solution?
		 */
		if (!alreadyLocked) {
			unlockOutputBuffer();
		}
		resetLogging(logging);
		return;
	}
	// memcpy is faster then strcpy because it is not looking for line terminator
	memcpy(accumulationBuffer + accumulatedSize, logging->buffer, newLength + 1);
	accumulatedSize += newLength;
	if (!alreadyLocked) {
		unlockOutputBuffer();
	}
	resetLogging(logging);
#endif /* EFI_TEXT_LOGGING */
}

/**
 * Actual communication layer invokes this method when it's ready to send some data out
 *
 * this method should always be invoked from the same thread!
 * @return pointer to the buffer which should be print to console
 */
char * swapOutputBuffers(int *actualOutputBufferSize) {
#if EFI_ENABLE_ASSERTS
	int expectedOutputSize;
#endif /* EFI_ENABLE_ASSERTS */
	{ // start of critical section
		bool alreadyLocked = lockOutputBuffer();
		/**
		 * we cannot output under syslock, we simply rotate which buffer is which
		 */
		char *temp = outputBuffer;

#if EFI_ENABLE_ASSERTS
		expectedOutputSize = accumulatedSize;
#endif /* EFI_ENABLE_ASSERTS */
		outputBuffer = accumulationBuffer;

		accumulationBuffer = temp;
		accumulatedSize = 0;
		accumulationBuffer[0] = 0;

		if (!alreadyLocked) {
			unlockOutputBuffer();
		}
	} // end of critical section

	*actualOutputBufferSize = efiStrlen(outputBuffer);
#if EFI_ENABLE_ASSERTS
	if (*actualOutputBufferSize != expectedOutputSize) {
		int sizeToShow = minI(10, *actualOutputBufferSize);
		int offsetToShow = *actualOutputBufferSize - sizeToShow;
		firmwareError(ERROR_LOGGING_SIZE_CALC, "lsize mismatch %d/%d [%s]", *actualOutputBufferSize, expectedOutputSize,
				&outputBuffer[offsetToShow]);
		return NULL;
	}
#endif /* EFI_ENABLE_ASSERTS */
	return outputBuffer;
}

void initLoggingCentral(void) {
	pendingBuffers0[0] = 0;
	pendingBuffers1[0] = 0;
	accumulationBuffer = pendingBuffers0;
	outputBuffer = pendingBuffers1;
	accumulatedSize = 0;
}

/**
 * rusEfi business logic invokes this method in order to eventually print stuff to rusEfi console
 *
 * this whole method is executed under syslock so that we can have multiple threads use the same shared buffer
 * in order to reduce memory usage
 *
 * this is really 'global lock + printf + scheduleLogging + unlock' a bit more clear
 */
void scheduleMsg(Logging *logging, const char *fmt, ...) {
	for (unsigned int i = 0;i<strlen(fmt);i++) {
		// todo: open question which layer would not handle CR/LF properly?
		efiAssertVoid(OBD_PCM_Processor_Fault, fmt[i] != '\n', "No CRLF please");
	}
#if EFI_TEXT_LOGGING
	if (logging == NULL) {
		warning(CUSTOM_ERR_LOGGING_NULL, "logging NULL");
		return;
	}
	int wasLocked = lockAnyContext();
	resetLogging(logging); // todo: is 'reset' really needed here?
	appendMsgPrefix(logging);

	va_list ap;
	va_start(ap, fmt);
	logging->vappendPrintf(fmt, ap);
	va_end(ap);

	appendMsgPostfix(logging);
	scheduleLogging(logging);
	if (!wasLocked) {
		unlockAnyContext();
	}
#endif /* EFI_TEXT_LOGGING */
}


#endif /* EFI_UNIT_TEST */
