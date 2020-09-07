/******************************************************************************
* \file             adc.c
*
* \version          1.20
*
* \brief            12 bit ADC (ADC) driver  
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
#include "adc/adc.h"

#if (defined(PDL_PERIPHERAL_ADC_ACTIVE))

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/// Look-up table for all enabled ADC instances and their internal data
stc_adc_instance_data_t m_astcAdcInstanceDataLut[ADC_INSTANCE_COUNT] =
{
#if (PDL_PERIPHERAL_ENABLE_ADC0 == PDL_ON)
    { 
        &ADC0,  // pstcInstance
        {
            0u, 0u ,0u, 0u, 0u ,
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)     
            0u
        #endif    
        }           
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_ADC1 == PDL_ON)
    {  
        &ADC1,  // pstcInstance
        {
            0u, 0u , 0u, 0u, 0u,
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)         
            0u
        #endif      
        }           
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_ADC2 == PDL_ON)
    {  
        &ADC2,  // pstcInstance
        {
            0u, 0u, 0u, 0u, 0u ,
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)        
            0u
        #endif      
        }           
    }
#endif
};

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/
#if (PDL_INTERRUPT_ENABLE_ADC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_ADC1 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_ADC2 == PDL_ON)
static void Adc_InitNvic(volatile stc_adcn_t* pstcAdc);
static void Adc_DeInitNvic(volatile stc_adcn_t* pstcAdc);
static stc_adc_intern_data_t* AdcGetInternDataPtr(volatile stc_adcn_t* pstcAdc);
#endif

/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/

/******************************************************************************/
/* Function implementation - global ('extern') and local ('static')           */
/******************************************************************************/

/******************************************************************************/
/* Local Functions                                                            */
/******************************************************************************/

#if (PDL_INTERRUPT_ENABLE_ADC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_ADC1 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_ADC2 == PDL_ON)      
/**
 ******************************************************************************
 ** \brief ADC instance interrupt service routine
 **
 ** ADC instance interrupt service routine, clear interrupt cause and
 ** implement interrupt callback function.
 **
 ** \param  pstcAdc              Pointer to ADC instance
 ** \param  pstcAdcInternData    Pointer to ADC intern data
 **        
 ******************************************************************************/
void AdcIrqHandler( volatile stc_adcn_t*   pstcAdc,
                    stc_adc_intern_data_t* pstcAdcInternData)
{
    if (1u == pstcAdc->SCCR_f.SOVR)   // Error case (FIFO overflow)
    {
        pstcAdc->SCCR_f.SOVR = 0u;

        if (pstcAdcInternData->pfnScanErrIrqCb != NULL)
        {
            pstcAdcInternData->pfnScanErrIrqCb();
        }
    }

    if (1u == pstcAdc->PCCR_f.POVR)  // Error case (Priority FIFO overflow)
    {
        pstcAdc->PCCR_f.POVR = 0u;

        if (pstcAdcInternData->pfnPrioErrIrqCb != NULL)
        {
            pstcAdcInternData->pfnPrioErrIrqCb();
        }
    }

    if (1u == pstcAdc->ADCR_f.PCIF)  // Priority Conversion
    {
        pstcAdc->ADCR_f.PCIF = 0u;

        if (pstcAdcInternData->pfnPrioIrqCb != NULL)
        {
            // Callback argument points to priority FIFO. User has to empty it in
            // the callback function, because the FIFO depth is known.
            pstcAdcInternData->pfnPrioIrqCb((volatile uint32_t*)&pstcAdc->PCFD);
        }
    }

    if (1u == pstcAdc->ADCR_f.SCIF)   // Scan conversion interrupt request?
    {
        pstcAdc->ADCR_f.SCIF = 0u;

        if (pstcAdcInternData->pfnScanIrqCb != NULL)
        {
            // Callback argument points to scan FIFO. User has to empty it in
            // the callback function, because the FIFO depth is known. 
            pstcAdcInternData->pfnScanIrqCb((volatile uint32_t*)&pstcAdc->SCFD);
        }
    }

    if (1u == pstcAdc->ADCR_f.CMPIF)  // Compare result interrupt request?
    {
        pstcAdc->ADCR_f.CMPIF = 0u;

        if (pstcAdcInternData->pfnComparisonIrqCb != NULL)
        {
            pstcAdcInternData->pfnComparisonIrqCb();
        }
    }
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
    if (1u == pstcAdc->WCMRCIF_f.RCINT)  // Range result interrupt request?
    {
        pstcAdc->WCMRCIF_f.RCINT = 0u;

        if (pstcAdcInternData->pfnRangeComparisonIrqCb != NULL)
        {
            pstcAdcInternData->pfnRangeComparisonIrqCb();
        }
    }
#endif
    return;
} // AdcIrqHandler

/**
 ******************************************************************************
 ** \brief Set NVIC Interrupt depending on ADC instance
 **
 ** \param pstcAdc Pointer to ADC instance
 **
 ******************************************************************************/
static void Adc_InitNvic(volatile stc_adcn_t* pstcAdc)
{
#if (PDL_INTERRUPT_ENABLE_ADC0 == PDL_ON)  
    if (pstcAdc == (volatile stc_adcn_t*)(&ADC0))
    {
        NVIC_ClearPendingIRQ(ADC0_IRQn);
        NVIC_EnableIRQ(ADC0_IRQn);
        NVIC_SetPriority(ADC0_IRQn, PDL_IRQ_LEVEL_ADC0);   
    }
#endif    
#if (PDL_INTERRUPT_ENABLE_ADC1 == PDL_ON)     
    if (pstcAdc == (volatile stc_adcn_t*)(&ADC1))
    {
        NVIC_ClearPendingIRQ(ADC1_IRQn);
        NVIC_EnableIRQ(ADC1_IRQn);
        NVIC_SetPriority(ADC1_IRQn, PDL_IRQ_LEVEL_ADC1);      
    }
#endif
#if (PDL_INTERRUPT_ENABLE_ADC2 == PDL_ON)     
    if (pstcAdc == (volatile stc_adcn_t*)(&ADC2))
    {
        NVIC_ClearPendingIRQ(ADC2_IRQn);
        NVIC_EnableIRQ(ADC2_IRQn);
        NVIC_SetPriority(ADC2_IRQn, PDL_IRQ_LEVEL_ADC2);    
    } 
#endif    
} // Adc_InitInterrupt

/**
 ******************************************************************************
 ** \brief Clear NVIC Interrupt depending on ADC instance
 **
 ** \param pstcAdc Pointer to ADC instance
 **
 ******************************************************************************/
static void Adc_DeInitNvic(volatile stc_adcn_t* pstcAdc)
{
    /* Only when all of ADC interrupt sources are disable, disable ADC IRQ */
#if (PDL_INTERRUPT_ENABLE_ADC0 == PDL_ON) 
    if (pstcAdc == (volatile stc_adcn_t*)(&ADC0))
    {
        NVIC_ClearPendingIRQ(ADC0_IRQn);
        NVIC_DisableIRQ(ADC0_IRQn);
        NVIC_SetPriority(ADC0_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);   
    }
#endif 
#if (PDL_INTERRUPT_ENABLE_ADC1 == PDL_ON)    
    if (pstcAdc == (volatile stc_adcn_t*)(&ADC1))
    {
        NVIC_ClearPendingIRQ(ADC1_IRQn);
        NVIC_DisableIRQ(ADC1_IRQn);
        NVIC_SetPriority(ADC1_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);      
    }
#endif
#if (PDL_INTERRUPT_ENABLE_ADC2 == PDL_ON)    
    if (pstcAdc == (volatile stc_adcn_t*)(&ADC2))
    {
        NVIC_ClearPendingIRQ(ADC2_IRQn);
        NVIC_DisableIRQ(ADC2_IRQn);
        NVIC_SetPriority(ADC2_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);     
    }
#endif    
} // Adc_DeInitInterrupt
#endif // #if (PDL_INTERRUPT_ENABLE_ADC0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_ADC1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_ADC2_LCD == PDL_ON)


/**
 ******************************************************************************
 ** \brief Return the internal data for a certain ADC instance.
 **
 ** \param pstcAdc Pointer to ADC instance
 **
 ** \return Pointer to internal data or NULL if instance is not enabled (or not known)
 **
 ******************************************************************************/
static stc_adc_intern_data_t* AdcGetInternDataPtr(volatile stc_adcn_t* pstcAdc) 
{
    uint8_t u8Instance;
   
    for (u8Instance = 0u; u8Instance < ADC_INSTANCE_COUNT; u8Instance++)
    {
        if (pstcAdc == m_astcAdcInstanceDataLut[u8Instance].pstcInstance)
        {
            return &m_astcAdcInstanceDataLut[u8Instance].stcInternData;
        }
    }

    return NULL;
}


/******************************************************************************/
/* Global Functions                                                           */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief Initialize ADC
 **
 ** This function initializes an ADC module
 **
 ** \param [in]  pstcAdc          ADC instance
 ** \param [in]  pstcConfig       ADC module configuration 
 **
 ** \retval Ok                    ADC initialized normally
 ** \retval ErrorInvalidParameter if one of following case matches:
 **                               - pstcAdc == NULL
 **                               - pstcConfig == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Adc_Init( volatile stc_adcn_t* pstcAdc,
                      const stc_adc_config_t*    pstcConfig )
{
	  //stc_adc0_pcis_field_t  stcPCIS;
    uint8_t u8PCIS = 0u;
    uint8_t u8CMPCR = 0u;
	
    // Pointer to internal data
    stc_adc_intern_data_t* pstcAdcInternData ; 
    
    // Registers bit field structure
    stc_adc_sccr_field_t  stcSCCR;
    stc_adc_pccr_field_t  stcPCCR;
    stc_adc_adst01_field_t stcADST01;
    
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)      
    stc_adc_wcmpcr_field_t stcWCMPCR; 
#endif    
  
    // Check for NULL pointer
    if ((NULL == pstcAdc    ) ||
        (NULL == pstcConfig ))
    {
        return ErrorInvalidParameter ;
    } 
    
    // Get pointer to internal data structure ...
    pstcAdcInternData = AdcGetInternDataPtr( pstcAdc ) ;
    
    // Check for instance available or not
    if(pstcAdcInternData == NULL)
    {
        return ErrorInvalidParameter ;
    }
    
    PDL_ZERO_STRUCT(stcSCCR);
    PDL_ZERO_STRUCT(stcPCCR);
    PDL_ZERO_STRUCT(stcADST01);

#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)      
    PDL_ZERO_STRUCT(stcWCMPCR);
#endif
    
    // Disable ADC in any case first
    pstcAdc->ADCEN_f.ENBL = 0u;

    // Clear all relevant registers
    pstcAdc->ADCR = 0u;
    pstcAdc->ADSR = 0u;
    pstcAdc->SCCR = 0u;

    // Set Sampling Time Selection Register
    pstcAdc->ADSS01 = (uint16_t)(0x0000FFFFul & pstcConfig->u32SamplingTimeSelect.u32AD_CHn);

    pstcAdc->ADSS23 = (uint16_t)((0xFFFF0000ul &  pstcConfig->u32SamplingTimeSelect.u32AD_CHn) >> 16u);


    // Set Sampling Times Config. 0
    switch (pstcConfig->enSamplingTimeN0)
    {
        case Value1:
            stcADST01.STX0 = 0u;
            break;
        case Value4:
            stcADST01.STX0 = 1u;
            break;
        case Value8:
            stcADST01.STX0 = 2u;
            break;
        case Value16:
            stcADST01.STX0 = 3u;
            break;
        case Value32:
            stcADST01.STX0 = 4u;
            break;
        case Value64:
            stcADST01.STX0 = 5u;
            break;
        case Value128:
            stcADST01.STX0 = 6u;
            break;
        case Value256:
            stcADST01.STX0 = 7u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    if (pstcConfig->u8SamplingTime0 > 31u)
    {
        return ErrorInvalidParameter;
    }
    
    stcADST01.ST0 = pstcConfig->u8SamplingTime0;


    
    // Set Sampling Times Config. 1
    switch (pstcConfig->enSamplingTimeN1)
    {
        case Value1:   
            stcADST01.STX1 = 0u;
            break;
        case Value4:
            stcADST01.STX1 = 1u; 
            break;
        case Value8:
            stcADST01.STX1 = 2u; 
            break;
        case Value16:
            stcADST01.STX1 = 3u; 
            break;
        case Value32:
            stcADST01.STX1 = 4u; 
            break;
        case Value64:
            stcADST01.STX1 = 5u; 
            break;
        case Value128:
            stcADST01.STX1 = 6u; 
            break;
        case Value256:
            stcADST01.STX1 = 7u; 
            break;
        default:
            return ErrorInvalidParameter;
    }

    if (pstcConfig->u8SamplingTime1 > 31u)
    {
        return ErrorInvalidParameter;
    }

    stcADST01.ST1 = pstcConfig->u8SamplingTime1;

    pstcAdc->ADST01_f = stcADST01;

    // Frequency Division for ADC Instance
    pstcAdc->ADCT = pstcConfig->u8ComparingClockDiv;
       
    // MSB, LSB alignment
    pstcAdc->ADSR_f.FDAS = (TRUE == pstcConfig->bLsbAlignment) ? 1u : 0u;
    
#if (PDL_ADC_TYPE == PDL_ADC_A)
    #if((PDL_MCU_TYPE != PDL_FM3_TYPE0)) 
    switch (pstcConfig->enEnableCycle)
    {
        case AdcEnableCycle36:
            pstcAdc->ADCEN_f.CYCLSL = 0u;
            break;
        case AdcEnableCycle20:
            pstcAdc->ADCEN_f.CYCLSL = 1u;
            break;
        case AdcEnableCycle9:
            pstcAdc->ADCEN_f.CYCLSL = 2u;
            break;
        case AdcEnableCycle44:
            pstcAdc->ADCEN_f.CYCLSL = 3u;
            break;    
        default:
            return ErrorInvalidParameter;
    }
    #endif
#elif (PDL_ADC_TYPE == PDL_ADC_B) 
    // Set enable time
    pstcAdc->ADCEN &= 0x00FFu;
    pstcAdc->ADCEN |= (uint16_t)((uint16_t)pstcConfig->u8EnableTime << 8u);
#endif  
    // Initialize scan fucntion
    if(NULL != pstcConfig->pstcScanInit)
    {
        // Set Scan Conversion Input Selection Register
        // [andreika]: fix! uint8_t->uint16_t
        pstcAdc->SCIS01 = (uint16_t)(0x0000FFFFul & pstcConfig->pstcScanInit->u32ScanCannelSelect.u32AD_CHn);

        pstcAdc->SCIS23 = (uint16_t)((0xFFFF0000ul & pstcConfig->pstcScanInit->u32ScanCannelSelect.u32AD_CHn) >> 16u);


        // Conversion mode single, repeated
        switch (pstcConfig->pstcScanInit->enScanMode)
        {
            case ScanSingleConversion:
                stcSCCR.RPT = 0u;
                break;
            case ScanRepeatConversion:
                stcSCCR.RPT = 1u;
                break;
            default:
                return ErrorInvalidParameter;
        }

      #if defined(FM_ADC0_SCTSL_AVAILABLE)
        // Scan Conversion Timer Start Enable and Timer Source
        if (TRUE == pstcConfig->pstcScanInit->bScanTimerStartEnable)
        {
            stcSCCR.SHEN = 1u;
            pstcAdc->SCTSL = (uint8_t) pstcConfig->pstcScanInit->enScanTimerTrigger;
        }
      #endif

        // Update hardware
        pstcAdc->SCCR_f = stcSCCR;

        // Scan Conversion FIFO Depth
        if (pstcConfig->pstcScanInit->u8ScanFifoDepth > 15u)
        {
            return ErrorInvalidParameter;
        }

        pstcAdc->SFNS = pstcConfig->pstcScanInit->u8ScanFifoDepth;

    } 
  
    // Initialize priority fucntion
    if(NULL != pstcConfig->pstcPrioInit)
    {  
        // Priority external Trigger Analog Input
        stcPCCR.ESCE = 0u;

        // Priority external Trigger Start Enable
        stcPCCR.PEEN = (TRUE == pstcConfig->pstcPrioInit->bPrioExtTrigStartEnable) ? 1u : 0u;

      #if defined(FM_ADC0_PRTSL_AVAILABLE)
        // Priority Conversion Timer Start Enable and Timer Source
        if (TRUE == pstcConfig->pstcPrioInit->bPrioTimerStartEnable)
        {
            stcPCCR.PHEN = 1u;
            pstcAdc->PRTSL = (uint8_t) pstcConfig->pstcPrioInit->enPrioTimerTrigger;
        }
      #endif
        
        // Update Hardware
        pstcAdc->PCCR_f = stcPCCR;

        // Priority Conversion FIFO Stage Count Setup
        pstcAdc->PFNS = pstcConfig->pstcPrioInit->u8PrioFifoDepth;

        // Priority Conversion Input Selection
        if (pstcConfig->pstcPrioInit->u8PrioLevel1AnalogChSel > 7u)
        {
            return ErrorInvalidParameter;
        }
        u8PCIS = pstcConfig->pstcPrioInit->u8PrioLevel1AnalogChSel & 0x07u;

        if (pstcConfig->pstcPrioInit->u8PrioLevel2AnalogChSel > 31u)
        {
            return ErrorInvalidParameter;
        }
        u8PCIS |= (uint8_t)((uint8_t)(0x1Fu & pstcConfig->pstcPrioInit->u8PrioLevel2AnalogChSel) << 3u);

        pstcAdc->PCIS = u8PCIS;
    }
  
    // Initialize comparison fucntion
    if(NULL != pstcConfig->pstcComparisonInit)
    {
        // Comparison Value
        pstcAdc->CMPD = (((pstcConfig->pstcComparisonInit->u16CompareValue >> 2u)) << 6u);
        
        if (pstcConfig->pstcComparisonInit->u8CompareChannel > 31u)
        {
            return ErrorInvalidParameter;
        }
        
        // Comparison Control
        if(pstcConfig->pstcComparisonInit->bCompIrqEqualGreater == TRUE)
        {
            u8CMPCR |= 0x40u;
        }
        
        if(TRUE == pstcConfig->pstcComparisonInit->bCompareAllChannels)
        {
            u8CMPCR |= 0x20u;
        }

        u8CMPCR |= 0x80u; // Enable comparison function
        u8CMPCR |= (0x1Fu & pstcConfig->pstcComparisonInit->u8CompareChannel);
        
        pstcAdc->CMPCR = u8CMPCR;  
    }
  
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)       
    if (NULL != pstcConfig->pstcRangeComparisonInit)
    {
        // Set lower and upper value of the range
        pstcAdc->WCMPDH = ((pstcConfig->pstcRangeComparisonInit->u16UpperLimitRangeValue >> 2u) << 6u);
        pstcAdc->WCMPDL = ((pstcConfig->pstcRangeComparisonInit->u16LowerLimitRangeValue >> 2u) << 6u);

        if (pstcConfig->pstcRangeComparisonInit->u8RangeCountValue > 7u)
        {
            return ErrorInvalidParameter;
        }

        // Set compare times
        stcWCMPCR.RCOCD = pstcConfig->pstcRangeComparisonInit->u8RangeCountValue;
        // In-range compare or out-range compare
        stcWCMPCR.RCOIRS = pstcConfig->pstcRangeComparisonInit->bWithinRange;
        // Enable range compare function
        stcWCMPCR.RCOE = 1;
        // Compare 1 channel or all
        pstcAdc->WCMPSR_f.WCMD = pstcConfig->pstcRangeComparisonInit->bRangeCompareAllChannels;

        if (pstcConfig->pstcRangeComparisonInit->u8RangeCompareChannel > 31u)
        {
            return ErrorInvalidParameter;
        }

        // Set the channel compared (according to single channel compare mode)
        pstcAdc->WCMPSR_f.WCCH |= pstcConfig->pstcRangeComparisonInit->u8RangeCompareChannel;

        // update hardware
        pstcAdc->WCMPCR_f = stcWCMPCR;
    }
#endif

#if (PDL_INTERRUPT_ENABLE_ADC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_ADC1 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_ADC2 == PDL_ON)    
    // Initialize interrupts
    if(NULL != pstcConfig->pstcIrqEn)
    {
        if(TRUE == pstcConfig->pstcIrqEn->bScanIrq)
        {
            pstcAdc->ADCR_f.SCIE = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bPrioIrq)
        {
            pstcAdc->ADCR_f.PCIE = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bComparisonIrq)
        {
            pstcAdc->ADCR_f.CMPIE = 1u;
        }
    #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)     
        if(TRUE == pstcConfig->pstcIrqEn->bRangeComparisonIrq)
        {
            pstcAdc->WCMPCR_f.RCOIE = 1u;
        }
    #endif    
        if(TRUE == pstcConfig->pstcIrqEn->bFifoOverrunIrq)
        {
            pstcAdc->ADCR_f.OVRIE = 1u;
        }
    }
    
    // Initialize interrupt callback functions
    if (NULL != pstcConfig->pstcIrqCb)  
    {
        pstcAdcInternData->pfnScanIrqCb = pstcConfig->pstcIrqCb->pfnScanIrqCb;
        pstcAdcInternData->pfnPrioIrqCb = pstcConfig->pstcIrqCb->pfnPrioIrqCb;
        pstcAdcInternData->pfnScanErrIrqCb = pstcConfig->pstcIrqCb->pfnScanErrIrqCb;
        pstcAdcInternData->pfnPrioErrIrqCb = pstcConfig->pstcIrqCb->pfnPrioErrIrqCb;
        pstcAdcInternData->pfnComparisonIrqCb = pstcConfig->pstcIrqCb->pfnComparisonIrqCb;
    #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
        pstcAdcInternData->pfnRangeComparisonIrqCb = pstcConfig->pstcIrqCb->pfnRangeComparisonIrqCb;
    #endif    
    }
    
    // Initialize NVIC
    if(TRUE == pstcConfig->bTouchNvic)
    {
        Adc_InitNvic(pstcAdc);
    }
#endif 
    
    return Ok;
} // Adc_Init

/**
 ******************************************************************************
 ** \brief De-Initialize ADC
 **
 ** This function deinitializes an ADC module
 **
 ** \param [in]  pstcAdc          ADC instance
 ** \param [in]  bTouchNvic       Touch NVIC or not
 **
 ** \retval Ok                    ADC instance fully disabled and reset
 ** \retval ErrorInvalidParameter If one of following case matches:
 **                               - pstcAdc == NULL
 **                               - pstcAdcInternData == NULL
 **
 ******************************************************************************/
en_result_t Adc_DeInit( volatile stc_adcn_t* pstcAdc, boolean_t bTouchNvic )
{
    // Pointer to internal data
    stc_adc_intern_data_t* pstcAdcInternData ; 
  
    // Check for NULL pointer
    if (NULL == pstcAdc)
    {
        return ErrorInvalidParameter ;
    }
    
    // Get pointer to internal data structure ...
    pstcAdcInternData = AdcGetInternDataPtr( pstcAdc ) ;
    
    // Check for instance available or not
    if(NULL == pstcAdcInternData)
    {
        return ErrorInvalidParameter ;
    }

    pstcAdc->ADCEN   = 0u;   // Diasble ADC (including ENBL bit)
    pstcAdc->ADCR    = 0u;
    pstcAdc->ADSR    = 0u;
    pstcAdc->SCCR    = 0u;
    pstcAdc->SFNS    = 0u;
    pstcAdc->SCIS01  = 0u;

    pstcAdc->SCIS23  = 0u;

    pstcAdc->PCCR    = 0u;
    pstcAdc->PFNS    = 0u;
    pstcAdc->PCFD    = 0u;
    pstcAdc->PCIS    = 0u;
    pstcAdc->CMPD    = 0u;
    pstcAdc->CMPCR   = 0u;
    pstcAdc->ADSS01  = 0u;

    pstcAdc->ADSS23  = 0u;

    pstcAdc->ADST01  = 0u;

    pstcAdc->ADCEN   = 0u;
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)      
    pstcAdc->WCMPDH  = 0u;
    pstcAdc->WCMPCR  = 0u;
    pstcAdc->WCMPDL  = 0u;
    pstcAdc->WCMPSR  = 0u;
    pstcAdc->WCMRCOT = 0u;
    pstcAdc->WCMRCIF = 0u;
#endif   
#if defined(FM_ADC0_SCTSL_AVAILABLE) && defined(FM_ADC0_PRTSL_AVAILABLE)
    pstcAdc->SCTSL   = 0u;
    pstcAdc->PRTSL   = 0u;
#endif
    
#if (PDL_INTERRUPT_ENABLE_ADC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_ADC1 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_ADC2 == PDL_ON)    
    pstcAdcInternData->pfnScanIrqCb = NULL;
    pstcAdcInternData->pfnPrioIrqCb = NULL;
    pstcAdcInternData->pfnScanErrIrqCb = NULL;
    pstcAdcInternData->pfnPrioErrIrqCb = NULL;
    pstcAdcInternData->pfnComparisonIrqCb = NULL;    
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
    pstcAdcInternData->pfnRangeComparisonIrqCb = NULL;
#endif   
    
    if(TRUE == bTouchNvic)
    {
        Adc_DeInitNvic(pstcAdc);
    }
#endif    
    
    return Ok;
} // Adc_DeInit

#if (PDL_INTERRUPT_ENABLE_ADC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_ADC1 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_ADC2 == PDL_ON)
/**
 ******************************************************************************
 ** \brief Enable ADC interrupt
 **
 ** This function enable the ADC interrupts which are selected.
 **
 ** \param [in]  pstcAdc          ADC instance
 ** \param [in]  pstcIrqSel       ADC interrupt selection
 **
 ** \retval Ok                    Enable interrupt normally
 ** \retval ErrorInvalidParameter If one of following cases matches:
 **                               - pstcAdc == NULL
 **                               - pstcIrqSel == NULL
 **                               - Other invalid configuration
 ******************************************************************************/      
en_result_t   Adc_EnableIrq( volatile stc_adcn_t* pstcAdc,
                             stc_adc_irq_sel_t* pstcIrqSel)
{
    // Check for NULL pointer
    if ((NULL == pstcAdc) || (NULL == pstcIrqSel))
    {
        return ErrorInvalidParameter ;
    }
    
    if(TRUE == pstcIrqSel->bScanIrq)
    {
        pstcAdc->ADCR_f.SCIE = 1u;
    }
    
    if(TRUE == pstcIrqSel->bPrioIrq)
    {
        pstcAdc->ADCR_f.PCIE = 1u;
    }
    
    if(TRUE == pstcIrqSel->bComparisonIrq)
    {
        pstcAdc->ADCR_f.CMPIE = 1u;
    }
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
    if(TRUE == pstcIrqSel->bRangeComparisonIrq)
    {
        pstcAdc->WCMPCR_f.RCOIE = 1u;
    }
#endif    
    if(TRUE == pstcIrqSel->bFifoOverrunIrq)
    {
        pstcAdc->ADCR_f.OVRIE = 1u;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable ADC interrupt
 **
 ** This function disable the ADC interrupts which are selected.
 **
 ** \param [in]  pstcAdc          ADC instance
 ** \param [in]  pstcIrqSel       ADC interrupt selection 
 **
 ** \retval Ok                    Disable interrupt normally
 ** \retval ErrorInvalidParameter If one of following cases matches:
 **                               - pstcAdc == NULL
 **                               - pstcIrqSel == NULL
 ******************************************************************************/    
en_result_t   Adc_DisableIrq( volatile stc_adcn_t* pstcAdc,
                              stc_adc_irq_sel_t* pstcIrqSel)
{
    // Check for NULL pointer
    if ((NULL == pstcAdc) || (NULL == pstcIrqSel))
    {
        return ErrorInvalidParameter ;
    }
    
    if(TRUE == pstcIrqSel->bScanIrq)
    {
        pstcAdc->ADCR_f.SCIE = 0u;
    }
    
    if(TRUE == pstcIrqSel->bPrioIrq)
    {
        pstcAdc->ADCR_f.PCIE = 0u;
    }
    
    if(TRUE == pstcIrqSel->bComparisonIrq)
    {
        pstcAdc->ADCR_f.CMPIE = 0u;
    }
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
    if(TRUE == pstcIrqSel->bRangeComparisonIrq)
    {
        pstcAdc->WCMPCR_f.RCOIE = 0u;
    }
#endif    
    if(TRUE == pstcIrqSel->bFifoOverrunIrq)
    {
        pstcAdc->ADCR_f.OVRIE = 0u;
    }
    
    return Ok;
}
#endif

/**
 ******************************************************************************
 ** \brief Get ADC interrupt flag
 **
 ** This function gets the ADC interrupt flag which is selected.
 **
 ** \param [in]  pstcAdc          ADC instance
 ** \param [in]  enIrq            ADC interrupt type
 ** \arg         AdcScanIrq              Scan conversion interrupt flag
 ** \arg         AdcPrioIrq              Priority scan conversion interrupt flag
 ** \arg         AdcScanFifoOverrunIrq   Scan FIFO overrun interrupt flag
 ** \arg         AdcPrioFifoOverrunIrq   Priority FIFO overrun interrupt flag
 ** \arg         AdcComparisonIrq        Comparison interrupt flag
 ** \arg         AdcRangeComparisonIrq   Range comparison interrupt flag
 **
 ** \retval TRUE                  The data count in scan FIFO match scan FIFO stage [AdcScanInt]   
 **                               The data count in priority FIFO match priority FIFO stage [AdcPrioInt]
 **                               Scan FIFO overruns [AdcScanFifoOverrunInt]
 **                               Priority FIFO overruns [AdcPrioFifoOverrunInt]
 **                               The comparison result match the condition set in the Adc_Init() [AdcComparisonInt]
 **                               The range comparison result match the condition set in the Adc_Init() [AdcRangeComparisonInt]
 ** \retval FALSE                 The data count in scan FIFO don't match scan FIFO stage [AdcScanInt]
 **                               The data count in priority FIFO don't match priority FIFO stage [AdcPrioInt]
 **                               Scan FIFO don't overrun [AdcScanFifoOverrunInt]
 **                               Priority FIFO don't overrun [AdcPrioFifoOverrunInt]
 **                               The comparison result don't match the condition set in the Adc_Init() [AdcComparisonInt]
 **                               The range comparison result don't match the condition set in the Adc_Init() [AdcRangeComparisonInt]
 **
 ******************************************************************************/    
boolean_t   Adc_GetIrqFlag(volatile stc_adcn_t* pstcAdc, en_adc_irq_t enIrq)
{
    boolean_t bRet = FALSE;
    switch(enIrq)
    {
        case AdcScanIrq:
            bRet = (pstcAdc->ADCR_f.SCIF == 1u) ? TRUE : FALSE;
            break;
        case AdcPrioIrq:
            bRet = (pstcAdc->ADCR_f.PCIF == 1u) ? TRUE : FALSE;
            break;
        case AdcScanFifoOverrunIrq:
            bRet = (pstcAdc->SCCR_f.SOVR == 1u) ? TRUE : FALSE;
            break;
        case AdcPrioFifoOverrunIrq:
            bRet = (pstcAdc->PCCR_f.POVR == 1u) ? TRUE : FALSE;
            break;    
        case AdcComparisonIrq:
            bRet = (pstcAdc->ADCR_f.CMPIF == 1u) ? TRUE : FALSE;
            break;
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)              
        case AdcRangeComparisonIrq:
            bRet = (pstcAdc->WCMRCIF_f.RCINT == 1u) ? TRUE : FALSE;
            break;
#endif            
        default:
            break;
    }
    
    return bRet;
}

/**
 ******************************************************************************
 ** \brief Clear ADC interrupt flag
 **
 ** This function clears the ADC interrupt which is selected.
 **
 ** \param [in]  pstcAdc          ADC instance
 ** \param [in]  enIrq            Interrupt type
 ** \arg         AdcScanIrq              Scan conversion interrupt flag
 ** \arg         AdcPrioIrq              Priority scan conversion interrupt flag
 ** \arg         AdcScanFifoOverrunIrq   Scan FIFO overrun interrupt flag
 ** \arg         AdcPrioFifoOverrunIrq   Priority FIFO overrun interrupt flag
 ** \arg         AdcComparisonIrq        Comparison interrupt flag
 ** \arg         AdcRangeComparisonIrq   Range comparison interrupt flag
 **
 ** \retval Ok                    Disable interrupt normally
 ** \retval ErrorInvalidParameter pstcAdc == NULL 
 **                               Other invalid configuration
 ******************************************************************************/
en_result_t Adc_ClrIrqFlag(volatile stc_adcn_t* pstcAdc, en_adc_irq_t enIrq)
{
    // Check for NULL pointer
    if (NULL == pstcAdc)
    {
        return ErrorInvalidParameter ;
    }
    
    switch(enIrq)
    {
        case AdcScanIrq:
            pstcAdc->ADCR_f.SCIF = 0u;
            break;
        case AdcPrioIrq:
            pstcAdc->ADCR_f.PCIF = 0u;
            break;
        case AdcScanFifoOverrunIrq:
            pstcAdc->SCCR_f.SOVR = 0u;
            break;
        case AdcPrioFifoOverrunIrq:
            pstcAdc->PCCR_f.POVR = 0u;
            break;    
        case AdcComparisonIrq:
            pstcAdc->ADCR_f.CMPIF = 0u;
            break;
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)            
        case AdcRangeComparisonIrq:
            pstcAdc->WCMRCIF_f.RCINT = 0u;
            break;
#endif            
        default:
            return ErrorInvalidParameter ;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Enable ADC
 **
 ** This function enables an ADC instance (Does not wait for readiness)
 **
 ** \param [in]  pstcAdc          ADC instance
 **
 ** \retval Ok                    ADC enable bit set (does not indicate readiness!)
 ** \retval ErrorInvalidParameter pstcAdc == NULL
 ******************************************************************************/
en_result_t Adc_Enable( volatile stc_adcn_t* pstcAdc )
{
    // Check for NULL pointer
    if (NULL == pstcAdc)
    {
        return ErrorInvalidParameter ;
    }

    // Enable ADC
    pstcAdc->ADCEN_f.ENBL = 1u;

    return Ok;
} // Adc_Enable

/**
 ******************************************************************************
 ** \brief Check ADC Readiness
 **
 ** This function checks for ADC instance readiness
 **
 ** \param [in]  pstcAdc          ADC instance
 **
 ** \retval Ok                    ADC instance ready
 ** \retval ErrorNotReady         ADC instance not ready
 ** \retval ErrorInvalidParameter pstcAdc == NULL
 ******************************************************************************/
en_result_t Adc_Ready( volatile stc_adcn_t* pstcAdc )
{
    // Check for NULL pointer
    if (NULL == pstcAdc)
    {
        return ErrorInvalidParameter ;
    }

    if (FALSE == pstcAdc->ADCEN_f.READY)
    {
        return ErrorNotReady;
    }

    return Ok;
} // Adc_Ready

/**
 ******************************************************************************
 ** \brief Enable ADC and wait for Readiness
 **
 ** This function enables an ADC instance and waits for readiness
 **
 ** \param [in]  pstcAdc          ADC instance
 **
 ** \retval Ok                    ADC instance enabled and ready
 ** \retval ErrorTimeout          ADC instance not ready
 ** \retval ErrorInvalidParameter pstcAdc == NULL
 ******************************************************************************/
en_result_t Adc_EnableWaitReady( volatile stc_adcn_t* pstcAdc )
{
    uint32_t u32TimeOutCounter = PDL_ADC_READY_WAIT_COUNT;

    // Check for NULL pointer
    if (NULL == pstcAdc)
    {
        return ErrorInvalidParameter ;
    }

    // Enable ADC
    pstcAdc->ADCEN_f.ENBL = 1u;

    // Polling for readiness
    while (u32TimeOutCounter)
    {
        if (TRUE == pstcAdc->ADCEN_f.READY)
        {
            return Ok;
        }

        u32TimeOutCounter--;
    }

    return ErrorTimeout;
} // Adc_EnableWaitReady

/**
 ******************************************************************************
 ** \brief Disable ADC
 **
 ** This function disables an ADC operation
 **
 ** \param [in]  pstcAdc          ADC instance
 **
 ** \retval Ok                    ADC instance disabled
 ** \retval ErrorTimeout          ADC instance not ready
 ** \retval ErrorInvalidParameter pstcAdc == NULL
 ******************************************************************************/
en_result_t     Adc_Disable( volatile stc_adcn_t* pstcAdc )
{
    // Check for NULL pointer
    if (NULL == pstcAdc)
    {
        return ErrorInvalidParameter ;
    }
    
    // Disable ADC
    pstcAdc->ADCEN_f.ENBL = 0u;
    
    return Ok;
}
                   
/**
 ******************************************************************************
 ** \brief ADC Scan Software Start Trigger
 **
 ** This function starts an AD Scan Conversion by Software trigger
 **
 ** \param [in]  pstcAdc          ADC instance
 **
 ** \retval Ok                    ADC instance triggered (or re-triggered)
 ** \retval ErrorInvalidParameter pstcAdc == NULL
 ******************************************************************************/
en_result_t Adc_SwTriggerScan( volatile stc_adcn_t* pstcAdc )
{
    // Check for NULL pointer
    if (NULL == pstcAdc)
    {
        return ErrorInvalidParameter ;
    }

    pstcAdc->SCCR_f.SSTR = 1u;     // Trigger AD Conversion

    return Ok;
} // Adc_SwStart

/**
 ******************************************************************************
 ** \brief Stop Scan conversion with repeat mode
 **
 ** This function stops an AD Scan Conversion with repeat mode
 **
 ** \param [in]  pstcAdc          ADC instance
 **
 ** \retval Ok                    ADC scan conversion stopped
 ** \retval ErrorInvalidParameter pstcAdc == NULL
 ******************************************************************************/
en_result_t  Adc_StopScanRepeat( volatile stc_adcn_t* pstcAdc )
{
    // Check for NULL pointer
    if (NULL == pstcAdc)
    {
        return ErrorInvalidParameter ;
    }
    
    pstcAdc->SCCR_f.RPT = 0u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Start Scan conversion with repeat mode
 **
 ** This function starts an AD Scan Conversion with repeat mode
 **
 ** \param [in]  pstcAdc          ADC instance
 **
 ** \retval Ok                    ADC scan conversion started
 ** \retval ErrorInvalidParameter pstcAdc == NULL
 ******************************************************************************/
en_result_t   Adc_StartScanRepeat( volatile stc_adcn_t* pstcAdc )
{
    // Check for NULL pointer
    if (NULL == pstcAdc)
    {
        return ErrorInvalidParameter ;
    }
    
    pstcAdc->SCCR_f.RPT = 1u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief ADC Priority Software Start Trigger
 **
 ** This function starts an AD Priority Conversion by Software trigger
 **
 ** \param [in]  pstcAdc          ADC instance
 **
 ** \retval Ok                    ADC instance triggered (or re-triggered)
 ** \retval ErrorInvalidParameter pstcAdc == NULL
 ******************************************************************************/
en_result_t Adc_SwTriggerPrio( volatile stc_adcn_t* pstcAdc )
{
  // Check for NULL pointer
  if (NULL == pstcAdc)
  {
    return ErrorInvalidParameter ;
  }
  
  pstcAdc->PCCR_f.PSTR = 1u;     // Trigger AD Conversion
  
  return Ok;
}
                      
/**
 ******************************************************************************
 ** \brief ADC Software Stop
 **
 ** This function requests a stop of the ADC
 **
 ** \param [in]  pstcAdc          ADC instance
 **
 ** \retval Ok                    ADC instance stop request
 ** \retval ErrorInvalidParameter pstcAdc == NULL
 ******************************************************************************/
en_result_t Adc_ForceStop( volatile stc_adcn_t* pstcAdc )
{
    // Check for NULL pointer
    if (NULL == pstcAdc)
    {
        return ErrorInvalidParameter ;
    }

    pstcAdc->ADSR_f.ADSTP = 1u;

    return Ok;
} // Adc_SwStop

/**
 ******************************************************************************
 ** \brief Get ADC conversion status
 **
 ** This function gets the status when ADC is in conversion
 **
 ** \param [in]  pstcAdc          ADC instance
 ** \param [in]  enStatus         ADC status type
 ** \arg ScanFifoEmptyStatus      Scan FIFO empty status
 ** \arg ScanFifoFullStatus       Scan FIFO full status
 ** \arg PrioFifoEmptyStatus      Priority FIFO empty status
 ** \arg PrioFifoFullStatus       Priority FIFO full status
 ** \arg Prio2PendingStatus       Priority conversion (level 2) pending status
 ** \arg PrioStatus               Priority conversion status
 ** \arg ScanStatus               Scan conversion status
 ** \arg RangeThresholdExcessFlag Range comparison threshold exceed status
 **
 ** \retval TRUE                  The Scan FIFO is empty [ScanFifoEmptyStatus]   
 **                               The Scan FIFO is full [ScanFifoFullStatus]
 **                               Priority FIFO is empty [PrioFifoEmptyStatus]
 **                               Priority FIFO is full [PrioFifoFullStatus]
 **                               Priority level 2 conversion is pending [Prio2PendingStatus]
 **                               Priority conversion is in progress [PrioStatus]
 **                               Scan conversion is in progress [ScanStatus]
 **                               The sample data beyond the upper limit threshold [RangeThresholdExcessFlag]
 ** \retval FALSE                 The Scan FIFO isn't empty [ScanFifoEmptyStatus] 
 **                               The Scan FIFO isn't full [ScanFifoFullStatus]
 **                               Priority FIFO isn't empty [PrioFifoEmptyStatus]
 **                               Priority FIFO isn't full [PrioFifoFullStatus]
 **                               Priority level 2 conversion isn't pending [Prio2PendingStatus]
 **                               Priority conversion isn't in progress [PrioStatus]
 **                               The sample data below the lower limit threshold [RangeThresholdExcessFlag]
 ******************************************************************************/  
boolean_t   Adc_GetStatus(volatile stc_adcn_t* pstcAdc, en_adc_status_t enStatus)
{
    boolean_t bRet = FALSE;
    switch (enStatus)
    {
        case ScanFifoEmptyStatus:
            bRet = (1u == pstcAdc->SCCR_f.SEMP) ? TRUE : FALSE;
            break;
        case ScanFifoFullStatus:
            bRet = (1u == pstcAdc->SCCR_f.SFUL) ? TRUE : FALSE;
            break;
        case PrioFifoEmptyStatus:
            bRet = (1u == pstcAdc->PCCR_f.PEMP) ? TRUE : FALSE;
            break;
        case PrioFifoFullStatus:
            bRet = (1u == pstcAdc->PCCR_f.PFUL) ? TRUE : FALSE;
            break;
        case Prio2PendingStatus:
            bRet = (1u == pstcAdc->ADSR_f.PCNS) ? TRUE : FALSE;
            break;
        case PrioStatus:  
            bRet = (1u == pstcAdc->ADSR_f.PCS) ? TRUE : FALSE;
            break;
        case ScanStatus: 
            bRet = (1u == pstcAdc->ADSR_f.SCS) ? TRUE : FALSE;
            break;
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)            
        case RangeThresholdExcessFlag:
            bRet = (1u == pstcAdc->WCMRCOT_f.RCOOF) ? TRUE : FALSE;
            break;
#endif            
        default:
            break;
    }
    
    return bRet;
}

/**
 ******************************************************************************
 ** \brief Reads out Scan Conversion FIFO
 **
 ** This function reads out the Scan Conversion FIFO.
 **
 ** \pre Adc_ScanFifioStatus() should be called before
 **
 ** \param [in]  pstcAdc        ADC instance
 **
 ** \return                     Recent Scan Conversion FIFO value including
 **                             INVL, RS1, RS0, and Channel data as is.
 **                             If pstcAdc == NULL 0xFFFFFFFF is returned.
 ******************************************************************************/
uint32_t Adc_ReadScanFifo( volatile stc_adcn_t* pstcAdc )
{
    // Check for NULL pointer
    if (NULL == pstcAdc)
    {
        return 0xFFFFFFFFu;
    }

    return pstcAdc->SCFD;
} // Adc_ReadScanFifo


/**
 ******************************************************************************
 ** \brief Clear scan FIFO
 **
 ** This function clears the FIFO for scan conversion
 **
 ** \param [in]  pstcAdc          ADC instance
 **
 ** \retval Ok                    Scan FIFO cleared
 ** \retval ErrorInvalidParameter pstcAdc == NULL
 ******************************************************************************/
en_result_t    Adc_ClrScanFifo(volatile stc_adcn_t* pstcAdc)
{
    // Check for NULL pointer
    if (NULL == pstcAdc)
    {
        return ErrorInvalidParameter ;
    }
    
    pstcAdc->SCCR_f.SFCLR = 1u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get Channel Number from FIFO data
 **
 ** This function returns the Channel data from input data from Scan Conversion
 ** FIFO
 **
 ** \pre Adc_ReadScanFifo() must be called before
 **
 ** \param [in]  pstcAdc        ADC instance
 ** \param [in]  u32FifoData    FIFO data
 **
 ** \return                     Recent Scan Conversion Channel value.
 ******************************************************************************/
uint8_t Adc_GetScanChannel(volatile stc_adcn_t* pstcAdc, 
                           uint32_t u32FifoData )
{
    return (uint8_t) (0x0000001Fu & u32FifoData);
} // Adc_GetScanChannel

/**
 ******************************************************************************
 ** \brief Get Valid Data Flag from FIFO data
 **
 ** This function checks the validity of scan conversion data from FIFO
 **
 ** \pre Adc_ReadScanFifo() must be called before
 **
 ** \param [in]  pstcAdc          ADC instance
 ** \param [in]  u32FifoData      FIFO data
 **
 ** \retval  AdcFifoDataValid     Recent FIFO data valid
 ** \retval  AdcFifoDataInvalid   Recent FIFO data invalid
 ******************************************************************************/
en_adc_fifo_data_valid_t Adc_GetScanDataValid(volatile stc_adcn_t* pstcAdc, 
                                              uint32_t u32FifoData )
{
    if (0u == (0x00001000u & u32FifoData))
    {
        return AdcFifoDataValid;
    }

    return AdcFifoDataInvalid;
} // Adc_GetScanDataValid

/**
 ******************************************************************************
 ** \brief Get Scan Conversion Start Cause from FIFO data
 **
 ** This function returns the Scan Conversion Start Cause from FIFO Data
 **
 ** \pre Adc_ReadScanFifo() must be called before
 **
 ** \param [in]  pstcAdc          ADC instance
 ** \param [in]  u32FifoData      FIFO data
 **
 ** \retval  AdcFifoSoftwareStart   Recent FIFO data cuased by Software Start
 ** \retval  AdcFifoTimerStart      Recent FIFO data caused by Timer Start
 ** \retval  AdcFifoErrorStart      Recent FIFO data caused by unknown factor
 ******************************************************************************/
en_adc_fifo_start_cause_t Adc_GetScanDataCause(volatile stc_adcn_t* pstcAdc, 
                                               uint32_t u32FifoData )
{
    if (0x00000100u == (0x00000300u & u32FifoData))
    {
        return AdcFifoSoftwareStart;
    }

    if (0x00000200u == (0x00000300u & u32FifoData))
    {
        return AdcFifoTimerStart;
    }

    return AdcFifoErrorStart;
} // Adc_GetPDataCause

/**
 ******************************************************************************
 ** \brief Get Scan Conversion FIFO data
 **
 ** This function returns the Scan Conversion FIFO Data
 **
 ** \pre Adc_ReadScanFifo() must be called before
 **
 ** \param [in]  pstcAdc          ADC instance
 ** \param [in]  u32FifoData      FIFO data
 **
 ** \retval  FIFO data value
 ******************************************************************************/
uint16_t  Adc_GetScanData(volatile stc_adcn_t* pstcAdc, 
                          uint32_t u32FifoData )
{
    if(0u == pstcAdc->ADSR_f.FDAS)
    {
        return ((u32FifoData >> 20u) & 0xFFFu); 
    }
    else
    {
        return ((u32FifoData >> 16u) & 0xFFFu); 
    }
    
}

/**
 ******************************************************************************
 ** \brief Reads out Priority Conversion FIFO
 **
 ** This function reads out the Priority Conversion FIFO.
 **
 ** \pre Adc_PrioFifioStatus() should be called before
 **
 ** \param [in]  pstcAdc        ADC instance
 **
 ** \return                     Recent Priority Conversion FIFO value including
 **                             INVL, RS2, RS1, RS0, and Channel data as is.
 **                             If pstcAdc == NULL 0xFFFFFFFF is returned.
 ******************************************************************************/
uint32_t Adc_ReadPrioFifo( volatile stc_adcn_t* pstcAdc )
{
    // Check for NULL pointer
    if (NULL == pstcAdc)
    {
        return 0xFFFFFFFFu;
    }

    return pstcAdc->PCFD;
} // Adc_ReadPrioFifo

/**
 ******************************************************************************
 ** \brief Clear priority FIFO
 **
 ** This function clears the FIFO for priority conversion
 **
 ** \param [in]  pstcAdc          ADC instance
 **
 ** \retval Ok                    Priority FIFO cleared
 ** \retval ErrorInvalidParameter pstcAdc == NULL
 ******************************************************************************/
en_result_t  Adc_ClrPrioFifo(volatile stc_adcn_t* pstcAdc)
{
    // Check for NULL pointer
    if (NULL == pstcAdc)
    {
        return ErrorInvalidParameter ;
    }
    
    pstcAdc->PCCR_f.PFCLR = 1u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get Channel Number from Priority FIFO data
 **
 ** This function returns the Channel data from input data from Priority
 ** Conversion FIFO
 **
 ** \pre Adc_ReadPrioFifo() must be called before
 **
 ** \param [in]  pstcAdc        ADC instance
 ** \param [in]  u32FifoData    Priority FIFO data
 **
 ** \return                     Recent Priority Conversion Channel value.
 ******************************************************************************/
uint8_t Adc_GetPrioChannel(volatile stc_adcn_t* pstcAdc, 
                           uint32_t u32FifoData )
{
    return (uint8_t) (0x0000001Fu & u32FifoData);
} // Adc_GetPrioChannel

/**
 ******************************************************************************
 ** \brief Get Valid Data Flag from Priority FIFO data
 **
 ** This function checks the validity of priority conversion from FIFO
 **
 ** \pre Adc_ReadPrioFifo() must be called before
 **
 ** \param [in]  pstcAdc          ADC instance
 ** \param [in]  u32FifoData      Priority FIFO data
 **
 ** \retval  AdcFifoDataValid     Recent FIFO data valid
 ** \retval  AdcFifoDataInvalid   Recent FIFO data invalid
 ******************************************************************************/
en_adc_fifo_data_valid_t Adc_GetPrioDataValid(volatile stc_adcn_t* pstcAdc, 
                                              uint32_t u32FifoData )
{
    if (0u == (0x00001000u & u32FifoData))
    {
        return AdcFifoDataValid;
    }

    return AdcFifoDataInvalid;
} // Adc_GetPrioDataValid

/**
 ******************************************************************************
 ** \brief Get Priority Conversion Start Cause from FIFO data
 **
 ** This function returns the Priority Conversion Start Cause from FIFO Data
 **
 ** \pre Adc_ReadPrioFifo() must be called before
 **
 ** \param [in]  pstcAdc             ADC instance
 ** \param [in]  u32FifoData         Priority FIFO data
 **
 ** \retval  AdcFifoSoftwareStart    Recent FIFO data cuased by Software Start
 ** \retval  AdcFifoTimerStart       Recent FIFO data causde by Timer Start
 ** \retval  AdcFifoExternalTrigger  Recent FIFO data causde by External Trigger
 ** \retval  AdcFifoErrorStart       Recent FIFO data causde by unknown factor
 ******************************************************************************/
en_adc_fifo_start_cause_t Adc_GetPrioDataCause(volatile stc_adcn_t* pstcAdc, 
                                               uint32_t u32FifoData )
{
    if (0x00000100u == (0x00000700u & u32FifoData))
    {
        return AdcFifoSoftwareStart;
    }

    if (0x00000200u == (0x00000700u & u32FifoData))
    {
        return AdcFifoTimerStart;
    }

    if (0x00000400u == (0x00000700u & u32FifoData))
    {
        return AdcFifoExternalTrigger;
    }

    return AdcFifoErrorStart;
} // Adc_GetPrioDataCause

/**
 ******************************************************************************
 ** \brief Get Priority Conversion FIFO data
 **
 ** This function returns the Priority Conversion FIFO Data
 **
 ** \pre Adc_ReadPrioFifo() must be called before
 **
 ** \param [in]  pstcAdc          ADC instance
 ** \param [in]  u32FifoData      FIFO data
 **
 ** \retval  FIFO data value
 ******************************************************************************/
uint16_t  Adc_GetPrioData(volatile stc_adcn_t* pstcAdc, 
                          uint32_t u32FifoData )
{
    if(0u == pstcAdc->ADSR_f.FDAS)
    {
        return ((u32FifoData >> 20u) & 0xFFFu); 
    }
    else
    {
        return ((u32FifoData >> 16u) & 0xFFFu); 
    }
    
}

#endif // #if (defined(PDL_PERIPHERAL_ADC_ACTIVE))

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
