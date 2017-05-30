
#include "main.h"
#include "bootloader/bootloader.h"

#if defined(EFI_BOOTLOADER_INCLUDE_CODE) || defined(__DOXYGEN__)

#if defined __GNUC__
#define BOOTLOADER_SECTION __attribute__((section(".bl")))	/* todo: add 'used'? */
#else
#define BOOTLOADER_SECTION @ ".bl"
#endif

#include <bootloader_generated.hxx>

int initBootloader(void) {
	// just make sure we use 'bootloader_code' array
	return (bootloader_code[3] == 0x20) ? 0 : 1;	/* todo: should always be 0x20? */
}

#endif /* EFI_BOOTLOADER_INCLUDE_CODE */
