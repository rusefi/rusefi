/**
 * @file	backup_ram.cpp
 * @brief	Non-volatile backup-RAM registers support
 *
 * @date Dec 19, 2017
 */

#ifndef BACKUP_RAM_H_
#define BACKUP_RAM_H_

#include "main.h"
#include "efiGpio.h"

typedef enum {
	/**
	 * IAC Stepper motor position, 16-bit (stored in BKP0R 0..15)
	 * Used in stepper.cpp
	 */
	BACKUP_STEPPER_POS,
	/**
	 * Ignition switch counter, 8-bit (stored in BKP0R 16..23)
	 * The counter stores the number of times the ignition switch is turned on. Used for prime injection pulse.
	 * We need a protection against 'fake' ignition switch on and off (i.e. no engine started), to avoid repeated prime pulses.
	 * So we check and update the ignition switch counter in non-volatile backup-RAM.
	 * See startPrimeInjectionPulse() in controllers/trigger/main_trigger_callback.cpp
	 */
	BACKUP_IGNITION_SWITCH_COUNTER,
	/**
	 * CJ125 Calibration data (16bit)
	 */
	BACKUP_CJ125_CALIBRATION_LAMBDA,
	/**
	 * CJ125 Calibration data (16bit)
	 */
	BACKUP_CJ125_CALIBRATION_HEATER,
} backup_ram_e;


// load data from backup-power RTC registers (non-volatile memory)
uint32_t backupRamLoad(backup_ram_e idx);
// use backup-power RTC registers (non-volatile memory) to store the data
void backupRamSave(backup_ram_e idx, uint32_t value);

#endif /* BACKUP_RAM_H_ */
