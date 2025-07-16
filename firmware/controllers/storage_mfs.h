/**
 * @file    storage_mfs.h
 * @brief
 *
 * @date Jan 3, 2025
 * @author Andrey Gusakov
 */

#pragma once

#include "flash_main.h"

StorageStatus mfsStorageWrite(int id, const uint8_t *ptr, size_t size);
StorageStatus mfsStorageRead(int id, uint8_t *ptr, size_t size);
StorageStatus mfsStorageFormat();

void initStorageMfs();
