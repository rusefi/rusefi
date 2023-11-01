/************************************************************************************//**
* \file         Source/_template/nvm.c
* \brief        Bootloader non-volatile memory driver source file.
* \ingroup      Target__template_nvm
* \internal
*----------------------------------------------------------------------------------------
*                          C O P Y R I G H T
*----------------------------------------------------------------------------------------
*   Copyright (c) 2019  by Feaser    http://www.feaser.com    All rights reserved
*
*----------------------------------------------------------------------------------------
*                            L I C E N S E
*----------------------------------------------------------------------------------------
* This file is part of OpenBLT. OpenBLT is free software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published by the Free
* Software Foundation, either version 3 of the License, or (at your option) any later
* version.
*
* OpenBLT is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
* PURPOSE. See the GNU General Public License for more details.
*
* You have received a copy of the GNU General Public License along with OpenBLT. It
* should be located in ".\Doc\license.html". If not, contact Feaser to obtain a copy.
*
* \endinternal
****************************************************************************************/

/************************************************************************************//**
* \defgroup   Target__template_nvm Non-volatile memory driver of a port
* \brief      This module implements the non-volatile memory driver of a microcontroller
*             port. Note that the default implementation if for a microcontroller that
*             has internal flash memory. At the time of this writing pretty much all 
*             microcontrollers use flash EEPROM as non-volatile memory to store the
*             program code. Assuming that this is also the case for the microcontroller
*             for which the port is developed, nothing needs to be modified in this
*             source file.
* \ingroup    Target__template
****************************************************************************************/

/****************************************************************************************
* Include files
****************************************************************************************/
#include "boot.h"                                /* bootloader generic header          */
#include "flash.h"

/************************************************************************************//**
** \brief     Initializes the NVM driver.
** \return    none.
**
****************************************************************************************/
void NvmInit(void)
{
  /* init the internal driver */
  FlashInit();
} /*** end of NvmInit ***/


/************************************************************************************//**
** \brief     Reinitializes the NVM driver. This function is called at the start of each
**            firmware update as opposed to NvmInit, which is only called once during
**            power on.
** \return    none.
**
****************************************************************************************/
void NvmReinit(void)
{
  /* reinitialize the internal driver */
  FlashReinit();
} /*** end of NvmReinit ***/


/************************************************************************************//**
** \brief     Programs the non-volatile memory.
** \param     addr Start address.
** \param     len  Length in bytes.
** \param     data Pointer to the data buffer.
** \return    BLT_TRUE if successful, BLT_FALSE otherwise.
**
****************************************************************************************/
blt_bool NvmWrite(blt_addr addr, blt_int32u len, blt_int8u *data)
{
  /* still here so the internal driver should try and perform the program operation */
  return FlashWrite(addr, len, data);
} /*** end of NvmWrite ***/


/************************************************************************************//**
** \brief     Erases the non-volatile memory.
** \param     addr Start address.
** \param     len  Length in bytes.
** \return    BLT_TRUE if successful, BLT_FALSE otherwise.
**
****************************************************************************************/
blt_bool NvmErase(blt_addr addr, blt_int32u len)
{
  /* still here so the internal driver should try and perform the erase operation */
  return FlashErase(addr, len);
} /*** end of NvmErase ***/


/************************************************************************************//**
** \brief     Verifies the checksum, which indicates that a valid user program is
**            present and can be started.
** \return    BLT_TRUE if successful, BLT_FALSE otherwise.
**
****************************************************************************************/
blt_bool NvmVerifyChecksum(void)
{
  /* check checksum using the interally supported method. */
  return FlashVerifyChecksum();
} /*** end of NvmVerifyChecksum ***/


/************************************************************************************//**
** \brief     Obtains the base address of the non-volatile memory available to the user
**            program. This is typically that start of the vector table.
** \return    Base address.
**
****************************************************************************************/
blt_addr NvmGetUserProgBaseAddress(void)
{
  return FlashGetUserProgBaseAddress();
} /*** end of NvmGetUserProgBaseAddress ***/


/************************************************************************************//**
** \brief     Once all erase and programming operations are completed, this
**            function is called, so at the end of the programming session and
**            right before a software reset is performed. It is used to calculate
**            a checksum and program this into flash. This checksum is later used
**            to determine if a valid user program is present in flash.
** \return    BLT_TRUE if successful, BLT_FALSE otherwise.
**
****************************************************************************************/
blt_bool NvmDone(void)
{
  /* compute and write checksum, which is programmed by the internal driver. */
  if (FlashWriteChecksum() == BLT_FALSE)
  {
    return BLT_FALSE;
  }

  /* finish up internal driver operations */
  return FlashDone();
} /*** end of NvmDone ***/


/*********************************** end of nvm.c **************************************/
