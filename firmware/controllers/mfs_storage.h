/**
 * @file    mfs_storage.h
 * @brief
 *
 * @date Jan 3, 2025
 * @author Andrey Gusakov
 */

#pragma once

#include "flash_main.h"

// IDs used as MFS record ids and internal RusEFI ids
// Convert to enum/class
#define EFI_SETTINGS_RECORD_ID		1
#define EFI_LTFT_RECORD_ID			2

int mfsStorageWrite(int id, const uint8_t *ptr, size_t size);
FlashState mfsStorageRead(int id, uint8_t *ptr, size_t size);
