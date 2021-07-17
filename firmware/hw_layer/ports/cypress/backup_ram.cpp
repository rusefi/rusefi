/**
 * @file	backup_ram.cpp
 * @brief	NVRAM emulation using Internal Flash (flash_int driver)
 *
 * @date May 22, 2020
 */

#include "backup_ram.h"
#include "flash_int.h"

#define BACKUP_NOT_INITIALIZED 0xFFFF
#define BACKUP_SAVED           0x5555
#define BACKUP_PENDING         0x0000

// we store the flash state at 0 index + all backup variables
static volatile uint32_t backupRam[BACKUP_RAM_NUM + 1];
static bool wasLoaded = false;
// these offsets are indices in the 'BACKUP_FLASH_ADDR' (32-bit array)
static const int backupStateOffset = 0, backupDataOffset = 1;
const size_t backupSize = (BACKUP_RAM_NUM + 1) * sizeof(uint32_t);

static void backupInit(void) {
	static_assert(backupSize <= BACKUP_FLASH_SIZE, "Backup flash overflow");

	// first, load the whole buffer into the memory
	intFlashRead((flashaddr_t)BACKUP_FLASH_ADDR, (char *)backupRam, backupSize);
	// check if we have a reliable properly saved data
	if (backupRam[backupStateOffset] != BACKUP_SAVED) {	
		// zero is the default value
		memset((void *)backupRam, 0, backupSize);
	}

	// we cannot trust the saved data anymore, until it's saved in backupRamFlush()
	// so we mark is as 'pending'
	backupRam[backupStateOffset] = BACKUP_PENDING;
	intFlashWrite(BACKUP_FLASH_ADDR + backupStateOffset, (char *)backupRam, sizeof(backupRam[backupStateOffset]));
	
	wasLoaded = true;
}

uint32_t backupRamLoad(backup_ram_e idx) {
	// this is executed only once during the firmware init
	if (!wasLoaded) {
		backupInit();
	}

	return backupRam[idx + backupDataOffset];
}

void backupRamSave(backup_ram_e idx, uint32_t value) {
	// this is executed only once during the firmware init
	if (!wasLoaded) {
		backupInit();
	}

	backupRam[idx + backupDataOffset] = value;
}

void backupRamFlush(void) {

    // todo: implement an incremental "append-to-the-end" algorithm to minimize sector erasings?

    // Enter the critical zone
    syssts_t sts = chSysGetStatusAndLockX();

	// rewrite the whole sector
	intFlashErase((flashaddr_t)BACKUP_FLASH_ADDR, BACKUP_FLASH_SIZE);
	// mark the data as valid & saved
	backupRam[backupStateOffset] = BACKUP_SAVED;
	// save the data to the flash
	intFlashWrite((flashaddr_t)BACKUP_FLASH_ADDR, (char *)backupRam, backupSize);

    // Leaving the critical zone
    chSysRestoreStatusX(sts);
	
	// there should not be any backup-RAM activity after this call
	// but if there is, at least try to reinitialize...
	wasLoaded = false;
}
