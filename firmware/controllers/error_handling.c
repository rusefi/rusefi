/**
 * @file error_handling.c
 *
 * @date Apr 1, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "error_handling.h"
#include "wave_math.h"

static time_t timeOfPreviousWarning = (systime_t) -10 * CH_FREQUENCY;

static Logging logger;

extern int warningEnabled;

/**
 * @returns TRUE in case there are too many warnings
 */
int warning(obd_code_e code, const char *fmt, ...) {
	time_t now = chTimeNow();
	if (overflowDiff(now, timeOfPreviousWarning) < CH_FREQUENCY || !warningEnabled)
		return TRUE; // we just had another warning, let's not spam
	timeOfPreviousWarning = now;

	resetLogging(&logger); // todo: is 'reset' really needed here?
	appendMsgPrefix(&logger);
	va_list ap;
	va_start(ap, fmt);
	vappendPrintf(&logger, fmt, ap);
	va_end(ap);
	append(&logger, DELIMETER);
	scheduleLogging(&logger);

	return FALSE;
}

void initErrorHandling(void) {
	initLogging(&logger, "error handling");

}
