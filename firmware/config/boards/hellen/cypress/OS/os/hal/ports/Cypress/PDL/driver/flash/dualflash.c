/*******************************************************************************
* \file              dualflash.c
* 
* \version           1.20
*
* \brief             dual flash driver
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
#include "flash/dualflash.h"

#if defined(PDL_PERIPHERAL_DUAL_FLASH_ACTIVE)

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/
/*!
 ******************************************************************************
 ** \brief Read a half word data from Flash
 ** \param addr Pointer to read data address
 ******************************************************************************
 */
#define Flash_Read(addr)        *(volatile uint8_t*)((uint32_t)(addr))

/*!
 ******************************************************************************
 ** \brief Wirte a half word data into Flash
 ** \param addr Pointer to read data address
 ** \param data Write data
 ******************************************************************************
 */
#define Flash_Write(addr, data) *(volatile  uint8_t*)((uint32_t)(addr)) = ( uint8_t)(data)

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/
static uint8_t DFlash_CheckToggle(uint8_t* p_sec_addr );
static uint8_t DFlash_ReadResetCmd(uint8_t* pResetSecAddr);

/******************************************************************************/
/* Notes:                                                                     */
/* The feature of Dual Flash is that High bank can erase/write low bank,      */
/* low bank can erase/write high bank.                                        */
/******************************************************************************/

/******************************************************************************/
/* Function implementation - global ('extern') and local ('static')           */
/******************************************************************************/
#if defined ( __ICCARM__ )
#pragma location=".flashcode"
#endif
/*!
 ******************************************************************************
 ** \brief Issue read/reset command
 **
 ** \param pResetSecAddr address of reset sector  
 **
 ** \retval Dummy data
 **
 ******************************************************************************
 */
static uint8_t DFlash_ReadResetCmd(uint8_t* pResetSecAddr)
{
    uint8_t  dummy;

    /*  issue read/reset command    */
    Flash_Write(0x0000u, 0xF0u) ;
    dummy = Flash_Read(pResetSecAddr) ;
    return dummy ;
}
#if defined ( __ICCARM__ )
#pragma location=".flashcode"
#endif
/*!
 ******************************************************************************
 ** \brief Flash chip erase       
 **
 ** \retval Ok                     Chip erase successfully done.
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                                - automatic algorithm of flash memory execution abnormally
 **
 ******************************************************************************
 */
en_result_t DFlash_ChipErase(void) 
{
    en_result_t emRetValue = Ok;

    Flash_Write(0xAA8u, 0xAAu);
    Flash_Write(0x554u, 0x55u);
    Flash_Write(0xAA8u, 0x80u);
    Flash_Write(0xAA8u, 0xAAu);
    Flash_Write(0x554u, 0x55u);
    Flash_Write(0xAA8u, 0x10u);

     /*  if execution result of the automatic algorithm of flash memory is abnormally completed  */
    if( DFLASH_CHK_TOGG_ABNORMAL == DFlash_CheckToggle((uint8_t*)0) )
    {
        /*  sending the read/reset command to the reset sector  */
        DFlash_ReadResetCmd((uint8_t*)0) ;

        /*  return flash operation abnormally   */
        emRetValue  = ErrorInvalidParameter ;
    }
        

    return emRetValue;
}

#if defined ( __ICCARM__ )
#pragma location=".flashcode"
#endif
/*!
 ******************************************************************************
 ** \brief Flash sector erase       
 **
 ** \param pu16SecAddr address of flash sector  
 **
 ** \return Operation status
 ** \retval Ok                     sector erase successfully done.
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                                - automatic algorithm of flash memory execution abnormally
 **
 ******************************************************************************
 */
en_result_t DFlash_SectorErase(uint16_t* pu16SecAddr) 
{
    en_result_t emRetValue = Ok;
    Flash_Write(0xAA8u |((uint32_t)pu16SecAddr&0xffff000u), 0xAAu);
    Flash_Write(0x554u |((uint32_t)pu16SecAddr&0xffff000u), 0x55u);
    Flash_Write(0xAA8u |((uint32_t)pu16SecAddr&0xffff000u), 0x80u);
    Flash_Write(0xAA8u |((uint32_t)pu16SecAddr&0xffff000u), 0xAAu);
    Flash_Write(0x554u |((uint32_t)pu16SecAddr&0xffff000u), 0x55u);
    Flash_Write(pu16SecAddr, 0x30u);

     /*  if execution result of the automatic algorithm of flash memory is abnormally completed  */
    if( DFLASH_CHK_TOGG_ABNORMAL == DFlash_CheckToggle((uint8_t*)pu16SecAddr) )
    {
        /*  sending the read/reset command to the reset sector  */
        DFlash_ReadResetCmd((uint8_t*)pu16SecAddr) ;

        /*  return flash operation abnormally   */
        emRetValue  = ErrorInvalidParameter ;
    }
        
    return emRetValue;
}

#if defined ( __ICCARM__ )
#pragma location=".flashcode"
#endif
/*!
 ******************************************************************************
 ** \brief Flash word write       
 **
 ** \param pu32Addr address of flash data
 ** \param pu32Data pointer to write data
 ** \param u32Size data size, 1 indicates 1 32-bit data
 **
 ** \return Operation status
 ** \retval Ok                     sector erase successfully done.
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                                - automatic algorithm of flash memory execution abnormally
 **
 ******************************************************************************
 */
en_result_t DFlash_WriteData32Bit( uint32_t*  pu32Addr, uint32_t* pu32Data,uint32_t u32Size ) 
{
    uint8_t *pAddr, *pData;
    en_result_t emRetValue = Ok;
    uint8_t  cnt=1;
    pAddr = (uint8_t*)pu32Addr;
    pData = (uint8_t*)pu32Data;  

    for(cnt = u32Size*sizeof(uint32_t);cnt;cnt--)
    {
        /*  issue write command   */
        Flash_Write(0xAA8u |((uint32_t)pu32Addr&0xffff000u), 0xAAu) ;
        Flash_Write(0x554u |((uint32_t)pu32Addr&0xffff000u), 0x55u) ;
        Flash_Write(0xAA8u |((uint32_t)pu32Addr&0xffff000u), 0xA0u) ;
        Flash_Write((uint32_t)pAddr, (uint16_t)*pData);
        
        /*  execution result of the automatic algorithm of flash memory is abnormally complete or verify error  */
        if(( DFLASH_CHK_TOGG_ABNORMAL == DFlash_CheckToggle(pAddr) ) ||
          ( Flash_Read((uint32_t)pAddr) != (uint16_t)*pData))
        {
            /*  issue read/reset command to reset sector    */
            DFlash_ReadResetCmd(pAddr) ;

            /*  return flash operation abnormally   */
            emRetValue  = ErrorInvalidParameter ;
        }      
        /* Prepare next h-word write */
        pAddr++;
        pData++; 
    }

    return emRetValue ;
}

#if defined ( __ICCARM__ )
#pragma location=".flashcode"
#endif
/*!
 ******************************************************************************
 ** \brief Flash half word write       
 **
 ** \param pu16Addr address of flash data
 ** \param pu16Data pointer to write data
 ** \param u32Size data size, 1 indicates 1 16-bit data
 **
 ** \return Operation status
 ** \retval Ok                     sector erase successfully done.
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                                - automatic algorithm of flash memory execution abnormally
 **
 ******************************************************************************
 */
en_result_t DFlash_WriteData16Bit( uint16_t*  pu16Addr, uint16_t* pu16Data, uint32_t u32Size ) 
{
    uint8_t *pAddr, *pData;
    en_result_t emRetValue = Ok;
    uint8_t  cnt=1;
    pAddr = (uint8_t*)pu16Addr;
    pData = (uint8_t*)pu16Data;
    for(cnt = u32Size*sizeof(uint16_t);cnt;cnt--)
    {
        /*  issue write command   */
        Flash_Write(0xAA8u |((uint32_t)pu16Addr&0xffff000u), 0xAAu) ;
        Flash_Write(0x554u |((uint32_t)pu16Addr&0xffff000u), 0x55u) ;
        Flash_Write(0xAA8u |((uint32_t)pu16Addr&0xffff000u), 0xA0u) ;
        Flash_Write((uint32_t)pAddr, (uint16_t)*pData);
        
        /*  execution result of the automatic algorithm of flash memory is abnormally complete or verify error  */
        if(( DFLASH_CHK_TOGG_ABNORMAL == DFlash_CheckToggle(pAddr) ) ||
          ( Flash_Read((uint32_t)pAddr) != (uint16_t)*pData))
        {
            /*  issue read/reset command to reset sector    */
            DFlash_ReadResetCmd(pAddr) ;

            /*  return flash operation abnormally   */
            emRetValue  = ErrorInvalidParameter ;
        }        
        /* Prepare next h-word write */
        pAddr++;
        pData++; 
    }


    return emRetValue ;
}

#if defined ( __ICCARM__ )
#pragma location=".flashcode"
#endif
/*!
 ******************************************************************************
 ** \brief Flash byte write       
 **
 ** \param pu8Addr address of flash data
 ** \param pu8Data pointer to write data
 ** \param u32Size data size, 1 indicates 1 8-bit data
 **
 ** \return Operation status
 ** \retval Ok                     sector erase successfully done.
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                                - automatic algorithm of flash memory execution abnormally
 **
 ******************************************************************************
 */
en_result_t DFlash_WriteData8Bit( uint8_t*  pu8Addr, uint8_t* pu8Data, uint32_t u32Size ) 
{
    en_result_t emRetValue = Ok;
    uint8_t  cnt=1;
    for(cnt = u32Size;cnt;cnt--)
    {
        /*  issue write command   */
        Flash_Write(0xAA8u |((uint32_t)pu8Addr&0xffff000u), 0xAAu) ;
        Flash_Write(0x554u |((uint32_t)pu8Addr&0xffff000u), 0x55u) ;
        Flash_Write(0xAA8u |((uint32_t)pu8Addr&0xffff000u), 0xA0u) ;
        Flash_Write((uint32_t)pu8Addr, (uint16_t)*pu8Data);
        
        /*  execution result of the automatic algorithm of flash memory is abnormally complete or verify error  */
        if(( DFLASH_CHK_TOGG_ABNORMAL == DFlash_CheckToggle(pu8Addr) ) ||
          ( Flash_Read((uint32_t)pu8Addr) != (uint16_t)*pu8Data))
        {
            /*  issue read/reset command to reset sector    */
            DFlash_ReadResetCmd(pu8Addr) ;

            /*  return flash operation abnormally   */
            emRetValue  = ErrorInvalidParameter ;
        }        
        /* Prepare next h-word write */
        pu8Addr++;
        pu8Data++; 
    }


    return emRetValue ;
}
#if defined ( __ICCARM__ )
#pragma location=".flashcode"
#endif
/*!
 ******************************************************************************
 ** \brief automatic algorithm of flash memory execution 
 **
 ** \param pAddr address of flash data
 **
 ** \return Operation status
 ** \retval DFLASH_RET_OK
 ** \retval DFLASH_RET_ABNORMAL
 ** \retval DFLASH_RET_INVALID_PARA
 **
 ******************************************************************************
 */
static uint8_t DFlash_CheckToggle( uint8_t* pAddr )
{
    uint8_t   usSequenceFlag1, usSequenceFlag2 ;  /*  hardware sequence flag */
    uint8_t   ucRetValue  = DFLASH_CHK_TOGG_NORMAL ;

    /* set hardware sequence flag */
    usSequenceFlag1 = Flash_Read(pAddr) ;
    usSequenceFlag2 = Flash_Read(pAddr) ;
    /*  if automatic algorithm is executing */
    while(DFLASH_CHK_TOGG_MASK == (( usSequenceFlag1 ^ usSequenceFlag2) & DFLASH_CHK_TOGG_MASK))
    {
        /*  if exceeds the timing limit */
        if(DFLASH_CHK_TLOV_MASK == ( usSequenceFlag1 & DFLASH_CHK_TLOV_MASK))
        {
            /* set hardware sequence flag */
            usSequenceFlag1 = Flash_Read(pAddr) ;
            usSequenceFlag2 = Flash_Read(pAddr) ;

            /*  if automatic algorithm is executing */
            if(DFLASH_CHK_TOGG_MASK == (( usSequenceFlag1 ^ usSequenceFlag2) & DFLASH_CHK_TOGG_MASK))
            {
                /*  abnormally complete */
                ucRetValue  = DFLASH_CHK_TOGG_ABNORMAL ;

                break;
            }
        }

        /* set hardware sequence flag */
        usSequenceFlag1 = Flash_Read(pAddr) ;
        usSequenceFlag2 = Flash_Read(pAddr) ;
    }

    return ucRetValue ;
}

#endif

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
