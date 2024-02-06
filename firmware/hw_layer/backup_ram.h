/**
 * @file	backup_ram.h
 * @brief	Non-volatile backup-RAM registers support
 *
 * @date Dec 19, 2017
 */

#pragma once

#include "efi_gpio.h"

#include "error_handling.h"

// These use very specific values to avoid interpreting random garbage memory as a real value
enum class ErrorCookie : uint32_t {
	None = 0,
	FirmwareError = 0xcafebabe,
	HardFault = 0xdeadbeef,
};

#if EFI_PROD_CODE
struct BackupSramData {
	static const uint32_t ExpectedCookie = 0xDEADBEEF;
	uint32_t Cookie = ExpectedCookie;
	uint32_t Version = FLASH_DATA_VERSION;

	// Error handling/recovery/reporting information
	struct {
		ErrorCookie Cookie;

		critical_msg_t ErrorString;
		port_extctx FaultCtx;
		uint32_t FaultType;
		uint32_t FaultAddress;
		uint32_t Csfr;

		uint32_t BootCount;
		uint32_t BootCountCookie;
	} Err;

	/**
	 * IAC Stepper motor position
	 * Used in stepper.cpp
	 */
	int StepperPosition = 0;

	/**
	 * Ignition switch counter, 8-bit (stored in BKP0R 16..23)
	 * The counter stores the number of times the ignition switch is turned on. Used for prime injection pulse.
	 * We need a protection against 'fake' ignition switch on and off (i.e. no engine started), to avoid repeated prime pulses.
	 * So we check and update the ignition switch counter in non-volatile backup-RAM.
	 * See startPrimeInjectionPulse() in controllers/trigger/main_trigger_callback.cpp
	 */
	uint16_t IgnCounter = 0;
};

BackupSramData* getBackupSram();
#endif // EFI_PROD_CODE
