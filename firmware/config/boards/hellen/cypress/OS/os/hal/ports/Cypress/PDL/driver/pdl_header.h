/*******************************************************************************
* \file             pdl_header.h
* 
* \version          1.0
*                       
* \brief            Includes driver header files enabled by PDL_ON / PDL_OFF
*                   defines in pdl_user.h.
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

#ifndef __PDL_HEADER_H__
#define __PDL_HEADER_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "pdl_user.h"

/* C binding of definitions if building with C++ compiler                     */
#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
 * Peripheral driver header files inclusion
 ******************************************************************************/
// Include adc.h if ADC is active
#if defined(PDL_PERIPHERAL_ADC_ACTIVE)
    #include "adc/adc.h"
#endif

// Include adc.h if AES is active
#if defined(PDL_PERIPHERAL_AES_ACTIVE)
    #include "aes/aes.h"
#endif  
  
// Include bt.h if BT is active
#if defined(PDL_PERIPHERAL_BT_ACTIVE)
    #include "bt/bt.h"
#endif

// Include can.h if CAN is active  
#if defined(PDL_PERIPHERAL_CAN_ACTIVE)
    #include "can/can.h"
#endif  

// Include canfd.h if CANFD is active  
#if defined(PDL_PERIPHERAL_CANFD_ACTIVE)
    #include "can/can_pre.h"
    #include "can/canfd.h"
#endif  
  
// Include clk.h if clock is active
#if defined(PDL_PERIPHERAL_CLK_ACTIVE)
    #include "clk/clk.h"
#endif

// Include cr.h if CR is active
#if defined(PDL_PERIPHERAL_CR_ACTIVE)
    #include "cr/cr.h"
#endif

// Include crc.h if CRC is active
#if defined(PDL_PERIPHERAL_CRC_ACTIVE)
    #include "crc/crc.h"
#endif   

// Include csv.h if CSV is active
#if defined(PDL_PERIPHERAL_CSV_ACTIVE)
    #include "csv/csv.h"
#endif

// Activate code in dac.c if one or more are set to PDL_ON
#if defined(PDL_PERIPHERAL_DAC_ACTIVE)
    #include "dac/dac.h"
#endif
  
// Include dma.h if DMA is active
#if defined(PDL_PERIPHERAL_DMA_ACTIVE)
    #include "dma/dma.h"
#endif

// Include dstc.c if DSTC is active
#if defined(PDL_PERIPHERAL_DSTC_ACTIVE)
    #include "dstc/dstc.h"
#endif  
  
// Include dt.h if DT is active
#if defined(PDL_PERIPHERAL_DT_ACTIVE)
    #include "dt/dt.h"
#endif
  
// Include exint.h if external interrupt is active
#if defined(PDL_PERIPHERAL_EXINT_ACTIVE) || defined(PDL_PERIPHERAL_NMI_ACTIVE)
    #include "exint/exint.h"
#endif

// Include pcrc.h if programmable crc is active
#if defined(PDL_PERIPHERAL_PCRC_ACTIVE)
    #include "pcrc/pcrc.h"
#endif

// Activate code in extif.c if set to PDL_ON
#if defined(PDL_PERIPHERAL_EXTIF_ACTIVE)
    #include "extif/extif.h"
#endif  
  
// Include dualflash.h if Dual Flash is active
#if defined(PDL_PERIPHERAL_DUAL_FLASH_ACTIVE)
    #include "flash/dualflash.h"
#endif

// Include mainflash.h if Main Flash is active
#if defined(PDL_PERIPHERAL_MAIN_FLASH_ACTIVE)
    #include "flash/mainflash.h"
#endif  
  
// Include mainflash.h if Work Flash is active
#if defined(PDL_PERIPHERAL_WORK_FLASH_ACTIVE)
    #include "flash/workflash.h"
#endif   
  
// Include gpio.h and fgpio.h if GPIO is active
#if defined(PDL_PERIPHERAL_GPIO_ACTIVE)
    #include "gpio/gpio.h"
#if (PDL_MCU_CORE == PDL_FM0P_CORE)  
    #include "gpio/fgpio.h"
#endif
#endif

// Include hsspi.h if HSSPI is active
#if defined(PDL_PERIPHERAL_HSSPI_ACTIVE)
    #include "hsspi/hsspi.h"
#endif  

// Include i2cs.h if I2CS is active
#if defined(PDL_PERIPHERAL_I2CS_ACTIVE)
    #include "i2cs/i2cs.h"
#endif   
   
// Include i2s.h if I2S is active
#if defined(PDL_PERIPHERAL_I2S_ACTIVE)
    #include "i2s/i2s.h"
#endif

// Include i2sl.h if I2S is active
#if defined(PDL_PERIPHERAL_I2SL_ACTIVE)
    #include "i2sl/i2sl.h"
#endif

// Include icc.h if ICC is active
#if defined(PDL_PERIPHERAL_ICC_ACTIVE)
    #include "icc/icc.h"
#endif  
  
// Include lpm.h if LPM is active
#if defined(PDL_PERIPHERAL_LPM_ACTIVE)
    #include "lpm/lpm.h"
#endif

// Include lvd.h if LVD is active
#if defined(PDL_PERIPHERAL_LVD_ACTIVE)
    #include "lvd/lvd.h"
#endif

// Include mfs.h if MFS is active
#if defined(PDL_PERIPHERAL_MFS_ACTIVE)
    #include "mfs/mfs.h"
#endif

// Include mft_frt.h if MFT'FRT is active
#if defined(PDL_PERIPHERAL_MFT_FRT_ACTIVE)
    #include "mft/mft_frt.h"
#endif

// Include mft_ocu.h if MFT'OCU is active
#if defined(PDL_PERIPHERAL_MFT_OCU_ACTIVE)
    #include "mft/mft_ocu.h"
#endif

// Include mft_wfg.h if MFT'WFG is active
#if defined(PDL_PERIPHERAL_MFT_WFG_ACTIVE)
    #include "mft/mft_wfg.h"
#endif

// Include mft_icu.h if MFT'ICU is active
#if defined(PDL_PERIPHERAL_MFT_ICU_ACTIVE)
    #include "mft/mft_icu.h"
#endif

// Include mft_adcmp.h if MFT'ADCMP is active
#if defined(PDL_PERIPHERAL_MFT_ADCMP_ACTIVE)
    #include "mft/mft_adcmp.h"
#endif

// Include pcrc.h if PCRC is active
#if defined(PDL_PERIPHERAL_PCRC_ACTIVE)
    #include "pcrc/pcrc.h"
#endif  
  
// Include ppg.h if MFT'PPG is active
#if defined(PDL_PERIPHERAL_PPG_ACTIVE)
    #include "ppg/ppg.h"
#endif

// Include qprc.h if QPRC is active
#if defined(PDL_PERIPHERAL_QPRC_ACTIVE)
    #include "qprc/qprc.h"
#endif

// Include reset.h if reset is active
#if defined(PDL_PERIPHERAL_RESET_ACTIVE)
    #include "reset/reset.h"
#endif

// Include rc.h if RC is active
#if defined(PDL_PERIPHERAL_RC_ACTIVE)
    #include "rc/rc.h"
#endif

// Include rtc.h if RTC is active
#if defined(PDL_PERIPHERAL_RTC_ACTIVE)
    #include "rtc/rtc.h"
#endif

#if defined (PDL_PERIPHERAL_LCD_ACTIVE)
    #include "lcd/lcd.h"
#endif

// Activate code in sd.c is set to PDL_ON
#if defined (PDL_PERIPHERAL_SD_ACTIVE)
    #include "sdif/sdif.h"    
    #include "sd_card/sd_card.h"
    #include "sd_card/sd_cmd.h"  
#endif  

// Include uid.h if UID is active
#if defined(PDL_PERIPHERAL_UID_ACTIVE)
    #include "uid/uid.h"
#endif
  
  // Include vbat.h if WC is active
#if defined(PDL_PERIPHERAL_VBAT_ACTIVE)
    #include "vbat/vbat.h"
#endif
  
// Include wc.h if WC is active
#if defined(PDL_PERIPHERAL_WC_ACTIVE)
    #include "wc/wc.h"
#endif

// Include wdg.h if WDG is active
#if defined(PDL_PERIPHERAL_WDG_ACTIVE)
    #include "wdg/wdg.h"
#endif

#if (PDL_UTILITY_ENABLE_I2C_POLLING_AT24CXX == PDL_ON) || \
    (PDL_UTILITY_ENABLE_I2C_IRQ_AT24CXX == PDL_ON)
    #include "eeprom/i2c_at24cxx.h"
#endif

#if (PDL_UTILITY_ENABLE_I2S_CODEC_AT24CXX == PDL_ON)
    #include "eeprom/i2c_at24cxx.h"
#endif      

#if (PDL_UTILITY_ENABLE_I2S_CODEC_WM8731 == PDL_ON)
    #include "i2s_codec/wm8731.h"
#endif      

#if (PDL_UTILITY_ENABLE_HBIF_S26KL512S == PDL_ON)
    #include "hyper_flash/s26kl512s.h"
#endif
      
#if (PDL_UTILITY_ENABLE_EXTIF_S34ML01G == PDL_ON)
    #include "nand_flash/s34ml01g.h"
#endif       
      
#if (PDL_UTILITY_ENABLE_EXTIF_IS42S16800 == PDL_ON)
    #include "sdram/is42s16800.h"
#endif              
      
#if (PDL_UTILITY_ENABLE_UART_PRINTF == PDL_ON)
    #include "printf_scanf/uart_io.h"
#endif

#if (PDL_UTILITY_ENABLE_SEG_LCD_TSDH1188 == PDL_ON)
    #include "seg_lcd/tsdh1188/tsdh1188.h"
#endif
      
#if (PDL_UTILITY_ENABLE_SEG_LCD_CL0107031 == PDL_ON)
    #include "seg_lcd/cl010/cl010.h"
#endif     

#if (PDL_UTILITY_ENABLE_QSPI_POLLING_S25FL164K == PDL_ON) || \
    (PDL_UTILITY_ENABLE_QSPI_IRQ_S25FL164K == PDL_ON)
    #include "qspi_flash/flashS25FL164K.h"  
#endif       

#ifdef __cplusplus
}
#endif

#endif  /* __PDL_HEADER_H__ */

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
