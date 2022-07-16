/**
 * @file	backup_ram.cpp
 *
 * @date Dec 19, 2017
 */

#include "backup_ram.h"

uint32_t backupRamLoad(backup_ram_e idx) {
#if HAL_USE_RTC
	switch (idx) {
	case BACKUP_STEPPER_POS:
		return RTCD1.rtc->BKP0R & 0xffff;
	case BACKUP_IGNITION_SWITCH_COUNTER:
		return (RTCD1.rtc->BKP0R >> 16) & 0xff;
	case BACKUP_CJ125_CALIBRATION_LAMBDA:
		return RTCD1.rtc->BKP1R & 0xffff;
	case BACKUP_CJ125_CALIBRATION_HEATER:
		return (RTCD1.rtc->BKP1R >> 16) & 0xffff;
// it is assembly code which reads this value
//	case DFU_JUMP_REQUESTED:
//		return RTCD1.rtc->BKP4R;
	default:
		firmwareError(OBD_PCM_Processor_Fault, "Invalid backup ram idx %d", idx);
		return 0;
	}
#else
	return 0;
#endif /* HAL_USE_RTC */
}

void backupRamSave(backup_ram_e idx, uint32_t value) {
#if HAL_USE_RTC
	switch (idx) {
	case BACKUP_STEPPER_POS:
		RTCD1.rtc->BKP0R = (RTCD1.rtc->BKP0R & ~0x0000ffff) | (value & 0xffff);
		break;
	case BACKUP_IGNITION_SWITCH_COUNTER:
		RTCD1.rtc->BKP0R = (RTCD1.rtc->BKP0R & ~0x00ff0000) | ((value & 0xff) << 16);
		break;
	case BACKUP_CJ125_CALIBRATION_LAMBDA:
		RTCD1.rtc->BKP1R = (RTCD1.rtc->BKP1R & ~0x0000ffff) | (value & 0xffff);
		break;
	case BACKUP_CJ125_CALIBRATION_HEATER:
		RTCD1.rtc->BKP1R = (RTCD1.rtc->BKP1R & ~0xffff0000) | ((value & 0xffff) << 16);
		break;
// todo: start using this code
	case DFU_JUMP_REQUESTED:
		RTCD1.rtc->BKP4R = value;
		break;
	default:
		firmwareError(OBD_PCM_Processor_Fault, "Invalid backup ram idx %d, value 0x08x", idx, value);
		break;
	}
#endif /* HAL_USE_RTC */
}

void backupRamFlush(void) {
	// nothing to do here, in STM32 all data is saved instantaneously
}

// STM32 only has 4k bytes of backup SRAM
static_assert(sizeof(BackupSramData) <= 4096);

extern BackupSramData __backup_sram_addr__;

BackupSramData* getBackupSram() {
	return &__backup_sram_addr__;
}
