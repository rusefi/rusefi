#pragma once

#include "hal.h"

#if defined(HAL_USE_USB_MSD) && HAL_USE_USB_MSD
void initUsbMsd();
void attachMsdSdCard(BaseBlockDevice* blkdev, uint8_t *blkbuf, size_t blkbufsize);
void deattachMsdSdCard(void);
// 'sdinfo' console command support: MSD command/failure counters and SD I/O error counts
void printMsdDiagnostics();
#endif

const unsigned char *getStorageImage();
size_t getStorageImageSize();
