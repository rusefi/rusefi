/*******************************************************************************
* \file             extif.c
* 
* \version          1.50
*                       
* \brief            External Bus Interface driver
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
#include "extif/extif.h"

#if (defined(PDL_PERIPHERAL_EXTIF_ACTIVE))

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/
#if (PDL_MCU_CORE == PDL_FM4_CORE)      
stc_extif_intern_data_t stcExtifInternData;
#endif

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
 ** \brief External Bus Interface Error Interrupt Service Routine
 **
 ** This function provides the user callback functions, if defined.
 **
 ******************************************************************************/
#if (PDL_MCU_CORE == PDL_FM4_CORE)      
void Extif_IrqHandler( void )
{
    if ((TRUE == stcExtifInternData.bSdramErrorInterruptEnable) &&
        (stcExtifInternData.pfnSdramErrorCallback != NULL))
    {
        if (1u == FM_EXBUS->MEMCERR_f.SDER)
        {
            FM_EXBUS->MEMCERR_f.SDER = 1u;    // Clear SDRAM error interrupt
            stcExtifInternData.pfnSdramErrorCallback();
        }
    }

    if ((TRUE == stcExtifInternData.bSramFlashErrorInterruptEnable) &&
        (stcExtifInternData.pfnSramFlashErrorCallback != NULL))
    {
        if (1u == FM_EXBUS->MEMCERR_f.SFER)
        {
            FM_EXBUS->MEMCERR_f.SFER = 1u;    // Clear SRAM/Flash error interrupt
            stcExtifInternData.pfnSramFlashErrorCallback();
        }
    }

    
#if (PDL_MCU_TYPE == PDL_FM4_TYPE4)
    if ((TRUE == stcExtifInternData.bGdcSdramErrorInterruptEnable) &&
        (stcExtifInternData.pfnGdcSdramErrorCallback != NULL))
    {
        if (1u == FM_EXBUS->MEMCERR_f.SDER)
        {
            FM_EXBUS->MEMCERR_f.SDER = 1u;    // Clear GDC SDRAM error interrupt
            stcExtifInternData.pfnGdcSdramErrorCallback();
        }
    }      
#endif          
}
#endif

/**
 ******************************************************************************
 ** \brief Setup (init) an EXT-I/F area
 **
 ** \param u8Area       Extbus I/F area (chip select) number
 ** \param pstcConfig   Pointer to area configuration
 **
 ** \retval Ok                      Setup successful
 ** \retval ErrorInvalidParameter   pstcConfig == NULL, Area number wrong,
 **                                 other invalid setting.
 ** \retval ErrorInvalidMode        SDMODE is set for area different from 8
 ******************************************************************************/
en_result_t Extif_InitArea( uint8_t                  u8Area,
                            const stc_extif_area_config_t* pstcConfig
                          )
{
    // Local register predefinitions
    stc_exbus_mode0_field_t   stcMODE;
    stc_exbus_tim0_field_t    stcTIM;
    stc_exbus_area0_field_t   stcAREA;
    stc_exbus_atim0_field_t   stcATIM;
    stc_exbus_dclkr_field_t   stcDCLKR;
#if (PDL_MCU_CORE == PDL_FM4_CORE)    
    stc_exbus_sdmode_field_t  stcSDMODE;
    stc_exbus_reftim_field_t  stcREFTIM;
    stc_exbus_pwrdwn_field_t  stcPWRDWN;
    stc_exbus_sdtim_field_t   stcSDTIM;
    stc_exbus_memcerr_field_t stcMEMCERR;
    stc_exbus_amode_field_t   stcAMODE;
    uint8_t                   u8InitNvic  = 0;
#endif    
    uint8_t                   u8Dummy;
    uint8_t                   u8InitGdcSdram = 0, u8InitExtifSdram = 0;
#if (PDL_MCU_TYPE == PDL_FM4_TYPE4)
    stc_sdramc_sdmode_field_t  stcGdcSDMODE;
    stc_sdramc_reftim_field_t  stcGdcREFTIM;
    stc_sdramc_pwrdwn_field_t  stcGdcPWRDWN;
    stc_sdramc_sdtim_field_t   stcGdcSDTIM;
    stc_exbus_memcerr_field_t  stcGdcMEMCERR;
#endif    
    PDL_ZERO_STRUCT(stcMODE);
    PDL_ZERO_STRUCT(stcTIM);
    PDL_ZERO_STRUCT(stcAREA);
    PDL_ZERO_STRUCT(stcATIM);
    PDL_ZERO_STRUCT(stcDCLKR);
#if (PDL_MCU_CORE == PDL_FM4_CORE)     
    PDL_ZERO_STRUCT(stcSDMODE);
    PDL_ZERO_STRUCT(stcREFTIM);
    PDL_ZERO_STRUCT(stcPWRDWN);
    PDL_ZERO_STRUCT(stcSDTIM);
    PDL_ZERO_STRUCT(stcMEMCERR);
    PDL_ZERO_STRUCT(stcAMODE);
#endif    
#if (PDL_MCU_TYPE == PDL_FM4_TYPE4)
    PDL_ZERO_STRUCT(stcGdcSDMODE);
    PDL_ZERO_STRUCT(stcGdcREFTIM);
    PDL_ZERO_STRUCT(stcGdcPWRDWN);
    PDL_ZERO_STRUCT(stcGdcSDTIM);
    PDL_ZERO_STRUCT(stcGdcMEMCERR);
#endif    

#if (PDL_MCU_CORE == PDL_FM4_CORE)       
    stcExtifInternData.pfnSdramErrorCallback     = NULL;
    stcExtifInternData.pfnSramFlashErrorCallback = NULL;
#endif    
    
    if ((u8Area > 8u) || (NULL == pstcConfig))
    {
        return ErrorInvalidParameter;
    }
#if (PDL_MCU_CORE == PDL_FM4_CORE)       
    u8InitExtifSdram = ((NULL != pstcConfig->pExtifSdramConfig) ? 1u : 0u);
#endif    
#if (PDL_MCU_TYPE == PDL_FM4_TYPE4)  
    u8InitGdcSdram =  ((NULL != pstcConfig->pGdcSdramConfig) ? 1u : 0u);
#endif    
    // Initialize NOR/SRAM/Nand interface when SDRAM mode is not used.
    if ((0u == u8InitExtifSdram) && (0u == u8InitGdcSdram))
    {
        switch (pstcConfig->enWidth)
        {
            case Extif8Bit:
                stcMODE.WDTH = 0u;
                break;
            case Extif16Bit:
                stcMODE.WDTH = 1u;
                break;
            default:
                return ErrorInvalidParameter;
        }

        // MODE Register preparation
        stcMODE.RBMON    = (TRUE == pstcConfig->bReadByteMask)           ? 1ul : 0ul ;
        stcMODE.WEOFF    = (TRUE == pstcConfig->bWriteEnableOff)         ? 1ul : 0ul ;
        stcMODE.NAND     = (TRUE == pstcConfig->bNandFlash)              ? 1ul : 0ul ;
        stcMODE.PAGE     = (TRUE == pstcConfig->bPageAccess)             ? 1ul : 0ul ;
    #if (PDL_MCU_TYPE != PDL_FM3_TYPE0)     
        stcMODE.RDY      = (TRUE == pstcConfig->bRdyOn)                  ? 1ul : 0ul ;
        stcMODE.SHRTDOUT = (TRUE == pstcConfig->bStopDataOutAtFirstIdle) ? 1ul : 0ul ;
        stcMODE.MPXMODE  = (TRUE == pstcConfig->bMultiplexMode)          ? 1ul : 0ul ;
        stcMODE.ALEINV   = (TRUE == pstcConfig->bAleInvert)              ? 1ul : 0ul ;
        stcMODE.MPXDOFF  = (TRUE == pstcConfig->bAddrOnDataLinesOff)     ? 1ul : 0ul ;
        stcMODE.MPXCSOF  = (TRUE == pstcConfig->bMpxcsOff)               ? 1ul : 0ul ;
        stcMODE.MOEXEUP  = (TRUE == pstcConfig->bMoexWidthAsFradc)       ? 1ul : 0ul ;
    #endif
        
        // TIM Register preparation
    #if (PDL_MCU_TYPE != PDL_FM3_TYPE0)    
        if ((Extif0Cycle   == pstcConfig->enReadAccessCycle) ||
            (ExtifDisabled == pstcConfig->enReadAccessCycle))
        {
            return ErrorInvalidParameter;
        }
        stcTIM.RACC  = (uint8_t) (pstcConfig->enReadAccessCycle - 1u);

        if ((Extif16Cycle  == pstcConfig->enReadAddressSetupCycle) ||
            (ExtifDisabled == pstcConfig->enReadAddressSetupCycle))
        {
            return ErrorInvalidParameter;
        }
        stcTIM.RADC  = (uint8_t) pstcConfig->enReadAddressSetupCycle;

        if ((FALSE == pstcConfig->bPageAccess) && (FALSE == pstcConfig->bMoexWidthAsFradc))
        {
            if ((Extif0Cycle   == pstcConfig->enFirstReadAddressCycle) ||
                (ExtifDisabled == pstcConfig->enFirstReadAddressCycle))
            {
                return ErrorInvalidParameter;
            }
            stcTIM.FRADC = (uint8_t) (pstcConfig->enFirstReadAddressCycle - 1u);
        }
        else if ((TRUE == pstcConfig->bPageAccess) && (FALSE == pstcConfig->bMoexWidthAsFradc)) 
        {
            if ((Extif16Cycle  == pstcConfig->enFirstReadAddressCycle) ||
                (ExtifDisabled == pstcConfig->enFirstReadAddressCycle))
            {
                return ErrorInvalidParameter;
            }
            stcTIM.FRADC = (uint8_t) pstcConfig->enFirstReadAddressCycle;
        }
        else
        {
            return ErrorInvalidParameter;       // Other setting for PAGE and MOEXEUP not allowed!
        }
    #endif
        
        if ((Extif0Cycle   == pstcConfig->enReadIdleCycle) ||
            (ExtifDisabled == pstcConfig->enReadIdleCycle))
        {
            return ErrorInvalidParameter;
        }
        stcTIM.RIDLC = (uint8_t) (pstcConfig->enReadIdleCycle - 1u);

        if ((Extif0Cycle   == pstcConfig->enWriteAccessCycle) ||
            (Extif1Cycle   == pstcConfig->enWriteAccessCycle) ||
            (Extif2Cycle   == pstcConfig->enWriteAccessCycle) ||
            (ExtifDisabled == pstcConfig->enWriteAccessCycle))
        {
            return ErrorInvalidParameter;
        }  
        stcTIM.WACC  = (uint8_t) (pstcConfig->enWriteAccessCycle - 1u);

        if ((Extif16Cycle  == pstcConfig->enWriteAddressSetupCycle) ||
            (Extif0Cycle   == pstcConfig->enWriteAddressSetupCycle) ||
            (ExtifDisabled == pstcConfig->enWriteAddressSetupCycle))
        {
            return ErrorInvalidParameter;
        }
        stcTIM.WADC  = (uint8_t) (pstcConfig->enWriteAddressSetupCycle - 1u);

        if ((Extif16Cycle  == pstcConfig->enWriteEnableCycle) ||
            (Extif0Cycle   == pstcConfig->enWriteEnableCycle) ||
            (ExtifDisabled == pstcConfig->enWriteEnableCycle))
        {
            return ErrorInvalidParameter;
        }
        stcTIM.WWEC  = (uint8_t) (pstcConfig->enWriteEnableCycle - 1u);

        if ((Extif0Cycle   == pstcConfig->enWriteIdleCycle) ||
            (ExtifDisabled == pstcConfig->enWriteIdleCycle))
        {
            return ErrorInvalidParameter;
        }  
        stcTIM.WIDLC = (uint8_t) (pstcConfig->enWriteIdleCycle - 1u);

        // Check timing contraints
        // WACC >= WADC + WWEC
        u8Dummy =  (uint8_t)stcTIM.WADC;      // u8Dummy provided because of volatile
        u8Dummy += (uint8_t)stcTIM.WWEC;      //  qualifiers, which cannot be mixed
        if ((uint8_t)stcTIM.WACC < u8Dummy)   //  in one expression
        {
            return ErrorInvalidParameter;
        }

        // RADC < RACC
        u8Dummy =  (uint8_t)stcTIM.RACC;      // see above
        if ((uint8_t)stcTIM.RADC >= u8Dummy)
        {
            return ErrorInvalidParameter;
        }

        // AREA Register preparation
        stcAREA.ADDR = (uint8_t) pstcConfig->u8AreaAddress;
        stcAREA.MASK = (uint8_t) pstcConfig->enAreaMask;

        // ATIM Register preparation
        if ((Extif0Cycle   == pstcConfig->enAddressLatchCycle) ||
            (ExtifDisabled == pstcConfig->enAddressLatchCycle))
        {
            return ErrorInvalidParameter;
        }
        stcATIM.ALC = (uint8_t) (pstcConfig->enAddressLatchCycle - 1u);

        if ((Extif16Cycle  == pstcConfig->enReadAddressSetupCycle) ||
            (ExtifDisabled == pstcConfig->enReadAddressSetupCycle))
        {
            return ErrorInvalidParameter;
        }
        stcATIM.ALES = (uint8_t) pstcConfig->enReadAddressSetupCycle;

        if ((Extif0Cycle   == pstcConfig->enAddressLatchWidthCycle) ||
            (ExtifDisabled == pstcConfig->enAddressLatchWidthCycle))
        {
            return ErrorInvalidParameter;
        }
        stcATIM.ALEW = (uint8_t) (pstcConfig->enAddressLatchWidthCycle - 1u);
    #if (PDL_MCU_CORE == PDL_FM4_CORE)      
        if (TRUE == pstcConfig->bSramFlashErrorInterruptEnable)
        {
            stcMEMCERR.SFION = 1u;
            stcExtifInternData.bSramFlashErrorInterruptEnable = TRUE;
            stcExtifInternData.pfnSramFlashErrorCallback = pstcConfig->pfnSramFlashErrorCallback;
        }
    #endif    
    }
#if (PDL_MCU_TYPE != PDL_FM3_TYPE0)     
    // DCLKR Register preparation
    if ((pstcConfig->u8MclkDivision == 0u) ||
        (pstcConfig->u8MclkDivision > 16u))
    {
        return ErrorInvalidParameter;
    }
    else
    {
        stcDCLKR.MDIV = (uint8_t) pstcConfig->u8MclkDivision - 1u;
    }

    stcDCLKR.MCLKON = (TRUE == pstcConfig->bMclkoutEnable) ? 1ul : 0ul ;
#endif        
#if (PDL_MCU_CORE == PDL_FM4_CORE) 
    // AMODE Register preparation
    stcAMODE.WAEN = (TRUE == pstcConfig->bPrecedReadContinuousWrite) ? 1ul : 0ul ;
    
    // Configure SDRAM I/F of external bus interface
    if(NULL != pstcConfig->pExtifSdramConfig)
    {
        // Check for SDMODE and cheip select area 8
        if ((TRUE == pstcConfig->pExtifSdramConfig->bSdramEnable) && (8u != u8Area))
        {
            return ErrorInvalidMode;
        }
     
        // SDMODE Register preparation
        stcSDMODE.SDON = (TRUE == pstcConfig->pExtifSdramConfig->bSdramEnable)        ? 1ul : 0ul ;
        stcSDMODE.PDON = (TRUE == pstcConfig->pExtifSdramConfig->bSdramPowerDownMode) ? 1ul : 0ul ;
        stcSDMODE.ROFF = (TRUE == pstcConfig->pExtifSdramConfig->bSdramRefreshOff)    ? 1ul : 0ul ;

        switch (pstcConfig->pExtifSdramConfig->enCasel)
        {
            case ExtifCas16Bit:
                stcSDMODE.CASEL = 0ul;
                break;
            case ExtifCas32Bit:
                stcSDMODE.CASEL = 1ul;
                break;
            default:
                return ErrorInvalidParameter;
        }

        stcSDMODE.RASEL  = (uint8_t) pstcConfig->pExtifSdramConfig->enRasel;
        stcSDMODE.BASEL  = (uint8_t) pstcConfig->pExtifSdramConfig->enBasel;
        stcSDMODE.MSDCLKOFF = 0u;
          
        // REFTIM Register preparation
        stcREFTIM.REFC = pstcConfig->pExtifSdramConfig->u16RefreshCount;
        stcREFTIM.NREF = pstcConfig->pExtifSdramConfig->u8RefreshNumber;
        stcREFTIM.PREF = (TRUE == pstcConfig->pExtifSdramConfig->bPreRefreshEnable) ? 1ul : 0ul ;

        // PWRDWN Register preparation
        stcPWRDWN.PDC = pstcConfig->pExtifSdramConfig->u16PowerDownCount;

        // SDTIM Register preparation
        stcSDTIM.CL    = (uint8_t) pstcConfig->pExtifSdramConfig->enSdramCasLatencyCycle;
        stcSDTIM.TRC   = (uint8_t) pstcConfig->pExtifSdramConfig->enSdramRasCycle;
        stcSDTIM.TRP   = (uint8_t) pstcConfig->pExtifSdramConfig->enSdramRasPrechargeCycle;
        stcSDTIM.TRCD  = (uint8_t) pstcConfig->pExtifSdramConfig->enSdramRasCasDelayCycle;
        stcSDTIM.TRAS  = (uint8_t) pstcConfig->pExtifSdramConfig->enSdramRasActiveCycle;
        stcSDTIM.TREFC = (uint8_t) pstcConfig->pExtifSdramConfig->enSdramRefreshCycle;
        stcSDTIM.TDPL  = (uint8_t) pstcConfig->pExtifSdramConfig->enSdramPrechargeCycle;

        // MEMCERR Register preparation and Callback Pointer intern data settings
        if (TRUE == pstcConfig->pExtifSdramConfig->bSdramErrorInterruptEnable)
        {
            stcMEMCERR.SDION = 1u;
            stcExtifInternData.bSdramErrorInterruptEnable = TRUE;
            stcExtifInternData.pfnSdramErrorCallback = pstcConfig->pExtifSdramConfig->pfnSdramErrorCallback;
        }
    }

#if (PDL_MCU_TYPE == PDL_FM4_TYPE4)    
    // Configure SDRAM I/F of GDC
    if(NULL != pstcConfig->pGdcSdramConfig)
    {
        // SDMODE Register preparation
        stcGdcSDMODE.SDON = (TRUE == pstcConfig->pGdcSdramConfig->bSdramEnable)        ? 1ul : 0ul ;
        stcGdcSDMODE.PDON = (TRUE == pstcConfig->pGdcSdramConfig->bSdramPowerDownMode) ? 1ul : 0ul ;
        stcGdcSDMODE.ROFF = (TRUE == pstcConfig->pGdcSdramConfig->bSdramRefreshOff)    ? 1ul : 0ul ;

        switch (pstcConfig->pGdcSdramConfig->enCasel)
        {
            case ExtifCas16Bit:
                stcGdcSDMODE.CASEL = 0ul;
                break;
            case ExtifCas32Bit:
                stcGdcSDMODE.CASEL = 1ul;
                break;
            default:
                return ErrorInvalidParameter;
        }

        stcGdcSDMODE.RASEL  = (uint8_t) pstcConfig->pGdcSdramConfig->enRasel;
        stcGdcSDMODE.BASEL  = (uint8_t) pstcConfig->pGdcSdramConfig->enBasel;
        stcGdcSDMODE.MSDCLKOFF = 0u;
          
        // REFTIM Register preparation
        stcGdcREFTIM.REFC = pstcConfig->pGdcSdramConfig->u16RefreshCount;
        stcGdcREFTIM.NREF = pstcConfig->pGdcSdramConfig->u8RefreshNumber;
        stcGdcREFTIM.PREF = (TRUE == pstcConfig->pGdcSdramConfig->bPreRefreshEnable) ? 1ul : 0ul ;

        // PWRDWN Register preparation
        stcGdcPWRDWN.PDC = pstcConfig->pGdcSdramConfig->u16PowerDownCount;

        // SDTIM Register preparation
        stcGdcSDTIM.CL    = (uint8_t) pstcConfig->pGdcSdramConfig->enSdramCasLatencyCycle;
        stcGdcSDTIM.TRC   = (uint8_t) pstcConfig->pGdcSdramConfig->enSdramRasCycle;
        stcGdcSDTIM.TRP   = (uint8_t) pstcConfig->pGdcSdramConfig->enSdramRasPrechargeCycle;
        stcGdcSDTIM.TRCD  = (uint8_t) pstcConfig->pGdcSdramConfig->enSdramRasCasDelayCycle;
        stcGdcSDTIM.TRAS  = (uint8_t) pstcConfig->pGdcSdramConfig->enSdramRasActiveCycle;
        stcGdcSDTIM.TREFC = (uint8_t) pstcConfig->pGdcSdramConfig->enSdramRefreshCycle;
        stcGdcSDTIM.TDPL  = (uint8_t) pstcConfig->pGdcSdramConfig->enSdramPrechargeCycle;

        // MEMCERR Register preparation and Callback Pointer intern data settings
        if (TRUE == pstcConfig->pGdcSdramConfig->bSdramErrorInterruptEnable)
        {
            stcGdcMEMCERR.SDION = 1u;
            stcExtifInternData.bGdcSdramErrorInterruptEnable = TRUE;
            stcExtifInternData.pfnGdcSdramErrorCallback = pstcConfig->pGdcSdramConfig->pfnSdramErrorCallback;
        }
    }
#endif      
#endif    
    
#if (PDL_MCU_CORE == PDL_FM4_CORE)      
    u8InitNvic = ((TRUE == pstcConfig->bSramFlashErrorInterruptEnable) ? 1u : 0u);
    u8InitNvic += ((TRUE == pstcConfig->pExtifSdramConfig->bSdramErrorInterruptEnable) ? 1u : 0u);
#endif    
#if (PDL_MCU_TYPE == PDL_FM4_TYPE4)    
    u8InitNvic += ((TRUE == pstcConfig->pGdcSdramConfig->bSdramErrorInterruptEnable) ? 1u : 0u);
#endif    
    
#if (PDL_MCU_CORE == PDL_FM4_CORE)      
    if (u8InitNvic > 0u)
    {
#if (FM4_SDRAMC_AVAILABLE == 1)
        NVIC_ClearPendingIRQ(EXTBUS_ERR_GDC_SDRAM_IRQn);
        NVIC_EnableIRQ(EXTBUS_ERR_GDC_SDRAM_IRQn);
        NVIC_SetPriority(EXTBUS_ERR_GDC_SDRAM_IRQn, PDL_IRQ_LEVEL_EXTIF);
#else
        NVIC_ClearPendingIRQ(EXTBUS_ERR_IRQn);
        NVIC_EnableIRQ(EXTBUS_ERR_IRQn);
        NVIC_SetPriority(EXTBUS_ERR_IRQn, PDL_IRQ_LEVEL_EXTIF);
#endif        
    }
#endif    
    // Finally setup hardware
    if (u8Area < 8u)
    {
        EXTIF.astcMODE[u8Area] = stcMODE;
        EXTIF.astcTIM[u8Area]  = stcTIM;
        EXTIF.astcAREA[u8Area] = stcAREA;
        EXTIF.astcATIM[u8Area] = stcATIM;
    }
    
    FM_EXBUS->DCLKR_f   = stcDCLKR;
#if (PDL_MCU_CORE == PDL_FM4_CORE)
    FM_EXBUS->AMODE_f   = stcAMODE;
    if(NULL != pstcConfig->pExtifSdramConfig)
    {
        // Clear mode register
        FM_EXBUS->SDMODE = 0u;
      
        // Set the registers
        FM_EXBUS->REFTIM_f  = stcREFTIM;
        FM_EXBUS->PWRDWN_f  = stcPWRDWN;
        FM_EXBUS->SDTIM_f   = stcSDTIM;
        FM_EXBUS->MEMCERR_f = stcMEMCERR;
        FM_EXBUS->SDMODE_f  = stcSDMODE;
    }
#if (PDL_MCU_TYPE == PDL_FM4_TYPE4) 
    if(NULL != pstcConfig->pGdcSdramConfig)
    {
        // Clear mode register
        FM_SDRAMC->SDMODE  = 0u;
      
        // Set the registers
        FM_SDRAMC->REFTIM_f  = stcGdcREFTIM;
        FM_SDRAMC->PWRDWN_f  = stcGdcPWRDWN;
        FM_SDRAMC->SDTIM_f   = stcGdcSDTIM;
        FM_EXBUS->MEMCERR_f  = stcGdcMEMCERR;
        FM_SDRAMC->SDMODE_f  = stcGdcSDMODE;
    }
#endif
#endif
    
    return Ok;
} // Extif_InitArea

#if (PDL_MCU_CORE == PDL_FM4_CORE)
/**
 ******************************************************************************
 ** \brief Read Error Status Register
 ** 
 ** \retval Ok      No error response exists
 ** \retval Error   Error response exists
 ******************************************************************************/
en_result_t Extif_ReadErrorStatus( void )
{
  if (TRUE == FM_EXBUS->EST_f.WERR)
  {
    return Error;
  }
  
  return Ok;
} // Extif_ReadErrorStatus

/**
 ******************************************************************************
 ** \brief Read Error Address Register
 ** 
 ** \return  uint32_t  Error Address 
 ******************************************************************************/
uint32_t Extif_ReadErrorAddress( void )
{
  return FM_EXBUS->WEAD;
} // Extif_ReadErrorAddress

/**
 ******************************************************************************
 ** \brief Clear Error Status Register
 ** 
 ** \retval Ok      Status Error cleared
 ******************************************************************************/
en_result_t Extif_ClearErrorStatus( void )
{
  FM_EXBUS->ESCLR_f.WERRCLR = 0u;
  
  return Ok;
} // Extif_ClearErrorStatus

/**
 ******************************************************************************
 ** \brief Check SDRAM command register is ready
 ** 
 ** \retval Ok              Writing to SDCMD register is possible
 ** \retval ErrorNotReady   Access to SDCMD register not possible
 ******************************************************************************/
en_result_t Extif_CheckSdcmdReady( void )
{
  if (TRUE == FM_EXBUS->SDCMD_f.PEND)
  {
    return ErrorNotReady;
  }
  
  return Ok;
} // Extif_CheckSdcmdReady

/**
 ******************************************************************************
 ** \brief Set a SDRAM command
 **
 ** \note This function calls Extif_CheckSdcmdReady().
 **
 ** \param u16Address   SDRAM address (MAD[15:00] pin values)
 ** \param bMsdwex      MDSWEX pin value
 ** \param bMcasx       MCASX pin value
 ** \param bMrasx       MRASX pin value
 ** \param bMcsx8       MCSX8 pin value
 ** \param bMadcke      MADCKE pin value
 **
 ** \retval Ok              Writing to SDCMD register was successful
 ** \retval ErrorNotReady   Access to SDCMD register was not possible
 ******************************************************************************/
en_result_t Extif_SetSdramCommand( uint16_t  u16Address,
                                   boolean_t bMsdwex,
                                   boolean_t bMcasx,
                                   boolean_t bMrasx,
                                   boolean_t bMcsx8,
                                   boolean_t bMadcke
                                 )
{
  stc_exbus_sdcmd_field_t stcSDCMD;
  
  if (ErrorNotReady ==  Extif_CheckSdcmdReady())
  {
    return ErrorNotReady;
  }
  
  stcSDCMD.SDAD  = u16Address;
  stcSDCMD.SDWE  = bMsdwex;
  stcSDCMD.SDCAS = bMcasx;
  stcSDCMD.SDRAS = bMrasx;
  stcSDCMD.SDCS  = bMcsx8;
  stcSDCMD.SDCKE = bMadcke;
  
  FM_EXBUS->SDCMD_f = stcSDCMD;
  
  return Ok;
}
#endif


#endif // #if (defined(PDL_PERIPHERAL_EXTIF_ACTIVE)
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
