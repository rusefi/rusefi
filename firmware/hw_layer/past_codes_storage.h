#pragma once

#include "ff.h"

// Call only from the MMC thread, with the filesystem mounted and board SD
// ownership held. Loads history once and writes dirty snapshots, retrying
// recoverable errors internally. A non-FR_OK result means close failed: the
// descriptor is still live and the caller must stop SD work without unmounting.
FRESULT sdServicePastCodes();
