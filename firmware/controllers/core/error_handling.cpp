/**
 * @file error_handling.cpp
 *
 * @date Apr 1, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine.h"
#include "os_access.h"
#include "perf_trace.h"

static char warningBuffer[ERROR_BUFFER_SIZE];
static critical_msg_t criticalErrorMessageBuffer;

#if EFI_SIMULATOR || EFI_PROD_CODE
//todo: move into simulator global
#include "memstreams.h"
class ErrorState {
public:
	/**
	 * Class constructors are a great way to have simple initialization sequence
	 */
	ErrorState();
	MemoryStream warningStream;
	MemoryStream firmwareErrorMessageStream;
};

ErrorState::ErrorState() {
	/**
	 * these methods only change RAM state of data structures without any HAL access thus safe in contructor
	 */
	msObjectInit(&warningStream, (uint8_t *) warningBuffer, ERROR_BUFFER_SIZE, 0);
	msObjectInit(&firmwareErrorMessageStream, criticalErrorMessageBuffer, sizeof(criticalErrorMessageBuffer), 0);
}

static ErrorState errorState;

#endif /* EFI_SIMULATOR || EFI_PROD_CODE */

#if EFI_HD44780_LCD
#include "lcd_HD44780.h"
#endif /* EFI_HD44780_LCD */

static LoggingWithStorage logger("error handling");

EXTERN_ENGINE;

#define WARNING_PREFIX "WARNING: "

extern int warningEnabled;
extern bool main_loop_started;

bool hasFirmwareErrorFlag = false;

const char *dbg_panic_file;
int dbg_panic_line;

#if EFI_TUNER_STUDIO && !defined(EFI_NO_CONFIG_WORKING_COPY)
extern persistent_config_s configWorkingCopy;
#endif

char *getFirmwareError(void) {
	return (char*) criticalErrorMessageBuffer;
}

#if EFI_PROD_CODE

extern ioportid_t criticalErrorLedPort;
extern ioportmask_t criticalErrorLedPin;
extern uint8_t criticalErrorLedState;

/**
 * low-level function is used here to reduce stack usage
 */

#define ON_CRITICAL_ERROR() \
		palWritePad(criticalErrorLedPort, criticalErrorLedPin, criticalErrorLedState); \
		turnAllPinsOff(); \
		enginePins.communicationLedPin.setValue(1);
#endif /* EFI_PROD_CODE */

#if EFI_SIMULATOR || EFI_PROD_CODE

void chDbgPanic3(const char *msg, const char * file, int line) {
	if (hasOsPanicError())
		return;
	dbg_panic_file = file;
	dbg_panic_line = line;
#if CH_DBG_SYSTEM_STATE_CHECK
	ch.dbg.panic_msg = msg;
#endif /* CH_DBG_SYSTEM_STATE_CHECK */

#if EFI_PROD_CODE
	ON_CRITICAL_ERROR();
#else
	printf("chDbgPanic3 %s %s%d", msg, file, line);
	exit(-1);
#endif

#if EFI_HD44780_LCD
	lcdShowPanicMessage((char *) msg);
#endif /* EFI_HD44780_LCD */

	if (!main_loop_started) {
		print("%s %s %s:%d\r\n", CRITICAL_PREFIX, msg, file, line);
//		chThdSleepSeconds(1);
		chSysHalt("Main loop did not start");
	}
}

// todo: look into chsnprintf
// todo: move to some util file & reuse for 'firmwareError' method
static void printToStream(MemoryStream *stream, const char *fmt, va_list ap) {
	stream->eos = 0; // reset
	chvprintf((BaseSequentialStream *) stream, fmt, ap);

	// Terminate, but don't write past the end of the buffer
	int terminatorLocation = minI(stream->eos, stream->size - 1);
	stream->buffer[terminatorLocation] = '\0';
}

static void printWarning(const char *fmt, va_list ap) {
	logger.reset(); // todo: is 'reset' really needed here?
	appendMsgPrefix(&logger);

	logger.append(WARNING_PREFIX);

	printToStream(&errorState.warningStream, fmt, ap);

	if (CONFIG(showHumanReadableWarning)) {
#if EFI_TUNER_STUDIO
 #if defined(EFI_NO_CONFIG_WORKING_COPY)
  memcpy(persistentState.persistentConfiguration.warning_message, warningBuffer, sizeof(warningBuffer));
 #else /* defined(EFI_NO_CONFIG_WORKING_COPY) */
  memcpy(configWorkingCopy.warning_message, warningBuffer, sizeof(warningBuffer));
 #endif /* defined(EFI_NO_CONFIG_WORKING_COPY) */
#endif /* EFI_TUNER_STUDIO */
	}

	logger.append(warningBuffer);
	logger.append(DELIMETER);
	scheduleLogging(&logger);
}

#else
WarningCodeState unitTestWarningCodeState;

#endif /* EFI_SIMULATOR || EFI_PROD_CODE */

/**
 * OBD_PCM_Processor_Fault is the general error code for now
 *
 * @returns TRUE in case there were warnings recently
 */
bool warning(obd_code_e code, const char *fmt, ...) {
	if (hasFirmwareErrorFlag)
		return true;

#if EFI_SIMULATOR
	printf("sim_warning %s\r\n", fmt);
#endif /* EFI_SIMULATOR */

#if EFI_SIMULATOR || EFI_PROD_CODE
	engine->engineState.warnings.addWarningCode(code);

	// todo: move this logic into WarningCodeState?
	efitimesec_t now = getTimeNowSeconds();
	if (engine->engineState.warnings.isWarningNow(now, false) || !warningEnabled)
		return true; // we just had another warning, let's not spam
	engine->engineState.warnings.timeOfPreviousWarning = now;

	va_list ap;
	va_start(ap, fmt);
	printWarning(fmt, ap);
	va_end(ap);
#else
	// todo: we need access to 'engine' here so that we can migrate to real 'engine->engineState.warnings'
	unitTestWarningCodeState.addWarningCode(code);
	printf("unit_test_warning: ");
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	printf("\r\n");

#endif /* EFI_SIMULATOR || EFI_PROD_CODE */
	return false;
}

char *getWarningMessage(void) {
	return warningBuffer;
}


#if EFI_CLOCK_LOCKS
uint32_t lastLockTime;
/**
 * Maximum time before requesting lock and releasing lock at the end of critical section
 */
uint32_t maxLockedDuration = 0;

/**
 * this depends on chdebug.h patch
 #if CH_DBG_SYSTEM_STATE_CHECK == TRUE
-#define _dbg_enter_lock() (ch.dbg.lock_cnt = (cnt_t)1)
-#define _dbg_leave_lock() (ch.dbg.lock_cnt = (cnt_t)0)
+#define _dbg_enter_lock() {(ch.dbg.lock_cnt = (cnt_t)1);  ON_LOCK_HOOK;}
+#define _dbg_leave_lock() {ON_UNLOCK_HOOK;(ch.dbg.lock_cnt = (cnt_t)0);}
 #endif
 */
#endif /* EFI_CLOCK_LOCKS */

void onLockHook(void) {
#if ENABLE_PERF_TRACE
	perfEventInstantGlobal(PE::GlobalLock);
#endif /* ENABLE_PERF_TRACE */

#if EFI_CLOCK_LOCKS
	lastLockTime = getTimeNowLowerNt();
#endif /* EFI_CLOCK_LOCKS */
}

void onUnlockHook(void) {
#if EFI_CLOCK_LOCKS
	uint32_t lockedDuration = getTimeNowLowerNt() - lastLockTime;
	if (lockedDuration > maxLockedDuration) {
		maxLockedDuration = lockedDuration;
	}
//	if (lockedDuration > 2800) {
//		// un-comment this if you want a nice stop for a breakpoint
//		maxLockedDuration = lockedDuration + 1;
//	}
#endif /* EFI_CLOCK_LOCKS */

#if ENABLE_PERF_TRACE
	perfEventInstantGlobal(PE::GlobalUnlock);
#endif /* ENABLE_PERF_TRACE */
}

#if EFI_SIMULATOR || EFI_UNIT_TEST
#include <stdexcept>
#endif

void firmwareError(obd_code_e code, const char *fmt, ...) {
#if EFI_PROD_CODE
	if (hasFirmwareErrorFlag)
		return;
	engine->limpManager.fatalError();
	engine->engineState.warnings.addWarningCode(code);
#ifdef EFI_PRINT_ERRORS_AS_WARNINGS
	va_list ap;
	va_start(ap, fmt);
	printWarning(fmt, ap);
	va_end(ap);
#endif
	ON_CRITICAL_ERROR()
	;
	hasFirmwareErrorFlag = true;
	if (indexOf(fmt, '%') == -1) {
		/**
		 * in case of simple error message let's reduce stack usage
		 * because chvprintf might be causing an error
		 */
		strncpy((char*) criticalErrorMessageBuffer, fmt, sizeof(criticalErrorMessageBuffer) - 1);
		criticalErrorMessageBuffer[sizeof(criticalErrorMessageBuffer) - 1] = 0; // just to be sure
	} else {
		// todo: look into chsnprintf once on Chibios 3
		errorState.firmwareErrorMessageStream.eos = 0; // reset
		va_list ap;
		va_start(ap, fmt);
		chvprintf((BaseSequentialStream *) &errorState.firmwareErrorMessageStream, fmt, ap);
		va_end(ap);
		// todo: reuse warning buffer helper method
		errorState.firmwareErrorMessageStream.buffer[errorState.firmwareErrorMessageStream.eos] = 0; // need to terminate explicitly
	}
	int size = strlen((char*)criticalErrorMessageBuffer);
	static char versionBuffer[32];
	chsnprintf(versionBuffer, sizeof(versionBuffer), " %d@%s", getRusEfiVersion(), FIRMWARE_ID);

	if (size + strlen(versionBuffer) < sizeof(criticalErrorMessageBuffer)) {
		strcpy((char*)(criticalErrorMessageBuffer) + size, versionBuffer);
	}

#else

	char errorBuffer[200];

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(errorBuffer, sizeof(errorBuffer), fmt, ap);
	va_end(ap);

	printf("\x1B[31m>>>>>>>>>> firmwareError [%s]\r\n\x1B[0m\r\n", errorBuffer);

#if EFI_SIMULATOR || EFI_UNIT_TEST
	throw std::logic_error(errorBuffer);
#endif /* EFI_SIMULATOR */
#endif
}

