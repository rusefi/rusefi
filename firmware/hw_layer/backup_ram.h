/**
 * @file	backup_ram.h
 * @brief	Non-volatile backup-RAM registers support
 *
 * @date Dec 19, 2017
 */

#pragma once

#include "efi_gpio.h"

#include "error_handling.h"

enum class backup_ram_e {
	/**
	 * IAC Stepper motor position, 16-bit (stored in BKP0R 0..15)
	 * Used in stepper.cpp
	 */
	StepperPosition,
	/**
	 * Ignition switch counter, 8-bit (stored in BKP0R 16..23)
	 * The counter stores the number of times the ignition switch is turned on. Used for prime injection pulse.
	 * We need a protection against 'fake' ignition switch on and off (i.e. no engine started), to avoid repeated prime pulses.
	 * So we check and update the ignition switch counter in non-volatile backup-RAM.
	 * See startPrimeInjectionPulse() in controllers/trigger/main_trigger_callback.cpp
	 */
	IgnCounter,

	/* The number of stored backup variables */
	BACKUP_RAM_NUM,
};

#define LAST_BACKUP_RAM_ENUM backup_ram_e::BACKUP_RAM_NUM


// load data from backup-power RTC registers (non-volatile memory)
uint32_t backupRamLoad(backup_ram_e idx);
// use backup-power RTC registers (non-volatile memory) to store the data
void backupRamSave(backup_ram_e idx, uint32_t value);
// make sure that all changes are saved before we shutdown the MCU
void backupRamFlush(void);

#if EFI_PROD_CODE
struct BackupSramData {
	uint32_t BootCount;
	uint32_t BootCountCookie;

	backupErrorState err;
};

BackupSramData* getBackupSram();
#endif // EFI_PROD_CODE
