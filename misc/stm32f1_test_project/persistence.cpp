#include "hal.h"
#include "persistence.h"

static const MFSConfig mfscfg_1k = {
    .flashp           = (BaseFlash *)&EFLD1,
    .erased           = 0xFFFFFFFFU,
    /* 128K flash device with 1K pages
     * use last 8 pages for settings
     * one bank is 4K */
    .bank_size        = 4096U,
    .bank0_start      = 120U,
    .bank0_sectors    = 4U,
    .bank1_start      = 124U,
    .bank1_sectors    = 4U
#ifdef STM32F103xE
    /* 256K flash device with 2K pages
     * use last 8 pages for settings
     * one bank is 8K */
    .bank_size        = 8096U,
    .bank0_start      = 120U,
    .bank0_sectors    = 4U,
    .bank1_start      = 124U,
    .bank1_sectors    = 4U
#endif
};

static const MFSConfig mfscfg_2k = {
    .flashp           = (BaseFlash *)&EFLD1,
    .erased           = 0xFFFFFFFFU,
    /* 256K flash device with 2K pages
     * use last 8 pages for settings
     * one bank is 8K */
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

int InitConfiguration() {
    size_t size = GetConfigurationSize();

    /* Starting EFL driver.*/
    eflStart(&EFLD1, NULL);

    mfsObjectInit(&mfs1);
    mfs_error_t err;

#define FLASH_SIZE_IN_K_ADDRESS     0x1FFFF7E0
    int flashSize = (*(uint16_t*)FLASH_SIZE_IN_K_ADDRESS);
    if (flashSize > 128) {
        err = mfsStart(&mfs1, &mfscfg_1k);
    } else {
        err = mfsStart(&mfs1, &mfscfg_2k);
    }
    if (err != MFS_NO_ERROR) {
        return -1;
    }

    err = mfsReadRecord(&mfs1, MFS_CONFIGURATION_RECORD_ID, &size, GetConfigurationPtr());
    if ((err != MFS_NO_ERROR) || (size != GetConfigurationSize() || !configuration.IsValid())) {
        /* load defaults */
        configuration.resetToDefaults();
    }

    return 0;
}

void pokeConfiguration() {
    configuration.version++;
//    mfsWriteRecord(&mfs1, MFS_CONFIGURATION_RECORD_ID, GetConfigurationSize(), GetConfigurationPtr());
}