/*******************************************************************************
* \file             uid.c
* 
* \version          1.20
*
* \brief            This file provides the source code to the API for the UID 
*                   driver.
*
********************************************************************************
* \copyright
* Copyright 2016, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
* CYPRESS PROVIDES THIS SOFTWARE "AS IS" AND MAKES NO WARRANTY
* OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS SOFTWARE,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
* PURPOSE.
*******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "uid/uid.h"

#if (defined(PDL_PERIPHERAL_UID_ACTIVE))

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/

/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/

/******************************************************************************/
/* Function implementation - global ('extern') and local ('static')           */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief Read Unique ID registers as is
 **
 ** This function reads out UIDR0 and UIDR1 as is without any shift to
 ** a pointered structure of the type stc_unique_id_t.
 ** Reserved bits are masked to '0'.
 **
 ** \param  [out] pstcUniqueId     Pointer to the Unique ID structure
 **
 ** \retval Ok                     Unique ID successfully written
 ** \retval ErrorInvalidParameter  pstcUniqueId == NULL
 ******************************************************************************/
en_result_t Uid_ReadUniqueId(stc_unique_id_t *pstcUniqueId)
{
  if (NULL == pstcUniqueId)
  {
    return ErrorInvalidParameter;
  }
  
  pstcUniqueId->u32Uidr0 = (0xFFFFFFF0ul & (FM_UNIQUE_ID->UIDR0));
  pstcUniqueId->u32Uidr1 = (0x00001FFFul & (FM_UNIQUE_ID->UIDR1));
  
  return Ok;
}
   
/**
 ******************************************************************************
 ** \brief Read Unique ID registers 0 and shifts it by 4 (LSB aligned)
 **
 ** This function reads out UIDR0 and aligns the value to the LSB by shifting
 ** by 4.
 **
 ** \return uint32_t                  Unique ID 0 >> 4
 ******************************************************************************/
uint32_t Uid_ReadUniqueId0(void)
{
  return ((FM_UNIQUE_ID->UIDR0) >> 4u);
}

/**
 ******************************************************************************
 ** \brief Read Unique ID registers 1 and masks the upper 19 bits to '0'
 **
 ** This function reads out UIDR1 and masks the upper 19 bits to '0'
 **
 ** \return uint32_t                  Unique ID 1 & 0x00001FFF
 ******************************************************************************/
uint32_t Uid_ReadUniqueId1(void)
{
  return ((FM_UNIQUE_ID->UIDR1) & 0x00001FFFul);
}

/**
 ******************************************************************************
 ** \brief Read Unique ID registers 0 and 1 and merge it LSB aligned to a 64 bit
 ** value
 **
 ** \return uint64_t                  Complete unique ID LSB aligned
 ******************************************************************************/
uint64_t Uid_ReadUniqueId64(void)
{
  uint64_t u64UniqueIdMsw;
  
  // Left shift by 60 bit in two steps
  u64UniqueIdMsw = (((FM_UNIQUE_ID->UIDR1) & 0x00001FFFul) << 16ul);
  u64UniqueIdMsw <<= 12ul;
  
  u64UniqueIdMsw |= ((FM_UNIQUE_ID->UIDR0) >> 4ul);
  return u64UniqueIdMsw;
}

#endif // #if (defined(PDL_PERIPHERAL_UID_ACTIVE))

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
