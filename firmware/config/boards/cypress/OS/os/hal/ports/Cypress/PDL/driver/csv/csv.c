/******************************************************************************
* \file             csv.c
*
* \version          1.20
*
* \brief            Clock Supervisor driver
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

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "csv/csv.h"

#if (defined(PDL_PERIPHERAL_CSV_ACTIVE))
/*---------------------------------------------------------------------------*/
/* local defines                                                             */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* local datatypes                                                           */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
#if (PDL_INTERRUPT_ENABLE_CSV == PDL_ON) 
static fn_fcs_int_callback *m_pfnIrqCb = NULL;
#endif

/*---------------------------------------------------------------------------*/
/* local functions prototypes                                                */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* global data                                                               */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* global functions                                                          */
/*---------------------------------------------------------------------------*/

#if (PDL_INTERRUPT_ENABLE_CSV == PDL_ON) 
/**
 ******************************************************************************
 ** \brief CSV interrupt handler
 ******************************************************************************/
void Csv_IrqHandler(void)
{
    bFM_CRG_INT_CLR_FCSC = 1;
    m_pfnIrqCb();
}

/**
 ******************************************************************************
 ** \brief Enables FCS interrupts
 ******************************************************************************/
en_result_t Csv_EnableFcsIrq(fn_fcs_int_callback* pfnIrqCb)
{
    if(NULL == pfnIrqCb)
    {
        return ErrorInvalidParameter;
    }
    
    m_pfnIrqCb = pfnIrqCb;
    bFM_CRG_INT_ENR_FCSE = 1u;
    
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE3)
    NVIC_ClearPendingIRQ(CSV_SWDT_LVD_IRQn); 
    NVIC_EnableIRQ(CSV_SWDT_LVD_IRQn); 
    NVIC_SetPriority(CSV_SWDT_LVD_IRQn, PDL_IRQ_LEVEL_CSV_SWDT_LVD);
#else    
    NVIC_ClearPendingIRQ(CSV_IRQn); 
    NVIC_EnableIRQ(CSV_IRQn); 
    NVIC_SetPriority(CSV_IRQn, PDL_IRQ_LEVEL_CSV);
#endif    
    return Ok;
}

/*!
 ******************************************************************************
 ** \brief Disables FCS interrupts
 ******************************************************************************
 */
void Csv_DisableFcsIrq(void) 
{
    bFM_CRG_INT_ENR_FCSE = 0u;  
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE3) 
    NVIC_ClearPendingIRQ(CSV_SWDT_LVD_IRQn);
    NVIC_DisableIRQ(CSV_SWDT_LVD_IRQn);
    NVIC_SetPriority(CSV_SWDT_LVD_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL); 
#else    
    NVIC_ClearPendingIRQ(CSV_IRQn);
    NVIC_DisableIRQ(CSV_IRQn);
    NVIC_SetPriority(CSV_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);   
#endif    
}
#endif

/*!
 ******************************************************************************
 ** \brief The main CSV function is enabled
 ******************************************************************************
 */
void Csv_EnableMainCsv(void) 
{
    bFM_CRG_CSV_CTL_MCSVE = 1u;
}

/*!
 ******************************************************************************
 ** \brief The main CSV function is disabled
 ******************************************************************************
 */
void Csv_DisableMainCsv(void) 
{
    bFM_CRG_CSV_CTL_MCSVE = 0u;  
}

/*!
 ******************************************************************************
 ** \brief The sub CSV function is enabled.
 ******************************************************************************
 */
void Csv_EnableSubCsv(void) 
{
    bFM_CRG_CSV_CTL_SCSVE = 1u;  
}

/*!
 ******************************************************************************
 ** \brief The sub CSV function is disabled
 ******************************************************************************
 */
void Csv_DisableSubCsv(void) 
{
    bFM_CRG_CSV_CTL_SCSVE = 0u; 
}

/*!
 ******************************************************************************
 ** \brief Get CSV status
 **
 ** \param pstcCsvStatus Pointer to status information struture of CSV 
 **
 ** \retval Ok
 ******************************************************************************
 */
uint8_t Csv_GetCsvFailCause(stc_csv_status_t* pstcCsvStatus)
{ 
    uint8_t u8Status;
    
    u8Status = FM_CRG->CSV_STR;
    
    if(0x01u == (u8Status & 0x01u))
    {
        pstcCsvStatus->bCsvMainClockStatus = TRUE;
    }
    else
    {
        pstcCsvStatus->bCsvMainClockStatus = FALSE;
    }
    
    if(0x02u == (u8Status & 0x02u))
    {
        pstcCsvStatus->bCsvSubClockStatus = TRUE;
    }
    else
    {
        pstcCsvStatus->bCsvSubClockStatus = FALSE;
    }
    
    return Ok;;
}

/**
 ******************************************************************************
 ** \brief The FCS function is enabled.
 ******************************************************************************/
void Csv_EnableFcs(void) 
{
    bFM_CRG_CSV_CTL_FCSDE = 1u;
}

/**
 ******************************************************************************
 ** \brief The FCS function is disabled
 ******************************************************************************/
void Csv_DisableFcs(void) 
{
    bFM_CRG_CSV_CTL_FCSDE = 0u;
}

/**
 ******************************************************************************
 ** \brief The FCS reset is enabled.
 ******************************************************************************/
void Csv_EnableFcsReset(void) 
{
    bFM_CRG_CSV_CTL_FCSRE = 1u;
}


/**
 ******************************************************************************
 ** \brief The FCS reset is disabled.
 ******************************************************************************/
void Csv_DisableFcsReset(void) 
{
    bFM_CRG_CSV_CTL_FCSRE = 0u;
}

/**
 ******************************************************************************
 ** \brief Clears the FCS interrupt cause.
 ******************************************************************************/
void Csv_ClrFcsIrqFlag(void) 
{
    bFM_CRG_INT_CLR_FCSC = 1u;  
}

/**
 ******************************************************************************
 ** \brief Get Anomalous frequency detection interrupt status
 **
 ** \return interrupt status
 ** \retval 0 No FCS interrupt has been asserted.
 ** \retval 1 An FCS interrupt has been asserted.
 ******************************************************************************/
uint8_t Csv_GetFcsIrqFlag(void) 
{
    return bFM_CRG_INT_STR_FCSI;
}

/**
 ******************************************************************************
 ** \brief FCS count cycle setting
 **
 ** \param enDiv          High-speed CR division
 ** \arg FcsCrDiv256      1/256 frequency of high-speed CR oscillation   
 ** \arg FcsCrDiv512      1/512 frequency of high-speed CR oscillation
 ** \arg FcsCrDiv1024     1/1024 frequency of high-speed CR oscillation
 **
 ** \retval Ok
 ******************************************************************************/
en_result_t Csv_SetFcsCrDiv(en_fcs_cr_div_t enDiv) 
{ 
    if ((FcsCrDiv256 != enDiv) && 
        (FcsCrDiv512 != enDiv) && 
        (FcsCrDiv1024 != enDiv))
    {
        return ErrorInvalidParameter;
    }

    FM_CRG->CSV_CTL &= 0x0fffu;
    FM_CRG->CSV_CTL |= ((uint16_t)enDiv << 12u);
    
    return Ok;
}


/**
 ******************************************************************************
 ** \brief Frequency lower detection window setting
 **
 ** \param u16LowerVal Lower value 
 ** \param u16UpperVal limit value
 ******************************************************************************/
void Csv_SetFcsDetectRange(uint16_t u16LowerVal, uint16_t u16UpperVal) 
{
    FM_CRG->FCSWL_CTL = u16LowerVal;  
    FM_CRG->FCSWH_CTL = u16UpperVal;
}

/**
 ******************************************************************************
 ** \brief Get the counter value of frequency detection using the main clock.
 **
 ** \return Frequency detection counter value
 ******************************************************************************/
uint16_t Csv_GetFcsDetectCount(void) 
{
    return FM_CRG->FCSWD_CTL;        
}

#endif

/*****************************************************************************/
/* END OF FILE */
