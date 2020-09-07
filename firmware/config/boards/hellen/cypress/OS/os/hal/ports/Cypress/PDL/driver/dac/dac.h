/******************************************************************************
* \file             dac.h
*
* \version          1.20
*
* \brief            Headerfile for Digital Analog Converter functions
*
********************************************************************************
* \copyright
* Copyright 2016,  Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
* CYPRESS PROVIDES THIS SOFTWARE "AS IS" AND MAKES NO WARRANTY
* OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS SOFTWARE,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
* PURPOSE.
*******************************************************************************/

#ifndef __DAC_H__
#define __DAC_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "pdl_user.h"
#include "mcu.h"

#if !defined(FM_DAC_AVAILABLE) && defined(PDL_PERIPHERAL_DAC_ACTIVE)
    #error "Do not enable DAC, as it is not available on this device"
#endif

#if (defined(PDL_PERIPHERAL_DAC_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
* \defgroup GroupDAC Digital Analog Converter (DAC)
* \{
* \defgroup GroupDAC_Macros Macros
* \defgroup GroupDAC_Functions Functions
* \defgroup GroupDAC_DataStructures Data Structures
* \defgroup GroupDAC_Types Enumerated Types
* \}
*/
/**
* \addtogroup GroupDAC
* \{
* The 12-bit Digital Analog Converter (DAC) converts 10- or 12-bit digital values to analog output values.<br>
* <b>Features of 12-bit D/A Converter:</b><br>
* - 12-bits or 10-bits mode<br>
* - R-2R method<br>
* - stops operating in following low power consumption modes:<br>
* -- RTC mode<br>
* -- Stop mode<br>
* -- Deep standby RTC mode<br>
* -- Deep standby stop mode<br>
* \section SectionDAC_ConfigurationConsideration Configuration Consideration
* To set up the DAC, provide configuration parameters in the stc_dac_config_t structure. For example, 
* specify the operating mode: 12-bit, 10-bit left aligned, or 10-bit right-aligned. Then call Dac_Init().<br>
* You must also call Dac_Enable() to start the peripheral before beginning conversions. There are two possible 
* DAC channels. You can enable or disable either of them independently. Note that in low power modes, 
* the DAC is stopped, regardless of whether you have enabled it.<br>
* Use Dac_SetValue() to set the digital value to be converted into analog signal.<br>
* 
* \section SectionDAC_MoreInfo More Information
* For more information on the DAC peripheral, refer to:<br> 
* <a href="http://www.cypress.com/file/223036/download">FM0+ Peripheral Manual - Analog Subsystem TRM.pdf</a><br>
* <a href="http://www.cypress.com/file/222981/download">FM4 Peripheral Manual - Analog Subsystem TRM.pdf</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/**
* \addtogroup GroupDAC_Macros
* \{
*/
/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/
  #define stc_dacn_t   FM_DAC_TypeDef
#ifdef FM_DAC0_BASE
  #define DAC0  (*(( stc_dacn_t *) FM_DAC0_BASE))
#endif
#ifdef FM_DAC1_BASE
  #define DAC1  (*(( stc_dacn_t *) FM_DAC1_BASE))
#endif


  #define Dac_SetValue0( pstcDac, u16DacValue )         Dac_SetValue(pstcDac, DacChannel0, u16DacValue)
  #define Dac_SetValue1( pstcDac, u16DacValue )         Dac_SetValue(pstcDac, DacChannel1, u16DacValue)
  #define Dac_Enable0( pstcDac )                        Dac_Enable(pstcDac, DacChannel0)
  #define Dac_Enable1( pstcDac )                        Dac_Enable(pstcDac, DacChannel1)
  #define Dac_Disable0( pstcDac )                       Dac_Disable(pstcDac, DacChannel0)
  #define Dac_Disable1( pstcDac )                       Dac_Disable(pstcDac, DacChannel1)

/** \} GroupDAC_Macros */

/**
* \addtogroup GroupDAC_Types
* \{
*/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/
typedef enum en_dac_type
{
  DacTypeUndefined = 0u,
  DacType12Bit = 1u,
  DacType10BitLeftAligned = 2u,
  DacType10BitRightAligned = 3u
}en_dac_type_t;
  
typedef enum en_dac_channel
{
  DacChannel0 = 0u,
  DacChannel1 = 1u,
  DacMaxChannel = 2u
}en_dac_channel_t;

/** \}GroupDAC_Types */

/**
* \addtogroup GroupDAC_DataStructures
* \{
*/

/**
 ******************************************************************************
 ** \brief DAC configuration.
 ******************************************************************************/
typedef struct stc_dac_config
{
    en_dac_channel_t  dac_channel;
    en_dac_type_t     dac_type;
} stc_dac_config_t;
/** \} GroupDAC_DataStructures */

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/

/**
* \addtogroup GroupDAC_Functions
* \{
*/ 

/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/
en_result_t Dac_Init( stc_dacn_t*       pstcDac,
                      const stc_dac_config_t* pstcConfig
                    );

en_result_t Dac_DeInit( stc_dacn_t* pstcDac );

en_result_t Dac_DeInitChannel( stc_dacn_t* pstcDac,
                               en_dac_channel_t enChannel
                             );

en_result_t Dac_SetValue( stc_dacn_t* pstcDac,
                           en_dac_channel_t enChannel,
                           uint16_t    u16DacValue
                         );

en_result_t Dac_Enable( stc_dacn_t* pstcDac, 
                        en_dac_channel_t enChannel
                      );

en_result_t Dac_Disable( stc_dacn_t* pstcDac, 
                         en_dac_channel_t enChannel 
                       );


/** \} GroupDAC_Functions */
/** \} GroupDAC */
                       
#ifdef __cplusplus
}
#endif

#endif // #if (defined(PDL_PERIPHERAL_DAC_ACTIVE))

#endif /* __DAC_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

