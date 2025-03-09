#pragma once

#include "hal.h"

#if HAL_USE_USB_MSD
void initUsbMsd();
void attachMsdSdCard(BaseBlockDevice* blkdev);
void deattachMsdSdCard(void);
#endif
