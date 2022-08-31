/**
 * @file error_handling.cpp
 *
 * @date Apr 1, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "os_access.h"
#include "backup_ram.h"

static critical_msg_t warningBuffer;
static critical_msg_t criticalErrorMessageBuffer;

#if EFI_HD44780_LCD
#include "HD44780.h"
#endif /* EFI_HD44780_LCD */

extern int warningEnabled;

bool hasFirmwareErrorFlag = false;

const char *dbg_panic_file;
int dbg_panic_line;

const char* getCriticalErrorMessage(void) {
	return criticalErrorMessageBuffer;
}

#if EFI_PROD_CODE
void checkLastBootError() {
	auto sramState = getBackupSram();
	
	switch (sramState->Cookie) {
	case ErrorCookie::FirmwareError:
		efiPrintf("Last boot had firmware error: %s", sramState->ErrorString);
		break;
	case ErrorCookie::HardFault: {
		efiPrintf("Last boot had hard fault type: %x addr: %x CSFR: %x", sramState->FaultType, sramState->FaultAddress, sramState->Csfr);

		// Print out the context as a sequence of uintptr
		uintptr_t* data = reinterpret_cast<uintptr_t*>(&sramState->FaultCtx);
		for (size_t i = 0; i < sizeof(port_extctx) / sizeof(uintptr_t); i++) {
			efiPrintf("Fault ctx %d: %x", i, data[i]);
		}

		break;
	}
	default:
		// No cookie stored or invalid cookie (ie, backup RAM contains random garbage)
		break;
	}

	// Reset cookie so we don't print it again.
	sramState->Cookie = ErrorCookie::None;

	if (sramState->BootCountCookie != 0xdeadbeef) {
		sramState->BootCountCookie = 0xdeadbeef;
		sramState->BootCount = 0;
	}

	efiPrintf("Power cycle count: %d", sramState->BootCount);
	sramState->BootCount++;
}

void logHardFault(uint32_t type, uintptr_t faultAddress, port_extctx* ctx, uint32_t csfr) {
	auto sramState = getBackupSram();
	sramState->Cookie = ErrorCookie::HardFault;
	sramState->FaultType = type;
	sramState->FaultAddress = faultAddress;
	sramState->Csfr = csfr;
	memcpy(&sramState->FaultCtx, ctx, sizeof(port_extctx));
}

extern ioportid_t criticalErrorLedPort;
extern ioportmask_t criticalErrorLedPin;
extern uint8_t criticalErrorLedState;
#endif /* EFI_PROD_CODE */

#if EFI_SIMULATOR || EFI_PROD_CODE

void chDbgPanic3(const char *msg, const char * file, int line) {
#if EFI_PROD_CODE
	// Attempt to break in to the debugger, if attached
	__asm volatile("BKPT #0\n");
#endif

	if (hasOsPanicError())
		return;
	dbg_panic_file = file;
	dbg_panic_line = line;
#if CH_DBG_SYSTEM_STATE_CHECK
	ch.dbg.panic_msg = msg;
#endif /* CH_DBG_SYSTEM_STATE_CHECK */

#if !EFI_PROD_CODE
	printf("chDbgPanic3 %s %s%d", msg, file, line);
	exit(-1);
#else // EFI_PROD_CODE

#if EFI_HD44780_LCD
	lcdShowPanicMessage((char *) msg);
#endif /* EFI_HD44780_LCD */

	firmwareError(OBD_PCM_Processor_Fault, "assert fail %s %s:%d", msg, file, line);

	// If on the main thread, longjmp back to the init process so we can keep USB alive
	if (chThdGetSelfX()->threadId == 0) {
		// Force unlock, since we may be throwing-under-lock
		chSysUnconditionalUnlock();

		// there was a port_disable in chSysHalt, reenable interrupts so USB works
		port_enable();

		__NO_RETURN void onAssertionFailure();
		onAssertionFailure();
	} else {
		// Not the main thread.
		// All hope is now lost.

		// Reboot!
		NVIC_SystemReset();
	}

#endif // EFI_PROD_CODE
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
	// we just had this same warning, let's not spam
	if (engine->engineState.warnings.isWarningNow(code) || !warningEnabled) {
		return true;
	}

	engine->engineState.warnings.addWarningCode(code);

	va_list ap;
	va_start(ap, fmt);
	chvsnprintf(warningBuffer, sizeof(warningBuffer), fmt, ap);
	va_end(ap);

	if (engineConfiguration->showHumanReadableWarning) {
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
	palWritePad(criticalErrorLedPort, criticalErrorLedPin, criticalErrorLedState);
	turnAllPinsOff();
	enginePins.communicationLedPin.setValue(1);

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

	int errorMessageSize = strlen((char*)criticalErrorMessageBuffer);
	static char versionBuffer[32];
	chsnprintf(versionBuffer, sizeof(versionBuffer), " %d@%s", getRusEfiVersion(), FIRMWARE_ID);

	if (errorMessageSize + strlen(versionBuffer) < sizeof(criticalErrorMessageBuffer)) {
		strcpy((char*)(criticalErrorMessageBuffer) + errorMessageSize, versionBuffer);
	}

	auto sramState = getBackupSram();
	strncpy(sramState->ErrorString, criticalErrorMessageBuffer, efi::size(sramState->ErrorString));
	sramState->Cookie = ErrorCookie::FirmwareError;
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
