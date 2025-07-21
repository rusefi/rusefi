/**
 * @file    storage_mfs.cpp
 * @brief   Storage interface to ChibiOS MFS driver
 *
 * @date Jan 4, 2025
 * @author Andrey Gusakov
 */

#include "pch.h"

#include "storage_mfs.h"
#include "flash_main.h"

/* if we use ChibiOS MFS for settings */
#if EFI_STORAGE_MFS == TRUE

#include "hal_mfs.h"

class SettingStorageMFS : public SettingStorageBase {
public:
	bool isReady() override;
	bool isIdSupported(size_t id) override;
	StorageStatus store(size_t id, const uint8_t *ptr, size_t size) override;
	StorageStatus read(size_t id, uint8_t *ptr, size_t size) override;
	StorageStatus format() override;

	SettingStorageMFS(MFSDriver *drv) {
		m_drv = drv;
	}

	bool m_ready = false;

private:
	MFSDriver *m_drv;
};

bool SettingStorageMFS::isReady(){
	return m_ready;
}

bool SettingStorageMFS::isIdSupported(size_t id) {
#if (EFI_STORAGE_INT_FLASH == TRUE)
	// If internal flash storage is enabled - settings are stored in there
	if ((id == EFI_SETTINGS_RECORD_ID) ||
		(id == EFI_SETTINGS_BACKUP_RECORD_ID)) {
		return false;
	}
#endif // EFI_STORAGE_INT_FLASH
	if ((id >= 1) && (id < MFS_CFG_MAX_RECORDS)) {
		return true;
	}

	return false;
}

StorageStatus SettingStorageMFS::store(size_t id, const uint8_t *ptr, size_t size) {
	efiPrintf("MFS: Writing storage ID %d ... %d bytes", id, size);
	efitick_t startNt = getTimeNowNt();

	// TODO: add watchdog disable and enable in case MFS is on internal flash and one bank
	mfs_error_t err = mfsWriteRecord(m_drv, id, size, ptr);

	efitick_t endNt = getTimeNowNt();
	int elapsed_Ms = US2MS(NT2US(endNt - startNt));

	if (err >= MFS_NO_ERROR) {
		efiPrintf("MFS: Write done with no errors after %d mS MFS status %d", elapsed_Ms, err);
	} else {
		efiPrintf("MFS: Write FAILED after %d with MFS status %d", elapsed_Ms, err);

		return StorageStatus::Failed;
	}

	return StorageStatus::Ok;
}

StorageStatus SettingStorageMFS::read(size_t id, uint8_t *ptr, size_t size) {
	efiPrintf("MFS: Reading storage ID %d ... %d bytes", id, size);

	size_t readed_size = size;
	mfs_error_t err = mfsReadRecord(m_drv, id, &readed_size, ptr);

	if (err >= MFS_NO_ERROR) {
		if (readed_size != size) {
			efiPrintf("MFS: Incorrect size expected %d readed %d", size, readed_size);
			return StorageStatus::IncompatibleVersion;
		}
		efiPrintf("MFS: Reding done with no errors and MFS status %d", err);
	} else {
		efiPrintf("MFS: Read FAILED with MFS status %d", err);

		// TODO: or corrupted?
		return StorageStatus::NotFound;
	}
	return StorageStatus::Ok;
}

StorageStatus SettingStorageMFS::format()
{
	efitick_t startNt = getTimeNowNt();

	mfs_error_t err;
	err = mfsErase(m_drv);

	efitick_t endNt = getTimeNowNt();
	int elapsed_Ms = US2MS(NT2US(endNt - startNt));
	efiPrintf("MFS: format done %d mS err %d", elapsed_Ms, err);

	return (err >= MFS_NO_ERROR) ? StorageStatus::Ok : StorageStatus::Failed;
}

//static void eraseStorage() {
//	mfsStorageFormat();
//}

/* Managed Flash Storage driver */
static MFSDriver mfsd;
static NO_CACHE mfs_nocache_buffer_t mfsbuf;

static SettingStorageMFS storageMFS(&mfsd);

extern bool boardInitMfs(void);
extern const MFSConfig *boardGetMfsConfig(void);

bool initStorageMfs() {
	if (boardInitMfs() == false) {
		return false;
	}

	const MFSConfig *mfsConfig = boardGetMfsConfig();

	/* MFS */
	mfsObjectInit(&mfsd, &mfsbuf);
	mfs_error_t err = mfsStart(&mfsd, mfsConfig);
	if (err < MFS_NO_ERROR) {
		efiPrintf("MFS: storage failed to start: %d", err);
		return false;
	}

	//addConsoleAction("erasestorage", eraseStorage);

	storageMFS.m_ready = true;

	return storageRegisterStorage(STORAGE_MFS_EXT_FLASH, &storageMFS);
}

#endif //EFI_STORAGE_MFS
