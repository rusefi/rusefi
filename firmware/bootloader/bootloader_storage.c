
#include "global.h"
#include "bootloader/bootloader.h"

#if defined(EFI_BOOTLOADER_INCLUDE_CODE)

#if defined __GNUC__
#define BOOTLOADER_SECTION __attribute__((section(".bl")))	/* todo: add 'used'? */
#else
#define BOOTLOADER_SECTION @ ".bl"
#endif

#include <bootloader_generated.hxx>

int initBootloader(void) {
	// just make sure we use 'bootloader_code' array
	// We're forcing the array to be included into the linked binary.
	// The first 4 bytes contain a 32-bit address in RAM, which always starts from 0x20000000 for STM32.
	// So the bootloader_code[3] is the most significant byte of the address which equals to 0x20.
	return (bootloader_code[3] == 0x20) ? 0 : 1;	/* todo: should always be 0x20? */
}

#endif /* EFI_BOOTLOADER_INCLUDE_CODE */
