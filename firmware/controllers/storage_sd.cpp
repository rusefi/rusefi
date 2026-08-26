/**
 * @file    storage_sd.cpp
 * @brief   Storage interface to FatFS
 *
 * @date Jul 16, 2025
 * @author Andrey Gusakov
 */

#include "pch.h"

#include "storage.h"

/* if we use ChibiOS MFS for settings */
#if EFI_STORAGE_SD == TRUE

#if EFI_SUPPORT_FATFS == FALSE
	#error EFI_SUPPORT_FATFS should be enabled for EFI_STORAGE_SD
#endif

#include "ff.h"
#include "mmc_card.h"
#include "mmc_card_util.h"

class SettingStorageSD : public SettingStorageBase {
public:
	bool isReady() override;
	bool isIdSupported(size_t id) override;
	StorageStatus store(size_t id, const uint8_t *ptr, size_t size) override;
	StorageStatus read(size_t id, uint8_t *ptr, size_t size) override;
	StorageStatus format() override;

	SettingStorageSD(FIL *fd) {
		m_fd = fd;
	}

private:
	struct FileNames {
		const char *primary;
		const char *temporary;
		const char *backup;
	};

	const FileNames *getIdFileNames(size_t id);
	StorageStatus readExactFile(const char *fileName, uint8_t *ptr, size_t size);
	bool fileExists(const char *fileName, bool& exists);
	void reportWriteFailure(size_t id);
	FIL *m_fd;
};

const SettingStorageSD::FileNames *SettingStorageSD::getIdFileNames(size_t id) {
	static constexpr FileNames ltftFiles = { "ltft.bin", "ltft.tmp", "ltft.bak" };
	static constexpr FileNames secondTablesFiles = { "second_tables.bin", "second_tables.tmp", "second_tables.bak" };
	static constexpr FileNames luaFiles = { "lua_script.bin", "lua_script.tmp", "lua_script.bak" };

	switch (id) {
	case EFI_LTFT_RECORD_ID:
		return &ltftFiles;
	case EFI_SECOND_TABLES_RECORD_ID:
		return &secondTablesFiles;
	case EFI_LUA_PAGE_RECORD_ID:
		return &luaFiles;
	default:
		return nullptr;
	}
}

bool SettingStorageSD::isReady() {
	return (sdCardGetCurrentMode() == SD_MODE_ECU);
}

bool SettingStorageSD::isIdSupported(size_t id) {
	return (getIdFileNames(id) != nullptr);
}

StorageStatus SettingStorageSD::store(size_t id, const uint8_t *ptr, size_t size) {
	const FileNames *fileNames = getIdFileNames(id);

	if (fileNames == nullptr) {
		return StorageStatus::NotSupported;
	}

	FsGuard guard;
	if (!guard.isLocked()) {
		efiPrintf("SD: write: failed to lock FS");
		return StorageStatus::NotAvailable;

	}

	efiPrintf("SD: Writing storage ID %u %s... %u bytes",
		static_cast<unsigned int>(id), fileNames->primary, static_cast<unsigned int>(size));
	efitick_t startNt = getTimeNowNt();

	// Build a complete replacement without touching the last known-good file.
	FRESULT err = f_open(m_fd, fileNames->temporary, FA_CREATE_ALWAYS | FA_WRITE);
	if (err != FR_OK) {
		printFatFsError("SD: failed to create temporary storage file", err);
		reportWriteFailure(id);
		return StorageStatus::Failed;
	}

	StorageStatus status = StorageStatus::Ok;
	UINT bytesWritten = 0;
	err = f_write(m_fd, ptr, size, &bytesWritten);
	if (err != FR_OK) {
		printFatFsError("SD: failed to write temporary storage file", err);
		status = StorageStatus::Failed;
	}

	if (bytesWritten != size) {
		efiPrintf("SD: failed to write whole file %u != %u",
			static_cast<unsigned int>(bytesWritten), static_cast<unsigned int>(size));
		status = StorageStatus::Failed;
	}

	if (status == StorageStatus::Ok) {
		err = f_sync(m_fd);
		if (err != FR_OK) {
			printFatFsError("SD: failed to sync temporary storage file", err);
			status = StorageStatus::Failed;
		}
	}

	err = f_close(m_fd);
	if (err != FR_OK) {
		printFatFsError("SD: failed to close temporary storage file", err);
		status = StorageStatus::Failed;
	}

	if (status != StorageStatus::Ok) {
		reportWriteFailure(id);
		return status;
	}

	bool primaryExists = false;
	if (!fileExists(fileNames->primary, primaryExists)) {
		reportWriteFailure(id);
		return StorageStatus::Failed;
	}

	bool primaryMoved = false;
	if (primaryExists) {
		err = f_unlink(fileNames->backup);
		if ((err != FR_OK) && (err != FR_NO_FILE)) {
			printFatFsError("SD: failed to remove old storage backup", err);
			reportWriteFailure(id);
			return StorageStatus::Failed;
		}

		err = f_rename(fileNames->primary, fileNames->backup);
		if (err != FR_OK) {
			printFatFsError("SD: failed to rotate storage backup", err);
			reportWriteFailure(id);
			return StorageStatus::Failed;
		}
		primaryMoved = true;
	}

	err = f_rename(fileNames->temporary, fileNames->primary);
	if (err != FR_OK) {
		printFatFsError("SD: failed to promote temporary storage file", err);
		if (primaryMoved) {
			FRESULT restoreErr = f_rename(fileNames->backup, fileNames->primary);
			if (restoreErr != FR_OK) {
				printFatFsError("SD: failed to restore storage backup", restoreErr);
			}
		}
		reportWriteFailure(id);
		return StorageStatus::Failed;
	}

	efitick_t endNt = getTimeNowNt();
	int elapsed_Ms = US2MS(NT2US(endNt - startNt));

	efiPrintf("SD: Write done after %d mS", elapsed_Ms);

	return status;
}

bool SettingStorageSD::fileExists(const char *fileName, bool& exists) {
	FRESULT err = f_open(m_fd, fileName, FA_READ);
	if (err == FR_NO_FILE) {
		exists = false;
		return true;
	}

	if (err != FR_OK) {
		printFatFsError("SD: failed to inspect storage file", err);
		return false;
	}

	exists = true;
	err = f_close(m_fd);
	if (err != FR_OK) {
		printFatFsError("SD: failed to close inspected storage file", err);
		return false;
	}

	return true;
}

void SettingStorageSD::reportWriteFailure(size_t id) {
	warning(ObdCode::CUSTOM_ERR_SD_MOUNT_FAILED, "SD: storage write failed for ID %u",
		static_cast<unsigned int>(id));
}

StorageStatus SettingStorageSD::readExactFile(const char *fileName, uint8_t *ptr, size_t size) {
	FRESULT err = f_open(m_fd, fileName, FA_READ);
	if (err == FR_NO_FILE) {
		return StorageStatus::NotFound;
	}

	if (err != FR_OK) {
		printFatFsError("SD: failed to open storage file", err);
		return StorageStatus::Failed;
	}

	StorageStatus status = StorageStatus::Ok;
	FSIZE_t fileSize = f_size(m_fd);
	if (fileSize != size) {
		efiPrintf("SD: unexpected storage file size %u != %u",
			static_cast<unsigned int>(fileSize), static_cast<unsigned int>(size));
		status = StorageStatus::Failed;
	}

	UINT bytesRead = 0;
	if (status == StorageStatus::Ok) {
		err = f_read(m_fd, ptr, size, &bytesRead);
		if (err != FR_OK) {
			printFatFsError("SD: failed to read storage file", err);
			status = StorageStatus::Failed;
		}

		if (bytesRead != size) {
			efiPrintf("SD: failed to read whole file %u != %u",
				static_cast<unsigned int>(bytesRead), static_cast<unsigned int>(size));
			status = StorageStatus::Failed;
		}
	}

	err = f_close(m_fd);
	if (err != FR_OK) {
		printFatFsError("SD: failed to close storage file", err);
		status = StorageStatus::Failed;
	}

	return status;
}

StorageStatus SettingStorageSD::read(size_t id, uint8_t *ptr, size_t size) {
	const FileNames *fileNames = getIdFileNames(id);

	if (fileNames == nullptr) {
		return StorageStatus::NotSupported;
	}

	FsGuard guard;
	if (!guard.isLocked()) {
		efiPrintf("SD: read: failed to lock FS");
		return StorageStatus::NotAvailable;

	}

	efiPrintf("SD: Reading storage ID %u %s ... %u bytes",
		static_cast<unsigned int>(id), fileNames->primary, static_cast<unsigned int>(size));

	StorageStatus primaryStatus = readExactFile(fileNames->primary, ptr, size);
	if (primaryStatus == StorageStatus::Ok) {
		efiPrintf("SD: Reading done");
		return StorageStatus::Ok;
	}

	efiPrintf("SD: primary storage file unavailable, trying backup");
	StorageStatus backupStatus = readExactFile(fileNames->backup, ptr, size);
	if (backupStatus == StorageStatus::Ok) {
		efiPrintf("SD: Reading backup done");
		return StorageStatus::Ok;
	}

	if (primaryStatus != StorageStatus::NotFound) {
		return primaryStatus;
	}

	return backupStatus;
}

StorageStatus SettingStorageSD::format() {
	/* TODO: actually format sd? */

	return StorageStatus::NotSupported;
}

static NO_CACHE FIL fd;

static SettingStorageSD storageSD(&fd);

bool initStorageSD() {
	return storageRegisterStorage(STORAGE_SD_CARD, &storageSD);
}

bool deinitStorageSD() {
	return storageUnregisterStorage(STORAGE_SD_CARD);
}

#endif //EFI_STORAGE_SD
