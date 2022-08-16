/******************************************************************************
* \file         hbif.c
*
* \version      1.20
*
* \brief        Hyper Bus Interface driver
*
******************************************************************************* 
\copyright
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


/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "hbif/hbif.h"

#if (defined(PDL_PERIPHERAL_HBIF_ACTIVE))

/*****************************************************************************/
/* Local pre-processor symbols/macros ('define')                             */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/
func_ptr_t pfnHbifIrqCb;
/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/
/**
 *****************************************************************************
 ** This function initialises one specific HBIF module with the parameters
 ** provided in the given configuration structure.
 ** After successful initialization the HBIF module is ready to use.
 **
 ** Hbif_Init() has to be called with the parameter pstcConfig of type
 ** stc_hbif_config_t.
 **
 ** All values in pstcConfig have to be in valid range (see hbif.h for allowed
 ** ranges of dedicated parameters). 
 **
 ** To reset and disable the HBIF module the function Hbif_DeInit() has to 
 ** be used.
 **
 ** \param [in] pstcConfig      Pointer to HBIF configuration parameters
 **
 ** \return - Ok on successful init
 **         - ErrorInvalidParameter if pointers are NULL
 *****************************************************************************/
en_result_t Hbif_Init(const stc_hbif_config_t* pstcConfig)
{
    if(NULL == pstcConfig)
    {
        return ErrorInvalidParameter;
    }

    // Configure memory space 0
    if (NULL != pstcConfig->pstcMem0Config)
    {   
        // Set continuous read merging Option
        FM_HYPERBUS->MCR0_f.CRMO = pstcConfig->pstcMem0Config->bMergeContinuousRead;
        
        // Set asymmetry cache system support
        FM_HYPERBUS->MCR0_f.ACS  = pstcConfig->pstcMem0Config->bSupportAsymmetryCache;
        
        // Access to memory or register
        FM_HYPERBUS->MCR0_f.CRT  = pstcConfig->pstcMem0Config->bRegisterSpace;
        
        // Set device type
        FM_HYPERBUS->MCR0_f.DEVTYPE = pstcConfig->pstcMem0Config->bHyperRamDevice;
        
        // Set wrap size
        switch (pstcConfig->pstcMem0Config->enWrapBustsize)
        {
            case HbifWrap64Bytes:
            case HbifWrap16Bytes:
            case HbifWrap32Bytes:
                FM_HYPERBUS->MCR0_f.WRAPSIZE = pstcConfig->pstcMem0Config->enWrapBustsize;
                break;
            default:
                return ErrorInvalidParameter;
        }
        
        // Set read chip select high cycle between operations
        if (pstcConfig->pstcMem0Config->enReadCsHighCycle > HbifCshi165Clk)
        {
            return ErrorInvalidParameter;
        }
        
        FM_HYPERBUS->MTR0_f.RCSHI = pstcConfig->pstcMem0Config->enReadCsHighCycle;
        
        // Set write chip select high cycle between operations
        if (pstcConfig->pstcMem0Config->enWriteCsHighCycle > HbifCshi165Clk)
        {
            return ErrorInvalidParameter;
        }
        
        FM_HYPERBUS->MTR0_f.WCSHI = pstcConfig->pstcMem0Config->enWriteCsHighCycle;
        
        // Set read Chip select setup cycle to next CK rising edge
        if (pstcConfig->pstcMem0Config->enReadCsNextClkCycle > HbifCcs16Clk)
        {
            return ErrorInvalidParameter;
        }
        
        FM_HYPERBUS->MTR0_f.RCSS = pstcConfig->pstcMem0Config->enReadCsNextClkCycle;
        
        // Set write Chip select setup cycle to next CK rising edge
        if (pstcConfig->pstcMem0Config->enWriteCsNextClkCycle > HbifCcs16Clk)
        {
            return ErrorInvalidParameter;
        }
        
        FM_HYPERBUS->MTR0_f.WCSS = pstcConfig->pstcMem0Config->enWriteCsNextClkCycle;
        
        // Set read chip select hold after CK falling Edge
        if (pstcConfig->pstcMem0Config->enReadCsHoldCycle > HbifCsh16Clk)
        {
            return ErrorInvalidParameter;
        }
        
        FM_HYPERBUS->MTR0_f.RCSH = pstcConfig->pstcMem0Config->enReadCsHoldCycle;
        
        // Set write chip select hold after CK falling Edge
        if (pstcConfig->pstcMem0Config->enWriteCsHoldCycle > HbifCsh16Clk)
        {
            return ErrorInvalidParameter;
        }
        
        FM_HYPERBUS->MTR0_f.WCSH = pstcConfig->pstcMem0Config->enWriteCsHoldCycle;
        
        // Set latency cycle for HyperRAM mode
        switch (pstcConfig->pstcMem0Config->enHyperRamLatencyCycle)
        {
            case HbRamLatency4Clk:
                FM_HYPERBUS->MTR0_f.LTCY = 15u;
                break;
            case HbRamLatency5Clk:
                FM_HYPERBUS->MTR0_f.LTCY = 0u;
                break;
            case HbRamLatency6Clk:
                FM_HYPERBUS->MTR0_f.LTCY = 1u;
                break;
            default:
                return ErrorInvalidParameter;
        }
        
        // Set base address of memory space 0
        FM_HYPERBUS->MBR0 = pstcConfig->pstcMem0Config->u32MemBaseAddress ;  
    }
    
    // Configure memory space 1
    if (NULL != pstcConfig->pstcMem1Config)
    {   
        // Set base address of memory space 1
        FM_HYPERBUS->MBR1 = pstcConfig->pstcMem1Config->u32MemBaseAddress ;  
        
        // Set continuous read merging Option
        FM_HYPERBUS->MCR1_f.CRMO = pstcConfig->pstcMem1Config->bMergeContinuousRead;
        
        // Set asymmetry cache system support
        FM_HYPERBUS->MCR1_f.ACS  = pstcConfig->pstcMem1Config->bSupportAsymmetryCache;
        
        // Access to memory or register
        FM_HYPERBUS->MCR1_f.CRT  = pstcConfig->pstcMem1Config->bRegisterSpace;
        
        // Set device type
        FM_HYPERBUS->MCR1_f.DEVTYPE = pstcConfig->pstcMem1Config->bHyperRamDevice;
        
        // Set wrap size
        switch (pstcConfig->pstcMem1Config->enWrapBustsize)
        {
            case HbifWrap64Bytes:
            case HbifWrap16Bytes:
            case HbifWrap32Bytes:
                FM_HYPERBUS->MCR1_f.WRAPSIZE = pstcConfig->pstcMem1Config->enWrapBustsize;
                break;
            default:
                return ErrorInvalidParameter;
        }
        
        // Set read chip select high cycle between operations
        if (pstcConfig->pstcMem1Config->enReadCsHighCycle > HbifCshi165Clk)
        {
            return ErrorInvalidParameter;
        }
        
        FM_HYPERBUS->MTR1_f.RCSHI = pstcConfig->pstcMem1Config->enReadCsHighCycle;
        
        // Set write chip select high cycle between operations
        if (pstcConfig->pstcMem1Config->enWriteCsHighCycle > HbifCshi165Clk)
        {
            return ErrorInvalidParameter;
        }
        
        FM_HYPERBUS->MTR1_f.WCSHI = pstcConfig->pstcMem1Config->enWriteCsHighCycle;
        
        // Set read Chip select setup cycle to next CK rising edge
        if (pstcConfig->pstcMem1Config->enReadCsNextClkCycle > HbifCcs16Clk)
        {
            return ErrorInvalidParameter;
        }
        
        FM_HYPERBUS->MTR1_f.RCSS = pstcConfig->pstcMem1Config->enReadCsNextClkCycle;
        
        // Set write Chip select setup cycle to next CK rising edge
        if (pstcConfig->pstcMem1Config->enWriteCsNextClkCycle > HbifCcs16Clk)
        {
            return ErrorInvalidParameter;
        }
        
        FM_HYPERBUS->MTR1_f.WCSS = pstcConfig->pstcMem1Config->enWriteCsNextClkCycle;
        
        // Set read chip select hold after CK falling Edge
        if (pstcConfig->pstcMem1Config->enReadCsHoldCycle > HbifCsh16Clk)
        {
            return ErrorInvalidParameter;
        }
        
        FM_HYPERBUS->MTR1_f.RCSH = pstcConfig->pstcMem1Config->enReadCsHoldCycle;
        
        // Set write chip select hold after CK falling Edge
        if (pstcConfig->pstcMem1Config->enWriteCsHoldCycle > HbifCsh16Clk)
        {
            return ErrorInvalidParameter;
        }
        
        FM_HYPERBUS->MTR1_f.WCSH = pstcConfig->pstcMem1Config->enWriteCsHoldCycle;
        
        // Set latency cycle for HyperRAM mode
        switch (pstcConfig->pstcMem1Config->enHyperRamLatencyCycle)
        {
            case HbRamLatency4Clk:
                FM_HYPERBUS->MTR1_f.LTCY = 15u;
                break;
            case HbRamLatency5Clk:
                FM_HYPERBUS->MTR1_f.LTCY = 0u;
                break;
            case HbRamLatency6Clk:
                FM_HYPERBUS->MTR1_f.LTCY = 1u;
                break;
            default:
                return ErrorInvalidParameter;
        }
    }

    // Disable write protection
    FM_HYPERBUS->WPR_f.WP = 0u;
    
    // Pull GPO pin to low
    FM_HYPERBUS->GPOR = 0u;
    
#if (PDL_INTERRUPT_ENABLE_HBIF == PDL_ON) 
    pfnHbifIrqCb = pstcConfig->pfnIrqCb;
    FM_HYPERBUS->IEN_f.INTP = pstcConfig->bInterruptPolarity;
    FM_HYPERBUS->IEN_f.RPCINTE = pstcConfig->bEnableInterrupt;
    
    if (TRUE == pstcConfig->bTouchNvic)
    {
        NVIC_ClearPendingIRQ(GDC_HYPERBUS_IRQn);
        NVIC_EnableIRQ(GDC_HYPERBUS_IRQn);
        NVIC_SetPriority(GDC_HYPERBUS_IRQn, PDL_IRQ_LEVEL_HBIF);  
    }
#endif    
    
    return Ok ;    
} // Hbif_Init

/**
 *****************************************************************************
 ** \brief Deinitializes the HBIF module.
 **
 ** Any pending transmission or reception will be aborted and all HBIF related
 ** registers are reset to their default values.
 **
 ** \param [in] bTouchNvic        TRUE = De-Init NVIC
 **
 ** \retval Ok                    HBIF module has been successfully deinitialized
 *****************************************************************************/
en_result_t Hbif_DeInit(boolean_t bTouchNvic)
{
    // Reset all HBIF registers
    FM_HYPERBUS->IEN = 0u;
    FM_HYPERBUS->MBR0 = 0u;
    FM_HYPERBUS->MBR1 = 0u;
    FM_HYPERBUS->MCR0 = 0u;
    FM_HYPERBUS->MCR1 = 0u;
    FM_HYPERBUS->MTR0 = 0u;
    FM_HYPERBUS->MTR1 = 0u;
    FM_HYPERBUS->GPOR = 0u;
    FM_HYPERBUS->WPR = 0u;
    
#if (PDL_INTERRUPT_ENABLE_HBIF == PDL_ON) 
    if(TRUE == bTouchNvic)
    {
        NVIC_ClearPendingIRQ(GDC_HYPERBUS_IRQn);
        NVIC_DisableIRQ(GDC_HYPERBUS_IRQn);
        NVIC_SetPriority(GDC_HYPERBUS_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);   
    }
#endif    
    
    return Ok ;  
} // Hbif_DeInit


/**
 ******************************************************************************
 ** \brief Get status of HBIF according to status type
 **
 ** \param [in] enStatus     HBIF status type
 ** \arg   HbifRstoDuringWrite            RSTO Error in Write Transaction
 ** \arg   HbifTransactionErrDuringWrite  Transaction Error in Write Transaction
 ** \arg   HbifDecodeErrDuringWrite       Decode Error in Write Transaction
 ** \arg   HbifWriteActive                Write Transaction Active
 ** \arg   HbifRdsStallDuringRead         RDS Stall Error in Read Transaction
 ** \arg   HbifRstoDuringRead             RSTO Error in Read Transaction
 ** \arg   HbifTransactionErrDuringRead   Transaction Error in Read Transaction
 ** \arg   HbifDecodeErrDuringRead        Decode Error in Read Transaction
 ** \arg   HbifReadActive                 Read Transaction Active
 ** 
 ** \retval FALSE                 If one of following conditions are met:
 **                               - RSTO Normal operation during write [enStatus = HbifRstoDuringWrite]
 **                               - Normal operation during write [enStatus = HbifTransactionErrDuringWrite]
 **                               - Decode normally [enStatus = HbifDecodeErrDuringWrite]
 **                               - Write transaction is idle [enStatus = HbifWriteActive]
 **                               - No RDS stall error [enStatus = HbifRdsStallDuringRead]
 **                               - RSTO Normal operation during read [enStatus = HbifRstoDuringRead]
 **                               - Noemal oepration during read [enStatus = HbifTransactionErrDuringRead]
 **                               - No decode error [enStatus = HbifDecodeErrDuringRead]
 **                               - Read transaction is idle [enStatus = HbifReadActive]
 ** \retval TRUE                 If one of following conditions are met:
 **                               - HyperBus memory is under reset (AXI SLVERR occurs) [enStatus = HbifRstoDuringWrite]
 **                               - Protocol is not supported (AXI SLVERR occurs) [enStatus = HbifTransactionErrDuringWrite]
 **                               - Access address is not reachable (AXI DECERR occurs) [enStatus = HbifDecodeErrDuringWrite]
 **                               - Write transaction is active [enStatus = HbifWriteActive]
 **                               - Detect read data error (AXI SLVERR occurs) [enStatus = HbifRdsStallDuringRead]
 **                               - HyperBus memory is under reset (AXI SLVERR occurs) [enStatus = HbifRstoDuringRead]
 **                               - Protocol is not supported (AXI SLVERR occurs) [enStatus = HbifTransactionErrDuringRead]
 **                               - Access address is not reachable (AXI DECERR occurs)(AXI SLVERR occurs) [enStatus = HbifDecodeErrDuringRead]
 **                               - Read transaction is active [enStatus = I2sRxFifoOverflow]
 ** 
 ******************************************************************************/
boolean_t Hbif_GetStatus(en_hbif_status_t enStatus)
{
    boolean_t bRet = FALSE;
      
    switch(enStatus)
    {
        case HbifRstoDuringWrite:
            bRet = (FM_HYPERBUS->CSR_f.WRSTOERR == 1u) ? TRUE : FALSE;
            break;
        case HbifTransactionErrDuringWrite:
            bRet = (FM_HYPERBUS->CSR_f.WTRSERR == 1u) ? TRUE : FALSE;
            break;
        case HbifDecodeErrDuringWrite:
            bRet = (FM_HYPERBUS->CSR_f.WDECERR == 1u) ? TRUE : FALSE;
            break;
        case HbifWriteActive:
            bRet = (FM_HYPERBUS->CSR_f.WACT == 1u) ? TRUE : FALSE;
            break;
        case HbifRdsStallDuringRead:
            bRet = (FM_HYPERBUS->CSR_f.RDSSTALL == 1u) ? TRUE : FALSE;
            break;
        case HbifRstoDuringRead:
            bRet = (FM_HYPERBUS->CSR_f.RRSTOERR == 1u) ? TRUE : FALSE;
            break;
        case HbifTransactionErrDuringRead:
            bRet = (FM_HYPERBUS->CSR_f.RTRSERR == 1u) ? TRUE : FALSE;
            break; 
        case HbifDecodeErrDuringRead:
            bRet = (FM_HYPERBUS->CSR_f.RDECERR == 1u) ? TRUE : FALSE;
            break;     
        case HbifReadActive:
            bRet = (FM_HYPERBUS->CSR_f.RACT == 1u) ? TRUE : FALSE;
            break;
    }
    
    return bRet;
}

/**
 *****************************************************************************
 ** \brief Get the interrupt flag of HyperBus interface
 **
 ** \retval FALSE HBIF interrupt flag is not set
 ** \retval TRUE HBIF interrupt flag is set
 *****************************************************************************/
boolean_t Hbif_GetInterruptFlag(void)
{
    boolean_t bFlag = FALSE;
    
    bFlag = (FM_HYPERBUS->ISR_f.RPCINTS == 1u) ? TRUE : FALSE;
    
    return bFlag;
}

/**
 *****************************************************************************
 ** \brief Set write protection of HBIF
 **
 ** \param bEnableWp Enable write protect or not
 **
 ** \retval Ok Write protection is set
 **
 *****************************************************************************/
en_result_t Hbif_SetWriteProtection(boolean_t bEnableWp)
{
    FM_HYPERBUS->WPR_f.WP = bEnableWp;
    
    return Ok;
}

/**
 *****************************************************************************
 ** \brief Set GPO0 output pin level of HBIF
 **
 ** \param bLevel Output level
 **
 ** \retval Ok GPO0 output pin level set
 **
 *****************************************************************************/
en_result_t Hbif_SetGpo0Level(boolean_t bLevel)
{
    FM_HYPERBUS->GPOR_f.GPO0 = bLevel;
    
    return Ok;
}

/**
 *****************************************************************************
 ** \brief Set GPO1 output pin level of HBIF
 **
 ** \param bLevel Output level
 **
 ** \retval Ok GPO1 output pin level set
 **
 *****************************************************************************/
en_result_t Hbif_SetGpo1Level(boolean_t bLevel)
{
    FM_HYPERBUS->GPOR_f.GPO1 = bLevel;
    
    return Ok;
}

#endif

/*****************************************************************************/
/* EOF (not truncated)                                                       */
/*****************************************************************************/
