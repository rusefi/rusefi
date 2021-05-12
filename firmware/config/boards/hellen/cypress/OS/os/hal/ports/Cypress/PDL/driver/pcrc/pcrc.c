/*******************************************************************************
* \file             pcrc.c
* 
* \version          1.20
*
* \brief            This file provides the source code to the API for the 
*                   Programmable CRC driver.
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
#include "pcrc/pcrc.h"

#if (defined(PDL_PERIPHERAL_PCRC_ACTIVE))

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
#if (PDL_INTERRUPT_ENABLE_PCRC == PDL_ON)
/**
 ******************************************************************************
 ** \brief Device dependent initialization of interrupts according CMSIS with
 **        level defined in pdl_user.h
 **
 ******************************************************************************/
static void PcrcInitNvic(void); 

/**
 ******************************************************************************
 ** \brief Device dependent de-initialization of interrupts according CMSIS 
 **
 ******************************************************************************/
static void PcrcDeInitNvic(void); 
#endif

/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/
#if (PDL_INTERRUPT_ENABLE_PCRC == PDL_ON)
static func_ptr_t m_pfnPcrcIrqCb = NULL;
#endif

/******************************************************************************/
/* Function implementation - global ('extern') and local ('static')           */
/******************************************************************************/

#if (PDL_INTERRUPT_ENABLE_PCRC == PDL_ON)
/**
 ******************************************************************************
 ** \brief Device dependent initialization of interrupts according CMSIS with
 **        level defined in pdl.h
 **
 ******************************************************************************/
static void PcrcInitNvic(void)
{    
    NVIC_ClearPendingIRQ(PCRC_I2S0_1_IRQn);
    NVIC_EnableIRQ(PCRC_I2S0_1_IRQn);
    NVIC_SetPriority(PCRC_I2S0_1_IRQn, PDL_IRQ_LEVEL_I2S_PCRC);

    return;
} 

/**
 ******************************************************************************
 ** \brief Device dependent de-initialization of interrupts according CMSIS 
 **
 ******************************************************************************/
static void PcrcDeInitNvic(void)
{ 
    NVIC_ClearPendingIRQ(PCRC_I2S0_1_IRQn);
    NVIC_DisableIRQ(PCRC_I2S0_1_IRQn);
    NVIC_SetPriority(PCRC_I2S0_1_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);

    return;
} 

/**
 ******************************************************************************
 ** \brief ISR of the PCRC module
 ** 
 ******************************************************************************/
void PcrcIrqHandler(void)
{    
    if(FM_PCRC->PCRC_CFG_f.CIRQ)
    {
        FM_PCRC->PCRC_CFG_f.CIRQCLR = 1u;
        if(NULL != m_pfnPcrcIrqCb)
        {
            m_pfnPcrcIrqCb(); 
        }
    }

    return;
}
#endif

/**
 ******************************************************************************
 ** \brief Initialisation of a Programmable CRC module.
 **
 ** \param pstcConfig    CRC module configuration 
 **
 ** \retval Ok                 Initializiation of Programmable CRC module successfully 
 **                            done.
 ** \retval ErrorInvalidParameter  If one of following cases match:
 **                                - pstcConfig == NULL,
 **                                - parameter out of range
 ******************************************************************************/
en_result_t Pcrc_Init(const stc_pcrc_config_t* pstcConfig)
{  
    en_result_t enResult;
    stc_pcrc_pcrc_cfg_field_t PCRC_CFG_f;

    PDL_ZERO_STRUCT(PCRC_CFG_f);

    /* Check for valid pointers and configrue parameters */
    if ((NULL == pstcConfig) ||
        (pstcConfig->enInputFormat > LsbFirstLittleEndian) ||
        (pstcConfig->enOutputFormat > LsbFirstLittleEndian) ||
        (pstcConfig->enInputDataSize > InputData32Bit))
    {
        enResult = ErrorInvalidParameter;
    }
    else
    {
        PCRC_CFG_f.FI = pstcConfig->enInputFormat;
        PCRC_CFG_f.FO = pstcConfig->enOutputFormat;
        PCRC_CFG_f.SZ = pstcConfig->enInputDataSize;
        PCRC_CFG_f.TEST = 0x20u;              //Always writes "100000" to perform write access to this register.

        /* Update hardware */
        FM_PCRC->PCRC_POLY = pstcConfig->u32GeneratorPolynomial;
        FM_PCRC->PCRC_SEED = pstcConfig->u32CrcInitValue;
        FM_PCRC->PCRC_FXOR = pstcConfig->u32FinalXorValue;
        FM_PCRC->PCRC_CFG_f = PCRC_CFG_f;
        FM_PCRC->PCRC_CFG_f.CDEN = ((pstcConfig->bUseDstc == TRUE) ? 1u : 0u);

#if (PDL_INTERRUPT_ENABLE_PCRC== PDL_ON)
        m_pfnPcrcIrqCb = pstcConfig->pfnIrqCb;
        FM_PCRC->PCRC_CFG_f.CIRQCLR = 1u;
        FM_PCRC->PCRC_CFG_f.CIEN = ((pstcConfig->bIrqEn == TRUE) ? 1u : 0u);
         // Initialize NVIC
        if(TRUE == pstcConfig->bTouchNvic)
        {
            PcrcInitNvic();
        }
#endif        
        enResult = Ok;
    }

    return (enResult);
}

/**
 ******************************************************************************
 ** \brief Re-Initialisation of a Programmable CRC module.
 ******************************************************************************/
void Pcrc_DeInit(void)
{
#if (PDL_INTERRUPT_ENABLE_PCRC == PDL_ON)
    PcrcDeInitNvic();
#endif
    /* clear hardware */
    FM_PCRC->PCRC_WR = 0u;
    FM_PCRC->PCRC_POLY = 0u;
    FM_PCRC->PCRC_SEED = 0u;
    FM_PCRC->PCRC_FXOR = 0u;
    FM_PCRC->PCRC_CFG = 0u;

    return;
} 

/**
 ******************************************************************************
 ** \brief Set CRC computing initial value.
 **
 ** \param  u32InitialValue: CRC initial value
 ** 
 ******************************************************************************/
void Pcrc_SetInitialValue(uint32_t u32InitialValue)
{
    FM_PCRC->PCRC_SEED = u32InitialValue;

    return;
}

/**
 ******************************************************************************
 ** \brief Set CRC computing result XOR value.
 **
 ** \param  u32FinalXorValue:  result XOR value
 ** 
 ******************************************************************************/
void Pcrc_SetFinalXorValue(uint32_t u32FinalXorValue)
{
    FM_PCRC->PCRC_FXOR = u32FinalXorValue;

    return;
}

/**
 ******************************************************************************
 ** \brief Set CRC output data Format.
 **
 ** \param  enOutputDataFormat:  CRC output data Format
 ** 
 ******************************************************************************/
void Pcrc_SetOutputDataFormat(en_crc_data_format_t enOutputDataFormat)
{
    FM_PCRC->PCRC_CFG_f.FO = enOutputDataFormat;

    return;
} 

/**
 ******************************************************************************
 ** \brief Set CRC input data Format.
 **
 ** \param  enInputDataFormat:  CRC input data Format
 ** 
 ******************************************************************************/
void Pcrc_SetInputDataFormat(en_crc_data_format_t enInputDataFormat)
{
    FM_PCRC->PCRC_CFG_f.FI = enInputDataFormat;

    return;
} 

/**
 ******************************************************************************
 ** \brief Set CRC input data size.
 **
 ** \param  enInputDataSize:  CRC input data size
 ** 
 ******************************************************************************/
void Pcrc_SetInputDataSize(en_crc_input_data_size_t enInputDataSize)
{
    FM_PCRC->PCRC_CFG_f.SZ = enInputDataSize;

    return;
} 

/**
 ******************************************************************************
 ** \brief Set  TEST field of CRC Computing Configuration Register 
 **
 ** \param  u8CfgTest:  TEST field value. 
 **                                      The correct operation is not achieved if perform writing values other 
 **                                      than "100000" to the TEST[5:0] register
 ** 
 ******************************************************************************/
void Pcrc_SetCfgTest(uint8_t u8CfgTest)
{
    FM_PCRC->PCRC_CFG_f.TEST = u8CfgTest;

    return;
} 

/**
 ******************************************************************************
 ** \brief Get CRC interrupt request flag.
 **
 ** \retval PRGCRC Interrupt request flag
 ** 
 ******************************************************************************/
uint8_t Pcrc_GetIntRequestFlag(void)
{
    return (FM_PCRC->PCRC_CFG_f.CIRQ);
} 

/**
 ******************************************************************************
 ** \brief Enable CRC interrupt request.
 ******************************************************************************/
void Pcrc_EnableIntRequest(void)
{
    FM_PCRC->PCRC_CFG_f.CIEN = 1u;

    return;
} 

/**
 ******************************************************************************
 ** \brief Disable CRC interrupt request.
 ******************************************************************************/
void Pcrc_DisableIntRequest(void)
{
    FM_PCRC->PCRC_CFG_f.CIEN = 0u;

    return;
}

/**
 ******************************************************************************
 ** \brief Allows the transfer request of input data for DMA.
 ******************************************************************************/
void Pcrc_EnableDmaTx(void)
{
    FM_PCRC->PCRC_CFG_f.CDEN = 1u;

    return;
} 

/**
 ******************************************************************************
 ** \brief Disable the transfer request of input data for DMA.
 ******************************************************************************/
void Pcrc_DisableDmaTx(void)
{
    FM_PCRC->PCRC_CFG_f.CDEN = 0u;

    return;
} 

/**
 ******************************************************************************
 ** \brief Get CRC computing unit state.
 **
 ** \retval  1 : Indicates the CRC computing unit is in busy state
 ** \retval  0 : Indicates the CRC computing unit is in idle state.
 ** 
 ******************************************************************************/
uint8_t Pcrc_GetLockStatus(void)
{
    return ((FM_PCRC->PCRC_CFG_f.LOCK == 1u) ? 1u : 0u);
} 

/**
 ******************************************************************************
 ** \brief Clear RGCRC Interrupt request.
 ******************************************************************************/
void Pcrc_ClrIntRequest(void)
{
    /* Caluculate CRC (Push 8bit data) */
    FM_PCRC->PCRC_CFG_f.CIRQCLR = 1u;

    return;
} 

/**
 ******************************************************************************
 ** \brief Write computing input data.
 **
 ** \param  u32WriteData: input data value
 **
 ******************************************************************************/
void Pcrc_WriteData(uint32_t u32WriteData)
{
    FM_PCRC->PCRC_WR = u32WriteData;

    return;
}

/**
 ******************************************************************************
 ** \brief Get computing output data.
 **
 ** \retval output data value
 ** 
 ******************************************************************************/
uint32_t Pcrc_ReadResult(void)
{
    return (FM_PCRC->PCRC_RD);
}

#endif /* #if (defined(PDL_PERIPHERAL_PCRC_ACTIVE)) */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
