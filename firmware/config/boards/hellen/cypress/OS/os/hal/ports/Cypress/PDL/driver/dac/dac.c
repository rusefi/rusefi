/******************************************************************************
* \file             dac.c
*
* \version          1.20
*
* \brief            Digital Analog Converter driver
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
#include "dac/dac.h"

#if (defined(PDL_PERIPHERAL_DAC_ACTIVE))

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
 ** \brief DAC Initialization
 **
 ** \param [in]  pstcDac           Pointer to DAC instance
 ** \param [in]  pstcConfig        Pointer to DAC configuration strucutre
 **
 ** \retval Ok                     DAC Instance sucessfully initialized
 ** \retval ErrorInvalidParameter  pstcDac == NULL or pstcConfig == NULL
 ******************************************************************************/
en_result_t Dac_Init( stc_dacn_t*       pstcDac,
                      const stc_dac_config_t* pstcConfig
                    )
{
    // Check for NULL pointers
    if ((NULL == pstcDac) ||
        (NULL == pstcConfig) ||
        (DacTypeUndefined == pstcConfig->dac_type) ||
        (DacMaxChannel <= pstcConfig->dac_channel)
       )
    {
      return ErrorInvalidParameter;
    }
    
#if (PDL_MCU_CORE == PDL_FM4_CORE)
    pstcDac->DACR_f.DAC10 = (DacType12Bit == pstcConfig->dac_type) ? 0u : 1u ;
    pstcDac->DACR_f.DAC10 = (DacType10BitLeftAligned == pstcConfig->dac_type) ? 0u : 1u ;
#endif
    
    return Ok;
} // Dac_Init


/**
 ******************************************************************************
 ** \brief DAC De-Initialization
 **
 ** \param [in]  pstcDac           Pointer to DAC instance
 ** 
 ** \retval Ok                     DAC Instance sucessfully de-initialized
 ** \retval ErrorInvalidParameter  pstcDac == NULL or pstcConfig == NULL
 ******************************************************************************/
en_result_t Dac_DeInit( stc_dacn_t* pstcDac )
{
   en_result_t enResult;
   
   enResult  = Dac_DeInitChannel( pstcDac, DacChannel0 );
   enResult |= Dac_DeInitChannel( pstcDac, DacChannel1 );
   
   return enResult;
} // Dac_DeInit


/**
 ******************************************************************************
 ** \brief DAC De-Initialization
 **
 ** \param [in]  pstcDac           Pointer to DAC instance
 ** \param [in]  enChannel         Pointer to DAC configuration strucutre
 ** 
 ** \retval Ok                     DAC Channel sucessfully de-initialized
 ** \retval ErrorInvalidParameter  pstcDac == NULL or pstcConfig == NULL
 ******************************************************************************/
en_result_t Dac_DeInitChannel( stc_dacn_t* pstcDac, en_dac_channel_t enChannel )
{
    // Check for NULL pointers
    if ( (NULL == pstcDac) ||
         (DacMaxChannel <= enChannel)
       )
    {
      return ErrorInvalidParameter;
    }      

    if( DacChannel0 ==  enChannel)
    {
        pstcDac->DACR = 0u;
        pstcDac->DADR = 0u;
    }

    return Ok;
} // Dac_DeInitChannel


/**
 ******************************************************************************
 ** \brief Set DAC Channel 0 12-bit value
 **
 ** \param [in]  pstcDac           Pointer to DAC instance
 ** \param [in]  u16DacValue       DAC Channel value to be output
 **
 ** \retval Ok                     Value written.
 ** \retval ErrorInvalidParameter  pstcDac == NULL or pstcConfig == NULL
 ******************************************************************************/
en_result_t Dac_SetValue( stc_dacn_t* pstcDac,
                           en_dac_channel_t enChannel,
                           uint16_t    u16DacValue
                         )
{
    if ( (NULL == pstcDac) ||
         (DacMaxChannel <= enChannel)
       )
    {
      return ErrorInvalidParameter;
    }
  
    pstcDac->DADR_f.DA = u16DacValue;

    return Ok;
} // Dac_SetValue


/**
 ******************************************************************************
 ** \brief Enable DAC Channel
 **
 ** \param [in]  pstcDac           Pointer to DAC instance
 ** \param [in]  u8Channel         DAC Channel number
 **
 ** \retval Ok                     DAC Channel enabled
 ** \retval ErrorInvalidParameter  pstcDac == NULL or pstcConfig == NULL
 ******************************************************************************/
 
en_result_t Dac_Enable( stc_dacn_t* pstcDac, en_dac_channel_t enChannel)
{
    if ( (NULL == pstcDac) ||
         (DacMaxChannel <= enChannel)
       )
    {
      return ErrorInvalidParameter;
    }
    
    pstcDac->DACR_f.DAE = 1u;
  
  return Ok;
} // Dac_Enable


/**
 ******************************************************************************
 ** \brief Disable DAC Channel
 **
 ** \param [in]  pstcDac        Pointer to DAC instance
 ** \param [in]  u8Channel      DAC Channel number
 **
 ** \retval Ok                  DAC Channel disabled
 ** \retval ErrorInvalidParameter  pstcDac == NULL or pstcConfig == NULL
 ******************************************************************************/
en_result_t Dac_Disable( stc_dacn_t* pstcDac, en_dac_channel_t enChannel )
{
    if ( (NULL == pstcDac) ||
         (DacMaxChannel <= enChannel)
       )
    {
      return ErrorInvalidParameter;
    }
  
    pstcDac->DACR_f.DAE = 0u;

  return Ok;
} // Dac_Disable


#endif // #if (defined(PDL_PERIPHERAL_DAC_ACTIVE))

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
