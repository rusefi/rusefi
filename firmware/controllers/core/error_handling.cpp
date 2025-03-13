/**
 * @file error_handling.cpp
 *
 * @date Apr 1, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "rusefi/efistringutil.h"
#include "os_util.h"
#include "backup_ram.h"
#include "error_handling_led.h"
#include "log_hard_fault.h"
#include "rusefi/critical_error.h"

/**
 * Executes the BKPT instruction that causes the debugger to stop.
 */
#define bkpt() __asm volatile("BKPT #0\n")

// see strncpy man page
// this implementation helps avoiding following gcc error/warning:
// error: 'strncpy' output may be truncated copying xxx bytes from a string of length xxx

char *strlncpy(char *dest, const char *src, size_t size)
{
	size_t i;

	for (i = 0; (i < (size - 1)) && (src[i] != '\0'); i++)
		dest[i] = src[i];
	for ( ; i < size; i++)
		dest[i] = '\0';

	return dest;
}

static critical_msg_t warningBuffer;
static critical_msg_t criticalErrorMessageBuffer;
static critical_msg_t configErrorMessageBuffer; // recoverable configuration error, non-critical

bool hasCriticalFirmwareErrorFlag = false;
static bool hasConfigErrorFlag = false;
static bool hasReportFile = false;

// todo: revisit very questionable code!
// todo: reuse hasCriticalFirmwareErrorFlag? something?
bool isInHardFaultHandler = false;

const char *dbg_panic_file;
int dbg_panic_line;

// todo: need vararg version of 'firmwareError' to make this method vararg?
void efiCriticalError(const char *message) {
  criticalError(message);
}

const char* getCriticalErrorMessage() {
	return criticalErrorMessageBuffer;
}

bool hasConfigError() {
	return hasConfigErrorFlag;
}

void clearConfigErrorMessage() {
	hasConfigErrorFlag = false;
}

bool hasErrorReportFile() {
	return hasReportFile;
}

const char* getConfigErrorMessageBuffer() {
	return configErrorMessageBuffer;
}

#if EFI_PROD_CODE

#if EFI_BACKUP_SRAM
static backupErrorState lastBootError;
static uint32_t bootCount = 0;
#endif // EFI_BACKUP_SRAM

void errorHandlerInit() {
#if EFI_BACKUP_SRAM
	/* copy error state from backup RAM and clear it in backup RAM.
	 * so few users can access previous error state and we should not care about who sohuld clear backup ram. */
	auto sramState = getBackupSram();
	memcpy(&lastBootError, &sramState->err, sizeof(backupErrorState));
	memset(&sramState->err, 0x00, sizeof(sramState->err));
	// Reset cookie so we don't report it again.
	sramState->err.Cookie = ErrorCookie::None;

	// Cookie can be some random value at first power on
	// reset to None to avoid generating 'Unknown' fail report
	if ((lastBootError.Cookie != ErrorCookie::FirmwareError) &&
		(lastBootError.Cookie != ErrorCookie::HardFault) &&
		(lastBootError.Cookie != ErrorCookie::ChibiOsPanic)) {
		lastBootError.Cookie = ErrorCookie::None;
	}

	//bootcount
	if (sramState->BootCountCookie != 0xdeadbeef) {
		sramState->BootCountCookie = 0xdeadbeef;
		sramState->BootCount = 0;
	}
	// save current bootcounter
	bootCount = sramState->BootCount;

	sramState->BootCount++;

	if (engineConfiguration->rethrowHardFault) {
		backupErrorState *err = &lastBootError;
		if (err->Cookie == ErrorCookie::HardFault) {
		    criticalError("Last boot had hard fault type: %lx addr: %lx CSFR: %lx",
		    	err->FaultType, err->FaultAddress, err->Csfr);
		}
	}

	Reset_Cause_t cause = getMCUResetCause();
	// if reset by watchdog, signal a fatal error
	if ((cause == Reset_Cause_IWatchdog) || (cause == Reset_Cause_WWatchdog)) {
		firmwareError(ObdCode::OBD_PCM_Processor_Fault, "Watchdog Reset detected! Check SD card for report file.");
	}
#endif // EFI_PROD_CODE

	// see https://github.com/rusefi/rusefi/wiki/Resilience
	addConsoleAction("chibi_fault", [](){ chDbgCheck(0); } );
	addConsoleAction("soft_fault", [](){ firmwareError(ObdCode::RUNTIME_CRITICAL_TEST_ERROR, "firmwareError: %d", getRusEfiVersion()); });
	addConsoleAction("hard_fault", [](){ causeHardFault(); } );
}

bool errorHandlerIsStartFromError() {
#if EFI_BACKUP_SRAM
	return (lastBootError.Cookie != ErrorCookie::None);
#else
	return 0;
#endif
}

const char *errorCookieToName(ErrorCookie cookie)
{
	switch (cookie) {
	case ErrorCookie::None:
		return "No error";
	case ErrorCookie::FirmwareError:
		return "firmware";
	case ErrorCookie::HardFault:
		return "HardFault";
	case ErrorCookie::ChibiOsPanic:
		return "ChibiOS panic";
	}

	return "Unknown";
}

#define printResetReason()											\
	PRINT("Reset Cause: %s", getMCUResetCause(getMCUResetCause()))

#define printErrorState()											\
do {																\
	PRINT("Power cycle count: %lu", bootCount);						\
																	\
	if (cookie == ErrorCookie::None) {								\
		break;														\
	}																\
																	\
	PRINT("Last error type %s", errorCookieToName(err->Cookie));	\
																	\
	switch (cookie) {												\
	case ErrorCookie::FirmwareError:								\
		{															\
			PRINT("%s", err->msg);									\
		}															\
		break;														\
	case ErrorCookie::HardFault:									\
		{															\
			PRINT("type: 0x%08lx addr: 0x%08lx CSFR: 0x%08lx",		\
				err->FaultType, err->FaultAddress, err->Csfr);		\
																	\
			auto ctx = &err->FaultCtx;								\
			PRINT("r0  0x%08lx", ctx->r0);							\
			PRINT("r1  0x%08lx", ctx->r1);							\
			PRINT("r2  0x%08lx", ctx->r2);							\
			PRINT("r3  0x%08lx", ctx->r3);							\
			PRINT("r12 0x%08lx", ctx->r12);							\
			PRINT("lr (thread)  0x%08lx", ctx->lr_thd);				\
			PRINT("pc  0x%08lx", ctx->pc);							\
			PRINT("xpsr  0x%08lx", ctx->xpsr);						\
																	\
			/* FPU registers - not very useful for debug */			\
			if (0) {												\
				/* Print rest the context as a sequence of uintptr */	\
				uintptr_t* data = reinterpret_cast<uintptr_t*>(&err->FaultCtx);	\
				for (size_t i = 8; i < sizeof(port_extctx) / sizeof(uintptr_t); i++) {	\
					PRINT("Fault ctx %d: 0x%08x", i, data[i]);		\
				}													\
			}														\
		}															\
		break;														\
	case ErrorCookie::ChibiOsPanic:									\
		{															\
			PRINT("msg %s", err->msg);								\
			PRINT("file %s", err->file);							\
			PRINT("line %d", err->line);							\
		}															\
		break;														\
	default:														\
		/* No cookie stored or invalid cookie (ie, backup RAM contains random garbage) */	\
		break;														\
	}																\
} while(0)

#define printErrorStack()											\
do {																\
	PRINT("SP 0x%08lx", err->sp);									\
	for (size_t i = 0; i < ERROR_STACK_DEPTH; i++) {				\
	  uint32_t cur = err->stack[i]; \
	  if (cur != 0) { \
		  PRINT(" 0x%08lx: 0x%08lx", err->sp - i * 4, cur);	\
		} \
	}																\
} while(0)

// TODO: reuse this code for writing crash report file
void errorHandlerShowBootReasonAndErrors() {
	//this is console print
	#define PRINT(...) efiPrintf(__VA_ARGS__)

	printResetReason();

#if EFI_BACKUP_SRAM
	backupErrorState *err = &lastBootError;
	ErrorCookie cookie = err->Cookie;

	printErrorState();
	printErrorStack();
#endif // EFI_BACKUP_SRAM
	#undef PRINT
}

#if EFI_FILE_LOGGING
#include "ff.h"

#define FAIL_REPORT_PREFIX	"fail"

PUBLIC_API_WEAK void onBoardWriteErrorFile(FIL *) {
}

static const char *errorHandlerGetErrorName(ErrorCookie cookie)
{
	switch (cookie) {
	case ErrorCookie::None:
		return "none";
	case ErrorCookie::FirmwareError:
		return "FWerror";
	case ErrorCookie::HardFault:
		return "HardFault";
	case ErrorCookie::ChibiOsPanic:
		return "OSpanic";
	}

	return "unknown";
}

bool needErrorReportFile = false;

void errorHandlerWriteReportFile(FIL *fd) {
#if EFI_BACKUP_SRAM
	backupErrorState *err = &lastBootError;
	ErrorCookie cookie = err->Cookie;
#else
	ErrorCookie cookie = ErrorCookie::None;
#endif

	if (cookie != ErrorCookie::None) {
		needErrorReportFile = true;
	}

	auto cause = getMCUResetCause();
	// TODO: should we also report Unknown?
	if ((cause != Reset_Cause_NRST_Pin) && (cause != Reset_Cause_BOR) &&
		(cause != Reset_Cause_POR) && (cause != Reset_Cause_Unknown)) {
		// not an expected cause
		needErrorReportFile = true;
	}

	if (needErrorReportFile) {
		char fileName[_MAX_FILLER + 20];
		memset(fd, 0, sizeof(FIL));						// clear the memory
		//TODO: use date + time for file name?
#if EFI_BACKUP_SRAM
		sprintf(fileName, "%05ld_%s_%s.txt",
			bootCount, FAIL_REPORT_PREFIX, errorHandlerGetErrorName(cookie));
#else
		sprintf(fileName, "last_%s_%s.txt",
			FAIL_REPORT_PREFIX, errorHandlerGetErrorName(cookie));
#endif

		FRESULT ret = f_open(fd, fileName, FA_CREATE_ALWAYS | FA_WRITE);
		if (ret == FR_OK) {
			//this is file print
			#define PRINT(format, ...) f_printf(fd, format "\r\n", __VA_ARGS__)
			printResetReason();
#if EFI_BACKUP_SRAM
			printErrorState();
			printErrorStack();
#endif // EFI_BACKUP_SRAM
      f_printf(fd, "rusEFI v%d@%u", getRusEfiVersion(), /*do we have a working way to print 64 bit values?!*/(int)SIGNATURE_HASH);
			// additional board-specific data
			onBoardWriteErrorFile(fd);
			// todo: figure out what else would be useful
			f_close(fd);
			enginePins.warningLedPin.setValue(1);
		}
	}
}

static int errorHandlerIsReportExist(ErrorCookie cookie) {
	bool exist = false;
	FRESULT fr;     /* Return value */
	DIR dj;         /* Directory object */
	FILINFO fno;    /* File information */
	TCHAR pattern[32];

	sprintf(pattern, "*%s*", errorHandlerGetErrorName(cookie));

	fr = f_findfirst(&dj, &fno, "", pattern);
	exist = ((fr == FR_OK) && (fno.fname[0]));
	f_closedir(&dj);

	return exist;
}

int errorHandlerCheckReportFiles() {
	hasReportFile =
		(errorHandlerIsReportExist(ErrorCookie::FirmwareError) > 0) ||
		(errorHandlerIsReportExist(ErrorCookie::HardFault) > 0) ||
		(errorHandlerIsReportExist(ErrorCookie::ChibiOsPanic) > 0);

	return hasReportFile;
}

static void errorHandlerDeleteTypedReport(ErrorCookie cookie) {
	bool failed = false;
	FRESULT fr;     /* Return value */
	DIR dj;         /* Directory object */
	FILINFO fno;    /* File information */
	TCHAR pattern[32];

	sprintf(pattern, "*%s*", errorHandlerGetErrorName(cookie));

	do {
		fr = f_findfirst(&dj, &fno, "", pattern);
		f_closedir(&dj);

		if ((fr == FR_OK) && (fno.fname[0])) {
			efiPrintf("deleting %s", fno.fname);
			FRESULT ret = f_unlink(fno.fname);
			if (ret != FR_OK) {
				efiPrintf("Faield to delete %s: %d", fno.fname, ret);
				failed = true;
			} else {
				efiPrintf("%s removed", fno.fname);
			}
		}
	} while ((!failed) && (fr == FR_OK) && (fno.fname[0]));
}

void errorHandlerDeleteReports() {
	errorHandlerDeleteTypedReport(ErrorCookie::FirmwareError);
	errorHandlerDeleteTypedReport(ErrorCookie::HardFault);
	errorHandlerDeleteTypedReport(ErrorCookie::ChibiOsPanic);

	// update
	errorHandlerCheckReportFiles();
}

#endif

#if EFI_BACKUP_SRAM
static void errorHandlerSaveStack(backupErrorState *err, uint32_t *sp)
{
	err->sp = (uint32_t)sp;
	for (size_t i = 0; i < ERROR_STACK_DEPTH; i++) {
		// avoid optimizatio and usage of __builtin_memcpy
		// to avoid "error: '__builtin_memcpy' reading 128 bytes from a region of size 4"
		err->stack[i] = *(volatile uint32_t *)sp;
		sp++;
	}
}
#endif // EFI_BACKUP_SRAM

void logHardFault(uint32_t type, uintptr_t faultAddress, void* sp, port_extctx* ctx, uint32_t csfr) {
    // todo: reuse hasCriticalFirmwareErrorFlag? something?
    isInHardFaultHandler = true;
	// Evidence first!
#if EFI_BACKUP_SRAM
	auto bkpram = getBackupSram();
	auto err = &bkpram->err;
	if (err->Cookie == ErrorCookie::None) {
		err->FaultType = type;
		err->FaultAddress = faultAddress;
		err->Csfr = csfr;
		memcpy(&err->FaultCtx, ctx, sizeof(port_extctx));
		err->Cookie = ErrorCookie::HardFault;
		// copy stack last as it can be corrupted and cause another exeption
		errorHandlerSaveStack(err, (uint32_t *)sp);
	}
#endif // EFI_BACKUP_SRAM
	// criticalShutdown() shutdown can cause cascaded fault.
	// So we first save some valuable evidence and only after try to gracefully shutdown HW
	criticalShutdown();
}

#endif /* EFI_PROD_CODE */

#if EFI_SIMULATOR || EFI_PROD_CODE

void chDbgPanic3(const char *msg, const char * file, int line) {
#if EFI_PROD_CODE
#if EFI_BACKUP_SRAM
	// following is allocated on stack
	// add some marker
	uint32_t tmp = 0xfffffa11;
#endif
	// Attempt to break in to the debugger, if attached
	if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
	{
		bkpt();
	}
#endif // EFI_PROD_CODE

#if EFI_BACKUP_SRAM
    auto bkpram = getBackupSram();
	auto err = &bkpram->err;
	if (err->Cookie == ErrorCookie::None) {
		strlncpy(err->file, file, efi::size(err->file));
		err->line = line;
		strlncpy(err->msg, msg, efi::size(err->msg));
		err->Cookie = ErrorCookie::ChibiOsPanic;
		// copy stack last as it can be corrupted and cause another exeption
		uint32_t *sp = &tmp;
		errorHandlerSaveStack(err, sp);
	}
#endif // EFI_BACKUP_SRAM

	if (hasOsPanicError())
		return;
	dbg_panic_file = file;
	dbg_panic_line = line;
#if CH_DBG_SYSTEM_STATE_CHECK
	ch0.dbg.panic_msg = msg;
#endif /* CH_DBG_SYSTEM_STATE_CHECK */

#if !EFI_PROD_CODE
	printf("chDbgPanic3 %s %s%d", msg, file, line);
	exit(-1);
#else // EFI_PROD_CODE

	criticalError("assert fail %s %s:%d", msg, file, line);

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
#endif /* EFI_SIMULATOR || EFI_PROD_CODE */

/**
 * ObdCode::OBD_PCM_Processor_Fault is the general error code for now
 *
 * @returns TRUE in case there were warnings recently
 */
bool warning(ObdCode code, const char *fmt, ...) {
	if (hasCriticalFirmwareErrorFlag) {
		return true;
	}

	bool known = engine->engineState.warnings.isWarningNow(code);

	// if known - just reset timer
	engine->engineState.warnings.addWarningCode(code);

#if EFI_SIMULATOR || EFI_PROD_CODE
	// we just had this same warning, let's not spam
	if (known) {
		return true;
	}

	// print Pxxxx (for standard OBD) or Cxxxx (for custom) prefix
	size_t size = snprintf(warningBuffer, sizeof(warningBuffer), "%s%04d: ",
		code < ObdCode::CUSTOM_NAN_ENGINE_LOAD ? "P" : "C", (int) code);

	va_list ap;
	va_start(ap, fmt);
	chvsnprintf(warningBuffer + size, sizeof(warningBuffer) - size, fmt, ap);
	va_end(ap);

	efiPrintf("WARNING: %s", warningBuffer);
#else
	printf("unit_test_warning: ");
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	printf("\r\n");

#endif /* EFI_SIMULATOR || EFI_PROD_CODE */
	return false;
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

void configError(const char *fmt, ...) {
		va_list ap;
		va_start(ap, fmt);
		chvsnprintf(configErrorMessageBuffer, sizeof(configErrorMessageBuffer), fmt, ap);
		va_end(ap);
		hasConfigErrorFlag = true;
}

const char* getConfigErrorMessage() {
	return configErrorMessageBuffer;
}

static void firmwareErrorV(ObdCode code, const char *fmt, va_list ap) {
#if EFI_PROD_CODE
#if EFI_BACKUP_SRAM
	// following is allocated on stack
	// add some marker
	uint32_t tmp = 0xfaaaaa11;
#endif
	if (hasCriticalFirmwareErrorFlag)
		return;
	hasCriticalFirmwareErrorFlag = true;

	// construct error message
	if (indexOf(fmt, '%') == -1) {
		/**
		 * in case of simple error message let's reduce stack usage
		 * chvsnprintf could cause an overflow if we're already low
		 */
		strlncpy((char*) criticalErrorMessageBuffer, fmt, sizeof(criticalErrorMessageBuffer));
		criticalErrorMessageBuffer[sizeof(criticalErrorMessageBuffer) - 1] = 0; // just to be sure
	} else {
		chvsnprintf(criticalErrorMessageBuffer, sizeof(criticalErrorMessageBuffer), fmt, ap);
	}

	int errorMessageSize = strlen((char*)criticalErrorMessageBuffer);
	static char versionBuffer[32];
	chsnprintf(versionBuffer, sizeof(versionBuffer), " %d@%s", getRusEfiVersion(), FIRMWARE_ID);

	if (errorMessageSize + strlen(versionBuffer) < sizeof(criticalErrorMessageBuffer)) {
		strcpy((char*)(criticalErrorMessageBuffer) + errorMessageSize, versionBuffer);
	}

	// Evidence first!
#if EFI_BACKUP_SRAM
	auto bkpram = getBackupSram();
	auto err = &bkpram->err;
	if (err->Cookie == ErrorCookie::None) {
		strlncpy(err->msg, criticalErrorMessageBuffer, sizeof(err->msg));
		err->Cookie = ErrorCookie::FirmwareError;
		// copy stack last as it can be corrupted and cause another exeption
		uint32_t *sp = &tmp;
		errorHandlerSaveStack(err, sp);
	}
#endif // EFI_BACKUP_SRAM

#if EFI_ENGINE_CONTROL
	getLimpManager()->fatalError();
#endif // EFI_ENGINE_CONTROL
	engine->engineState.warnings.addWarningCode(code);
	// criticalShutdown() shutdown can cause cascaded fault.
	// So we first save some valuable evidence and only after try to gracefully shutdown HW
	criticalShutdown();
	enginePins.communicationLedPin.setValue(1, /*force*/true);
#else // EFI_PROD_CODE

	// large buffer on stack is risky we better use normal memory
	static char errorBuffer[200];

	vsnprintf(errorBuffer, sizeof(errorBuffer), fmt, ap);

	printf("\x1B[31m>>>>>>>>>> firmwareError [%s]\r\n\x1B[0m\r\n", errorBuffer);

	throw std::logic_error(errorBuffer);
#endif // EFI_PROD_CODE
}

void firmwareError(ObdCode code, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	firmwareErrorV(code, fmt, ap);
	va_end(ap);
}

// This is critical error for plain C use.
// From plain C file we cannot use somethings like this:
// #define criticalError(...) firmwareError(ObdCode::OBD_PCM_Processor_Fault, __VA_ARGS__)
// because of ObdCode::
// Also we can not write some wrapper that will pass variable length argument list to another variable length argument list function
void criticalErrorC(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	firmwareErrorV(ObdCode::OBD_PCM_Processor_Fault, fmt, ap);
	va_end(ap);
}
