/**
 * @file	backup_ram.cpp
 *
 * @date Dec 19, 2017
 */

#include "backup_ram.h"

static uint32_t bkupram[static_cast<size_t>(backup_ram_e::BACKUP_RAM_NUM)];

uint32_t backupRamLoad(backup_ram_e idx) {
	return bkupram[static_cast<size_t>(idx)];
}

void backupRamSave(backup_ram_e idx, uint32_t value) {
	bkupram[static_cast<size_t>(idx)] = value;
}
