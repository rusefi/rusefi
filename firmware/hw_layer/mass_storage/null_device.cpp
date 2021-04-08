/**
 * @file	zero_device.cpp
 *
 * @date Feb 12, 2021
 * @author Matthew Kennedy, (c) 2021
 *
 * This file implements a special block device that simply reports "no media"
 * Use it when you need to mount *something* but don't have an SD card available.
 */

#include "hal.h"

#include "mass_storage_device.h"

#include "global.h"

#if EFI_EMBED_INI_MSD
#include "ramdisk.h"
#include "compressed_block_device.h"

#ifdef EFI_USE_COMPRESSED_INI_MSD
#include "ramdisk_image_compressed.h"
#else
#include "ramdisk_image.h"
#endif

// If the ramdisk image told us not to use it, don't use it.
#ifdef RAMDISK_INVALID
#undef EFI_EMBED_INI_MSD
#define EFI_EMBED_INI_MSD FALSE
#endif

#endif

#include <cstring>

struct NullDevice {
	const struct BaseBlockDeviceVMT *vmt;
	_base_block_device_data
};

static bool nd_is_inserted(void*) {
	// This function is the whole point - we have no media!
	return false;
}

static bool nd_is_protected(void*) {
	return false;
}

static bool nd_return_success(void*) {
	return HAL_SUCCESS;
}

static bool nd_return_success_read(void*, uint32_t, uint8_t* buffer, uint32_t n) {
	// write zeroes to the buffer to prevent somebody reading random memory
	memset(buffer, 0, n);

	return HAL_SUCCESS;
}

static bool nd_return_success_write(void*, uint32_t, const uint8_t*, uint32_t) {
	return HAL_SUCCESS;
}

static bool nd_get_info(void*, BlockDeviceInfo* bdip) {
	// We have to report non-zero size here because Windows
	// will query the size of the block device even if we indicate
	// that the device has no media
	// If we report zeroes, it breaks USB until you unplug this device
	bdip->blk_num = 1000;
	bdip->blk_size = 512;
	return HAL_SUCCESS;
}

static const struct BaseBlockDeviceVMT ndVmt = {
	(size_t)0, // instanceOffset
	nd_is_inserted,
	nd_is_protected,

	// These functions just claim success to make the host happy
	nd_return_success,
	nd_return_success,
	nd_return_success_read,
	nd_return_success_write,
	nd_return_success,
	nd_get_info
};

#if EFI_EMBED_INI_MSD
#ifdef EFI_USE_COMPRESSED_INI_MSD
static CompressedBlockDevice cbd;
#else
static RamDisk ramdisk;
#endif
#else
// This device is always ready and has no state
static NullDevice nd = { &ndVmt, BLK_READY };
#endif

#if HAL_USE_USB_MSD
#if STM32_USB_USE_OTG2
  USBDriver *usb_driver = &USBD2;
#else
  USBDriver *usb_driver = &USBD1;
#endif

static NO_CACHE uint8_t blkbuf0[MMCSD_BLOCK_SIZE];
static NO_CACHE uint8_t blkbuf1[MMCSD_BLOCK_SIZE];


static MassStorageController<2> msd(usb_driver);

static const scsi_inquiry_response_t iniDriveInquiry = {
    0x00,           /* direct access block device     */
    0x80,           /* removable                      */
    0x04,           /* SPC-2                          */
    0x02,           /* response data format           */
    0x20,           /* response has 0x20 + 4 bytes    */
    0x00,
    0x00,
    0x00,
    "rusEFI",
    "INI Drive",
    {'v',CH_KERNEL_MAJOR+'0','.',CH_KERNEL_MINOR+'0'}
};

static const scsi_inquiry_response_t sdCardInquiry = {
    0x00,           /* direct access block device     */
    0x80,           /* removable                      */
    0x04,           /* SPC-2                          */
    0x02,           /* response data format           */
    0x20,           /* response has 0x20 + 4 bytes    */
    0x00,
    0x00,
    0x00,
    "rusEFI",
    "SD Card",
    {'v',CH_KERNEL_MAJOR+'0','.',CH_KERNEL_MINOR+'0'}
};

void attachMsdSdCard(BaseBlockDevice* blkdev) {
	msd.attachLun(1, blkdev, blkbuf1, &sdCardInquiry, nullptr);
}

void initUsbMsd() {
	// attach a null device in place of the SD card for now
	msd.attachLun(1, (BaseBlockDevice*)&nd, blkbuf1, &sdCardInquiry, nullptr);


#if EFI_EMBED_INI_MSD
#ifdef EFI_USE_COMPRESSED_INI_MSD
	uzlib_init();
	compressedBlockDeviceObjectInit(&cbd);
	compressedBlockDeviceStart(&cbd, ramdisk_image_gz, sizeof(ramdisk_image_gz));
	msd.attachLun(0, (BaseBlockDevice*)&cbd, blkbuf0, &iniDriveInquiry, nullptr);
#else // not EFI_USE_COMPRESSED_INI_MSD
	ramdiskObjectInit(&ramdisk);

	constexpr size_t ramdiskSize = sizeof(ramdisk_image);
	constexpr size_t blockSize = 512;
	constexpr size_t blockCount = ramdiskSize / blockSize;

	// Ramdisk should be a round number of blocks
	static_assert(ramdiskSize % blockSize == 0);

	ramdiskStart(&ramdisk, const_cast<uint8_t*>(ramdisk_image), blockSize, blockCount, /*readonly =*/ true);

	msd.attachLun(0, (BaseBlockDevice*)&ramdisk, blkbuf0, &iniDriveInquiry, nullptr);
#endif // EFI_USE_COMPRESSED_INI_MSD
#else // not EFI_EMBED_INI_MSD
	// No embedded ini file, just mount the null device instead
	msd.attachLun(0, (BaseBlockDevice*)&nd, blkbuf0, &iniDriveInquiry, nullptr);
#endif

	// start the mass storage thread
	msd.Start();
}
#endif
