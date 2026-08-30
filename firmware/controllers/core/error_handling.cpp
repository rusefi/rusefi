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
#include "rusefi/efistring.h"
#include "board_overrides.h"
#include "flash_main.h"

#if EFI_USE_OPENBLT
/* communication with OpenBLT that is plain C, not to modify external file */
extern "C" {
	#include "openblt/shared_params.h"
};
#endif

using namespace rusefi::stringutil;

/**
 * Executes the BKPT instruction that causes the debugger to stop.
 */
#define bkpt() __asm volatile("BKPT #0\n")

static critical_msg_t warningBuffer;
static critical_msg_t criticalErrorMessageBuffer;
static critical_msg_t configErrorMessageBuffer; // recoverable configuration error, non-critical

bool hasCriticalFirmwareErrorFlag = false;
/**
 * not critical error: TS would display text error message until clearConfigErrorMessage() is invoked
 */
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

#if EFI_PROD_CODE

#if EFI_BACKUP_SRAM
static backupErrorState lastBootError;
static uint32_t bootCount = 0;
#endif // EFI_BACKUP_SRAM

// Set by errorHandlerInit() when the previous reset was caused by a watchdog.
// The critical error itself is raised later, see errorHandlerRaiseWatchdogResetError()
static bool watchdogResetDetected = false;
// true once errorHandlerRaiseWatchdogResetError() actually latched the critical error
static bool watchdogResetErrorRaised = false;

#if EFI_USE_OPENBLT
static void setOpenBltSwCounter(int counter) {
	if (counter < 0 || counter > 254) {
		efiPrintf("OpenBLT SW reset counter must be 0..254");
		return;
	}

	SharedParamsInit();
	if (SharedParamsWriteByIndex(2, static_cast<uint8_t>(counter))) {
		efiPrintf("OpenBLT SW reset counter set to %d", counter);
	} else {
		efiPrintf("Failed to set OpenBLT SW reset counter");
	}
}
#endif // EFI_USE_OPENBLT

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
		(lastBootError.Cookie != ErrorCookie::ChibiOsPanic) &&
		(lastBootError.Cookie != ErrorCookie::Reboot)) {
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
	// if reset by watchdog, remember it: the fatal error is raised by errorHandlerRaiseWatchdogResetError()
	// once hardware is initialized, so that the SD card report can still be written
	watchdogResetDetected = (cause == Reset_Cause_IWatchdog) || (cause == Reset_Cause_WWatchdog);
#endif // EFI_BACKUP_SRAM

	// see https://wiki.rusefi.com/Resilience
	addConsoleAction("chibi_fault", [](){ chDbgCheck(0); } );
	addConsoleAction("soft_fault", [](){ firmwareError(ObdCode::RUNTIME_CRITICAL_TEST_ERROR, "firmwareError: %d", getRusEfiVersion()); });
	addConsoleAction("hard_fault", [](){ causeHardFault(); } );
#if EFI_USE_OPENBLT
	addConsoleActionI("set_openblt_sw_counter", setOpenBltSwCounter);
#endif // EFI_USE_OPENBLT
}

bool errorHandlerIsStartFromError() {
#if EFI_BACKUP_SRAM
	return (lastBootError.Cookie != ErrorCookie::None);
#else
	return 0;
#endif
}

/**
 * Raises the critical error for a watchdog reset detected by errorHandlerInit().
 *
 * This is deliberately NOT done inside errorHandlerInit(): that runs before any hardware is
 * initialized, and a critical error latched that early makes initHardware() return without
 * setting up SPI/pins and makes initMmc() refuse the SD card - so the report file this very
 * error message promises was never written (#10174). The MCU has just been reset, so the
 * hardware is in a known-good state; the purpose of this error is only to keep the engine
 * from running until a human has looked at the ECU. Raising it right after initHardware()
 * keeps that policy (nothing engine-related has started yet) while letting the SD thread
 * initialize the card and write the report - see initMmc().
 *
 * Must be called after initHardware() and before initMmcCard().
 */
void errorHandlerRaiseWatchdogResetError() {
	if (!watchdogResetDetected) {
		return;
	}
	if (hasFirmwareError()) {
		// some other critical error was already latched during hardware init: that one wins,
		// and hardware really may be in an unexpected state, so keep the SD card guard active
		return;
	}
	firmwareError(ObdCode::OBD_PCM_Processor_Fault, "Watchdog Reset detected! Check SD card for report file.");
	watchdogResetErrorRaised = true;
}

bool errorHandlerIsWatchdogResetError() {
	return watchdogResetErrorRaised;
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
	case ErrorCookie::Reboot:
		return "Deliberate reboot";
	}

	return "Unknown";
}

const char *rebootReasonToName(RebootReason reason)
{
	switch (reason) {
	case RebootReason::Unknown:
		return "unspecified";
	case RebootReason::Command:
		return "command (TS/console/CAN)";
	case RebootReason::DfuJump:
		return "jump to DFU bootloader";
	case RebootReason::OpenBltJump:
		return "jump to OpenBLT bootloader";
	}

	return "unknown";
}

#define printResetReason()											\
	PRINT("Reset Cause: %s", getMCUResetCause(getMCUResetCause()))

#if EFI_USE_OPENBLT
#define printResetCounters()										\
	do {															\
		uint8_t wd_counter = 0;										\
		uint8_t sw_counter = 0;										\
		SharedParamsReadByIndex(1, &wd_counter);					\
		SharedParamsReadByIndex(2, &sw_counter);					\
		PRINT("WD resets: %u", (unsigned int)wd_counter);			\
		PRINT("SW resets: %u", (unsigned int)sw_counter);			\
	} while (0)
#else
#define printResetCounters()										\
	do {} while(0)
#endif


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
	case ErrorCookie::Reboot:										\
		{															\
			PRINT("Deliberate reboot: %s",							\
				rebootReasonToName((RebootReason)err->RebootReason));	\
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
	printResetCounters();

#if EFI_BACKUP_SRAM
	backupErrorState *err = &lastBootError;
	ErrorCookie cookie = err->Cookie;

	printErrorState();
	if (cookie != ErrorCookie::None) {
		printErrorStack();
	}
#endif // EFI_BACKUP_SRAM
	#undef PRINT
}

#if EFI_FILE_LOGGING
#include "ff.h"

#define FAIL_REPORT_PREFIX	"fail"

void onBoardWriteErrorFile(FIL *) {
  // placeholder, remove in Nov 2026
}

std::optional<setup_custom_board_write_error_file_type> custom_board_onBoardWriteErrorFile;

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
	case ErrorCookie::Reboot:
		return "Reboot";
	}

	return "unknown";
}

// report name for a watchdog reset that left no cookie in backup RAM (CPU died without running any of our fault paths)
#define WATCHDOG_REPORT_NAME "Watchdog"

static const char *errorHandlerGetReportName(ErrorCookie cookie, Reset_Cause_t cause) {
	if ((cookie == ErrorCookie::None) &&
		((cause == Reset_Cause_IWatchdog) || (cause == Reset_Cause_WWatchdog))) {
		return WATCHDOG_REPORT_NAME;
	}
	return errorHandlerGetErrorName(cookie);
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
			bootCount, FAIL_REPORT_PREFIX, errorHandlerGetReportName(cookie, cause));
#else
		sprintf(fileName, "last_%s_%s.txt",
			FAIL_REPORT_PREFIX, errorHandlerGetReportName(cookie, cause));
#endif

		FRESULT ret = f_open(fd, fileName, FA_CREATE_ALWAYS | FA_WRITE);
		if (ret == FR_OK) {
			//this is file print
			#define PRINT(format, ...) f_printf(fd, format "\r\n", __VA_ARGS__)
			printResetReason();
			printResetCounters();
#if EFI_BACKUP_SRAM
			printErrorState();
			if (cookie != ErrorCookie::None) {
				printErrorStack();
			}
#endif // EFI_BACKUP_SRAM
			f_printf(fd, "rusEFI v%d@%u", getRusEfiVersion(), /*do we have a working way to print 64 bit values?!*/(int)SIGNATURE_HASH);
			// additional board-specific data
			call_board_override(custom_board_onBoardWriteErrorFile, fd);
			// todo: figure out what else would be useful
			f_close(fd);
			enginePins.warningLedPin.setValue(1);
		}
	}
}

static int errorHandlerIsReportExist(const char *reportName) {
	bool exist = false;
	FRESULT fr;     /* Return value */
	DIR dj;         /* Directory object */
	FILINFO fno;    /* File information */
	TCHAR pattern[32];

	sprintf(pattern, "*%s*", reportName);

	fr = f_findfirst(&dj, &fno, "", pattern);
	exist = ((fr == FR_OK) && (fno.fname[0]));
	f_closedir(&dj);

	return exist;
}

int errorHandlerCheckReportFiles() {
	hasReportFile =
		(errorHandlerIsReportExist(errorHandlerGetErrorName(ErrorCookie::FirmwareError)) > 0) ||
		(errorHandlerIsReportExist(errorHandlerGetErrorName(ErrorCookie::HardFault)) > 0) ||
		(errorHandlerIsReportExist(errorHandlerGetErrorName(ErrorCookie::ChibiOsPanic)) > 0) ||
		(errorHandlerIsReportExist(WATCHDOG_REPORT_NAME) > 0);

	return hasReportFile;
}

static void errorHandlerDeleteTypedReport(const char *reportName) {
	bool failed = false;
	FRESULT fr;     /* Return value */
	DIR dj;         /* Directory object */
	FILINFO fno;    /* File information */
	TCHAR pattern[32];

	sprintf(pattern, "*%s*", reportName);

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
	errorHandlerDeleteTypedReport(errorHandlerGetErrorName(ErrorCookie::FirmwareError));
	errorHandlerDeleteTypedReport(errorHandlerGetErrorName(ErrorCookie::HardFault));
	errorHandlerDeleteTypedReport(errorHandlerGetErrorName(ErrorCookie::ChibiOsPanic));
	errorHandlerDeleteTypedReport(WATCHDOG_REPORT_NAME);

	// update
	errorHandlerCheckReportFiles();
}

#endif // EFI_FILE_LOGGING

void errorHandlerResetCounters() {
#if EFI_USE_OPENBLT
	SharedParamsWriteByIndex(1, 0);
	SharedParamsWriteByIndex(2, 0);
#endif
}

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

/* Crash marker in the RTC backup-domain registers (BKPxR). These survive a
 * soft reset (and the bootloader jump) as long as VDD keeps the backup domain
 * alive - the RTC clock is proof: it keeps its time across our crashes. So a
 * crash that kills the console link still reports itself on the next boot. */
#define CRASH_MARKER_MAGIC_FAULT	0xC0FFEE01
#define CRASH_MARKER_MAGIC_ASSERT	0xC0FFEE02

/* Debug aid populated by the ChibiOS fork's __dbg_check_lock_from_isr()
 * (SV#6 path): the return address of the chSysLockFromISR() caller and the
 * dbg counters at the halt. Persisted into BKP9R/BKP10R by
 * writeAssertCrashMarker and reported on the next boot - see chdebug.c. */
uint32_t rusEfiLastSv6Caller = 0;
uint32_t rusEfiLastSv6State = 0;

/* Debug aid populated by the ChibiOS fork's chSysGetStatusAndLockX() on every
 * X-class lock: the return address of the locker and the IPSR (active
 * exception number) at that moment. The last pair written before an SV#6
 * identifies the crashing context. Persisted into BKP11R/BKP12R. */
uint32_t rusEfiLastXLockCaller = 0;
uint32_t rusEfiLastXLockState = 0;

static void crashMarkerEnableWrite() {
	PWR->CR |= PWR_CR_DBP;       // disable backup domain write protection
	RCC->BDCR |= RCC_BDCR_RTCEN; // BKP registers live on the RTC clock domain
}

static void writeCrashMarker(uint32_t magic, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e) {
	crashMarkerEnableWrite();
	RTC->BKP0R = magic;
	RTC->BKP1R = a;
	RTC->BKP2R = b;
	RTC->BKP3R = c;
	RTC->BKP4R = d;
	RTC->BKP5R = e;
}

/* Pack a string into 32-bit little-endian words (16 bytes max), zero padded. */
static void packStringIntoWords(const char* s, uint32_t* w0, uint32_t* w1, uint32_t* w2, uint32_t* w3) {
	uint32_t words[4] = { 0, 0, 0, 0 };
	if (s != nullptr) {
		for (size_t i = 0; i < sizeof(words) && s[i] != '\0'; i++) {
			words[i / 4] |= (uint32_t)(uint8_t)s[i] << (8 * (i % 4));
		}
	}
	*w0 = words[0];
	*w1 = words[1];
	*w2 = words[2];
	*w3 = words[3];
}

static void writeAssertCrashMarker(int line, const char* msg, const char* file) {
	/* The line number is of limited use: every ChibiOS chSysHalt/assert goes
	 * through the CH_CFG_SYSTEM_HALT_HOOK which expands __LINE__ at its
	 * invocation site in chsys.c (always 220). The panic MESSAGE is the real
	 * identifier ("SV#4", "not ready", "not owner", ...), so pack it into
	 * the backup registers: BKP2R..BKP5R = msg (16 bytes), BKP6R..BKP8R =
	 * file (12 bytes). */
	uint32_t m0, m1, m2, m3, f0, f1, f2;
	packStringIntoWords(msg, &m0, &m1, &m2, &m3);
	packStringIntoWords(file, &f0, &f1, &f2, &m3 /* dummy */);
	crashMarkerEnableWrite();
	RTC->BKP0R = CRASH_MARKER_MAGIC_ASSERT;
	RTC->BKP1R = (uint32_t)line;
	RTC->BKP2R = m0;
	RTC->BKP3R = m1;
	RTC->BKP4R = m2;
	RTC->BKP5R = m3;
	RTC->BKP6R = f0;
	RTC->BKP7R = f1;
	RTC->BKP8R = f2;
#if EFI_PROD_CODE
	/* SV#6 call-site debug aid (see chdebug.c __dbg_check_lock_from_isr). */
	RTC->BKP9R = rusEfiLastSv6Caller;
	RTC->BKP10R = rusEfiLastSv6State;
	/* X-lock context debug aid (see chsys.c chSysGetStatusAndLockX). */
	RTC->BKP11R = rusEfiLastXLockCaller;
	RTC->BKP12R = rusEfiLastXLockState;
#endif
}

/* Give the console thread a chance to flush the FAULT/assert line before the
 * reboot. Plain busy-wait: no scheduler/lock involvement (the faulting thread
 * may hold the system lock). ~0.5 s at 288 MHz. */
static void crashDelayForConsoleFlush() {
	for (volatile uint32_t i = 0; i < 50000000; i++) {
		__asm__ volatile("");
	}
}

/* The crash report is printed exactly ONCE, right after initRtc()
 * (printPreviousCrashIfAny, from the boot banner). The 5-second re-print
 * was removed on user request (2026-08-30): it spammed the console with
 * crash-marker/Reset-Cause lines - on the bench with its ~5 s debug resets
 * the repeat cadence made every boot look like a crash storm. The report
 * stays visible in the console log from the boot banner itself. */
static uint32_t crashMarkerMagic;
static uint32_t crashMarkerArgs[5]; // BKP1R..BKP5R

static void unpackWordsIntoString(char* out, size_t outSize, uint32_t w0, uint32_t w1, uint32_t w2, uint32_t w3) {
	uint32_t words[4] = { w0, w1, w2, w3 };
	for (size_t i = 0; i + 1 < outSize && i < sizeof(words); i++) {
		char c = (char)((words[i / 4] >> (8 * (i % 4))) & 0xFF);
		if (c == '\0') {
			out[i] = '\0';
			return;
		}
		out[i] = c;
	}
	out[outSize - 1] = '\0';
}

static void printCrashReportLines() {
	efiPrintf("*** crash marker: BKP0R=0x%08x BKP1R=0x%08x",
		(unsigned)crashMarkerMagic, (unsigned)crashMarkerArgs[0]);
	if (crashMarkerMagic == CRASH_MARKER_MAGIC_FAULT) {
		efiPrintf("*** PREVIOUS CRASH: fault type=%u pc=0x%08x lr=0x%08x faultAddr=0x%08x cfsr=0x%08x",
			(unsigned)crashMarkerArgs[0], (unsigned)crashMarkerArgs[1], (unsigned)crashMarkerArgs[2],
			(unsigned)crashMarkerArgs[3], (unsigned)crashMarkerArgs[4]);
	} else if (crashMarkerMagic == CRASH_MARKER_MAGIC_ASSERT) {
		/* BKP2R..BKP5R hold the panic message, BKP6R..BKP8R the file (see
		 * writeAssertCrashMarker). Note the line is usually 220 (chsys.c halt
		 * hook) regardless of the real failing check - the message identifies
		 * the actual assert. */
		char msg[17] = "";
		char file[13] = "";
		unpackWordsIntoString(msg, sizeof(msg), crashMarkerArgs[1], crashMarkerArgs[2], crashMarkerArgs[3], crashMarkerArgs[4]);
		unpackWordsIntoString(file, sizeof(file), RTC->BKP6R, RTC->BKP7R, RTC->BKP8R, 0);
		efiPrintf("*** PREVIOUS CRASH: assert (line=%u, msg='%s', file='%s')",
			(unsigned)crashMarkerArgs[0], msg, file);
		/* SV#6 call-site debug aid: nonzero only when the halt came from
		 * __dbg_check_lock_from_isr (persisted across the reboot). */
		uint32_t sv6Caller = RTC->BKP9R;
		uint32_t sv6State = RTC->BKP10R;
		if (sv6Caller != 0) {
			efiPrintf("*** PREVIOUS CRASH: sv6 caller=0x%08x lock=%u isr=%u",
				(unsigned)sv6Caller, (unsigned)(sv6State >> 16), (unsigned)(sv6State & 0xffff));
		}
		/* X-lock context debug aid: last chSysGetStatusAndLockX caller + IPSR. */
		uint32_t xlockCaller = RTC->BKP11R;
		uint32_t xlockIpsr = RTC->BKP12R;
		if (xlockCaller != 0) {
			efiPrintf("*** PREVIOUS CRASH: xlock caller=0x%08x ipsr=%u",
				(unsigned)xlockCaller, (unsigned)xlockIpsr);
		}
	}
	efiPrintf("Reset Cause: %s", getMCUResetCause(getMCUResetCause()));
}

void printPreviousCrashIfAny() {
	crashMarkerEnableWrite();
	crashMarkerMagic = RTC->BKP0R;
	crashMarkerArgs[0] = RTC->BKP1R;
	crashMarkerArgs[1] = RTC->BKP2R;
	crashMarkerArgs[2] = RTC->BKP3R;
	crashMarkerArgs[3] = RTC->BKP4R;
	crashMarkerArgs[4] = RTC->BKP5R;
	RTC->BKP0R = 0; // consumed
	printCrashReportLines();
}

void logHardFault(uint32_t type, uintptr_t faultAddress, void* sp, port_extctx* ctx, uint32_t csfr) {
    // todo: reuse hasCriticalFirmwareErrorFlag? something?
    isInHardFaultHandler = true;

	/* Write the fault crash marker BEFORE any printf: efiPrintf takes the
	 * X-class system lock, whose chSysLockFromISR branch trips SV#6 in the
	 * fault-handler context (IPSR != 0 but no OSAL_IRQ_PROLOGUE, isr_cnt 0),
	 * and the secondary assert halt would overwrite this marker with
	 * C0FFEE02, hiding the real fault (m74_9, 2026-08-20). */
	writeCrashMarker(CRASH_MARKER_MAGIC_FAULT, type, ctx->pc, ctx->lr_thd,
		(uint32_t)faultAddress, csfr);

	/* Best-effort console print: skipped by efiPrintfInternal while the fault
	 * handler is active, so the marker above is the authoritative record. */
	efiPrintf("FAULT type=%u pc=0x%08x lr=0x%08x faultAddr=0x%08x cfsr=0x%08x",
		(unsigned)type, (unsigned)ctx->pc, (unsigned)ctx->lr_thd,
		(unsigned)faultAddress, (unsigned)csfr);
	crashDelayForConsoleFlush();
	// Evidence first!
#if EFI_BACKUP_SRAM
	auto bkpram = getBackupSram();
	auto err = &bkpram->err;
	if (err->Cookie == ErrorCookie::None) {
		// Stamp everything that does not require touching memory through 'sp' and set the
		// cookie BEFORE the first dereference of 'sp': a corrupted PSP, or (F7 guard pages) a
		// PSP sitting inside a no-access page, faults again right here -> lockup -> watchdog
		// reset. What was already written is then still reported on the next boot.
		err->FaultType = type;
		err->FaultAddress = faultAddress;
		err->Csfr = csfr;
		err->sp = (uint32_t)sp;
		memset(&err->FaultCtx, 0, sizeof(port_extctx));
		err->Cookie = ErrorCookie::HardFault;
		// exception frame: main registers including PC and LR
		memcpy(&err->FaultCtx, sp, sizeof(port_extctx));
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
	/* Inside the fault handler any lock-taking print (criticalError ->
	 * efiPrintf -> X-class lock) re-trips SV#6 via chSysLockFromISR and the
	 * assert crash marker would overwrite the fault marker written by
	 * logHardFault. The fault marker is already persisted - reboot now. */
	if (isInHardFaultHandler) {
		crashDelayForConsoleFlush();
		rebootNow();
	}
#endif // EFI_PROD_CODE
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
	writeAssertCrashMarker(line, msg, file);

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

		// Give the console thread a moment to flush the assert fail line, then reboot.
		crashDelayForConsoleFlush();

		// Reboot!
		rebootNow();
	}

#endif // EFI_PROD_CODE
}
#endif /* EFI_SIMULATOR || EFI_PROD_CODE */

#if EFI_UNIT_TEST
bool silentUnitTest = true; // performance optimization since console output is pricey
#endif // EFI_UNIT_TEST

/**
 * @returns TRUE in case there were warnings recently
 */
bool warningVA(ObdCode code, bool reportToTs, const char *fmt, va_list args) {
	if (hasCriticalFirmwareErrorFlag) {
		return true;
	}

	bool known = engine->engineState.warnings.isWarningNow(code);

	if (known) {
		// if known - just reset timer
		engine->engineState.warnings.addWarningCode(code);
#if EFI_SIMULATOR || EFI_PROD_CODE
		// we just had this same warning, let's not spam
		return true;
#endif
	}

	// print Pxxxx (for standard OBD) or Cxxxx (for custom) prefix
	size_t size = snprintf(warningBuffer, sizeof(warningBuffer), "%s%04d: ",
		ObdCodeIsCustom(code) ? "C" : "P", (int) code);

	chvsnprintf(warningBuffer + size, sizeof(warningBuffer) - size, fmt, args);

	engine->engineState.warnings.addWarningCode(code, reportToTs ? warningBuffer : nullptr);
#if EFI_SIMULATOR || EFI_PROD_CODE
	efiPrintf("WARNING: %s", warningBuffer);
#else
	if (!silentUnitTest) {
		printf("WARNING: %s\n", warningBuffer);
	}
#endif /* EFI_SIMULATOR || EFI_PROD_CODE */

#if MODULE_DTC_MANAGER
	// Create FreezeFrame
	DtcTriggerEvent(warningBuffer, code);
#endif

	return false;
}

bool warning(ObdCode code, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	bool ret = warningVA(code, false, fmt, args);
	va_end(args);
	return ret;
}

bool warningTsReport(ObdCode code, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	bool ret = warningVA(code, true, fmt, args);
	va_end(args);
	return ret;
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

// bumped on every configError() so refreshConfigErrorState() can tell whether the
// current message is its own or was raised by an unregistered legacy caller
static uint32_t configErrorSeq = 0;

void configError(const char *fmt, ...) {
		va_list ap;
		va_start(ap, fmt);
		chvsnprintf(configErrorMessageBuffer, sizeof(configErrorMessageBuffer), fmt, ap);
		va_end(ap);
		hasConfigErrorFlag = true;
		configErrorSeq++;
}

const char* getConfigErrorMessage() {
	return configErrorMessageBuffer;
}

std::optional<setup_custom_bool_type> custom_board_updateConfigError;

static bool refreshRaisedConfigError = false;
static uint32_t refreshRaisedSeq = 0;

void refreshConfigErrorState() {
	// core producers go here, worst first, before the board hook
	bool active = checkSettingsWriteFailure();
	if (!active) {
		active = get_board_override_result(custom_board_updateConfigError, false);
	}
	if (active) {
		// the producer has just called configError() with its message
		refreshRaisedConfigError = true;
		refreshRaisedSeq = configErrorSeq;
		return;
	}
	if (refreshRaisedConfigError) {
		refreshRaisedConfigError = false;
		// a seq mismatch means a legacy caller raised after our condition fired - leave theirs latched
		if (refreshRaisedSeq == configErrorSeq) {
			clearConfigErrorMessage();
		}
	}
}

#if EFI_UNIT_TEST
void resetConfigErrorStateForUnitTest() {
	custom_board_updateConfigError = {};
	refreshRaisedConfigError = false;
	refreshRaisedSeq = 0;
	configErrorSeq = 0;
	trackSettingsWriteResult(true);
	clearConfigErrorMessage();
}
#endif

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
  UNUSED(code);

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
