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

class SettingStorageSD : public SettingStorageBase {
public:
	bool isIdSupported(size_t id) override;
	StorageStatus store(size_t id, const uint8_t *ptr, size_t size) override;
	StorageStatus read(size_t id, uint8_t *ptr, size_t size) override;
	StorageStatus format() override;

	SettingStorageSD(FIL *fd) {
		m_fd = fd;
	}

private:
	const char *getIdFileName(size_t id);
	FIL *m_fd;
};

const char *SettingStorageSD::getIdFileName(size_t id)
{
	switch (id) {
	case EFI_LTFT_RECORD_ID:
		return "ltft.bin";
	default:
		return nullptr;
	}
}

bool SettingStorageSD::isIdSupported(size_t id)
{
	return (getIdFileName(id) != nullptr);
}

StorageStatus SettingStorageSD::store(size_t id, const uint8_t *ptr, size_t size) {
	const char *fileName = getIdFileName(id);

	if (fileName == nullptr) {
		return StorageStatus::NotSupported;
	}

	efiPrintf("SD: Writing storage ID %d  %s... %d bytes", id, fileName, size);
	efitick_t startNt = getTimeNowNt();

	/* Create new or truncate file. */
	FRESULT err = f_open(m_fd, fileName, FA_CREATE_ALWAYS | FA_WRITE);
	if (err != FR_OK) {
		efiPrintf("SD: failed to create file %d", err);
		return StorageStatus::Failed;
	}

	StorageStatus status = StorageStatus::Ok;
	size_t bytesWritten = 0;
	err = f_write(m_fd, ptr, size, &bytesWritten);
	if (err != FR_OK) {
		efiPrintf("SD: failed to write %d", err);
		status = StorageStatus::Failed;
	}

	if (bytesWritten != size) {
		efiPrintf("SD: failed to write whole file %d != %d", bytesWritten, size);
		status = StorageStatus::Failed;
	}

	f_close(m_fd);

	efitick_t endNt = getTimeNowNt();
	int elapsed_Ms = US2MS(NT2US(endNt - startNt));

	efiPrintf("SD: Write done after %d mS", elapsed_Ms);

	return status;
}

StorageStatus SettingStorageSD::read(size_t id, uint8_t *ptr, size_t size) {
	const char *fileName = getIdFileName(id);

	if (fileName == nullptr) {
		return StorageStatus::NotSupported;
	}

	efiPrintf("SD: Reading storage ID %d %s ... %d bytes", id, fileName, size);

	/* Create new or truncate file. */
	FRESULT err = f_open(m_fd, fileName, FA_READ);
	if (err != FR_OK) {
		efiPrintf("SD: failed to open file %d", err);
		return StorageStatus::NotFound;
	}

	StorageStatus status = StorageStatus::Ok;
	size_t bytesRead = 0;
	err = f_read(m_fd, ptr, size, &bytesRead);
	if (err != FR_OK) {
		efiPrintf("SD: failed to read %d", err);
		status = StorageStatus::Failed;
	}

	if (bytesRead != size) {
		efiPrintf("SD: failed to read whole file %d != %d", bytesRead, size);
		status = StorageStatus::Failed;
	}

	f_close(m_fd);

	efiPrintf("SD: Reading done");

	return status;
}

StorageStatus SettingStorageSD::format() {
	/* TODO: actually format sd? */

	return StorageStatus::NotSupported;
}

static NO_CACHE FIL fd;

static SettingStorageSD storageSD(&fd);

SettingStorageBase *initStorageSD() {
	return &storageSD;
}

#endif //EFI_STORAGE_SD
