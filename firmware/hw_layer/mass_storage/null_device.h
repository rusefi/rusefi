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

#if HAL_USE_USB_MSD
void msdMountNullDevice(USBMassStorageDriver* msdp, USBDriver *usbp, uint8_t* blkbuf, const scsi_inquiry_response_t* inquiry);
#endif
