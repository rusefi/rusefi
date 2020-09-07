/*******************************************************************************
* \file             lpm.c
* 
* \version          1.30
*
* \brief            This file provides the source code to the API for the LPM 
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
#include "lpm/lpm.h"

#if (defined(PDL_PERIPHERAL_LPM_ACTIVE))

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
// SCR address
#define CM_CORE_CSR  *((volatile unsigned int*)(0xE000ED10UL))
   
// Backup registers base address   
#define LPCM_BACKUP_REG_BASE   ((volatile uint8_t*)0x40035900)
   
/*---------------------------------------------------------------------------*/
/* local datatypes                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* global data                                                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* global functions                                                          */
/*---------------------------------------------------------------------------*/

/**
 ******************************************************************************
 ** \brief Read Backup Registers
 **
 ** \param [out]  stcBackUpReg     double pointer to user backup register
 **                                structure
 **
 ** \retval Ok                     Successfully read
 ******************************************************************************/ 
static en_result_t ReadBackupRegisters(stc_backupreg_t* stcBackUpReg)
{
  uint8_t  u8Counter;
  
#if (PDL_PERIPHERAL_VBAT_AVAILABLE == PDL_ON)  
  // Read Command to VBAT domain
  FM_RTC->WTCR20_f.BREAD = 1u;
  
  // Poll for read completed
  while(1 == FM_RTC->WTCR20_f.BREAD)
  {}
#endif
 
  for(u8Counter = 0u; u8Counter < PDL_BACK_UP_REGISTERS; u8Counter += 4u)
  {
#if (PDL_PERIPHERAL_VBAT_AVAILABLE == PDL_ON)      
    *(uint32_t*)((uint32_t)(&stcBackUpReg->u8BREG00) + (uint32_t)u8Counter)
      = *(uint32_t*)((uint32_t)(&FM_RTC->BREG00_03) + u8Counter);
#else
    *(uint32_t*)((uint32_t)(&stcBackUpReg->u8BREG00) + (uint32_t)u8Counter)
      = *(uint32_t*)((uint32_t)(&FM_DS->BUR01) + u8Counter);
#endif    
  }
  
  return Ok;
} // ReadBackupRegisters

/**
 ******************************************************************************
 ** \brief Write Backup Registers
 **
 ** \param [out]  stcBackUpReg     structure of backup registers
 **
 ** \retval Ok                     Successfully written
 ******************************************************************************/ 
static en_result_t WriteBackupRegisters(stc_backupreg_t* stcBackUpReg)
{
  uint8_t u8Counter;

  for(u8Counter = 0u; u8Counter < PDL_BACK_UP_REGISTERS; u8Counter += 4u)
  {
#if (PDL_PERIPHERAL_VBAT_AVAILABLE == PDL_ON)      
    *(uint32_t*)((uint32_t)(&FM_RTC->BREG00_03) + u8Counter)
      = *(uint32_t*)((uint32_t)(&stcBackUpReg->u8BREG00) + u8Counter);
#else
    *(uint32_t*)((uint32_t)(&FM_DS->BUR01) + u8Counter)
      = *(uint32_t*)((uint32_t)(&stcBackUpReg->u8BREG00) + u8Counter);
#endif    
  }

#if (PDL_PERIPHERAL_VBAT_AVAILABLE == PDL_ON) 
  // Write Command to VBAT domain
  FM_RTC->WTCR20_f.BWRITE = 1u;
  
  // Poll for read completed
  while(1 == FM_RTC->WTCR20_f.BWRITE)
  {}
#endif

  return Ok;
} // WriteBackupRegisters     


/**
 ******************************************************************************
 ** \brief WRFSR dummy read
 ******************************************************************************/
static uint16_t WRFSRDummyRead(void)
{
    uint16_t Dummy;
    Dummy = FM_DS->WRFSR;
    return Dummy;
}

/**
 ******************************************************************************
 ** \brief WRFSR dummy read
 ******************************************************************************/
static uint16_t WIFSRDummyRead(void)
{
    uint16_t Dummy;
    Dummy = FM_DS->WIFSR;
    return Dummy;
}


/**
 ******************************************************************************
 ** \brief go to standby mode
 **
 ** \param enMode the type of standby mode.
 **        This parameter can be one of the following values:
 ** \arg   StbSleepMode    Sleep mode
 ** \arg   StbTimerMode    Timer mode
 ** \arg   StbStopMode     Stop mode
 ** \arg   StbRtcMode      RTC mode
 ** \arg   DeepStbRtcMode  deep standby RTC mode
 ** \arg   DeepStbStopMode deep standby stop mode

 ** \param bIoRemain Set io status when enter standby mode.
 **        This parameter can be one of the following values:
 ** \arg   FALSE Set IO to Hi-z
 ** \arg   TRUE  Remain IO status
 ******************************************************************************/
void Lpm_GoToStandByMode(en_lpm_mode_t enMode, boolean_t bIoRemain)
{
    WRFSRDummyRead();
    WIFSRDummyRead();
    switch (enMode)
    {
        case StbSleepMode:
            CM_CORE_CSR &= 0xFFFFFFFFBu;
            __WFI();
            break;
        case StbTimerMode:
            if( bIoRemain == 1u )
            {
                FM_CRG->STB_CTL = 0x1ACC0000u; // Retains status of each pin
            }
            else
            {
                FM_CRG->STB_CTL = 0x1ACC0010u; // Sets the status of each pin to high impedance
            }
            CM_CORE_CSR |= 0x00000004u;
            __WFI();
            break;
        case StbStopMode:
            FM_DS->PMD_CTL &= ~0x01u; /* RTCE=0 */
            if( bIoRemain == 1u )
            {
                FM_CRG->STB_CTL = 0x1ACC0002u; // Retains status of each pin
            }
            else
            {
                FM_CRG->STB_CTL = 0x1ACC0012u; // Sets the status of each pin to high impedance
            }
            CM_CORE_CSR |= 0x00000004u;
            __WFI();
            break;    
        case StbRtcMode:
            FM_DS->PMD_CTL |= 0x01u; /* RTCE=1 */
            if( bIoRemain == 1u )
            {
                FM_CRG->STB_CTL = 0x1ACC0002u;  // Retains status of each pin
            }
            else
            {
                FM_CRG->STB_CTL = 0x1ACC0012u;  // Sets the status of each pin to high impedance
            }
            CM_CORE_CSR |= 0x00000004u;
            __WFI();
            break;
        case DeepStbRtcMode: 
            FM_DS->PMD_CTL |= 0x01u; /* RTCE=1 */
            if( bIoRemain == 1u )
            {
                FM_CRG->STB_CTL = 0x1ACC0006u;  // Retains status of each pin
            }
            else
            {
                FM_CRG->STB_CTL = 0x1ACC0016u; // Sets the status of each pin to high impedance
            }
            CM_CORE_CSR |= 0x00000004u;
            __WFI();
            break;
        case DeepStbStopMode:
            FM_DS->PMD_CTL &= ~0x01u; /* RTCE=0 */
            if( bIoRemain == 1u )
            {
                FM_CRG->STB_CTL = 0x1ACC0006u;  // Retains status of each pin
            }
            else
            {
                FM_CRG->STB_CTL = 0x1ACC0016u; // Sets the status of each pin to high impedance
            }
            CM_CORE_CSR |= 0x00000004u;
            __WFI();
            break;                       
        default:
            break;
    }
}

/**
 ******************************************************************************
 ** \brief Configure return cause from deep standby mode
 **
 ** \param pstcCause Pointer to structure of return cause types
 **         
 ** \retval Ok  Deep standby mode is configured normally
 **
 ** \note Wakeup from WKUP0 pin is always enabled
 **
 ******************************************************************************/
en_result_t Lpm_ConfigDeepStbRetCause(stc_dstb_ret_cause_t* pstcCause)
{
    if(NULL == pstcCause)
    {
        return ErrorInvalidParameter;
    }
  
    bFM_DS_WIER_WRTCE = ((pstcCause->bRtcEn == TRUE) ? 1u : 0u);
    bFM_DS_WIER_WLVDE = ((pstcCause->bLvdEn == TRUE)? 1u : 0u);
    bFM_DS_WIER_WUI1E = ((pstcCause->bWakeup1En == TRUE) ? 1u : 0u);
    bFM_DS_WIER_WUI2E = ((pstcCause->bWakeup2En == TRUE) ? 1u : 0u);
    bFM_DS_WIER_WUI3E = ((pstcCause->bWakeup3En == TRUE) ? 1u : 0u);
    bFM_DS_WIER_WUI4E = ((pstcCause->bWakeup4En == TRUE) ? 1u : 0u);
    bFM_DS_WIER_WUI5E = ((pstcCause->bWakeup5En == TRUE) ? 1u : 0u);
#if (defined(FM_HDMICEC_AVAILABLE))
    bFM_DS_WIER_WCEC0E = ((pstcCause->bCec0En == TRUE) ? 1u : 0u);
    bFM_DS_WIER_WCEC1E = ((pstcCause->bCec1En == TRUE) ? 1u : 0u);
#endif
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)          
    bFM_DS_WIER_WUI6E = ((pstcCause->bWakeup6En == TRUE) ? 1u : 0u);
    bFM_DS_WIER_WUI7E = ((pstcCause->bWakeup7En == TRUE) ? 1u : 0u);
    bFM_DS_WIER_WUI8E = ((pstcCause->bWakeup8En == TRUE) ? 1u : 0u);
    bFM_DS_WIER_WUI9E = ((pstcCause->bWakeup9En == TRUE) ? 1u : 0u);
    bFM_DS_WIER_WUI10E = ((pstcCause->bWakeup10En == TRUE) ? 1u : 0u);
    bFM_DS_WIER_WUI11E = ((pstcCause->bWakeup11En == TRUE) ? 1u : 0u);
#endif    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Read deep standby mode return cause flag 
 **         
 ** \retval DeepStbNoFlag      No flag set
 ** \retval DeepStbInitx       Wakeup from deep standby mode by INITX reset
 ** \retval DeepStbLvdReset    Wakeup from deep standby mode by LVD reset
 ** \retval DeepStbRtcInt      Wakeup from deep standby mode by RTC interrupt
 ** \retval DeepStbLvdInt      Wakeup from deep standby mode by LVD interrupt
 ** \retval DeepStbWkupPin0    Wakeup from deep standby mode by WKUP0
 ** \retval DeepStbWkupPin1    Wakeup from deep standby mode by WKUP1
 ** \retval DeepStbWkupPin2    Wakeup from deep standby mode by WKUP2
 ** \retval DeepStbWkupPin3    Wakeup from deep standby mode by WKUP3
 ** \retval DeepStbWkupPin4    Wakeup from deep standby mode by WKUP4
 ** \retval DeepStbWkupPin5    Wakeup from deep standby mode by WKUP5
 ** \retval DeepStbCec0        Wakeup from deep standby mode by CEC0 interrupt
 ** \retval DeepStbCec1        Wakeup from deep standby mode by CEC1 interrupt
 ** \retval DeepStbWkupPin6    Wakeup from deep standby mode by WKUP6
 ** \retval DeepStbWkupPin7    Wakeup from deep standby mode by WKUP7
 ** \retval DeepStbWkupPin8    Wakeup from deep standby mode by WKUP8
 ** \retval DeepStbWkupPin9    Wakeup from deep standby mode by WKUP9
 ** \retval DeepStbWkupPin10   Wakeup from deep standby mode by WKUP10
 ** \retval DeepStbWkupPin11   Wakeup from deep standby mode by WKUP11
 ******************************************************************************/
en_dstb_ret_cause_t Lpm_ReadDeepStbRetCause(void)
{
    uint16_t rWIFSR,rWRFSR;
    rWRFSR = FM_DS->WRFSR;
    rWIFSR = FM_DS->WIFSR;
    
    if(rWRFSR & WRFSR_WINITX)
    {
        return DeepStbInitx;
    }
    
    if(rWRFSR & WIFSR_WLVDI)
    {
        return DeepStbLvdReset;
    }
    
    if(rWIFSR & WIFSR_WRTCI)
    {
        return DeepStbRtcInt;
    }
    else if(rWIFSR & WIFSR_WLVDI)
    {
        return DeepStbLvdInt;
    }
    else if(rWIFSR & WIFSR_WUI0)
    {
        return DeepStbWkupPin0;
    }
    else if(rWIFSR & WIFSR_WUI1)
    {
        return DeepStbWkupPin1;
    }
    else if(rWIFSR & WIFSR_WUI2)
    {
        return DeepStbWkupPin2;
    }
    else if(rWIFSR & WIFSR_WUI3)
    {
        return DeepStbWkupPin3;
    }
    else if(rWIFSR & WIFSR_WUI4)
    {
        return DeepStbWkupPin4;
    }
    else if(rWIFSR & WIFSR_WUI5)
    {
        return DeepStbWkupPin5;
    }
    else if(rWIFSR & WIFSR_WCEC0I)
    {
        return DeepStbCec0;
    }
    else if(rWIFSR & WIFSR_WCEC1I)
    {
        return DeepStbCec1;
    }
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)    
    else if(rWIFSR & WIFSR_WUI6)
    {
        return DeepStbWkupPin6;
    }
    else if(rWIFSR & WIFSR_WUI7)
    {
        return DeepStbWkupPin7;
    }
    else if(rWIFSR & WIFSR_WUI8)
    {
        return DeepStbWkupPin8;
    }
    else if(rWIFSR & WIFSR_WUI9)
    {
        return DeepStbWkupPin9;
    }
    else if(rWIFSR & WIFSR_WUI10)
    {
        return DeepStbWkupPin10;
    }    
    else if(rWIFSR & WIFSR_WUI11)
    {
        return DeepStbWkupPin11;
    }
#endif
    
    return DeepStbNoFlag;
}

/**
 ******************************************************************************
 ** \brief Set the valid level of wkup pin
 ** 
 ** \param enPinIndex Pin index
 ** \arg WkupPin1          WKUP1 pin index
 ** \arg WkupPin2          WKUP2 pin index
 ** \arg WkupPin3          WKUP3 pin index
 ** \arg WkupPin4          WKUP4 pin index
 ** \arg WkupPin5          WKUP5 pin index
 ** \arg WkupPin6          WKUP6 pin index
 ** \arg WkupPin7          WKUP7 pin index
 ** \arg WkupPin8          WKUP8 pin index
 ** \arg WkupPin9          WKUP9 pin index
 ** \arg WkupPin10         WKUP10 pin index
 ** \arg WkupPin11         WKUP11 pin index
 **
 ** \param enLevel Wakeup pin valid value
 ** \arg WkupLowLevelValid     Set low as active level for WKUPx
 ** \arg WkupHighLevelValid    Set high as active level for WKUPx
 **         
 ** \retval Ok                     The valid level of wkup pin is set normally
 ** \retval ErrorInvalidParameter  Invalid value of enPinIndex
 **
 ******************************************************************************/
en_result_t Lpm_SetWkupPinLevel(en_dstb_wkup_pin_t enPinIndex, en_wkup_valid_level_t enLevel)
{
    if(enPinIndex >= WkupPinMax)
    {
        return ErrorInvalidParameter;
    }
  
    FM_DS->WILVR &= ~(1ul << enPinIndex);
    FM_DS->WILVR |= ((uint8_t)enLevel << enPinIndex);
    
    return Ok;
}

#if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)
/**
 ******************************************************************************
 ** \brief Configure the internal voltage in deep standby mode
 ** 
 ** \param enVoltage Internal voltage
 ** \arg LpmInternalVoltage120   Internal voltage is 1.20v
 ** \arg LpmInternalVoltage110   Internal voltage is 1.10v
 ** \arg LpmInternalVoltage105   Internal voltage is 1.05v
 **         
 ** \retval Ok                     Internal volatge is set normally
 ** \retval ErrorInvalidParameter  Invalid value of enVoltage
 **
 ** \note This setting is active during deep standby only.
 **
 ******************************************************************************/
en_result_t Lpm_ConfigInternalVoltage(en_lpm_internal_voltage_t enVoltage)
{
    switch (enVoltage)
    {
        case LpmInternalVoltage120:
            FM_DS->REG_CTL2_f.TT_SEL = 0u;
            break;
        case LpmInternalVoltage105:
            FM_DS->REG_CTL2_f.TT_SEL = 1u;
            FM_DS->REG_CTL2_f.TT_SVD = 1u;
            break;
        case LpmInternalVoltage110:
            FM_DS->REG_CTL2_f.TT_SEL = 1u;
            FM_DS->REG_CTL2_f.TT_SVD = 2u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Configure main oscillator types in deep standby mode
 ** 
 ** \param enMainOsc Main oscillator types
 ** \arg LpmMainOsc4M        Support 4MHz main osc
 ** \arg LpmMainOsc4M8M      Support 4MHz,8MHz main osc
 ** \arg LpmMainOsc4M8M16M   Support 4MHz,8MHz,16MHz main osc
 ** \arg LpmMainOsc48M       Support 48MHz main osc
 **         
 ** \retval Ok                     Main oscillator types is set normally
 ** \retval ErrorInvalidParameter  Invalid value of enMainOsc
 **
 ** \note This setting is active during deep standby only.
 **
 ******************************************************************************/
en_result_t Lpm_SelMainOscTypes(en_lpm_main_osc_t enMainOsc)
{
    switch (enMainOsc)
    {
        case LpmMainOsc4M:
            FM_DS->MOSC_CTL_f.IMAINSEL = 0u;
            break;
        case LpmMainOsc4M8M:
            FM_DS->MOSC_CTL_f.IMAINSEL = 1u;
            break;
        case LpmMainOsc4M8M16M:
            FM_DS->MOSC_CTL_f.IMAINSEL = 2u;
            break;
        case LpmMainOsc48M:
            FM_DS->MOSC_CTL_f.IMAINSEL = 3u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Configure Flash power in deep standby mode
 ** 
 ** \param bPowerOn  Flash power on or not
 ** \arg TRUE        Flash power on
 ** \arg FALSE       Flash power off
 **         
 ** \retval Ok                     Main oscillator types is set normally
 ** \retval ErrorInvalidParameter  Invalid value of enMainOsc
 **
 ** \note This setting is active during stop/RTC/Sub Timer mode
 **
 ******************************************************************************/
en_result_t Lpm_ConfigFlashPower(boolean_t bPowerOn)
{
    FM_DS->STBFLASHPDX_f.STBFLASHPDX = ((bPowerOn == TRUE) ? 1u : 0u);
    
    return Ok;
}
#endif

/**
 ******************************************************************************
 ** \brief Configue the sub clock supply 
 ** 
 ** \param bSupplyCec Whether to supply sub clock to CEC
 ** \param bSupplyRtc Whether to supply sub clock to RTC
 **         
 ** \retval Ok                  The sub clock supply is configured normally
 ** \retval ErrorInvalidMode    The device does not support this function
 **
 ** \note Not all FM product has this function, please check it in the 
 **       product manual.
 **
 ******************************************************************************/
en_result_t Lpm_ConfigSubClk(boolean_t bSupplyCec, boolean_t bSupplyRtc)
{
#if defined(bFM_DS_RCK_CTL_CECCKE)
    if(TRUE == bSupplyCec)
    {
        bFM_DS_RCK_CTL_CECCKE = 1u;
    }
    else
    {
        bFM_DS_RCK_CTL_CECCKE = 0u;
    }
    
    if(TRUE == bSupplyRtc)
    {
        bFM_DS_RCK_CTL_RTCCKE = 1u;
    }
    else
    {
        bFM_DS_RCK_CTL_RTCCKE = 0u;
    }

    return Ok;
#else
    return ErrorInvalidMode;
#endif
}

/**
 ******************************************************************************
 ** \brief Configure deep standby mode RAM retention
 ** 
 ** \param bRamRetain Whether to retain data in the RAM
 **         
 ** \retval Ok Deep standby mode RAM retention set
 **
 ** \note Not all FM0+ product has this function, please check it in the 
 **       product manual.
 **
 ******************************************************************************/
en_result_t Lpm_ConfigDeepStbRAMRetention(boolean_t bRamRetain)
{
    if(FALSE == bRamRetain)
    {
        FM_DS->DSRAMR &= ~0x03u; 
    }
    else
    {
        FM_DS->DSRAMR |= 0x03u; 
    }
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Write the backup register
 ** 
 ** \param enRegIndex register index
 ** \arg BackupReg1     Back up register 1 index
 ** \arg BackupReg2     Back up register 2 index
 ** \arg BackupReg3     Back up register 3 index
 ** \arg BackupReg4     Back up register 4 index
 ** \arg BackupReg5     Back up register 5 index
 ** \arg BackupReg6     Back up register 6 index
 ** \arg BackupReg7     Back up register 7 index
 ** \arg BackupReg8     Back up register 8 index
 ** \arg BackupReg9     Back up register 9 index
 ** \arg BackupReg10    Back up register 10 index
 ** \arg BackupReg11    Back up register 11 index
 ** \arg BackupReg12    Back up register 12 index
 ** \arg BackupReg13    Back up register 13 index
 ** \arg BackupReg14    Back up register 14 index
 ** \arg BackupReg15    Back up register 15 index
 ** \arg BackupReg16    Back up register 16 index
 ** \param u8Data the data written into backup register
 **         
 ** \retval Ok The backup register written normally
 ** \retval ErrorInvalidParameter enRegIndex > BackupReg15
 **
 ******************************************************************************/
en_result_t Lpm_WriteBackupReg(en_dstb_bakup_reg_t enRegIndex, uint8_t u8Data)
{
    if(enRegIndex > BackupReg15)
    {
        return ErrorInvalidParameter;
    }
  
    *(uint8_t*)(LPCM_BACKUP_REG_BASE + (uint8_t)enRegIndex) = u8Data;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Read the backup register
 ** 
 ** \param enRegIndex Backup register index
 ** \arg BackupReg1     Back up register 1 index
 ** \arg BackupReg2     Back up register 2 index
 ** \arg BackupReg3     Back up register 3 index
 ** \arg BackupReg4     Back up register 4 index
 ** \arg BackupReg5     Back up register 5 index
 ** \arg BackupReg6     Back up register 6 index
 ** \arg BackupReg7     Back up register 7 index
 ** \arg BackupReg8     Back up register 8 index
 ** \arg BackupReg9     Back up register 9 index
 ** \arg BackupReg10    Back up register 10 index
 ** \arg BackupReg11    Back up register 11 index
 ** \arg BackupReg12    Back up register 12 index
 ** \arg BackupReg13    Back up register 13 index
 ** \arg BackupReg14    Back up register 14 index
 ** \arg BackupReg15    Back up register 15 index
 ** \arg BackupReg16    Back up register 16 index
 **         
 ** \return Backup register data
 **
 ******************************************************************************/
uint8_t Lpm_ReadBackupReg(en_dstb_bakup_reg_t enRegIndex)
{
    return *(uint8_t*)(LPCM_BACKUP_REG_BASE + (uint8_t)enRegIndex);
}

/**
 ******************************************************************************
 ** \brief Read Backup Register (Byte)
 **
 ** \param [in]   u8AddressOffset  Byte address offset to Back-up register
 ** \param [out]  u8Data           Pointer to Byte data to be read
 **
 ** \retval Ok                     Successfully written
 ** \retval ErrorInvalidParameter  Address offset out of range
 ******************************************************************************/ 
en_result_t Lpm_Readu8DataBackupRegister(uint8_t u8AddressOffset, uint8_t* u8Data)
{
  stc_backupreg_t stcBackUpReg;
  
  if (u8AddressOffset >= PDL_BACK_UP_REGISTERS)
  {
    return ErrorInvalidParameter;
  }
  
  ReadBackupRegisters(&stcBackUpReg);
  
  *u8Data =  *(uint8_t*)((uint32_t)(&stcBackUpReg.u8BREG00) + (uint32_t)u8AddressOffset);
  
  return Ok;
} // Lpm_Readu8DataBackupRegister

/**
 ******************************************************************************
 ** \brief Write Backup Registers (Byte)
 **
 ** \param [in]   u8AddressOffset  Byte address offset to Back-up register
 ** \param [in]   u8Data           Byte data to be written
 **
 ** \retval Ok                     Successfully written
 ** \retval ErrorInvalidParameter  Address offset out of range
 ******************************************************************************/ 
en_result_t Lpm_Writeu8DataBackupRegister(uint8_t u8AddressOffset, uint8_t u8Data)
{
  stc_backupreg_t stcBackUpReg;
  
  if (u8AddressOffset >= PDL_BACK_UP_REGISTERS)
  {
    return ErrorInvalidParameter;
  }
  
  ReadBackupRegisters(&stcBackUpReg);
  *(uint8_t*)((uint32_t)(&stcBackUpReg.u8BREG00) + (uint32_t)u8AddressOffset) = u8Data;
  WriteBackupRegisters(&stcBackUpReg);
  
  return Ok;
} // Lpm_Writeu8DataBackupRegister

/**
 ******************************************************************************
 ** \brief Read Backup Register (16-Bit)
 **
 ** \param [in]   u8AddressOffset  Byte address offset to Back-up register
 ** \param [out]  u16Data          Pointer to 16-Bit data to be read
 **
 ** \retval Ok                     Successfully written
 ** \retval ErrorInvalidParameter  Address offset out of range (highest priority)
 ** \retval ErrorAddressAlignment  Address not aligned to 16-bit
 ******************************************************************************/ 
en_result_t Lpm_Readu16DataBackupRegister (uint8_t u8AddressOffset, uint16_t* u16Data)
{
  stc_backupreg_t stcBackUpReg;
  
  if (u8AddressOffset >= PDL_BACK_UP_REGISTERS)
  {
    return ErrorInvalidParameter;
  }
  
  if (0u != (u8AddressOffset % 2u))
  {
    return ErrorAddressAlignment;
  }
  
  ReadBackupRegisters(&stcBackUpReg);
  
  *u16Data =  *(uint16_t*)((uint32_t)(&stcBackUpReg.u8BREG00) + (uint32_t)u8AddressOffset);
  
  return Ok;
} // Lpm_Readu16DataBackupRegister

/**
 ******************************************************************************
 ** \brief Write Backup Registers (16-Bit)
 **
 ** \param [in]   u8AddressOffset  Byte address offset to Back-up register
 ** \param [in]   u16Data          16-Bit data to be written
 **
 ** \retval Ok                     Successfully written
 ** \retval ErrorInvalidParameter  Address offset out of range (highest priority)
 ** \retval ErrorAddressAlignment  Address not aligned to 16-bit
 ******************************************************************************/ 
en_result_t Lpm_Writeu16DataBackupRegister(uint8_t u8AddressOffset, uint16_t u16Data)
{
  stc_backupreg_t stcBackUpReg;
  
  if (u8AddressOffset >= PDL_BACK_UP_REGISTERS)
  {
    return ErrorInvalidParameter;
  }
  
  if (0u != (u8AddressOffset % 2u))
  {
    return ErrorAddressAlignment;
  }
  
  ReadBackupRegisters(&stcBackUpReg);
  *(uint16_t*)((uint32_t)(&stcBackUpReg.u8BREG00) + (uint32_t)u8AddressOffset) = u16Data;
  WriteBackupRegisters(&stcBackUpReg);
  
  return Ok;
} // Lpm_Writeu16DataBackupRegister

/**
 ******************************************************************************
 ** \brief Read Backup Register (32-Bit)
 **
 ** \param [in]   u8AddressOffset  Byte address offset to Back-up register
 ** \param [out]  u32Data          Pointer to 32-Bit data to be read
 **
 ** \retval Ok                     Successfully written
 ** \retval ErrorInvalidParameter  Address offset out of range (highest priority)
 ** \retval ErrorAddressAlignment  Address not aligned to 32-bit
 ******************************************************************************/ 
en_result_t Lpm_Readu32DataBackupRegister (uint8_t u8AddressOffset, uint32_t* u32Data)
{
  stc_backupreg_t stcBackUpReg;
  
  if (u8AddressOffset >= PDL_BACK_UP_REGISTERS)
  {
    return ErrorInvalidParameter;
  }
  
  if (0u != (u8AddressOffset % 4u))
  {
    return ErrorAddressAlignment;
  }
  
  ReadBackupRegisters(&stcBackUpReg);
  
  *u32Data =  *(uint32_t*)((uint32_t)(&stcBackUpReg.u8BREG00) + (uint32_t)u8AddressOffset);
  
  return Ok;
} // Lpm_Readu32DataBackupRegister

/**
 ******************************************************************************
 ** \brief Write Backup Registers (32-Bit)
 **
 ** \param [in]   u8AddressOffset  Byte address offset to Back-up register
 ** \param [in]   u32Data          Pointer to 32-Bit data
 **
 ** \retval Ok                     Successfully written
 ** \retval ErrorInvalidParameter  Address offset out of range (highest priority)
 ** \retval ErrorAddressAlignment  Address not aligned to 32-bit
 ******************************************************************************/ 
en_result_t Lpm_Writeu32DataBackupRegister(uint8_t u8AddressOffset, uint32_t u32Data)
{
  stc_backupreg_t stcBackUpReg;
  
  if (u8AddressOffset >= PDL_BACK_UP_REGISTERS)
  {
    return ErrorInvalidParameter;
  }
  
  if (0u != (u8AddressOffset % 4))
  {
    return ErrorAddressAlignment;
  }
  
  ReadBackupRegisters(&stcBackUpReg);
  *(uint32_t*)((uint32_t)(&stcBackUpReg.u8BREG00) + (uint32_t)u8AddressOffset) = u32Data;
  WriteBackupRegisters(&stcBackUpReg);
  
  return Ok;
} // Lpm_Writeu32DataBackupRegister

#endif // #if (defined(PDL_PERIPHERAL_LPM_ACTIVE))

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
