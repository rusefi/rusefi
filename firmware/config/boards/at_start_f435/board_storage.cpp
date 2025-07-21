/**
 * @file boards/subaru_eg33/board_storage.h
 *
 * @brief Storage configuration file
 *
 * @date Nov 03, 2023
 * @author Andrey Gusakov, 2023
 */

#include "pch.h"

/* This board stores settings in internal flash bank 2 */
#if EFI_STORAGE_MFS == TRUE

#include "hal_mfs.h"

/* use 128K + 128K on bank 2 for settings */
const MFSConfig mfscfg1 = {
  .flashp           = (BaseFlash *)&EFLD2,
  .erased           = 0xFFFFFFFFU,
  .bank_size        = 128U * 1024U,
  .bank0_start      = 0U,
  .bank0_sectors    = 32U,
  .bank1_start      = 32U,
  .bank1_sectors    = 32U
};

bool boardInitMfs()
{
  /* Starting EFL driver.*/
  eflStart(&EFLD2, NULL);

  return true;
}

const MFSConfig *boardGetMfsConfig()
{
	return &mfscfg1;
}

#endif /* EFI_STORAGE_MFS == TRUE */
