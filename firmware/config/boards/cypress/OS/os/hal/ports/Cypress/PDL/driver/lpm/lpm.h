/*******************************************************************************
* \file              lpm.h
* 
* \version           1.30
*
* \brief             Headerfile for LPM functions.
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

#ifndef __LPM_H__
#define __LPM_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if (defined(PDL_PERIPHERAL_LPM_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
* \defgroup GroupLPM Low Power consumption Mode (LPM)
* \{ 
* \defgroup GroupLPM_Macros Macros
* \defgroup GroupLPM_Functions Functions
* \defgroup GroupLPM_DataStructures Data Structures
* \defgroup GroupLPM_Types Enumerated Types
* \}
*/

/**
* \addtogroup GroupLPM
* \{
* The Low Power Consumption Mode (LPM) peripheral enables reduced power consumption.<br>
* To reduce the power consumption, the system provides several low power consumption modes. LPM supports:<br>
* * Standby sleep
* * Standby timer
* * Standby RTC
* * Standby stop
* * Deep standby RTC
* * Deep standby stop<br>
* The LPM peripheral differs among different devices. For example, 
* deep standby may not be available on a particular device. You can find 
* information for your device in the corresponding peripheral manual.
*
* \section SectionLPM_ConfigurationConsideration Configuration Consideration
* There is no configuration structure. To enter low power consumption mode 
* call Lpm_GoToStandByMode() and specify which low power mode to use. 
* See en_lpm_mode_t for the choices. The second argument specifies 
* whether to save IO settings or set all IO to high impedance.<br>
* When you make this call, the MCU enters low power consumption mode 
* immediately. This function does not return until the MCU wakes up 
* for some reason. The return path will vary. From standby modes, the 
* function returns normally and your program can resume operation. 
* From deep standby modes, control returns from the reset vector. <br>
* To configure the reset vector, use Lpm_ConfigDeepStbRetCause() to 
* set the return causes and Lpm_ReadDeepStbRetCause() to read the return 
* cause after the MCU wakes up.<br>
* \note Wakeup from the WKUP0 pin is always enabled.
* Use API function calls to:<br>
* * Read or write the contents of the backup registers
* * Retain the contents of RAM
* * Set the wakeup pin level
* * Configure the subclock for RTC and CEC in deep standby mode
* Lpm_SetWkupPinLevel() is used to set the WKUP pin active level.<br> 
* \note Low power feature availability varies among FM products, as does 
* the size of the backup registers. The deep standby mode may not be 
* available for some FM products. Consult the corresponding data sheet or 
* peripheral manual for details for your part.<br>
* 
* \section SectionLPM_MoreInfo More Information
* For more information on the LPM peripheral, refer to:<br>
* <a href="http://www.cypress.com/file/223051/download">FM0+ Peripheral Manual - Core Subsystem TRM.pdf</a><br>
* <a href="http://www.cypress.com/file/222996/download">FM4 Peripheral Manual - Core Subsystem TRM.pdf</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/**
* \addtogroup GroupLPM_Macros
* \{
*/

/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')      
 ******************************************************************************/
#if (PDL_PERIPHERAL_VBAT_AVAILABLE == PDL_ON)   
#define PDL_BACK_UP_REGISTERS       (32u)   
#else
#define PDL_BACK_UP_REGISTERS       (16u)     
#endif   
   
#define WIFSR_WRTCI                 (1ul<<0u)
#define WIFSR_WLVDI                 (1ul<<1u)
#define WIFSR_WUI0                  (1ul<<2u)
#define WIFSR_WUI1                  (1ul<<3u)
#define WIFSR_WUI2                  (1ul<<4u)
#define WIFSR_WUI3                  (1ul<<5u)
#define WIFSR_WUI4                  (1ul<<6u)
#define WIFSR_WUI5                  (1ul<<7u)
#define WIFSR_WCEC0I                (1ul<<8u)
#define WIFSR_WCEC1I                (1ul<<9u)
#define WIFSR_WUI6                  (1ul<<10u)
#define WIFSR_WUI7                  (1ul<<11u)
#define WIFSR_WUI8                  (1ul<<12u)
#define WIFSR_WUI9                  (1ul<<13u)
#define WIFSR_WUI10                 (1ul<<14u)
#define WIFSR_WUI11                 (1ul<<15u)
  
#define WRFSR_WINITX                 (1ul<<0u)
#define WRFSR_WLVDH                  (1ul<<1u)  
    
/** \} GroupLPM_Macros */

/**
* \addtogroup GroupLPM_Types
* \{
*/

/**
 ******************************************************************************
 ** \brief define standby mode type
 ******************************************************************************/
typedef enum en_lpm_mode
{
    StbSleepMode    = 0u,    //!< standby sleep mode
    StbTimerMode    = 1u,    //!< standby timer mode    
    StbStopMode     = 2u,    //!< standby stop mode     
    StbRtcMode      = 3u,    //!< standby RTC mode 
    DeepStbRtcMode  = 4u,    //!< deep standby RTC mode
    DeepStbStopMode = 5u,    //!< deep standby stop mode      
    
} en_lpm_mode_t;

/**
 ******************************************************************************
 ** \brief define deep standby mode return cause flag
 ******************************************************************************/
typedef enum en_ret_cause_flag
{
    DeepStbNoFlag    = 0u,     ///< No flag set
    DeepStbInitx     = 1u,     ///< return from INITX input reset
    DeepStbLvdReset  = 2u,     ///< return from LVD reset
    DeepStbRtcInt    = 3u,     ///< return from RTC interrupt
    DeepStbLvdInt    = 4u,     ///< return from LVD interrupt
    DeepStbWkupPin0  = 5u,     ///< return from Wkup pin1 detection
    DeepStbWkupPin1  = 6u,     ///< return from Wkup pin1 detection
    DeepStbWkupPin2  = 7u,     ///< return from Wkup pin2 detection
    DeepStbWkupPin3  = 8u,     ///< return from Wkup pin3 detection
    DeepStbWkupPin4  = 9u,     ///< return from Wkup pin4 detection
    DeepStbWkupPin5  = 10u,    ///< return from Wkup pin5 detection
    DeepStbCec0      = 11u,    ///< return from Wkup CEC1 reception interrupt
    DeepStbCec1      = 12u,    ///< return from Wkup CEC2 reception interrupt
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)    
    DeepStbWkupPin6  = 13u,    ///< return from Wkup pin6 detection
    DeepStbWkupPin7  = 14u,    ///< return from Wkup pin7 detection
    DeepStbWkupPin8  = 15u,    ///< return from Wkup pin8 detection
    DeepStbWkupPin9  = 16u,    ///< return from Wkup pin9 detection
    DeepStbWkupPin10  = 17u,   ///< return from Wkup pin10 detection
    DeepStbWkupPin11  = 18u,   ///< return from Wkup pin11 detection
#endif    
} en_dstb_ret_cause_t;

/**
******************************************************************************
 ** \brief define wkup pin index
******************************************************************************/
typedef enum en_dstb_wkup_pin
{
    WkupPin1 = 0u,      ///< index of wkup pin 1
    WkupPin2 = 1u,      ///< index of wkup pin 2
    WkupPin3 = 2u,      ///< index of wkup pin 3
    WkupPin4 = 3u,      ///< index of wkup pin 4
    WkupPin5 = 4u,      ///< index of wkup pin 5
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)    
    WkupPin6 = 5u,      ///< index of wkup pin 6
    WkupPin7 = 6u,      ///< index of wkup pin 7
    WkupPin8 = 7u,      ///< index of wkup pin 8
    WkupPin9 = 8u,      ///< index of wkup pin 9
    WkupPin10 = 9u,     ///< index of wkup pin 10
    WkupPin11 = 10u,    ///< index of wkup pin 11   
#endif    
    WkupPinMax,
} en_dstb_wkup_pin_t;

/**
 ******************************************************************************
 ** \brief Valid level of wakeup pin
 ******************************************************************************/
typedef enum en_wkup_valid_level
{
    WkupLowLevelValid   = 0u,  ///< Set low level as valid level
    WkupHighLevelValid  = 1u,  ///< Set high level as valid level
    
}en_wkup_valid_level_t;

/**
 ******************************************************************************
 ** \brief Internal voltage selection in deep standby mode
 ******************************************************************************/
typedef enum en_lpm_internal_voltage
{
    LpmInternalVoltage120 = 0u, ///< Internal voltage is 1.20v in deepstandby mode. 
    LpmInternalVoltage105 = 1u, ///< Internal voltage is 1.05v in deepstandby mode. 
    LpmInternalVoltage110 = 2u, ///< Internal voltage is 1.10v in deepstandby mode. 
    
}en_lpm_internal_voltage_t;

/**
 ******************************************************************************
 ** \brief Main oscillator selection types
 ******************************************************************************/
typedef enum en_lpm_main_osc
{
    LpmMainOsc4M         = 0u, ///< Support 4MHz main oscillator in deep standby mode, has the lowest power consumption
    LpmMainOsc4M8M       = 1u, ///< Support 4MHz and 8MHz main oscillator in deep standby mode, has the middle power consumption
    LpmMainOsc4M8M16M    = 2u, ///< Support 4MHz, 8MHz, and 16MHz main oscillator in deep standby mode, has higher power consumption
    LpmMainOsc48M        = 3u, ///< Support 48MHz main oscillator in deep standby mode, has the highest power consumption
    
}en_lpm_main_osc_t;

/**
 ******************************************************************************
 ** \brief backup register index
 *******************************************************************************/
typedef enum en_dstb_bakup_reg
{
    BackupReg1 = 1u,      ///< index of backup register 1
    BackupReg2 = 2u,      ///< index of backup register 2
    BackupReg3 = 3u,      ///< index of backup register 3
    BackupReg4 = 4u,      ///< index of backup register 4
    BackupReg5 = 5u,      ///< index of backup register 5
    BackupReg6 = 6u,      ///< index of backup register 6
    BackupReg7 = 7u,      ///< index of backup register 7
    BackupReg8 = 8u,      ///< index of backup register 8
    BackupReg9 = 9u,      ///< index of backup register 9
    BackupReg10 = 10u,    ///< index of backup register 10
    BackupReg11 = 11u,    ///< index of backup register 11
    BackupReg12 = 12u,    ///< index of backup register 12
    BackupReg13 = 13u,    ///< index of backup register 13
    BackupReg14 = 14u,    ///< index of backup register 14
    BackupReg15 = 15u,    ///< index of backup register 15
    BackupReg16 = 16u,    ///< index of backup register 16
    
} en_dstb_bakup_reg_t;

/** \}GroupLPM_Types */

/**
* \addtogroup GroupLPM_DataStructures
* \{
*/

/**
 ******************************************************************************
 ** \brief Deep standby mode return cause
 *******************************************************************************/
typedef struct stc_ret_cause
{
    boolean_t   bRtcEn;     //!< Return from RTC interrupt enable
    boolean_t   bLvdEn;     //!< Return from LVD interrupt enable
    boolean_t   bWakeup1En; //!< Return from Wakeup1 pin enable
    boolean_t   bWakeup2En; //!< Return from Wakeup2 pin enable
    boolean_t   bWakeup3En; //!< Return from Wakeup3 pin enable
    boolean_t   bWakeup4En; //!< Return from Wakeup4 pin enable
    boolean_t   bWakeup5En; //!< Return from Wakeup5 pin enable
    boolean_t   bCec0En;    //!< Return from CEC0 interrupt enable
    boolean_t   bCec1En;    //!< Return from CEC1 interrupt enable
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)        
    boolean_t   bWakeup6En; //!< Return from Wakeup6 pin enable
    boolean_t   bWakeup7En; //!< Return from Wakeup7 pin enable
    boolean_t   bWakeup8En; //!< Return from Wakeup8 pin enable
    boolean_t   bWakeup9En; //!< Return from Wakeup9 pin enable
    boolean_t   bWakeup10En; //!< Return from Wakeup10 pin enable
    boolean_t   bWakeup11En; //!< Return from Wakeup10 pin enable
#endif    
}stc_dstb_ret_cause_t;

/**
 ******************************************************************************
 ** \brief backup register index
 *******************************************************************************/
typedef struct stc_backupreg
{
    uint8_t u8BREG00;
    uint8_t u8BREG01;
    uint8_t u8BREG02;
    uint8_t u8BREG03;
    uint8_t u8BREG04;
    uint8_t u8BREG05;
    uint8_t u8BREG06;
    uint8_t u8BREG07;
    uint8_t u8BREG08;
    uint8_t u8BREG09;
    uint8_t u8BREG0A;
    uint8_t u8BREG0B;
    uint8_t u8BREG0C;
    uint8_t u8BREG0D;
    uint8_t u8BREG0E;
    uint8_t u8BREG0F;
    uint8_t u8BREG10;
    uint8_t u8BREG11;
    uint8_t u8BREG12;
    uint8_t u8BREG13;
    uint8_t u8BREG14;
    uint8_t u8BREG15;
    uint8_t u8BREG16;
    uint8_t u8BREG17;
    uint8_t u8BREG18;
    uint8_t u8BREG19;
    uint8_t u8BREG1A;
    uint8_t u8BREG1B;
    uint8_t u8BREG1C;
    uint8_t u8BREG1D;
    uint8_t u8BREG1E;
    uint8_t u8BREG1F;
} stc_backupreg_t;

/** \} GroupLPM_DataStructures */

/**
* \addtogroup GroupLPM_Functions
* \{
*/

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/

/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/
void Lpm_GoToStandByMode(en_lpm_mode_t enMode, boolean_t bIoRemain);
en_result_t Lpm_ConfigDeepStbRetCause(stc_dstb_ret_cause_t* pstcCause);
en_dstb_ret_cause_t Lpm_ReadDeepStbRetCause(void);
en_result_t Lpm_SetWkupPinLevel(en_dstb_wkup_pin_t enPinIndex, en_wkup_valid_level_t enLevel);
en_result_t Lpm_ConfigSubClk(boolean_t bSupplyCec, boolean_t bSupplyRtc);
en_result_t Lpm_ConfigDeepStbRAMRetention(boolean_t bRamRetain);
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)
en_result_t Lpm_ConfigInternalVoltage(en_lpm_internal_voltage_t enVoltage);
en_result_t Lpm_SelMainOscTypes(en_lpm_main_osc_t enMainOsc);
en_result_t Lpm_ConfigFlashPower(boolean_t bPowerOn);
#endif
en_result_t Lpm_Readu8DataBackupRegister(uint8_t u8AddressOffset, uint8_t* u8Data) ;
en_result_t Lpm_Writeu8DataBackupRegister(uint8_t u8AddressOffset, uint8_t u8Data) ;
en_result_t Lpm_Readu16DataBackupRegister(uint8_t u8AddressOffset, uint16_t* u16Data) ;
en_result_t Lpm_Writeu16DataBackupRegister(uint8_t u8AddressOffset, uint16_t u16Data) ;
en_result_t Lpm_Readu32DataBackupRegister(uint8_t u8AddressOffset, uint32_t* u32Data) ;
en_result_t Lpm_Writeu32DataBackupRegister(uint8_t u8AddressOffset, uint32_t u32Data) ;

/** \} GroupLPM_Functions */
/** \} GroupLPM */

#ifdef __cplusplus
}
#endif

#endif // #if (defined(PDL_PERIPHERAL_LPM_ACTIVE))

#endif /* __LPM_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
