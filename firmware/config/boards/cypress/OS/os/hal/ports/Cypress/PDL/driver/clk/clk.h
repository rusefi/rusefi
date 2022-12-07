/******************************************************************************
* \file             clk.h
*
* \version          1.20
*
* \brief            Header file of Clock functions
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

#ifndef __CLK_H__
#define __CLK_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if (defined(PDL_PERIPHERAL_CLK_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
* \defgroup GroupCLK Clock Functions (CLK)
* \{
* \defgroup GroupCLK_Macros Macros
* \defgroup GroupCLK_Types Enumerated Types
* \defgroup GroupCLK_DataStructures Data Structures
* \defgroup GroupCLK_GlobalVariables Global Variables
* \defgroup GroupCLK_Functions Functions
* \}
*/

/**
* \addtogroup GroupCLK
* The clock generation unit generates various types of clocks that can be used 
* to operate the MCU. There are five possible source clocks:<br> 
* - Main clock (CLKMO)<br>
* - Sub clock (CLKSO)<br>
* - High-speed CR clock (CLKHC)<br>
* - Low-speed CR clock (CLKLC)<br>
* - Main PLL clock (CLKPLL)<br>
* Select one of the source clocks to be the master clock. You divide the master clock frequency to 
* generate internal bus clocks. There are five internal bus clocks:<br>
* - Base clock (FCLK/HCLK)<br>
* - APB0 bus clock (PCLK0)<br>
* - APB1 bus clock (PCLK1) <br>
* - APB2 bus clock (PCLK2)<br>
* - TRACE clock (TPIUCLK)<br>
* 
* \section SectionCLK_ConfigurationConsideration Configuration Consideration
* The data sheet for a particular series specifies the  
* "Internal operating clock frequency: Fcc (Base clock HCLK/FCLK)" 
* for a CPU. The master clock value should not be larger than the maximum value of the Internal operating clock frequency.<br>
* Specify the source clock by calling Clk_SetSource(). You can use API function calls to enable or disable any source clock.<br>
* To set up bus clocks, provide configuration parameters in the stc_clk_config_t  structure. For example, you specify dividers for 
* each of the internal bus clocks, including the base clock. You also set values for the main PLL clock. Then call Clk_Init(). <br>
* 
* Detailed information for each of the internal bus clocks:<br>
* - Base clock (HCLK/FCLK)
* HCLK and FCLK are collectively called the base clock. Both HCLK and FCLK are 
* supplied to the CPU.
* HCLK is the clock for peripherals connected to the AHB bus.
* The clock frequency can be set to between 1/1 and 1/16 frequency of the 
* master clock. This clock stops in timer mode, RTC mode, stop mode, deep 
* standby RTC mode, and deep standby stop mode. In sleep mode, the CPU stops 
* the supply of HCLK while continuing the supply of FCLK.<br>
* - APB0 bus clock (PCLK0)
* PCLK0 is the clock for peripherals connected to the APB0 bus.
* The clock frequency can be set to between 1/1 and 1/8 frequency of the base clock.
* This clock stops in timer mode, RTC mode, stop mode, deep standby RTC mode, 
* and deep standby stop mode.<br>
* - APB1 bus clock (PCLK1)
* PCLK1 is the clock for peripherals connected to the APB1 bus.
* The clock frequency can be set to between 1/1 and 1/8 frequency of the base clock.
* This clock stops in timer mode, RTC mode, stop mode, deep standby RTC mode, 
* and deep standby stop mode.<br>
* - APB2 bus clock (PLCK2)
* PCLK2 is the clock for peripherals connected to the APB2 bus.
* The clock frequency can be set to between 1/1 and 1/8 frequency of the base clock.
* This clock stops in timer mode, RTC mode, stop mode, deep standby RTC mode, and deep standby stop mode.<br>
* - TRACE clock (TPIUCLK)
* This is the clock for TRACE.
* The clock frequency can be set to between 1/1 and 1/8 frequency of the base clock.
* This clock stops in timer mode, RTC mode, stop mode, deep standby RTC mode and deep standby stop mode.
* This clock output is enabled only for products equipped with Embedded Trace Macrocell.<br>
*
* \section SectionCLK_MoreInfo More Information
For more information on the CLK peripheral, refer to:<br> 
* <a href="http://www.cypress.com/file/223051/download">FM0+ Peripheral Manual Core Subsystem</a><br>
* <a href="http://www.cypress.com/file/222996/download">FM4 Peripheral Manual Core Subsystem</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/
  
/**
* \addtogroup GroupCLK_Macros
* \{
*/  
/******************************************************************************
 * Global type definitions
 ******************************************************************************/
// PLL Input Clock Select (PINC) Bit Position
#define FM_PSW_TMR_PINC_BITPOS 0x10u
  
// Interrupt enable bitpositions in INT_ENR
#define FM_INT_ENR_PCSE_BITPOS 0x04u
#define FM_INT_ENR_SCSE_BITPOS 0x02u
#define FM_INT_ENR_MCSE_BITPOS 0x01u
  
// Interrupt status bitpositions in INT_STR
#define FM_INT_STR_PCSI_BITPOS 0x04u
#define FM_INT_STR_SCSI_BITPOS 0x02u
#define FM_INT_STR_MCSI_BITPOS 0x01u
  
// Interrupt ckear bitpositions in INT_CLR
#define FM_INT_CLR_PCSC_BITPOS 0x04u
#define FM_INT_CLR_SCSC_BITPOS 0x02u
#define FM_INT_CLR_MCSC_BITPOS 0x01u

/** \} GroupCLK_Macros */

/**
* \addtogroup GroupCLK_Types
* \{
*/

/**
 ******************************************************************************
 ** \brief Clock Source
 ** 
 ** Differentiator for the different clock sources
 **
 ** \note The enumerated values do not correspond to the RCS/RCM bits of the
 **       clock control and status registers due to having upward compatibility,
 **       if this bit coding may change in future devices. The correct bit
 **       patterns are set by switch(en_clk_source)/case statements individually
 **       in the corresponding function(s).
 ******************************************************************************/
typedef enum en_clk_source
{
    ClkMain = 0u,  ///< Main Clock Oscillator
    ClkSub  = 1u,  ///< Sub Clock Oscillator
    ClkHsCr = 2u,  ///< High-Speed CR Clock Oscillator
    ClkLsCr = 3u,  ///< Low-Speed CR Clock Oscillator
    ClkPll  = 4u,  ///< PLL Clock
    ClkHsCrPll = 5u   ///< High-Speed CR PLL Clock
} en_clk_source_t;

/**
 ******************************************************************************
 ** \brief Base Clock Prescaler Settings
 ** 
 ** Enumeration of the dividers of the Base Clock (HCLK)
 ******************************************************************************/
typedef enum en_clk_baseclkdiv
{
    BaseClkDiv1  = 0u,  ///< HCLK Division 1/1
    BaseClkDiv2  = 1u,  ///< HCLK Division 1/2
    BaseClkDiv3  = 2u,  ///< HCLK Division 1/3
    BaseClkDiv4  = 3u,  ///< HCLK Division 1/4
    BaseClkDiv6  = 4u,  ///< HCLK Division 1/6
    BaseClkDiv8  = 5u,  ///< HCLK Division 1/8    
    BaseClkDiv16 = 6u,  ///< HCLK Division 1/16
    BaseClkErr   = 7u   ///< HCLK prohibited setting
} en_clk_baseclkdiv_t;

/**
 ******************************************************************************
 ** \brief APB0 Prescaler Settings
 ** 
 ** Enumeration of the dividers of the APB0 (PCLK0)
 ******************************************************************************/
typedef enum en_clk_apb0div
{
    Apb0Div1  = 0u,  ///< PCLK0 Division 1/1
    Apb0Div2  = 1u,  ///< PCLK0 Division 1/2
    Apb0Div4  = 2u,  ///< PCLK0 Division 1/4
    Apb0Div8  = 3u   ///< PCLK0 Division 1/8
} en_clk_apb0div_t;

/**
 ******************************************************************************
 ** \brief APB1 Prescaler Settings
 ** 
 ** Enumeration of the dividers of the APB0 (PCLK1)
 ******************************************************************************/
typedef enum en_clk_apb1div
{
    Apb1Div1  = 0u,  ///< PCLK1 Division 1/1
    Apb1Div2  = 1u,  ///< PCLK1 Division 1/2
    Apb1Div4  = 2u,  ///< PCLK1 Division 1/4
    Apb1Div8  = 3u   ///< PCLK1 Division 1/8
} en_clk_apb1div_t;

#if (PDL_MCU_CORE == PDL_FM3_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)    
/**
 ******************************************************************************
 ** \brief APB2 Prescaler Settings
 ** 
 ** Enumeration of the dividers of the APB0 (PCLK2)
 ******************************************************************************/
typedef enum en_clk_apb2div
{
    Apb2Div1  = 0u,  ///< PCLK2 Division 1/1
    Apb2Div2  = 1u,  ///< PCLK2 Division 1/2
    Apb2Div4  = 2u,  ///< PCLK2 Division 1/4
    Apb2Div8  = 3u   ///< PCLK2 Division 1/8
} en_clk_apb2div_t;
#endif

/**
 ******************************************************************************
 ** \brief Sub Clock oscillation stablilization wait time
 ** 
 ** Enumeration for the Sub Clock oscillation stabilization wait time settings
 ******************************************************************************/
typedef enum en_clk_scowaittime
{
    ScoWaitExp10  =  0u,  ///< 2^10 / F(CL) => ~10.3  ms
    ScoWaitExp11  =  1u,  ///< 2^11 / F(CL) => ~20.5  ms
    ScoWaitExp12  =  2u,  ///< 2^12 / F(CL) => ~41    ms
    ScoWaitExp13  =  3u,  ///< 2^13 / F(CL) => ~82    ms
    ScoWaitExp14  =  4u,  ///< 2^14 / F(CL) => ~164   ms
    ScoWaitExp15  =  5u,  ///< 2^15 / F(CL) => ~327   ms
    ScoWaitExp16  =  6u,  ///< 2^16 / F(CL) => ~655   ms
    ScoWaitExp17  =  7u,  ///< 2^17 / F(CL) => ~1.31  s      
    ScoWaitExp18  =  8u,  ///< 2^17 / F(CL) => ~2.62  s
    ScoWaitExp19  =  9u,  ///< 2^17 / F(CL) => ~5.24  s 
    ScoWaitExp20  = 10u,  ///< 2^17 / F(CL) => ~10.48 s
    ScoWaitExp21  = 11u,  ///< 2^17 / F(CL) => ~20.96 s 
    ScoWaitErr    = 12u   ///< Prohibited Setting
} en_clk_scowaittime_t;

/**
 ******************************************************************************
 ** \brief Main Clock oscillation stablilization wait time
 ** 
 ** Enumeration for the Main Clock oscillation stabilization wait time settings
 ******************************************************************************/
typedef enum en_clk_mcowaittime
{
    McoWaitExp11   = 0u,   ///< 2^1  / F(CH) => ~500  ns (F(CH) = 4 MHz)
    McoWaitExp15   = 1u,   ///< 2^5  / F(CH) => ~8    us (F(CH) = 4 MHz)
    McoWaitExp16   = 2u,   ///< 2^6  / F(CH) => ~16   us (F(CH) = 4 MHz)
    McoWaitExp17   = 3u,   ///< 2^7  / F(CH) => ~32   us (F(CH) = 4 MHz)
    McoWaitExp18   = 4u,   ///< 2^8  / F(CH) => ~64   us (F(CH) = 4 MHz)
    McoWaitExp19   = 5u,   ///< 2^9  / F(CH) => ~128  us (F(CH) = 4 MHz)
    McoWaitExp110  = 6u,   ///< 2^10 / F(CH) => ~256  us (F(CH) = 4 MHz)
    McoWaitExp111  = 7u,   ///< 2^11 / F(CH) => ~512  us (F(CH) = 4 MHz)
    McoWaitExp112  = 8u,   ///< 2^12 / F(CH) => ~1.0  ms (F(CH) = 4 MHz)
    McoWaitExp113  = 9u,   ///< 2^13 / F(CH) => ~2.0  ms (F(CH) = 4 MHz)
    McoWaitExp114  = 10u,  ///< 2^14 / F(CH) => ~4.0  ms (F(CH) = 4 MHz)
    McoWaitExp115  = 11u,  ///< 2^15 / F(CH) => ~8.0  ms (F(CH) = 4 MHz)
    McoWaitExp117  = 12u,  ///< 2^17 / F(CH) => ~33.0 ms (F(CH) = 4 MHz)
    McoWaitExp119  = 13u,  ///< 2^18 / F(CH) => ~131  ms (F(CH) = 4 MHz)
    McoWaitExp121  = 14u,  ///< 2^21 / F(CH) => ~524  ms (F(CH) = 4 MHz)
    McoWaitExp123  = 15u   ///< 2^23 / F(CH) => ~2.0  s  (F(CH) = 4 MHz)
} en_clk_mcowaittime_t;

/**
 ******************************************************************************
 ** \brief PLL Clock oscillation stablilization wait time
 ** 
 ** Enumeration for the PLL Clock oscillation stabilization wait time settings
 ******************************************************************************/
typedef enum en_clk_pllowaittime
{
    PlloWaitExp19   = 0u,   ///< 2^9  / F(CH) => ~128  us (F(CH) = 4 MHz)
    PlloWaitExp110  = 1u,   ///< 2^10 / F(CH) => ~256  us (F(CH) = 4 MHz)
    PlloWaitExp111  = 2u,   ///< 2^11 / F(CH) => ~512  us (F(CH) = 4 MHz)
    PlloWaitExp112  = 3u,   ///< 2^12 / F(CH) => ~1.02 ms (F(CH) = 4 MHz)
    PlloWaitExp113  = 4u,   ///< 2^13 / F(CH) => ~2.05 ms (F(CH) = 4 MHz)
    PlloWaitExp114  = 5u,   ///< 2^14 / F(CH) => ~4.10 ms (F(CH) = 4 MHz)
    PlloWaitExp115  = 6u,   ///< 2^15 / F(CH) => ~8.20 ms (F(CH) = 4 MHz)
    PlloWaitExp116  = 7u    ///< 2^16 / F(CH) => ~16.4 ms (F(CH) = 4 MHz)
} en_clk_pllowaittime_t;

/**
 ******************************************************************************
 ** \brief PLL SourceClock (PINC bit of PSW_TMR)
 ** 
 ** Enumeration for the PLL Clock Source.
 **
 ** \attention HS-RC source only available, if used device supports PLL-CLKHC
 **            setting! PINC bit availability is <b>not</b> checked in this
 **            driver!
 ******************************************************************************/
typedef enum en_clk_pll_src
{
    PllSrcClkMo = 0u,   ///< Use Main Clock as PLL source (always available, default)
    PllSrcClkHc = 123u  ///< Use HS-RC Clock as PLL source (only if available!)
} en_clk_pll_src_t;

/**
 ******************************************************************************
 ** \brief Clock Gate peripheral enumerators
 **  
 ** \note Though all peripherals are listed here, it doesn't mean a certain  
 **       product has all these peripheral, please check the at "product lineup" 
 **       section in the product data sheet for which peripheral are available.
 ******************************************************************************/
typedef enum en_clk_gate_peripheral
{
    ClkGateGpio   = 0u,    ///< GPIO clock gate
    ClkGateExtif  = 1u,    ///< External bus clock gate
    ClkGateDma    = 2u,    ///< DMA clock gate
    ClkGateAdc0   = 3u,    ///< ADC0 clock gate
    ClkGateAdc1   = 4u,    ///< ADC1 clock gate
    ClkGateAdc2   = 5u,    ///< ADC2 clock gate
    ClkGateAdc3   = 6u,    ///< ADC3 clock gate
    ClkGateMfs0   = 7u,    ///< MFS0 clock gate
    ClkGateMfs1   = 8u,    ///< MFS1 clock gate
    ClkGateMfs2   = 9u,    ///< MFS2 clock gate
    ClkGateMfs3   = 10u,   ///< MFS3 clock gate
    ClkGateMfs4   = 11u,   ///< MFS4 clock gate
    ClkGateMfs5   = 12u,   ///< MFS5 clock gate
    ClkGateMfs6   = 13u,   ///< MFS6 clock gate
    ClkGateMfs7   = 14u,   ///< MFS7 clock gate
    ClkGateMfs8   = 15u,   ///< MFS8 clock gate
    ClkGateMfs9   = 16u,   ///< MFS9 clock gate
    ClkGateMfs10  = 17u,   ///< MFS10 clock gate
    ClkGateMfs11  = 18u,   ///< MFS11 clock gate
    ClkGateMfs12  = 19u,   ///< MFS12 clock gate
    ClkGateMfs13  = 20u,   ///< MFS13 clock gate
    ClkGateMfs14  = 21u,   ///< MFS14 clock gate
    ClkGateMfs15  = 22u,   ///< MFS15 clock gate
    ClkGateQprc0  = 23u,   ///< QPRC0 clock gate
    ClkGateQprc1  = 24u,   ///< QPRC1 clock gate
    ClkGateQprc2  = 25u,   ///< QPRC2 clock gate
    ClkGateQprc3  = 26u,   ///< QPRC3 clock gate
    ClkGateMft0   = 27u,   ///< MFT0, PPG0/2/4/6 clock gate
    ClkGateMft1   = 28u,   ///< MFT1, PPG8/10/12/14 clock gate
    ClkGateMft2   = 29u,   ///< MFT2, PPG16/18/20/22 clock gate
    ClkGateMft3   = 30u,   ///< MFT3, PPG24/26/28/30 clock gate
    ClkGateBt0123      = 31u,   ///< BT0/1/2/3 clock gate
    ClkGateBt4567      = 32u,   ///< BT4/5/6/7 clock gate
    ClkGateBt891011    = 33u,   ///< BT8/9/10/11 clock gate
    ClkGateBt12131415  = 34u,   ///< BT12/13/14/15 clock gate
    ClkGateQspi   = 35u,   ///< Quad SPI clock gate
    ClkGateCec0   = 36u,   ///< HDMI-CEC0 clock gate
    ClkGateCec1   = 37u,   ///< HDMI-CEC1 clock gate
    ClkGatePcrc   = 38u,   ///< CRC clock gate
    ClkGateI2s0   = 39u,   ///< I2S0 clock gate
    ClkGateI2s1   = 40u,   ///< I2S1 clock gate    
    ClkGateSd     = 41u,   ///< SD clock gate
    ClkGateCan0   = 42u,   ///< CAN0 clock gate
    ClkGateCan1   = 43u,   ///< CAN1 clock gate
    ClkGateCan2   = 44u,   ///< CAN2 clock gate
    ClkGateUsb0   = 45u,   ///< USB0 clock gate
    ClkGateUsb1   = 46u,   ///< USB1 clock gate
    ClkGateIcc0   = 47u,   ///< IC-Card interface 0
    ClkGateIcc1   = 48u,   ///< IC-Card interface 1
    ClkGateI2sl0  = 49u,   ///< I2S-Lite 0
    ClkGateI2sl1  = 50u,   ///< I2S-Lite 1    
} en_clk_gate_peripheral_t;

/**
 ******************************************************************************
 ** \brief Reset peripheral enumerators
 **
 ** \note Though all peripherals are listed here, it doesn't mean a certain  
 **       product has all these peripheral, please check the at "product lineup" 
 **       section in the product data sheet for which peripheral are available. 
 ******************************************************************************/
typedef enum en_clk_reset_peripheral
{
    ClkResetExtif  = 1u,    ///< External bus Reset
    ClkResetDma    = 2u,    ///< DMA Reset
    ClkResetAdc0   = 3u,    ///< ADC0 Reset
    ClkResetAdc1   = 4u,    ///< ADC1 Reset
    ClkResetAdc2   = 5u,    ///< ADC2 Reset
    ClkResetAdc3   = 6u,    ///< ADC3 Reset
    ClkResetMfs0   = 7u,    ///< MFS0 Reset
    ClkResetMfs1   = 8u,    ///< MFS1 Reset
    ClkResetMfs2   = 9u,    ///< MFS2 Reset
    ClkResetMfs3   = 10u,   ///< MFS3 Reset
    ClkResetMfs4   = 11u,   ///< MFS4 Reset
    ClkResetMfs5   = 12u,   ///< MFS5 Reset
    ClkResetMfs6   = 13u,   ///< MFS6 Reset
    ClkResetMfs7   = 14u,   ///< MFS7 Reset
    ClkResetMfs8   = 15u,   ///< MFS8 Reset
    ClkResetMfs9   = 16u,   ///< MFS9 Reset
    ClkResetMfs10  = 17u,   ///< MFS10 Reset
    ClkResetMfs11  = 18u,   ///< MFS11 Reset
    ClkResetMfs12  = 19u,   ///< MFS12 Reset
    ClkResetMfs13  = 20u,   ///< MFS13 Reset
    ClkResetMfs14  = 21u,   ///< MFS14 Reset
    ClkResetMfs15  = 22u,   ///< MFS15 Reset
    ClkResetQprc0  = 23u,   ///< QPRC0 Reset
    ClkResetQprc1  = 24u,   ///< QPRC1 Reset
    ClkResetQprc2  = 25u,   ///< QPRC2 Reset
    ClkResetQprc3  = 26u,   ///< QPRC3 Reset
    ClkResetMft0   = 27u,   ///< MFT0, PPG0/2/4/6 Reset
    ClkResetMft1   = 28u,   ///< MFT1, PPG8/10/12/14 Reset
    ClkResetMft2   = 29u,   ///< MFT2, PPG16/18/20/22 Reset
    ClkResetMft3   = 30u,   ///< MFT3, PPG24/26/28/30 Reset
    ClkResetBt0123      = 31u,   ///< BT0/1/2/3 Reset
    ClkResetBt4567      = 32u,   ///< BT4/5/6/7 Reset
    ClkResetBt891011    = 33u,   ///< BT8/9/19/11 Reset
    ClkResetBt12131415  = 34u,   ///< BT12/13/14/15 Reset
    ClkResetQspi   = 35u,   ///< Quad SPI Reset
    ClkResetCec0   = 36u,   ///< CEC0 Reset
    ClkResetCec1   = 37u,   ///< CEC1 Reset
    ClkResetPcrc   = 38u,   ///< Programmable CRC Reset
    ClkResetI2s0   = 39u,   ///< I2S0 Reset
    ClkResetI2s1   = 40u,   ///< I2S1 Reset
    ClkResetSd     = 41u,   ///< SD Card I/F Reset
    ClkResetCan0   = 42u,   ///< CAN0 Reset
    ClkResetCan1   = 43u,   ///< CAN1 Reset
    ClkResetCan2   = 44u,   ///< CAN2 Reset
    ClkResetUsb0   = 45u,   ///< USB0 Reset
    ClkResetUsb1   = 46u,   ///< USB1 Reset   
    ClkResetIcc0   = 47u,   ///< IC-Card interface 0
    ClkResetIcc1   = 48u,   ///< IC-Card interface 1
    ClkResetI2sl0  = 49u,   ///< I2S-Lite 0
    ClkResetI2sl1  = 50u,   ///< I2S-Lite 1
} en_clk_reset_peripheral_t;

/** \}GroupCLK_Types */

/**
* \addtogroup GroupCLK_DataStructures
* \{
*/

/**
 ******************************************************************************
 ** \brief Datatype for holding internal data needed for CLK
 ******************************************************************************/
#if (PDL_INTERRUPT_ENABLE_CLK == PDL_ON)
typedef struct stc_clk_intern_data
{
    func_ptr_t            pfnPllStabCb;     ///< PLL stabilization callback function pointer
    func_ptr_t            pfnScoStabCb;     ///< Sub Clock stabilization callback function pointer
    func_ptr_t            pfnMcoStabCb;     ///< Main Clock stabilization callback function pointer
} stc_clk_intern_data_t ;
#endif

/**
 ******************************************************************************
 ** \brief Clock configuration
 ** 
 ** The Clock configuration settings
 ******************************************************************************/
typedef struct stc_clk_config
{   
    en_clk_baseclkdiv_t   enBaseClkDiv;     ///< See description of #en_clk_baseclkdiv_t
    en_clk_apb0div_t      enAPB0Div;        ///< See description of #en_clk_apb0div_t
    en_clk_apb1div_t      enAPB1Div;        ///< See description of #en_clk_apb1div_t
#if (PDL_MCU_CORE == PDL_FM3_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)  
    en_clk_apb2div_t      enAPB2Div;        ///< See description of #en_clk_apb2div_t 
#endif    
    boolean_t             bAPB1Disable;     ///< TRUE: Disables APB1 regardless of divider settings
#if (PDL_MCU_CORE == PDL_FM3_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)      
    boolean_t             bAPB2Disable;     ///< TRUE: Disables APB2 regardless of divider settings
#endif       
    en_clk_mcowaittime_t  enMCOWaitTime;    ///< See description of #en_clk_mcowaittime_t
    en_clk_scowaittime_t  enSCOWaitTime;    ///< See description of #en_clk_scowaittime_t
    en_clk_pllowaittime_t enPLLOWaitTime;   ///< See description of #en_clk_pllowaittime_t
    uint8_t               u8PllK;           ///< PLL input clock frequency division ratio, PLLK
    uint8_t               u8PllM;           ///< PLL VCO clock frequency division ratio, PLLM
    uint8_t               u8PllN;           ///< PLL feedback frequency division ration, PLLN
#if (PDL_INTERRUPT_ENABLE_CLK == PDL_ON)
    boolean_t             bPllIrq;          ///< TRUE: enable PLL oscillation stabilization completion interrupt
    boolean_t             bMcoIrq;          ///< TRUE: enable Main Clock oscillation stabilization completion interrupt
    boolean_t             bScoIrq;          ///< TRUE: enable Sub Clock oscillation stabilization completion interrupt
    func_ptr_t            pfnPllStabCb;     ///< PLL stabilization callback function pointer
    func_ptr_t            pfnMcoStabCb;     ///< Main Clock stabilization callback function pointer
    func_ptr_t            pfnScoStabCb;     ///< Sub Clock stabilization callback function pointer
#endif   
} stc_clk_config_t;

/** \} GroupCLK_DataStructures */

/**
* \addtogroup GroupCLK_GlobalVariables
* \{
*/
/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/
#if (PDL_INTERRUPT_ENABLE_CLK == PDL_ON)
extern stc_clk_intern_data_t stcClkInternData;
#endif

/** \} GroupCLK_GlobalVariables */
/**
* \addtogroup GroupCLK_Functions
* \{
*/

/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/

#if (PDL_INTERRUPT_ENABLE_CLK == PDL_ON)
void Clk_IrqHandler(void) ;
#endif
/* Set clock dividor, wait time, interrupt */
en_result_t Clk_Init(const stc_clk_config_t* pstcClk) ;
/* Clock enable/disable */
#if (PDL_MCU_CORE == PDL_FM0P_CORE) 
en_result_t Clk_EnableHscr(boolean_t bBlock);
en_result_t Clk_DisableHscr(void);
#endif
en_result_t Clk_EnableMainClock(boolean_t bBlock);
en_result_t Clk_DisableMainClock(void);
en_result_t Clk_EnableSubClock(boolean_t bBlock);
en_result_t Clk_DisableSubClock(void);
en_result_t Clk_EnablePllClock(boolean_t bBlock);
en_result_t Clk_DisablePllClock(void);
/* Set the source clock */
en_result_t Clk_SetSource(en_clk_source_t enSource);
#if (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)  
/* Peripheral clock enable/disable/status read */
en_result_t Clk_PeripheralClockEnable(en_clk_gate_peripheral_t enPeripheral) ;
en_result_t Clk_PeripheralClockDisable(en_clk_gate_peripheral_t enPeripheral) ;
boolean_t Clk_PeripheralGetClockState(en_clk_gate_peripheral_t enPeripheral) ;
en_result_t Clk_PeripheralClockEnableAll(void) ;
en_result_t Clk_PeripheralClockDisableAll(void) ;
/* Peripheral set reset/release reset */
en_result_t Clk_PeripheralSetReset(en_clk_reset_peripheral_t enPeripheral) ;
en_result_t Clk_PeripheralClearReset(en_clk_reset_peripheral_t enPeripheral) ;
#endif

/** \} GroupCLK_Functions */
/** \} GroupCLK */

#ifdef __cplusplus
}
#endif

#endif // #if (defined(PDL_PERIPHERAL_CLK_ACTIVE))

#endif /* __CLK_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
