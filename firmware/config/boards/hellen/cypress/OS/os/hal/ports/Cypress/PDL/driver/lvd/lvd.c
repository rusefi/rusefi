/*******************************************************************************
* \file             lvd.c
* 
* \version          1.20
*
* \brief            This file provides the source code to the API for the LVD 
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
#include "lvd/lvd.h"

#if (defined(PDL_PERIPHERAL_LVD_ACTIVE))

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/
#if (PDL_INTERRUPT_ENABLE_LVD == PDL_ON)
static stc_lvd_intern_data_t stcLvdInternData = {{0u, 0u}};
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

/* Unlock code for LVD_RLR */
#define LvdCtlUnlock    (0x1ACCE553)

#if (PDL_INTERRUPT_ENABLE_LVD == PDL_ON)
/**
 ******************************************************************************
 ** \brief ISR of the LVD module
 ** 
 ******************************************************************************/
void Lvd_IrqHandler(void)
{    
    if (1u == FM_LVD->LVD_STR_f.LVDIR)
    {
        if (NULL != stcLvdInternData.pfnIrqCallback)
        {
            stcLvdInternData.pfnIrqCallback[0](); /* Clear LVD interrupt */
        }
        
        FM_LVD->LVD_CLR_f.LVDCL = 0;
    }
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)    
    if (1u == FM_LVD->LVD_STR_f.LVD2IR)
    {
        if (NULL != stcLvdInternData.pfnIrqCallback)
        {
            stcLvdInternData.pfnIrqCallback[1](); /* Clear LVD interrupt */
        }
        
        FM_LVD->LVD_CLR_f.LVD2CL = 0;
    }
#endif    
} /* LvdIrqHandler */
#endif

/**
 ******************************************************************************
 ** \brief Init the LVD module
 ** 
 ** Generating interrupts is able to use by specifying callback function.
 ** If callback function is not specified (NULL), interrupt (LVD function)
 ** is enabled, but NVIC is not enabled.
 ** So user has to check interrupt status by #Lvd_GetIrqStatus.
 ** This function enables the interrupt and the user setting voltage for
 ** the LVD module.
 **
 ** \param [in]  pstcConfig    LVD module configuration 
 **
 ** \retval Ok                    Initializiation of LVD interupt successfully 
 **                               done.
 ** \retval ErrorInvalidParameter pstcConfig == NULL or invalid 
 ******************************************************************************/
en_result_t Lvd_Init(const stc_lvd_config_t* pstcConfig)
{
    en_result_t             enResult;
    stc_lvd_lvd_ctl_field_t  stcLVD_CTL;   
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)    
    stc_lvd_lvd_ctl2_field_t stcLVD_CTL2;
    stc_lvd_lvd2_ctl_field_t  stcLVD2_CTL;   
    stc_lvd_lvd2_ctl2_field_t stcLVD2_CTL2;
#endif

    PDL_ZERO_STRUCT(stcLVD_CTL);
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)    
    PDL_ZERO_STRUCT(stcLVD_CTL2);
    PDL_ZERO_STRUCT(stcLVD2_CTL);
    PDL_ZERO_STRUCT(stcLVD2_CTL2);
#endif

    enResult = ErrorInvalidParameter;
    /* Check for valid pointer */
    if (NULL != pstcConfig)
    {
        enResult = Ok;

        /* Prepare LVD_CTL */
        if(pstcConfig->enLvd0IrqDetectVoltage < Lvd0IrqDetectVoltageMax)
        {
            stcLVD_CTL.SVHI = (uint8_t) pstcConfig->enLvd0IrqDetectVoltage;
        }
        else
        {
            enResult = ErrorInvalidParameter;
        }
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)
        /* Prepare LVD_CTL2 */
        if(pstcConfig->enLvd0IrqReleaseVoltage < Lvd0IrqReleaseVoltageMax)
        {
            stcLVD_CTL2.SVHRLI = (uint8_t) pstcConfig->enLvd0IrqReleaseVoltage;
        }
        else
        {
            enResult = ErrorInvalidParameter;
        }
        
        stcLVD_CTL2.LVDRLIE = ((pstcConfig->bLvd0ReleaseVoltageEnable == TRUE) ? 1 : 0);
#endif
        
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE1) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE3) || (PDL_MCU_TYPE == PDL_FM3_TYPE7) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE6) || (PDL_MCU_TYPE == PDL_FM3_TYPE8) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE9) || (PDL_MCU_TYPE == PDL_FM3_TYPE10) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE11) || (PDL_MCU_TYPE == PDL_FM3_TYPE12)           
        if(pstcConfig->enLvdResetVoltage < LvdResetVoltageMax)
        {
            stcLVD_CTL.SVHR = (uint8_t) pstcConfig->enLvdResetVoltage;
        }
        else
        {
            enResult = ErrorInvalidParameter;
        }
#endif   
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)
        /* Prepare LVD_CTL */
        if(pstcConfig->enLvd1IrqDetectVoltage < Lvd1IrqDetectVoltageMax)
        {
            stcLVD2_CTL.SVH2I = (uint8_t) pstcConfig->enLvd1IrqDetectVoltage;
        }
        else
        {
            enResult = ErrorInvalidParameter;
        }
        
        /* Prepare LVD2_CTL2 */
        if(pstcConfig->enLvd1IrqReleaseVoltage < Lvd1IrqReleaseVoltageMax)
        {
            stcLVD2_CTL2.SVH2RLI = (uint8_t) pstcConfig->enLvd1IrqReleaseVoltage;
        }
        else
        {
            enResult = ErrorInvalidParameter;
        }
     
        stcLVD2_CTL2.LVD2RLIE = ((pstcConfig->bLvd1ReleaseVoltageEnable == TRUE) ? 1 : 0);
#endif        
        if (Ok == enResult)
        {
        #if (PDL_INTERRUPT_ENABLE_LVD == PDL_ON)  
            /* Setup callback function pointer in internal data */
            stcLvdInternData.pfnIrqCallback[0] = pstcConfig->pfnIrqCallback[0]; 
            stcLvdInternData.pfnIrqCallback[1] = pstcConfig->pfnIrqCallback[1]; 

            /* If callback function is set, interrupt is used. */
            if ((NULL != pstcConfig->pfnIrqCallback[0]) || 
                (NULL != pstcConfig->pfnIrqCallback[1]) )
            {
            #if (PDL_MCU_TYPE == PDL_FM0P_TYPE3)  
                /* Init NVIC */
                NVIC_ClearPendingIRQ(CSV_SWDT_LVD_IRQn);
                NVIC_EnableIRQ(CSV_SWDT_LVD_IRQn);
                NVIC_SetPriority(CSV_SWDT_LVD_IRQn, PDL_IRQ_LEVEL_CSV_SWDT_LVD);
            #else
                /* Init NVIC */
                NVIC_ClearPendingIRQ(LVD_IRQn);
                NVIC_EnableIRQ(LVD_IRQn);
                NVIC_SetPriority(LVD_IRQn, PDL_IRQ_LEVEL_LVD);
            #endif                
            }
        #endif
            FM_LVD->LVD_CLR = 0x00u;        /* Clear possible pending LVD interrupt (LVDCL (bit #7)) */
            FM_LVD->LVD_RLR = LvdCtlUnlock;/* unlock LVD_CTL */
            FM_LVD->LVD_CTL_f = stcLVD_CTL;
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)            
            FM_LVD->LVD_CTL2_f = stcLVD_CTL2;
            FM_LVD->LVD2_CTL_f = stcLVD2_CTL;
            FM_LVD->LVD2_CTL2_f = stcLVD2_CTL2;
#endif
            FM_LVD->LVD_RLR = 0u;           /* lock LVD_CTL */
        }
    }

    return (enResult);
} /* Lvd_InitIrq */

/**
 ******************************************************************************
 ** \brief De-Init the Interrupt of the LVD module
 **
 ** \retval Ok                    De-Initializiation of LVD interupt
 **                               successfully done.
 **
 ******************************************************************************/
en_result_t Lvd_DeInit(void)
{
#if (PDL_INTERRUPT_ENABLE_LVD == PDL_ON)  
    /* De-Init NVIC */
    if ((NULL != stcLvdInternData.pfnIrqCallback[0]) || 
        (NULL != stcLvdInternData.pfnIrqCallback[1]))
    {
    #if (PDL_MCU_TYPE == PDL_FM0P_TYPE3)  
        NVIC_ClearPendingIRQ(CSV_SWDT_LVD_IRQn);
        NVIC_DisableIRQ(CSV_SWDT_LVD_IRQn);
        NVIC_SetPriority(CSV_SWDT_LVD_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    #else  
        NVIC_ClearPendingIRQ(LVD_IRQn);
        NVIC_DisableIRQ(LVD_IRQn);
        NVIC_SetPriority(LVD_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    #endif   
        stcLvdInternData.pfnIrqCallback[0] = NULL;
        stcLvdInternData.pfnIrqCallback[1] = NULL;
    }
#endif
    FM_LVD->LVD_RLR = LvdCtlUnlock;/* unlock LVD_CTL */
    FM_LVD->LVD_CTL = 0u;           /* Clea all (LVDIE (bit #7)) */
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)
    FM_LVD->LVD_CTL2 = 0u;
    FM_LVD->LVD2_CTL = 0u;
    FM_LVD->LVD2_CTL2 = 0u;
#endif    
    FM_LVD->LVD_RLR = 0u;           /* lock LVD_CTL */
    
    return (Ok);
} /* Lvd_DeInitIrq */

/**
 ******************************************************************************
 ** \brief Enable Interrupt circuit of the LVD module
 **
 ** \param u8Channel LVD channel
 **
 ** \retval Ok                    Interrupt enabled
 **
 ******************************************************************************/
en_result_t Lvd_EnableIrqDetect(uint8_t u8Channel)
{
    if (u8Channel == 0u)
    {
        FM_LVD->LVD_RLR = LvdCtlUnlock;    /* unlock LVD_CTL */
        FM_LVD->LVD_CTL_f.LVDIE = 1u;
        FM_LVD->LVD_RLR = 0u;               /* lock LVD_CTL */
    } 
    else if (u8Channel == 1u)
    {
    #if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)    
        FM_LVD->LVD_RLR = LvdCtlUnlock;    /* unlock LVD_CTL */
        FM_LVD->LVD2_CTL_f.LVD2IE = 1u;
        FM_LVD->LVD_RLR = 0u;               /* lock LVD_CTL */
    #endif    
    }
    else
    {
        return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable Interrupt circuit of the LVD module
 **
 ** \param u8Channel LVD channel
 **
 ** \retval Ok                    Interrupt Disabled
 **
 ******************************************************************************/
en_result_t Lvd_DisableIrqDetect(uint8_t u8Channel)
{
    if (u8Channel == 0u)
    {
        FM_LVD->LVD_RLR = LvdCtlUnlock;    /* unlock LVD_CTL */
        FM_LVD->LVD_CTL_f.LVDIE = 0u;
        FM_LVD->LVD_RLR = 0u;               /* lock LVD_CTL */
    } 
    else if (u8Channel == 1u)
    {
    #if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)   
        FM_LVD->LVD_RLR = LvdCtlUnlock;    /* unlock LVD_CTL */
        FM_LVD->LVD2_CTL_f.LVD2IE = 0u;
        FM_LVD->LVD_RLR = 0u;               /* lock LVD_CTL */
    #endif    
    }
    else
    {
        return ErrorInvalidParameter;
    }
    
    return Ok;
}


#if (PDL_MCU_TYPE == PDL_FM0P_TYPE1) || (PDL_MCU_TYPE == PDL_FM0P_TYPE2) ||\
    (PDL_MCU_TYPE == PDL_FM3_TYPE3) || (PDL_MCU_TYPE == PDL_FM3_TYPE7) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE6) || (PDL_MCU_TYPE == PDL_FM3_TYPE8) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE9) || (PDL_MCU_TYPE == PDL_FM3_TYPE10) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE11) || (PDL_MCU_TYPE == PDL_FM3_TYPE12)   
/**
 ******************************************************************************
 ** \brief Enable reset circuit of the LVD module
 **
 ** \retval Ok                    Reset Enabled
 **
 ******************************************************************************/
en_result_t Lvd_EnableReset(void)
{
    FM_LVD->LVD_RLR = LvdCtlUnlock;    /* unlock LVD_CTL */
    FM_LVD->LVD_CTL_f.LVDRE = 1u;
    FM_LVD->LVD_RLR = 0;               /* lock LVD_CTL */
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable reset circuit of the LVD module
 **
 ** \retval Ok                    Reset Disabled
 **
 ******************************************************************************/
en_result_t Lvd_DisableReset(void)
{
    FM_LVD->LVD_RLR = LvdCtlUnlock;    /* unlock LVD_CTL */
    FM_LVD->LVD_CTL_f.LVDRE = 0u;
    FM_LVD->LVD_RLR = 0u;               /* lock LVD_CTL */

    return Ok;
}

#endif

/**
 ******************************************************************************
 ** \brief Get the LVD interrupt status
 ** 
 ** \param u8Channel LVD channel
 **
 ** \retval FALSE       A low-voltage detection is not detected.
 ** \retval TRUE        A low-voltage detection has been detected.
 ** 
 ******************************************************************************/
boolean_t Lvd_GetIrqStatus(uint8_t u8Channel)
{
    boolean_t bRetVal = FALSE;

    if(0u == u8Channel)
    {
        /* Check the LVD interrupt status */
        if (TRUE == FM_LVD->LVD_STR_f.LVDIR)
        {
            bRetVal = TRUE;
        }
    }
    else if(1u == u8Channel)
    {
    #if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)  
        /* Check the LVD interrupt status */
        if (TRUE == FM_LVD->LVD_STR_f.LVD2IR)
        {
            bRetVal = TRUE;
        }
    #endif    
    }
    return (bRetVal);
} /* Lvd_GetIntOperationStatus */

/**
 ******************************************************************************
 ** \brief Clear the LVD interrupt status
 **
 ******************************************************************************/
en_result_t Lvd_ClrIrqStatus(uint8_t u8Channel)
{
    if (0u == u8Channel)
    {
        FM_LVD->LVD_CLR_f.LVDCL = 0x00u;  /* Clear LVD interrupt */
    }
    else if (1u == u8Channel)
    {
    #if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)  
        FM_LVD->LVD_CLR_f.LVD2CL = 0x00u;  /* Clear LVD interrupt */
    #endif    
    }
    else
    {
        return ErrorInvalidParameter;
    }
    
    return Ok;
} /* Lvd_ClearIntStatus */

/**
 ******************************************************************************
 ** \brief Get the operation status of the LVD interrupt
 **
 ** \retval FALSE       Stabilization wait state or monitoring stop state
 ** \retval TRUE        Monitoring state
 **
 ******************************************************************************/
boolean_t Lvd_GetIrqOperationStatus(uint8_t u8Channel)
{
    boolean_t bRetVal = FALSE;

    if(0u == u8Channel)
    {
        /* Check the operation status of LVD interrupt */
        if (TRUE == FM_LVD->LVD_STR2_f.LVDIRDY)
        {
            bRetVal = TRUE;
        }
    }
    else if (1u == u8Channel)
    {
    #if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)  
        /* Check the operation status of LVD interrupt */
        if (TRUE == FM_LVD->LVD_STR2_f.LVD2IRDY)
        {
            bRetVal = TRUE;
        }
    #endif    
    }
    return (bRetVal);
} /* Lvd_GetIntOperationStatus */

#if (PDL_MCU_TYPE == PDL_FM0P_TYPE1) || (PDL_MCU_TYPE == PDL_FM0P_TYPE2) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE3) || (PDL_MCU_TYPE == PDL_FM3_TYPE7) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE6) || (PDL_MCU_TYPE == PDL_FM3_TYPE8) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE9) || (PDL_MCU_TYPE == PDL_FM3_TYPE10) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE11) || (PDL_MCU_TYPE == PDL_FM3_TYPE12)   
/**
 ******************************************************************************
 ** \brief Get the operation status of the LVD reset
 **
 ** \retval FALSE       Stabilization wait state or monitoring stop state
 ** \retval TRUE        Monitoring state
 **
 ******************************************************************************/
boolean_t Lvd_GetResetOperationStatus(void)
{
    boolean_t bRetVal = FALSE;

    /* Check the operation status of LVD interrupt */
    if (TRUE == FM_LVD->LVD_STR2_f.LVDRRDY)
    {
        bRetVal = TRUE;
    }
    return (bRetVal);
} /* Lvd_GetIntOperationStatus */
#endif

#endif /* #if (defined(PDL_PERIPHERAL_LVD_ACTIVE)) */

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
