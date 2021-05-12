/******************************************************************************
* \file             crc.C
*
* \version          1.20
*
* \brief            Cyclic Redundancy Check driver
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
#include "crc/crc.h"

#if (defined(PDL_PERIPHERAL_CRC_ACTIVE))

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
 ** \brief Initialisation of a CRC module.
 **
 ** \param [in]  pstcConfig        CRC module configuration 
 **
 ** \retval Ok                     Initializiation of CRC module successfully 
 **                                done.
 ** \retval ErrorInvalidParameter  pstcConfig == NULL,
 **                                parameter out of range
 ******************************************************************************/
en_result_t Crc_Init(const stc_crc_config_t* pstcConfig)
{
    en_result_t enResult;
    stc_crc_crccr_field_t stcCRCCR; /* local preset of CRCCR */

    /* Check for valid pointers */
    if (NULL == pstcConfig)
    {
        enResult = ErrorInvalidParameter;
    }
    else
    {
        enResult = Ok;
        /* Check CRC mode select and setting */
        switch (pstcConfig->enMode)
        {
            case Crc16:
                stcCRCCR.CRC32 = FALSE;
                /* Mask the initial value for 16 bits */
                FM_CRC->CRCINIT = pstcConfig->u32CrcInitValue & 0x0000FFFFul;
                break;
            case Crc32:
                stcCRCCR.CRC32 = TRUE;
                FM_CRC->CRCINIT = pstcConfig->u32CrcInitValue;
                break;
            default:
                enResult = ErrorInvalidParameter;
                break;
        }

        if (Ok == enResult)
        {
            /* XOR is executed finaly or not */
            if (TRUE == pstcConfig->bFinalXor)
            {
                stcCRCCR.FXOR = TRUE;
            }
            else
            {
                stcCRCCR.FXOR = FALSE;
            }
            /* Bit order for CRC result */
            if (TRUE == pstcConfig->bResultLsbFirst)
            {
                stcCRCCR.CRCLSF = TRUE;
            }
            else
            {
                stcCRCCR.CRCLSF = FALSE;
            }
            /* Byte order for CRC result */
            if (TRUE == pstcConfig->bResultLittleEndian)
            {
                stcCRCCR.CRCLTE = TRUE;
            }
            else
            {
                stcCRCCR.CRCLTE = FALSE;
            }
            /* Bit order for DATA */
            if (TRUE == pstcConfig->bDataLsbFirst)
            {
                stcCRCCR.LSBFST = TRUE;
            }
            else
            {
                stcCRCCR.LSBFST = FALSE;
            }
            /* Byte order for DATA */
            if (TRUE == pstcConfig->bDataLittleEndian)
            {
                stcCRCCR.LTLEND = TRUE;
            }
            else
            {
                stcCRCCR.LTLEND = FALSE;
            }
            /* Initialize CRC configuration */
            stcCRCCR.INIT = TRUE;

            /* Write setting to CRC control register */
            FM_CRC->CRCCR_f = stcCRCCR;
        }
    }

    return (enResult);
} /* Crc_Init */


/**
 ******************************************************************************
 ** \brief De-Initialisation of a CRC module.
 **
 ******************************************************************************/
void Crc_DeInit(void)
{
    /* clear hardware */
    FM_CRC->CRCIN = 0u;
    FM_CRC->CRCCR = 0u;

} /* Crc_DeInit */

/**
 ******************************************************************************
 ** \brief Push 8-bit integer data to a CRC module with if no DMA is used.
 **
 ** \param [in]  u8DataToPush  8-Bit data to be pushed to CRC
 **
 ******************************************************************************/
void Crc_Push8(uint8_t u8DataToPush)
{
    /* Caluculate CRC (Push 8bit data) */
    FM_CRC->CRCINLL = u8DataToPush;

} /* Crc_Push8 */

/**
 ******************************************************************************
 ** \brief Push 16-bit integer data to a CRC module with if no DMA is used.
 **
 ** \note Be careful with the endianess. Byte swapping might have to be
 **       performed before pushing 16-bit data.
 **
 ** \param [in]  u16DataToPush  16-Bit data to be pushed to CRC
 **
 ******************************************************************************/
void Crc_Push16(uint16_t u16DataToPush)
{
    /* Caluculate CRC (Push 16bit data) */
    FM_CRC->CRCINL = u16DataToPush;

} /* Crc_Push16 */

/**
 ******************************************************************************
 ** \brief Push 32-bit integer data to a CRC module with if no DMA is used.
 **
 ** \param [in]  u32DataToPush  32-Bit data to be pushed to CRC
 **
 ******************************************************************************/
void Crc_Push32(uint32_t u32DataToPush)
{
    /* Caluculate CRC (Push 32bit data) */
    FM_CRC->CRCIN = u32DataToPush;

} /* Crc_Push32 */

/**
 ******************************************************************************
 ** \brief Read CRC result register
 **
 ** \note This function returns a 32-bit value regardless of a valid
 **       pointer to the CRC instance anyhow.
 **
 ** \retval value of CRC result
 **
 ******************************************************************************/
uint32_t Crc_ReadResult(void)
{
    /* Return value of CRC result register */
    return (FM_CRC->CRCR);
} /* Crc_ReadResult */


#endif /* #if (defined(PDL_PERIPHERAL_CRC_ACTIVE)) */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
