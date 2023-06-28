#include "hal.h"
#include "persistence.h"

static const MFSConfig mfscfg_1k = {
  .flashp           = (BaseFlash *)&EFLD1,
  .erased           = 0xFFFFFFFFU,
// 1k page * 1 sector = 1024
  .bank_size        = 1024U,
  .bank0_start      = 62U,
  .bank0_sectors    = 1U,
  .bank1_start      = 63U,
  .bank1_sectors    = 1U
};

static const MFSConfig mfscfg_2k = {
    .flashp           = (BaseFlash *)&EFLD1,
    .erased           = 0xFFFFFFFFU,
    /* 256K flash device with 2K pages
     * use last 8 pages for settings
     * one bank is 8K */
// 2k page * 4 sectors = 8096
    .bank_size        = 8096U,
    .bank0_start      = 120U,
    .bank0_sectors    = 4U,
    .bank1_start      = 124U,
    .bank1_sectors    = 4U
};


static MFSDriver mfs1;
TestConfiguration configuration;

static uint8_t *GetConfigurationPtr() {
    return (uint8_t *)&configuration;
}

static size_t GetConfigurationSize() {
    return sizeof(TestConfiguration);
}

static bool isMfsOkIsh(mfs_error_t state) {
    return state == MFS_NO_ERROR || state == MFS_WARN_REPAIR || state == MFS_WARN_GC;
}

mfs_error_t flashState;

int InitConfiguration() {

    /* Starting EFL driver.*/
    eflStart(&EFLD1, NULL);

    mfsObjectInit(&mfs1);

#define FLASH_SIZE_IN_K_ADDRESS     0x1FFFF7E0
    int flashSize = (*(uint16_t*)FLASH_SIZE_IN_K_ADDRESS);
    if (flashSize > 128) {
        flashState = mfsStart(&mfs1, &mfscfg_2k);
    } else {
        flashState = mfsStart(&mfs1, &mfscfg_1k);
    }

    size_t size = GetConfigurationSize();
    flashState = mfsReadRecord(&mfs1, MFS_CONFIGURATION_RECORD_ID, &size, GetConfigurationPtr());
    if (!isMfsOkIsh(flashState) || size != GetConfigurationSize() || !configuration.IsValid()) {
        /* load defaults */
        configuration.resetToDefaults();
    } else {
        configuration.rebootCounter++;
    }

    return 0;
}

void pokeConfiguration() {
    configuration.updateCounter++;
    flashState = mfsWriteRecord(&mfs1, MFS_CONFIGURATION_RECORD_ID, GetConfigurationSize(), GetConfigurationPtr());
}