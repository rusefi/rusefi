/**
 * @file error_handling.cpp
 *
 * @date Apr 1, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "os_access.h"

static critical_msg_t warningBuffer;
static critical_msg_t criticalErrorMessageBuffer;

#if EFI_HD44780_LCD
#include "HD44780.h"
#endif /* EFI_HD44780_LCD */

extern int warningEnabled;

bool hasFirmwareErrorFlag = false;

const char *dbg_panic_file;
int dbg_panic_line;

const char* getFirmwareError(void) {
	return criticalErrorMessageBuffer;
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

	firmwareError(OBD_PCM_Processor_Fault, "assert fail %s %s:%d", msg, file, line);

	// Force unlock, since we may be throwing-under-lock
	chSysUnconditionalUnlock();

	// there was a port_disable in chSysHalt, reenable interrupts so USB works
	port_enable();

	// If on the main thread, longjmp back to the init process so we can keep USB alive
	if (chThdGetSelfX()->threadId == 0) {
		void onAssertionFailure();
		onAssertionFailure();
	} else {
		// Not the main thread, simply try to terminate ourselves and let other threads continue living (so the user can diagnose, etc)
		chThdTerminate(chThdGetSelfX());
	}
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
	chvsnprintf(warningBuffer, sizeof(warningBuffer), fmt, ap);
	va_end(ap);

	if (CONFIG(showHumanReadableWarning)) {
#if EFI_TUNER_STUDIO
		memcpy(persistentState.persistentConfiguration.warning_message, warningBuffer, sizeof(warningBuffer));
#endif /* EFI_TUNER_STUDIO */
	}

	efiPrintf("WARNING: %s", warningBuffer);
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

const char* getWarningMessage(void) {
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
	chvsnprintf(warningBuffer, sizeof(warningBuffer), fmt, ap);
	va_end(ap);
#endif
	ON_CRITICAL_ERROR()
	;
	hasFirmwareErrorFlag = true;
	if (indexOf(fmt, '%') == -1) {
		/**
		 * in case of simple error message let's reduce stack usage
		 * chvsnprintf could cause an overflow if we're already low
		 */
		strncpy((char*) criticalErrorMessageBuffer, fmt, sizeof(criticalErrorMessageBuffer) - 1);
		criticalErrorMessageBuffer[sizeof(criticalErrorMessageBuffer) - 1] = 0; // just to be sure
	} else {
		va_list ap;
		va_start(ap, fmt);
		chvsnprintf(criticalErrorMessageBuffer, sizeof(criticalErrorMessageBuffer), fmt, ap);
		va_end(ap);
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

