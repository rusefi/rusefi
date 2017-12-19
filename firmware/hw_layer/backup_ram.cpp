/**
 * @file	backup_ram.cpp
 *
 * @date Dec 19, 2017
 */

#include "backup_ram.h"

uint32_t backupRamLoad(backup_ram_e idx) {
	switch (idx) {
	case BACKUP_STEPPER_POS:
		return RTCD1.rtc->BKP0R & 0xffff;
	case BACKUP_IGNITION_SWITCH_COUNTER:
		return (RTCD1.rtc->BKP0R >> 16) & 0xff;
	default:
		//scheduleMsg(logger, "Invalid backup ram idx %d", idx);
		return 0;
	}
}

void backupRamSave(backup_ram_e idx, uint32_t value) {
	switch (idx) {
	case BACKUP_STEPPER_POS:
		RTCD1.rtc->BKP0R = (RTCD1.rtc->BKP0R & ~0x0000ffff) | (value & 0xffff);
		break;
	case BACKUP_IGNITION_SWITCH_COUNTER:
		RTCD1.rtc->BKP0R = (RTCD1.rtc->BKP0R & ~0x00ff0000) | ((value & 0xff) << 16);
		break;
	default:
		//scheduleMsg(logger, "Invalid backup ram idx %d, value 0x08x", idx, value);
		break;
	}
}
