#include "pch.h"

#include "mass_storage_init.h"
#include "mass_storage_device.h"
#include "null_device.h"

#if HAL_USE_USB_MSD

#if EFI_EMBED_INI_MSD
	#if EFI_USE_COMPRESSED_INI_MSD
		#include "compressed_block_device.h"
		#include "ramdisk_image_compressed.h"
	#else
		#include "ramdisk.h"
		#include "ramdisk_image.h"
	#endif

	// If the ramdisk image told us not to use it, don't use it.
	#ifdef RAMDISK_INVALID
		#undef EFI_EMBED_INI_MSD
		#define EFI_EMBED_INI_MSD FALSE
	#endif
#endif

#if EFI_EMBED_INI_MSD
	#if EFI_USE_COMPRESSED_INI_MSD
		/* WARNING: CompressedBlockDevice will consume ~34Kb RAM! */
		/* Enabling this option will also consume (2048-256) additional RAM bytes for increased USB_MSD_THREAD_WA_SIZE */
		static CompressedBlockDevice cbd;
	#else
		/* WARNING: this will add 128K of ROM data */
		static RamDisk ramdisk;
	#endif
#endif

#if STM32_USB_USE_OTG1
  USBDriver *usb_driver = &USBD1;
#elif STM32_USB_USE_OTG2
  USBDriver *usb_driver = &USBD2;
#else
  #error MSD needs OTG1 or OTG2 to be enabled
#endif

// One block buffer per LUN
static NO_CACHE uint8_t blkbuf0[MMCSD_BLOCK_SIZE];

static MassStorageController msd(usb_driver);

static const scsi_inquiry_response_t iniDriveInquiry = {
    0x00,           /* direct access block device     */
    0x80,           /* removable                      */
    0x04,           /* SPC-2                          */
    0x02,           /* response data format           */
    sizeof(scsi_inquiry_response_t) - 5,	// size of this struct, minus bytes up to and including this one
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
    sizeof(scsi_inquiry_response_t) - 5,	// size of this struct, minus bytes up to and including this one
    0x00,
    0x00,
    0x00,
    "rusEFI",
    "SD Card",
    {'v',CH_KERNEL_MAJOR+'0','.',CH_KERNEL_MINOR+'0'}
};

void attachMsdSdCard(BaseBlockDevice* blkdev, uint8_t *blkbuf, size_t blkbufsize) {
	if ((blkbuf == NULL) || (blkbufsize == 0)) {
		// if no specific buffer was provided use default
		blkbuf = blkbuf0;
		blkbufsize = sizeof(blkbuf0);
	}
	msd.attachLun(1, blkdev, blkbuf, blkbufsize, &sdCardInquiry, nullptr);

#if EFI_TUNER_STUDIO
	// SD MSD attached, enable indicator in TS
	engine->outputChannels.sd_msd = true;
#endif
}

void deattachMsdSdCard(void) {
	// this is safe to use same read/write buffer couse all luns are handled from one thread
	msd.attachLun(1, (BaseBlockDevice*)&ND1, blkbuf0, sizeof(blkbuf0), &sdCardInquiry, nullptr);

#if EFI_TUNER_STUDIO
	// SD MSD attached, enable indicator in TS
	engine->outputChannels.sd_msd = false;
#endif
}

static BaseBlockDevice* getRamdiskDevice() {
#if EFI_EMBED_INI_MSD
#if EFI_USE_COMPRESSED_INI_MSD
	uzlib_init();
	compressedBlockDeviceObjectInit(&cbd);
	compressedBlockDeviceStart(&cbd, ramdisk_image_gz, sizeof(ramdisk_image_gz));

	return (BaseBlockDevice*)&cbd;
#else // not EFI_USE_COMPRESSED_INI_MSD
	ramdiskObjectInit(&ramdisk);

	constexpr size_t ramdiskSize = sizeof(ramdisk_image);
	constexpr size_t blockSize = 512;
	constexpr size_t blockCount = ramdiskSize / blockSize;

	// Ramdisk should be a round number of blocks
	static_assert(ramdiskSize % blockSize == 0);

	ramdiskStart(&ramdisk, const_cast<uint8_t*>(ramdisk_image), blockSize, blockCount, /*readonly =*/ true);

	return (BaseBlockDevice*)&ramdisk;
#endif // EFI_USE_COMPRESSED_INI_MSD
#else // not EFI_EMBED_INI_MSD
	// No embedded ini file, just mount the null device instead
	return (BaseBlockDevice*)&ND1;
#endif
}

void initUsbMsd() {
	// Attach the ini ramdisk
	msd.attachLun(0, getRamdiskDevice(), blkbuf0, sizeof(blkbuf0), &iniDriveInquiry, nullptr);

	// attach a null device in place of the SD card for now - the SD thread may replace it later
	// this is safe to use same read/write buffer couse all luns are handled from one thread
	msd.attachLun(1, (BaseBlockDevice*)&ND1, blkbuf0, sizeof(blkbuf0), &sdCardInquiry, nullptr);

	// start the mass storage thread
	msd.start();
}

#endif // HAL_USE_USB_MSD
