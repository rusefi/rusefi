/**
 * @file error_handling.cpp
 *
 * @date Apr 1, 2014
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "main.h"
#include "error_handling.h"
#include "io_pins.h"
#include "memstreams.h"
#include "efilib2.h"
#include "engine.h"
//todo: move into simulator global
#include <chprintf.h>

#if EFI_HD44780_LCD || defined(__DOXYGEN__)
#include "lcd_HD44780.h"
#endif /* EFI_HD44780_LCD */

static LoggingWithStorage logger("error handling");

EXTERN_ENGINE;

#define WARNING_PREFIX "WARNING: "

extern int warningEnabled;
extern bool main_loop_started;
extern bool hasFirmwareErrorFlag;

const char *dbg_panic_file;
int dbg_panic_line;

void chDbgPanic3(const char *msg, const char * file, int line) {
	if (hasFatalError())
		return;
	dbg_panic_file = file;
	dbg_panic_line = line;
#if CH_DBG_SYSTEM_STATE_CHECK || defined(__DOXYGEN__)
	dbg_panic_msg = msg;
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
		chSysHalt();
	}
}

#define WARNING_BUFFER_SIZE 80
static char warningBuffer[WARNING_BUFFER_SIZE];
static bool isWarningStreamInitialized = false;
static MemoryStream warningStream;

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

/**
 * OBD_PCM_Processor_Fault is the general error code for now
 *
 * @returns TRUE in case there are too many warnings
 */
bool warning(obd_code_e code, const char *fmt, ...) {
	if (hasFirmwareErrorFlag)
		return true;
	efiAssert(isWarningStreamInitialized, "warn stream not initialized", false);

#if EFI_UNIT_TEST || EFI_SIMULATOR || defined(__DOXYGEN__)
	printf("warning %s\r\n", fmt);
#endif

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

	return FALSE;
}

char *getWarning(void) {
	return warningBuffer;
}

uint32_t lastLockTime;
uint32_t maxLockTime = 0;

// todo: move this field to trigger_central?
bool isInsideTriggerHandler = false;

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
	msObjectInit(&warningStream, (uint8_t *) warningBuffer, WARNING_BUFFER_SIZE, 0);
	isWarningStreamInitialized = true;
}

//todo: move into simulator global
typedef VTList virtual_timers_list_t;

extern virtual_timers_list_t vtlist;
extern bool main_loop_started;

int getVtSizeEstimate(void) {
	virtual_timer_t *first = vtlist.vt_next;
	virtual_timer_t *cur = first->vt_next;
	int c = 0;
	while (c++ < 20 && cur != first) {
		cur = cur->vt_next;
	}
	return c;
}

int globalVt;

int allReady = 0;
void assertVtList(void) {
	if (!main_loop_started)
		return;
	globalVt = getVtSizeEstimate();
	//efiAssertVoid(globalVt > 3, "VT list?");
	if(globalVt <=3 ) {
		allReady++;
	}
}
