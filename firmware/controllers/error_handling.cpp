/**
 * @file error_handling.cpp
 *
 * @date Apr 1, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "main.h"
#include "error_handling.h"
#include "io_pins.h"
#include "efilib2.h"
#include "engine.h"

#if EFI_SIMULATOR || EFI_PROD_CODE
//todo: move into simulator global
#include "memstreams.h"
#include <chprintf.h>
static MemoryStream warningStream;
static MemoryStream firmwareErrorMessageStream;
extern OutputPin communicationPin;

#endif


#define WARNING_BUFFER_SIZE 80
static char warningBuffer[WARNING_BUFFER_SIZE];
static bool isWarningStreamInitialized = false;




#if EFI_HD44780_LCD || defined(__DOXYGEN__)
#include "lcd_HD44780.h"
#endif /* EFI_HD44780_LCD */

static LoggingWithStorage logger("error handling");

EXTERN_ENGINE;

#define WARNING_PREFIX "WARNING: "

extern int warningEnabled;
extern bool main_loop_started;

fatal_msg_t errorMessageBuffer;
bool hasFirmwareErrorFlag = false;

const char *dbg_panic_file;
int dbg_panic_line;

char *getFirmwareError(void) {
	return (char*) errorMessageBuffer;
}

#if EFI_SIMULATOR || EFI_PROD_CODE

void chDbgPanic3(const char *msg, const char * file, int line) {
	if (hasFatalError())
		return;
	dbg_panic_file = file;
	dbg_panic_line = line;
#if CH_DBG_SYSTEM_STATE_CHECK || defined(__DOXYGEN__)
	ch.dbg.panic_msg = msg;
#endif /* CH_DBG_SYSTEM_STATE_CHECK */

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	ON_FATAL_ERROR();
#else
	printf("chDbgPanic3 %s %s%d", msg, file, line);
	exit(-1);
#endif

#if EFI_HD44780_LCD || defined(__DOXYGEN__)
	lcdShowFatalMessage((char *) msg);
#endif /* EFI_HD44780_LCD */

	if (!main_loop_started) {
		print("fatal %s %s:%d\r\n", msg, file, line);
		chThdSleepSeconds(1);
		chSysHalt("Main loop did not start");
	}
}


/**
 * @param forIndicator if we want to retrieving value for TS indicator, this case a minimal period is applued
 */
bool isWarningNow(efitimesec_t now, bool forIndicator) {
	int period = forIndicator ? maxI(3, engineConfiguration->warningPeriod) : engineConfiguration->warningPeriod;
	return absI(now - engine->engineState.timeOfPreviousWarning) < period;
}

void addWarningCode(obd_code_e code) {
	engine->engineState.warningCounter++;
	engine->engineState.lastErrorCode = code;
}

// todo: move to some util file & reuse for 'firmwareError' method
void printToStream(MemoryStream *stream, const char *fmt, va_list ap) {
	stream->eos = 0; // reset
	chvprintf((BaseSequentialStream *) stream, fmt, ap);
	stream->buffer[stream->eos] = 0;
}
#endif

int warningCounter = 0;

/**
 * OBD_PCM_Processor_Fault is the general error code for now
 *
 * @returns TRUE in case there are too many warnings
 */
bool warning(obd_code_e code, const char *fmt, ...) {
	if (hasFirmwareErrorFlag)
		return true;

#if EFI_SIMULATOR || defined(__DOXYGEN__)
	printf("sim_warning %s\r\n", fmt);
#endif

#if EFI_SIMULATOR || EFI_PROD_CODE
	efiAssert(isWarningStreamInitialized, "warn stream not initialized", false);
	addWarningCode(code);

	efitimesec_t now = getTimeNowSeconds();
	if (isWarningNow(now, false) || !warningEnabled)
		return true; // we just had another warning, let's not spam
	engine->engineState.timeOfPreviousWarning = now;

	resetLogging(&logger); // todo: is 'reset' really needed here?
	appendMsgPrefix(&logger);

	append(&logger, WARNING_PREFIX);

	va_list ap;
	va_start(ap, fmt);
	printToStream(&warningStream, fmt, ap);
	va_end(ap);

	append(&logger, warningBuffer);
	append(&logger, DELIMETER);
	scheduleLogging(&logger);
#else
	warningCounter++;
	printf("unit_test_warning: ");
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	printf("\r\n");

#endif
	return false;
}

char *getWarning(void) {
	return warningBuffer;
}

uint32_t lastLockTime;
uint32_t maxLockTime = 0;

void onLockHook(void) {
	lastLockTime = GET_TIMESTAMP();
}

void onUnlockHook(void) {
	uint32_t t = GET_TIMESTAMP() - lastLockTime;
	if (t > maxLockTime) {
		maxLockTime = t;
	}
//	if (t > 2800) {
//		// un-comment this if you want a nice stop for a breakpoint
//		maxLockTime = t + 1;
//	}
}

void initErrorHandling(void) {
#if EFI_SIMULATOR || EFI_PROD_CODE || defined(__DOXYGEN__)
	msObjectInit(&warningStream, (uint8_t *) warningBuffer, WARNING_BUFFER_SIZE, 0);
	msObjectInit(&firmwareErrorMessageStream, errorMessageBuffer, sizeof(errorMessageBuffer), 0);
#endif
	isWarningStreamInitialized = true;
}

void firmwareError(obd_code_e code, const char *fmt, ...) {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	if (hasFirmwareErrorFlag)
		return;
	addWarningCode(code);
	ON_FATAL_ERROR()
	;
	hasFirmwareErrorFlag = true;
	if (indexOf(fmt, '%') == -1) {
		/**
		 * in case of simple error message let's reduce stack usage
		 * because chvprintf might be causing an error
		 */
		strncpy((char*) errorMessageBuffer, fmt, sizeof(errorMessageBuffer) - 1);
		errorMessageBuffer[sizeof(errorMessageBuffer) - 1] = 0; // just to be sure
	} else {
		firmwareErrorMessageStream.eos = 0; // reset
		va_list ap;
		va_start(ap, fmt);
		chvprintf((BaseSequentialStream *) &firmwareErrorMessageStream, fmt, ap);
		va_end(ap);
		// todo: reuse warning buffer helper method
		firmwareErrorMessageStream.buffer[firmwareErrorMessageStream.eos] = 0; // need to terminate explicitly
	}
#else
	printf("firmwareError [%s]\r\n", fmt);

	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	printf("\r\n");

#if EFI_SIMULATOR || defined(__DOXYGEN__)
	exit(-1);
#endif /* EFI_SIMULATOR */
#endif
}

