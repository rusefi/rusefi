/**
 * @file	backup_ram.cpp
 *
 * @date Dec 19, 2017
 */

#include "backup_ram.h"

// STM32 only has 4k bytes of backup SRAM
static_assert(sizeof(BackupSramData) <= 4096);

extern BackupSramData __backup_sram_addr__;

BackupSramData* getBackupSram() {
	auto data = &__backup_sram_addr__;

	if (data->Cookie != BackupSramData::ExpectedCookie ||
		data->Version != FLASH_DATA_VERSION) {
		// Cookie was invalid or version changed, reset to defaults
		*data = {};
	}

	return data;
}
