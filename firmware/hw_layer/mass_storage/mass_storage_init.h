#pragma once

#include "hal.h"

#if HAL_USE_USB_MSD
void initUsbMsd();
void attachMsdSdCard(BaseBlockDevice* blkdev, uint8_t *blkbuf, size_t blkbufsize);
void deattachMsdSdCard(void);
#endif

const unsigned char *getStorageImage();
size_t getStorageImageSize();
