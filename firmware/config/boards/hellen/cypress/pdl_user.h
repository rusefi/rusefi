/*******************************************************************************
* \file             pdl_user.h
* 
* \version          1.10
*
* \brief            User settings headerfile for Peripheral Driver Library
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
#ifndef __PDL_USER_H__
#define __PDL_USER_H__

#include "pdl.h"

/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/

/**
 ******************************************************************************
 ** User Defines for PDL resource activation
 **
 ** Possible definitions are PDL_ON and PDL_OFF.
 **
 ******************************************************************************/
// ADC
#define PDL_PERIPHERAL_ENABLE_ADC0              PDL_ON
#define PDL_PERIPHERAL_ENABLE_ADC1              PDL_ON
#define PDL_PERIPHERAL_ENABLE_ADC2              PDL_ON

// AES
#define PDL_PERIPHERAL_ENABLE_AES               PDL_OFF

// Base Timers
#define PDL_PERIPHERAL_ENABLE_BT0               PDL_ON
#define PDL_PERIPHERAL_ENABLE_BT1               PDL_ON
#define PDL_PERIPHERAL_ENABLE_BT2               PDL_ON
#define PDL_PERIPHERAL_ENABLE_BT3               PDL_ON
#define PDL_PERIPHERAL_ENABLE_BT4               PDL_OFF
#define PDL_PERIPHERAL_ENABLE_BT5               PDL_OFF
#define PDL_PERIPHERAL_ENABLE_BT6               PDL_OFF
#define PDL_PERIPHERAL_ENABLE_BT7               PDL_OFF
#define PDL_PERIPHERAL_ENABLE_BT8               PDL_OFF
#define PDL_PERIPHERAL_ENABLE_BT9               PDL_OFF
#define PDL_PERIPHERAL_ENABLE_BT10              PDL_OFF
#define PDL_PERIPHERAL_ENABLE_BT11              PDL_OFF
#define PDL_PERIPHERAL_ENABLE_BT12              PDL_OFF
#define PDL_PERIPHERAL_ENABLE_BT13              PDL_OFF
#define PDL_PERIPHERAL_ENABLE_BT14              PDL_OFF
#define PDL_PERIPHERAL_ENABLE_BT15              PDL_OFF

// Modes of Base Timers
#define PDL_PERIPHERAL_ENABLE_BT_PWM_MODE       PDL_OFF
#define PDL_PERIPHERAL_ENABLE_BT_PPG_MODE       PDL_OFF
#define PDL_PERIPHERAL_ENABLE_BT_RT_MODE        PDL_ON
#define PDL_PERIPHERAL_ENABLE_BT_PWC_MODE       PDL_OFF

// CAN
#define PDL_PERIPHERAL_ENABLE_CAN0              PDL_OFF
#define PDL_PERIPHERAL_ENABLE_CAN1              PDL_OFF

// CAN-FD
#define PDL_PERIPHERAL_ENABLE_CANFD0            PDL_ON
#define PDL_PERIPHERAL_ENABLE_CANFD1            PDL_OFF

// Clock
#define PDL_PERIPHERAL_ENABLE_CLK               PDL_ON

// CR Trimming
#define PDL_PERIPHERAL_ENABLE_CR                PDL_OFF

// CRC
#define PDL_PERIPHERAL_ENABLE_CRC0              PDL_OFF

// Clock Supervisor
#define PDL_PERIPHERAL_ENABLE_CSV               PDL_OFF

// DAC
#define PDL_PERIPHERAL_ENABLE_DAC0              PDL_OFF

// DMA
#define PDL_PERIPHERAL_ENABLE_DMA0              PDL_ON
#define PDL_PERIPHERAL_ENABLE_DMA1              PDL_OFF
#define PDL_PERIPHERAL_ENABLE_DMA2              PDL_OFF
#define PDL_PERIPHERAL_ENABLE_DMA3              PDL_OFF
#define PDL_PERIPHERAL_ENABLE_DMA4              PDL_OFF
#define PDL_PERIPHERAL_ENABLE_DMA5              PDL_OFF
#define PDL_PERIPHERAL_ENABLE_DMA6              PDL_OFF
#define PDL_PERIPHERAL_ENABLE_DMA7              PDL_OFF

// DSTC
#define PDL_PERIPHERAL_ENABLE_DSTC              PDL_OFF

// Dual Timer(s)
#define PDL_PERIPHERAL_ENABLE_DT0               PDL_OFF

// Ethernet
// Please activate/deactivate in emac_user.h

// External Interrupts
#define PDL_PERIPHERAL_ENABLE_EXINT0            PDL_OFF
#define PDL_PERIPHERAL_ENABLE_EXINT1            PDL_OFF
#define PDL_PERIPHERAL_ENABLE_EXINT2            PDL_OFF
#define PDL_PERIPHERAL_ENABLE_EXINT3            PDL_OFF
#define PDL_PERIPHERAL_ENABLE_EXINT4            PDL_OFF
#define PDL_PERIPHERAL_ENABLE_EXINT5            PDL_OFF
#define PDL_PERIPHERAL_ENABLE_EXINT6            PDL_OFF
#define PDL_PERIPHERAL_ENABLE_EXINT7            PDL_OFF
#define PDL_PERIPHERAL_ENABLE_EXINT8            PDL_ON  	// P10 crank
#define PDL_PERIPHERAL_ENABLE_EXINT9            PDL_ON		// P29 res_in
#define PDL_PERIPHERAL_ENABLE_EXINT10           PDL_ON		// P18 cam
#define PDL_PERIPHERAL_ENABLE_EXINT11           PDL_ON		// P1B aux1
#define PDL_PERIPHERAL_ENABLE_EXINT12           PDL_OFF
#define PDL_PERIPHERAL_ENABLE_EXINT13           PDL_ON		// PC7 res2
#define PDL_PERIPHERAL_ENABLE_EXINT14           PDL_OFF
#define PDL_PERIPHERAL_ENABLE_EXINT15           PDL_OFF
#define PDL_PERIPHERAL_ENABLE_EXINT16           PDL_OFF
#define PDL_PERIPHERAL_ENABLE_EXINT17           PDL_OFF
#define PDL_PERIPHERAL_ENABLE_EXINT18           PDL_OFF
#define PDL_PERIPHERAL_ENABLE_EXINT19           PDL_ON		// P32 drv_miso
#define PDL_PERIPHERAL_ENABLE_EXINT20           PDL_OFF
#define PDL_PERIPHERAL_ENABLE_EXINT21           PDL_OFF
#define PDL_PERIPHERAL_ENABLE_EXINT22           PDL_OFF
#define PDL_PERIPHERAL_ENABLE_EXINT23           PDL_OFF
#define PDL_PERIPHERAL_ENABLE_EXINT24           PDL_ON		// P19 vss
#define PDL_PERIPHERAL_ENABLE_EXINT25           PDL_ON		// P25 aux4
#define PDL_PERIPHERAL_ENABLE_EXINT26           PDL_ON		// P1E aux2
#define PDL_PERIPHERAL_ENABLE_EXINT27           PDL_ON		// P1F aux3
#define PDL_PERIPHERAL_ENABLE_EXINT28           PDL_OFF
#define PDL_PERIPHERAL_ENABLE_EXINT29           PDL_OFF
#define PDL_PERIPHERAL_ENABLE_EXINT30           PDL_OFF
#define PDL_PERIPHERAL_ENABLE_EXINT31           PDL_ON		// P60 vbus_det

// External Bus Interface
#define PDL_PERIPHERAL_ENABLE_EXTIF             PDL_OFF

// Flash routines
#define PDL_PERIPHERAL_ENABLE_MAIN_FLASH        PDL_ON
#define PDL_PERIPHERAL_ENABLE_DUAL_FLASH        PDL_ON
#define PDL_PERIPHERAL_ENABLE_WORK_FLASH        PDL_OFF

// GPIO header inclusion
#define PDL_PERIPHERAL_ENABLE_GPIO              PDL_ON

// Hyber Bus Interface
#define PDL_PERIPHERAL_ENABLE_HBIF              PDL_OFF

// High-Speed Quad SPI
#define PDL_PERIPHERAL_ENABLE_HSSPI0            PDL_OFF

// I2C Slave
#define PDL_PERIPHERAL_ENABLE_I2CS0             PDL_OFF
   
// ICC
#define PDL_PERIPHERAL_ENABLE_ICC0              PDL_OFF
#define PDL_PERIPHERAL_ENABLE_ICC1              PDL_OFF

// I2S
#define PDL_PERIPHERAL_ENABLE_I2S0              PDL_OFF
#define PDL_PERIPHERAL_ENABLE_I2S1              PDL_OFF

// I2S-Lite
#define PDL_PERIPHERAL_ENABLE_I2SL0             PDL_OFF
#define PDL_PERIPHERAL_ENABLE_I2SL1             PDL_OFF

// LCD controller
#define PDL_PERIPHERAL_ENABLE_LCD               PDL_OFF

// LPM
#define PDL_PERIPHERAL_ENABLE_LPM               PDL_OFF

// Low Voltage Detection
#define PDL_PERIPHERAL_ENABLE_LVD               PDL_OFF

// Multi Function Serial Interfaces
#define PDL_PERIPHERAL_ENABLE_MFS0              PDL_ON
#define PDL_PERIPHERAL_ENABLE_MFS1              PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFS2              PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFS3              PDL_ON
#define PDL_PERIPHERAL_ENABLE_MFS4              PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFS5              PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFS6              PDL_ON
#define PDL_PERIPHERAL_ENABLE_MFS7              PDL_ON
#define PDL_PERIPHERAL_ENABLE_MFS8              PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFS9              PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFS10             PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFS11             PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFS12             PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFS13             PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFS14             PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFS15             PDL_OFF

// Modes of Multi Function Serial Interfaces
#define PDL_PERIPHERAL_ENABLE_MFS_UART_MODE     PDL_ON
#define PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE     PDL_ON
#define PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE      PDL_ON
#define PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE      PDL_ON

// Multi Function Timer Interfaces
#define PDL_PERIPHERAL_ENABLE_MFT0_FRT          PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFT0_OCU          PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFT0_WFG          PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFT0_ICU          PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFT0_ADCMP        PDL_OFF

#define PDL_PERIPHERAL_ENABLE_MFT1_FRT          PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFT1_OCU          PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFT1_WFG          PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFT1_ICU          PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFT1_ADCMP        PDL_OFF

#define PDL_PERIPHERAL_ENABLE_MFT2_FRT          PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFT2_OCU          PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFT2_WFG          PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFT2_ICU          PDL_OFF
#define PDL_PERIPHERAL_ENABLE_MFT2_ADCMP        PDL_OFF

// PPG
#define PDL_PERIPHERAL_ENABLE_PPG               PDL_OFF

// Programmable-CRC
#define PDL_PERIPHERAL_ENABLE_PCRC              PDL_OFF

// NMI
#define PDL_PERIPHERAL_ENABLE_NMI               PDL_OFF

// Quad Decoder
#define PDL_PERIPHERAL_ENABLE_QPRC0             PDL_OFF
#define PDL_PERIPHERAL_ENABLE_QPRC1             PDL_OFF
#define PDL_PERIPHERAL_ENABLE_QPRC2             PDL_OFF
#define PDL_PERIPHERAL_ENABLE_QPRC3             PDL_OFF

// Remote Control routines
#define PDL_PERIPHERAL_ENABLE_RC0               PDL_OFF
#define PDL_PERIPHERAL_ENABLE_RC1               PDL_OFF

// RX modes of Remote Control routines
#define PDL_PERIPHERAL_ENABLE_RCRX_SIRCS_MODE   PDL_OFF
#define PDL_PERIPHERAL_ENABLE_RCRX_NEC_MODE     PDL_OFF
#define PDL_PERIPHERAL_ENABLE_RCRX_CEC_MODE     PDL_OFF

// TX modes of Remote Control routines
#define PDL_PERIPHERAL_ENABLE_RCTX_CEC_MODE     PDL_OFF

// Reset Cause
#define PDL_PERIPHERAL_ENABLE_RESET             PDL_OFF

// Real Time Clock
#define PDL_PERIPHERAL_ENABLE_RTC0              PDL_OFF

// SD
#define PDL_PERIPHERAL_ENABLE_SD0               PDL_OFF

// Unique ID
#define PDL_PERIPHERAL_ENABLE_UID               PDL_OFF

// USB
#define PDL_PERIPHERAL_USB_ACTIVE               PDL_ON
#define PDL_PERIPHERAL_ENABLE_USB0_DEVICE       PDL_ON
#define PDL_PERIPHERAL_ENABLE_USB0_HOST         PDL_OFF
#define PDL_PERIPHERAL_ENABLE_USB1_DEVICE       PDL_OFF
#define PDL_PERIPHERAL_ENABLE_USB1_HOST         PDL_OFF

// USB Device Middle Ware
#define PDL_USBDEVICECDCCOM_ENABLED             PDL_ON
#define PDL_USBDEVICEHIDCOM_ENABLED             PDL_OFF
#define PDL_USBDEVICEHIDJOYSTICK_ENABLED        PDL_OFF
#define PDL_USBDEVICEHIDKEYBOARD_ENABLED        PDL_OFF
#define PDL_USBDEVICEHIDMOUSE_ENABLED           PDL_OFF
#define PDL_USBDEVICELIBUSB_ENABLED             PDL_OFF
#define PDL_USBDEVICEPRINTER_ENABLED            PDL_OFF

// USB Host Middle Ware
#define PDL_USBHOSTHIDCOM_ENABLED               PDL_OFF
#define PDL_USBHOSTHIDKEYBOARD_ENABLED          PDL_OFF
#define PDL_USBHOSTHIDMOUSE_ENABLED             PDL_OFF
#define PDL_USBHOSTMASSSTORAGE_ENABLED          PDL_OFF

// VBAT domain
#define PDL_PERIPHERAL_ENABLE_VBAT              PDL_OFF

// Watch Counter
#define PDL_PERIPHERAL_ENABLE_WC0               PDL_OFF

// Watchdog Timers
#define PDL_PERIPHERAL_ENABLE_HWWDG             PDL_OFF
#define PDL_PERIPHERAL_ENABLE_SWWDG             PDL_OFF

/**
 ******************************************************************************
 ** User Interrupt activation settings
 **
 ** Possible values are PDL_INT_ACTIVE (Interrupts active) and PDL_INT_INACTIVE
 ** (Interrupts deactivated)
 ******************************************************************************/
// ADC
#define PDL_INTERRUPT_ENABLE_ADC0              PDL_ON
#define PDL_INTERRUPT_ENABLE_ADC1              PDL_ON
#define PDL_INTERRUPT_ENABLE_ADC2              PDL_ON

// Base Timers
#define PDL_INTERRUPT_ENABLE_BT0               PDL_ON
#define PDL_INTERRUPT_ENABLE_BT1               PDL_ON
#define PDL_INTERRUPT_ENABLE_BT2               PDL_ON
#define PDL_INTERRUPT_ENABLE_BT3               PDL_ON
#define PDL_INTERRUPT_ENABLE_BT4               PDL_OFF
#define PDL_INTERRUPT_ENABLE_BT5               PDL_OFF
#define PDL_INTERRUPT_ENABLE_BT6               PDL_OFF
#define PDL_INTERRUPT_ENABLE_BT7               PDL_OFF
#define PDL_INTERRUPT_ENABLE_BT8               PDL_OFF
#define PDL_INTERRUPT_ENABLE_BT9               PDL_OFF
#define PDL_INTERRUPT_ENABLE_BT10              PDL_OFF
#define PDL_INTERRUPT_ENABLE_BT11              PDL_OFF
#define PDL_INTERRUPT_ENABLE_BT12              PDL_OFF
#define PDL_INTERRUPT_ENABLE_BT13              PDL_OFF
#define PDL_INTERRUPT_ENABLE_BT14              PDL_OFF
#define PDL_INTERRUPT_ENABLE_BT15              PDL_OFF

// CAN
#define PDL_INTERRUPT_ENABLE_CAN0              PDL_OFF
#define PDL_INTERRUPT_ENABLE_CAN1              PDL_OFF

// CAN-FD
#define PDL_INTERRUPT_ENABLE_CANFD0            PDL_ON
#define PDL_INTERRUPT_ENABLE_CANFD1            PDL_OFF

// Clock
#define PDL_INTERRUPT_ENABLE_CLK               PDL_OFF

// Clock Supervisor
#define PDL_INTERRUPT_ENABLE_CSV               PDL_OFF

// DMA
#define PDL_INTERRUPT_ENABLE_DMA0              PDL_OFF
#define PDL_INTERRUPT_ENABLE_DMA1              PDL_OFF
#define PDL_INTERRUPT_ENABLE_DMA2              PDL_OFF
#define PDL_INTERRUPT_ENABLE_DMA3              PDL_OFF
#define PDL_INTERRUPT_ENABLE_DMA4              PDL_OFF
#define PDL_INTERRUPT_ENABLE_DMA5              PDL_OFF
#define PDL_INTERRUPT_ENABLE_DMA6              PDL_OFF
#define PDL_INTERRUPT_ENABLE_DMA7              PDL_OFF

// Dual Timer(s)
#define PDL_INTERRUPT_ENABLE_DT0               PDL_OFF

// DSTC
#define PDL_INTERRUPT_ENABLE_DSTC              PDL_OFF

// External Interrupts (automatically set by peripheral enable)
#if (PDL_PERIPHERAL_ENABLE_EXINT0 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT0            PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT0            PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT1 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT1            PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT1            PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT2 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT2            PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT2            PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT3 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT3            PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT3            PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT4 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT4            PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT4            PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT5 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT5            PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT5            PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT6 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT6            PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT6            PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT7 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT7            PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT7            PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT8 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT8            PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT8            PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT9 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT9            PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT9            PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT10 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT10           PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT10           PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT11 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT11           PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT11           PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT12 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT12           PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT12           PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT13 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT13           PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT13           PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT14 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT14           PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT14           PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT15 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT15           PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT15           PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT16== PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT16           PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT16           PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT17 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT17           PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT17           PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT18 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT18           PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT18           PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT19 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT19           PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT19           PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT20 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT20           PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT20           PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT21 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT21           PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT21           PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT22 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT22           PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT22           PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT23 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT23           PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT23           PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT24 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT24           PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT24           PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT25 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT25           PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT25           PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT26 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT26           PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT26           PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT27 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT27           PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT27           PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT28 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT28           PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT28           PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT29 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT29           PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT29           PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT30 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT30           PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT30           PDL_OFF
#endif
#if (PDL_PERIPHERAL_ENABLE_EXINT31 == PDL_ON)
#define PDL_INTERRUPT_ENABLE_EXINT31           PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_EXINT31           PDL_OFF
#endif

// External Bus Interface
#define PDL_INTERRUPT_ENABLE_EXTIF             PDL_OFF

// Flash
#define PDL_INTERRUPT_ENABLE_FLASH             PDL_ON

// Hyper Bus Interface
#define PDL_INTERRUPT_ENABLE_HBIF              PDL_OFF

// High-Speed Quad SPI
#define PDL_INTERRUPT_ENABLE_HSSPI0            PDL_OFF

// I2C Slave
#define PDL_INTERRUPT_ENABLE_I2CS0             PDL_OFF

// ICC
#define PDL_INTERRUPT_ENABLE_ICC0              PDL_OFF
#define PDL_INTERRUPT_ENABLE_ICC1              PDL_OFF

// LCD
#define PDL_INTERRUPT_ENABLE_LCD               PDL_OFF

// Low Voltage Detection
#define PDL_INTERRUPT_ENABLE_LVD               PDL_OFF

// I2S
#define PDL_INTERRUPT_ENABLE_I2S0              PDL_OFF
#define PDL_INTERRUPT_ENABLE_I2S1              PDL_OFF

// I2S-Lite
#define PDL_INTERRUPT_ENABLE_I2SL0             PDL_OFF
#define PDL_INTERRUPT_ENABLE_I2SL1             PDL_OFF

// Multi Function Serial Interfaces
#define PDL_INTERRUPT_ENABLE_MFS0              PDL_ON
#define PDL_INTERRUPT_ENABLE_MFS1              PDL_OFF
#define PDL_INTERRUPT_ENABLE_MFS2              PDL_OFF
#define PDL_INTERRUPT_ENABLE_MFS3              PDL_ON
#define PDL_INTERRUPT_ENABLE_MFS4              PDL_OFF
#define PDL_INTERRUPT_ENABLE_MFS5              PDL_OFF
#define PDL_INTERRUPT_ENABLE_MFS6              PDL_ON
#define PDL_INTERRUPT_ENABLE_MFS7              PDL_ON
#define PDL_INTERRUPT_ENABLE_MFS8              PDL_OFF
#define PDL_INTERRUPT_ENABLE_MFS9              PDL_OFF
#define PDL_INTERRUPT_ENABLE_MFS10             PDL_OFF
#define PDL_INTERRUPT_ENABLE_MFS11             PDL_OFF
#define PDL_INTERRUPT_ENABLE_MFS12             PDL_OFF
#define PDL_INTERRUPT_ENABLE_MFS13             PDL_OFF
#define PDL_INTERRUPT_ENABLE_MFS14             PDL_OFF
#define PDL_INTERRUPT_ENABLE_MFS15             PDL_OFF

// Multi Function Timer Interfaces
#define PDL_INTERRUPT_ENABLE_MFT0_FRT          PDL_OFF
#define PDL_INTERRUPT_ENABLE_MFT0_OCU          PDL_OFF
#define PDL_INTERRUPT_ENABLE_MFT0_WFG          PDL_OFF
#define PDL_INTERRUPT_ENABLE_MFT0_ICU          PDL_OFF

#define PDL_INTERRUPT_ENABLE_MFT1_FRT          PDL_OFF
#define PDL_INTERRUPT_ENABLE_MFT1_OCU          PDL_OFF
#define PDL_INTERRUPT_ENABLE_MFT1_WFG          PDL_OFF
#define PDL_INTERRUPT_ENABLE_MFT1_ICU          PDL_OFF

#define PDL_INTERRUPT_ENABLE_MFT2_FRT          PDL_OFF
#define PDL_INTERRUPT_ENABLE_MFT2_OCU          PDL_OFF
#define PDL_INTERRUPT_ENABLE_MFT2_WFG          PDL_OFF
#define PDL_INTERRUPT_ENABLE_MFT2_ICU          PDL_OFF

// NMI
#if (PDL_PERIPHERAL_ENABLE_NMI== PDL_ON)
#define PDL_INTERRUPT_ENABLE_NMI               PDL_ON
#else
#define PDL_INTERRUPT_ENABLE_NMI               PDL_OFF
#endif

// Programmable-CRC
#define PDL_INTERRUPT_ENABLE_PCRC              PDL_OFF

// PPG
#define PDL_INTERRUPT_ENABLE_PPG               PDL_OFF

// Quad Decoder
#define PDL_INTERRUPT_ENABLE_QPRC0             PDL_OFF
#define PDL_INTERRUPT_ENABLE_QPRC1             PDL_OFF
#define PDL_INTERRUPT_ENABLE_QPRC2             PDL_OFF
#define PDL_INTERRUPT_ENABLE_QPRC3             PDL_OFF

// Real Time Clock
#define PDL_INTERRUPT_ENABLE_RTC0              PDL_OFF

// Remote Control routines
#define PDL_INTERRUPT_ENABLE_RC0               PDL_OFF
#define PDL_INTERRUPT_ENABLE_RC1               PDL_OFF

// SD Host
#define PDL_INTERRUPT_ENABLE_SD0               PDL_OFF

// USB
#define PDL_INTERRUPT_ENABLE_USB0_DEVICE       PDL_ON
#define PDL_INTERRUPT_ENABLE_USB0_HOST         PDL_OFF
#define PDL_INTERRUPT_ENABLE_USB1_DEVICE       PDL_OFF
#define PDL_INTERRUPT_ENABLE_USB1_HOST         PDL_OFF

// Watch Counter
#define PDL_INTERRUPT_ENABLE_WC0               PDL_OFF

// Watchdog Timers
#define PDL_INTERRUPT_ENABLE_HWWDG             PDL_OFF
#define PDL_INTERRUPT_ENABLE_SWWDG             PDL_OFF

/**
 ******************************************************************************
 ** User DSTC enable settings
 **
 ** Possible values are PDL_ON or PDL_OFF
 ******************************************************************************/
// ADC
#define PDL_DSTC_ENABLE_ADC0_PRIO               PDL_OFF
#define PDL_DSTC_ENABLE_ADC0_SCAN               PDL_OFF
#define PDL_DSTC_ENABLE_ADC1_PRIO               PDL_OFF
#define PDL_DSTC_ENABLE_ADC1_SCAN               PDL_OFF
#define PDL_DSTC_ENABLE_ADC2_PRIO               PDL_OFF
#define PDL_DSTC_ENABLE_ADC2_SCAN               PDL_OFF

// BT
#define PDL_DSTC_ENABLE_BT0_IRQ0                PDL_OFF
#define PDL_DSTC_ENABLE_BT0_IRQ1                PDL_OFF
#define PDL_DSTC_ENABLE_BT1_IRQ0                PDL_OFF
#define PDL_DSTC_ENABLE_BT1_IRQ1                PDL_OFF
#define PDL_DSTC_ENABLE_BT2_IRQ0                PDL_OFF
#define PDL_DSTC_ENABLE_BT2_IRQ1                PDL_OFF
#define PDL_DSTC_ENABLE_BT3_IRQ0                PDL_OFF
#define PDL_DSTC_ENABLE_BT3_IRQ1                PDL_OFF
#define PDL_DSTC_ENABLE_BT4_IRQ0                PDL_OFF
#define PDL_DSTC_ENABLE_BT4_IRQ1                PDL_OFF
#define PDL_DSTC_ENABLE_BT5_IRQ0                PDL_OFF
#define PDL_DSTC_ENABLE_BT5_IRQ1                PDL_OFF
#define PDL_DSTC_ENABLE_BT6_IRQ0                PDL_OFF
#define PDL_DSTC_ENABLE_BT6_IRQ1                PDL_OFF
#define PDL_DSTC_ENABLE_BT7_IRQ0                PDL_OFF
#define PDL_DSTC_ENABLE_BT7_IRQ1                PDL_OFF
#define PDL_DSTC_ENABLE_BT8_IRQ0                PDL_OFF
#define PDL_DSTC_ENABLE_BT8_IRQ1                PDL_OFF
#define PDL_DSTC_ENABLE_BT9_IRQ0                PDL_OFF
#define PDL_DSTC_ENABLE_BT9_IRQ1                PDL_OFF
#define PDL_DSTC_ENABLE_BT10_IRQ0               PDL_OFF
#define PDL_DSTC_ENABLE_BT10_IRQ1               PDL_OFF
#define PDL_DSTC_ENABLE_BT11_IRQ0               PDL_OFF
#define PDL_DSTC_ENABLE_BT11_IRQ1               PDL_OFF
#define PDL_DSTC_ENABLE_BT12_IRQ0               PDL_OFF
#define PDL_DSTC_ENABLE_BT12_IRQ1               PDL_OFF
#define PDL_DSTC_ENABLE_BT13_IRQ0               PDL_OFF
#define PDL_DSTC_ENABLE_BT13_IRQ1               PDL_OFF
#define PDL_DSTC_ENABLE_BT14_IRQ0               PDL_OFF
#define PDL_DSTC_ENABLE_BT14_IRQ1               PDL_OFF
#define PDL_DSTC_ENABLE_BT15_IRQ0               PDL_OFF
#define PDL_DSTC_ENABLE_BT15_IRQ1               PDL_OFF

// CAN-FD
#define PDL_DSTC_ENABLE_CANFD0                  PDL_OFF

// EXINT
#define PDL_DSTC_ENABLE_EXTINT0                 PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT1                 PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT2                 PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT3                 PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT4                 PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT5                 PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT6                 PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT7                 PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT8                 PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT9                 PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT10                PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT11                PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT12                PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT13                PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT14                PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT15                PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT16                PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT17                PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT18                PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT19                PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT20                PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT21                PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT22                PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT23                PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT24                PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT25                PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT26                PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT27                PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT28                PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT29                PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT30                PDL_OFF
#define PDL_DSTC_ENABLE_EXTINT31                PDL_OFF

// HSSPI
#define PDL_DSTC_ENABLE_HSSPI0_TX               PDL_OFF
#define PDL_DSTC_ENABLE_HSSPI0_RX               PDL_OFF

// I2C Slave
#define PDL_DSTC_ENABLE_I2CS0_RX                PDL_OFF
#define PDL_DSTC_ENABLE_I2CS0_TX                PDL_OFF  
   
// I2S
#define PDL_DSTC_ENABLE_I2S0_TX                 PDL_OFF
#define PDL_DSTC_ENABLE_I2S0_RX                 PDL_OFF
#define PDL_DSTC_ENABLE_I2S1_TX                 PDL_OFF
#define PDL_DSTC_ENABLE_I2S1_RX                 PDL_OFF

// MFS
#define PDL_DSTC_ENABLE_MFS0_RX                 PDL_OFF
#define PDL_DSTC_ENABLE_MFS0_TX                 PDL_OFF
#define PDL_DSTC_ENABLE_MFS1_RX                 PDL_OFF
#define PDL_DSTC_ENABLE_MFS1_TX                 PDL_OFF
#define PDL_DSTC_ENABLE_MFS2_RX                 PDL_OFF
#define PDL_DSTC_ENABLE_MFS2_TX                 PDL_OFF
#define PDL_DSTC_ENABLE_MFS3_RX                 PDL_OFF
#define PDL_DSTC_ENABLE_MFS3_TX                 PDL_OFF
#define PDL_DSTC_ENABLE_MFS4_RX                 PDL_OFF
#define PDL_DSTC_ENABLE_MFS4_TX                 PDL_OFF
#define PDL_DSTC_ENABLE_MFS5_RX                 PDL_OFF
#define PDL_DSTC_ENABLE_MFS5_TX                 PDL_OFF
#define PDL_DSTC_ENABLE_MFS6_RX                 PDL_OFF
#define PDL_DSTC_ENABLE_MFS6_TX                 PDL_OFF
#define PDL_DSTC_ENABLE_MFS7_RX                 PDL_OFF
#define PDL_DSTC_ENABLE_MFS7_TX                 PDL_OFF
#define PDL_DSTC_ENABLE_MFS8_RX                 PDL_OFF
#define PDL_DSTC_ENABLE_MFS8_TX                 PDL_OFF
#define PDL_DSTC_ENABLE_MFS9_RX                 PDL_OFF
#define PDL_DSTC_ENABLE_MFS9_TX                 PDL_OFF
#define PDL_DSTC_ENABLE_MFS10_RX                PDL_OFF
#define PDL_DSTC_ENABLE_MFS10_TX                PDL_OFF
#define PDL_DSTC_ENABLE_MFS11_RX                PDL_OFF
#define PDL_DSTC_ENABLE_MFS11_TX                PDL_OFF
#define PDL_DSTC_ENABLE_MFS12_RX                PDL_OFF
#define PDL_DSTC_ENABLE_MFS12_TX                PDL_OFF
#define PDL_DSTC_ENABLE_MFS13_RX                PDL_OFF
#define PDL_DSTC_ENABLE_MFS13_TX                PDL_OFF
#define PDL_DSTC_ENABLE_MFS14_RX                PDL_OFF
#define PDL_DSTC_ENABLE_MFS14_TX                PDL_OFF
#define PDL_DSTC_ENABLE_MFS15_RX                PDL_OFF
#define PDL_DSTC_ENABLE_MFS15_TX                PDL_OFF

// MFT
#define PDL_DSTC_ENABLE_MFT0_FRT0_PEAK          PDL_OFF
#define PDL_DSTC_ENABLE_MFT0_FRT0_ZERO          PDL_OFF
#define PDL_DSTC_ENABLE_MFT0_FRT1_PEAK          PDL_OFF
#define PDL_DSTC_ENABLE_MFT0_FRT1_ZERO          PDL_OFF
#define PDL_DSTC_ENABLE_MFT0_FRT2_PEAK          PDL_OFF
#define PDL_DSTC_ENABLE_MFT0_FRT2_ZERO          PDL_OFF
#define PDL_DSTC_ENABLE_MFT0_ICU0               PDL_OFF
#define PDL_DSTC_ENABLE_MFT0_ICU1               PDL_OFF
#define PDL_DSTC_ENABLE_MFT0_ICU2               PDL_OFF
#define PDL_DSTC_ENABLE_MFT0_ICU3               PDL_OFF
#define PDL_DSTC_ENABLE_MFT0_OCU0               PDL_OFF
#define PDL_DSTC_ENABLE_MFT0_OCU1               PDL_OFF
#define PDL_DSTC_ENABLE_MFT0_OCU2               PDL_OFF
#define PDL_DSTC_ENABLE_MFT0_OCU3               PDL_OFF
#define PDL_DSTC_ENABLE_MFT0_OCU4               PDL_OFF
#define PDL_DSTC_ENABLE_MFT0_OCU5               PDL_OFF
#define PDL_DSTC_ENABLE_MFT0_WFG10              PDL_OFF
#define PDL_DSTC_ENABLE_MFT0_WFG32              PDL_OFF
#define PDL_DSTC_ENABLE_MFT0_WFG54              PDL_OFF
#define PDL_DSTC_ENABLE_MFT1_FRT0_PEAK          PDL_OFF
#define PDL_DSTC_ENABLE_MFT1_FRT0_ZERO          PDL_OFF
#define PDL_DSTC_ENABLE_MFT1_FRT1_PEAK          PDL_OFF
#define PDL_DSTC_ENABLE_MFT1_FRT1_ZERO          PDL_OFF
#define PDL_DSTC_ENABLE_MFT1_FRT2_PEAK          PDL_OFF
#define PDL_DSTC_ENABLE_MFT1_FRT2_ZERO          PDL_OFF
#define PDL_DSTC_ENABLE_MFT1_ICU0               PDL_OFF
#define PDL_DSTC_ENABLE_MFT1_ICU1               PDL_OFF
#define PDL_DSTC_ENABLE_MFT1_ICU2               PDL_OFF
#define PDL_DSTC_ENABLE_MFT1_ICU3               PDL_OFF
#define PDL_DSTC_ENABLE_MFT1_OCU0               PDL_OFF
#define PDL_DSTC_ENABLE_MFT1_OCU1               PDL_OFF
#define PDL_DSTC_ENABLE_MFT1_OCU2               PDL_OFF
#define PDL_DSTC_ENABLE_MFT1_OCU3               PDL_OFF
#define PDL_DSTC_ENABLE_MFT1_OCU4               PDL_OFF
#define PDL_DSTC_ENABLE_MFT1_OCU5               PDL_OFF
#define PDL_DSTC_ENABLE_MFT1_WFG10              PDL_OFF
#define PDL_DSTC_ENABLE_MFT1_WFG32              PDL_OFF
#define PDL_DSTC_ENABLE_MFT1_WFG54              PDL_OFF
#define PDL_DSTC_ENABLE_MFT2_FRT0_PEAK          PDL_OFF
#define PDL_DSTC_ENABLE_MFT2_FRT0_ZERO          PDL_OFF
#define PDL_DSTC_ENABLE_MFT2_FRT1_PEAK          PDL_OFF
#define PDL_DSTC_ENABLE_MFT2_FRT1_ZERO          PDL_OFF
#define PDL_DSTC_ENABLE_MFT2_FRT2_PEAK          PDL_OFF
#define PDL_DSTC_ENABLE_MFT2_FRT2_ZERO          PDL_OFF
#define PDL_DSTC_ENABLE_MFT2_ICU0               PDL_OFF
#define PDL_DSTC_ENABLE_MFT2_ICU1               PDL_OFF
#define PDL_DSTC_ENABLE_MFT2_ICU2               PDL_OFF
#define PDL_DSTC_ENABLE_MFT2_ICU3               PDL_OFF
#define PDL_DSTC_ENABLE_MFT2_OCU0               PDL_OFF
#define PDL_DSTC_ENABLE_MFT2_OCU1               PDL_OFF
#define PDL_DSTC_ENABLE_MFT2_OCU2               PDL_OFF
#define PDL_DSTC_ENABLE_MFT2_OCU3               PDL_OFF
#define PDL_DSTC_ENABLE_MFT2_OCU4               PDL_OFF
#define PDL_DSTC_ENABLE_MFT2_OCU5               PDL_OFF
#define PDL_DSTC_ENABLE_MFT2_WFG10              PDL_OFF
#define PDL_DSTC_ENABLE_MFT2_WFG32              PDL_OFF
#define PDL_DSTC_ENABLE_MFT2_WFG54              PDL_OFF

// Programmable CRC
#define PDL_DSTC_ENABLE_PCRC                    PDL_OFF

// PPG
#define PDL_DSTC_ENABLE_PPG0                    PDL_OFF
#define PDL_DSTC_ENABLE_PPG2                    PDL_OFF
#define PDL_DSTC_ENABLE_PPG4                    PDL_OFF
#define PDL_DSTC_ENABLE_PPG8                    PDL_OFF
#define PDL_DSTC_ENABLE_PPG10                   PDL_OFF
#define PDL_DSTC_ENABLE_PPG12                   PDL_OFF
#define PDL_DSTC_ENABLE_PPG16                   PDL_OFF
#define PDL_DSTC_ENABLE_PPG18                   PDL_OFF
#define PDL_DSTC_ENABLE_PPG20                   PDL_OFF

// QPRC
#define PDL_DSTC_ENABLE_QPRC0_COUNT_INVERSION   PDL_OFF
#define PDL_DSTC_ENABLE_QPRC0_OUT_OF_RANGE      PDL_OFF
#define PDL_DSTC_ENABLE_QPRC0_PC_MATCH          PDL_OFF
#define PDL_DSTC_ENABLE_QPRC0_PC_MATCH_RC_MATCH PDL_OFF
#define PDL_DSTC_ENABLE_QPRC0_PC_RC_MATCH       PDL_OFF
#define PDL_DSTC_ENABLE_QPRC0_UFL_OFL_Z         PDL_OFF
#define PDL_DSTC_ENABLE_QPRC1_COUNT_INVERSION   PDL_OFF
#define PDL_DSTC_ENABLE_QPRC1_OUT_OF_RANGE      PDL_OFF
#define PDL_DSTC_ENABLE_QPRC1_PC_MATCH          PDL_OFF
#define PDL_DSTC_ENABLE_QPRC1_PC_MATCH_RC_MATCH PDL_OFF
#define PDL_DSTC_ENABLE_QPRC1_PC_RC_MATCH       PDL_OFF
#define PDL_DSTC_ENABLE_QPRC1_UFL_OFL_Z         PDL_OFF
#define PDL_DSTC_ENABLE_QPRC2_COUNT_INVERSION   PDL_OFF
#define PDL_DSTC_ENABLE_QPRC2_OUT_OF_RANGE      PDL_OFF
#define PDL_DSTC_ENABLE_QPRC2_PC_MATCH          PDL_OFF
#define PDL_DSTC_ENABLE_QPRC2_PC_MATCH_RC_MATCH PDL_OFF
#define PDL_DSTC_ENABLE_QPRC2_PC_RC_MATCH       PDL_OFF
#define PDL_DSTC_ENABLE_QPRC2_UFL_OFL_Z         PDL_OFF
#define PDL_DSTC_ENABLE_QPRC3_COUNT_INVERSION   PDL_OFF
#define PDL_DSTC_ENABLE_QPRC3_OUT_OF_RANGE      PDL_OFF
#define PDL_DSTC_ENABLE_QPRC3_PC_MATCH          PDL_OFF
#define PDL_DSTC_ENABLE_QPRC3_PC_MATCH_RC_MATCH PDL_OFF
#define PDL_DSTC_ENABLE_QPRC3_PC_RC_MATCH       PDL_OFF
#define PDL_DSTC_ENABLE_QPRC3_UFL_OFL_Z         PDL_OFF

// USB
#define PDL_DSTC_ENABLE_USB0_EP1                PDL_OFF
#define PDL_DSTC_ENABLE_USB0_EP2                PDL_OFF
#define PDL_DSTC_ENABLE_USB0_EP3                PDL_OFF
#define PDL_DSTC_ENABLE_USB0_EP4                PDL_OFF
#define PDL_DSTC_ENABLE_USB0_EP5                PDL_OFF
#define PDL_DSTC_ENABLE_USB1_EP1                PDL_OFF
#define PDL_DSTC_ENABLE_USB1_EP2                PDL_OFF
#define PDL_DSTC_ENABLE_USB1_EP3                PDL_OFF
#define PDL_DSTC_ENABLE_USB1_EP4                PDL_OFF
#define PDL_DSTC_ENABLE_USB1_EP5                PDL_OFF

// WC
#define PDL_DSTC_ENABLE_WC                      PDL_OFF

/**
 ******************************************************************************
 ** User Interrupt level settings
 **
 ** Possible values are 0 (high priority) to 15 (low priority)
 ******************************************************************************/
#if (PDL_MCU_CORE == PDL_FM4_CORE)    // FM4
// Analog Digital Converters
#define PDL_IRQ_LEVEL_ADC0              15u	// slow
#define PDL_IRQ_LEVEL_ADC1              2u	// fast adc must be higher than BT*
#define PDL_IRQ_LEVEL_ADC2              15u

// Base Timers
#define PDL_IRQ_LEVEL_BT0               4u
#define PDL_IRQ_LEVEL_BT1               4u
#define PDL_IRQ_LEVEL_BT2               3u	// adcfast timer has more priority
#define PDL_IRQ_LEVEL_BT3               3u
#define PDL_IRQ_LEVEL_BT4               15u
#define PDL_IRQ_LEVEL_BT5               15u
#define PDL_IRQ_LEVEL_BT6               15u
#define PDL_IRQ_LEVEL_BT7               15u
#define PDL_IRQ_LEVEL_BT8               15u
#define PDL_IRQ_LEVEL_BT9               15u
#define PDL_IRQ_LEVEL_BT10              15u
#define PDL_IRQ_LEVEL_BT11              15u
#define PDL_IRQ_LEVEL_BT12_13_14_15     15u

// CAN
#define PDL_IRQ_LEVEL_CAN0              15u
#define PDL_IRQ_LEVEL_CAN1_CANFD0       15u	// 6u

// Clock Stabilization Irq
#define PDL_IRQ_LEVEL_CLK               15u

// Clock Supervisor
#define PDL_IRQ_LEVEL_CSV               15u

// DMA
#define PDL_IRQ_LEVEL_DMA0              15u
#define PDL_IRQ_LEVEL_DMA1              15u
#define PDL_IRQ_LEVEL_DMA2              15u
#define PDL_IRQ_LEVEL_DMA3              15u
#define PDL_IRQ_LEVEL_DMA4              15u
#define PDL_IRQ_LEVEL_DMA5              15u
#define PDL_IRQ_LEVEL_DMA6              15u
#define PDL_IRQ_LEVEL_DMA7              15u

// DSTC
#define PDL_IRQ_LEVEL_DSTC              15u

// Dual Timer(s)
#define PDL_IRQ_LEVEL_DT0               15u

// Ethernet
// Please set IRQ level in emac_user.h

// External Bus Interface
#define PDL_IRQ_LEVEL_EXTIF             15u

// External Interrupts + NMI
#define PDL_IRQ_LEVEL_EXINT0            2u	// the same as fast ADC
#define PDL_IRQ_LEVEL_EXINT1            2u
#define PDL_IRQ_LEVEL_EXINT2            2u
#define PDL_IRQ_LEVEL_EXINT3            2u
#define PDL_IRQ_LEVEL_EXINT4            2u
#define PDL_IRQ_LEVEL_EXINT5            2u
#define PDL_IRQ_LEVEL_EXINT6            2u
#define PDL_IRQ_LEVEL_EXINT7            2u
#define PDL_IRQ_LEVEL_EXINT8            2u
#define PDL_IRQ_LEVEL_EXINT9            2u
#define PDL_IRQ_LEVEL_EXINT10           2u
#define PDL_IRQ_LEVEL_EXINT11           2u
#define PDL_IRQ_LEVEL_EXINT12           2u
#define PDL_IRQ_LEVEL_EXINT13           2u
#define PDL_IRQ_LEVEL_EXINT14           2u
#define PDL_IRQ_LEVEL_EXINT15           2u
#define PDL_IRQ_LEVEL_EXINT16_17_18_19  2u
#define PDL_IRQ_LEVEL_EXINT20_21_22_23  2u
#define PDL_IRQ_LEVEL_EXINT24_25_26_27  2u
#define PDL_IRQ_LEVEL_EXINT28_29_30_31  2u

// Hyper Bus Interface
#define PDL_IRQ_LEVEL_HBIF              15u

// High Speed Quad SPI
#define PDL_IRQ_LEVEL_HSSPI0            15u

// I2S and Programmable CRC
#define PDL_IRQ_LEVEL_I2S_PCRC          15u

// IC Card
#define PDL_IRQ_LEVEL_ICC0_1         15u  

// Low Voltage Detection Interrupt
#define PDL_IRQ_LEVEL_LVD               15u

// Multi Function Serial Interfaces
#define PDL_IRQ_LEVEL_MFS0_TX           15u
#define PDL_IRQ_LEVEL_MFS0_RX           15u
#define PDL_IRQ_LEVEL_MFS1_TX           15u
#define PDL_IRQ_LEVEL_MFS1_RX           15u
#define PDL_IRQ_LEVEL_MFS2_TX           15u
#define PDL_IRQ_LEVEL_MFS2_RX           15u
#define PDL_IRQ_LEVEL_MFS3_TX           15u
#define PDL_IRQ_LEVEL_MFS3_RX           15u
#define PDL_IRQ_LEVEL_MFS4_TX           15u
#define PDL_IRQ_LEVEL_MFS4_RX           15u
#define PDL_IRQ_LEVEL_MFS5_TX           15u
#define PDL_IRQ_LEVEL_MFS5_RX           15u
#define PDL_IRQ_LEVEL_MFS6_TX           15u
#define PDL_IRQ_LEVEL_MFS6_RX           15u
#define PDL_IRQ_LEVEL_MFS7_TX           15u
#define PDL_IRQ_LEVEL_MFS7_RX           15u
#define PDL_IRQ_LEVEL_MFS8_TX           15u
#define PDL_IRQ_LEVEL_MFS8_RX           15u
#define PDL_IRQ_LEVEL_MFS9_TX           15u
#define PDL_IRQ_LEVEL_MFS9_RX           15u
#define PDL_IRQ_LEVEL_MFS10_TX          15u
#define PDL_IRQ_LEVEL_MFS10_RX          15u
#define PDL_IRQ_LEVEL_MFS11_TX          15u
#define PDL_IRQ_LEVEL_MFS11_RX          15u
#define PDL_IRQ_LEVEL_MFS12_TX          15u
#define PDL_IRQ_LEVEL_MFS12_RX          15u
#define PDL_IRQ_LEVEL_MFS13_TX          15u
#define PDL_IRQ_LEVEL_MFS13_RX          15u
#define PDL_IRQ_LEVEL_MFS14_TX          15u
#define PDL_IRQ_LEVEL_MFS14_RX          15u
#define PDL_IRQ_LEVEL_MFS15_TX          15u
#define PDL_IRQ_LEVEL_MFS15_RX          15u

// Multi Function Timer Interrupts
#define PDL_IRQ_LEVEL_MFT0_FRT          15u
#define PDL_IRQ_LEVEL_MFT0_OCU          15u
#define PDL_IRQ_LEVEL_MFT0_WFG          15u
#define PDL_IRQ_LEVEL_MFT0_ICU          15u

#define PDL_IRQ_LEVEL_MFT1_FRT          15u
#define PDL_IRQ_LEVEL_MFT1_OCU          15u
#define PDL_IRQ_LEVEL_MFT1_WFG          15u
#define PDL_IRQ_LEVEL_MFT1_ICU          15u

#define PDL_IRQ_LEVEL_MFT2_FRT          15u
#define PDL_IRQ_LEVEL_MFT2_OCU          15u
#define PDL_IRQ_LEVEL_MFT2_WFG          15u
#define PDL_IRQ_LEVEL_MFT2_ICU          15u

// NMI
#define PDL_IRQ_LEVEL_NMI               15u

// PPG Interrupts
#define PDL_IRQ_LEVEL_PPG00_02_04       15u
#define PDL_IRQ_LEVEL_PPG08_10_12       15u
#define PDL_IRQ_LEVEL_PPG16_18_20       15u

// Quad Decoder and Revolution Counter
#define PDL_IRQ_LEVEL_QPRC0             15u
#define PDL_IRQ_LEVEL_QPRC1             15u
#define PDL_IRQ_LEVEL_QPRC2             15u
#define PDL_IRQ_LEVEL_QPRC3             15u

// Real Time Clock
#define PDL_IRQ_LEVEL_RTC0              15u

// SD Host
#define PDL_IRQ_LEVEL_SD                15u

// USB
#define PDL_IRQ_LEVEL_USB0              6u
#define PDL_IRQ_LEVEL_USB1              6u

// Watch Counter
#define PDL_IRQ_LEVEL_WC0               15u   
   
// Watchdog Timers
#define PDL_IRQ_LEVEL_HWWDG             15u
#define PDL_IRQ_LEVEL_SWWDG             15u

#else
#error MCU core not found!
#endif
/**
 ******************************************************************************
 ** PDL resource activiation check
 **
 ** \note It does not check, if a device has actually all instances available!
 **
 ******************************************************************************/
// Activate code in adc.c if one or more are set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_ADC0 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_ADC1 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_ADC2 == PDL_ON)
    #define PDL_PERIPHERAL_ADC_ACTIVE
#endif

// Activate code in aes.c if set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_AES == PDL_ON)
    #define PDL_PERIPHERAL_AES_ACTIVE
#endif

// Activate code in bt.c if one or more are set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_BT0 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_BT1 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_BT2 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_BT3 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_BT4 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_BT5 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_BT6 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_BT7 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_BT8 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_BT9 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_BT10 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_BT11 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_BT12 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_BT13 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_BT14 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_BT15 == PDL_ON)
    #define PDL_PERIPHERAL_BT_ACTIVE
#endif

// Activate code in can.c if set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_CAN0 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_CAN1 == PDL_ON)
    #define PDL_PERIPHERAL_CAN_ACTIVE
#endif

// Activate code in canfd.c if set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_CANFD0 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_CANFD1 == PDL_ON)
    #define PDL_PERIPHERAL_CANFD_ACTIVE
#endif

// Activate code in crc.c if set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_CRC0 == PDL_ON)
    #define PDL_PERIPHERAL_CRC_ACTIVE
#endif

// Activate code in clk.c if set to PDL_ON or WC enabled
#if (PDL_PERIPHERAL_ENABLE_CLK == PDL_ON)
    #define PDL_PERIPHERAL_CLK_ACTIVE
#endif

// Activate code in crtrim.c if set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_CR == PDL_ON)
    #define PDL_PERIPHERAL_CR_ACTIVE
#endif

// Activate code in csv.c if set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_CSV == PDL_ON)
    #define PDL_PERIPHERAL_CSV_ACTIVE
#endif

// Activate code in dac.c if one or more are set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_DAC0 == PDL_ON)
    #define PDL_PERIPHERAL_DAC_ACTIVE
#endif

// Activate code for dma.c
#if  (PDL_PERIPHERAL_ENABLE_DMA0 == PDL_ON) || \
     (PDL_PERIPHERAL_ENABLE_DMA1 == PDL_ON) || \
     (PDL_PERIPHERAL_ENABLE_DMA2 == PDL_ON) || \
     (PDL_PERIPHERAL_ENABLE_DMA3 == PDL_ON) || \
     (PDL_PERIPHERAL_ENABLE_DMA4 == PDL_ON) || \
     (PDL_PERIPHERAL_ENABLE_DMA5 == PDL_ON) || \
     (PDL_PERIPHERAL_ENABLE_DMA6 == PDL_ON) || \
     (PDL_PERIPHERAL_ENABLE_DMA7 == PDL_ON)
    #define PDL_PERIPHERAL_DMA_ACTIVE
#endif

// Activate code in dstc.c if one or more are set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_DSTC == PDL_ON)
    #define PDL_PERIPHERAL_DSTC_ACTIVE
#endif

// Activate code in dt.c if one or more are set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_DT0 == PDL_ON)
    #define PDL_PERIPHERAL_DT_ACTIVE
#endif

// Activate code in exint.c if one or more are set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_EXINT0           == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT1           == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT2           == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT3           == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT4           == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT5           == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT6           == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT7           == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT8           == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT9           == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT10          == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT11          == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT12          == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT13          == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT14          == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT15          == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT16          == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT17          == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT18          == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT19          == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT20          == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT21          == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT22          == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT23          == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT24          == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT25          == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT26          == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT27          == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT28          == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT29          == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT30          == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_EXINT31          == PDL_ON)
    #define PDL_PERIPHERAL_EXINT_ACTIVE
#endif

// Activate code in extif.c if set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_EXTIF        == PDL_ON)
    #define PDL_PERIPHERAL_EXTIF_ACTIVE
#endif

// Activate code in dualflash.c if set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_DUAL_FLASH        == PDL_ON)
    #define PDL_PERIPHERAL_DUAL_FLASH_ACTIVE
#endif

// Activate code in mainflash.c if set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_MAIN_FLASH        == PDL_ON)
    #define PDL_PERIPHERAL_MAIN_FLASH_ACTIVE
#endif

// Activate code in workflash.c if set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_WORK_FLASH        == PDL_ON)
    #define PDL_PERIPHERAL_WORK_FLASH_ACTIVE
#endif

// Activate code in gpio.h if set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_GPIO == PDL_ON)
    #define PDL_PERIPHERAL_GPIO_ACTIVE
#endif

// Activate code in hbif.h if set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_HBIF == PDL_ON)
    #define PDL_PERIPHERAL_HBIF_ACTIVE
#endif

// Activate code in hsspi.h if set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_HSSPI0 == PDL_ON)
    #define PDL_PERIPHERAL_HSSPI_ACTIVE
#endif

// Activate code in i2cs.h if set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_I2CS0 == PDL_ON)
    #define PDL_PERIPHERAL_I2CS_ACTIVE
#endif      
      
// Activate code in icc.h if set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_ICC0 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_ICC1 == PDL_ON)
    #define PDL_PERIPHERAL_ICC_ACTIVE
#endif

// Activate code in i2s.h if set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_I2S0 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_I2S1 == PDL_ON)
    #define PDL_PERIPHERAL_I2S_ACTIVE
#endif

// Activate code in i2sl.h if set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_I2SL0 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_I2SL1 == PDL_ON)
    #define PDL_PERIPHERAL_I2SL_ACTIVE
#endif

// Activate code in lcd.c if set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_LCD == PDL_ON)
    #define PDL_PERIPHERAL_LCD_ACTIVE
#endif

// Activate code in lpm.c if set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_LPM == PDL_ON)
    #define PDL_PERIPHERAL_LPM_ACTIVE
#endif

// Activate code in lvd.c if set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_LVD == PDL_ON)
    #define PDL_PERIPHERAL_LVD_ACTIVE
#endif

// Activate code in mfs.c if one or more are set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_MFS0  == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFS1  == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFS2  == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFS3  == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFS4  == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFS5  == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFS6  == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFS7  == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFS8  == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFS9  == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFS10 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFS11 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFS12 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFS13 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFS14 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFS15 == PDL_ON)
    #define PDL_PERIPHERAL_MFS_ACTIVE
#endif

// Activate code in mft_frt.c if one or more are set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_MFT0_FRT == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFT1_FRT == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFT2_FRT == PDL_ON)
    #define PDL_PERIPHERAL_MFT_FRT_ACTIVE
#endif

// Activate code in mft_ocu.c if one or more are set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_MFT0_OCU == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFT1_OCU == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFT2_OCU == PDL_ON)
    #define PDL_PERIPHERAL_MFT_OCU_ACTIVE
#endif

// Activate code in mft_wfg.c if one or more are set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_MFT0_WFG == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFT1_WFG == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFT2_WFG == PDL_ON)
    #define PDL_PERIPHERAL_MFT_WFG_ACTIVE
#endif

// Activate code in mft_icu.c if one or more are set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_MFT0_ICU == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFT1_ICU == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFT2_ICU == PDL_ON)
    #define PDL_PERIPHERAL_MFT_ICU_ACTIVE
#endif

// Activate code in mft_adcmp.c if one or more are set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_MFT0_ADCMP == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFT1_ADCMP == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_MFT2_ADCMP == PDL_ON)
    #define PDL_PERIPHERAL_MFT_ADCMP_ACTIVE
#endif

// Activate NMI code in exint.c if one or more are set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_NMI == PDL_ON)
    #define PDL_PERIPHERAL_NMI_ACTIVE
#endif

// Activate code in pcrc.c if one or more are set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_PCRC == PDL_ON)
    #define PDL_PERIPHERAL_PCRC_ACTIVE
#endif

// Activate code in ppg.c if one or more are set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_PPG == PDL_ON)
    #define PDL_PERIPHERAL_PPG_ACTIVE
#endif

// Activate code in qprc.c if set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_QPRC0 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_QPRC1 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_QPRC2 == PDL_ON)
    #define PDL_PERIPHERAL_QPRC_ACTIVE
#endif

// Activate code in rc.c if set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_RC0 == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_RC1 == PDL_ON)
    #define PDL_PERIPHERAL_RC_ACTIVE
#endif	

// Reset Cause
#if (PDL_PERIPHERAL_ENABLE_RESET == PDL_ON)
    #define PDL_PERIPHERAL_RESET_ACTIVE
#endif

// Real Time Clock
#if (PDL_PERIPHERAL_ENABLE_RTC0 == PDL_ON)
    #define PDL_PERIPHERAL_RTC_ACTIVE
#endif

// SD
#if (PDL_PERIPHERAL_ENABLE_SD0 == PDL_ON)
    #define PDL_PERIPHERAL_SD_ACTIVE
#endif

// Unique ID
#if (PDL_PERIPHERAL_ENABLE_UID == PDL_ON)
    #define PDL_PERIPHERAL_UID_ACTIVE
#endif

// Activate code in vbat.c if one or more are set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_VBAT == PDL_ON)
    #define PDL_PERIPHERAL_VBAT_ACTIVE
#endif

// Activate code in wc.c if one or more are set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_WC0 == PDL_ON)
    #define PDL_PERIPHERAL_WC_ACTIVE
#endif

// Activate code in wdg.c is set to PDL_ON
#if (PDL_PERIPHERAL_ENABLE_HWWDG == PDL_ON) || \
    (PDL_PERIPHERAL_ENABLE_SWWDG == PDL_ON)
    #define PDL_PERIPHERAL_WDG_ACTIVE
#endif

/**
 ******************************************************************************
 ** \brief PDL utility enable setting
 ******************************************************************************/
// Printf/Scanf via UART function
#define PDL_UTILITY_ENABLE_UART_PRINTF              PDL_OFF
#define PDL_UTILITY_ENABLE_UART_SCANF               PDL_OFF

// AT24CXX(EEPROM) driver
#define PDL_UTILITY_ENABLE_I2C_POLLING_AT24CXX      PDL_OFF
#define PDL_UTILITY_ENABLE_I2C_IRQ_AT24CXX          PDL_OFF

// S25FL164K(SPI Flash) QSPI access driver with or without using interrupt
#define PDL_UTILITY_ENABLE_QSPI_POLLING_S25FL164K   PDL_OFF
#define PDL_UTILITY_ENABLE_QSPI_IRQ_S25FL164K       PDL_OFF

// S26KL512S (Hyper Bus Flash) driver
#define PDL_UTILITY_ENABLE_HBIF_S26KL512S           PDL_OFF

// WM8731(I2S Codec) driver
#define PDL_UTILITY_ENABLE_I2S_CODEC_WM8731         PDL_OFF

// SV6P1615(External Bus SRAM) driver
#define PDL_UTILITY_ENABLE_EXTIF_SV6P1615           PDL_OFF

// IS42S16800(SDRAM) driver
#define PDL_UTILITY_ENABLE_EXTIF_IS42S16800         PDL_OFF

// HY57V281620(SDRAM) driver
#define PDL_UTILITY_ENABLE_EXTIF_HY57V281620        PDL_OFF

// K9F5608U0D (Nand Flash) driver
#define PDL_UTILITY_ENABLE_EXTIF_K9F5608U0D         PDL_OFF

// S34ML01G (Nand Flash) driver
#define PDL_UTILITY_ENABLE_EXTIF_S34ML01G           PDL_OFF

// TSDH1188 (Segment LCD) driver
#define PDL_UTILITY_ENABLE_SEG_LCD_TSDH1188         PDL_OFF

// CL010-7031-04(Segment LCD) driver
#define PDL_UTILITY_ENABLE_SEG_LCD_CL0107031        PDL_OFF

/**
 ******************************************************************************
 ** \brief Enable/disable print on the terminal window
 ******************************************************************************/
#define DEBUG_PRINT

#endif // __PDL_USER_H__

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
