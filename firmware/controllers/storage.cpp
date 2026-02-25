/**
 * @file    storage.cpp
 * @brief   adapter for unify access to MFS and legacy 'internal flash' storage drivers
 *
 * @date Jan 4, 2025
 * @author Andrey Gusakov
 */

#include "pch.h"

#include "storage.h"
#include "rusefi_lua.h"

#if EFI_CONFIGURATION_STORAGE
#include "mpu_util.h"
#endif

/* If any setting storage is exist or we are in unit test */
#if EFI_CONFIGURATION_STORAGE || defined(EFI_UNIT_TEST)
bool storageAllowWriteID(StorageItemId id)
{
#if (EFI_STORAGE_INT_FLASH == TRUE) || defined(EFI_UNIT_TEST)
	if ((id == EFI_SETTINGS_RECORD_ID) ||
		(id == EFI_SETTINGS_BACKUP_RECORD_ID)) {
		// special case, settings can be stored in internal flash

		// writing internal flash can cause cpu freeze
		// check if HW support flash writing while executing
		if (mcuCanFlashWhileRunning()) {
			return true;
		}

#if EFI_SHAFT_POSITION_INPUT
		// MCU does not support write while executing, check if engine is stopped
		if (engine->triggerCentral.directSelfStimulation || engine->rpmCalculator.isStopped()) {
			return true;
		} else {
			// rusEfi usually runs on hardware which halts execution while writing to internal flash, so we
			// postpone writes to until engine is stopped. Writes in case of self-stimulation are fine.
			return false;
		}
#endif // EFI_SHAFT_POSITION_INPUT
	}
#endif // EFI_STORAGE_INT_FLASH

	// TODO: we expect every other ID to be stored in external flash...
	return true;
}
#endif // EFI_CONFIGURATION_STORAGE || defined(EFI_UNIT_TEST)

/* If any setting storage is exist */
#if EFI_CONFIGURATION_STORAGE

#if EFI_STORAGE_INT_FLASH == TRUE
#include "storage_flash.h"
#endif

#if EFI_STORAGE_MFS == TRUE
#include "storage_mfs.h"
#endif

#if EFI_STORAGE_SD == TRUE
#include "storage_sd.h"
#endif

#define STORAGE_MANAGER_POLL_INTERVAL_MS	100

static constexpr size_t storagesCount = STORAGE_TOTAL;

static SettingStorageBase *storages[storagesCount];

chibios_rt::Mailbox<msg_t, 16> storageManagerMb;

#define MSG_CMD_WRITE		(0)
// force settings save independently of mcuCanFlashWhileRunning()
#define MSG_CMD_WRITE_NOW	(1)
#define MSG_CMD_READ		(2)
#define MSG_CMD_PING		(3)
#define MSG_CMD_REG			(4)
#define MSG_CMD_UNREG		(5)

#define MSG_ID_MASK		0x1f

static bool storageWriteID(uint32_t id) {
	// Do the actual flash write operation for given ID
	if (id == EFI_SETTINGS_RECORD_ID) {
		return writeToFlashNowImpl();
	} else if (id == EFI_LTFT_RECORD_ID) {
		engine->module<LongTermFuelTrim>()->store();
		return true;
	} else if (id == EFI_LUA_RECORD_ID) {
		saveLuaToMemory();
		return true;
	} else {
		efiPrintf("Requested to write unknown record id %ld", id);
		// to clear pending bit
		return true;
	}
	return true;
}

static bool storageReadID(uint32_t id) {
	// Do actual read and call consumers callback

	if (id == EFI_LTFT_RECORD_ID) {
		engine->module<LongTermFuelTrim>()->load();
		return true;
	} else {
		efiPrintf("Requested to read unknown record id %ld", id);
		// to clear pending bit
		return true;
	}
}

static const char *storageTypeToName(StorageType type) {
	switch (type) {
	case STORAGE_INT_FLASH:
		return "INT_FLASH";
	case STORAGE_MFS_INT_FLASH:
		return "MFS_INT_FLASH";
	case STORAGE_MFS_EXT_FLASH:
		return "MFS_EXT_FLASH";
	case STORAGE_SD_CARD:
		return "SD_CARD";
	default:
		break;
	}

	return "UNK";
}

#define for_all_storages	SettingStorageBase* storage = nullptr; \
	for (size_t i = 0; i < storagesCount; i++) \
		if ((storage = storages[i]) != nullptr)

static bool storageIsIdAvailableForId(StorageItemId id) {
	for_all_storages {
		if ((storage->isReady()) && (storage->isIdSupported(id))) {
			return true;
		}
	}

	return false;
}

StorageStatus storageWrite(StorageItemId id, const uint8_t *ptr, size_t size) {
	bool success = false;
	StorageStatus status = StorageStatus::NotSupported;

	for_all_storages {
		if ((!storage->isReady()) || (!storage->isIdSupported(id))) {
			continue;
		}

		status = storage->store(id, ptr, size);
		if (status == StorageStatus::Ok) {
			success = true;
		}
	}

	return (success ? StorageStatus::Ok : status);
}

StorageStatus storageRead(StorageItemId id, uint8_t *ptr, size_t size) {
	bool success = false;
	StorageStatus status = StorageStatus::NotSupported;

	for_all_storages {
		if ((!storage->isReady()) || (!storage->isIdSupported(id))) {
			continue;
		}

		status = storage->read(id, ptr, size);
		if (status == StorageStatus::Ok) {
			success = true;
		}
	}

	return (success ? StorageStatus::Ok : status);
}

static bool storageManagerSendCmd(uint32_t cmd, uint32_t arg)
{
	msg_t msg = ((cmd & 0xff) << 24) | (arg & 0x00ffffff);
	// Note: we are not sure about caller context and using universal (but not optimal) chSysGetStatusAndLockX()
	chibios_rt::CriticalSectionLocker csl;
	return (storageManagerMb.postI(msg) == MSG_OK);
}

bool storageRequestWriteID(StorageItemId id, bool forced) {
	return storageManagerSendCmd(forced ? MSG_CMD_WRITE_NOW : MSG_CMD_WRITE, (uint32_t)id);
}

bool storageReqestReadID(StorageItemId id) {
	return storageManagerSendCmd(MSG_CMD_READ, (uint32_t)id);
}

bool storageRegisterStorage(StorageType type, SettingStorageBase *storage) {
	if (type >= STORAGE_TOTAL) {
		return false;
	}

	if (storageIsStorageRegistered(type)) {
		/* already registered */
		efiPrintf("Trying to register already exist storage %s", storageTypeToName(type));
		return false;
	}

	storages[type] = storage;
	efiPrintf("Storage %s registered", storageTypeToName(type));

	storageManagerSendCmd(MSG_CMD_PING, 0);

	return true;
}

bool storageUnregisterStorage(StorageType type) {
	if (type >= STORAGE_TOTAL) {
		return false;
	}

	if (!storageIsStorageRegistered(type)) {
		/* already unregistered */
		efiPrintf("Trying to unregister non-exist storage %s", storageTypeToName(type));
		return false;
	}

	storages[type] = nullptr;
	efiPrintf("Storage %s unregistered", storageTypeToName(type));

	return true;
}

bool storageIsStorageRegistered(StorageType type) {
	if (type >= STORAGE_TOTAL) {
		return false;
	}

	return (storages[type] != nullptr);
}

bool storagRequestRegisterStorage(StorageType id)
{
	return storageManagerSendCmd(MSG_CMD_REG, (uint32_t)id);
}

bool storagRequestUnregisterStorage(StorageType id)
{
	return storageManagerSendCmd(MSG_CMD_UNREG, (uint32_t)id);
}

static uint32_t pendingWrites = 0;
static uint32_t pendingReads = 0;

#if (EFI_STORAGE_MFS == TRUE) || (EFI_STORAGE_SD == TRUE)
/* in case of MFS or SD card we need more stack */
static THD_WORKING_AREA(storageManagerThreadStack, 3 * UTILITY_THREAD_STACK_SIZE);
#else
static THD_WORKING_AREA(storageManagerThreadStack, UTILITY_THREAD_STACK_SIZE);
#endif

static void storageManagerThread(void*) {
	chRegSetThreadName("storage manger");

	while (true) {
		msg_t ret;
		msg_t msg;

		// Wait for a request to come in or timeout
		ret = storageManagerMb.fetch(&msg, TIME_MS2I(STORAGE_MANAGER_POLL_INTERVAL_MS));
		if (ret == MSG_OK) {
			uint8_t cmd = (msg >> 24) & 0xff;
			uint32_t id = msg & MSG_ID_MASK;

			switch (cmd) {
			case MSG_CMD_READ:
				pendingReads |= BIT(id);
				break;
			case MSG_CMD_WRITE:
				pendingWrites |= BIT(id);
				break;
			case MSG_CMD_WRITE_NOW:
				pendingWrites |= BIT(id);
				// skip storageAllowWriteID() check
				if (storageWriteID(id)) {
					pendingWrites &= ~BIT(id);
				}
				break;
			case MSG_CMD_PING:
				/* nop */
				break;
			case MSG_CMD_REG:
#if EFI_STORAGE_INT_FLASH == TRUE
				if ((StorageType)id == STORAGE_INT_FLASH) {
					initStorageFlash();
				}
#endif // EFI_STORAGE_INT_FLASH
#if EFI_STORAGE_MFS == TRUE
				if ((StorageType)id == STORAGE_MFS_EXT_FLASH) {
					initStorageMfs();
				}
#endif // EFI_STORAGE_MFS
#if EFI_STORAGE_SD == TRUE
				if ((StorageType)id == STORAGE_SD_CARD) {
					initStorageSD();
				}
#endif // EFI_STORAGE_SD
				break;
			case MSG_CMD_UNREG:
				storageUnregisterStorage((StorageType)id);
				break;
			default:
				/* ignore */
				break;
			}
		}

		// check if we can read some of pending IDs...
		for (size_t i = 0; (i < EFI_STORAGE_TOTAL_ITEMS) && pendingReads; i++) {
			if ((pendingReads & BIT(i)) == 0) {
				continue;
			}

			StorageItemId id = (StorageItemId)i;
			if (!storageIsIdAvailableForId(id)) {
				continue;
			}

			if (storageReadID(id)) {
				pendingReads &= ~BIT(id);
			}
		}

		// check if we can write some of pendind IDs...
		for (size_t i = 0; (i < EFI_STORAGE_TOTAL_ITEMS) && pendingWrites; i++) {
			if ((pendingWrites & BIT(i)) == 0) {
				continue;
			}

			StorageItemId id = (StorageItemId)i;
			if (!storageIsIdAvailableForId(id)) {
				continue;
			}

			if (!storageAllowWriteID(id)) {
				continue;
			}

			if (storageWriteID(id)) {
				pendingWrites &= ~BIT(id);
			}
		}
	}
}

/* misc helpers */
bool getNeedToWriteConfiguration() {
	return (pendingWrites & BIT(EFI_SETTINGS_RECORD_ID)) != 0;
}

void initStorage() {
	bool settingsStorageReady = false;
	// may be unused
	(void)settingsStorageReady;

#if EFI_STORAGE_INT_FLASH == TRUE
	settingsStorageReady = initStorageFlash();
#endif // EFI_STORAGE_INT_FLASH

#if EFI_STORAGE_MFS == TRUE
	if (settingsStorageReady) {
		// Skip MFS if internal storage is used for persistentState
		// Init of MFS may take significant time, lets postpone it until storage manager thread
		storagRequestRegisterStorage(STORAGE_MFS_EXT_FLASH);
	} else {
		// Set long timeout to watchdog as this code is called before any thread is started
		// and no one is feeding watchdog
		startWatchdog(WATCHDOG_MFS_START_TIMEOUT_MS);

		initStorageMfs();

		// restart the watchdog with the default timeout
		startWatchdog();
	}
#endif // EFI_STORAGE_MFS

	chThdCreateStatic(storageManagerThreadStack, sizeof(storageManagerThreadStack), PRIO_STORAGE_MANAGER, storageManagerThread, nullptr);
}

#endif // EFI_CONFIGURATION_STORAGE
