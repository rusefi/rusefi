#include "mass_storage_init.h"
#include "mass_storage_device.h"
#include "null_device.h"
#include "tunerstudio_outputs.h"

#if HAL_USE_USB_MSD

#if EFI_EMBED_INI_MSD
	#ifdef EFI_USE_COMPRESSED_INI_MSD
		#include "compressed_block_device.h"
		#include "ramdisk_image_compressed.h"

		static CompressedBlockDevice cbd;
	#else
		#include "ramdisk.h"
		#include "ramdisk_image.h"

		static RamDisk ramdisk;
	#endif

	// If the ramdisk image told us not to use it, don't use it.
	#ifdef RAMDISK_INVALID
		#undef EFI_EMBED_INI_MSD
		#define EFI_EMBED_INI_MSD FALSE
	#endif
#endif

#if STM32_USB_USE_OTG2
  USBDriver *usb_driver = &USBD2;
#else
  USBDriver *usb_driver = &USBD1;
#endif

// One block buffer per LUN
static NO_CACHE uint8_t blkbuf0[MMCSD_BLOCK_SIZE];
static NO_CACHE uint8_t blkbuf1[MMCSD_BLOCK_SIZE];

static MassStorageController msd(usb_driver);

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

#if EFI_TUNER_STUDIO
	// SD MSD attached, enable indicator in TS
	tsOutputChannels.sd_msd = true;
#endif
}

static BaseBlockDevice* getRamdiskDevice() {
#if EFI_EMBED_INI_MSD
#ifdef EFI_USE_COMPRESSED_INI_MSD
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
	msd.attachLun(0, getRamdiskDevice(), blkbuf0, &iniDriveInquiry, nullptr);

	// attach a null device in place of the SD card for now - the SD thread may replace it later
	msd.attachLun(1, (BaseBlockDevice*)&ND1, blkbuf1, &sdCardInquiry, nullptr);

	// start the mass storage thread
	msd.Start();
}

#endif // HAL_USE_USB_MSD
