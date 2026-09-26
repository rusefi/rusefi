#pragma once

#include "storage.h"

namespace storage_detail {

inline StorageStatus readFirstSuccessful(
	SettingStorageBase *const *storages,
	size_t storageCount,
	StorageItemId id,
	uint8_t *ptr,
	size_t size) {
	StorageStatus status = StorageStatus::NotSupported;

	for (size_t i = storageCount; i > 0; i--) {
		SettingStorageBase *storage = storages[i - 1];
		if (storage == nullptr) {
			continue;
		}

		if ((!storage->isReady()) || (!storage->isIdSupported(id))) {
			continue;
		}

		status = storage->read(id, ptr, size);
		if (status == StorageStatus::Ok) {
			return StorageStatus::Ok;
		}
	}

	return status;
}

} // namespace storage_detail
