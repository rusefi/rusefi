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
	const char *getIdFileName(size_t id);
	const char *getIdTemporaryFileName(size_t id);
	const char *getIdBackupFileName(size_t id);
	StorageStatus readExactFile(const char *fileName, uint8_t *ptr, size_t size);
	FIL *m_fd;
};

const char *SettingStorageSD::getIdFileName(size_t id) {
	switch (id) {
	case EFI_LTFT_RECORD_ID:
		return "ltft.bin";
	case EFI_SECOND_TABLES_RECORD_ID:
		return "second_tables.bin";
	case EFI_LUA_PAGE_RECORD_ID:
		return "lua_script.bin";
	default:
		return nullptr;
	}
}

const char *SettingStorageSD::getIdTemporaryFileName(size_t id) {
	switch (id) {
	case EFI_LTFT_RECORD_ID:
		return "ltft.tmp";
	case EFI_SECOND_TABLES_RECORD_ID:
		return "second_tables.tmp";
	case EFI_LUA_PAGE_RECORD_ID:
		return "lua_script.tmp";
	default:
		return nullptr;
	}
}

const char *SettingStorageSD::getIdBackupFileName(size_t id) {
	switch (id) {
	case EFI_LTFT_RECORD_ID:
		return "ltft.bak";
	case EFI_SECOND_TABLES_RECORD_ID:
		return "second_tables.bak";
	case EFI_LUA_PAGE_RECORD_ID:
		return "lua_script.bak";
	default:
		return nullptr;
	}
}

bool SettingStorageSD::isReady() {
	return (sdCardGetCurrentMode() == SD_MODE_ECU);
}

bool SettingStorageSD::isIdSupported(size_t id) {
	return (getIdFileName(id) != nullptr);
}

StorageStatus SettingStorageSD::store(size_t id, const uint8_t *ptr, size_t size) {
	const char *fileName = getIdFileName(id);
	const char *temporaryFileName = getIdTemporaryFileName(id);
	const char *backupFileName = getIdBackupFileName(id);

	if ((fileName == nullptr) || (temporaryFileName == nullptr) || (backupFileName == nullptr)) {
		return StorageStatus::NotSupported;
	}

	FsGuard guard;
	if (!guard.isLocked()) {
		efiPrintf("SD: write: failed to lock FS");
		return StorageStatus::NotAvailable;

	}

	efiPrintf("SD: Writing storage ID %d  %s... %d bytes", id, fileName, size);
	efitick_t startNt = getTimeNowNt();

	// Never truncate the last known-good file. Write and flush a sibling first,
	// then rotate the old primary to a backup before promoting the new file.
	FRESULT err = f_open(m_fd, temporaryFileName, FA_CREATE_ALWAYS | FA_WRITE);
	if (err != FR_OK) {
		printFatFsError("SD: failed to create temporary file", err);
		return StorageStatus::Failed;
	}

	StorageStatus status = StorageStatus::Ok;
	size_t bytesWritten = 0;
	err = f_write(m_fd, ptr, size, &bytesWritten);
	if (err != FR_OK) {
		printFatFsError("SD: failed to write", err);
		status = StorageStatus::Failed;
	}

	if (bytesWritten != size) {
		efiPrintf("SD: failed to write whole file %d != %d", bytesWritten, size);
		status = StorageStatus::Failed;
	}

	if (status == StorageStatus::Ok) {
		err = f_sync(m_fd);
		if (err != FR_OK) {
			printFatFsError("SD: failed to sync temporary file", err);
			status = StorageStatus::Failed;
		}
	}

	err = f_close(m_fd);
	if (err != FR_OK) {
		printFatFsError("SD: failed to close temporary file", err);
		status = StorageStatus::Failed;
	}

	if (status != StorageStatus::Ok) {
		(void)f_unlink(temporaryFileName);
		return status;
	}

	err = f_unlink(backupFileName);
	if ((err != FR_OK) && (err != FR_NO_FILE)) {
		printFatFsError("SD: failed to remove previous backup", err);
		(void)f_unlink(temporaryFileName);
		return StorageStatus::Failed;
	}

	bool primaryMoved = false;
	err = f_rename(fileName, backupFileName);
	if (err == FR_OK) {
		primaryMoved = true;
	} else if (err != FR_NO_FILE) {
		printFatFsError("SD: failed to preserve previous file", err);
		(void)f_unlink(temporaryFileName);
		return StorageStatus::Failed;
	}

	err = f_rename(temporaryFileName, fileName);
	if (err != FR_OK) {
		printFatFsError("SD: failed to promote temporary file", err);
		if (primaryMoved) {
			(void)f_rename(backupFileName, fileName);
		}
		(void)f_unlink(temporaryFileName);
		return StorageStatus::Failed;
	}

	efitick_t endNt = getTimeNowNt();
	int elapsed_Ms = US2MS(NT2US(endNt - startNt));

	efiPrintf("SD: Write done after %d mS", elapsed_Ms);

	return status;
}

StorageStatus SettingStorageSD::readExactFile(const char *fileName, uint8_t *ptr, size_t size) {
	FRESULT err = f_open(m_fd, fileName, FA_READ);
	if (err != FR_OK) {
		return (err == FR_NO_FILE) ? StorageStatus::NotFound : StorageStatus::Failed;
	}

	const FSIZE_t fileSize = f_size(m_fd);
	if (fileSize != size) {
		efiPrintf("SD: invalid storage file size %d != %d for %s", (size_t)fileSize, size, fileName);
		(void)f_close(m_fd);
		return StorageStatus::Failed;
	}

	size_t bytesRead = 0;
	err = f_read(m_fd, ptr, size, &bytesRead);
	StorageStatus status = StorageStatus::Ok;
	if (err != FR_OK) {
		printFatFsError("SD: failed to read", err);
		status = StorageStatus::Failed;
	} else if (bytesRead != size) {
		efiPrintf("SD: failed to read whole file %d != %d", bytesRead, size);
		status = StorageStatus::Failed;
	}

	err = f_close(m_fd);
	if (err != FR_OK) {
		printFatFsError("SD: failed to close file", err);
		status = StorageStatus::Failed;
	}

	return status;
}

StorageStatus SettingStorageSD::read(size_t id, uint8_t *ptr, size_t size) {
	const char *fileName = getIdFileName(id);
	const char *backupFileName = getIdBackupFileName(id);

	if ((fileName == nullptr) || (backupFileName == nullptr)) {
		return StorageStatus::NotSupported;
	}

	FsGuard guard;
	if (!guard.isLocked()) {
		efiPrintf("SD: read: failed to lock FS");
		return StorageStatus::NotAvailable;

	}

	efiPrintf("SD: Reading storage ID %d %s ... %d bytes", id, fileName, size);

	StorageStatus status = readExactFile(fileName, ptr, size);
	if (status != StorageStatus::Ok) {
		efiPrintf("SD: primary storage file invalid, trying %s", backupFileName);
		status = readExactFile(backupFileName, ptr, size);
		if (status == StorageStatus::Ok) {
			efiPrintf("SD: recovered storage ID %d from backup", id);
		}
	}

	efiPrintf("SD: Reading done");

	return status;
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
