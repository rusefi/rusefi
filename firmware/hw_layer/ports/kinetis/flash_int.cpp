/**
 *
 * @file    flash_int.c
 * @brief	Lower-level code for Kinetis related to internal flash memory
 * @author  andreika <prometheus.pcb@gmail.com>
 */

#include "pch.h"

#if EFI_INTERNAL_FLASH

#include "flash_int.h"
#include "fsl_ftfx_flexnvm.h"
#include <string.h>


//#define KINETIS_FLASH_DEBUG

typedef uint32_t flashdata_t;

static bool wasInit = false;
static bool isLocked = true;
static bool isInitializing = false;
static flexnvm_config_t flashCfg;

static status_t initStatus = -1;
static status_t protectStatus = -1;
static status_t securityStatus = -1;
static uint8_t protectValue = 0;
static ftfx_security_state_t sstate;

static kinetis_clock_type_e savedClockType = KINETIS_DEFAULT_CLK;

#ifdef KINETIS_FLASH_DEBUG
void flashPrintStatus(void) {
	debugLog("* flashInit status=%d\r\n", initStatus);
	debugLog("* FLEXNVM_DflashGetProtection status=%d prot=%d\r\n", protectStatus, protectValue);
	debugLog("* FLEXNVM_GetSecurityState status=%d security=0x%08x\r\n", securityStatus, sstate);

	uint32_t dflashBlockBase = 0;
    uint32_t dflashTotalSize = 0;
    uint32_t dflashSectorSize = 0;

    /* Get flash properties*/
    FLEXNVM_GetProperty(&flashCfg, kFLEXNVM_PropertyDflashBlockBaseAddr, &dflashBlockBase);
    FLEXNVM_GetProperty(&flashCfg, kFLEXNVM_PropertyDflashTotalSize, &dflashTotalSize);
    FLEXNVM_GetProperty(&flashCfg, kFLEXNVM_PropertyDflashSectorSize, &dflashSectorSize);

	debugLog("Data Flash Base Address: (0x%x)\r\n", dflashBlockBase);
	debugLog("Data Flash Total Size:\t%d KB, Hex: (0x%x)\r\n", (dflashTotalSize / 1024), dflashTotalSize);
	debugLog("Data Flash Sector Size:\t%d KB, Hex: (0x%x)\r\n", (dflashSectorSize / 1024), dflashSectorSize);
}
#endif /* KINETIS_FLASH_DEBUG */

// should be called only from flashUnlock()
static bool flashInit(void) {
	isInitializing = true;
	
	memset(&flashCfg, 0, sizeof(flashCfg));
	initStatus = FLEXNVM_Init(&flashCfg);
	protectStatus = FLEXNVM_DflashGetProtection(&flashCfg, &protectValue);
	securityStatus = FLEXNVM_GetSecurityState(&flashCfg, &sstate);

	wasInit = true;
	isInitializing = false;

#ifdef KINETIS_FLASH_DEBUG
	flashPrintStatus();
#endif /* KINETIS_FLASH_DEBUG */

	return (initStatus == kStatus_FTFx_Success);
}

bool flashUnlock(void) {
//	chSysUnconditionalLock();
	// this is strictly non-reentrant function!
	if (!isLocked) {
//		chSysUnconditionalUnlock();
		return false;	// already unlocked
	}
	isLocked = false;
	
	savedClockType = ke1xf_clock_get_current_type();
	// flash commands won't work in HSRUN clock mode
	// we set a lower frequency clock
	ke1xf_clock_init(kinetis_clock_int_osc_run);
	
	// before any flash access function, we should have called init() first (but not from init() itself)
	if (!isInitializing && !wasInit) {
		flashInit();
	}

	return true;
}

bool flashLock(void) {
	// this is strictly non-reentrant function!
	if (isLocked) {
		return false;	// already locked
	}

	// restore clock
	ke1xf_clock_init(savedClockType);

	isLocked = true;
//	chSysUnconditionalUnlock();

	return true;
}

static int alignToWord(int v) {
	return (v + FSL_FEATURE_FLASH_FLEX_NVM_SECTOR_CMD_ADDRESS_ALIGMENT - 1) & ~(FSL_FEATURE_FLASH_FLEX_NVM_SECTOR_CMD_ADDRESS_ALIGMENT - 1);
}

int intFlashErase(flashaddr_t address, size_t size) {
	if (!flashUnlock())
		return FLASH_RETURN_NO_PERMISSION;
		
	flashaddr_t sizeAligned = alignToWord(size);
	status_t status = FLEXNVM_DflashErase(&flashCfg, address, sizeAligned, kFTFx_ApiEraseKey);

	flashLock();

#ifdef KINETIS_FLASH_DEBUG
	debugLog("* flashErase(addr=%08x siz=%d sizeAligned=%d)=%d\r\n", address, size, sizeAligned, status);
#endif /* KINETIS_FLASH_DEBUG */

	if (status == kStatus_FTFx_Success)
		return FLASH_RETURN_SUCCESS;
	return -(int)status;
}

int intFlashWrite(flashaddr_t address, const char* buffer, size_t size) {
	if (!flashUnlock())
		return FLASH_RETURN_NO_PERMISSION;

	//FLEXNVM_DflashSetProtection(&flashCfg, 0xff);

	flashaddr_t sizeAligned = alignToWord(size);
	status_t status = FLEXNVM_DflashProgram(&flashCfg, address, (uint8_t *)buffer, sizeAligned);

	flashLock();

#ifdef KINETIS_FLASH_DEBUG
	debugLog("* flashWrite(addr=%08x siz=%d sizeAligned=%d)=%d\r\n", address, size, sizeAligned, status);
#endif /* KINETIS_FLASH_DEBUG */

	if (status == kStatus_FTFx_Success)
		return FLASH_RETURN_SUCCESS;
	return -(int)status;
}

bool intFlashIsErased(flashaddr_t address, size_t size) {
	/* Check for default set bits in the flash memory
	 * For efficiency, compare flashdata_t values as much as possible,
	 * then, fallback to byte per byte comparison. */
	while (size >= sizeof(flashdata_t)) {
		if (*(volatile flashdata_t*) address != (flashdata_t) (-1)) // flashdata_t being unsigned, -1 is 0xFF..FF
			return false;
		address += sizeof(flashdata_t);
		size -= sizeof(flashdata_t);
	}
	while (size > 0) {
		if (*(char*) address != 0xFF)
			return false;
		++address;
		--size;
	}

	return TRUE;
}

bool intFlashCompare(flashaddr_t address, const char* buffer, size_t size) {
#if 0
	uint32_t failAddr = 0, failDat = 0;
	status_t status = FLEXNVM_DflashVerifyProgram(&flashCfg, address, size, (const uint8_t *)buffer, 
                                             kFTFx_MarginValueUser, &failAddr, &failDat);
	return (status == kStatus_FTFx_Success);
#endif
	/* For efficiency, compare flashdata_t values as much as possible,
	 * then, fallback to byte per byte comparison. */
	while (size >= sizeof(flashdata_t)) {
		if (*(volatile flashdata_t*) address != *(flashdata_t*) buffer)
			return FALSE;
		address += sizeof(flashdata_t);
		buffer += sizeof(flashdata_t);
		size -= sizeof(flashdata_t);
	}
	while (size > 0) {
		if (*(volatile char*) address != *buffer)
			return FALSE;
		++address;
		++buffer;
		--size;
	}

	return TRUE;
}

int intFlashRead(flashaddr_t source, char* destination, size_t size) {
	memcpy(destination, (char*) source, size);
	return FLASH_RETURN_SUCCESS;
}

#endif /* EFI_INTERNAL_FLASH */
