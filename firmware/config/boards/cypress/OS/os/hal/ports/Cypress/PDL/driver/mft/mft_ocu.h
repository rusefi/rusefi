/*******************************************************************************
* \file              mft_ocu.h
* 
* \version           1.20
*
* \brief             Headerfile for MFT_OCU functions.
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

#ifndef __MFT_OCU_H__
#define __MFT_OCU_H__
/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if (defined(PDL_PERIPHERAL_MFT_OCU_ACTIVE))
  
/**
* \defgroup GroupMFT_OCU Output Compare Unit (MFT OCU)
* \{
* \defgroup GroupMFT_OCU_Macros Macros
* \defgroup GroupMFT_OCU_Functions Functions
* \defgroup GroupMFT_OCU_GlobalVariables Global Variables
* \defgroup GroupMFT_OCU_DataStructures Data Structures
* \defgroup GroupMFT_OCU_Types Enumerated Types
* \}
*/

/**
* \addtogroup GroupMFT_OCU
* \{
* The Multi-function Timer (MFT) is a collection of peripherals used primarily
* for motor control. There may be up to three MFTs supported on a particular FM microcontroller.
* One MFT can control a 3-phase motor. A microcontroller that supports multiple MFTs can 
* control multiple 3-phase motors.<br>
* The peripherals in the MFT are:<br>
* - Free-run Timer (FRT)
* - Output Compare Unit (OCU)
* - Waveform Generator (WFG)
* - Input Capture Unit (ICU)
* - ADC Start Compare (ADCMP)
* Each MFT has three independent OCU instances with two channels per unit.<br>
* Each OCU generates and outputs PWM signals based on the counter value of the
* FRT and a compare value in the OCU. When the count value matches the compare value,
* the output signal is inverted. Changing the ratio of the compare value of the OCU to the count 
* cycle of the FRT changes the duty cycle of the PWM. You can also change the match conditions 
* for even and odd OCU channels (0-5).<br>
* You can change the OCU compare value at runtime. The OCU supports a buffered compare value. 
* If you use the buffer, the compare value updates at a defined moment (for example, at 
* a zero count or peak count). Otherwise it updates immediately.<br>
* Each OCU uses one of five possible FRT inputs. Three inputs are from the three FRT instances 
* of the corresponding MFT module. The other two FRT inputs are routed from the other 
* possible MFT instances. This enables each OCU to be synchronized using a single FRT, 
* or multiple FRTs.<br>
* You can configure the OCU to generate an interrupt when the compare value matches the counter.<br>
* The output of each OCU is directly routed to a Waveform Generator.
* \section SectionMFT_OCU_ConfigurationConsideration Configuration Consideration
* To set up an OCU, first you should configure an FRT.  See the description in the MFT FRT. <br>
* For the OCU itself, provide configuration parameters in the stc_mft_ocu_config_t structure, 
* such as the FRT to use, whether to enable the buffer for the compare value, and so on. 
* You also enable or disable the interrupt when there is a match between the compare value and 
* counter value. Then call Mft_Ocu_Init().<br>
* You must also call Mft_Ocu_WriteOccp() to set the compare value. This is not part of the 
* configuration structure, because you may modify this at runtime. If you are using buffering,
* the value updates at the defined moment.<br>
* Finally, call Mft_Ocu_EnableOperation() to start the OCU.<br>
* You can enable or disable interrupts at runtime. In polling mode, use Mft_Ocu_GetIrqFlag()  
* to check if the interrupt occurs, and clear the interrupt flag by Mft_Ocu_ClrIrqFlag().
* \section SectionMFT_OCU_MoreInfo More Information
* For more information on the MFT OCU peripheral, refer to:<br>
* <a href="http://www.cypress.com/file/223041/download">FM0+ Peripheral Manual - Timer Subsystem TRM.pdf</a><br>
* <a href="http://www.cypress.com/file/222986/download">FM4 Peripheral Manual - Timer Subsystem TRM.pdf</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/**
* \addtogroup GroupMFT_OCU_Macros
* \{
*/

/******************************************************************************
 * Global definitions
 ******************************************************************************/
#define stc_mftn_ocu_t FM_MFT_OCU_TypeDef

#define MFT0_OCU       (*((volatile stc_mftn_ocu_t *) FM_MFT0_OCU_BASE))
#define MFT1_OCU       (*((volatile stc_mftn_ocu_t *) FM_MFT1_OCU_BASE))
#define MFT2_OCU       (*((volatile stc_mftn_ocu_t *) FM_MFT2_OCU_BASE))

#define  MFT_OCU_CH0             0u
#define  MFT_OCU_CH1             1u
#define  MFT_OCU_CH2             2u
#define  MFT_OCU_CH3             3u 
#define  MFT_OCU_CH4             4u
#define  MFT_OCU_CH5             5u
#define  MFT_OCU_MAXCH           6u

#define  MFT_OCU_CH10            0u
#define  MFT_OCU_CH32            1u
#define  MFT_OCU_CH54            2u

#define  OCU_INSTANCE_COUNT       (PDL_PERIPHERAL_ENABLE_MFT0_OCU == PDL_ON ? 1u :0u) + \
                                  (PDL_PERIPHERAL_ENABLE_MFT1_OCU == PDL_ON ? 1u :0u) + \
                                  (PDL_PERIPHERAL_ENABLE_MFT2_OCU == PDL_ON ? 1u :0u)  

/** \} GroupMFT_OCU_Macros */

/**
* \addtogroup GroupMFT_OCU_Types
* \{
*/

/**
 ******************************************************************************
 ** \brief Enumeration to define an index for each OCU instance
 ******************************************************************************/   
typedef enum en_ocu_instance_index
{
#if (PDL_PERIPHERAL_ENABLE_MFT0_OCU == PDL_ON)  
    OcuInstanceIndexOcu0 = 0u,  ///< Instance index of OCU0
#endif
#if (PDL_PERIPHERAL_ENABLE_MFT1_OCU == PDL_ON) 
    OcuInstanceIndexOcu1,      ///< Instance index of OCU1
#endif 
#if (PDL_PERIPHERAL_ENABLE_MFT2_OCU == PDL_ON) 
    OcuInstanceIndexOcu2,      ///< Instance index of OCU2
#endif    

} en_ocu_instance_index_t;  
  
/**
 ******************************************************************************
 ** \brief select the FRT to be connected to OCU
 ******************************************************************************/
typedef enum en_mft_ocu_frt
{
    Frt0ToOcu   = 0u,       ///< connect FRT0 to OCU
    Frt1ToOcu   = 1u,       ///< connect FRT1 to OCU
    Frt2ToOcu   = 2u,       ///< connect FRT2 to OCU

}en_mft_ocu_frt_t;

/**
 ******************************************************************************
 ** \brief output level of the RT pin state
 ******************************************************************************/
typedef enum en_ocu_rt_out_state
{
    RtLowLevel  = 0u,         ///< output low level to RT pin
    RtHighLevel = 1u,         ///< output high level to RT pin
}en_ocu_rt_out_state_t;

/**
 ******************************************************************************
 ** \brief buffer register function of OCCP
 ******************************************************************************/
typedef enum en_ocu_occp_buf
{
    OccpBufDisable           = 0u,       ///< disable the buffer function
    OccpBufTrsfByFrtZero     = 1u,       ///< buffer transfer when counter value is 0x0000
    OccpBufTrsfByFrtPeak     = 2u,       ///< buffer transfer when counter value is TCCP
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)     
    OccpBufTrsfByFrtZeroPeak = 3u,       ///< buffer transfer when the value is both 0 and TCCP
#endif    
#if (PDL_MCU_CORE == PDL_FM4_CORE)
    #if(PDL_MCU_TYPE >= PDL_FM4_TYPE3)
    OccpBufTrsfByFrtZeroMszcZero = 4u,       ///< buffer transfer when counter value is 0x0000 and zero value detection mask counter value is 0
    OccpBufTrsfByFrtPeakMspcZero = 5u,       ///< buffer transfer when counter value is TCCP and peak value detection mask counter value is 0
    OccpBufTrsfByFrtZeroMszcZeroOrFrtPeakMspcZero = 6u, ///< buffer transfer when counter value is 0x0000 and zero value detection mask counter value is 0 or                                                        
                                                        ///< when counter value is TCCP and peak value detection mask counter value is 0
    #endif
#endif
}en_ocu_occp_buf_t;

#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)   
/**
 ******************************************************************************
 ** \brief buffer register function of OCSE
 ******************************************************************************/
typedef enum en_ocu_ocse_buf
{
    OcseBufDisable           = 0u,       ///< disable the buffer function
    OcseBufTrsfByFrtZero     = 1u,       ///< buffer transfer when counter value is 0x0000
    OcseBufTrsfByFrtPeak     = 2u,       ///< buffer transfer when counter value is TCCP
    OcseBufTrsfByFrtZeroPeak = 3u,       ///< buffer transfer when the value is both 0 and TCCP
#if (PDL_MCU_CORE == PDL_FM4_CORE)
    #if(PDL_MCU_TYPE >= PDL_FM4_TYPE3)
    OcseBufTrsfByFrtZeroMszcZero     = 4u,  ///< buffer transfer when FRT counter value is 0x0000 and zero value detection mask counter value is 0
    OcseBufTrsfByFrtPeakMspcZero     = 5u,  ///< buffer transfer when FRT counter value is TCCP and peak value detection mask counter value is 0    
    OcseBufTrsfByFrtZeroMszcZeroOrFrtPeakMspcZero = 6u,  ///< buffer transfer when FRT counter value is 0x0000 and zero value detection mask counter value is 0 or
                                                         ///< when FRT counter value is TCCP and peak value detection mask counter value is 0 
    #endif
#endif
}en_ocu_ocse_buf_t;
#endif

#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE) 
/**
 ******************************************************************************
 ** \brief RT output status
 ******************************************************************************/
typedef enum en_rt_status
{
    RtOutputHold     = 0u,              ///< RT output hold
    RtOutputHigh     = 1u,              ///< RT output high 
    RtOutputLow      = 2u,              ///< RT output low  
    RtOutputReverse  = 3u,              ///< RT output reverse  
  
}en_rt_even_status_t, en_rt_odd_status_t;

/**
 ******************************************************************************
 ** \brief The condition for IOP flag set
 ******************************************************************************/
typedef enum en_iop_flag_set_condition
{
    IopFlagHold = 0u,           ///< IOP flag hold
    IopFlagSet  = 1u,           ///< IOP flag set
  
}en_iop_flag_even_t, en_iop_flag_odd_t;

#endif

/**
 ******************************************************************************
 ** \brief The compare mode of OCU couple channels (FM3 mode)
 **
 ** \note The OCU channel with 1 change mode or 2 change mode must connect 
 **       with FRT with up count mode.
 **       The OCU channel with active high mode or active low mode must  
 **       connect with FRT with up/down count mode.
 ******************************************************************************/
typedef enum en_ocu_compare_mode 
{
    OcuOdd1ChangeEven1Change    = 0u,    ///< Odd OCU channel is set to 1 change mode, even OCU channel is set to 1 change mode.
    OcuOdd2ChangeEven1Change    = 1u,    ///< Odd OCU channel is set to 2 change mode, even OCU channel is set to 1 change mode.
    OcuOdd1ChangeEvenActiveHigh = 2u,    ///< Odd OCU channel is set to 1 change mode, even OCU channel is set to active high mode.
    OcuOddActiveHighEven1Change = 3u,    ///< Odd OCU channel is set to active high mode, even OCU channel is set to 1 change mode.
    OcuOddActiveLowEven1Change  = 4u,    ///< Odd OCU channel is set to active low mode, even OCU channel is set to 1 change mode.
    OcuOddActiveHighEvenActiveHigh = 5u, ///< Odd OCU channel is set to active high mode, even OCU channel is set to active high mode.
    OcuOddActiveLowEvenActiveLow   = 6u, ///< Odd OCU channel is set to active low mode, even OCU channel is set to active low mode.
    
}en_ocu_compare_mode_t;

/** \} GroupMFT_OCU_Types */

/**
* \addtogroup GroupMFT_OCU_DataStructures
* \{
*/

/**
 ******************************************************************************
 ** \brief OCU configure
 ******************************************************************************/
typedef struct stc_mft_ocu_config
{
    en_mft_ocu_frt_t    enFrtConnect;  ///< select the FRT to be connected to OCU
    en_ocu_occp_buf_t   enOccpBufMode; ///< buffer register function of OCCP
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)   
    en_ocu_ocse_buf_t   enOcseBufMode; ///< buffer register function of OCSE
    boolean_t           bFm4;          ///< TRUE: set the compare mode of used channels with Mft_Ocu_SetEvenChCompareMode() and Mft_Ocu_SetOddChCompareMode()  
                                       ///< FALSE: set the compare mode of used channels with Mft_Ocu_SetCompareMode_Fm3() 
#endif    
    en_ocu_rt_out_state_t enPinState;  ///< RT output level state

#if (PDL_INTERRUPT_ENABLE_MFT0_OCU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_OCU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_OCU == PDL_ON)    
    boolean_t  bIrqEnable;             ///< TRUE: enable OCU interrupt, FALSE: disable OCU interrupt
    func_ptr_t pfnIrqCallback;         ///< Pointer to OCU interrupt callback function.
    boolean_t  bTouchNvic;             ///< TRUE: enable NVIC, FALSE: disable NVIC
#endif    
    
}stc_mft_ocu_config_t;

#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE) 

/**
 ******************************************************************************
 ** \brief The compare configuration of even OCU channel
 ******************************************************************************/
typedef struct stc_ocu_even_compare_config
{
    en_rt_even_status_t enFrtZeroEvenMatchEvenChRtStatus;    ///< Even channel's RT output status when even channel match occurs at the condition of FRT count=0x0000      
    en_rt_even_status_t enFrtZeroEvenNotMatchEvenChRtStatus; ///< Even channel's RT output status when even channel match doesn't occur at the condition of FRT count=0x0000    
    
    en_rt_even_status_t enFrtUpCntEvenMatchEvenChRtStatus;   ///< Even channel's RT output status when even channel match occurs at the condition of FRT is counting up     
    
    en_rt_even_status_t enFrtPeakEvenMatchEvenChRtStatus;  ///< Even channel's RT output status when even channel match occurs at the condition of FRT count=Peak
    en_rt_even_status_t enFrtPeakEvenNotMatchEvenChStatus; ///< Even channel's RT output status when even channel match doesn't occur at the condition of FRT count=Peak
    
    en_rt_even_status_t enFrtDownCntEvenMatchEvenChRtStatus;    ///< Even channel's RT output status when even channel match occurs at the condition of FRT is counting down 
    
    en_iop_flag_even_t enIopFlagWhenFrtZeroEvenMatch;    ///< Even channel OCU's IOP flag status when even channel match occurs at the condition of FRT count=0x0000
    en_iop_flag_even_t enIopFlagWhenFrtUpCntEvenMatch;   ///< Even channel OCU's IOP flag status when even channel match occurs at the condition of FRT is counting up
    en_iop_flag_even_t enIopFlagWhenFrtPeakEvenMatch;    ///< Even channel OCU's IOP flag status when even channel match occurs at the condition of FRT count=Peak
    en_iop_flag_even_t enIopFlagWhenFrtDownCntEvenMatch; ///< Even channel OCU's IOP flag status when even channel match occurs at the condition of FRT is counting down
    
#if (PDL_MCU_CORE == PDL_FM4_CORE)
    #if(PDL_MCU_TYPE >= PDL_FM4_TYPE3)
    boolean_t bFrtZeroPeakExtendMatchCondition;          ///< TRUE: The condition of that OCCP <= FRT counter value will be treated as comparison match
                                                         ///< FALSE: The condition of that OCCP > FRT counter value will be treated as comparison not match
                                                         
    #endif
#endif
    
}stc_ocu_even_compare_config_t;

/**
 ******************************************************************************
 ** \brief The compare configuration of odd OCU channel
 ******************************************************************************/
typedef struct stc_ocu_odd_compare_config
{
    en_rt_odd_status_t enFrtZeroOddMatchEvenMatchOddChRtStatus;       ///< Odd channel's RT output status when even channel and odd channel match occurs at the condition of FRT count=0x0000   
    en_rt_odd_status_t enFrtZeroOddMatchEvenNotMatchOddChRtStatus;    ///< Odd channel's RT output status when even channel not match and odd channel match occurs at the condition of FRT count=0x0000 
    en_rt_odd_status_t enFrtZeroOddNotMatchEvenMatchOddChRtStatus;    ///< Odd channel's RT output status when even channel match and odd channel not match occurs at the condition of FRT count=0x0000 
    en_rt_odd_status_t enFrtZeroOddNotMatchEvenNotMatchOddChRtStatus; ///< Odd channel's RT output status when even channel not match and odd channel not match occurs at the condition of FRT count=0x0000
    
    en_rt_odd_status_t enFrtUpCntOddMatchEvenMatchOddChRtStatus;      ///< Odd channel's RT output status when even channel and odd channel match occurs at the condition of FRT is counting up 
    en_rt_odd_status_t enFrtUpCntOddMatchEvenNotMatchOddChRtStatus;   ///< Odd channel's RT output status when even channel not match and odd channel match occurs at the condition of FRT is counting up
    en_rt_odd_status_t enFrtUpCntOddNotMatchEvenMatchOddChRtStatus;   ///< Odd channel's RT output status when even channel match and odd channel not match occurs at the condition of FRT is counting up
    
    en_rt_odd_status_t enFrtPeakOddMatchEvenMatchOddChRtStatus;      ///< Odd channel's RT output status when even channel and odd channel match occurs at the condition of FRT count=Peak
    en_rt_odd_status_t enFrtPeakOddMatchEvenNotMatchOddChRtStatus;   ///< Odd channel's RT output status when even channel not match and odd channel match occurs at the condition of FRT count=Peak
    en_rt_odd_status_t enFrtPeakOddNotMatchEvenMatchOddChRtStatus;   ///< Odd channel's RT output status when even channel match and odd channel not match occurs at the condition of FRT count=Peak
    en_rt_odd_status_t enFrtPeakOddNotMatchEvenNotMatchOddChRtStatus;///< Odd channel's RT output status when even channel not match and odd channel not match occurs at the condition of FRT count=Peak
    
    en_rt_odd_status_t enFrtDownOddMatchEvenMatchOddChRtStatus;      ///< Odd channel's RT output status when even channel and odd channel match occurs at the condition of FRT is counting down
    en_rt_odd_status_t enFrtDownOddMatchEvenNotMatchOddChRtStatus;   ///< Odd channel's RT output status when even channel not match and odd channel match occurs at the condition of FRT is counting down
    en_rt_odd_status_t enFrtDownOddNotMatchEvenMatchOddChRtStatus;   ///< Odd channel's RT output status when even channel match and odd channel not match occurs at the condition of FRT is coutning down
    
    en_iop_flag_odd_t enIopFlagWhenFrtZeroOddMatch;    ///< Odd channel OCU's IOP flag status when Odd channel match occurs at the condition of FRT count=0x0000
    en_iop_flag_odd_t enIopFlagWhenFrtUpCntOddMatch;   ///< Odd channel OCU's IOP flag status when Odd channel match occurs at the condition of FRT is counting up
    en_iop_flag_odd_t enIopFlagWhenFrtPeakOddMatch;    ///< Odd channel OCU's IOP flag status when Odd channel match occurs at the condition of FRT count=Peak
    en_iop_flag_odd_t enIopFlagWhenFrtDownCntOddMatch; ///< Odd channel OCU's IOP flag status when Odd channel match occurs at the condition of FRT is counting down

#if (PDL_MCU_CORE == PDL_FM4_CORE)
    #if(PDL_MCU_TYPE >= PDL_FM4_TYPE3)
    boolean_t bFrtZeroPeakExtendMatchCondition;          ///< TRUE: The condition of that OCCP <= FRT counter value will be treated as comparison match
                                                         ///< FALSE: The condition of that OCCP > FRT counter value will be treated as comparison not match
                                                         
    #endif
#endif
    
}stc_ocu_odd_compare_config_t;

#endif

/**
 ******************************************************************************
 ** \brief Structure of OCU internal data
 ******************************************************************************/
typedef struct stc_mft_ocu_intern_data
{
    func_ptr_t  pfnOcu0IrqCallback;  ///< Callback function pointer of OCU0 interrupt 
    func_ptr_t  pfnOcu1IrqCallback;  ///< Callback function pointer of OCU1 interrupt 
    func_ptr_t  pfnOcu2IrqCallback;  ///< Callback function pointer of OCU2 interrupt 
    func_ptr_t  pfnOcu3IrqCallback;  ///< Callback function pointer of OCU3 interrupt 
    func_ptr_t  pfnOcu4IrqCallback;  ///< Callback function pointer of OCU4 interrupt 
    func_ptr_t  pfnOcu5IrqCallback;  ///< Callback function pointer of OCU5 interrupt 
}stc_mft_ocu_intern_data_t;

/**
 ******************************************************************************
 ** \brief OCU instance data type
 ******************************************************************************/
typedef struct stc_mft_ocu_instance_data
{
  volatile stc_mftn_ocu_t*  pstcInstance;  ///< pointer to registers of an instance
  stc_mft_ocu_intern_data_t stcInternData; ///< module internal data of instance
} stc_mft_ocu_instance_data_t;

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/** \} GroupMFT_OCU_DataStructures */

/**
* \addtogroup GroupMFT_OCU_GlobalVariables
* \{
*/

/******************************************************************************/
/* Global variable declarations ('extern', definition in C source)            */
/******************************************************************************/
/// Look-up table for all enabled OCU  instances and their internal data
extern stc_mft_ocu_instance_data_t m_astcMftOcuInstanceDataLut[OCU_INSTANCE_COUNT];

/** \} GroupMFT_OCU_GlobalVariables */

/**
* \addtogroup GroupMFT_OCU_Functions
* \{
*/

/******************************************************************************/
/* Global function prototypes ('extern', definition in C source)              */
/******************************************************************************/
/* Init */
en_result_t Mft_Ocu_Init( volatile stc_mftn_ocu_t* pstcOcu, 
                          uint8_t u8Ch, 
                          const stc_mft_ocu_config_t*  pstcOcuConfig);
en_result_t Mft_Ocu_DeInit( volatile stc_mftn_ocu_t* pstcOcu, 
                            uint8_t u8Ch,
                            boolean_t bTouchNvic);
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)
/* Compare configuration */
en_result_t Mft_Ocu_SetEvenChCompareMode(volatile stc_mftn_ocu_t* pstcOcu,
                                         uint8_t EvenCh,
                                         stc_ocu_even_compare_config_t* pstcConfig);
en_result_t Mft_Ocu_SetOddChCompareMode (volatile stc_mftn_ocu_t* pstcOcu,
                                         uint8_t OddCh,
                                         stc_ocu_odd_compare_config_t* pstcConfig);
#endif
en_result_t Mft_Ocu_SetCompareMode_Fm3(volatile stc_mftn_ocu_t* pstcOcu,
                                       uint8_t u8CoupleCh,
                                       en_ocu_compare_mode_t enCompareMode);

/* Func/Int enable/disable */
en_result_t Mft_Ocu_EnableOperation(volatile stc_mftn_ocu_t* pstcOcu, uint8_t u8Ch);
en_result_t Mft_Ocu_DisableOperation(volatile stc_mftn_ocu_t* pstcOcu, uint8_t u8Ch);
#if (PDL_INTERRUPT_ENABLE_MFT0_OCU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_OCU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_OCU == PDL_ON)
en_result_t Mft_Ocu_EnableIrq(volatile stc_mftn_ocu_t* pstcOcu,
                              uint8_t u8Ch,func_ptr_t  pfnCallback);
en_result_t Mft_Ocu_DisableIrq(volatile stc_mftn_ocu_t* pstcOcu,uint8_t u8Ch);
#endif

/* Count write/read */
en_result_t Mft_Ocu_WriteOccp(volatile stc_mftn_ocu_t* pstcOcu, 
                              uint8_t u8Ch, uint16_t u16Occp);
uint16_t Mft_Ocu_ReadOccp(volatile stc_mftn_ocu_t* pstcOcu, uint8_t u8Ch);

/* Status read/clear */
en_irq_flag_t Mft_Ocu_GetIrqFlag(volatile stc_mftn_ocu_t* pstcOcu, uint8_t u8Ch);
en_result_t Mft_Ocu_ClrIrqFlag(volatile stc_mftn_ocu_t* pstcOcu, uint8_t u8Ch);

/* Get RT pin level */
en_ocu_rt_out_state_t Mft_Ocu_GetRtPinLevel(volatile stc_mftn_ocu_t* pstcOcu, 
                                            uint8_t u8Ch);

/* IRQ handler */
void Mft_Ocu_IrqHandler( volatile stc_mftn_ocu_t* pstcOcu,
                         stc_mft_ocu_intern_data_t* pstcMftOcuInternData) ;

/** \} GroupMFT_OCU_Functions */
/** \} GroupMFT_OCU */

#ifdef __cplusplus
}
#endif

#endif // #if (defined(PDL_PERIPHERAL_ADC_ACTIVE))

#endif
