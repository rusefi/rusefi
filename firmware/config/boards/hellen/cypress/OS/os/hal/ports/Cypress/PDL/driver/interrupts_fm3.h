/*******************************************************************************
* \file             interrupts_fm3.h
* 
* \version          1.0
*                       
* \brief            Low-level interrupt processing layer to handle the hardware
*                   interrupts.
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
 #ifndef __INTERRUPTS_FM3_H__
#define __INTERRUPTS_FM3_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/

#include "pdl_user.h"

#if (PDL_MCU_CORE == PDL_FM3_CORE)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

//@{

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/

/*****************************************************************************/
/* Check whether interrupt is enable when peripheral is inactive             */
/*****************************************************************************/

// Include adc.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_ADC0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_ADC1) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_ADC2)
    #if defined(PDL_PERIPHERAL_ADC_ACTIVE)
        #include "adc/adc.h"
    #else
        #error Do not enable ADC interrupt when it is inactive!
    #endif
#endif

// Include bt.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_BT0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT1) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT2) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT3) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT4) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT5) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT6) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT7) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT8) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT9) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT10) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT11) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT12) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT13) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT14) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT15)
    #if defined(PDL_PERIPHERAL_BT_ACTIVE)
        #include "bt/bt.h"
    #else
        #error Do not enable BT interrupt when it is inactive!
    #endif
#endif

// Include can.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_CAN0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_CAN1)
    #if defined(PDL_PERIPHERAL_CAN_ACTIVE)
        #include "can/can.h"
    #else
        #error Do not enable CAN interrupt when it is inactive!
    #endif
#endif

// Include cec.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_CEC0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_CEC1)
    #if defined(PDL_PERIPHERAL_CEC_ACTIVE)
        #include "cec.h"
    #else
        #error Do not enable CEC interrupt when it is inactive!
    #endif
#endif

// Include clk.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_CLK)
    #if defined(PDL_PERIPHERAL_CLK_ACTIVE)
        #include "clk/clk.h"
    #else
        #error Do not enable CLK interrupt when it is inactive!
    #endif
#endif

// Include csv.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_CSV)
    #if defined(PDL_PERIPHERAL_CSV_ACTIVE)
        #include "csv/csv.h"
    #else
        #error Do not enable CSV interrupt when it is inactive!
    #endif
#endif

// Include dma.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DMA0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_DMA1) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_DMA2) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_DMA3) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_DMA4) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_DMA5) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_DMA6) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_DMA7)
    #if defined(PDL_PERIPHERAL_DMA_ACTIVE)
        #include "dma/dma.h"
    #else
        #error Do not enable DMA interrupt when it is inactive!
    #endif
#endif

// Include dt.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DT0)
    #if defined(PDL_PERIPHERAL_DT_ACTIVE)
        #include "dt/dt.h"
    #else
        #error Do not enable DT interrupt when it is inactive!
    #endif
#endif

// Include ether.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_ETHER0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_ETHER1)
    #if defined(PDL_PERIPHERAL_ETHER_ACTIVE)
        #include "ether.h"
    #else
        #error Do not enable ETHER interrupt when it is inactive!
    #endif
#endif

// Include exint.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT1) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT2) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT3) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT4) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT5) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT6) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT7) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT8) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT9) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT10) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT11) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT12) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT13) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT14) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT15) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT16) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT17) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT18) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT19) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT20) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT21) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT22) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT23) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT24) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT25) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT26) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT27) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT28) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT30) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT31)
    #if defined(PDL_PERIPHERAL_EXINT_ACTIVE)
        #include "exint/exint.h"
    #else
        #error Do not enable EXINT interrupt when it is inactive!
    #endif
#endif

// Include NMI code in exint.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_NMI) 
    #if defined(PDL_PERIPHERAL_NMI_ACTIVE)
        #include "exint/exint.h"
    #else
        #error Do not enable NMI interrupt when it is inactive.
    #endif
#endif

// Include lcd.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_LCD)
    #if defined(PDL_PERIPHERAL_LCD_ACTIVE)
        #include "lcd/lcd.h"
    #else
        #error Do not enable LCD interrupt when it is inactive!
    #endif
#endif

// Include lvd.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_LVD)
    #if defined(PDL_PERIPHERAL_LVD_ACTIVE)
        #include "lvd/lvd.h"
    #else
        #error Do not enable LVD interrupt when it is inactive!
    #endif
#endif

// Include main_flash.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_FLASH)
    #if defined(PDL_PERIPHERAL_MAIN_FLASH_ACTIVE)
        #include "flash/mainflash.h"
    #else
        #error Do not enable MAIN_FLASH interrupt when it is inactive!
    #endif
#endif

// Include mfs.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS8) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS1) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS9) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS2) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS10) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS3) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS11) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS4) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS12) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS5) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS13) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS6) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS14) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS7) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS15)
    #if defined(PDL_PERIPHERAL_MFS_ACTIVE)
        #include "mfs/mfs.h"
    #else
        #error Do not enable MFS interrupt when it is inactive!
    #endif
#endif

// Include mft_frt.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_FRT) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFT1_FRT) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFT2_FRT)
    #if defined(PDL_PERIPHERAL_MFT_FRT_ACTIVE)
        #include "mft/mft_frt.h"
    #else
        #error Do not enable MFT_FRT interrupt when it is inactive!
    #endif
#endif

// Include mft_icu.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_ICU) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFT1_ICU) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFT2_ICU)
    #if defined(PDL_PERIPHERAL_MFT_ICU_ACTIVE)
        #include "mft/mft_icu.h"
    #else
        #error Do not enable MFT_ICU interrupt when it is inactive!
    #endif
#endif

// Include mft_ocu.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_OCU) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFT1_OCU) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFT2_OCU)
    #if defined(PDL_PERIPHERAL_MFT_OCU_ACTIVE)
        #include "mft/mft_ocu.h"
    #else
        #error Do not enable MFT_OCU interrupt when it is inactive!
    #endif
#endif

// Include mft_wfg.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_WFG) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFT1_WFG) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFT2_WFG) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_WFG_DTIF)
    #if defined(PDL_PERIPHERAL_MFT_WFG_ACTIVE)
        #include "mft/mft_wfg.h"
    #else
        #error Do not enable MFT_WFG interrupt when it is inactive!
    #endif
#endif

// Include ppg.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_PPG)
    #if defined(PDL_PERIPHERAL_PPG_ACTIVE)
        #include "ppg/ppg.h"
    #else
        #error Do not enable PPG interrupt when it is inactive!
    #endif
#endif

// Include qprc.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_QPRC0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_QPRC1) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_QPRC2)
    #if defined(PDL_PERIPHERAL_QPRC_ACTIVE)
        #include "qprc/qprc.h"
    #else
        #error Do not enable QPRC interrupt when it is inactive!
    #endif
#endif

// Include rtc.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_RTC)
    #if defined(PDL_PERIPHERAL_RTC_ACTIVE)
        #include "rtc/rtc.h"
    #else
        #error Do not enable RTC interrupt when it is inactive!
    #endif
#endif

// Include usb.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_USB0_DEVICE) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_USB0_HOST) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_USB1_DEVICE) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_USB1_HOST)
    #if defined(PDL_PERIPHERAL_USB_ACTIVE)
        #include "usb/usb.h"
    #else
        #error Do not enable USB interrupt when it is inactive!
    #endif
#endif

// Include wc.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_WC)
    #if defined(PDL_PERIPHERAL_WC_ACTIVE)
        #include "wc/wc.h"
    #else
        #error Do not enable WC interrupt when it is inactive!
    #endif
#endif

// Include wdg.h if active and interrupts are enabled.
#if (PDL_ON == PDL_INTERRUPT_ENABLE_SWWDG) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_HWWDG)
    #if defined(PDL_PERIPHERAL_WDG_ACTIVE)
        #include "wdg/wdg.h"
    #else
        #error Do not enable WDG interrupt when it is inactive!
    #endif
#endif

    #if (PDL_ON == PDL_INTERRUPT_ENABLE_DSTC)
        #if defined(PDL_PERIPHERAL_DSTC_ACTIVE)
            #include "dstc/dstc.h"
        #else
            #error Do not enable DSTC interrupt when it is inactive!
        #endif
    #endif
//@} // PdlInterrupts
#ifdef __cplusplus
}
#endif

#endif // PDL_MCU_CORE == PDL_FM3_CORE)

#endif // #ifndef __INTERRUPTS_FM3_H__
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
