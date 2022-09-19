/*******************************************************************************
* \file             dt.c
* 
* \version          1.20
*
* \brief            This file provides the source code to the API for the DT 
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
#include "dt/dt.h"

#if (defined(PDL_PERIPHERAL_DT_ACTIVE))

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/// Look-up table for all enabled DT instances and their internal data
static stc_dt_instance_data_t m_astcDtInstanceDataLut[DtInstanceIndexMax] =
{
#if (PDL_PERIPHERAL_ENABLE_DT0 == PDL_ON)
    {
        &DT0,     /* pstcInstance */
        {{0u,0u}} /* stcInternData (not initialized yet) */
    }
#endif
};

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/
typedef union stc_dt_timerXcontrol_field 
{
    volatile uint32_t stcTIMER1CTRL;
    volatile stc_dt_timer1control_field_t stcTIMER1CTRL_f;
    volatile uint32_t stcTIMER2CTRL;
    volatile stc_dt_timer2control_field_t stcTIMER2CTRL_f;
}stc_dt_timerXcontrol_field_t;

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/
static stc_dt_intern_data_t* DtGetInternDataPtr(volatile stc_dtn_t** ppstcDt,
                                                uint8_t u8Ch);
#if (PDL_INTERRUPT_ENABLE_DT0 == PDL_ON)
static void DtInitNvic(void);
static void DtDeInitNvic(void);
#endif

/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 ******************************************************************************
 ** \brief Check pointer to Dual Timer instance and enable channel
 **
 ** \param [in,out] ppstcDt   Pointer of pointer to Dual Timer instance register area
 ** \param [in]     u8Channel Channel number
 **
 ** \retval Pointer to internal data or NULL if instance is not enabled.
 **         (or channel is invalid)
 **
 ******************************************************************************/
static stc_dt_intern_data_t* DtGetInternDataPtr(volatile stc_dtn_t** ppstcDt,
                                                uint8_t u8Channel)
{
    stc_dt_intern_data_t* pstcDtInternData = NULL;
    uint32_t              u32Instance;

    /* check for channel */
    if ((NULL != ppstcDt)
    &&  (NULL != *ppstcDt)
    &&  (DtMaxChannels > u8Channel)
       )
    {
        /* Get ptr to internal data struct ... */
        for (u32Instance = 0; u32Instance < (uint32_t)DtInstanceIndexMax; u32Instance++)
        {
            if (*ppstcDt == m_astcDtInstanceDataLut[u32Instance].pstcInstance)
            {
                /* Set actual address of register list of current channel */
                *ppstcDt = &((*ppstcDt)[u8Channel]);
                pstcDtInternData = &m_astcDtInstanceDataLut[u32Instance].stcInternData;
                break;
            }
        }
    }

    return (pstcDtInternData);
} /* DtGetInternDataPtr */

#if (PDL_INTERRUPT_ENABLE_DT0 == PDL_ON)
/**
 ******************************************************************************
 ** \brief Device dependent initialization of interrupts according CMSIS with
 **        level defined in pdl.h
 **
 ******************************************************************************/
static void DtInitNvic(void)
{    
#if (PDL_MCU_CORE == PDL_FM4_CORE)
    NVIC_ClearPendingIRQ(DT_IRQn);
    NVIC_EnableIRQ(DT_IRQn);
    NVIC_SetPriority(DT_IRQn, PDL_IRQ_LEVEL_DT0);
#elif (PDL_MCU_CORE == PDL_FM3_CORE)
    NVIC_ClearPendingIRQ(DT_QPRC0_2_IRQn);
    NVIC_EnableIRQ(DT_QPRC0_2_IRQn);
    NVIC_SetPriority(DT_QPRC0_2_IRQn, PDL_IRQ_LEVEL_DT_QPRC);
#else
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE3)
    NVIC_ClearPendingIRQ(WC_RTC_DT_IRQn);
    NVIC_EnableIRQ(WC_RTC_DT_IRQn);
    NVIC_SetPriority(WC_RTC_DT_IRQn, PDL_IRQ_LEVEL_DT_RTC_WC);
#elif (PDL_MCU_TYPE == PDL_FM0P_TYPE2)
    NVIC_ClearPendingIRQ(DT_IRQn);
    NVIC_EnableIRQ(DT_IRQn);
    NVIC_SetPriority(DT_IRQn, PDL_IRQ_LEVEL_DT);
#else
    NVIC_ClearPendingIRQ(DT_QPRC_IRQn);
    NVIC_EnableIRQ(DT_QPRC_IRQn);
    NVIC_SetPriority(DT_QPRC_IRQn, PDL_IRQ_LEVEL_DT_QPRC);
#endif
#endif
} /* Dt_InitIrq */

/**
 ******************************************************************************
 ** \brief Device dependent de-initialization of interrupts according CMSIS 
 **
 ******************************************************************************/
static void DtDeInitNvic(void)
{
#if (PDL_MCU_CORE == PDL_FM4_CORE)
    NVIC_ClearPendingIRQ(DT_IRQn);
    NVIC_DisableIRQ(DT_IRQn);
    NVIC_SetPriority(DT_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
#elif (PDL_MCU_CORE == PDL_FM3_CORE)    
    NVIC_ClearPendingIRQ(DT_QPRC0_2_IRQn);
    NVIC_DisableIRQ(DT_QPRC0_2_IRQn);
    NVIC_SetPriority(DT_QPRC0_2_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
#else
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE3)
    NVIC_ClearPendingIRQ(WC_RTC_DT_IRQn);
    NVIC_DisableIRQ(WC_RTC_DT_IRQn);
    NVIC_SetPriority(WC_RTC_DT_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
#elif (PDL_MCU_TYPE == PDL_FM0P_TYPE2)
    NVIC_ClearPendingIRQ(DT_IRQn);
    NVIC_DisableIRQ(DT_IRQn);
    NVIC_SetPriority(DT_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
#else
    NVIC_ClearPendingIRQ(DT_QPRC_IRQn);
    NVIC_DisableIRQ(DT_QPRC_IRQn);
    NVIC_SetPriority(DT_QPRC_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
#endif  
#endif  
} /* Dt_DeInitIrq */

/**
 *****************************************************************************
 ** \brief ISR callback for DT (channel 0 and 1)
 **
 ** This callbacks are called by the global DT ISR whenever an DT triggers an
 ** interrupt (on channel 0 and/or 1). It calls the callback functions that
 ** has been given during Dt initialization (see Dt_Init() and
 ** #stc_dt_channel_config_t) for each channel individually. If the pointer
 ** to a callback function is NULL, no call is performed.
 **
 ** The active interrupt request flags are cleared by the ISR
 **
 ** \param [in]  u8Channel          Channel number
 **
 *****************************************************************************/
void DtIrqHandler1()
{
    volatile stc_dtn_t*   pstcDt;
    stc_dt_intern_data_t* pstcDtInternData;

    pstcDt = &DT0;
    pstcDtInternData = &m_astcDtInstanceDataLut[0].stcInternData;
    
    if (TRUE == pstcDt->TIMER1RIS_f.TIMER1RIS)  /* Timer 0 interrupt? */
    {
        pstcDt->TIMER1INTCLR = 0u;               /* Clear interrupt */

        /* Check for callback function pointer */
        if (NULL != pstcDtInternData->pfnIrqCallbackIntern[0])
        {
            pstcDtInternData->pfnIrqCallbackIntern[0]() ;
        }
    }
} /* DtIrqHandler */


/**
 *****************************************************************************
 ** \brief ISR callback for DT (channel 0 and 1)
 **
 ** This callbacks are called by the global DT ISR whenever an DT triggers an
 ** interrupt (on channel 0 and/or 1). It calls the callback functions that
 ** has been given during Dt initialization (see Dt_Init() and
 ** #stc_dt_channel_config_t) for each channel individually. If the pointer
 ** to a callback function is NULL, no call is performed.
 **
 ** The active interrupt request flags are cleared by the ISR
 **
 ** \param [in]  u8Channel          Channel number
 **
 *****************************************************************************/
void DtIrqHandler2()
{
    volatile stc_dtn_t*   pstcDt;
    stc_dt_intern_data_t* pstcDtInternData;

    pstcDt = &DT0;
    pstcDtInternData = &m_astcDtInstanceDataLut[0].stcInternData;
    
    if (TRUE == pstcDt->TIMER2RIS_f.TIMER2RIS)  /* Timer 0 interrupt? */
    {
        pstcDt->TIMER1INTCLR = 0u;               /* Clear interrupt */

        /* Check for callback function pointer */
        if (NULL != pstcDtInternData->pfnIrqCallbackIntern[0])
        {
            pstcDtInternData->pfnIrqCallbackIntern[0]() ;
        }
    }
} /* DtIrqHandler */
#endif


/**
 *****************************************************************************
 ** \brief Initialize DT
 **
 ** This function initializes the specified channel of Dual Timer.
 **
 ** \param [in]  pstcConfig       Dual timer configuration
 ** \param [in]  u8Channel        Channel number
 **
 ** \retval Ok                    Process successfully done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **             - pstcConfig == NULL
 **             - u8Channel >= DtMaxChannels
 **             - pstcDtInternData == NULL (invalid or disabled DT unit
 **                                         (PDL_PERIPHERAL_ENABLE_DT))
 **             - one or more enumerated values in pstcDt out of enumaration
 **
 *****************************************************************************/
en_result_t Dt_Init(const stc_dt_channel_config_t* pstcConfig,
                    uint8_t u8Channel
                   )
{
    en_result_t                    enResult;
    /* Pointer to Dual Timer instance register area */
    volatile stc_dtn_t*            pstcDt;
    /* Pointer to internal data */
    stc_dt_intern_data_t*          pstcDtInternData;
    

    
    stc_dt_timerXcontrol_field_t stcTIMERXCTRL;
    PDL_ZERO_STRUCT(stcTIMERXCTRL);
    
    pstcDt = &DT0;
    /*-------- Initialize internal data -------*/
    /* Get pointer to internal data structure and check channel... */
    pstcDtInternData = DtGetInternDataPtr(&pstcDt, u8Channel);
    /* ... and check for NULL */
    if ((NULL == pstcDtInternData)
    ||  (NULL == pstcConfig)
       )
    {
        enResult = ErrorInvalidParameter;
    }
    else
    {
        enResult = Ok;
        /*-------- Configure the DT timer -------*/
        if(DtChannel0 == u8Channel)
        {
          pstcDt->TIMER1CONTROL = 0u;              /* Disable DT TIMER1 */
        }
        else
        {
          pstcDt->TIMER2CONTROL = 0u;              /* Disable DT TIMER2 */
        }
          
        /* Set Timer Mode */
        switch (pstcConfig->u8Mode)
        {
            /* Free run mode */
            case DtFreeRun:
                stcTIMERXCTRL.stcTIMER1CTRL_f.TIMERMODE = FALSE;
                stcTIMERXCTRL.stcTIMER1CTRL_f.ONESHOT   = FALSE;
                break;
            /* Periodic mode */
            case DtPeriodic:
                stcTIMERXCTRL.stcTIMER1CTRL_f.TIMERMODE = TRUE;
                stcTIMERXCTRL.stcTIMER1CTRL_f.ONESHOT   = FALSE;
                break;
            /* One shot mode */
            case DtOneShot:
                stcTIMERXCTRL.stcTIMER1CTRL_f.TIMERMODE = FALSE;
                stcTIMERXCTRL.stcTIMER1CTRL_f.ONESHOT   = TRUE;
                break;
            default:
                enResult = ErrorInvalidParameter;
                break;
        }

        /* Set Prescaler */
        switch (pstcConfig->u8PrescalerDiv)
        {
            /* Clock/1 */
            case DtPrescalerDiv1:
                stcTIMERXCTRL.stcTIMER1CTRL_f.TIMERPRE = DT_PRE_TIMER_DIV_1;
                break;
            /* Clock/16 */
            case DtPrescalerDiv16:
                stcTIMERXCTRL.stcTIMER1CTRL_f.TIMERPRE = DT_PRE_TIMER_DIV_16;
                break;
            /* Clock/256 */
            case DtPrescalerDiv256:
                stcTIMERXCTRL.stcTIMER1CTRL_f.TIMERPRE = DT_PRE_TIMER_DIV_256;
                break;
            default:
                enResult = ErrorInvalidParameter;
                break;
        }

        /* Set Counter Size */
        switch (pstcConfig->u8CounterSize)
        {
            /* 16bit */
            case DtCounterSize16:
                stcTIMERXCTRL.stcTIMER1CTRL_f.TIMERSIZE = FALSE;
                break;
            /* 32bit */
            case DtCounterSize32:
                stcTIMERXCTRL.stcTIMER1CTRL_f.TIMERSIZE = TRUE;
                break;
            default:
                enResult = ErrorInvalidParameter;
                break;
        }
#if (PDL_INTERRUPT_ENABLE_DT0 == PDL_ON)        
        /* Set interrupt */
        if(pstcConfig->bIrqEnable == TRUE)
        {
            stcTIMERXCTRL.stcTIMER1CTRL_f.INTENABLE = 1u;
        }
        
        /* Set interrupt callback function */
        pstcDtInternData->pfnIrqCallbackIntern[u8Channel] = pstcConfig->pfnIrqCallback;
        
        /* Set NVIC */
        if(pstcConfig->bTouchNvic == TRUE)
        {
            DtInitNvic();
        }
#endif        
    }
    
    if (Ok == enResult)
    {
        /* Set control register */
        if(DtChannel0 == u8Channel)
        {
            pstcDt->TIMER1CONTROL = stcTIMERXCTRL.stcTIMER1CTRL;
        }
        else
        {
            pstcDt->TIMER2CONTROL = stcTIMERXCTRL.stcTIMER2CTRL;
        }
    }
    
    return (enResult);
} /* Dt_Init */

/**
 *****************************************************************************
 ** \brief De-Initialize DT
 **
 ** This function de-initializes the specified channel of Dual Timer.
 ** Dt-DeInit() accesses the DT hardware register. They are reset.
 **
 ** \param [in]  u8Channel        Channel number
 ** \param [in]  bTouchNvic       Touch NVIC or not
 **
 ** \retval Ok                    Process successfully done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **             - pstcDt == NULL
 **             - u8Ch >= DtMaxChannels
 **             - pstcDtInternData == NULL (invalid or disabled DT unit
 **                                         (PDL_PERIPHERAL_ENABLE_DT)) 
 **
 *****************************************************************************/
en_result_t Dt_DeInit(uint8_t u8Channel, boolean_t bTouchNvic)
{
    en_result_t           enResult;
    /* Pointer to Dual Timer instance register area */
    volatile stc_dtn_t*   pstcDt;
    /* Pointer to internal data */
    stc_dt_intern_data_t* pstcDtInternData;

    pstcDt = &DT0;
    /*-------- Initialize internal data -------*/
    /* Get pointer to internal data structure and check channel... */
    pstcDtInternData = DtGetInternDataPtr(&pstcDt, u8Channel);
    /* ... and check for NULL */
    if (NULL == pstcDtInternData)
    {
        enResult = ErrorInvalidParameter;
    }
    else
    {
      if(DtChannel0 == u8Channel)
      {
        /* Clear all registers */
        pstcDt->TIMER1CONTROL = 0u;

        pstcDt->TIMER1LOAD    = 0u;
        pstcDt->TIMER1INTCLR  = 1u;
        pstcDt->TIMER1BGLOAD  = 0u;

#if (PDL_INTERRUPT_ENABLE_DT0 == PDL_ON)          
        /* Clear callback */
        pstcDtInternData->pfnIrqCallbackIntern[u8Channel] = NULL;
        
        if(bTouchNvic == TRUE)
        {
            DtDeInitNvic();
        }
#endif        
        enResult = Ok;
      }
      else if(DtChannel1 == u8Channel)
      {
        /* Clear all registers */
        pstcDt->TIMER2CONTROL = 0u;

        pstcDt->TIMER2LOAD    = 0u;
        pstcDt->TIMER2INTCLR  = 1u;
        pstcDt->TIMER2BGLOAD  = 0u;

#if (PDL_INTERRUPT_ENABLE_DT0 == PDL_ON)          
        /* Clear callback */
        pstcDtInternData->pfnIrqCallbackIntern[u8Channel] = NULL;
        
        if(bTouchNvic == TRUE)
        {
            DtDeInitNvic();
        }
#endif  
        enResult = Ok;
      }
      else
      {
        enResult = ErrorInvalidParameter;
      }
    }

    return (enResult);
} /* Dt_DeInit */

/**
 *****************************************************************************
 ** \brief Enable Timer Counter
 **
 ** This function enables the timer counter.
 **
 ** \param [in]  u8Channel        Channel number
 **
 ** \retval Ok                    Process successfully done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **             - u8Channel >= DtMaxChannels
 **             - pstcDtInternData == NULL (invalid or disabled DT unit
 **                                         (PDL_PERIPHERAL_ENABLE_DT)) 
 **
 *****************************************************************************/
en_result_t Dt_EnableCount(uint8_t u8Channel)
{
    en_result_t           enResult;
    /* Pointer to Dual Timer instance register area */
    volatile stc_dtn_t*   pstcDt;
    /* Pointer to internal data */
    stc_dt_intern_data_t* pstcDtInternData;

    enResult = ErrorInvalidParameter;

    pstcDt = &DT0;
    /* Get pointer to internal data structure and check channel... */
    pstcDtInternData = DtGetInternDataPtr(&pstcDt, u8Channel);
    /* ... and check for NULL */
    if (NULL != pstcDtInternData)
    {
      /* Enable timer counter */
      if ( DtChannel0 == u8Channel )
      {
        pstcDt->TIMER1CONTROL_f.TIMEREN = TRUE;
        enResult = Ok;
      }
      else if ( DtChannel1 == u8Channel )
      {
        pstcDt->TIMER2CONTROL_f.TIMEREN = TRUE;
        enResult = Ok;
      }
        
    }

    return (enResult);
} /* Dt_EnableCount */

/**
 *****************************************************************************
 ** \brief Disable Timer Counter
 **
 ** This function disables the timer counter.
 **
 ** \param [in]  u8Channel        Channel number
 **
 ** \retval Ok                    Process successfully done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **             - u8Channel >= DtMaxChannels
 **             - pstcDtInternData == NULL (invalid or disabled DT unit
 **                                         (PDL_PERIPHERAL_ENABLE_DT)) 
 **
 *****************************************************************************/
en_result_t Dt_DisableCount(uint8_t u8Channel)
{
    en_result_t           enResult;
    /* Pointer to Dual Timer instance register area */
    volatile stc_dtn_t*   pstcDt;
    /* Pointer to internal data */
    stc_dt_intern_data_t* pstcDtInternData;

    enResult = ErrorInvalidParameter;

    pstcDt = &DT0;
    /* Get pointer to internal data structure and check channel... */
    pstcDtInternData = DtGetInternDataPtr(&pstcDt, u8Channel);
    /* ... and check for NULL */
    if (NULL != pstcDtInternData)
    {
        /* Disable timer counter */
        if ( DtChannel0 == u8Channel )
        {
          pstcDt->TIMER1CONTROL_f.TIMEREN = FALSE;
          enResult = Ok;
        }
        else if ( DtChannel1 == u8Channel )
        {
          pstcDt->TIMER2CONTROL_f.TIMEREN = FALSE;
          enResult = Ok;
        }
    }

    return (enResult);
} /* Dt_DisableCount */

#if (PDL_INTERRUPT_ENABLE_DT0 == PDL_ON)
/**
 *****************************************************************************
 ** \brief Enable Interrupt
 **
 ** This function enables the interruption.
 **
 ** \param [in]  u8Channel        Channel number
 **
 ** \retval Ok                    Process successfully done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **             - u8Channel >= DtMaxChannels
 **             - pstcDtInternData == NULL (invalid or disabled DT unit
 **                                         (PDL_PERIPHERAL_ENABLE_DT)) 
 **
 *****************************************************************************/
en_result_t Dt_EnableIrq(uint8_t u8Channel)
{
    en_result_t           enResult;
    /* Pointer to Dual Timer instance register area */
    volatile stc_dtn_t*   pstcDt;
    /* Pointer to internal data */
    stc_dt_intern_data_t* pstcDtInternData;

    enResult = ErrorInvalidParameter;

    pstcDt = &DT0;
    /* Get pointer to internal data structure and check channel... */
    pstcDtInternData = DtGetInternDataPtr(&pstcDt, u8Channel);
    /* ... and check for NULL */
    if (NULL != pstcDtInternData)
    {
        /* Enable interrupt */
        if ( DtChannel0 == u8Channel )
        {
          pstcDt->TIMER1CONTROL_f.INTENABLE = TRUE;
          enResult = Ok;
        }
        else if ( DtChannel1 == u8Channel )
        {
          pstcDt->TIMER2CONTROL_f.INTENABLE = TRUE;
          enResult = Ok;
        }
    }
    return (enResult);
} /* Dt_EnableIrq */

/**
 *****************************************************************************
 ** \brief Disable Interrupt
 **
 ** This function disables the interruption.
 **
 ** \param [in]  u8Channel        Channel number
 **
 ** \retval Ok                    Process successfully done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **             - u8Channel >= DtMaxChannels
 **             - pstcDtInternData == NULL (invalid or disabled DT unit
 **                                         (PDL_PERIPHERAL_ENABLE_DT)) 
 **
 *****************************************************************************/
en_result_t Dt_DisableIrq(uint8_t u8Channel)
{
    en_result_t           enResult;
    /* Pointer to Dual Timer instance register area */
    volatile stc_dtn_t*   pstcDt;
    /* Pointer to internal data */
    stc_dt_intern_data_t* pstcDtInternData;

    enResult = ErrorInvalidParameter;

    pstcDt = &DT0;
    /* Get pointer to internal data structure and check channel... */
    pstcDtInternData = DtGetInternDataPtr(&pstcDt, u8Channel);
    /* ... and check for NULL */
    if (NULL != pstcDtInternData)
    {
        /* Disable interrupt */
        if ( DtChannel0 == u8Channel )
        {
          pstcDt->TIMER1CONTROL_f.INTENABLE = FALSE;
          enResult = Ok;
        }
        else if ( DtChannel1 == u8Channel )
        {
          pstcDt->TIMER2CONTROL_f.INTENABLE = FALSE;
          enResult = Ok;
        }

    }

    return (enResult);
} /* Dt_DisableIrq */
#endif

/**
 ******************************************************************************
 ** \brief Get interrupt status
 ** The Function can return the interrupt status (TimerXRIS)
 **
 ** \param [in]     u8Channel       Channel number
 **
 ** \retval boolean_t:the interrupt status
 **
 ******************************************************************************/
boolean_t Dt_GetIrqFlag(uint8_t u8Channel)
{
    /* Pointer to Dual Timer instance register area */
    volatile stc_dtn_t*   pstcDt;
    boolean_t             bRetVal = FALSE;
    /* Pointer to internal data */
    stc_dt_intern_data_t* pstcDtInternData;

    pstcDt = &DT0;
    /* Get pointer to internal data structure and check channel... */
    pstcDtInternData = DtGetInternDataPtr(&pstcDt, u8Channel);
    /* ... and check for NULL */
    if (NULL != pstcDtInternData)
    {
        /* Check the interrupt status */
        if ( DtChannel0 == u8Channel )
        {
          if (TRUE == pstcDt->TIMER1RIS_f.TIMER1RIS)
          {
              bRetVal = TRUE;
          }
        }
        else if ( DtChannel1 == u8Channel )
        {
          if (TRUE == pstcDt->TIMER2RIS_f.TIMER2RIS)
          {
              bRetVal = TRUE;
          }
        }
    }

    return (bRetVal);
} /* Dt_GetIrqFlag */

/**
 ******************************************************************************
 ** \brief Get mask interrupt status
 ** The Function can return the mask interrupt status (TimerXMIS)
 **
 ** \param [in]     u8Channel       Channel number
 **
 ** \retval boolean_t:the mask interrupt status
 **
 ******************************************************************************/
boolean_t Dt_GetMaskIrqFlag(uint8_t u8Channel)
{
    /* Pointer to Dual Timer instance register area */
    volatile stc_dtn_t*   pstcDt;
    boolean_t             bRetVal = FALSE;
    /* Pointer to internal data */
    stc_dt_intern_data_t* pstcDtInternData;

    pstcDt = &DT0;
    /* Get pointer to internal data structure and check channel... */
    pstcDtInternData = DtGetInternDataPtr(&pstcDt, u8Channel);
    /* ... and check for NULL */
    if (NULL != pstcDtInternData)
    {
        /* Check the mask interrupt status */
        if ( DtChannel0 == u8Channel )
        {
          if (TRUE == pstcDt->TIMER1MIS_f.TIMER1MIS)
          {
              bRetVal = TRUE;
          }
        }
        else if ( DtChannel1 == u8Channel )
        {
          if (TRUE == pstcDt->TIMER2MIS_f.TIMER2MIS)
          {
              bRetVal = TRUE;
          }
        }
    }

    return (bRetVal);
} /* Dt_GetMaskIrqFlag */

/**
 ******************************************************************************
 ** \brief Clear interrupt status
 ** The Function clears the interrupt status
 **
 ** \param [in]  u8Channel            Channel number
 **
 ** \retval Ok                        Process successfully done.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - u8Channel >= DtMaxChannels
 **             - pstcDtInternData == NULL (invalid or disabled DT unit
 **                                         (PDL_PERIPHERAL_ENABLE_DT)) 
 **
 ******************************************************************************/
en_result_t Dt_ClrIrqFlag(uint8_t u8Channel)
{
    en_result_t           enResult;
    /* Pointer to Dual Timer instance register area */
    volatile stc_dtn_t*   pstcDt;
    /* Pointer to internal data */
    stc_dt_intern_data_t* pstcDtInternData;

    enResult = ErrorInvalidParameter;

    pstcDt = &DT0;
    /* Get pointer to internal data structure and check channel... */
    pstcDtInternData = DtGetInternDataPtr(&pstcDt, u8Channel);
    /* ... and check for NULL */
    if (NULL != pstcDtInternData)
    {
        /* Clear the interrupt status */
        if ( DtChannel0 == u8Channel )
        {
          pstcDt->TIMER1INTCLR = 1;
          enResult = Ok;
        }
        else if ( DtChannel1 == u8Channel )
        {
          pstcDt->TIMER2INTCLR = 1;
          enResult = Ok;
        }
    }

    return (enResult);
} /* Dt_ClrIrqFlag */

/**
 ******************************************************************************
 ** \brief Write load value
 ** The Function writes the load value to load register
 **
 ** \param [in]  u32LoadVal     Load value to set to load register
 ** \param [in]  u8Channel      Channel number
 **
 ** \retval Ok                        Process successfully done.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - u8Channel >= DtMaxChannels
 **             - pstcDtInternData == NULL (invalid or disabled DT unit
 **                                         (PDL_PERIPHERAL_ENABLE_DT)) 
 **
 ******************************************************************************/
en_result_t Dt_WriteLoadVal(uint32_t u32LoadVal,
                            uint8_t u8Channel
                           )
{
    en_result_t           enResult;
    /* Pointer to Dual Timer instance register area */
    volatile stc_dtn_t*   pstcDt;
    /* Pointer to internal data */
    stc_dt_intern_data_t* pstcDtInternData;

    enResult = ErrorInvalidParameter;

    pstcDt = &DT0;
    /* Get pointer to internal data structure and check channel... */
    pstcDtInternData = DtGetInternDataPtr(&pstcDt, u8Channel);
    /* ... and check for NULL */
    if (NULL != pstcDtInternData)
    {
        /* 16bit mode */
        if ( DtChannel0 == u8Channel )
        {
          if (FALSE == pstcDt->TIMER1CONTROL_f.TIMERSIZE)
          {
              u32LoadVal &= 0x0000FFFFu;
          }
          /* Write load value to register */
          pstcDt->TIMER1LOAD = u32LoadVal;
          enResult = Ok;
        }
        else if ( DtChannel1 == u8Channel )
        {
          if (FALSE == pstcDt->TIMER2CONTROL_f.TIMERSIZE)
          {
              u32LoadVal &= 0x0000FFFFu;
          }
          /* Write load value to register */
          pstcDt->TIMER2LOAD = u32LoadVal;
          enResult = Ok;
        }
    }

    return (enResult);
} /* Dt_WriteLoadVal */

/**
 ******************************************************************************
 ** \brief Write back-ground load value
 ** The Function writes the load value to back-ground load register
 **
 ** \param [in]  u32BgLoadVal   Load value to set to back-ground load register
 ** \param [in]  u8Channel      Channel number
 **
 ** \retval Ok                        Process successfully done.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - u8Channel >= DtMaxChannels
 **             - pstcDtInternData == NULL (invalid or disabled DT unit
 **                                         (PDL_PERIPHERAL_ENABLE_DT)) 
 **
 ******************************************************************************/
en_result_t Dt_WriteBgLoadVal(uint32_t u32BgLoadVal,
                              uint8_t u8Channel
                             )
{
    en_result_t           enResult;
    /* Pointer to Dual Timer instance register area */
    volatile stc_dtn_t*   pstcDt;
    /* Pointer to internal data */
    stc_dt_intern_data_t* pstcDtInternData;

    enResult = ErrorInvalidParameter;

    pstcDt = &DT0;
    /* Get pointer to internal data structure and check channel... */
    pstcDtInternData = DtGetInternDataPtr(&pstcDt, u8Channel);
    /* ... and check for NULL */
    if (NULL != pstcDtInternData)
    {
        /* 16bit mode */
        if ( DtChannel0 == u8Channel )
        {
          if (FALSE == pstcDt->TIMER1CONTROL_f.TIMERSIZE)
          {
              u32BgLoadVal &= 0x0000FFFFu;
          }
          /* Write back-ground load value to register */
          pstcDt->TIMER1BGLOAD = u32BgLoadVal;
          enResult = Ok;
        }
        else if ( DtChannel1 == u8Channel )
        {
          if (FALSE == pstcDt->TIMER2CONTROL_f.TIMERSIZE)
          {
              u32BgLoadVal &= 0x0000FFFFu;
          }
          /* Write back-ground load value to register */
          pstcDt->TIMER2BGLOAD = u32BgLoadVal;
          enResult = Ok;
        }
    }

    return (enResult);
} /* Dt_WriteBgLoadVal */

/**
 ******************************************************************************
 ** \brief Read current count value
 ** The Function reads the value from value register
 **
 ** \param [in]  u8Channel        Channel number
 **
 ** \retval uint32_t:current counter value
 **
 ******************************************************************************/
uint32_t Dt_ReadCurCntVal(uint8_t u8Channel)
{
    /* Pointer to Dual Timer instance register area */
    volatile stc_dtn_t*   pstcDt;
    uint32_t              u32DtValue = 0;
    /* Pointer to internal data */
    stc_dt_intern_data_t* pstcDtInternData;

    pstcDt = &DT0;
    /* Get pointer to internal data structure and check channel... */
    pstcDtInternData = DtGetInternDataPtr(&pstcDt, u8Channel);
    /* ... and check for NULL */
    if (NULL != pstcDtInternData)
    {
        /* Read current count value */
        if ( DtChannel0 == u8Channel )
        {
          u32DtValue = pstcDt->TIMER1VALUE;
          /* 16bit mode */
          if (FALSE == pstcDt->TIMER1CONTROL_f.TIMERSIZE)
          {
              u32DtValue &= 0x0000FFFFu;
          }
        }
        else if ( DtChannel1 == u8Channel )
        {
          u32DtValue = pstcDt->TIMER2VALUE;
          /* 16bit mode */
          if (FALSE == pstcDt->TIMER2CONTROL_f.TIMERSIZE)
          {
              u32DtValue &= 0x0000FFFFu;
          }
        }
    }

    return (u32DtValue);
} /* Dt_ReadCurCntVal */

#endif /* #if (defined(PDL_PERIPHERAL_DT_ACTIVE)) */

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

