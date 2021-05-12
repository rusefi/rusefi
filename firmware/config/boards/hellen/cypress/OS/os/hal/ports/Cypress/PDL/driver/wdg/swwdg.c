/*******************************************************************************
* \file             swwdg.c
* 
* \version          1.20
*
* \brief            This file provides the source code to the API for the SWWDG 
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
#include "wdg/swwdg.h"

#if (defined(PDL_PERIPHERAL_WDG_ACTIVE))

#if (PDL_PERIPHERAL_ENABLE_SWWDG == PDL_ON)
/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/
/* for WDOGLOAD */
#define SWWDG_DEFAULT_VAL   (0xFFFFFFFFu)

/* for WDOGCONTROL */
#define SWWDG_CTL_INTEN     (0x01u)
#define SWWDG_CTL_RESEN     (0x02u)
#define SWWDG_CTL_TWD100    (0x00u)
#define SWWDG_CTL_TWD75     (0x04u)
#define SWWDG_CTL_TWD50     (0x08u)
#define SWWDG_CTL_TWD25     (0x0Cu)
#define SWWDG_CTL_SPM       (0x10u)

/* for WDOGSPMC */
#define SWWDG_SPMC_TGR      (0x01u)

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/
static func_ptr_t pfnSwwdgCallback; ///< callback function pointer for SW-Wdg Irq

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
 ** \brief Software Watchdog Interrupt Handler
 ******************************************************************************/
void SwwdgIrqHandler(void)
{
    /* Check software interrupt status */
    if (TRUE == FM_SWWDT->WDOGRIS_f.RIS)
    {
        if (NULL != pfnSwwdgCallback)
        {
            pfnSwwdgCallback();
        }
    }
} /* SwwdgIrqHandler */

/**
 ******************************************************************************
 ** \brief Initialize Software Watchdog
 **
 ** \param [in] pstcConfig   Pointer to Software Watchdog configuration
 **
 ** \retval Ok                      Setup successful
 ** \retval ErrorInvalidParameter   pstcConfig == NULL
 **
 ** \note This function only initializes the Software Watchdog configuration.
 **       If Swwdg_Start() is called, MCU start the Software Watchdog.
 **
 ******************************************************************************/
en_result_t Swwdg_Init(const stc_swwdg_config_t* pstcConfig)
{
    en_result_t enResult;
    uint8_t     u8WdogControl = 0u;  /* Preset Watchdog Control Register */
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)      
    uint8_t     u8WdogSpmc    = 0u;  /* Preset Window Watchdog Control Register */
#endif
    enResult = ErrorInvalidParameter;

    /* Check for NULL Pointer */
    if (NULL != pstcConfig)
    {
        enResult = Ok;
        /* Un Lock */
        FM_SWWDT->WDOGLOCK = SWWDG_REG_UNLOCK;
    #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
        /* Window watchdog mode */
        if (TRUE == pstcConfig->bWinWdgEnable)
        {
            /* Set SPM bit */
            u8WdogControl |= SWWDG_CTL_SPM;
            /* Set reset enable when reload without timing window was occured */
            if (TRUE == pstcConfig->bWinWdgResetEnable)
            {
                u8WdogSpmc = SWWDG_SPMC_TGR;
            }
            /* Set timing window for window watchdog mode */
            switch (pstcConfig->u8TimingWindow)
            {
                case en_swwdg_timing_window_100:
                    u8WdogControl |= SWWDG_CTL_TWD100;
                    break;
                case en_swwdg_timing_window_75:
                    u8WdogControl |= SWWDG_CTL_TWD75;
                    break;
                case en_swwdg_timing_window_50:
                    u8WdogControl |= SWWDG_CTL_TWD50;
                    break;
                case en_swwdg_timing_window_25:
                    u8WdogControl |= SWWDG_CTL_TWD25;
                    break;
                default:
                    enResult = ErrorInvalidParameter;
                    break;
            }
        }
    #endif
        if (Ok == enResult)
        {
            /* Set reset enable */
            if (TRUE == pstcConfig->bResetEnable)
            {
                /* Set RESEN bit */
                u8WdogControl |= SWWDG_CTL_RESEN;
            }

            // Set interval
            FM_SWWDT->WDOGLOAD = pstcConfig->u32LoadValue;
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
            FM_SWWDT->WDOGSPMC    = u8WdogSpmc;    /* Setup Window watchdog and */
        #endif    
            FM_SWWDT->WDOGCONTROL = u8WdogControl; /* Setup SW-Watchdog */
            FM_SWWDT->WDOGLOCK    = 0u;             /* Lock it! */

            /* Initialize callback to NULL */
            pfnSwwdgCallback = pstcConfig->pfnSwwdgIrqCb;

        #if (PDL_MCU_TYPE == PDL_FM0P_TYPE3)
            /* Enable NVIC */
            NVIC_ClearPendingIRQ(CSV_SWDT_LVD_IRQn);
            NVIC_EnableIRQ(CSV_SWDT_LVD_IRQn);
            NVIC_SetPriority(CSV_SWDT_LVD_IRQn, PDL_IRQ_LEVEL_CSV_SWDT_LVD);
        #else
            /* Enable NVIC */
            NVIC_ClearPendingIRQ(SWDT_IRQn);
            NVIC_EnableIRQ(SWDT_IRQn);
            NVIC_SetPriority(SWDT_IRQn, PDL_IRQ_LEVEL_SWWDG);
        #endif            
        }
    }

    return (enResult);
} /* Swwdg_Init */

/**
 ******************************************************************************
 ** \brief Un-Initialize Software Watchdog
 **
 ******************************************************************************/
void Swwdg_DeInit(void)
{
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE3)  
    /* Disable NVIC */
    NVIC_ClearPendingIRQ(CSV_SWDT_LVD_IRQn);
    NVIC_DisableIRQ(CSV_SWDT_LVD_IRQn);
    NVIC_SetPriority(CSV_SWDT_LVD_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
#else
    /* Disable NVIC */
    NVIC_ClearPendingIRQ(SWDT_IRQn);
    NVIC_DisableIRQ(SWDT_IRQn);
    NVIC_SetPriority(SWDT_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
#endif    

    FM_SWWDT->WDOGLOCK    = SWWDG_REG_UNLOCK;  /* Un Lock */
    FM_SWWDT->WDOGCONTROL = 0u;                 /* Reset SW-Watchdog */
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)      
    FM_SWWDT->WDOGSPMC    = 0u;                 /* Reset Window watchdog */
#endif    
    FM_SWWDT->WDOGLOAD    = SWWDG_DEFAULT_VAL; /* Set default value to load value */

    pfnSwwdgCallback       = NULL;              /* Initialize callback to NULL */
} /* Swwdg_DeInit */

/**
 ******************************************************************************
 ** \brief Start the Software Watchdog
 **
 ** \retval Ok                          Setup successful
 ** \retval ErrorOperationInProgress    Software Watchdog is active now
 **
 ** \note Please initialize by calling Swwdt_Init() before executing this function.
 **
 ******************************************************************************/
en_result_t Swwdg_Start(void)
{
    en_result_t enResult;

    /* If software watchdog is active, error is returned. */
    if (TRUE == FM_SWWDT->WDOGCONTROL_f.INTEN)
    {
        enResult = ErrorOperationInProgress;
    }
    else
    {
        FM_SWWDT->WDOGLOCK = SWWDG_REG_UNLOCK; /* Un Lock */
        FM_SWWDT->WDOGCONTROL_f.INTEN = TRUE;  /* Enable interrupt and count */
        FM_SWWDT->WDOGLOCK = 0u;                /* Lock */

        enResult = Ok;
    }

    return (enResult);
} /* Swwdg_Start */

/**
 ******************************************************************************
 ** \brief Stop the Software Watchdog
 **
 ******************************************************************************/
void Swwdg_Stop(void)
{
    if (TRUE == FM_SWWDT->WDOGCONTROL_f.INTEN)
    {
        FM_SWWDT->WDOGLOCK = SWWDG_REG_UNLOCK; /* Un Lock */
        FM_SWWDT->WDOGCONTROL_f.INTEN = FALSE; /* Disable interrupt and count */
        FM_SWWDT->WDOGLOCK = 0u;                /* Lock */
    }
} /* Swwdg_Stop */

/**
 ******************************************************************************
 ** \brief Write the load value for Software Watchdog
 **
 ** \param [in]  u32LoadValue   Load value
 **
 ******************************************************************************/
void Swwdg_WriteWdgLoad(uint32_t u32LoadValue)
{
    FM_SWWDT->WDOGLOCK = SWWDG_REG_UNLOCK;     /* Un Lock */
    FM_SWWDT->WDOGLOAD = u32LoadValue;         /* Write the load value */
    FM_SWWDT->WDOGLOCK = 0u;                    /* Lock */
} /* Swwdg_WriteWdgLoad */

/**
 ******************************************************************************
 ** \brief Read the count value for Software Watchdog
 **
 ** \retval uint32_t:Value of value register
 **
 ******************************************************************************/
uint32_t Swwdg_ReadWdgValue(void)
{
    return (FM_SWWDT->WDOGVALUE);
} /* Swwdg_ReadWdgValue */

/**
 ******************************************************************************
 ** \brief Feed Software Watchdog (Call function)
 **
 ** This function feeds the Software Watchdog with the unlock, feed, and lock
 ** sequence.
 **
 ******************************************************************************/
void Swwdg_Feed(void)
{
    FM_SWWDT->WDOGLOCK   = SWWDG_REG_UNLOCK;   /* Release Lock */
    FM_SWWDT->WDOGINTCLR = 1u;                  /* Clear possible interrupt and reload value */
    FM_SWWDT->WDOGLOCK   = 0u;                  /* Lock again it! */
} /* Swwdg_Feed */

/**
 ******************************************************************************
 ** \brief Enable Debug Break Watchdog Timer Control
 **
 ******************************************************************************/
void Swwdg_EnableDbgBrkWdgCtl(void)
{
    stc_crg_dbwdt_ctl_field_t stcDBWDT_CTL;

    stcDBWDT_CTL         = FM_CRG->DBWDT_CTL_f;
    stcDBWDT_CTL.DPSWBE  = TRUE;
    FM_CRG->DBWDT_CTL_f = stcDBWDT_CTL;
} /* Swwdt_EnableDbgBrkWdtCtl */

/**
 ******************************************************************************
 ** \brief Disable Debug Break Watchdog Timer Control
 **
 ******************************************************************************/
void Swwdg_DisableDbgBrkWdgCtl(void)
{
    stc_crg_dbwdt_ctl_field_t stcDBWDT_CTL;

    stcDBWDT_CTL         = FM_CRG->DBWDT_CTL_f;
    stcDBWDT_CTL.DPSWBE  = FALSE;
    FM_CRG->DBWDT_CTL_f = stcDBWDT_CTL;
} /* Swwdt_DisableDbgBrkWdtCtl */

#endif /* #if (PDL_PERIPHERAL_ENABLE_SWWDG == PDL_ON) */

#endif /* #if (defined(PDL_PERIPHERAL_WDG_ACTIVE)) */

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
