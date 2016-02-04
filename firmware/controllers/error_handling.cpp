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

#if EFI_HD44780_LCD
#include "lcd_HD44780.h"
#endif /* EFI_HD44780_LCD */

static time_t timeOfPreviousWarning = -10;
static LoggingWithStorage logger("error handling");

#define WARNING_PREFIX "WARNING: "

extern int warningEnabled;
extern bool main_loop_started;

const char *dbg_panic_file;
int dbg_panic_line;

void chDbgPanic3(const char *msg, const char * file, int line) {
	if (hasFatalError())
		return;
	dbg_panic_file = file;
	dbg_panic_line = line;
#if CH_DBG_SYSTEM_STATE_CHECK
	dbg_panic_msg = msg;
#endif /* CH_DBG_SYSTEM_STATE_CHECK */

	ON_FATAL_ERROR();
#if EFI_HD44780_LCD
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
static MemoryStream warningStream;

static int warningCounter = 0;
/**
 * OBD_PCM_Processor_Fault is the general error code for now
 *
 * @returns TRUE in case there are too many warnings
 */
int warning(obd_code_e code, const char *fmt, ...) {
	UNUSED(code);
  
	int now = getTimeNowSeconds();
	if (absI(now - timeOfPreviousWarning) < 10 || !warningEnabled)
		return true; // we just had another warning, let's not spam
	timeOfPreviousWarning = now;

	warningCounter++;

	resetLogging(&logger); // todo: is 'reset' really needed here?
	appendMsgPrefix(&logger);

	va_list ap;
	va_start(ap, fmt);
	append(&logger, WARNING_PREFIX);
	warningStream.eos = 0; // reset
	chvprintf((BaseSequentialStream *) &warningStream, fmt, ap);
	warningStream.buffer[warningStream.eos] = 0;
	va_end(ap);

	append(&logger, warningBuffer);
	append(&logger, DELIMETER);
	scheduleLogging(&logger);

	return FALSE;
}

char *getWarninig(void) {
	return warningBuffer;
}

uint32_t lastLockTime;
uint32_t maxLockTime = 0;

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
}

extern virtual_timers_list_t vtlist;
extern bool main_loop_started;

int getVtSizeEstimate(void) {
	VirtualTimer *first = vtlist.vt_next;
	VirtualTimer *cur = first->vt_next;
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
