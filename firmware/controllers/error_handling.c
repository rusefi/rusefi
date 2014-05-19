/**
 * @file error_handling.c
 *
 * @date Apr 1, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "error_handling.h"
#include "io_pins.h"

#if EFI_HD44780_LCD
#include "lcd_HD44780.h"
#endif /* EFI_HD44780_LCD */

static time_t timeOfPreviousWarning = -10;
static Logging logger;

extern int warningEnabled;
extern int main_loop_started;

const char *dbg_panic_file;
int dbg_panic_line;

void chDbgPanic3(const char *msg, const char * file, int line) {
	if (hasFatalError())
		return;
	dbg_panic_file = file;
	dbg_panic_line = line;
	dbg_panic_msg = msg;


	setOutputPinValue(LED_ERROR, 1);
#if EFI_HD44780_LCD
	lcdShowFatalMessage((char *) msg);
#endif /* EFI_HD44780_LCD */
	if (!main_loop_started) {
		print("fatal %s %s:%d\r\n", msg, file, line);
		chThdSleepSeconds(1);
		chSysHalt();
	}
}

/**
 * @returns TRUE in case there are too many warnings
 */
int warning(obd_code_e code, const char *fmt, ...) {
	int now = getTimeNowSeconds();
	if (absI(now - timeOfPreviousWarning) < 10 || !warningEnabled)
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
