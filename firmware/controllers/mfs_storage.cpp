/**
 * @file    mfs_storage.cpp
 * @brief   Storage interface to ChibiOS MFS driver
 *
 * @date Jan 4, 2025
 * @author Andrey Gusakov
 */

#include "pch.h"

#include "storage.h"

/* if we use ChibiOS MFS for settings */
#if EFI_STORAGE_MFS == TRUE

#include "hal_mfs.h"

/* Managed Flash Storage driver */
static MFSDriver mfsd;
static NO_CACHE mfs_nocache_buffer_t mfsbuf;

extern void boardInitMfs(void);
extern const MFSConfig *boardGetMfsConfig(void);

StorageStatus mfsStorageWrite(int id, const uint8_t *ptr, size_t size) {
	efiPrintf("Writing storage ID %d ... %d bytes", id, size);
	efitick_t startNt = getTimeNowNt();

	// TODO: add watchdog disable and enable in case MFS is on internal flash and one bank
	mfs_error_t err = mfsWriteRecord(&mfsd, id, size, ptr);

	efitick_t endNt = getTimeNowNt();
	int elapsed_Ms = US2MS(NT2US(endNt - startNt));

	if (err >= MFS_NO_ERROR) {
		efiPrintf("Write done with no errors after %d mS MFS status %d", elapsed_Ms, err);
	} else {
		efiPrintf("Write FAILED after %d with MFS status %d", elapsed_Ms, err);

		return StorageStatus::Failed;
	}

	return StorageStatus::Ok;
}

StorageStatus mfsStorageRead(int id, uint8_t *ptr, size_t size) {
	efiPrintf("Reading storage ID %d ... %d bytes", id, size);

	size_t readed_size = size;
	mfs_error_t err = mfsReadRecord(&mfsd, id, &readed_size, ptr);

	if (err >= MFS_NO_ERROR) {
		if (readed_size != size) {
			efiPrintf("Incorrect size expected %d readed %d", size, readed_size);
			return StorageStatus::IncompatibleVersion;
		}
		efiPrintf("Reding done with no errors and MFS status %d", err);
	} else {
		efiPrintf("Read FAILED with MFS status %d", err);

		// TODO: or corrupted?
		return StorageStatus::NotFound;
	}
	return StorageStatus::Ok;
}

StorageStatus mfsStorageFormat()
{
	efitick_t startNt = getTimeNowNt();

	mfs_error_t err;
	err = mfsErase(&mfsd);

	efitick_t endNt = getTimeNowNt();
	int elapsed_Ms = US2MS(NT2US(endNt - startNt));
	efiPrintf("MFS erase done %d mS err %d", elapsed_Ms, err);

	return (err >= MFS_NO_ERROR) ? StorageStatus::Ok : StorageStatus::Failed;
}

static void eraseStorage() {
	mfsStorageFormat();
}

void initStorageMfs() {
	boardInitMfs();
	const MFSConfig *mfsConfig = boardGetMfsConfig();

	/* MFS */
	mfsObjectInit(&mfsd, &mfsbuf);
	mfs_error_t err = mfsStart(&mfsd, mfsConfig);
	if (err < MFS_NO_ERROR) {
		/* hm...? */
	}

	addConsoleAction("erasestorage", eraseStorage);
}

#endif //EFI_STORAGE_MFS
