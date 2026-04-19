/**
 * @file	stm32_opt.cpp
 * @brief	STM32 options bytes manipulation helpers
 *
 * @date Apr 15, 2026
 * @author Andrey Belomutskiy, (c) 2012-2020
 * @author Andrey Gusakov
 */

#include "pch.h"

#include "hardware.h"
#include "os_util.h"

#if EFI_PROD_CODE

#include "mpu_util.h"
#include "flash_main.h"

#ifdef STM32F4XX
#include "stm32f4xx_hal_flash.h"
#elif defined(STM32F7XX)
#include "stm32f7xx_hal_flash.h"
#elif defined(STM32H7XX)
#include "stm32h7xx_hal_flash.h"

#define OB_WRP_SECTOR_All		OB_WRP_SECTOR_ALL
#define OPTIONBYTE_BOOTADDR_0	OPTIONBYTE_BOOTADD
#define OPTIONBYTE_BOOTADDR_1	OPTIONBYTE_BOOTADD

#define FLASH_OPTCR_BOR_LEV_Pos	FLASH_OPTSR_BOR_LEV_Pos

#endif

void printWRPBits() {
	uint32_t optcr = FLASH->OPTCR;
	// OPTCR bits [27:16] are nWRP[11:0]
	uint16_t wrp = (optcr >> 16) & 0xFFF;
	efiPrintf("OPTCR: %08lx WRP: %03x", optcr, wrp);

#if defined(STM32F4)
	if (isStm32F42x()) {
		// F427/437 has OPTCR1 at 0x40023C18
		uint32_t optcr1 = *(__IO uint32_t *)(0x40023C18);
		// OPTCR1 bits [27:16] are nWRP[23:12]
		uint16_t wrp1 = (optcr1 >> 16) & 0xFFF;
		efiPrintf("OPTCR1: %08lx WRP12-23: %03x", optcr1, wrp1);
	}
#endif
}

void printOptBytes() {
	FLASH_OBProgramInitTypeDef obConfig;

	HAL_FLASHEx_OBGetConfig(&obConfig);

	if (obConfig.OptionType & OPTIONBYTE_WRP) {
		efiPrintf("nWRP = 0x%04lx", obConfig.WRPSector);
	}

	if (obConfig.OptionType & OPTIONBYTE_RDP) {
		efiPrintf("RDP = 0x%02lx %s",
			obConfig.RDPLevel,
			(obConfig.RDPLevel == OB_RDP_LEVEL_0) ? "Level0" :
				(obConfig.RDPLevel == OB_RDP_LEVEL_2) ? "Level2" : "Level1");
	}

	if (obConfig.OptionType & OPTIONBYTE_USER) {
#if defined(OB_WWDG_SW)
		efiPrintf("OB_WWDG_SW = %d", !!(obConfig.USERConfig & OB_WWDG_SW));
#endif
		efiPrintf("OB_IWDG_SW = %d", !!(obConfig.USERConfig & OB_IWDG_SW));
		efiPrintf("OB_STOP_NO_RST = %d", !!(obConfig.USERConfig & OB_STOP_NO_RST));
		efiPrintf("OB_STDBY_NO_RST = %d", !!(obConfig.USERConfig & OB_STDBY_NO_RST));
#if defined(OB_IWDG_STOP_ACTIVE)
		efiPrintf("OB_IWDG_STOP_ACTIVE = %d", !!(obConfig.USERConfig & OB_IWDG_STOP_ACTIVE));
#endif
#if defined(OB_IWDG_STDBY_ACTIVE)
		efiPrintf("OB_IWDG_STDBY_ACTIVE = %d", !!(obConfig.USERConfig & OB_IWDG_STDBY_ACTIVE));
#endif
#if defined (FLASH_OPTCR_nDBOOT)
		efiPrintf("OB_DUAL_BOOT_ENABLE = %d", !!(obConfig.USERConfig & OB_DUAL_BOOT_ENABLE));
#endif
#if defined (FLASH_OPTCR_nDBANK)
		efiPrintf("OB_NDBANK_DUAL_BANK = %d", !!(obConfig.USERConfig & OB_NDBANK_DUAL_BANK));
#endif
	}

	if (obConfig.OptionType & OPTIONBYTE_BOR) {
		efiPrintf("BORLevel = 0x%lx", obConfig.BORLevel >> FLASH_OPTCR_BOR_LEV_Pos);
	}

#if defined(STM32F7) || defined(STM32H7)
	if (obConfig.OptionType & OPTIONBYTE_BOOTADDR_0) {
		efiPrintf("BootAddr0 = 0x%04lx", obConfig.BootAddr0);
	}

	if (obConfig.OptionType & OPTIONBYTE_BOOTADDR_1) {
		efiPrintf("BootAddr1 = 0x%04lx", obConfig.BootAddr1);
	}
#endif
}

void removeWRP() {
	suspendLinearTimeWatcher();

	FLASH_OBProgramInitTypeDef OBInit;
	memset(&OBInit, 0, sizeof(OBInit));

	OBInit.OptionType = OPTIONBYTE_WRP;
	OBInit.WRPState = OB_WRPSTATE_DISABLE;
	OBInit.WRPSector = OB_WRP_SECTOR_All;
	OBInit.RDPLevel = OB_RDP_LEVEL_0;
#if defined(STM32F4)
	if (isStm32F42x()) {
		#define FLASH_BANK_2_hack     ((uint32_t)2) /*!< Bank 2   */
		OBInit.Banks = FLASH_BANK_1 | FLASH_BANK_2_hack;
	} else {
		OBInit.Banks = FLASH_BANK_1;
	}
#endif

	HAL_FLASH_OB_Unlock();
	HAL_StatusTypeDef status = HAL_FLASHEx_OBProgram(&OBInit);
	efiPrintf("removeWRP: program status %d", status);
	if (status == HAL_OK) {
		efiPrintf("removeWRP: launching...");
		HAL_FLASH_OB_Launch();
	}
	HAL_FLASH_OB_Lock();
}

#endif // EFI_PROD_CODE
