#pragma once

#include "pch.h"

#if HAL_USE_USB_MSD
void initUsbMsd();
void attachMsdSdCard(BaseBlockDevice* blkdev, uint8_t *blkbuf, size_t blkbufsize);
void deattachMsdSdCard(void);

#if EFI_EMBED_INI_MSD
	#if EFI_USE_COMPRESSED_INI_MSD
		#include "compressed_block_device.h"
		#include "ramdisk_image_compressed.h"
	#else // EFI_USE_COMPRESSED_INI_MSD
		#include "ramdisk.h"
		#include "ramdisk_image.h"
	#endif // EFI_USE_COMPRESSED_INI_MSD

	// If the ramdisk image told us not to use it, don't use it.
	#ifdef RAMDISK_INVALID
		#undef EFI_EMBED_INI_MSD
		#define EFI_EMBED_INI_MSD FALSE
	#endif // RAMDISK_INVALID
#endif // EFI_EMBED_INI_MSD


#endif // HAL_USE_USB_MSD
