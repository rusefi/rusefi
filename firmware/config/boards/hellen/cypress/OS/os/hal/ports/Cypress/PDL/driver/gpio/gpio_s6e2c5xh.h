/*************************************************************************************
* Copyright (C) 2013-2015, Cypress Semiconductor Corporation. All rights reserved.    
*                                                                                     
* This software, including source code, documentation and related                     
* materials ( "Software" ), is owned by Cypress Semiconductor                         
* Corporation ( "Cypress" ) and is protected by and subject to worldwide              
* patent protection (United States and foreign), United States copyright              
* laws and international treaty provisions. Therefore, you may use this               
* Software only as provided in the license agreement accompanying the                 
* software package from which you obtained this Software ( "EULA" ).                  
* If no EULA applies, Cypress hereby grants you a personal, nonexclusive,             
* non-transferable license to copy, modify, and compile the                           
* Software source code solely for use in connection with Cypress's                    
* integrated circuit products. Any reproduction, modification, translation,           
* compilation, or representation of this Software except as specified                 
* above is prohibited without the express written permission of Cypress.              
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO                                
* WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING,                                
* BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED                                        
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A                                     
* PARTICULAR PURPOSE. Cypress reserves the right to make                              
* changes to the Software without notice. Cypress does not assume any                 
* liability arising out of the application or use of the Software or any              
* product or circuit described in the Software. Cypress does not                      
* authorize its products for use in any products where a malfunction or               
* failure of the Cypress product may reasonably be expected to result in              
* significant property damage, injury or death ( "High Risk Product" ). By            
* including Cypress's product in a High Risk Product, the manufacturer                
* of such system or application assumes all risk of such use and in doing             
* so agrees to indemnify Cypress against all liability.                               
*/
/************************************************************************************/
/** \file gpio_s6e2c5xh.h
 **
 ** Header file for S6E2C5XH GPIO functions, included in gpio.h
 **
 ** History:
 **   - 2015-12-04  2.0  NOSU	Clean ALL FM Series pin files
 **
 ** Timestamp:
 **   - 2015-12-16 18:30:36  Auto created by GpioHeaderGenerator Rev 1.0.0
 **
 ******************************************************************************/

#ifndef __GPIO_S6E2C5XH_H__
#define __GPIO_S6E2C5XH_H__

#include <stdint.h>

#define PINCONFIG_SET_REG(pinreg,pos,width,value)    \
          ((pinreg) = ((pinreg) & ~(((1u<<(width))-1u)<<(pos)) | \
          ((value) << (pos))))

/******************************************************************************
   GPIO
*******************************************************************************/

/*---- GPIO bit P00 ----*/
#define GPIO1PIN_P00_GET                ( bFM_GPIO_PDIR0_P0 )

#define GPIO1PIN_P00_PUT(v)             ( bFM_GPIO_PDOR0_P0=(v) )

#define GPIO1PIN_P00_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P00_INITIN(v) \
                                                           : GPIO1PIN_P00_INITOUT(v) )

#define GPIO1PIN_P00_INITIN(v)          do{ \
                                            bFM_GPIO_PCR0_P0=(v).bPullup; \
                                            bFM_GPIO_DDR0_P0=0u; \
                                            bFM_GPIO_PFR0_P0=0u; }while(0u)

#define GPIO1PIN_P00_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR0_P0=(v).bInitVal; \
                                            bFM_GPIO_DDR0_P0=1u; \
                                            bFM_GPIO_PFR0_P0=0u; }while(0u)

/*---- GPIO bit NP00 ----*/
#define GPIO1PIN_NP00_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR0_P0)) )

#define GPIO1PIN_NP00_PUT(v)            ( bFM_GPIO_PDOR0_P0=(uint32_t)(!(v)) )

#define GPIO1PIN_NP00_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP00_INITIN(v) \
                                                           : GPIO1PIN_NP00_INITOUT(v) )

#define GPIO1PIN_NP00_INITIN(v)         do{ \
                                            bFM_GPIO_PCR0_P0=(v).bPullup; \
                                            bFM_GPIO_DDR0_P0=0u; \
                                            bFM_GPIO_PFR0_P0=0u; }while(0u)

#define GPIO1PIN_NP00_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR0_P0=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR0_P0=1u; \
                                            bFM_GPIO_PFR0_P0=0u; }while(0u)

/*---- GPIO bit P01 ----*/
#define GPIO1PIN_P01_GET                ( bFM_GPIO_PDIR0_P1 )

#define GPIO1PIN_P01_PUT(v)             ( bFM_GPIO_PDOR0_P1=(v) )

#define GPIO1PIN_P01_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P01_INITIN(v) \
                                                           : GPIO1PIN_P01_INITOUT(v) )

#define GPIO1PIN_P01_INITIN(v)          do{ \
                                            bFM_GPIO_PCR0_P1=(v).bPullup; \
                                            bFM_GPIO_DDR0_P1=0u; \
                                            bFM_GPIO_PFR0_P1=0u; }while(0u)

#define GPIO1PIN_P01_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR0_P1=(v).bInitVal; \
                                            bFM_GPIO_DDR0_P1=1u; \
                                            bFM_GPIO_PFR0_P1=0u; }while(0u)

/*---- GPIO bit NP01 ----*/
#define GPIO1PIN_NP01_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR0_P1)) )

#define GPIO1PIN_NP01_PUT(v)            ( bFM_GPIO_PDOR0_P1=(uint32_t)(!(v)) )

#define GPIO1PIN_NP01_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP01_INITIN(v) \
                                                           : GPIO1PIN_NP01_INITOUT(v) )

#define GPIO1PIN_NP01_INITIN(v)         do{ \
                                            bFM_GPIO_PCR0_P1=(v).bPullup; \
                                            bFM_GPIO_DDR0_P1=0u; \
                                            bFM_GPIO_PFR0_P1=0u; }while(0u)

#define GPIO1PIN_NP01_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR0_P1=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR0_P1=1u; \
                                            bFM_GPIO_PFR0_P1=0u; }while(0u)

/*---- GPIO bit P02 ----*/
#define GPIO1PIN_P02_GET                ( bFM_GPIO_PDIR0_P2 )

#define GPIO1PIN_P02_PUT(v)             ( bFM_GPIO_PDOR0_P2=(v) )

#define GPIO1PIN_P02_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P02_INITIN(v) \
                                                           : GPIO1PIN_P02_INITOUT(v) )

#define GPIO1PIN_P02_INITIN(v)          do{ \
                                            bFM_GPIO_PCR0_P2=(v).bPullup; \
                                            bFM_GPIO_DDR0_P2=0u; \
                                            bFM_GPIO_PFR0_P2=0u; }while(0u)

#define GPIO1PIN_P02_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR0_P2=(v).bInitVal; \
                                            bFM_GPIO_DDR0_P2=1u; \
                                            bFM_GPIO_PFR0_P2=0u; }while(0u)

/*---- GPIO bit NP02 ----*/
#define GPIO1PIN_NP02_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR0_P2)) )

#define GPIO1PIN_NP02_PUT(v)            ( bFM_GPIO_PDOR0_P2=(uint32_t)(!(v)) )

#define GPIO1PIN_NP02_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP02_INITIN(v) \
                                                           : GPIO1PIN_NP02_INITOUT(v) )

#define GPIO1PIN_NP02_INITIN(v)         do{ \
                                            bFM_GPIO_PCR0_P2=(v).bPullup; \
                                            bFM_GPIO_DDR0_P2=0u; \
                                            bFM_GPIO_PFR0_P2=0u; }while(0u)

#define GPIO1PIN_NP02_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR0_P2=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR0_P2=1u; \
                                            bFM_GPIO_PFR0_P2=0u; }while(0u)

/*---- GPIO bit P03 ----*/
#define GPIO1PIN_P03_GET                ( bFM_GPIO_PDIR0_P3 )

#define GPIO1PIN_P03_PUT(v)             ( bFM_GPIO_PDOR0_P3=(v) )

#define GPIO1PIN_P03_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P03_INITIN(v) \
                                                           : GPIO1PIN_P03_INITOUT(v) )

#define GPIO1PIN_P03_INITIN(v)          do{ \
                                            bFM_GPIO_PCR0_P3=(v).bPullup; \
                                            bFM_GPIO_DDR0_P3=0u; \
                                            bFM_GPIO_PFR0_P3=0u; }while(0u)

#define GPIO1PIN_P03_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR0_P3=(v).bInitVal; \
                                            bFM_GPIO_DDR0_P3=1u; \
                                            bFM_GPIO_PFR0_P3=0u; }while(0u)

/*---- GPIO bit NP03 ----*/
#define GPIO1PIN_NP03_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR0_P3)) )

#define GPIO1PIN_NP03_PUT(v)            ( bFM_GPIO_PDOR0_P3=(uint32_t)(!(v)) )

#define GPIO1PIN_NP03_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP03_INITIN(v) \
                                                           : GPIO1PIN_NP03_INITOUT(v) )

#define GPIO1PIN_NP03_INITIN(v)         do{ \
                                            bFM_GPIO_PCR0_P3=(v).bPullup; \
                                            bFM_GPIO_DDR0_P3=0u; \
                                            bFM_GPIO_PFR0_P3=0u; }while(0u)

#define GPIO1PIN_NP03_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR0_P3=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR0_P3=1u; \
                                            bFM_GPIO_PFR0_P3=0u; }while(0u)

/*---- GPIO bit P04 ----*/
#define GPIO1PIN_P04_GET                ( bFM_GPIO_PDIR0_P4 )

#define GPIO1PIN_P04_PUT(v)             ( bFM_GPIO_PDOR0_P4=(v) )

#define GPIO1PIN_P04_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P04_INITIN(v) \
                                                           : GPIO1PIN_P04_INITOUT(v) )

#define GPIO1PIN_P04_INITIN(v)          do{ \
                                            bFM_GPIO_PCR0_P4=(v).bPullup; \
                                            bFM_GPIO_DDR0_P4=0u; \
                                            bFM_GPIO_PFR0_P4=0u; }while(0u)

#define GPIO1PIN_P04_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR0_P4=(v).bInitVal; \
                                            bFM_GPIO_DDR0_P4=1u; \
                                            bFM_GPIO_PFR0_P4=0u; }while(0u)

/*---- GPIO bit NP04 ----*/
#define GPIO1PIN_NP04_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR0_P4)) )

#define GPIO1PIN_NP04_PUT(v)            ( bFM_GPIO_PDOR0_P4=(uint32_t)(!(v)) )

#define GPIO1PIN_NP04_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP04_INITIN(v) \
                                                           : GPIO1PIN_NP04_INITOUT(v) )

#define GPIO1PIN_NP04_INITIN(v)         do{ \
                                            bFM_GPIO_PCR0_P4=(v).bPullup; \
                                            bFM_GPIO_DDR0_P4=0u; \
                                            bFM_GPIO_PFR0_P4=0u; }while(0u)

#define GPIO1PIN_NP04_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR0_P4=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR0_P4=1u; \
                                            bFM_GPIO_PFR0_P4=0u; }while(0u)

/*---- GPIO bit P08 ----*/
#define GPIO1PIN_P08_GET                ( bFM_GPIO_PDIR0_P8 )

#define GPIO1PIN_P08_PUT(v)             ( bFM_GPIO_PDOR0_P8=(v) )

#define GPIO1PIN_P08_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P08_INITIN(v) \
                                                           : GPIO1PIN_P08_INITOUT(v) )

#define GPIO1PIN_P08_INITIN(v)          do{ \
                                            bFM_GPIO_PCR0_P8=(v).bPullup; \
                                            bFM_GPIO_DDR0_P8=0u; \
                                            bFM_GPIO_PFR0_P8=0u; }while(0u)

#define GPIO1PIN_P08_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR0_P8=(v).bInitVal; \
                                            bFM_GPIO_DDR0_P8=1u; \
                                            bFM_GPIO_PFR0_P8=0u; }while(0u)

/*---- GPIO bit NP08 ----*/
#define GPIO1PIN_NP08_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR0_P8)) )

#define GPIO1PIN_NP08_PUT(v)            ( bFM_GPIO_PDOR0_P8=(uint32_t)(!(v)) )

#define GPIO1PIN_NP08_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP08_INITIN(v) \
                                                           : GPIO1PIN_NP08_INITOUT(v) )

#define GPIO1PIN_NP08_INITIN(v)         do{ \
                                            bFM_GPIO_PCR0_P8=(v).bPullup; \
                                            bFM_GPIO_DDR0_P8=0u; \
                                            bFM_GPIO_PFR0_P8=0u; }while(0u)

#define GPIO1PIN_NP08_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR0_P8=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR0_P8=1u; \
                                            bFM_GPIO_PFR0_P8=0u; }while(0u)

/*---- GPIO bit P09 ----*/
#define GPIO1PIN_P09_GET                ( bFM_GPIO_PDIR0_P9 )

#define GPIO1PIN_P09_PUT(v)             ( bFM_GPIO_PDOR0_P9=(v) )

#define GPIO1PIN_P09_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P09_INITIN(v) \
                                                           : GPIO1PIN_P09_INITOUT(v) )

#define GPIO1PIN_P09_INITIN(v)          do{ \
                                            bFM_GPIO_PCR0_P9=(v).bPullup; \
                                            bFM_GPIO_DDR0_P9=0u; \
                                            bFM_GPIO_PFR0_P9=0u; }while(0u)

#define GPIO1PIN_P09_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR0_P9=(v).bInitVal; \
                                            bFM_GPIO_DDR0_P9=1u; \
                                            bFM_GPIO_PFR0_P9=0u; }while(0u)

/*---- GPIO bit NP09 ----*/
#define GPIO1PIN_NP09_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR0_P9)) )

#define GPIO1PIN_NP09_PUT(v)            ( bFM_GPIO_PDOR0_P9=(uint32_t)(!(v)) )

#define GPIO1PIN_NP09_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP09_INITIN(v) \
                                                           : GPIO1PIN_NP09_INITOUT(v) )

#define GPIO1PIN_NP09_INITIN(v)         do{ \
                                            bFM_GPIO_PCR0_P9=(v).bPullup; \
                                            bFM_GPIO_DDR0_P9=0u; \
                                            bFM_GPIO_PFR0_P9=0u; }while(0u)

#define GPIO1PIN_NP09_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR0_P9=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR0_P9=1u; \
                                            bFM_GPIO_PFR0_P9=0u; }while(0u)

/*---- GPIO bit P0A ----*/
#define GPIO1PIN_P0A_GET                ( bFM_GPIO_PDIR0_PA )

#define GPIO1PIN_P0A_PUT(v)             ( bFM_GPIO_PDOR0_PA=(v) )

#define GPIO1PIN_P0A_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P0A_INITIN(v) \
                                                           : GPIO1PIN_P0A_INITOUT(v) )

#define GPIO1PIN_P0A_INITIN(v)          do{ \
                                            bFM_GPIO_PCR0_PA=(v).bPullup; \
                                            bFM_GPIO_DDR0_PA=0u; \
                                            bFM_GPIO_PFR0_PA=0u; }while(0u)

#define GPIO1PIN_P0A_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR0_PA=(v).bInitVal; \
                                            bFM_GPIO_DDR0_PA=1u; \
                                            bFM_GPIO_PFR0_PA=0u; }while(0u)

/*---- GPIO bit NP0A ----*/
#define GPIO1PIN_NP0A_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR0_PA)) )

#define GPIO1PIN_NP0A_PUT(v)            ( bFM_GPIO_PDOR0_PA=(uint32_t)(!(v)) )

#define GPIO1PIN_NP0A_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP0A_INITIN(v) \
                                                           : GPIO1PIN_NP0A_INITOUT(v) )

#define GPIO1PIN_NP0A_INITIN(v)         do{ \
                                            bFM_GPIO_PCR0_PA=(v).bPullup; \
                                            bFM_GPIO_DDR0_PA=0u; \
                                            bFM_GPIO_PFR0_PA=0u; }while(0u)

#define GPIO1PIN_NP0A_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR0_PA=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR0_PA=1u; \
                                            bFM_GPIO_PFR0_PA=0u; }while(0u)

/*---- GPIO bit P10 ----*/
#define GPIO1PIN_P10_GET                ( bFM_GPIO_PDIR1_P0 )

#define GPIO1PIN_P10_PUT(v)             ( bFM_GPIO_PDOR1_P0=(v) )

#define GPIO1PIN_P10_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P10_INITIN(v) \
                                                           : GPIO1PIN_P10_INITOUT(v) )

#define GPIO1PIN_P10_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN00=0u; \
                                            bFM_GPIO_PCR1_P0=(v).bPullup; \
                                            bFM_GPIO_DDR1_P0=0u; \
                                            bFM_GPIO_PFR1_P0=0u; }while(0u)

#define GPIO1PIN_P10_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN00=0u; \
                                            bFM_GPIO_PDOR1_P0=(v).bInitVal; \
                                            bFM_GPIO_DDR1_P0=1u; \
                                            bFM_GPIO_PFR1_P0=0u; }while(0u)

/*---- GPIO bit NP10 ----*/
#define GPIO1PIN_NP10_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_P0)) )

#define GPIO1PIN_NP10_PUT(v)            ( bFM_GPIO_PDOR1_P0=(uint32_t)(!(v)) )

#define GPIO1PIN_NP10_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP10_INITIN(v) \
                                                           : GPIO1PIN_NP10_INITOUT(v) )

#define GPIO1PIN_NP10_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN00=0u; \
                                            bFM_GPIO_PCR1_P0=(v).bPullup; \
                                            bFM_GPIO_DDR1_P0=0u; \
                                            bFM_GPIO_PFR1_P0=0u; }while(0u)

#define GPIO1PIN_NP10_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN00=0u; \
                                            bFM_GPIO_PDOR1_P0=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_P0=1u; \
                                            bFM_GPIO_PFR1_P0=0u; }while(0u)

/*---- GPIO bit P11 ----*/
#define GPIO1PIN_P11_GET                ( bFM_GPIO_PDIR1_P1 )

#define GPIO1PIN_P11_PUT(v)             ( bFM_GPIO_PDOR1_P1=(v) )

#define GPIO1PIN_P11_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P11_INITIN(v) \
                                                           : GPIO1PIN_P11_INITOUT(v) )

#define GPIO1PIN_P11_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN01=0u; \
                                            bFM_GPIO_PCR1_P1=(v).bPullup; \
                                            bFM_GPIO_DDR1_P1=0u; \
                                            bFM_GPIO_PFR1_P1=0u; }while(0u)

#define GPIO1PIN_P11_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN01=0u; \
                                            bFM_GPIO_PDOR1_P1=(v).bInitVal; \
                                            bFM_GPIO_DDR1_P1=1u; \
                                            bFM_GPIO_PFR1_P1=0u; }while(0u)

/*---- GPIO bit NP11 ----*/
#define GPIO1PIN_NP11_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_P1)) )

#define GPIO1PIN_NP11_PUT(v)            ( bFM_GPIO_PDOR1_P1=(uint32_t)(!(v)) )

#define GPIO1PIN_NP11_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP11_INITIN(v) \
                                                           : GPIO1PIN_NP11_INITOUT(v) )

#define GPIO1PIN_NP11_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN01=0u; \
                                            bFM_GPIO_PCR1_P1=(v).bPullup; \
                                            bFM_GPIO_DDR1_P1=0u; \
                                            bFM_GPIO_PFR1_P1=0u; }while(0u)

#define GPIO1PIN_NP11_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN01=0u; \
                                            bFM_GPIO_PDOR1_P1=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_P1=1u; \
                                            bFM_GPIO_PFR1_P1=0u; }while(0u)

/*---- GPIO bit P12 ----*/
#define GPIO1PIN_P12_GET                ( bFM_GPIO_PDIR1_P2 )

#define GPIO1PIN_P12_PUT(v)             ( bFM_GPIO_PDOR1_P2=(v) )

#define GPIO1PIN_P12_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P12_INITIN(v) \
                                                           : GPIO1PIN_P12_INITOUT(v) )

#define GPIO1PIN_P12_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN02=0u; \
                                            bFM_GPIO_PCR1_P2=(v).bPullup; \
                                            bFM_GPIO_DDR1_P2=0u; \
                                            bFM_GPIO_PFR1_P2=0u; }while(0u)

#define GPIO1PIN_P12_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN02=0u; \
                                            bFM_GPIO_PDOR1_P2=(v).bInitVal; \
                                            bFM_GPIO_DDR1_P2=1u; \
                                            bFM_GPIO_PFR1_P2=0u; }while(0u)

/*---- GPIO bit NP12 ----*/
#define GPIO1PIN_NP12_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_P2)) )

#define GPIO1PIN_NP12_PUT(v)            ( bFM_GPIO_PDOR1_P2=(uint32_t)(!(v)) )

#define GPIO1PIN_NP12_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP12_INITIN(v) \
                                                           : GPIO1PIN_NP12_INITOUT(v) )

#define GPIO1PIN_NP12_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN02=0u; \
                                            bFM_GPIO_PCR1_P2=(v).bPullup; \
                                            bFM_GPIO_DDR1_P2=0u; \
                                            bFM_GPIO_PFR1_P2=0u; }while(0u)

#define GPIO1PIN_NP12_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN02=0u; \
                                            bFM_GPIO_PDOR1_P2=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_P2=1u; \
                                            bFM_GPIO_PFR1_P2=0u; }while(0u)

/*---- GPIO bit P13 ----*/
#define GPIO1PIN_P13_GET                ( bFM_GPIO_PDIR1_P3 )

#define GPIO1PIN_P13_PUT(v)             ( bFM_GPIO_PDOR1_P3=(v) )

#define GPIO1PIN_P13_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P13_INITIN(v) \
                                                           : GPIO1PIN_P13_INITOUT(v) )

#define GPIO1PIN_P13_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN03=0u; \
                                            bFM_GPIO_PCR1_P3=(v).bPullup; \
                                            bFM_GPIO_DDR1_P3=0u; \
                                            bFM_GPIO_PFR1_P3=0u; }while(0u)

#define GPIO1PIN_P13_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN03=0u; \
                                            bFM_GPIO_PDOR1_P3=(v).bInitVal; \
                                            bFM_GPIO_DDR1_P3=1u; \
                                            bFM_GPIO_PFR1_P3=0u; }while(0u)

/*---- GPIO bit NP13 ----*/
#define GPIO1PIN_NP13_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_P3)) )

#define GPIO1PIN_NP13_PUT(v)            ( bFM_GPIO_PDOR1_P3=(uint32_t)(!(v)) )

#define GPIO1PIN_NP13_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP13_INITIN(v) \
                                                           : GPIO1PIN_NP13_INITOUT(v) )

#define GPIO1PIN_NP13_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN03=0u; \
                                            bFM_GPIO_PCR1_P3=(v).bPullup; \
                                            bFM_GPIO_DDR1_P3=0u; \
                                            bFM_GPIO_PFR1_P3=0u; }while(0u)

#define GPIO1PIN_NP13_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN03=0u; \
                                            bFM_GPIO_PDOR1_P3=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_P3=1u; \
                                            bFM_GPIO_PFR1_P3=0u; }while(0u)

/*---- GPIO bit P14 ----*/
#define GPIO1PIN_P14_GET                ( bFM_GPIO_PDIR1_P4 )

#define GPIO1PIN_P14_PUT(v)             ( bFM_GPIO_PDOR1_P4=(v) )

#define GPIO1PIN_P14_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P14_INITIN(v) \
                                                           : GPIO1PIN_P14_INITOUT(v) )

#define GPIO1PIN_P14_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN04=0u; \
                                            bFM_GPIO_PCR1_P4=(v).bPullup; \
                                            bFM_GPIO_DDR1_P4=0u; \
                                            bFM_GPIO_PFR1_P4=0u; }while(0u)

#define GPIO1PIN_P14_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN04=0u; \
                                            bFM_GPIO_PDOR1_P4=(v).bInitVal; \
                                            bFM_GPIO_DDR1_P4=1u; \
                                            bFM_GPIO_PFR1_P4=0u; }while(0u)

/*---- GPIO bit NP14 ----*/
#define GPIO1PIN_NP14_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_P4)) )

#define GPIO1PIN_NP14_PUT(v)            ( bFM_GPIO_PDOR1_P4=(uint32_t)(!(v)) )

#define GPIO1PIN_NP14_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP14_INITIN(v) \
                                                           : GPIO1PIN_NP14_INITOUT(v) )

#define GPIO1PIN_NP14_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN04=0u; \
                                            bFM_GPIO_PCR1_P4=(v).bPullup; \
                                            bFM_GPIO_DDR1_P4=0u; \
                                            bFM_GPIO_PFR1_P4=0u; }while(0u)

#define GPIO1PIN_NP14_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN04=0u; \
                                            bFM_GPIO_PDOR1_P4=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_P4=1u; \
                                            bFM_GPIO_PFR1_P4=0u; }while(0u)

/*---- GPIO bit P15 ----*/
#define GPIO1PIN_P15_GET                ( bFM_GPIO_PDIR1_P5 )

#define GPIO1PIN_P15_PUT(v)             ( bFM_GPIO_PDOR1_P5=(v) )

#define GPIO1PIN_P15_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P15_INITIN(v) \
                                                           : GPIO1PIN_P15_INITOUT(v) )

#define GPIO1PIN_P15_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN05=0u; \
                                            bFM_GPIO_PCR1_P5=(v).bPullup; \
                                            bFM_GPIO_DDR1_P5=0u; \
                                            bFM_GPIO_PFR1_P5=0u; }while(0u)

#define GPIO1PIN_P15_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN05=0u; \
                                            bFM_GPIO_PDOR1_P5=(v).bInitVal; \
                                            bFM_GPIO_DDR1_P5=1u; \
                                            bFM_GPIO_PFR1_P5=0u; }while(0u)

/*---- GPIO bit NP15 ----*/
#define GPIO1PIN_NP15_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_P5)) )

#define GPIO1PIN_NP15_PUT(v)            ( bFM_GPIO_PDOR1_P5=(uint32_t)(!(v)) )

#define GPIO1PIN_NP15_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP15_INITIN(v) \
                                                           : GPIO1PIN_NP15_INITOUT(v) )

#define GPIO1PIN_NP15_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN05=0u; \
                                            bFM_GPIO_PCR1_P5=(v).bPullup; \
                                            bFM_GPIO_DDR1_P5=0u; \
                                            bFM_GPIO_PFR1_P5=0u; }while(0u)

#define GPIO1PIN_NP15_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN05=0u; \
                                            bFM_GPIO_PDOR1_P5=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_P5=1u; \
                                            bFM_GPIO_PFR1_P5=0u; }while(0u)

/*---- GPIO bit P16 ----*/
#define GPIO1PIN_P16_GET                ( bFM_GPIO_PDIR1_P6 )

#define GPIO1PIN_P16_PUT(v)             ( bFM_GPIO_PDOR1_P6=(v) )

#define GPIO1PIN_P16_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P16_INITIN(v) \
                                                           : GPIO1PIN_P16_INITOUT(v) )

#define GPIO1PIN_P16_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN06=0u; \
                                            bFM_GPIO_PCR1_P6=(v).bPullup; \
                                            bFM_GPIO_DDR1_P6=0u; \
                                            bFM_GPIO_PFR1_P6=0u; }while(0u)

#define GPIO1PIN_P16_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN06=0u; \
                                            bFM_GPIO_PDOR1_P6=(v).bInitVal; \
                                            bFM_GPIO_DDR1_P6=1u; \
                                            bFM_GPIO_PFR1_P6=0u; }while(0u)

/*---- GPIO bit NP16 ----*/
#define GPIO1PIN_NP16_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_P6)) )

#define GPIO1PIN_NP16_PUT(v)            ( bFM_GPIO_PDOR1_P6=(uint32_t)(!(v)) )

#define GPIO1PIN_NP16_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP16_INITIN(v) \
                                                           : GPIO1PIN_NP16_INITOUT(v) )

#define GPIO1PIN_NP16_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN06=0u; \
                                            bFM_GPIO_PCR1_P6=(v).bPullup; \
                                            bFM_GPIO_DDR1_P6=0u; \
                                            bFM_GPIO_PFR1_P6=0u; }while(0u)

#define GPIO1PIN_NP16_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN06=0u; \
                                            bFM_GPIO_PDOR1_P6=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_P6=1u; \
                                            bFM_GPIO_PFR1_P6=0u; }while(0u)

/*---- GPIO bit P17 ----*/
#define GPIO1PIN_P17_GET                ( bFM_GPIO_PDIR1_P7 )

#define GPIO1PIN_P17_PUT(v)             ( bFM_GPIO_PDOR1_P7=(v) )

#define GPIO1PIN_P17_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P17_INITIN(v) \
                                                           : GPIO1PIN_P17_INITOUT(v) )

#define GPIO1PIN_P17_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN07=0u; \
                                            bFM_GPIO_PCR1_P7=(v).bPullup; \
                                            bFM_GPIO_DDR1_P7=0u; \
                                            bFM_GPIO_PFR1_P7=0u; }while(0u)

#define GPIO1PIN_P17_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN07=0u; \
                                            bFM_GPIO_PDOR1_P7=(v).bInitVal; \
                                            bFM_GPIO_DDR1_P7=1u; \
                                            bFM_GPIO_PFR1_P7=0u; }while(0u)

/*---- GPIO bit NP17 ----*/
#define GPIO1PIN_NP17_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_P7)) )

#define GPIO1PIN_NP17_PUT(v)            ( bFM_GPIO_PDOR1_P7=(uint32_t)(!(v)) )

#define GPIO1PIN_NP17_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP17_INITIN(v) \
                                                           : GPIO1PIN_NP17_INITOUT(v) )

#define GPIO1PIN_NP17_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN07=0u; \
                                            bFM_GPIO_PCR1_P7=(v).bPullup; \
                                            bFM_GPIO_DDR1_P7=0u; \
                                            bFM_GPIO_PFR1_P7=0u; }while(0u)

#define GPIO1PIN_NP17_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN07=0u; \
                                            bFM_GPIO_PDOR1_P7=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_P7=1u; \
                                            bFM_GPIO_PFR1_P7=0u; }while(0u)

/*---- GPIO bit P18 ----*/
#define GPIO1PIN_P18_GET                ( bFM_GPIO_PDIR1_P8 )

#define GPIO1PIN_P18_PUT(v)             ( bFM_GPIO_PDOR1_P8=(v) )

#define GPIO1PIN_P18_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P18_INITIN(v) \
                                                           : GPIO1PIN_P18_INITOUT(v) )

#define GPIO1PIN_P18_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN08=0u; \
                                            bFM_GPIO_PCR1_P8=(v).bPullup; \
                                            bFM_GPIO_DDR1_P8=0u; \
                                            bFM_GPIO_PFR1_P8=0u; }while(0u)

#define GPIO1PIN_P18_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN08=0u; \
                                            bFM_GPIO_PDOR1_P8=(v).bInitVal; \
                                            bFM_GPIO_DDR1_P8=1u; \
                                            bFM_GPIO_PFR1_P8=0u; }while(0u)

/*---- GPIO bit NP18 ----*/
#define GPIO1PIN_NP18_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_P8)) )

#define GPIO1PIN_NP18_PUT(v)            ( bFM_GPIO_PDOR1_P8=(uint32_t)(!(v)) )

#define GPIO1PIN_NP18_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP18_INITIN(v) \
                                                           : GPIO1PIN_NP18_INITOUT(v) )

#define GPIO1PIN_NP18_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN08=0u; \
                                            bFM_GPIO_PCR1_P8=(v).bPullup; \
                                            bFM_GPIO_DDR1_P8=0u; \
                                            bFM_GPIO_PFR1_P8=0u; }while(0u)

#define GPIO1PIN_NP18_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN08=0u; \
                                            bFM_GPIO_PDOR1_P8=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_P8=1u; \
                                            bFM_GPIO_PFR1_P8=0u; }while(0u)

/*---- GPIO bit P19 ----*/
#define GPIO1PIN_P19_GET                ( bFM_GPIO_PDIR1_P9 )

#define GPIO1PIN_P19_PUT(v)             ( bFM_GPIO_PDOR1_P9=(v) )

#define GPIO1PIN_P19_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P19_INITIN(v) \
                                                           : GPIO1PIN_P19_INITOUT(v) )

#define GPIO1PIN_P19_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN09=0u; \
                                            bFM_GPIO_PCR1_P9=(v).bPullup; \
                                            bFM_GPIO_DDR1_P9=0u; \
                                            bFM_GPIO_PFR1_P9=0u; }while(0u)

#define GPIO1PIN_P19_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN09=0u; \
                                            bFM_GPIO_PDOR1_P9=(v).bInitVal; \
                                            bFM_GPIO_DDR1_P9=1u; \
                                            bFM_GPIO_PFR1_P9=0u; }while(0u)

/*---- GPIO bit NP19 ----*/
#define GPIO1PIN_NP19_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_P9)) )

#define GPIO1PIN_NP19_PUT(v)            ( bFM_GPIO_PDOR1_P9=(uint32_t)(!(v)) )

#define GPIO1PIN_NP19_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP19_INITIN(v) \
                                                           : GPIO1PIN_NP19_INITOUT(v) )

#define GPIO1PIN_NP19_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN09=0u; \
                                            bFM_GPIO_PCR1_P9=(v).bPullup; \
                                            bFM_GPIO_DDR1_P9=0u; \
                                            bFM_GPIO_PFR1_P9=0u; }while(0u)

#define GPIO1PIN_NP19_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN09=0u; \
                                            bFM_GPIO_PDOR1_P9=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_P9=1u; \
                                            bFM_GPIO_PFR1_P9=0u; }while(0u)

/*---- GPIO bit P1A ----*/
#define GPIO1PIN_P1A_GET                ( bFM_GPIO_PDIR1_PA )

#define GPIO1PIN_P1A_PUT(v)             ( bFM_GPIO_PDOR1_PA=(v) )

#define GPIO1PIN_P1A_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P1A_INITIN(v) \
                                                           : GPIO1PIN_P1A_INITOUT(v) )

#define GPIO1PIN_P1A_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN10=0u; \
                                            bFM_GPIO_PCR1_PA=(v).bPullup; \
                                            bFM_GPIO_DDR1_PA=0u; \
                                            bFM_GPIO_PFR1_PA=0u; }while(0u)

#define GPIO1PIN_P1A_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN10=0u; \
                                            bFM_GPIO_PDOR1_PA=(v).bInitVal; \
                                            bFM_GPIO_DDR1_PA=1u; \
                                            bFM_GPIO_PFR1_PA=0u; }while(0u)

/*---- GPIO bit NP1A ----*/
#define GPIO1PIN_NP1A_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_PA)) )

#define GPIO1PIN_NP1A_PUT(v)            ( bFM_GPIO_PDOR1_PA=(uint32_t)(!(v)) )

#define GPIO1PIN_NP1A_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP1A_INITIN(v) \
                                                           : GPIO1PIN_NP1A_INITOUT(v) )

#define GPIO1PIN_NP1A_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN10=0u; \
                                            bFM_GPIO_PCR1_PA=(v).bPullup; \
                                            bFM_GPIO_DDR1_PA=0u; \
                                            bFM_GPIO_PFR1_PA=0u; }while(0u)

#define GPIO1PIN_NP1A_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN10=0u; \
                                            bFM_GPIO_PDOR1_PA=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_PA=1u; \
                                            bFM_GPIO_PFR1_PA=0u; }while(0u)

/*---- GPIO bit P1B ----*/
#define GPIO1PIN_P1B_GET                ( bFM_GPIO_PDIR1_PB )

#define GPIO1PIN_P1B_PUT(v)             ( bFM_GPIO_PDOR1_PB=(v) )

#define GPIO1PIN_P1B_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P1B_INITIN(v) \
                                                           : GPIO1PIN_P1B_INITOUT(v) )

#define GPIO1PIN_P1B_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN11=0u; \
                                            bFM_GPIO_PCR1_PB=(v).bPullup; \
                                            bFM_GPIO_DDR1_PB=0u; \
                                            bFM_GPIO_PFR1_PB=0u; }while(0u)

#define GPIO1PIN_P1B_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN11=0u; \
                                            bFM_GPIO_PDOR1_PB=(v).bInitVal; \
                                            bFM_GPIO_DDR1_PB=1u; \
                                            bFM_GPIO_PFR1_PB=0u; }while(0u)

/*---- GPIO bit NP1B ----*/
#define GPIO1PIN_NP1B_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_PB)) )

#define GPIO1PIN_NP1B_PUT(v)            ( bFM_GPIO_PDOR1_PB=(uint32_t)(!(v)) )

#define GPIO1PIN_NP1B_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP1B_INITIN(v) \
                                                           : GPIO1PIN_NP1B_INITOUT(v) )

#define GPIO1PIN_NP1B_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN11=0u; \
                                            bFM_GPIO_PCR1_PB=(v).bPullup; \
                                            bFM_GPIO_DDR1_PB=0u; \
                                            bFM_GPIO_PFR1_PB=0u; }while(0u)

#define GPIO1PIN_NP1B_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN11=0u; \
                                            bFM_GPIO_PDOR1_PB=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_PB=1u; \
                                            bFM_GPIO_PFR1_PB=0u; }while(0u)

/*---- GPIO bit P1C ----*/
#define GPIO1PIN_P1C_GET                ( bFM_GPIO_PDIR1_PC )

#define GPIO1PIN_P1C_PUT(v)             ( bFM_GPIO_PDOR1_PC=(v) )

#define GPIO1PIN_P1C_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P1C_INITIN(v) \
                                                           : GPIO1PIN_P1C_INITOUT(v) )

#define GPIO1PIN_P1C_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN12=0u; \
                                            bFM_GPIO_PCR1_PC=(v).bPullup; \
                                            bFM_GPIO_DDR1_PC=0u; \
                                            bFM_GPIO_PFR1_PC=0u; }while(0u)

#define GPIO1PIN_P1C_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN12=0u; \
                                            bFM_GPIO_PDOR1_PC=(v).bInitVal; \
                                            bFM_GPIO_DDR1_PC=1u; \
                                            bFM_GPIO_PFR1_PC=0u; }while(0u)

/*---- GPIO bit NP1C ----*/
#define GPIO1PIN_NP1C_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_PC)) )

#define GPIO1PIN_NP1C_PUT(v)            ( bFM_GPIO_PDOR1_PC=(uint32_t)(!(v)) )

#define GPIO1PIN_NP1C_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP1C_INITIN(v) \
                                                           : GPIO1PIN_NP1C_INITOUT(v) )

#define GPIO1PIN_NP1C_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN12=0u; \
                                            bFM_GPIO_PCR1_PC=(v).bPullup; \
                                            bFM_GPIO_DDR1_PC=0u; \
                                            bFM_GPIO_PFR1_PC=0u; }while(0u)

#define GPIO1PIN_NP1C_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN12=0u; \
                                            bFM_GPIO_PDOR1_PC=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_PC=1u; \
                                            bFM_GPIO_PFR1_PC=0u; }while(0u)

/*---- GPIO bit P1D ----*/
#define GPIO1PIN_P1D_GET                ( bFM_GPIO_PDIR1_PD )

#define GPIO1PIN_P1D_PUT(v)             ( bFM_GPIO_PDOR1_PD=(v) )

#define GPIO1PIN_P1D_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P1D_INITIN(v) \
                                                           : GPIO1PIN_P1D_INITOUT(v) )

#define GPIO1PIN_P1D_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN13=0u; \
                                            bFM_GPIO_PCR1_PD=(v).bPullup; \
                                            bFM_GPIO_DDR1_PD=0u; \
                                            bFM_GPIO_PFR1_PD=0u; }while(0u)

#define GPIO1PIN_P1D_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN13=0u; \
                                            bFM_GPIO_PDOR1_PD=(v).bInitVal; \
                                            bFM_GPIO_DDR1_PD=1u; \
                                            bFM_GPIO_PFR1_PD=0u; }while(0u)

/*---- GPIO bit NP1D ----*/
#define GPIO1PIN_NP1D_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_PD)) )

#define GPIO1PIN_NP1D_PUT(v)            ( bFM_GPIO_PDOR1_PD=(uint32_t)(!(v)) )

#define GPIO1PIN_NP1D_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP1D_INITIN(v) \
                                                           : GPIO1PIN_NP1D_INITOUT(v) )

#define GPIO1PIN_NP1D_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN13=0u; \
                                            bFM_GPIO_PCR1_PD=(v).bPullup; \
                                            bFM_GPIO_DDR1_PD=0u; \
                                            bFM_GPIO_PFR1_PD=0u; }while(0u)

#define GPIO1PIN_NP1D_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN13=0u; \
                                            bFM_GPIO_PDOR1_PD=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_PD=1u; \
                                            bFM_GPIO_PFR1_PD=0u; }while(0u)

/*---- GPIO bit P1E ----*/
#define GPIO1PIN_P1E_GET                ( bFM_GPIO_PDIR1_PE )

#define GPIO1PIN_P1E_PUT(v)             ( bFM_GPIO_PDOR1_PE=(v) )

#define GPIO1PIN_P1E_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P1E_INITIN(v) \
                                                           : GPIO1PIN_P1E_INITOUT(v) )

#define GPIO1PIN_P1E_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN14=0u; \
                                            bFM_GPIO_PCR1_PE=(v).bPullup; \
                                            bFM_GPIO_DDR1_PE=0u; \
                                            bFM_GPIO_PFR1_PE=0u; }while(0u)

#define GPIO1PIN_P1E_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN14=0u; \
                                            bFM_GPIO_PDOR1_PE=(v).bInitVal; \
                                            bFM_GPIO_DDR1_PE=1u; \
                                            bFM_GPIO_PFR1_PE=0u; }while(0u)

/*---- GPIO bit NP1E ----*/
#define GPIO1PIN_NP1E_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_PE)) )

#define GPIO1PIN_NP1E_PUT(v)            ( bFM_GPIO_PDOR1_PE=(uint32_t)(!(v)) )

#define GPIO1PIN_NP1E_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP1E_INITIN(v) \
                                                           : GPIO1PIN_NP1E_INITOUT(v) )

#define GPIO1PIN_NP1E_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN14=0u; \
                                            bFM_GPIO_PCR1_PE=(v).bPullup; \
                                            bFM_GPIO_DDR1_PE=0u; \
                                            bFM_GPIO_PFR1_PE=0u; }while(0u)

#define GPIO1PIN_NP1E_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN14=0u; \
                                            bFM_GPIO_PDOR1_PE=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_PE=1u; \
                                            bFM_GPIO_PFR1_PE=0u; }while(0u)

/*---- GPIO bit P1F ----*/
#define GPIO1PIN_P1F_GET                ( bFM_GPIO_PDIR1_PF )

#define GPIO1PIN_P1F_PUT(v)             ( bFM_GPIO_PDOR1_PF=(v) )

#define GPIO1PIN_P1F_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P1F_INITIN(v) \
                                                           : GPIO1PIN_P1F_INITOUT(v) )

#define GPIO1PIN_P1F_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN15=0u; \
                                            bFM_GPIO_PCR1_PF=(v).bPullup; \
                                            bFM_GPIO_DDR1_PF=0u; \
                                            bFM_GPIO_PFR1_PF=0u; }while(0u)

#define GPIO1PIN_P1F_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN15=0u; \
                                            bFM_GPIO_PDOR1_PF=(v).bInitVal; \
                                            bFM_GPIO_DDR1_PF=1u; \
                                            bFM_GPIO_PFR1_PF=0u; }while(0u)

/*---- GPIO bit NP1F ----*/
#define GPIO1PIN_NP1F_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_PF)) )

#define GPIO1PIN_NP1F_PUT(v)            ( bFM_GPIO_PDOR1_PF=(uint32_t)(!(v)) )

#define GPIO1PIN_NP1F_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP1F_INITIN(v) \
                                                           : GPIO1PIN_NP1F_INITOUT(v) )

#define GPIO1PIN_NP1F_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN15=0u; \
                                            bFM_GPIO_PCR1_PF=(v).bPullup; \
                                            bFM_GPIO_DDR1_PF=0u; \
                                            bFM_GPIO_PFR1_PF=0u; }while(0u)

#define GPIO1PIN_NP1F_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN15=0u; \
                                            bFM_GPIO_PDOR1_PF=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_PF=1u; \
                                            bFM_GPIO_PFR1_PF=0u; }while(0u)

/*---- GPIO bit P20 ----*/
#define GPIO1PIN_P20_GET                ( bFM_GPIO_PDIR2_P0 )

#define GPIO1PIN_P20_PUT(v)             ( bFM_GPIO_PDOR2_P0=(v) )

#define GPIO1PIN_P20_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P20_INITIN(v) \
                                                           : GPIO1PIN_P20_INITOUT(v) )

#define GPIO1PIN_P20_INITIN(v)          do{ \
                                            bFM_GPIO_PCR2_P0=(v).bPullup; \
                                            bFM_GPIO_DDR2_P0=0u; \
                                            bFM_GPIO_PFR2_P0=0u; }while(0u)

#define GPIO1PIN_P20_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR2_P0=(v).bInitVal; \
                                            bFM_GPIO_DDR2_P0=1u; \
                                            bFM_GPIO_PFR2_P0=0u; }while(0u)

/*---- GPIO bit NP20 ----*/
#define GPIO1PIN_NP20_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR2_P0)) )

#define GPIO1PIN_NP20_PUT(v)            ( bFM_GPIO_PDOR2_P0=(uint32_t)(!(v)) )

#define GPIO1PIN_NP20_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP20_INITIN(v) \
                                                           : GPIO1PIN_NP20_INITOUT(v) )

#define GPIO1PIN_NP20_INITIN(v)         do{ \
                                            bFM_GPIO_PCR2_P0=(v).bPullup; \
                                            bFM_GPIO_DDR2_P0=0u; \
                                            bFM_GPIO_PFR2_P0=0u; }while(0u)

#define GPIO1PIN_NP20_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR2_P0=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR2_P0=1u; \
                                            bFM_GPIO_PFR2_P0=0u; }while(0u)

/*---- GPIO bit P21 ----*/
#define GPIO1PIN_P21_GET                ( bFM_GPIO_PDIR2_P1 )

#define GPIO1PIN_P21_PUT(v)             ( bFM_GPIO_PDOR2_P1=(v) )

#define GPIO1PIN_P21_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P21_INITIN(v) \
                                                           : GPIO1PIN_P21_INITOUT(v) )

#define GPIO1PIN_P21_INITIN(v)          do{ \
                                            bFM_GPIO_PCR2_P1=(v).bPullup; \
                                            bFM_GPIO_DDR2_P1=0u; \
                                            bFM_GPIO_PFR2_P1=0u; }while(0u)

#define GPIO1PIN_P21_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR2_P1=(v).bInitVal; \
                                            bFM_GPIO_DDR2_P1=1u; \
                                            bFM_GPIO_PFR2_P1=0u; }while(0u)

/*---- GPIO bit NP21 ----*/
#define GPIO1PIN_NP21_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR2_P1)) )

#define GPIO1PIN_NP21_PUT(v)            ( bFM_GPIO_PDOR2_P1=(uint32_t)(!(v)) )

#define GPIO1PIN_NP21_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP21_INITIN(v) \
                                                           : GPIO1PIN_NP21_INITOUT(v) )

#define GPIO1PIN_NP21_INITIN(v)         do{ \
                                            bFM_GPIO_PCR2_P1=(v).bPullup; \
                                            bFM_GPIO_DDR2_P1=0u; \
                                            bFM_GPIO_PFR2_P1=0u; }while(0u)

#define GPIO1PIN_NP21_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR2_P1=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR2_P1=1u; \
                                            bFM_GPIO_PFR2_P1=0u; }while(0u)

/*---- GPIO bit P22 ----*/
#define GPIO1PIN_P22_GET                ( bFM_GPIO_PDIR2_P2 )

#define GPIO1PIN_P22_PUT(v)             ( bFM_GPIO_PDOR2_P2=(v) )

#define GPIO1PIN_P22_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P22_INITIN(v) \
                                                           : GPIO1PIN_P22_INITOUT(v) )

#define GPIO1PIN_P22_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN31=0u; \
                                            bFM_GPIO_PCR2_P2=(v).bPullup; \
                                            bFM_GPIO_DDR2_P2=0u; \
                                            bFM_GPIO_PFR2_P2=0u; }while(0u)

#define GPIO1PIN_P22_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN31=0u; \
                                            bFM_GPIO_PDOR2_P2=(v).bInitVal; \
                                            bFM_GPIO_DDR2_P2=1u; \
                                            bFM_GPIO_PFR2_P2=0u; }while(0u)

/*---- GPIO bit NP22 ----*/
#define GPIO1PIN_NP22_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR2_P2)) )

#define GPIO1PIN_NP22_PUT(v)            ( bFM_GPIO_PDOR2_P2=(uint32_t)(!(v)) )

#define GPIO1PIN_NP22_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP22_INITIN(v) \
                                                           : GPIO1PIN_NP22_INITOUT(v) )

#define GPIO1PIN_NP22_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN31=0u; \
                                            bFM_GPIO_PCR2_P2=(v).bPullup; \
                                            bFM_GPIO_DDR2_P2=0u; \
                                            bFM_GPIO_PFR2_P2=0u; }while(0u)

#define GPIO1PIN_NP22_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN31=0u; \
                                            bFM_GPIO_PDOR2_P2=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR2_P2=1u; \
                                            bFM_GPIO_PFR2_P2=0u; }while(0u)

/*---- GPIO bit P23 ----*/
#define GPIO1PIN_P23_GET                ( bFM_GPIO_PDIR2_P3 )

#define GPIO1PIN_P23_PUT(v)             ( bFM_GPIO_PDOR2_P3=(v) )

#define GPIO1PIN_P23_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P23_INITIN(v) \
                                                           : GPIO1PIN_P23_INITOUT(v) )

#define GPIO1PIN_P23_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN30=0u; \
                                            bFM_GPIO_PCR2_P3=(v).bPullup; \
                                            bFM_GPIO_DDR2_P3=0u; \
                                            bFM_GPIO_PFR2_P3=0u; }while(0u)

#define GPIO1PIN_P23_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN30=0u; \
                                            bFM_GPIO_PDOR2_P3=(v).bInitVal; \
                                            bFM_GPIO_DDR2_P3=1u; \
                                            bFM_GPIO_PFR2_P3=0u; }while(0u)

/*---- GPIO bit NP23 ----*/
#define GPIO1PIN_NP23_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR2_P3)) )

#define GPIO1PIN_NP23_PUT(v)            ( bFM_GPIO_PDOR2_P3=(uint32_t)(!(v)) )

#define GPIO1PIN_NP23_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP23_INITIN(v) \
                                                           : GPIO1PIN_NP23_INITOUT(v) )

#define GPIO1PIN_NP23_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN30=0u; \
                                            bFM_GPIO_PCR2_P3=(v).bPullup; \
                                            bFM_GPIO_DDR2_P3=0u; \
                                            bFM_GPIO_PFR2_P3=0u; }while(0u)

#define GPIO1PIN_NP23_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN30=0u; \
                                            bFM_GPIO_PDOR2_P3=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR2_P3=1u; \
                                            bFM_GPIO_PFR2_P3=0u; }while(0u)

/*---- GPIO bit P24 ----*/
#define GPIO1PIN_P24_GET                ( bFM_GPIO_PDIR2_P4 )

#define GPIO1PIN_P24_PUT(v)             ( bFM_GPIO_PDOR2_P4=(v) )

#define GPIO1PIN_P24_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P24_INITIN(v) \
                                                           : GPIO1PIN_P24_INITOUT(v) )

#define GPIO1PIN_P24_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN29=0u; \
                                            bFM_GPIO_PCR2_P4=(v).bPullup; \
                                            bFM_GPIO_DDR2_P4=0u; \
                                            bFM_GPIO_PFR2_P4=0u; }while(0u)

#define GPIO1PIN_P24_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN29=0u; \
                                            bFM_GPIO_PDOR2_P4=(v).bInitVal; \
                                            bFM_GPIO_DDR2_P4=1u; \
                                            bFM_GPIO_PFR2_P4=0u; }while(0u)

/*---- GPIO bit NP24 ----*/
#define GPIO1PIN_NP24_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR2_P4)) )

#define GPIO1PIN_NP24_PUT(v)            ( bFM_GPIO_PDOR2_P4=(uint32_t)(!(v)) )

#define GPIO1PIN_NP24_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP24_INITIN(v) \
                                                           : GPIO1PIN_NP24_INITOUT(v) )

#define GPIO1PIN_NP24_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN29=0u; \
                                            bFM_GPIO_PCR2_P4=(v).bPullup; \
                                            bFM_GPIO_DDR2_P4=0u; \
                                            bFM_GPIO_PFR2_P4=0u; }while(0u)

#define GPIO1PIN_NP24_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN29=0u; \
                                            bFM_GPIO_PDOR2_P4=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR2_P4=1u; \
                                            bFM_GPIO_PFR2_P4=0u; }while(0u)

/*---- GPIO bit P25 ----*/
#define GPIO1PIN_P25_GET                ( bFM_GPIO_PDIR2_P5 )

#define GPIO1PIN_P25_PUT(v)             ( bFM_GPIO_PDOR2_P5=(v) )

#define GPIO1PIN_P25_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P25_INITIN(v) \
                                                           : GPIO1PIN_P25_INITOUT(v) )

#define GPIO1PIN_P25_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN28=0u; \
                                            bFM_GPIO_PCR2_P5=(v).bPullup; \
                                            bFM_GPIO_DDR2_P5=0u; \
                                            bFM_GPIO_PFR2_P5=0u; }while(0u)

#define GPIO1PIN_P25_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN28=0u; \
                                            bFM_GPIO_PDOR2_P5=(v).bInitVal; \
                                            bFM_GPIO_DDR2_P5=1u; \
                                            bFM_GPIO_PFR2_P5=0u; }while(0u)

/*---- GPIO bit NP25 ----*/
#define GPIO1PIN_NP25_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR2_P5)) )

#define GPIO1PIN_NP25_PUT(v)            ( bFM_GPIO_PDOR2_P5=(uint32_t)(!(v)) )

#define GPIO1PIN_NP25_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP25_INITIN(v) \
                                                           : GPIO1PIN_NP25_INITOUT(v) )

#define GPIO1PIN_NP25_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN28=0u; \
                                            bFM_GPIO_PCR2_P5=(v).bPullup; \
                                            bFM_GPIO_DDR2_P5=0u; \
                                            bFM_GPIO_PFR2_P5=0u; }while(0u)

#define GPIO1PIN_NP25_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN28=0u; \
                                            bFM_GPIO_PDOR2_P5=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR2_P5=1u; \
                                            bFM_GPIO_PFR2_P5=0u; }while(0u)

/*---- GPIO bit P26 ----*/
#define GPIO1PIN_P26_GET                ( bFM_GPIO_PDIR2_P6 )

#define GPIO1PIN_P26_PUT(v)             ( bFM_GPIO_PDOR2_P6=(v) )

#define GPIO1PIN_P26_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P26_INITIN(v) \
                                                           : GPIO1PIN_P26_INITOUT(v) )

#define GPIO1PIN_P26_INITIN(v)          do{ \
                                            bFM_GPIO_PCR2_P6=(v).bPullup; \
                                            bFM_GPIO_DDR2_P6=0u; \
                                            bFM_GPIO_PFR2_P6=0u; }while(0u)

#define GPIO1PIN_P26_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR2_P6=(v).bInitVal; \
                                            bFM_GPIO_DDR2_P6=1u; \
                                            bFM_GPIO_PFR2_P6=0u; }while(0u)

/*---- GPIO bit NP26 ----*/
#define GPIO1PIN_NP26_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR2_P6)) )

#define GPIO1PIN_NP26_PUT(v)            ( bFM_GPIO_PDOR2_P6=(uint32_t)(!(v)) )

#define GPIO1PIN_NP26_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP26_INITIN(v) \
                                                           : GPIO1PIN_NP26_INITOUT(v) )

#define GPIO1PIN_NP26_INITIN(v)         do{ \
                                            bFM_GPIO_PCR2_P6=(v).bPullup; \
                                            bFM_GPIO_DDR2_P6=0u; \
                                            bFM_GPIO_PFR2_P6=0u; }while(0u)

#define GPIO1PIN_NP26_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR2_P6=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR2_P6=1u; \
                                            bFM_GPIO_PFR2_P6=0u; }while(0u)

/*---- GPIO bit P27 ----*/
#define GPIO1PIN_P27_GET                ( bFM_GPIO_PDIR2_P7 )

#define GPIO1PIN_P27_PUT(v)             ( bFM_GPIO_PDOR2_P7=(v) )

#define GPIO1PIN_P27_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P27_INITIN(v) \
                                                           : GPIO1PIN_P27_INITOUT(v) )

#define GPIO1PIN_P27_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN27=0u; \
                                            bFM_GPIO_PCR2_P7=(v).bPullup; \
                                            bFM_GPIO_DDR2_P7=0u; \
                                            bFM_GPIO_PFR2_P7=0u; }while(0u)

#define GPIO1PIN_P27_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN27=0u; \
                                            bFM_GPIO_PDOR2_P7=(v).bInitVal; \
                                            bFM_GPIO_DDR2_P7=1u; \
                                            bFM_GPIO_PFR2_P7=0u; }while(0u)

/*---- GPIO bit NP27 ----*/
#define GPIO1PIN_NP27_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR2_P7)) )

#define GPIO1PIN_NP27_PUT(v)            ( bFM_GPIO_PDOR2_P7=(uint32_t)(!(v)) )

#define GPIO1PIN_NP27_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP27_INITIN(v) \
                                                           : GPIO1PIN_NP27_INITOUT(v) )

#define GPIO1PIN_NP27_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN27=0u; \
                                            bFM_GPIO_PCR2_P7=(v).bPullup; \
                                            bFM_GPIO_DDR2_P7=0u; \
                                            bFM_GPIO_PFR2_P7=0u; }while(0u)

#define GPIO1PIN_NP27_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN27=0u; \
                                            bFM_GPIO_PDOR2_P7=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR2_P7=1u; \
                                            bFM_GPIO_PFR2_P7=0u; }while(0u)

/*---- GPIO bit P28 ----*/
#define GPIO1PIN_P28_GET                ( bFM_GPIO_PDIR2_P8 )

#define GPIO1PIN_P28_PUT(v)             ( bFM_GPIO_PDOR2_P8=(v) )

#define GPIO1PIN_P28_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P28_INITIN(v) \
                                                           : GPIO1PIN_P28_INITOUT(v) )

#define GPIO1PIN_P28_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN26=0u; \
                                            bFM_GPIO_PCR2_P8=(v).bPullup; \
                                            bFM_GPIO_DDR2_P8=0u; \
                                            bFM_GPIO_PFR2_P8=0u; }while(0u)

#define GPIO1PIN_P28_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN26=0u; \
                                            bFM_GPIO_PDOR2_P8=(v).bInitVal; \
                                            bFM_GPIO_DDR2_P8=1u; \
                                            bFM_GPIO_PFR2_P8=0u; }while(0u)

/*---- GPIO bit NP28 ----*/
#define GPIO1PIN_NP28_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR2_P8)) )

#define GPIO1PIN_NP28_PUT(v)            ( bFM_GPIO_PDOR2_P8=(uint32_t)(!(v)) )

#define GPIO1PIN_NP28_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP28_INITIN(v) \
                                                           : GPIO1PIN_NP28_INITOUT(v) )

#define GPIO1PIN_NP28_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN26=0u; \
                                            bFM_GPIO_PCR2_P8=(v).bPullup; \
                                            bFM_GPIO_DDR2_P8=0u; \
                                            bFM_GPIO_PFR2_P8=0u; }while(0u)

#define GPIO1PIN_NP28_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN26=0u; \
                                            bFM_GPIO_PDOR2_P8=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR2_P8=1u; \
                                            bFM_GPIO_PFR2_P8=0u; }while(0u)

/*---- GPIO bit P29 ----*/
#define GPIO1PIN_P29_GET                ( bFM_GPIO_PDIR2_P9 )

#define GPIO1PIN_P29_PUT(v)             ( bFM_GPIO_PDOR2_P9=(v) )

#define GPIO1PIN_P29_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P29_INITIN(v) \
                                                           : GPIO1PIN_P29_INITOUT(v) )

#define GPIO1PIN_P29_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN25=0u; \
                                            bFM_GPIO_PCR2_P9=(v).bPullup; \
                                            bFM_GPIO_DDR2_P9=0u; \
                                            bFM_GPIO_PFR2_P9=0u; }while(0u)

#define GPIO1PIN_P29_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN25=0u; \
                                            bFM_GPIO_PDOR2_P9=(v).bInitVal; \
                                            bFM_GPIO_DDR2_P9=1u; \
                                            bFM_GPIO_PFR2_P9=0u; }while(0u)

/*---- GPIO bit NP29 ----*/
#define GPIO1PIN_NP29_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR2_P9)) )

#define GPIO1PIN_NP29_PUT(v)            ( bFM_GPIO_PDOR2_P9=(uint32_t)(!(v)) )

#define GPIO1PIN_NP29_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP29_INITIN(v) \
                                                           : GPIO1PIN_NP29_INITOUT(v) )

#define GPIO1PIN_NP29_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN25=0u; \
                                            bFM_GPIO_PCR2_P9=(v).bPullup; \
                                            bFM_GPIO_DDR2_P9=0u; \
                                            bFM_GPIO_PFR2_P9=0u; }while(0u)

#define GPIO1PIN_NP29_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN25=0u; \
                                            bFM_GPIO_PDOR2_P9=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR2_P9=1u; \
                                            bFM_GPIO_PFR2_P9=0u; }while(0u)

/*---- GPIO bit P2A ----*/
#define GPIO1PIN_P2A_GET                ( bFM_GPIO_PDIR2_PA )

#define GPIO1PIN_P2A_PUT(v)             ( bFM_GPIO_PDOR2_PA=(v) )

#define GPIO1PIN_P2A_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P2A_INITIN(v) \
                                                           : GPIO1PIN_P2A_INITOUT(v) )

#define GPIO1PIN_P2A_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN24=0u; \
                                            bFM_GPIO_PCR2_PA=(v).bPullup; \
                                            bFM_GPIO_DDR2_PA=0u; \
                                            bFM_GPIO_PFR2_PA=0u; }while(0u)

#define GPIO1PIN_P2A_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN24=0u; \
                                            bFM_GPIO_PDOR2_PA=(v).bInitVal; \
                                            bFM_GPIO_DDR2_PA=1u; \
                                            bFM_GPIO_PFR2_PA=0u; }while(0u)

/*---- GPIO bit NP2A ----*/
#define GPIO1PIN_NP2A_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR2_PA)) )

#define GPIO1PIN_NP2A_PUT(v)            ( bFM_GPIO_PDOR2_PA=(uint32_t)(!(v)) )

#define GPIO1PIN_NP2A_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP2A_INITIN(v) \
                                                           : GPIO1PIN_NP2A_INITOUT(v) )

#define GPIO1PIN_NP2A_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN24=0u; \
                                            bFM_GPIO_PCR2_PA=(v).bPullup; \
                                            bFM_GPIO_DDR2_PA=0u; \
                                            bFM_GPIO_PFR2_PA=0u; }while(0u)

#define GPIO1PIN_NP2A_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN24=0u; \
                                            bFM_GPIO_PDOR2_PA=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR2_PA=1u; \
                                            bFM_GPIO_PFR2_PA=0u; }while(0u)

/*---- GPIO bit P32 ----*/
#define GPIO1PIN_P32_GET                ( bFM_GPIO_PDIR3_P2 )

#define GPIO1PIN_P32_PUT(v)             ( bFM_GPIO_PDOR3_P2=(v) )

#define GPIO1PIN_P32_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P32_INITIN(v) \
                                                           : GPIO1PIN_P32_INITOUT(v) )

#define GPIO1PIN_P32_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_P2=(v).bPullup; \
                                            bFM_GPIO_DDR3_P2=0u; \
                                            bFM_GPIO_PFR3_P2=0u; }while(0u)

#define GPIO1PIN_P32_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_P2=(v).bInitVal; \
                                            bFM_GPIO_DDR3_P2=1u; \
                                            bFM_GPIO_PFR3_P2=0u; }while(0u)

/*---- GPIO bit NP32 ----*/
#define GPIO1PIN_NP32_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_P2)) )

#define GPIO1PIN_NP32_PUT(v)            ( bFM_GPIO_PDOR3_P2=(uint32_t)(!(v)) )

#define GPIO1PIN_NP32_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP32_INITIN(v) \
                                                           : GPIO1PIN_NP32_INITOUT(v) )

#define GPIO1PIN_NP32_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_P2=(v).bPullup; \
                                            bFM_GPIO_DDR3_P2=0u; \
                                            bFM_GPIO_PFR3_P2=0u; }while(0u)

#define GPIO1PIN_NP32_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_P2=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_P2=1u; \
                                            bFM_GPIO_PFR3_P2=0u; }while(0u)

/*---- GPIO bit P33 ----*/
#define GPIO1PIN_P33_GET                ( bFM_GPIO_PDIR3_P3 )

#define GPIO1PIN_P33_PUT(v)             ( bFM_GPIO_PDOR3_P3=(v) )

#define GPIO1PIN_P33_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P33_INITIN(v) \
                                                           : GPIO1PIN_P33_INITOUT(v) )

#define GPIO1PIN_P33_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_P3=(v).bPullup; \
                                            bFM_GPIO_DDR3_P3=0u; \
                                            bFM_GPIO_PFR3_P3=0u; }while(0u)

#define GPIO1PIN_P33_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_P3=(v).bInitVal; \
                                            bFM_GPIO_DDR3_P3=1u; \
                                            bFM_GPIO_PFR3_P3=0u; }while(0u)

/*---- GPIO bit NP33 ----*/
#define GPIO1PIN_NP33_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_P3)) )

#define GPIO1PIN_NP33_PUT(v)            ( bFM_GPIO_PDOR3_P3=(uint32_t)(!(v)) )

#define GPIO1PIN_NP33_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP33_INITIN(v) \
                                                           : GPIO1PIN_NP33_INITOUT(v) )

#define GPIO1PIN_NP33_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_P3=(v).bPullup; \
                                            bFM_GPIO_DDR3_P3=0u; \
                                            bFM_GPIO_PFR3_P3=0u; }while(0u)

#define GPIO1PIN_NP33_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_P3=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_P3=1u; \
                                            bFM_GPIO_PFR3_P3=0u; }while(0u)

/*---- GPIO bit P34 ----*/
#define GPIO1PIN_P34_GET                ( bFM_GPIO_PDIR3_P4 )

#define GPIO1PIN_P34_PUT(v)             ( bFM_GPIO_PDOR3_P4=(v) )

#define GPIO1PIN_P34_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P34_INITIN(v) \
                                                           : GPIO1PIN_P34_INITOUT(v) )

#define GPIO1PIN_P34_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_P4=(v).bPullup; \
                                            bFM_GPIO_DDR3_P4=0u; \
                                            bFM_GPIO_PFR3_P4=0u; }while(0u)

#define GPIO1PIN_P34_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_P4=(v).bInitVal; \
                                            bFM_GPIO_DDR3_P4=1u; \
                                            bFM_GPIO_PFR3_P4=0u; }while(0u)

/*---- GPIO bit NP34 ----*/
#define GPIO1PIN_NP34_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_P4)) )

#define GPIO1PIN_NP34_PUT(v)            ( bFM_GPIO_PDOR3_P4=(uint32_t)(!(v)) )

#define GPIO1PIN_NP34_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP34_INITIN(v) \
                                                           : GPIO1PIN_NP34_INITOUT(v) )

#define GPIO1PIN_NP34_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_P4=(v).bPullup; \
                                            bFM_GPIO_DDR3_P4=0u; \
                                            bFM_GPIO_PFR3_P4=0u; }while(0u)

#define GPIO1PIN_NP34_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_P4=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_P4=1u; \
                                            bFM_GPIO_PFR3_P4=0u; }while(0u)

/*---- GPIO bit P35 ----*/
#define GPIO1PIN_P35_GET                ( bFM_GPIO_PDIR3_P5 )

#define GPIO1PIN_P35_PUT(v)             ( bFM_GPIO_PDOR3_P5=(v) )

#define GPIO1PIN_P35_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P35_INITIN(v) \
                                                           : GPIO1PIN_P35_INITOUT(v) )

#define GPIO1PIN_P35_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_P5=(v).bPullup; \
                                            bFM_GPIO_DDR3_P5=0u; \
                                            bFM_GPIO_PFR3_P5=0u; }while(0u)

#define GPIO1PIN_P35_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_P5=(v).bInitVal; \
                                            bFM_GPIO_DDR3_P5=1u; \
                                            bFM_GPIO_PFR3_P5=0u; }while(0u)

/*---- GPIO bit NP35 ----*/
#define GPIO1PIN_NP35_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_P5)) )

#define GPIO1PIN_NP35_PUT(v)            ( bFM_GPIO_PDOR3_P5=(uint32_t)(!(v)) )

#define GPIO1PIN_NP35_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP35_INITIN(v) \
                                                           : GPIO1PIN_NP35_INITOUT(v) )

#define GPIO1PIN_NP35_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_P5=(v).bPullup; \
                                            bFM_GPIO_DDR3_P5=0u; \
                                            bFM_GPIO_PFR3_P5=0u; }while(0u)

#define GPIO1PIN_NP35_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_P5=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_P5=1u; \
                                            bFM_GPIO_PFR3_P5=0u; }while(0u)

/*---- GPIO bit P36 ----*/
#define GPIO1PIN_P36_GET                ( bFM_GPIO_PDIR3_P6 )

#define GPIO1PIN_P36_PUT(v)             ( bFM_GPIO_PDOR3_P6=(v) )

#define GPIO1PIN_P36_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P36_INITIN(v) \
                                                           : GPIO1PIN_P36_INITOUT(v) )

#define GPIO1PIN_P36_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_P6=(v).bPullup; \
                                            bFM_GPIO_DDR3_P6=0u; \
                                            bFM_GPIO_PFR3_P6=0u; }while(0u)

#define GPIO1PIN_P36_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_P6=(v).bInitVal; \
                                            bFM_GPIO_DDR3_P6=1u; \
                                            bFM_GPIO_PFR3_P6=0u; }while(0u)

/*---- GPIO bit NP36 ----*/
#define GPIO1PIN_NP36_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_P6)) )

#define GPIO1PIN_NP36_PUT(v)            ( bFM_GPIO_PDOR3_P6=(uint32_t)(!(v)) )

#define GPIO1PIN_NP36_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP36_INITIN(v) \
                                                           : GPIO1PIN_NP36_INITOUT(v) )

#define GPIO1PIN_NP36_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_P6=(v).bPullup; \
                                            bFM_GPIO_DDR3_P6=0u; \
                                            bFM_GPIO_PFR3_P6=0u; }while(0u)

#define GPIO1PIN_NP36_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_P6=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_P6=1u; \
                                            bFM_GPIO_PFR3_P6=0u; }while(0u)

/*---- GPIO bit P37 ----*/
#define GPIO1PIN_P37_GET                ( bFM_GPIO_PDIR3_P7 )

#define GPIO1PIN_P37_PUT(v)             ( bFM_GPIO_PDOR3_P7=(v) )

#define GPIO1PIN_P37_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P37_INITIN(v) \
                                                           : GPIO1PIN_P37_INITOUT(v) )

#define GPIO1PIN_P37_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_P7=(v).bPullup; \
                                            bFM_GPIO_DDR3_P7=0u; \
                                            bFM_GPIO_PFR3_P7=0u; }while(0u)

#define GPIO1PIN_P37_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_P7=(v).bInitVal; \
                                            bFM_GPIO_DDR3_P7=1u; \
                                            bFM_GPIO_PFR3_P7=0u; }while(0u)

/*---- GPIO bit NP37 ----*/
#define GPIO1PIN_NP37_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_P7)) )

#define GPIO1PIN_NP37_PUT(v)            ( bFM_GPIO_PDOR3_P7=(uint32_t)(!(v)) )

#define GPIO1PIN_NP37_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP37_INITIN(v) \
                                                           : GPIO1PIN_NP37_INITOUT(v) )

#define GPIO1PIN_NP37_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_P7=(v).bPullup; \
                                            bFM_GPIO_DDR3_P7=0u; \
                                            bFM_GPIO_PFR3_P7=0u; }while(0u)

#define GPIO1PIN_NP37_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_P7=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_P7=1u; \
                                            bFM_GPIO_PFR3_P7=0u; }while(0u)

/*---- GPIO bit P38 ----*/
#define GPIO1PIN_P38_GET                ( bFM_GPIO_PDIR3_P8 )

#define GPIO1PIN_P38_PUT(v)             ( bFM_GPIO_PDOR3_P8=(v) )

#define GPIO1PIN_P38_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P38_INITIN(v) \
                                                           : GPIO1PIN_P38_INITOUT(v) )

#define GPIO1PIN_P38_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_P8=(v).bPullup; \
                                            bFM_GPIO_DDR3_P8=0u; \
                                            bFM_GPIO_PFR3_P8=0u; }while(0u)

#define GPIO1PIN_P38_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_P8=(v).bInitVal; \
                                            bFM_GPIO_DDR3_P8=1u; \
                                            bFM_GPIO_PFR3_P8=0u; }while(0u)

/*---- GPIO bit NP38 ----*/
#define GPIO1PIN_NP38_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_P8)) )

#define GPIO1PIN_NP38_PUT(v)            ( bFM_GPIO_PDOR3_P8=(uint32_t)(!(v)) )

#define GPIO1PIN_NP38_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP38_INITIN(v) \
                                                           : GPIO1PIN_NP38_INITOUT(v) )

#define GPIO1PIN_NP38_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_P8=(v).bPullup; \
                                            bFM_GPIO_DDR3_P8=0u; \
                                            bFM_GPIO_PFR3_P8=0u; }while(0u)

#define GPIO1PIN_NP38_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_P8=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_P8=1u; \
                                            bFM_GPIO_PFR3_P8=0u; }while(0u)

/*---- GPIO bit P39 ----*/
#define GPIO1PIN_P39_GET                ( bFM_GPIO_PDIR3_P9 )

#define GPIO1PIN_P39_PUT(v)             ( bFM_GPIO_PDOR3_P9=(v) )

#define GPIO1PIN_P39_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P39_INITIN(v) \
                                                           : GPIO1PIN_P39_INITOUT(v) )

#define GPIO1PIN_P39_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_P9=(v).bPullup; \
                                            bFM_GPIO_DDR3_P9=0u; \
                                            bFM_GPIO_PFR3_P9=0u; }while(0u)

#define GPIO1PIN_P39_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_P9=(v).bInitVal; \
                                            bFM_GPIO_DDR3_P9=1u; \
                                            bFM_GPIO_PFR3_P9=0u; }while(0u)

/*---- GPIO bit NP39 ----*/
#define GPIO1PIN_NP39_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_P9)) )

#define GPIO1PIN_NP39_PUT(v)            ( bFM_GPIO_PDOR3_P9=(uint32_t)(!(v)) )

#define GPIO1PIN_NP39_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP39_INITIN(v) \
                                                           : GPIO1PIN_NP39_INITOUT(v) )

#define GPIO1PIN_NP39_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_P9=(v).bPullup; \
                                            bFM_GPIO_DDR3_P9=0u; \
                                            bFM_GPIO_PFR3_P9=0u; }while(0u)

#define GPIO1PIN_NP39_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_P9=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_P9=1u; \
                                            bFM_GPIO_PFR3_P9=0u; }while(0u)

/*---- GPIO bit P3A ----*/
#define GPIO1PIN_P3A_GET                ( bFM_GPIO_PDIR3_PA )

#define GPIO1PIN_P3A_PUT(v)             ( bFM_GPIO_PDOR3_PA=(v) )

#define GPIO1PIN_P3A_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P3A_INITIN(v) \
                                                           : GPIO1PIN_P3A_INITOUT(v) )

#define GPIO1PIN_P3A_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_PA=(v).bPullup; \
                                            bFM_GPIO_DDR3_PA=0u; \
                                            bFM_GPIO_PFR3_PA=0u; }while(0u)

#define GPIO1PIN_P3A_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_PA=(v).bInitVal; \
                                            bFM_GPIO_DDR3_PA=1u; \
                                            bFM_GPIO_PFR3_PA=0u; }while(0u)

/*---- GPIO bit NP3A ----*/
#define GPIO1PIN_NP3A_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_PA)) )

#define GPIO1PIN_NP3A_PUT(v)            ( bFM_GPIO_PDOR3_PA=(uint32_t)(!(v)) )

#define GPIO1PIN_NP3A_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP3A_INITIN(v) \
                                                           : GPIO1PIN_NP3A_INITOUT(v) )

#define GPIO1PIN_NP3A_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_PA=(v).bPullup; \
                                            bFM_GPIO_DDR3_PA=0u; \
                                            bFM_GPIO_PFR3_PA=0u; }while(0u)

#define GPIO1PIN_NP3A_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_PA=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_PA=1u; \
                                            bFM_GPIO_PFR3_PA=0u; }while(0u)

/*---- GPIO bit P3B ----*/
#define GPIO1PIN_P3B_GET                ( bFM_GPIO_PDIR3_PB )

#define GPIO1PIN_P3B_PUT(v)             ( bFM_GPIO_PDOR3_PB=(v) )

#define GPIO1PIN_P3B_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P3B_INITIN(v) \
                                                           : GPIO1PIN_P3B_INITOUT(v) )

#define GPIO1PIN_P3B_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_PB=(v).bPullup; \
                                            bFM_GPIO_DDR3_PB=0u; \
                                            bFM_GPIO_PFR3_PB=0u; }while(0u)

#define GPIO1PIN_P3B_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_PB=(v).bInitVal; \
                                            bFM_GPIO_DDR3_PB=1u; \
                                            bFM_GPIO_PFR3_PB=0u; }while(0u)

/*---- GPIO bit NP3B ----*/
#define GPIO1PIN_NP3B_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_PB)) )

#define GPIO1PIN_NP3B_PUT(v)            ( bFM_GPIO_PDOR3_PB=(uint32_t)(!(v)) )

#define GPIO1PIN_NP3B_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP3B_INITIN(v) \
                                                           : GPIO1PIN_NP3B_INITOUT(v) )

#define GPIO1PIN_NP3B_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_PB=(v).bPullup; \
                                            bFM_GPIO_DDR3_PB=0u; \
                                            bFM_GPIO_PFR3_PB=0u; }while(0u)

#define GPIO1PIN_NP3B_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_PB=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_PB=1u; \
                                            bFM_GPIO_PFR3_PB=0u; }while(0u)

/*---- GPIO bit P3C ----*/
#define GPIO1PIN_P3C_GET                ( bFM_GPIO_PDIR3_PC )

#define GPIO1PIN_P3C_PUT(v)             ( bFM_GPIO_PDOR3_PC=(v) )

#define GPIO1PIN_P3C_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P3C_INITIN(v) \
                                                           : GPIO1PIN_P3C_INITOUT(v) )

#define GPIO1PIN_P3C_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_PC=(v).bPullup; \
                                            bFM_GPIO_DDR3_PC=0u; \
                                            bFM_GPIO_PFR3_PC=0u; }while(0u)

#define GPIO1PIN_P3C_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_PC=(v).bInitVal; \
                                            bFM_GPIO_DDR3_PC=1u; \
                                            bFM_GPIO_PFR3_PC=0u; }while(0u)

/*---- GPIO bit NP3C ----*/
#define GPIO1PIN_NP3C_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_PC)) )

#define GPIO1PIN_NP3C_PUT(v)            ( bFM_GPIO_PDOR3_PC=(uint32_t)(!(v)) )

#define GPIO1PIN_NP3C_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP3C_INITIN(v) \
                                                           : GPIO1PIN_NP3C_INITOUT(v) )

#define GPIO1PIN_NP3C_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_PC=(v).bPullup; \
                                            bFM_GPIO_DDR3_PC=0u; \
                                            bFM_GPIO_PFR3_PC=0u; }while(0u)

#define GPIO1PIN_NP3C_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_PC=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_PC=1u; \
                                            bFM_GPIO_PFR3_PC=0u; }while(0u)

/*---- GPIO bit P3D ----*/
#define GPIO1PIN_P3D_GET                ( bFM_GPIO_PDIR3_PD )

#define GPIO1PIN_P3D_PUT(v)             ( bFM_GPIO_PDOR3_PD=(v) )

#define GPIO1PIN_P3D_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P3D_INITIN(v) \
                                                           : GPIO1PIN_P3D_INITOUT(v) )

#define GPIO1PIN_P3D_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_PD=(v).bPullup; \
                                            bFM_GPIO_DDR3_PD=0u; \
                                            bFM_GPIO_PFR3_PD=0u; }while(0u)

#define GPIO1PIN_P3D_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_PD=(v).bInitVal; \
                                            bFM_GPIO_DDR3_PD=1u; \
                                            bFM_GPIO_PFR3_PD=0u; }while(0u)

/*---- GPIO bit NP3D ----*/
#define GPIO1PIN_NP3D_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_PD)) )

#define GPIO1PIN_NP3D_PUT(v)            ( bFM_GPIO_PDOR3_PD=(uint32_t)(!(v)) )

#define GPIO1PIN_NP3D_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP3D_INITIN(v) \
                                                           : GPIO1PIN_NP3D_INITOUT(v) )

#define GPIO1PIN_NP3D_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_PD=(v).bPullup; \
                                            bFM_GPIO_DDR3_PD=0u; \
                                            bFM_GPIO_PFR3_PD=0u; }while(0u)

#define GPIO1PIN_NP3D_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_PD=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_PD=1u; \
                                            bFM_GPIO_PFR3_PD=0u; }while(0u)

/*---- GPIO bit P3E ----*/
#define GPIO1PIN_P3E_GET                ( bFM_GPIO_PDIR3_PE )

#define GPIO1PIN_P3E_PUT(v)             ( bFM_GPIO_PDOR3_PE=(v) )

#define GPIO1PIN_P3E_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P3E_INITIN(v) \
                                                           : GPIO1PIN_P3E_INITOUT(v) )

#define GPIO1PIN_P3E_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_PE=(v).bPullup; \
                                            bFM_GPIO_DDR3_PE=0u; \
                                            bFM_GPIO_PFR3_PE=0u; }while(0u)

#define GPIO1PIN_P3E_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_PE=(v).bInitVal; \
                                            bFM_GPIO_DDR3_PE=1u; \
                                            bFM_GPIO_PFR3_PE=0u; }while(0u)

/*---- GPIO bit NP3E ----*/
#define GPIO1PIN_NP3E_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_PE)) )

#define GPIO1PIN_NP3E_PUT(v)            ( bFM_GPIO_PDOR3_PE=(uint32_t)(!(v)) )

#define GPIO1PIN_NP3E_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP3E_INITIN(v) \
                                                           : GPIO1PIN_NP3E_INITOUT(v) )

#define GPIO1PIN_NP3E_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_PE=(v).bPullup; \
                                            bFM_GPIO_DDR3_PE=0u; \
                                            bFM_GPIO_PFR3_PE=0u; }while(0u)

#define GPIO1PIN_NP3E_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_PE=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_PE=1u; \
                                            bFM_GPIO_PFR3_PE=0u; }while(0u)

/*---- GPIO bit P40 ----*/
#define GPIO1PIN_P40_GET                ( bFM_GPIO_PDIR4_P0 )

#define GPIO1PIN_P40_PUT(v)             ( bFM_GPIO_PDOR4_P0=(v) )

#define GPIO1PIN_P40_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P40_INITIN(v) \
                                                           : GPIO1PIN_P40_INITOUT(v) )

#define GPIO1PIN_P40_INITIN(v)          do{ \
                                            bFM_GPIO_PCR4_P0=(v).bPullup; \
                                            bFM_GPIO_DDR4_P0=0u; \
                                            bFM_GPIO_PFR4_P0=0u; }while(0u)

#define GPIO1PIN_P40_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR4_P0=(v).bInitVal; \
                                            bFM_GPIO_DDR4_P0=1u; \
                                            bFM_GPIO_PFR4_P0=0u; }while(0u)

/*---- GPIO bit NP40 ----*/
#define GPIO1PIN_NP40_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR4_P0)) )

#define GPIO1PIN_NP40_PUT(v)            ( bFM_GPIO_PDOR4_P0=(uint32_t)(!(v)) )

#define GPIO1PIN_NP40_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP40_INITIN(v) \
                                                           : GPIO1PIN_NP40_INITOUT(v) )

#define GPIO1PIN_NP40_INITIN(v)         do{ \
                                            bFM_GPIO_PCR4_P0=(v).bPullup; \
                                            bFM_GPIO_DDR4_P0=0u; \
                                            bFM_GPIO_PFR4_P0=0u; }while(0u)

#define GPIO1PIN_NP40_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR4_P0=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR4_P0=1u; \
                                            bFM_GPIO_PFR4_P0=0u; }while(0u)

/*---- GPIO bit P41 ----*/
#define GPIO1PIN_P41_GET                ( bFM_GPIO_PDIR4_P1 )

#define GPIO1PIN_P41_PUT(v)             ( bFM_GPIO_PDOR4_P1=(v) )

#define GPIO1PIN_P41_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P41_INITIN(v) \
                                                           : GPIO1PIN_P41_INITOUT(v) )

#define GPIO1PIN_P41_INITIN(v)          do{ \
                                            bFM_GPIO_PCR4_P1=(v).bPullup; \
                                            bFM_GPIO_DDR4_P1=0u; \
                                            bFM_GPIO_PFR4_P1=0u; }while(0u)

#define GPIO1PIN_P41_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR4_P1=(v).bInitVal; \
                                            bFM_GPIO_DDR4_P1=1u; \
                                            bFM_GPIO_PFR4_P1=0u; }while(0u)

/*---- GPIO bit NP41 ----*/
#define GPIO1PIN_NP41_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR4_P1)) )

#define GPIO1PIN_NP41_PUT(v)            ( bFM_GPIO_PDOR4_P1=(uint32_t)(!(v)) )

#define GPIO1PIN_NP41_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP41_INITIN(v) \
                                                           : GPIO1PIN_NP41_INITOUT(v) )

#define GPIO1PIN_NP41_INITIN(v)         do{ \
                                            bFM_GPIO_PCR4_P1=(v).bPullup; \
                                            bFM_GPIO_DDR4_P1=0u; \
                                            bFM_GPIO_PFR4_P1=0u; }while(0u)

#define GPIO1PIN_NP41_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR4_P1=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR4_P1=1u; \
                                            bFM_GPIO_PFR4_P1=0u; }while(0u)

/*---- GPIO bit P42 ----*/
#define GPIO1PIN_P42_GET                ( bFM_GPIO_PDIR4_P2 )

#define GPIO1PIN_P42_PUT(v)             ( bFM_GPIO_PDOR4_P2=(v) )

#define GPIO1PIN_P42_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P42_INITIN(v) \
                                                           : GPIO1PIN_P42_INITOUT(v) )

#define GPIO1PIN_P42_INITIN(v)          do{ \
                                            bFM_GPIO_PCR4_P2=(v).bPullup; \
                                            bFM_GPIO_DDR4_P2=0u; \
                                            bFM_GPIO_PFR4_P2=0u; }while(0u)

#define GPIO1PIN_P42_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR4_P2=(v).bInitVal; \
                                            bFM_GPIO_DDR4_P2=1u; \
                                            bFM_GPIO_PFR4_P2=0u; }while(0u)

/*---- GPIO bit NP42 ----*/
#define GPIO1PIN_NP42_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR4_P2)) )

#define GPIO1PIN_NP42_PUT(v)            ( bFM_GPIO_PDOR4_P2=(uint32_t)(!(v)) )

#define GPIO1PIN_NP42_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP42_INITIN(v) \
                                                           : GPIO1PIN_NP42_INITOUT(v) )

#define GPIO1PIN_NP42_INITIN(v)         do{ \
                                            bFM_GPIO_PCR4_P2=(v).bPullup; \
                                            bFM_GPIO_DDR4_P2=0u; \
                                            bFM_GPIO_PFR4_P2=0u; }while(0u)

#define GPIO1PIN_NP42_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR4_P2=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR4_P2=1u; \
                                            bFM_GPIO_PFR4_P2=0u; }while(0u)

/*---- GPIO bit P43 ----*/
#define GPIO1PIN_P43_GET                ( bFM_GPIO_PDIR4_P3 )

#define GPIO1PIN_P43_PUT(v)             ( bFM_GPIO_PDOR4_P3=(v) )

#define GPIO1PIN_P43_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P43_INITIN(v) \
                                                           : GPIO1PIN_P43_INITOUT(v) )

#define GPIO1PIN_P43_INITIN(v)          do{ \
                                            bFM_GPIO_PCR4_P3=(v).bPullup; \
                                            bFM_GPIO_DDR4_P3=0u; \
                                            bFM_GPIO_PFR4_P3=0u; }while(0u)

#define GPIO1PIN_P43_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR4_P3=(v).bInitVal; \
                                            bFM_GPIO_DDR4_P3=1u; \
                                            bFM_GPIO_PFR4_P3=0u; }while(0u)

/*---- GPIO bit NP43 ----*/
#define GPIO1PIN_NP43_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR4_P3)) )

#define GPIO1PIN_NP43_PUT(v)            ( bFM_GPIO_PDOR4_P3=(uint32_t)(!(v)) )

#define GPIO1PIN_NP43_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP43_INITIN(v) \
                                                           : GPIO1PIN_NP43_INITOUT(v) )

#define GPIO1PIN_NP43_INITIN(v)         do{ \
                                            bFM_GPIO_PCR4_P3=(v).bPullup; \
                                            bFM_GPIO_DDR4_P3=0u; \
                                            bFM_GPIO_PFR4_P3=0u; }while(0u)

#define GPIO1PIN_NP43_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR4_P3=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR4_P3=1u; \
                                            bFM_GPIO_PFR4_P3=0u; }while(0u)

/*---- GPIO bit P44 ----*/
#define GPIO1PIN_P44_GET                ( bFM_GPIO_PDIR4_P4 )

#define GPIO1PIN_P44_PUT(v)             ( bFM_GPIO_PDOR4_P4=(v) )

#define GPIO1PIN_P44_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P44_INITIN(v) \
                                                           : GPIO1PIN_P44_INITOUT(v) )

#define GPIO1PIN_P44_INITIN(v)          do{ \
                                            bFM_GPIO_PCR4_P4=(v).bPullup; \
                                            bFM_GPIO_DDR4_P4=0u; \
                                            bFM_GPIO_PFR4_P4=0u; }while(0u)

#define GPIO1PIN_P44_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR4_P4=(v).bInitVal; \
                                            bFM_GPIO_DDR4_P4=1u; \
                                            bFM_GPIO_PFR4_P4=0u; }while(0u)

/*---- GPIO bit NP44 ----*/
#define GPIO1PIN_NP44_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR4_P4)) )

#define GPIO1PIN_NP44_PUT(v)            ( bFM_GPIO_PDOR4_P4=(uint32_t)(!(v)) )

#define GPIO1PIN_NP44_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP44_INITIN(v) \
                                                           : GPIO1PIN_NP44_INITOUT(v) )

#define GPIO1PIN_NP44_INITIN(v)         do{ \
                                            bFM_GPIO_PCR4_P4=(v).bPullup; \
                                            bFM_GPIO_DDR4_P4=0u; \
                                            bFM_GPIO_PFR4_P4=0u; }while(0u)

#define GPIO1PIN_NP44_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR4_P4=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR4_P4=1u; \
                                            bFM_GPIO_PFR4_P4=0u; }while(0u)

/*---- GPIO bit P45 ----*/
#define GPIO1PIN_P45_GET                ( bFM_GPIO_PDIR4_P5 )

#define GPIO1PIN_P45_PUT(v)             ( bFM_GPIO_PDOR4_P5=(v) )

#define GPIO1PIN_P45_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P45_INITIN(v) \
                                                           : GPIO1PIN_P45_INITOUT(v) )

#define GPIO1PIN_P45_INITIN(v)          do{ \
                                            bFM_GPIO_PCR4_P5=(v).bPullup; \
                                            bFM_GPIO_DDR4_P5=0u; \
                                            bFM_GPIO_PFR4_P5=0u; }while(0u)

#define GPIO1PIN_P45_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR4_P5=(v).bInitVal; \
                                            bFM_GPIO_DDR4_P5=1u; \
                                            bFM_GPIO_PFR4_P5=0u; }while(0u)

/*---- GPIO bit NP45 ----*/
#define GPIO1PIN_NP45_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR4_P5)) )

#define GPIO1PIN_NP45_PUT(v)            ( bFM_GPIO_PDOR4_P5=(uint32_t)(!(v)) )

#define GPIO1PIN_NP45_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP45_INITIN(v) \
                                                           : GPIO1PIN_NP45_INITOUT(v) )

#define GPIO1PIN_NP45_INITIN(v)         do{ \
                                            bFM_GPIO_PCR4_P5=(v).bPullup; \
                                            bFM_GPIO_DDR4_P5=0u; \
                                            bFM_GPIO_PFR4_P5=0u; }while(0u)

#define GPIO1PIN_NP45_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR4_P5=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR4_P5=1u; \
                                            bFM_GPIO_PFR4_P5=0u; }while(0u)

/*---- GPIO bit P46 ----*/
#define GPIO1PIN_P46_GET                ( bFM_RTC_VBDIR_VDIR3 )

#define GPIO1PIN_P46_PUT(v)             do{ FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR3=(v); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        } while(0u)

#define GPIO1PIN_P46_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P46_INITIN(v) \
                                                           : GPIO1PIN_P46_INITOUT(v) )

#define GPIO1PIN_P46_INITIN(v)          do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBPCR_VPCR3=(v).bPullup; \
                                            bFM_RTC_VBDDR_VDDR3=0u; \
                                            PINCONFIG_SET_REG(FM_RTC->VBPFR, 4u, 2u, 0x00u); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

#define GPIO1PIN_P46_INITOUT(v)      do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR3=(v).bInitVal; \
                                            bFM_RTC_VBDDR_VDDR3=1u; \
                                            PINCONFIG_SET_REG(FM_RTC->VBPFR, 4u, 2u, 0x00u); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

/*---- GPIO bit NP46 ----*/
#define GPIO1PIN_NP46_GET               ( (uint32_t)(!(uint32_t)(bFM_RTC_VBDIR_VDIR3)) )

#define GPIO1PIN_NP46_PUT(v)            do{ FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR3=(uint32_t)(!(v)); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        } while(0u)

#define GPIO1PIN_NP46_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP46_INITIN(v) \
                                                           : GPIO1PIN_NP46_INITOUT(v) )

#define GPIO1PIN_NP46_INITIN(v)         do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBPCR_VPCR3=(v).bPullup; \
                                            bFM_RTC_VBDDR_VDDR3=0u; \
                                            PINCONFIG_SET_REG(FM_RTC->VBPFR, 4u, 2u, 0x00u); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

#define GPIO1PIN_NP46_INITOUT(v)        do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR3=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_RTC_VBDDR_VDDR3=1u; \
                                            PINCONFIG_SET_REG(FM_RTC->VBPFR, 4u, 2u, 0x00u); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

/*---- GPIO bit P47 ----*/
#define GPIO1PIN_P47_GET                ( bFM_RTC_VBDIR_VDIR2 )

#define GPIO1PIN_P47_PUT(v)             do{ FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR2=(v); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        } while(0u)

#define GPIO1PIN_P47_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P47_INITIN(v) \
                                                           : GPIO1PIN_P47_INITOUT(v) )

#define GPIO1PIN_P47_INITIN(v)          do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBPCR_VPCR2=(v).bPullup; \
                                            bFM_RTC_VBDDR_VDDR2=0u; \
                                            PINCONFIG_SET_REG(FM_RTC->VBPFR, 4u, 2u, 0x00u); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

#define GPIO1PIN_P47_INITOUT(v)      do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR2=(v).bInitVal; \
                                            bFM_RTC_VBDDR_VDDR2=1u; \
                                            PINCONFIG_SET_REG(FM_RTC->VBPFR, 4u, 2u, 0x00u); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

/*---- GPIO bit NP47 ----*/
#define GPIO1PIN_NP47_GET               ( (uint32_t)(!(uint32_t)(bFM_RTC_VBDIR_VDIR2)) )

#define GPIO1PIN_NP47_PUT(v)            do{ FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR2=(uint32_t)(!(v)); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        } while(0u)

#define GPIO1PIN_NP47_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP47_INITIN(v) \
                                                           : GPIO1PIN_NP47_INITOUT(v) )

#define GPIO1PIN_NP47_INITIN(v)         do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBPCR_VPCR2=(v).bPullup; \
                                            bFM_RTC_VBDDR_VDDR2=0u; \
                                            PINCONFIG_SET_REG(FM_RTC->VBPFR, 4u, 2u, 0x00u); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

#define GPIO1PIN_NP47_INITOUT(v)        do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR2=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_RTC_VBDDR_VDDR2=1u; \
                                            PINCONFIG_SET_REG(FM_RTC->VBPFR, 4u, 2u, 0x00u); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

/*---- GPIO bit P48 ----*/
#define GPIO1PIN_P48_GET                ( bFM_RTC_VBDIR_VDIR0 )

#define GPIO1PIN_P48_PUT(v)             do{ FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR0=(v); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        } while(0u)

#define GPIO1PIN_P48_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P48_INITIN(v) \
                                                           : GPIO1PIN_P48_INITOUT(v) )

#define GPIO1PIN_P48_INITIN(v)          do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBPCR_VPCR0=(v).bPullup; \
                                            bFM_RTC_VBDDR_VDDR0=0u; \
                                            bFM_RTC_VBPFR_VPFR0=0u; \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

#define GPIO1PIN_P48_INITOUT(v)      do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR0=(v).bInitVal; \
                                            bFM_RTC_VBDDR_VDDR0=1u; \
                                            bFM_RTC_VBPFR_VPFR0=0u; \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

/*---- GPIO bit NP48 ----*/
#define GPIO1PIN_NP48_GET               ( (uint32_t)(!(uint32_t)(bFM_RTC_VBDIR_VDIR0)) )

#define GPIO1PIN_NP48_PUT(v)            do{ FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR0=(uint32_t)(!(v)); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        } while(0u)

#define GPIO1PIN_NP48_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP48_INITIN(v) \
                                                           : GPIO1PIN_NP48_INITOUT(v) )

#define GPIO1PIN_NP48_INITIN(v)         do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBPCR_VPCR0=(v).bPullup; \
                                            bFM_RTC_VBDDR_VDDR0=0u; \
                                            bFM_RTC_VBPFR_VPFR0=0u; \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

#define GPIO1PIN_NP48_INITOUT(v)        do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR0=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_RTC_VBDDR_VDDR0=1u; \
                                            bFM_RTC_VBPFR_VPFR0=0u; \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

/*---- GPIO bit P49 ----*/
#define GPIO1PIN_P49_GET                ( bFM_RTC_VBDIR_VDIR1 )

#define GPIO1PIN_P49_PUT(v)             do{ FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR1=(v); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        } while(0u)

#define GPIO1PIN_P49_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P49_INITIN(v) \
                                                           : GPIO1PIN_P49_INITOUT(v) )

#define GPIO1PIN_P49_INITIN(v)          do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBPCR_VPCR1=(v).bPullup; \
                                            bFM_RTC_VBDDR_VDDR1=0u; \
                                            bFM_RTC_VBPFR_VPFR1=0u; \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

#define GPIO1PIN_P49_INITOUT(v)      do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR1=(v).bInitVal; \
                                            bFM_RTC_VBDDR_VDDR1=1u; \
                                            bFM_RTC_VBPFR_VPFR1=0u; \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

/*---- GPIO bit NP49 ----*/
#define GPIO1PIN_NP49_GET               ( (uint32_t)(!(uint32_t)(bFM_RTC_VBDIR_VDIR1)) )

#define GPIO1PIN_NP49_PUT(v)            do{ FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR1=(uint32_t)(!(v)); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        } while(0u)

#define GPIO1PIN_NP49_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP49_INITIN(v) \
                                                           : GPIO1PIN_NP49_INITOUT(v) )

#define GPIO1PIN_NP49_INITIN(v)         do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBPCR_VPCR1=(v).bPullup; \
                                            bFM_RTC_VBDDR_VDDR1=0u; \
                                            bFM_RTC_VBPFR_VPFR1=0u; \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

#define GPIO1PIN_NP49_INITOUT(v)        do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR1=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_RTC_VBDDR_VDDR1=1u; \
                                            bFM_RTC_VBPFR_VPFR1=0u; \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

/*---- GPIO bit P60 ----*/
#define GPIO1PIN_P60_GET                ( bFM_GPIO_PDIR6_P0 )

#define GPIO1PIN_P60_PUT(v)             ( bFM_GPIO_PDOR6_P0=(v) )

#define GPIO1PIN_P60_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P60_INITIN(v) \
                                                           : GPIO1PIN_P60_INITOUT(v) )

#define GPIO1PIN_P60_INITIN(v)          do{ \
                                            bFM_GPIO_PCR6_P0=(v).bPullup; \
                                            bFM_GPIO_DDR6_P0=0u; \
                                            bFM_GPIO_PFR6_P0=0u; }while(0u)

#define GPIO1PIN_P60_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR6_P0=(v).bInitVal; \
                                            bFM_GPIO_DDR6_P0=1u; \
                                            bFM_GPIO_PFR6_P0=0u; }while(0u)

/*---- GPIO bit NP60 ----*/
#define GPIO1PIN_NP60_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR6_P0)) )

#define GPIO1PIN_NP60_PUT(v)            ( bFM_GPIO_PDOR6_P0=(uint32_t)(!(v)) )

#define GPIO1PIN_NP60_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP60_INITIN(v) \
                                                           : GPIO1PIN_NP60_INITOUT(v) )

#define GPIO1PIN_NP60_INITIN(v)         do{ \
                                            bFM_GPIO_PCR6_P0=(v).bPullup; \
                                            bFM_GPIO_DDR6_P0=0u; \
                                            bFM_GPIO_PFR6_P0=0u; }while(0u)

#define GPIO1PIN_NP60_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR6_P0=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR6_P0=1u; \
                                            bFM_GPIO_PFR6_P0=0u; }while(0u)

/*---- GPIO bit P61 ----*/
#define GPIO1PIN_P61_GET                ( bFM_GPIO_PDIR6_P1 )

#define GPIO1PIN_P61_PUT(v)             ( bFM_GPIO_PDOR6_P1=(v) )

#define GPIO1PIN_P61_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P61_INITIN(v) \
                                                           : GPIO1PIN_P61_INITOUT(v) )

#define GPIO1PIN_P61_INITIN(v)          do{ \
                                            bFM_GPIO_PCR6_P1=(v).bPullup; \
                                            bFM_GPIO_DDR6_P1=0u; \
                                            bFM_GPIO_PFR6_P1=0u; }while(0u)

#define GPIO1PIN_P61_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR6_P1=(v).bInitVal; \
                                            bFM_GPIO_DDR6_P1=1u; \
                                            bFM_GPIO_PFR6_P1=0u; }while(0u)

/*---- GPIO bit NP61 ----*/
#define GPIO1PIN_NP61_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR6_P1)) )

#define GPIO1PIN_NP61_PUT(v)            ( bFM_GPIO_PDOR6_P1=(uint32_t)(!(v)) )

#define GPIO1PIN_NP61_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP61_INITIN(v) \
                                                           : GPIO1PIN_NP61_INITOUT(v) )

#define GPIO1PIN_NP61_INITIN(v)         do{ \
                                            bFM_GPIO_PCR6_P1=(v).bPullup; \
                                            bFM_GPIO_DDR6_P1=0u; \
                                            bFM_GPIO_PFR6_P1=0u; }while(0u)

#define GPIO1PIN_NP61_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR6_P1=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR6_P1=1u; \
                                            bFM_GPIO_PFR6_P1=0u; }while(0u)

/*---- GPIO bit P62 ----*/
#define GPIO1PIN_P62_GET                ( bFM_GPIO_PDIR6_P2 )

#define GPIO1PIN_P62_PUT(v)             ( bFM_GPIO_PDOR6_P2=(v) )

#define GPIO1PIN_P62_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P62_INITIN(v) \
                                                           : GPIO1PIN_P62_INITOUT(v) )

#define GPIO1PIN_P62_INITIN(v)          do{ \
                                            bFM_GPIO_PCR6_P2=(v).bPullup; \
                                            bFM_GPIO_DDR6_P2=0u; \
                                            bFM_GPIO_PFR6_P2=0u; }while(0u)

#define GPIO1PIN_P62_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR6_P2=(v).bInitVal; \
                                            bFM_GPIO_DDR6_P2=1u; \
                                            bFM_GPIO_PFR6_P2=0u; }while(0u)

/*---- GPIO bit NP62 ----*/
#define GPIO1PIN_NP62_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR6_P2)) )

#define GPIO1PIN_NP62_PUT(v)            ( bFM_GPIO_PDOR6_P2=(uint32_t)(!(v)) )

#define GPIO1PIN_NP62_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP62_INITIN(v) \
                                                           : GPIO1PIN_NP62_INITOUT(v) )

#define GPIO1PIN_NP62_INITIN(v)         do{ \
                                            bFM_GPIO_PCR6_P2=(v).bPullup; \
                                            bFM_GPIO_DDR6_P2=0u; \
                                            bFM_GPIO_PFR6_P2=0u; }while(0u)

#define GPIO1PIN_NP62_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR6_P2=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR6_P2=1u; \
                                            bFM_GPIO_PFR6_P2=0u; }while(0u)

/*---- GPIO bit P63 ----*/
#define GPIO1PIN_P63_GET                ( bFM_GPIO_PDIR6_P3 )

#define GPIO1PIN_P63_PUT(v)             ( bFM_GPIO_PDOR6_P3=(v) )

#define GPIO1PIN_P63_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P63_INITIN(v) \
                                                           : GPIO1PIN_P63_INITOUT(v) )

#define GPIO1PIN_P63_INITIN(v)          do{ \
                                            bFM_GPIO_PCR6_P3=(v).bPullup; \
                                            bFM_GPIO_DDR6_P3=0u; \
                                            bFM_GPIO_PFR6_P3=0u; }while(0u)

#define GPIO1PIN_P63_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR6_P3=(v).bInitVal; \
                                            bFM_GPIO_DDR6_P3=1u; \
                                            bFM_GPIO_PFR6_P3=0u; }while(0u)

/*---- GPIO bit NP63 ----*/
#define GPIO1PIN_NP63_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR6_P3)) )

#define GPIO1PIN_NP63_PUT(v)            ( bFM_GPIO_PDOR6_P3=(uint32_t)(!(v)) )

#define GPIO1PIN_NP63_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP63_INITIN(v) \
                                                           : GPIO1PIN_NP63_INITOUT(v) )

#define GPIO1PIN_NP63_INITIN(v)         do{ \
                                            bFM_GPIO_PCR6_P3=(v).bPullup; \
                                            bFM_GPIO_DDR6_P3=0u; \
                                            bFM_GPIO_PFR6_P3=0u; }while(0u)

#define GPIO1PIN_NP63_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR6_P3=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR6_P3=1u; \
                                            bFM_GPIO_PFR6_P3=0u; }while(0u)

/*---- GPIO bit P6E ----*/
#define GPIO1PIN_P6E_GET                ( bFM_GPIO_PDIR6_PE )

#define GPIO1PIN_P6E_PUT(v)             ( bFM_GPIO_PDOR6_PE=(v) )

#define GPIO1PIN_P6E_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P6E_INITIN(v) \
                                                           : GPIO1PIN_P6E_INITOUT(v) )

#define GPIO1PIN_P6E_INITIN(v)          do{ \
                                            bFM_GPIO_PCR6_PE=(v).bPullup; \
                                            bFM_GPIO_DDR6_PE=0u; \
                                            bFM_GPIO_PFR6_PE=0u; }while(0u)

#define GPIO1PIN_P6E_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR6_PE=(v).bInitVal; \
                                            bFM_GPIO_DDR6_PE=1u; \
                                            bFM_GPIO_PFR6_PE=0u; }while(0u)

/*---- GPIO bit NP6E ----*/
#define GPIO1PIN_NP6E_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR6_PE)) )

#define GPIO1PIN_NP6E_PUT(v)            ( bFM_GPIO_PDOR6_PE=(uint32_t)(!(v)) )

#define GPIO1PIN_NP6E_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP6E_INITIN(v) \
                                                           : GPIO1PIN_NP6E_INITOUT(v) )

#define GPIO1PIN_NP6E_INITIN(v)         do{ \
                                            bFM_GPIO_PCR6_PE=(v).bPullup; \
                                            bFM_GPIO_DDR6_PE=0u; \
                                            bFM_GPIO_PFR6_PE=0u; }while(0u)

#define GPIO1PIN_NP6E_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR6_PE=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR6_PE=1u; \
                                            bFM_GPIO_PFR6_PE=0u; }while(0u)

/*---- GPIO bit P70 ----*/
#define GPIO1PIN_P70_GET                ( bFM_GPIO_PDIR7_P0 )

#define GPIO1PIN_P70_PUT(v)             ( bFM_GPIO_PDOR7_P0=(v) )

#define GPIO1PIN_P70_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P70_INITIN(v) \
                                                           : GPIO1PIN_P70_INITOUT(v) )

#define GPIO1PIN_P70_INITIN(v)          do{ \
                                            bFM_GPIO_PCR7_P0=(v).bPullup; \
                                            bFM_GPIO_DDR7_P0=0u; \
                                            bFM_GPIO_PFR7_P0=0u; }while(0u)

#define GPIO1PIN_P70_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR7_P0=(v).bInitVal; \
                                            bFM_GPIO_DDR7_P0=1u; \
                                            bFM_GPIO_PFR7_P0=0u; }while(0u)

/*---- GPIO bit NP70 ----*/
#define GPIO1PIN_NP70_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR7_P0)) )

#define GPIO1PIN_NP70_PUT(v)            ( bFM_GPIO_PDOR7_P0=(uint32_t)(!(v)) )

#define GPIO1PIN_NP70_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP70_INITIN(v) \
                                                           : GPIO1PIN_NP70_INITOUT(v) )

#define GPIO1PIN_NP70_INITIN(v)         do{ \
                                            bFM_GPIO_PCR7_P0=(v).bPullup; \
                                            bFM_GPIO_DDR7_P0=0u; \
                                            bFM_GPIO_PFR7_P0=0u; }while(0u)

#define GPIO1PIN_NP70_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR7_P0=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR7_P0=1u; \
                                            bFM_GPIO_PFR7_P0=0u; }while(0u)

/*---- GPIO bit P71 ----*/
#define GPIO1PIN_P71_GET                ( bFM_GPIO_PDIR7_P1 )

#define GPIO1PIN_P71_PUT(v)             ( bFM_GPIO_PDOR7_P1=(v) )

#define GPIO1PIN_P71_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P71_INITIN(v) \
                                                           : GPIO1PIN_P71_INITOUT(v) )

#define GPIO1PIN_P71_INITIN(v)          do{ \
                                            bFM_GPIO_PCR7_P1=(v).bPullup; \
                                            bFM_GPIO_DDR7_P1=0u; \
                                            bFM_GPIO_PFR7_P1=0u; }while(0u)

#define GPIO1PIN_P71_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR7_P1=(v).bInitVal; \
                                            bFM_GPIO_DDR7_P1=1u; \
                                            bFM_GPIO_PFR7_P1=0u; }while(0u)

/*---- GPIO bit NP71 ----*/
#define GPIO1PIN_NP71_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR7_P1)) )

#define GPIO1PIN_NP71_PUT(v)            ( bFM_GPIO_PDOR7_P1=(uint32_t)(!(v)) )

#define GPIO1PIN_NP71_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP71_INITIN(v) \
                                                           : GPIO1PIN_NP71_INITOUT(v) )

#define GPIO1PIN_NP71_INITIN(v)         do{ \
                                            bFM_GPIO_PCR7_P1=(v).bPullup; \
                                            bFM_GPIO_DDR7_P1=0u; \
                                            bFM_GPIO_PFR7_P1=0u; }while(0u)

#define GPIO1PIN_NP71_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR7_P1=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR7_P1=1u; \
                                            bFM_GPIO_PFR7_P1=0u; }while(0u)

/*---- GPIO bit P72 ----*/
#define GPIO1PIN_P72_GET                ( bFM_GPIO_PDIR7_P2 )

#define GPIO1PIN_P72_PUT(v)             ( bFM_GPIO_PDOR7_P2=(v) )

#define GPIO1PIN_P72_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P72_INITIN(v) \
                                                           : GPIO1PIN_P72_INITOUT(v) )

#define GPIO1PIN_P72_INITIN(v)          do{ \
                                            bFM_GPIO_PCR7_P2=(v).bPullup; \
                                            bFM_GPIO_DDR7_P2=0u; \
                                            bFM_GPIO_PFR7_P2=0u; }while(0u)

#define GPIO1PIN_P72_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR7_P2=(v).bInitVal; \
                                            bFM_GPIO_DDR7_P2=1u; \
                                            bFM_GPIO_PFR7_P2=0u; }while(0u)

/*---- GPIO bit NP72 ----*/
#define GPIO1PIN_NP72_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR7_P2)) )

#define GPIO1PIN_NP72_PUT(v)            ( bFM_GPIO_PDOR7_P2=(uint32_t)(!(v)) )

#define GPIO1PIN_NP72_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP72_INITIN(v) \
                                                           : GPIO1PIN_NP72_INITOUT(v) )

#define GPIO1PIN_NP72_INITIN(v)         do{ \
                                            bFM_GPIO_PCR7_P2=(v).bPullup; \
                                            bFM_GPIO_DDR7_P2=0u; \
                                            bFM_GPIO_PFR7_P2=0u; }while(0u)

#define GPIO1PIN_NP72_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR7_P2=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR7_P2=1u; \
                                            bFM_GPIO_PFR7_P2=0u; }while(0u)

/*---- GPIO bit P73 ----*/
#define GPIO1PIN_P73_GET                ( bFM_GPIO_PDIR7_P3 )

#define GPIO1PIN_P73_PUT(v)             ( bFM_GPIO_PDOR7_P3=(v) )

#define GPIO1PIN_P73_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P73_INITIN(v) \
                                                           : GPIO1PIN_P73_INITOUT(v) )

#define GPIO1PIN_P73_INITIN(v)          do{ \
                                            bFM_GPIO_PCR7_P3=(v).bPullup; \
                                            bFM_GPIO_DDR7_P3=0u; \
                                            bFM_GPIO_PFR7_P3=0u; }while(0u)

#define GPIO1PIN_P73_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR7_P3=(v).bInitVal; \
                                            bFM_GPIO_DDR7_P3=1u; \
                                            bFM_GPIO_PFR7_P3=0u; }while(0u)

/*---- GPIO bit NP73 ----*/
#define GPIO1PIN_NP73_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR7_P3)) )

#define GPIO1PIN_NP73_PUT(v)            ( bFM_GPIO_PDOR7_P3=(uint32_t)(!(v)) )

#define GPIO1PIN_NP73_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP73_INITIN(v) \
                                                           : GPIO1PIN_NP73_INITOUT(v) )

#define GPIO1PIN_NP73_INITIN(v)         do{ \
                                            bFM_GPIO_PCR7_P3=(v).bPullup; \
                                            bFM_GPIO_DDR7_P3=0u; \
                                            bFM_GPIO_PFR7_P3=0u; }while(0u)

#define GPIO1PIN_NP73_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR7_P3=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR7_P3=1u; \
                                            bFM_GPIO_PFR7_P3=0u; }while(0u)

/*---- GPIO bit P74 ----*/
#define GPIO1PIN_P74_GET                ( bFM_GPIO_PDIR7_P4 )

#define GPIO1PIN_P74_PUT(v)             ( bFM_GPIO_PDOR7_P4=(v) )

#define GPIO1PIN_P74_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P74_INITIN(v) \
                                                           : GPIO1PIN_P74_INITOUT(v) )

#define GPIO1PIN_P74_INITIN(v)          do{ \
                                            bFM_GPIO_PCR7_P4=(v).bPullup; \
                                            bFM_GPIO_DDR7_P4=0u; \
                                            bFM_GPIO_PFR7_P4=0u; }while(0u)

#define GPIO1PIN_P74_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR7_P4=(v).bInitVal; \
                                            bFM_GPIO_DDR7_P4=1u; \
                                            bFM_GPIO_PFR7_P4=0u; }while(0u)

/*---- GPIO bit NP74 ----*/
#define GPIO1PIN_NP74_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR7_P4)) )

#define GPIO1PIN_NP74_PUT(v)            ( bFM_GPIO_PDOR7_P4=(uint32_t)(!(v)) )

#define GPIO1PIN_NP74_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP74_INITIN(v) \
                                                           : GPIO1PIN_NP74_INITOUT(v) )

#define GPIO1PIN_NP74_INITIN(v)         do{ \
                                            bFM_GPIO_PCR7_P4=(v).bPullup; \
                                            bFM_GPIO_DDR7_P4=0u; \
                                            bFM_GPIO_PFR7_P4=0u; }while(0u)

#define GPIO1PIN_NP74_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR7_P4=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR7_P4=1u; \
                                            bFM_GPIO_PFR7_P4=0u; }while(0u)

/*---- GPIO bit P75 ----*/
#define GPIO1PIN_P75_GET                ( bFM_GPIO_PDIR7_P5 )

#define GPIO1PIN_P75_PUT(v)             ( bFM_GPIO_PDOR7_P5=(v) )

#define GPIO1PIN_P75_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P75_INITIN(v) \
                                                           : GPIO1PIN_P75_INITOUT(v) )

#define GPIO1PIN_P75_INITIN(v)          do{ \
                                            bFM_GPIO_PCR7_P5=(v).bPullup; \
                                            bFM_GPIO_DDR7_P5=0u; \
                                            bFM_GPIO_PFR7_P5=0u; }while(0u)

#define GPIO1PIN_P75_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR7_P5=(v).bInitVal; \
                                            bFM_GPIO_DDR7_P5=1u; \
                                            bFM_GPIO_PFR7_P5=0u; }while(0u)

/*---- GPIO bit NP75 ----*/
#define GPIO1PIN_NP75_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR7_P5)) )

#define GPIO1PIN_NP75_PUT(v)            ( bFM_GPIO_PDOR7_P5=(uint32_t)(!(v)) )

#define GPIO1PIN_NP75_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP75_INITIN(v) \
                                                           : GPIO1PIN_NP75_INITOUT(v) )

#define GPIO1PIN_NP75_INITIN(v)         do{ \
                                            bFM_GPIO_PCR7_P5=(v).bPullup; \
                                            bFM_GPIO_DDR7_P5=0u; \
                                            bFM_GPIO_PFR7_P5=0u; }while(0u)

#define GPIO1PIN_NP75_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR7_P5=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR7_P5=1u; \
                                            bFM_GPIO_PFR7_P5=0u; }while(0u)

/*---- GPIO bit P76 ----*/
#define GPIO1PIN_P76_GET                ( bFM_GPIO_PDIR7_P6 )

#define GPIO1PIN_P76_PUT(v)             ( bFM_GPIO_PDOR7_P6=(v) )

#define GPIO1PIN_P76_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P76_INITIN(v) \
                                                           : GPIO1PIN_P76_INITOUT(v) )

#define GPIO1PIN_P76_INITIN(v)          do{ \
                                            bFM_GPIO_PCR7_P6=(v).bPullup; \
                                            bFM_GPIO_DDR7_P6=0u; \
                                            bFM_GPIO_PFR7_P6=0u; }while(0u)

#define GPIO1PIN_P76_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR7_P6=(v).bInitVal; \
                                            bFM_GPIO_DDR7_P6=1u; \
                                            bFM_GPIO_PFR7_P6=0u; }while(0u)

/*---- GPIO bit NP76 ----*/
#define GPIO1PIN_NP76_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR7_P6)) )

#define GPIO1PIN_NP76_PUT(v)            ( bFM_GPIO_PDOR7_P6=(uint32_t)(!(v)) )

#define GPIO1PIN_NP76_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP76_INITIN(v) \
                                                           : GPIO1PIN_NP76_INITOUT(v) )

#define GPIO1PIN_NP76_INITIN(v)         do{ \
                                            bFM_GPIO_PCR7_P6=(v).bPullup; \
                                            bFM_GPIO_DDR7_P6=0u; \
                                            bFM_GPIO_PFR7_P6=0u; }while(0u)

#define GPIO1PIN_NP76_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR7_P6=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR7_P6=1u; \
                                            bFM_GPIO_PFR7_P6=0u; }while(0u)

/*---- GPIO bit P77 ----*/
#define GPIO1PIN_P77_GET                ( bFM_GPIO_PDIR7_P7 )

#define GPIO1PIN_P77_PUT(v)             ( bFM_GPIO_PDOR7_P7=(v) )

#define GPIO1PIN_P77_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P77_INITIN(v) \
                                                           : GPIO1PIN_P77_INITOUT(v) )

#define GPIO1PIN_P77_INITIN(v)          do{ \
                                            bFM_GPIO_PCR7_P7=(v).bPullup; \
                                            bFM_GPIO_DDR7_P7=0u; \
                                            bFM_GPIO_PFR7_P7=0u; }while(0u)

#define GPIO1PIN_P77_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR7_P7=(v).bInitVal; \
                                            bFM_GPIO_DDR7_P7=1u; \
                                            bFM_GPIO_PFR7_P7=0u; }while(0u)

/*---- GPIO bit NP77 ----*/
#define GPIO1PIN_NP77_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR7_P7)) )

#define GPIO1PIN_NP77_PUT(v)            ( bFM_GPIO_PDOR7_P7=(uint32_t)(!(v)) )

#define GPIO1PIN_NP77_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP77_INITIN(v) \
                                                           : GPIO1PIN_NP77_INITOUT(v) )

#define GPIO1PIN_NP77_INITIN(v)         do{ \
                                            bFM_GPIO_PCR7_P7=(v).bPullup; \
                                            bFM_GPIO_DDR7_P7=0u; \
                                            bFM_GPIO_PFR7_P7=0u; }while(0u)

#define GPIO1PIN_NP77_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR7_P7=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR7_P7=1u; \
                                            bFM_GPIO_PFR7_P7=0u; }while(0u)

/*---- GPIO bit P78 ----*/
#define GPIO1PIN_P78_GET                ( bFM_GPIO_PDIR7_P8 )

#define GPIO1PIN_P78_PUT(v)             ( bFM_GPIO_PDOR7_P8=(v) )

#define GPIO1PIN_P78_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P78_INITIN(v) \
                                                           : GPIO1PIN_P78_INITOUT(v) )

#define GPIO1PIN_P78_INITIN(v)          do{ \
                                            bFM_GPIO_PCR7_P8=(v).bPullup; \
                                            bFM_GPIO_DDR7_P8=0u; \
                                            bFM_GPIO_PFR7_P8=0u; }while(0u)

#define GPIO1PIN_P78_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR7_P8=(v).bInitVal; \
                                            bFM_GPIO_DDR7_P8=1u; \
                                            bFM_GPIO_PFR7_P8=0u; }while(0u)

/*---- GPIO bit NP78 ----*/
#define GPIO1PIN_NP78_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR7_P8)) )

#define GPIO1PIN_NP78_PUT(v)            ( bFM_GPIO_PDOR7_P8=(uint32_t)(!(v)) )

#define GPIO1PIN_NP78_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP78_INITIN(v) \
                                                           : GPIO1PIN_NP78_INITOUT(v) )

#define GPIO1PIN_NP78_INITIN(v)         do{ \
                                            bFM_GPIO_PCR7_P8=(v).bPullup; \
                                            bFM_GPIO_DDR7_P8=0u; \
                                            bFM_GPIO_PFR7_P8=0u; }while(0u)

#define GPIO1PIN_NP78_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR7_P8=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR7_P8=1u; \
                                            bFM_GPIO_PFR7_P8=0u; }while(0u)

/*---- GPIO bit P79 ----*/
#define GPIO1PIN_P79_GET                ( bFM_GPIO_PDIR7_P9 )

#define GPIO1PIN_P79_PUT(v)             ( bFM_GPIO_PDOR7_P9=(v) )

#define GPIO1PIN_P79_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P79_INITIN(v) \
                                                           : GPIO1PIN_P79_INITOUT(v) )

#define GPIO1PIN_P79_INITIN(v)          do{ \
                                            bFM_GPIO_PCR7_P9=(v).bPullup; \
                                            bFM_GPIO_DDR7_P9=0u; \
                                            bFM_GPIO_PFR7_P9=0u; }while(0u)

#define GPIO1PIN_P79_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR7_P9=(v).bInitVal; \
                                            bFM_GPIO_DDR7_P9=1u; \
                                            bFM_GPIO_PFR7_P9=0u; }while(0u)

/*---- GPIO bit NP79 ----*/
#define GPIO1PIN_NP79_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR7_P9)) )

#define GPIO1PIN_NP79_PUT(v)            ( bFM_GPIO_PDOR7_P9=(uint32_t)(!(v)) )

#define GPIO1PIN_NP79_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP79_INITIN(v) \
                                                           : GPIO1PIN_NP79_INITOUT(v) )

#define GPIO1PIN_NP79_INITIN(v)         do{ \
                                            bFM_GPIO_PCR7_P9=(v).bPullup; \
                                            bFM_GPIO_DDR7_P9=0u; \
                                            bFM_GPIO_PFR7_P9=0u; }while(0u)

#define GPIO1PIN_NP79_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR7_P9=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR7_P9=1u; \
                                            bFM_GPIO_PFR7_P9=0u; }while(0u)

/*---- GPIO bit P7A ----*/
#define GPIO1PIN_P7A_GET                ( bFM_GPIO_PDIR7_PA )

#define GPIO1PIN_P7A_PUT(v)             ( bFM_GPIO_PDOR7_PA=(v) )

#define GPIO1PIN_P7A_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P7A_INITIN(v) \
                                                           : GPIO1PIN_P7A_INITOUT(v) )

#define GPIO1PIN_P7A_INITIN(v)          do{ \
                                            bFM_GPIO_PCR7_PA=(v).bPullup; \
                                            bFM_GPIO_DDR7_PA=0u; \
                                            bFM_GPIO_PFR7_PA=0u; }while(0u)

#define GPIO1PIN_P7A_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR7_PA=(v).bInitVal; \
                                            bFM_GPIO_DDR7_PA=1u; \
                                            bFM_GPIO_PFR7_PA=0u; }while(0u)

/*---- GPIO bit NP7A ----*/
#define GPIO1PIN_NP7A_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR7_PA)) )

#define GPIO1PIN_NP7A_PUT(v)            ( bFM_GPIO_PDOR7_PA=(uint32_t)(!(v)) )

#define GPIO1PIN_NP7A_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP7A_INITIN(v) \
                                                           : GPIO1PIN_NP7A_INITOUT(v) )

#define GPIO1PIN_NP7A_INITIN(v)         do{ \
                                            bFM_GPIO_PCR7_PA=(v).bPullup; \
                                            bFM_GPIO_DDR7_PA=0u; \
                                            bFM_GPIO_PFR7_PA=0u; }while(0u)

#define GPIO1PIN_NP7A_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR7_PA=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR7_PA=1u; \
                                            bFM_GPIO_PFR7_PA=0u; }while(0u)

/*---- GPIO bit P7B ----*/
#define GPIO1PIN_P7B_GET                ( bFM_GPIO_PDIR7_PB )

#define GPIO1PIN_P7B_PUT(v)             ( bFM_GPIO_PDOR7_PB=(v) )

#define GPIO1PIN_P7B_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P7B_INITIN(v) \
                                                           : GPIO1PIN_P7B_INITOUT(v) )

#define GPIO1PIN_P7B_INITIN(v)          do{ \
                                            bFM_DAC1_DACR_DAE=0u; \
                                            bFM_GPIO_PCR7_PB=(v).bPullup; \
                                            bFM_GPIO_DDR7_PB=0u; \
                                            bFM_GPIO_PFR7_PB=0u; }while(0u)

#define GPIO1PIN_P7B_INITOUT(v)         do{ \
                                            bFM_DAC1_DACR_DAE=0u; \
                                            bFM_GPIO_PDOR7_PB=(v).bInitVal; \
                                            bFM_GPIO_DDR7_PB=1u; \
                                            bFM_GPIO_PFR7_PB=0u; }while(0u)

/*---- GPIO bit NP7B ----*/
#define GPIO1PIN_NP7B_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR7_PB)) )

#define GPIO1PIN_NP7B_PUT(v)            ( bFM_GPIO_PDOR7_PB=(uint32_t)(!(v)) )

#define GPIO1PIN_NP7B_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP7B_INITIN(v) \
                                                           : GPIO1PIN_NP7B_INITOUT(v) )

#define GPIO1PIN_NP7B_INITIN(v)         do{ \
                                            bFM_DAC1_DACR_DAE=0u; \
                                            bFM_GPIO_PCR7_PB=(v).bPullup; \
                                            bFM_GPIO_DDR7_PB=0u; \
                                            bFM_GPIO_PFR7_PB=0u; }while(0u)

#define GPIO1PIN_NP7B_INITOUT(v)        do{ \
                                            bFM_DAC1_DACR_DAE=0u; \
                                            bFM_GPIO_PDOR7_PB=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR7_PB=1u; \
                                            bFM_GPIO_PFR7_PB=0u; }while(0u)

/*---- GPIO bit P7C ----*/
#define GPIO1PIN_P7C_GET                ( bFM_GPIO_PDIR7_PC )

#define GPIO1PIN_P7C_PUT(v)             ( bFM_GPIO_PDOR7_PC=(v) )

#define GPIO1PIN_P7C_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P7C_INITIN(v) \
                                                           : GPIO1PIN_P7C_INITOUT(v) )

#define GPIO1PIN_P7C_INITIN(v)          do{ \
                                            bFM_DAC0_DACR_DAE=0u; \
                                            bFM_GPIO_PCR7_PC=(v).bPullup; \
                                            bFM_GPIO_DDR7_PC=0u; \
                                            bFM_GPIO_PFR7_PC=0u; }while(0u)

#define GPIO1PIN_P7C_INITOUT(v)         do{ \
                                            bFM_DAC0_DACR_DAE=0u; \
                                            bFM_GPIO_PDOR7_PC=(v).bInitVal; \
                                            bFM_GPIO_DDR7_PC=1u; \
                                            bFM_GPIO_PFR7_PC=0u; }while(0u)

/*---- GPIO bit NP7C ----*/
#define GPIO1PIN_NP7C_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR7_PC)) )

#define GPIO1PIN_NP7C_PUT(v)            ( bFM_GPIO_PDOR7_PC=(uint32_t)(!(v)) )

#define GPIO1PIN_NP7C_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP7C_INITIN(v) \
                                                           : GPIO1PIN_NP7C_INITOUT(v) )

#define GPIO1PIN_NP7C_INITIN(v)         do{ \
                                            bFM_DAC0_DACR_DAE=0u; \
                                            bFM_GPIO_PCR7_PC=(v).bPullup; \
                                            bFM_GPIO_DDR7_PC=0u; \
                                            bFM_GPIO_PFR7_PC=0u; }while(0u)

#define GPIO1PIN_NP7C_INITOUT(v)        do{ \
                                            bFM_DAC0_DACR_DAE=0u; \
                                            bFM_GPIO_PDOR7_PC=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR7_PC=1u; \
                                            bFM_GPIO_PFR7_PC=0u; }while(0u)

/*---- GPIO bit P7D ----*/
#define GPIO1PIN_P7D_GET                ( bFM_GPIO_PDIR7_PD )

#define GPIO1PIN_P7D_PUT(v)             ( bFM_GPIO_PDOR7_PD=(v) )

#define GPIO1PIN_P7D_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P7D_INITIN(v) \
                                                           : GPIO1PIN_P7D_INITOUT(v) )

#define GPIO1PIN_P7D_INITIN(v)          do{ \
                                            bFM_GPIO_PCR7_PD=(v).bPullup; \
                                            bFM_GPIO_DDR7_PD=0u; \
                                            bFM_GPIO_PFR7_PD=0u; }while(0u)

#define GPIO1PIN_P7D_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR7_PD=(v).bInitVal; \
                                            bFM_GPIO_DDR7_PD=1u; \
                                            bFM_GPIO_PFR7_PD=0u; }while(0u)

/*---- GPIO bit NP7D ----*/
#define GPIO1PIN_NP7D_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR7_PD)) )

#define GPIO1PIN_NP7D_PUT(v)            ( bFM_GPIO_PDOR7_PD=(uint32_t)(!(v)) )

#define GPIO1PIN_NP7D_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP7D_INITIN(v) \
                                                           : GPIO1PIN_NP7D_INITOUT(v) )

#define GPIO1PIN_NP7D_INITIN(v)         do{ \
                                            bFM_GPIO_PCR7_PD=(v).bPullup; \
                                            bFM_GPIO_DDR7_PD=0u; \
                                            bFM_GPIO_PFR7_PD=0u; }while(0u)

#define GPIO1PIN_NP7D_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR7_PD=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR7_PD=1u; \
                                            bFM_GPIO_PFR7_PD=0u; }while(0u)

/*---- GPIO bit P7E ----*/
#define GPIO1PIN_P7E_GET                ( bFM_GPIO_PDIR7_PE )

#define GPIO1PIN_P7E_PUT(v)             ( bFM_GPIO_PDOR7_PE=(v) )

#define GPIO1PIN_P7E_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P7E_INITIN(v) \
                                                           : GPIO1PIN_P7E_INITOUT(v) )

#define GPIO1PIN_P7E_INITIN(v)          do{ \
                                            bFM_GPIO_PCR7_PE=(v).bPullup; \
                                            bFM_GPIO_DDR7_PE=0u; \
                                            bFM_GPIO_PFR7_PE=0u; }while(0u)

#define GPIO1PIN_P7E_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR7_PE=(v).bInitVal; \
                                            bFM_GPIO_DDR7_PE=1u; \
                                            bFM_GPIO_PFR7_PE=0u; }while(0u)

/*---- GPIO bit NP7E ----*/
#define GPIO1PIN_NP7E_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR7_PE)) )

#define GPIO1PIN_NP7E_PUT(v)            ( bFM_GPIO_PDOR7_PE=(uint32_t)(!(v)) )

#define GPIO1PIN_NP7E_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP7E_INITIN(v) \
                                                           : GPIO1PIN_NP7E_INITOUT(v) )

#define GPIO1PIN_NP7E_INITIN(v)         do{ \
                                            bFM_GPIO_PCR7_PE=(v).bPullup; \
                                            bFM_GPIO_DDR7_PE=0u; \
                                            bFM_GPIO_PFR7_PE=0u; }while(0u)

#define GPIO1PIN_NP7E_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR7_PE=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR7_PE=1u; \
                                            bFM_GPIO_PFR7_PE=0u; }while(0u)

/*---- GPIO bit P80 ----*/
#define GPIO1PIN_P80_GET                ( bFM_GPIO_PDIR8_P0 )

#define GPIO1PIN_P80_PUT(v)             ( bFM_GPIO_PDOR8_P0=(v) )

#define GPIO1PIN_P80_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P80_INITIN(v) \
                                                           : GPIO1PIN_P80_INITOUT(v) )

#define GPIO1PIN_P80_INITIN(v)          do{ \
                                            bFM_GPIO_SPSR_USB0C=0u; \
                                            bFM_GPIO_DDR8_P0=0u; \
                                            bFM_GPIO_PFR8_P0=0u; }while(0u)

#define GPIO1PIN_P80_INITOUT(v)         do{ \
                                            bFM_GPIO_SPSR_USB0C=0u; \
                                            bFM_GPIO_PDOR8_P0=(v).bInitVal; \
                                            bFM_GPIO_DDR8_P0=1u; \
                                            bFM_GPIO_PFR8_P0=0u; }while(0u)

/*---- GPIO bit NP80 ----*/
#define GPIO1PIN_NP80_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR8_P0)) )

#define GPIO1PIN_NP80_PUT(v)            ( bFM_GPIO_PDOR8_P0=(uint32_t)(!(v)) )

#define GPIO1PIN_NP80_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP80_INITIN(v) \
                                                           : GPIO1PIN_NP80_INITOUT(v) )

#define GPIO1PIN_NP80_INITIN(v)         do{ \
                                            bFM_GPIO_SPSR_USB0C=0u; \
                                            bFM_GPIO_DDR8_P0=0u; \
                                            bFM_GPIO_PFR8_P0=0u; }while(0u)

#define GPIO1PIN_NP80_INITOUT(v)        do{ \
                                            bFM_GPIO_SPSR_USB0C=0u; \
                                            bFM_GPIO_PDOR8_P0=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR8_P0=1u; \
                                            bFM_GPIO_PFR8_P0=0u; }while(0u)

/*---- GPIO bit P81 ----*/
#define GPIO1PIN_P81_GET                ( bFM_GPIO_PDIR8_P1 )

#define GPIO1PIN_P81_PUT(v)             ( bFM_GPIO_PDOR8_P1=(v) )

#define GPIO1PIN_P81_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P81_INITIN(v) \
                                                           : GPIO1PIN_P81_INITOUT(v) )

#define GPIO1PIN_P81_INITIN(v)          do{ \
                                            bFM_GPIO_SPSR_USB0C=0u; \
                                            bFM_GPIO_DDR8_P1=0u; \
                                            bFM_GPIO_PFR8_P1=0u; }while(0u)

#define GPIO1PIN_P81_INITOUT(v)         do{ \
                                            bFM_GPIO_SPSR_USB0C=0u; \
                                            bFM_GPIO_PDOR8_P1=(v).bInitVal; \
                                            bFM_GPIO_DDR8_P1=1u; \
                                            bFM_GPIO_PFR8_P1=0u; }while(0u)

/*---- GPIO bit NP81 ----*/
#define GPIO1PIN_NP81_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR8_P1)) )

#define GPIO1PIN_NP81_PUT(v)            ( bFM_GPIO_PDOR8_P1=(uint32_t)(!(v)) )

#define GPIO1PIN_NP81_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP81_INITIN(v) \
                                                           : GPIO1PIN_NP81_INITOUT(v) )

#define GPIO1PIN_NP81_INITIN(v)         do{ \
                                            bFM_GPIO_SPSR_USB0C=0u; \
                                            bFM_GPIO_DDR8_P1=0u; \
                                            bFM_GPIO_PFR8_P1=0u; }while(0u)

#define GPIO1PIN_NP81_INITOUT(v)        do{ \
                                            bFM_GPIO_SPSR_USB0C=0u; \
                                            bFM_GPIO_PDOR8_P1=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR8_P1=1u; \
                                            bFM_GPIO_PFR8_P1=0u; }while(0u)

/*---- GPIO bit P82 ----*/
#define GPIO1PIN_P82_GET                ( bFM_GPIO_PDIR8_P2 )

#define GPIO1PIN_P82_PUT(v)             ( bFM_GPIO_PDOR8_P2=(v) )

#define GPIO1PIN_P82_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P82_INITIN(v) \
                                                           : GPIO1PIN_P82_INITOUT(v) )

#define GPIO1PIN_P82_INITIN(v)          do{ \
                                            bFM_GPIO_SPSR_USB1C=0u; \
                                            bFM_GPIO_DDR8_P2=0u; \
                                            bFM_GPIO_PFR8_P2=0u; }while(0u)

#define GPIO1PIN_P82_INITOUT(v)         do{ \
                                            bFM_GPIO_SPSR_USB1C=0u; \
                                            bFM_GPIO_PDOR8_P2=(v).bInitVal; \
                                            bFM_GPIO_DDR8_P2=1u; \
                                            bFM_GPIO_PFR8_P2=0u; }while(0u)

/*---- GPIO bit NP82 ----*/
#define GPIO1PIN_NP82_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR8_P2)) )

#define GPIO1PIN_NP82_PUT(v)            ( bFM_GPIO_PDOR8_P2=(uint32_t)(!(v)) )

#define GPIO1PIN_NP82_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP82_INITIN(v) \
                                                           : GPIO1PIN_NP82_INITOUT(v) )

#define GPIO1PIN_NP82_INITIN(v)         do{ \
                                            bFM_GPIO_SPSR_USB1C=0u; \
                                            bFM_GPIO_DDR8_P2=0u; \
                                            bFM_GPIO_PFR8_P2=0u; }while(0u)

#define GPIO1PIN_NP82_INITOUT(v)        do{ \
                                            bFM_GPIO_SPSR_USB1C=0u; \
                                            bFM_GPIO_PDOR8_P2=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR8_P2=1u; \
                                            bFM_GPIO_PFR8_P2=0u; }while(0u)

/*---- GPIO bit P83 ----*/
#define GPIO1PIN_P83_GET                ( bFM_GPIO_PDIR8_P3 )

#define GPIO1PIN_P83_PUT(v)             ( bFM_GPIO_PDOR8_P3=(v) )

#define GPIO1PIN_P83_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P83_INITIN(v) \
                                                           : GPIO1PIN_P83_INITOUT(v) )

#define GPIO1PIN_P83_INITIN(v)          do{ \
                                            bFM_GPIO_SPSR_USB1C=0u; \
                                            bFM_GPIO_DDR8_P3=0u; \
                                            bFM_GPIO_PFR8_P3=0u; }while(0u)

#define GPIO1PIN_P83_INITOUT(v)         do{ \
                                            bFM_GPIO_SPSR_USB1C=0u; \
                                            bFM_GPIO_PDOR8_P3=(v).bInitVal; \
                                            bFM_GPIO_DDR8_P3=1u; \
                                            bFM_GPIO_PFR8_P3=0u; }while(0u)

/*---- GPIO bit NP83 ----*/
#define GPIO1PIN_NP83_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR8_P3)) )

#define GPIO1PIN_NP83_PUT(v)            ( bFM_GPIO_PDOR8_P3=(uint32_t)(!(v)) )

#define GPIO1PIN_NP83_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP83_INITIN(v) \
                                                           : GPIO1PIN_NP83_INITOUT(v) )

#define GPIO1PIN_NP83_INITIN(v)         do{ \
                                            bFM_GPIO_SPSR_USB1C=0u; \
                                            bFM_GPIO_DDR8_P3=0u; \
                                            bFM_GPIO_PFR8_P3=0u; }while(0u)

#define GPIO1PIN_NP83_INITOUT(v)        do{ \
                                            bFM_GPIO_SPSR_USB1C=0u; \
                                            bFM_GPIO_PDOR8_P3=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR8_P3=1u; \
                                            bFM_GPIO_PFR8_P3=0u; }while(0u)

/*---- GPIO bit PA0 ----*/
#define GPIO1PIN_PA0_GET                ( bFM_GPIO_PDIRA_P0 )

#define GPIO1PIN_PA0_PUT(v)             ( bFM_GPIO_PDORA_P0=(v) )

#define GPIO1PIN_PA0_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PA0_INITIN(v) \
                                                           : GPIO1PIN_PA0_INITOUT(v) )

#define GPIO1PIN_PA0_INITIN(v)          do{ \
                                            bFM_GPIO_PCRA_P0=(v).bPullup; \
                                            bFM_GPIO_DDRA_P0=0u; \
                                            bFM_GPIO_PFRA_P0=0u; }while(0u)

#define GPIO1PIN_PA0_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORA_P0=(v).bInitVal; \
                                            bFM_GPIO_DDRA_P0=1u; \
                                            bFM_GPIO_PFRA_P0=0u; }while(0u)

/*---- GPIO bit NPA0 ----*/
#define GPIO1PIN_NPA0_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRA_P0)) )

#define GPIO1PIN_NPA0_PUT(v)            ( bFM_GPIO_PDORA_P0=(uint32_t)(!(v)) )

#define GPIO1PIN_NPA0_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPA0_INITIN(v) \
                                                           : GPIO1PIN_NPA0_INITOUT(v) )

#define GPIO1PIN_NPA0_INITIN(v)         do{ \
                                            bFM_GPIO_PCRA_P0=(v).bPullup; \
                                            bFM_GPIO_DDRA_P0=0u; \
                                            bFM_GPIO_PFRA_P0=0u; }while(0u)

#define GPIO1PIN_NPA0_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORA_P0=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRA_P0=1u; \
                                            bFM_GPIO_PFRA_P0=0u; }while(0u)

/*---- GPIO bit PA1 ----*/
#define GPIO1PIN_PA1_GET                ( bFM_GPIO_PDIRA_P1 )

#define GPIO1PIN_PA1_PUT(v)             ( bFM_GPIO_PDORA_P1=(v) )

#define GPIO1PIN_PA1_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PA1_INITIN(v) \
                                                           : GPIO1PIN_PA1_INITOUT(v) )

#define GPIO1PIN_PA1_INITIN(v)          do{ \
                                            bFM_GPIO_PCRA_P1=(v).bPullup; \
                                            bFM_GPIO_DDRA_P1=0u; \
                                            bFM_GPIO_PFRA_P1=0u; }while(0u)

#define GPIO1PIN_PA1_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORA_P1=(v).bInitVal; \
                                            bFM_GPIO_DDRA_P1=1u; \
                                            bFM_GPIO_PFRA_P1=0u; }while(0u)

/*---- GPIO bit NPA1 ----*/
#define GPIO1PIN_NPA1_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRA_P1)) )

#define GPIO1PIN_NPA1_PUT(v)            ( bFM_GPIO_PDORA_P1=(uint32_t)(!(v)) )

#define GPIO1PIN_NPA1_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPA1_INITIN(v) \
                                                           : GPIO1PIN_NPA1_INITOUT(v) )

#define GPIO1PIN_NPA1_INITIN(v)         do{ \
                                            bFM_GPIO_PCRA_P1=(v).bPullup; \
                                            bFM_GPIO_DDRA_P1=0u; \
                                            bFM_GPIO_PFRA_P1=0u; }while(0u)

#define GPIO1PIN_NPA1_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORA_P1=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRA_P1=1u; \
                                            bFM_GPIO_PFRA_P1=0u; }while(0u)

/*---- GPIO bit PA2 ----*/
#define GPIO1PIN_PA2_GET                ( bFM_GPIO_PDIRA_P2 )

#define GPIO1PIN_PA2_PUT(v)             ( bFM_GPIO_PDORA_P2=(v) )

#define GPIO1PIN_PA2_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PA2_INITIN(v) \
                                                           : GPIO1PIN_PA2_INITOUT(v) )

#define GPIO1PIN_PA2_INITIN(v)          do{ \
                                            bFM_GPIO_PCRA_P2=(v).bPullup; \
                                            bFM_GPIO_DDRA_P2=0u; \
                                            bFM_GPIO_PFRA_P2=0u; }while(0u)

#define GPIO1PIN_PA2_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORA_P2=(v).bInitVal; \
                                            bFM_GPIO_DDRA_P2=1u; \
                                            bFM_GPIO_PFRA_P2=0u; }while(0u)

/*---- GPIO bit NPA2 ----*/
#define GPIO1PIN_NPA2_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRA_P2)) )

#define GPIO1PIN_NPA2_PUT(v)            ( bFM_GPIO_PDORA_P2=(uint32_t)(!(v)) )

#define GPIO1PIN_NPA2_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPA2_INITIN(v) \
                                                           : GPIO1PIN_NPA2_INITOUT(v) )

#define GPIO1PIN_NPA2_INITIN(v)         do{ \
                                            bFM_GPIO_PCRA_P2=(v).bPullup; \
                                            bFM_GPIO_DDRA_P2=0u; \
                                            bFM_GPIO_PFRA_P2=0u; }while(0u)

#define GPIO1PIN_NPA2_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORA_P2=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRA_P2=1u; \
                                            bFM_GPIO_PFRA_P2=0u; }while(0u)

/*---- GPIO bit PA3 ----*/
#define GPIO1PIN_PA3_GET                ( bFM_GPIO_PDIRA_P3 )

#define GPIO1PIN_PA3_PUT(v)             ( bFM_GPIO_PDORA_P3=(v) )

#define GPIO1PIN_PA3_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PA3_INITIN(v) \
                                                           : GPIO1PIN_PA3_INITOUT(v) )

#define GPIO1PIN_PA3_INITIN(v)          do{ \
                                            bFM_GPIO_PCRA_P3=(v).bPullup; \
                                            bFM_GPIO_DDRA_P3=0u; \
                                            bFM_GPIO_PFRA_P3=0u; }while(0u)

#define GPIO1PIN_PA3_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORA_P3=(v).bInitVal; \
                                            bFM_GPIO_DDRA_P3=1u; \
                                            bFM_GPIO_PFRA_P3=0u; }while(0u)

/*---- GPIO bit NPA3 ----*/
#define GPIO1PIN_NPA3_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRA_P3)) )

#define GPIO1PIN_NPA3_PUT(v)            ( bFM_GPIO_PDORA_P3=(uint32_t)(!(v)) )

#define GPIO1PIN_NPA3_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPA3_INITIN(v) \
                                                           : GPIO1PIN_NPA3_INITOUT(v) )

#define GPIO1PIN_NPA3_INITIN(v)         do{ \
                                            bFM_GPIO_PCRA_P3=(v).bPullup; \
                                            bFM_GPIO_DDRA_P3=0u; \
                                            bFM_GPIO_PFRA_P3=0u; }while(0u)

#define GPIO1PIN_NPA3_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORA_P3=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRA_P3=1u; \
                                            bFM_GPIO_PFRA_P3=0u; }while(0u)

/*---- GPIO bit PA4 ----*/
#define GPIO1PIN_PA4_GET                ( bFM_GPIO_PDIRA_P4 )

#define GPIO1PIN_PA4_PUT(v)             ( bFM_GPIO_PDORA_P4=(v) )

#define GPIO1PIN_PA4_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PA4_INITIN(v) \
                                                           : GPIO1PIN_PA4_INITOUT(v) )

#define GPIO1PIN_PA4_INITIN(v)          do{ \
                                            bFM_GPIO_PCRA_P4=(v).bPullup; \
                                            bFM_GPIO_DDRA_P4=0u; \
                                            bFM_GPIO_PFRA_P4=0u; }while(0u)

#define GPIO1PIN_PA4_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORA_P4=(v).bInitVal; \
                                            bFM_GPIO_DDRA_P4=1u; \
                                            bFM_GPIO_PFRA_P4=0u; }while(0u)

/*---- GPIO bit NPA4 ----*/
#define GPIO1PIN_NPA4_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRA_P4)) )

#define GPIO1PIN_NPA4_PUT(v)            ( bFM_GPIO_PDORA_P4=(uint32_t)(!(v)) )

#define GPIO1PIN_NPA4_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPA4_INITIN(v) \
                                                           : GPIO1PIN_NPA4_INITOUT(v) )

#define GPIO1PIN_NPA4_INITIN(v)         do{ \
                                            bFM_GPIO_PCRA_P4=(v).bPullup; \
                                            bFM_GPIO_DDRA_P4=0u; \
                                            bFM_GPIO_PFRA_P4=0u; }while(0u)

#define GPIO1PIN_NPA4_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORA_P4=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRA_P4=1u; \
                                            bFM_GPIO_PFRA_P4=0u; }while(0u)

/*---- GPIO bit PA5 ----*/
#define GPIO1PIN_PA5_GET                ( bFM_GPIO_PDIRA_P5 )

#define GPIO1PIN_PA5_PUT(v)             ( bFM_GPIO_PDORA_P5=(v) )

#define GPIO1PIN_PA5_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PA5_INITIN(v) \
                                                           : GPIO1PIN_PA5_INITOUT(v) )

#define GPIO1PIN_PA5_INITIN(v)          do{ \
                                            bFM_GPIO_PCRA_P5=(v).bPullup; \
                                            bFM_GPIO_DDRA_P5=0u; \
                                            bFM_GPIO_PFRA_P5=0u; }while(0u)

#define GPIO1PIN_PA5_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORA_P5=(v).bInitVal; \
                                            bFM_GPIO_DDRA_P5=1u; \
                                            bFM_GPIO_PFRA_P5=0u; }while(0u)

/*---- GPIO bit NPA5 ----*/
#define GPIO1PIN_NPA5_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRA_P5)) )

#define GPIO1PIN_NPA5_PUT(v)            ( bFM_GPIO_PDORA_P5=(uint32_t)(!(v)) )

#define GPIO1PIN_NPA5_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPA5_INITIN(v) \
                                                           : GPIO1PIN_NPA5_INITOUT(v) )

#define GPIO1PIN_NPA5_INITIN(v)         do{ \
                                            bFM_GPIO_PCRA_P5=(v).bPullup; \
                                            bFM_GPIO_DDRA_P5=0u; \
                                            bFM_GPIO_PFRA_P5=0u; }while(0u)

#define GPIO1PIN_NPA5_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORA_P5=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRA_P5=1u; \
                                            bFM_GPIO_PFRA_P5=0u; }while(0u)

/*---- GPIO bit PA6 ----*/
#define GPIO1PIN_PA6_GET                ( bFM_GPIO_PDIRA_P6 )

#define GPIO1PIN_PA6_PUT(v)             ( bFM_GPIO_PDORA_P6=(v) )

#define GPIO1PIN_PA6_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PA6_INITIN(v) \
                                                           : GPIO1PIN_PA6_INITOUT(v) )

#define GPIO1PIN_PA6_INITIN(v)          do{ \
                                            bFM_GPIO_PCRA_P6=(v).bPullup; \
                                            bFM_GPIO_DDRA_P6=0u; \
                                            bFM_GPIO_PFRA_P6=0u; }while(0u)

#define GPIO1PIN_PA6_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORA_P6=(v).bInitVal; \
                                            bFM_GPIO_DDRA_P6=1u; \
                                            bFM_GPIO_PFRA_P6=0u; }while(0u)

/*---- GPIO bit NPA6 ----*/
#define GPIO1PIN_NPA6_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRA_P6)) )

#define GPIO1PIN_NPA6_PUT(v)            ( bFM_GPIO_PDORA_P6=(uint32_t)(!(v)) )

#define GPIO1PIN_NPA6_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPA6_INITIN(v) \
                                                           : GPIO1PIN_NPA6_INITOUT(v) )

#define GPIO1PIN_NPA6_INITIN(v)         do{ \
                                            bFM_GPIO_PCRA_P6=(v).bPullup; \
                                            bFM_GPIO_DDRA_P6=0u; \
                                            bFM_GPIO_PFRA_P6=0u; }while(0u)

#define GPIO1PIN_NPA6_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORA_P6=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRA_P6=1u; \
                                            bFM_GPIO_PFRA_P6=0u; }while(0u)

/*---- GPIO bit PA7 ----*/
#define GPIO1PIN_PA7_GET                ( bFM_GPIO_PDIRA_P7 )

#define GPIO1PIN_PA7_PUT(v)             ( bFM_GPIO_PDORA_P7=(v) )

#define GPIO1PIN_PA7_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PA7_INITIN(v) \
                                                           : GPIO1PIN_PA7_INITOUT(v) )

#define GPIO1PIN_PA7_INITIN(v)          do{ \
                                            bFM_GPIO_PCRA_P7=(v).bPullup; \
                                            bFM_GPIO_DDRA_P7=0u; \
                                            bFM_GPIO_PFRA_P7=0u; }while(0u)

#define GPIO1PIN_PA7_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORA_P7=(v).bInitVal; \
                                            bFM_GPIO_DDRA_P7=1u; \
                                            bFM_GPIO_PFRA_P7=0u; }while(0u)

/*---- GPIO bit NPA7 ----*/
#define GPIO1PIN_NPA7_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRA_P7)) )

#define GPIO1PIN_NPA7_PUT(v)            ( bFM_GPIO_PDORA_P7=(uint32_t)(!(v)) )

#define GPIO1PIN_NPA7_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPA7_INITIN(v) \
                                                           : GPIO1PIN_NPA7_INITOUT(v) )

#define GPIO1PIN_NPA7_INITIN(v)         do{ \
                                            bFM_GPIO_PCRA_P7=(v).bPullup; \
                                            bFM_GPIO_DDRA_P7=0u; \
                                            bFM_GPIO_PFRA_P7=0u; }while(0u)

#define GPIO1PIN_NPA7_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORA_P7=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRA_P7=1u; \
                                            bFM_GPIO_PFRA_P7=0u; }while(0u)

/*---- GPIO bit PA8 ----*/
#define GPIO1PIN_PA8_GET                ( bFM_GPIO_PDIRA_P8 )

#define GPIO1PIN_PA8_PUT(v)             ( bFM_GPIO_PDORA_P8=(v) )

#define GPIO1PIN_PA8_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PA8_INITIN(v) \
                                                           : GPIO1PIN_PA8_INITOUT(v) )

#define GPIO1PIN_PA8_INITIN(v)          do{ \
                                            bFM_GPIO_PCRA_P8=(v).bPullup; \
                                            bFM_GPIO_DDRA_P8=0u; \
                                            bFM_GPIO_PFRA_P8=0u; }while(0u)

#define GPIO1PIN_PA8_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORA_P8=(v).bInitVal; \
                                            bFM_GPIO_DDRA_P8=1u; \
                                            bFM_GPIO_PFRA_P8=0u; }while(0u)

/*---- GPIO bit NPA8 ----*/
#define GPIO1PIN_NPA8_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRA_P8)) )

#define GPIO1PIN_NPA8_PUT(v)            ( bFM_GPIO_PDORA_P8=(uint32_t)(!(v)) )

#define GPIO1PIN_NPA8_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPA8_INITIN(v) \
                                                           : GPIO1PIN_NPA8_INITOUT(v) )

#define GPIO1PIN_NPA8_INITIN(v)         do{ \
                                            bFM_GPIO_PCRA_P8=(v).bPullup; \
                                            bFM_GPIO_DDRA_P8=0u; \
                                            bFM_GPIO_PFRA_P8=0u; }while(0u)

#define GPIO1PIN_NPA8_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORA_P8=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRA_P8=1u; \
                                            bFM_GPIO_PFRA_P8=0u; }while(0u)

/*---- GPIO bit PA9 ----*/
#define GPIO1PIN_PA9_GET                ( bFM_GPIO_PDIRA_P9 )

#define GPIO1PIN_PA9_PUT(v)             ( bFM_GPIO_PDORA_P9=(v) )

#define GPIO1PIN_PA9_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PA9_INITIN(v) \
                                                           : GPIO1PIN_PA9_INITOUT(v) )

#define GPIO1PIN_PA9_INITIN(v)          do{ \
                                            bFM_GPIO_PCRA_P9=(v).bPullup; \
                                            bFM_GPIO_DDRA_P9=0u; \
                                            bFM_GPIO_PFRA_P9=0u; }while(0u)

#define GPIO1PIN_PA9_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORA_P9=(v).bInitVal; \
                                            bFM_GPIO_DDRA_P9=1u; \
                                            bFM_GPIO_PFRA_P9=0u; }while(0u)

/*---- GPIO bit NPA9 ----*/
#define GPIO1PIN_NPA9_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRA_P9)) )

#define GPIO1PIN_NPA9_PUT(v)            ( bFM_GPIO_PDORA_P9=(uint32_t)(!(v)) )

#define GPIO1PIN_NPA9_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPA9_INITIN(v) \
                                                           : GPIO1PIN_NPA9_INITOUT(v) )

#define GPIO1PIN_NPA9_INITIN(v)         do{ \
                                            bFM_GPIO_PCRA_P9=(v).bPullup; \
                                            bFM_GPIO_DDRA_P9=0u; \
                                            bFM_GPIO_PFRA_P9=0u; }while(0u)

#define GPIO1PIN_NPA9_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORA_P9=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRA_P9=1u; \
                                            bFM_GPIO_PFRA_P9=0u; }while(0u)

/*---- GPIO bit PAA ----*/
#define GPIO1PIN_PAA_GET                ( bFM_GPIO_PDIRA_PA )

#define GPIO1PIN_PAA_PUT(v)             ( bFM_GPIO_PDORA_PA=(v) )

#define GPIO1PIN_PAA_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PAA_INITIN(v) \
                                                           : GPIO1PIN_PAA_INITOUT(v) )

#define GPIO1PIN_PAA_INITIN(v)          do{ \
                                            bFM_GPIO_PCRA_PA=(v).bPullup; \
                                            bFM_GPIO_DDRA_PA=0u; \
                                            bFM_GPIO_PFRA_PA=0u; }while(0u)

#define GPIO1PIN_PAA_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORA_PA=(v).bInitVal; \
                                            bFM_GPIO_DDRA_PA=1u; \
                                            bFM_GPIO_PFRA_PA=0u; }while(0u)

/*---- GPIO bit NPAA ----*/
#define GPIO1PIN_NPAA_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRA_PA)) )

#define GPIO1PIN_NPAA_PUT(v)            ( bFM_GPIO_PDORA_PA=(uint32_t)(!(v)) )

#define GPIO1PIN_NPAA_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPAA_INITIN(v) \
                                                           : GPIO1PIN_NPAA_INITOUT(v) )

#define GPIO1PIN_NPAA_INITIN(v)         do{ \
                                            bFM_GPIO_PCRA_PA=(v).bPullup; \
                                            bFM_GPIO_DDRA_PA=0u; \
                                            bFM_GPIO_PFRA_PA=0u; }while(0u)

#define GPIO1PIN_NPAA_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORA_PA=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRA_PA=1u; \
                                            bFM_GPIO_PFRA_PA=0u; }while(0u)

/*---- GPIO bit PAB ----*/
#define GPIO1PIN_PAB_GET                ( bFM_GPIO_PDIRA_PB )

#define GPIO1PIN_PAB_PUT(v)             ( bFM_GPIO_PDORA_PB=(v) )

#define GPIO1PIN_PAB_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PAB_INITIN(v) \
                                                           : GPIO1PIN_PAB_INITOUT(v) )

#define GPIO1PIN_PAB_INITIN(v)          do{ \
                                            bFM_GPIO_PCRA_PB=(v).bPullup; \
                                            bFM_GPIO_DDRA_PB=0u; \
                                            bFM_GPIO_PFRA_PB=0u; }while(0u)

#define GPIO1PIN_PAB_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORA_PB=(v).bInitVal; \
                                            bFM_GPIO_DDRA_PB=1u; \
                                            bFM_GPIO_PFRA_PB=0u; }while(0u)

/*---- GPIO bit NPAB ----*/
#define GPIO1PIN_NPAB_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRA_PB)) )

#define GPIO1PIN_NPAB_PUT(v)            ( bFM_GPIO_PDORA_PB=(uint32_t)(!(v)) )

#define GPIO1PIN_NPAB_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPAB_INITIN(v) \
                                                           : GPIO1PIN_NPAB_INITOUT(v) )

#define GPIO1PIN_NPAB_INITIN(v)         do{ \
                                            bFM_GPIO_PCRA_PB=(v).bPullup; \
                                            bFM_GPIO_DDRA_PB=0u; \
                                            bFM_GPIO_PFRA_PB=0u; }while(0u)

#define GPIO1PIN_NPAB_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORA_PB=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRA_PB=1u; \
                                            bFM_GPIO_PFRA_PB=0u; }while(0u)

/*---- GPIO bit PAC ----*/
#define GPIO1PIN_PAC_GET                ( bFM_GPIO_PDIRA_PC )

#define GPIO1PIN_PAC_PUT(v)             ( bFM_GPIO_PDORA_PC=(v) )

#define GPIO1PIN_PAC_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PAC_INITIN(v) \
                                                           : GPIO1PIN_PAC_INITOUT(v) )

#define GPIO1PIN_PAC_INITIN(v)          do{ \
                                            bFM_GPIO_PCRA_PC=(v).bPullup; \
                                            bFM_GPIO_DDRA_PC=0u; \
                                            bFM_GPIO_PFRA_PC=0u; }while(0u)

#define GPIO1PIN_PAC_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORA_PC=(v).bInitVal; \
                                            bFM_GPIO_DDRA_PC=1u; \
                                            bFM_GPIO_PFRA_PC=0u; }while(0u)

/*---- GPIO bit NPAC ----*/
#define GPIO1PIN_NPAC_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRA_PC)) )

#define GPIO1PIN_NPAC_PUT(v)            ( bFM_GPIO_PDORA_PC=(uint32_t)(!(v)) )

#define GPIO1PIN_NPAC_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPAC_INITIN(v) \
                                                           : GPIO1PIN_NPAC_INITOUT(v) )

#define GPIO1PIN_NPAC_INITIN(v)         do{ \
                                            bFM_GPIO_PCRA_PC=(v).bPullup; \
                                            bFM_GPIO_DDRA_PC=0u; \
                                            bFM_GPIO_PFRA_PC=0u; }while(0u)

#define GPIO1PIN_NPAC_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORA_PC=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRA_PC=1u; \
                                            bFM_GPIO_PFRA_PC=0u; }while(0u)

/*---- GPIO bit PAD ----*/
#define GPIO1PIN_PAD_GET                ( bFM_GPIO_PDIRA_PD )

#define GPIO1PIN_PAD_PUT(v)             ( bFM_GPIO_PDORA_PD=(v) )

#define GPIO1PIN_PAD_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PAD_INITIN(v) \
                                                           : GPIO1PIN_PAD_INITOUT(v) )

#define GPIO1PIN_PAD_INITIN(v)          do{ \
                                            bFM_GPIO_PCRA_PD=(v).bPullup; \
                                            bFM_GPIO_DDRA_PD=0u; \
                                            bFM_GPIO_PFRA_PD=0u; }while(0u)

#define GPIO1PIN_PAD_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORA_PD=(v).bInitVal; \
                                            bFM_GPIO_DDRA_PD=1u; \
                                            bFM_GPIO_PFRA_PD=0u; }while(0u)

/*---- GPIO bit NPAD ----*/
#define GPIO1PIN_NPAD_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRA_PD)) )

#define GPIO1PIN_NPAD_PUT(v)            ( bFM_GPIO_PDORA_PD=(uint32_t)(!(v)) )

#define GPIO1PIN_NPAD_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPAD_INITIN(v) \
                                                           : GPIO1PIN_NPAD_INITOUT(v) )

#define GPIO1PIN_NPAD_INITIN(v)         do{ \
                                            bFM_GPIO_PCRA_PD=(v).bPullup; \
                                            bFM_GPIO_DDRA_PD=0u; \
                                            bFM_GPIO_PFRA_PD=0u; }while(0u)

#define GPIO1PIN_NPAD_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORA_PD=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRA_PD=1u; \
                                            bFM_GPIO_PFRA_PD=0u; }while(0u)

/*---- GPIO bit PAE ----*/
#define GPIO1PIN_PAE_GET                ( bFM_GPIO_PDIRA_PE )

#define GPIO1PIN_PAE_PUT(v)             ( bFM_GPIO_PDORA_PE=(v) )

#define GPIO1PIN_PAE_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PAE_INITIN(v) \
                                                           : GPIO1PIN_PAE_INITOUT(v) )

#define GPIO1PIN_PAE_INITIN(v)          do{ \
                                            bFM_GPIO_PCRA_PE=(v).bPullup; \
                                            bFM_GPIO_DDRA_PE=0u; \
                                            bFM_GPIO_PFRA_PE=0u; }while(0u)

#define GPIO1PIN_PAE_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORA_PE=(v).bInitVal; \
                                            bFM_GPIO_DDRA_PE=1u; \
                                            bFM_GPIO_PFRA_PE=0u; }while(0u)

/*---- GPIO bit NPAE ----*/
#define GPIO1PIN_NPAE_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRA_PE)) )

#define GPIO1PIN_NPAE_PUT(v)            ( bFM_GPIO_PDORA_PE=(uint32_t)(!(v)) )

#define GPIO1PIN_NPAE_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPAE_INITIN(v) \
                                                           : GPIO1PIN_NPAE_INITOUT(v) )

#define GPIO1PIN_NPAE_INITIN(v)         do{ \
                                            bFM_GPIO_PCRA_PE=(v).bPullup; \
                                            bFM_GPIO_DDRA_PE=0u; \
                                            bFM_GPIO_PFRA_PE=0u; }while(0u)

#define GPIO1PIN_NPAE_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORA_PE=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRA_PE=1u; \
                                            bFM_GPIO_PFRA_PE=0u; }while(0u)

/*---- GPIO bit PAF ----*/
#define GPIO1PIN_PAF_GET                ( bFM_GPIO_PDIRA_PF )

#define GPIO1PIN_PAF_PUT(v)             ( bFM_GPIO_PDORA_PF=(v) )

#define GPIO1PIN_PAF_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PAF_INITIN(v) \
                                                           : GPIO1PIN_PAF_INITOUT(v) )

#define GPIO1PIN_PAF_INITIN(v)          do{ \
                                            bFM_GPIO_PCRA_PF=(v).bPullup; \
                                            bFM_GPIO_DDRA_PF=0u; \
                                            bFM_GPIO_PFRA_PF=0u; }while(0u)

#define GPIO1PIN_PAF_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORA_PF=(v).bInitVal; \
                                            bFM_GPIO_DDRA_PF=1u; \
                                            bFM_GPIO_PFRA_PF=0u; }while(0u)

/*---- GPIO bit NPAF ----*/
#define GPIO1PIN_NPAF_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRA_PF)) )

#define GPIO1PIN_NPAF_PUT(v)            ( bFM_GPIO_PDORA_PF=(uint32_t)(!(v)) )

#define GPIO1PIN_NPAF_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPAF_INITIN(v) \
                                                           : GPIO1PIN_NPAF_INITOUT(v) )

#define GPIO1PIN_NPAF_INITIN(v)         do{ \
                                            bFM_GPIO_PCRA_PF=(v).bPullup; \
                                            bFM_GPIO_DDRA_PF=0u; \
                                            bFM_GPIO_PFRA_PF=0u; }while(0u)

#define GPIO1PIN_NPAF_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORA_PF=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRA_PF=1u; \
                                            bFM_GPIO_PFRA_PF=0u; }while(0u)

/*---- GPIO bit PC0 ----*/
#define GPIO1PIN_PC0_GET                ( bFM_GPIO_PDIRC_P0 )

#define GPIO1PIN_PC0_PUT(v)             ( bFM_GPIO_PDORC_P0=(v) )

#define GPIO1PIN_PC0_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PC0_INITIN(v) \
                                                           : GPIO1PIN_PC0_INITOUT(v) )

#define GPIO1PIN_PC0_INITIN(v)          do{ \
                                            bFM_GPIO_PCRC_P0=(v).bPullup; \
                                            bFM_GPIO_DDRC_P0=0u; \
                                            bFM_GPIO_PFRC_P0=0u; }while(0u)

#define GPIO1PIN_PC0_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORC_P0=(v).bInitVal; \
                                            bFM_GPIO_DDRC_P0=1u; \
                                            bFM_GPIO_PFRC_P0=0u; }while(0u)

/*---- GPIO bit NPC0 ----*/
#define GPIO1PIN_NPC0_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRC_P0)) )

#define GPIO1PIN_NPC0_PUT(v)            ( bFM_GPIO_PDORC_P0=(uint32_t)(!(v)) )

#define GPIO1PIN_NPC0_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPC0_INITIN(v) \
                                                           : GPIO1PIN_NPC0_INITOUT(v) )

#define GPIO1PIN_NPC0_INITIN(v)         do{ \
                                            bFM_GPIO_PCRC_P0=(v).bPullup; \
                                            bFM_GPIO_DDRC_P0=0u; \
                                            bFM_GPIO_PFRC_P0=0u; }while(0u)

#define GPIO1PIN_NPC0_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORC_P0=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRC_P0=1u; \
                                            bFM_GPIO_PFRC_P0=0u; }while(0u)

/*---- GPIO bit PC1 ----*/
#define GPIO1PIN_PC1_GET                ( bFM_GPIO_PDIRC_P1 )

#define GPIO1PIN_PC1_PUT(v)             ( bFM_GPIO_PDORC_P1=(v) )

#define GPIO1PIN_PC1_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PC1_INITIN(v) \
                                                           : GPIO1PIN_PC1_INITOUT(v) )

#define GPIO1PIN_PC1_INITIN(v)          do{ \
                                            bFM_GPIO_PCRC_P1=(v).bPullup; \
                                            bFM_GPIO_DDRC_P1=0u; \
                                            bFM_GPIO_PFRC_P1=0u; }while(0u)

#define GPIO1PIN_PC1_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORC_P1=(v).bInitVal; \
                                            bFM_GPIO_DDRC_P1=1u; \
                                            bFM_GPIO_PFRC_P1=0u; }while(0u)

/*---- GPIO bit NPC1 ----*/
#define GPIO1PIN_NPC1_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRC_P1)) )

#define GPIO1PIN_NPC1_PUT(v)            ( bFM_GPIO_PDORC_P1=(uint32_t)(!(v)) )

#define GPIO1PIN_NPC1_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPC1_INITIN(v) \
                                                           : GPIO1PIN_NPC1_INITOUT(v) )

#define GPIO1PIN_NPC1_INITIN(v)         do{ \
                                            bFM_GPIO_PCRC_P1=(v).bPullup; \
                                            bFM_GPIO_DDRC_P1=0u; \
                                            bFM_GPIO_PFRC_P1=0u; }while(0u)

#define GPIO1PIN_NPC1_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORC_P1=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRC_P1=1u; \
                                            bFM_GPIO_PFRC_P1=0u; }while(0u)

/*---- GPIO bit PC2 ----*/
#define GPIO1PIN_PC2_GET                ( bFM_GPIO_PDIRC_P2 )

#define GPIO1PIN_PC2_PUT(v)             ( bFM_GPIO_PDORC_P2=(v) )

#define GPIO1PIN_PC2_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PC2_INITIN(v) \
                                                           : GPIO1PIN_PC2_INITOUT(v) )

#define GPIO1PIN_PC2_INITIN(v)          do{ \
                                            bFM_GPIO_PCRC_P2=(v).bPullup; \
                                            bFM_GPIO_DDRC_P2=0u; \
                                            bFM_GPIO_PFRC_P2=0u; }while(0u)

#define GPIO1PIN_PC2_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORC_P2=(v).bInitVal; \
                                            bFM_GPIO_DDRC_P2=1u; \
                                            bFM_GPIO_PFRC_P2=0u; }while(0u)

/*---- GPIO bit NPC2 ----*/
#define GPIO1PIN_NPC2_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRC_P2)) )

#define GPIO1PIN_NPC2_PUT(v)            ( bFM_GPIO_PDORC_P2=(uint32_t)(!(v)) )

#define GPIO1PIN_NPC2_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPC2_INITIN(v) \
                                                           : GPIO1PIN_NPC2_INITOUT(v) )

#define GPIO1PIN_NPC2_INITIN(v)         do{ \
                                            bFM_GPIO_PCRC_P2=(v).bPullup; \
                                            bFM_GPIO_DDRC_P2=0u; \
                                            bFM_GPIO_PFRC_P2=0u; }while(0u)

#define GPIO1PIN_NPC2_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORC_P2=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRC_P2=1u; \
                                            bFM_GPIO_PFRC_P2=0u; }while(0u)

/*---- GPIO bit PC3 ----*/
#define GPIO1PIN_PC3_GET                ( bFM_GPIO_PDIRC_P3 )

#define GPIO1PIN_PC3_PUT(v)             ( bFM_GPIO_PDORC_P3=(v) )

#define GPIO1PIN_PC3_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PC3_INITIN(v) \
                                                           : GPIO1PIN_PC3_INITOUT(v) )

#define GPIO1PIN_PC3_INITIN(v)          do{ \
                                            bFM_GPIO_PCRC_P3=(v).bPullup; \
                                            bFM_GPIO_DDRC_P3=0u; \
                                            bFM_GPIO_PFRC_P3=0u; }while(0u)

#define GPIO1PIN_PC3_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORC_P3=(v).bInitVal; \
                                            bFM_GPIO_DDRC_P3=1u; \
                                            bFM_GPIO_PFRC_P3=0u; }while(0u)

/*---- GPIO bit NPC3 ----*/
#define GPIO1PIN_NPC3_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRC_P3)) )

#define GPIO1PIN_NPC3_PUT(v)            ( bFM_GPIO_PDORC_P3=(uint32_t)(!(v)) )

#define GPIO1PIN_NPC3_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPC3_INITIN(v) \
                                                           : GPIO1PIN_NPC3_INITOUT(v) )

#define GPIO1PIN_NPC3_INITIN(v)         do{ \
                                            bFM_GPIO_PCRC_P3=(v).bPullup; \
                                            bFM_GPIO_DDRC_P3=0u; \
                                            bFM_GPIO_PFRC_P3=0u; }while(0u)

#define GPIO1PIN_NPC3_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORC_P3=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRC_P3=1u; \
                                            bFM_GPIO_PFRC_P3=0u; }while(0u)

/*---- GPIO bit PC4 ----*/
#define GPIO1PIN_PC4_GET                ( bFM_GPIO_PDIRC_P4 )

#define GPIO1PIN_PC4_PUT(v)             ( bFM_GPIO_PDORC_P4=(v) )

#define GPIO1PIN_PC4_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PC4_INITIN(v) \
                                                           : GPIO1PIN_PC4_INITOUT(v) )

#define GPIO1PIN_PC4_INITIN(v)          do{ \
                                            bFM_GPIO_PCRC_P4=(v).bPullup; \
                                            bFM_GPIO_DDRC_P4=0u; \
                                            bFM_GPIO_PFRC_P4=0u; }while(0u)

#define GPIO1PIN_PC4_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORC_P4=(v).bInitVal; \
                                            bFM_GPIO_DDRC_P4=1u; \
                                            bFM_GPIO_PFRC_P4=0u; }while(0u)

/*---- GPIO bit NPC4 ----*/
#define GPIO1PIN_NPC4_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRC_P4)) )

#define GPIO1PIN_NPC4_PUT(v)            ( bFM_GPIO_PDORC_P4=(uint32_t)(!(v)) )

#define GPIO1PIN_NPC4_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPC4_INITIN(v) \
                                                           : GPIO1PIN_NPC4_INITOUT(v) )

#define GPIO1PIN_NPC4_INITIN(v)         do{ \
                                            bFM_GPIO_PCRC_P4=(v).bPullup; \
                                            bFM_GPIO_DDRC_P4=0u; \
                                            bFM_GPIO_PFRC_P4=0u; }while(0u)

#define GPIO1PIN_NPC4_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORC_P4=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRC_P4=1u; \
                                            bFM_GPIO_PFRC_P4=0u; }while(0u)

/*---- GPIO bit PC5 ----*/
#define GPIO1PIN_PC5_GET                ( bFM_GPIO_PDIRC_P5 )

#define GPIO1PIN_PC5_PUT(v)             ( bFM_GPIO_PDORC_P5=(v) )

#define GPIO1PIN_PC5_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PC5_INITIN(v) \
                                                           : GPIO1PIN_PC5_INITOUT(v) )

#define GPIO1PIN_PC5_INITIN(v)          do{ \
                                            bFM_GPIO_PCRC_P5=(v).bPullup; \
                                            bFM_GPIO_DDRC_P5=0u; \
                                            bFM_GPIO_PFRC_P5=0u; }while(0u)

#define GPIO1PIN_PC5_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORC_P5=(v).bInitVal; \
                                            bFM_GPIO_DDRC_P5=1u; \
                                            bFM_GPIO_PFRC_P5=0u; }while(0u)

/*---- GPIO bit NPC5 ----*/
#define GPIO1PIN_NPC5_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRC_P5)) )

#define GPIO1PIN_NPC5_PUT(v)            ( bFM_GPIO_PDORC_P5=(uint32_t)(!(v)) )

#define GPIO1PIN_NPC5_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPC5_INITIN(v) \
                                                           : GPIO1PIN_NPC5_INITOUT(v) )

#define GPIO1PIN_NPC5_INITIN(v)         do{ \
                                            bFM_GPIO_PCRC_P5=(v).bPullup; \
                                            bFM_GPIO_DDRC_P5=0u; \
                                            bFM_GPIO_PFRC_P5=0u; }while(0u)

#define GPIO1PIN_NPC5_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORC_P5=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRC_P5=1u; \
                                            bFM_GPIO_PFRC_P5=0u; }while(0u)

/*---- GPIO bit PC6 ----*/
#define GPIO1PIN_PC6_GET                ( bFM_GPIO_PDIRC_P6 )

#define GPIO1PIN_PC6_PUT(v)             ( bFM_GPIO_PDORC_P6=(v) )

#define GPIO1PIN_PC6_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PC6_INITIN(v) \
                                                           : GPIO1PIN_PC6_INITOUT(v) )

#define GPIO1PIN_PC6_INITIN(v)          do{ \
                                            bFM_GPIO_PCRC_P6=(v).bPullup; \
                                            bFM_GPIO_DDRC_P6=0u; \
                                            bFM_GPIO_PFRC_P6=0u; }while(0u)

#define GPIO1PIN_PC6_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORC_P6=(v).bInitVal; \
                                            bFM_GPIO_DDRC_P6=1u; \
                                            bFM_GPIO_PFRC_P6=0u; }while(0u)

/*---- GPIO bit NPC6 ----*/
#define GPIO1PIN_NPC6_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRC_P6)) )

#define GPIO1PIN_NPC6_PUT(v)            ( bFM_GPIO_PDORC_P6=(uint32_t)(!(v)) )

#define GPIO1PIN_NPC6_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPC6_INITIN(v) \
                                                           : GPIO1PIN_NPC6_INITOUT(v) )

#define GPIO1PIN_NPC6_INITIN(v)         do{ \
                                            bFM_GPIO_PCRC_P6=(v).bPullup; \
                                            bFM_GPIO_DDRC_P6=0u; \
                                            bFM_GPIO_PFRC_P6=0u; }while(0u)

#define GPIO1PIN_NPC6_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORC_P6=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRC_P6=1u; \
                                            bFM_GPIO_PFRC_P6=0u; }while(0u)

/*---- GPIO bit PC7 ----*/
#define GPIO1PIN_PC7_GET                ( bFM_GPIO_PDIRC_P7 )

#define GPIO1PIN_PC7_PUT(v)             ( bFM_GPIO_PDORC_P7=(v) )

#define GPIO1PIN_PC7_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PC7_INITIN(v) \
                                                           : GPIO1PIN_PC7_INITOUT(v) )

#define GPIO1PIN_PC7_INITIN(v)          do{ \
                                            bFM_GPIO_PCRC_P7=(v).bPullup; \
                                            bFM_GPIO_DDRC_P7=0u; \
                                            bFM_GPIO_PFRC_P7=0u; }while(0u)

#define GPIO1PIN_PC7_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORC_P7=(v).bInitVal; \
                                            bFM_GPIO_DDRC_P7=1u; \
                                            bFM_GPIO_PFRC_P7=0u; }while(0u)

/*---- GPIO bit NPC7 ----*/
#define GPIO1PIN_NPC7_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRC_P7)) )

#define GPIO1PIN_NPC7_PUT(v)            ( bFM_GPIO_PDORC_P7=(uint32_t)(!(v)) )

#define GPIO1PIN_NPC7_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPC7_INITIN(v) \
                                                           : GPIO1PIN_NPC7_INITOUT(v) )

#define GPIO1PIN_NPC7_INITIN(v)         do{ \
                                            bFM_GPIO_PCRC_P7=(v).bPullup; \
                                            bFM_GPIO_DDRC_P7=0u; \
                                            bFM_GPIO_PFRC_P7=0u; }while(0u)

#define GPIO1PIN_NPC7_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORC_P7=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRC_P7=1u; \
                                            bFM_GPIO_PFRC_P7=0u; }while(0u)

/*---- GPIO bit PC8 ----*/
#define GPIO1PIN_PC8_GET                ( bFM_GPIO_PDIRC_P8 )

#define GPIO1PIN_PC8_PUT(v)             ( bFM_GPIO_PDORC_P8=(v) )

#define GPIO1PIN_PC8_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PC8_INITIN(v) \
                                                           : GPIO1PIN_PC8_INITOUT(v) )

#define GPIO1PIN_PC8_INITIN(v)          do{ \
                                            bFM_GPIO_PCRC_P8=(v).bPullup; \
                                            bFM_GPIO_DDRC_P8=0u; \
                                            bFM_GPIO_PFRC_P8=0u; }while(0u)

#define GPIO1PIN_PC8_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORC_P8=(v).bInitVal; \
                                            bFM_GPIO_DDRC_P8=1u; \
                                            bFM_GPIO_PFRC_P8=0u; }while(0u)

/*---- GPIO bit NPC8 ----*/
#define GPIO1PIN_NPC8_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRC_P8)) )

#define GPIO1PIN_NPC8_PUT(v)            ( bFM_GPIO_PDORC_P8=(uint32_t)(!(v)) )

#define GPIO1PIN_NPC8_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPC8_INITIN(v) \
                                                           : GPIO1PIN_NPC8_INITOUT(v) )

#define GPIO1PIN_NPC8_INITIN(v)         do{ \
                                            bFM_GPIO_PCRC_P8=(v).bPullup; \
                                            bFM_GPIO_DDRC_P8=0u; \
                                            bFM_GPIO_PFRC_P8=0u; }while(0u)

#define GPIO1PIN_NPC8_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORC_P8=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRC_P8=1u; \
                                            bFM_GPIO_PFRC_P8=0u; }while(0u)

/*---- GPIO bit PC9 ----*/
#define GPIO1PIN_PC9_GET                ( bFM_GPIO_PDIRC_P9 )

#define GPIO1PIN_PC9_PUT(v)             ( bFM_GPIO_PDORC_P9=(v) )

#define GPIO1PIN_PC9_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PC9_INITIN(v) \
                                                           : GPIO1PIN_PC9_INITOUT(v) )

#define GPIO1PIN_PC9_INITIN(v)          do{ \
                                            bFM_GPIO_PCRC_P9=(v).bPullup; \
                                            bFM_GPIO_DDRC_P9=0u; \
                                            bFM_GPIO_PFRC_P9=0u; }while(0u)

#define GPIO1PIN_PC9_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORC_P9=(v).bInitVal; \
                                            bFM_GPIO_DDRC_P9=1u; \
                                            bFM_GPIO_PFRC_P9=0u; }while(0u)

/*---- GPIO bit NPC9 ----*/
#define GPIO1PIN_NPC9_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRC_P9)) )

#define GPIO1PIN_NPC9_PUT(v)            ( bFM_GPIO_PDORC_P9=(uint32_t)(!(v)) )

#define GPIO1PIN_NPC9_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPC9_INITIN(v) \
                                                           : GPIO1PIN_NPC9_INITOUT(v) )

#define GPIO1PIN_NPC9_INITIN(v)         do{ \
                                            bFM_GPIO_PCRC_P9=(v).bPullup; \
                                            bFM_GPIO_DDRC_P9=0u; \
                                            bFM_GPIO_PFRC_P9=0u; }while(0u)

#define GPIO1PIN_NPC9_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORC_P9=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRC_P9=1u; \
                                            bFM_GPIO_PFRC_P9=0u; }while(0u)

/*---- GPIO bit PCA ----*/
#define GPIO1PIN_PCA_GET                ( bFM_GPIO_PDIRC_PA )

#define GPIO1PIN_PCA_PUT(v)             ( bFM_GPIO_PDORC_PA=(v) )

#define GPIO1PIN_PCA_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PCA_INITIN(v) \
                                                           : GPIO1PIN_PCA_INITOUT(v) )

#define GPIO1PIN_PCA_INITIN(v)          do{ \
                                            bFM_GPIO_PCRC_PA=(v).bPullup; \
                                            bFM_GPIO_DDRC_PA=0u; \
                                            bFM_GPIO_PFRC_PA=0u; }while(0u)

#define GPIO1PIN_PCA_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORC_PA=(v).bInitVal; \
                                            bFM_GPIO_DDRC_PA=1u; \
                                            bFM_GPIO_PFRC_PA=0u; }while(0u)

/*---- GPIO bit NPCA ----*/
#define GPIO1PIN_NPCA_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRC_PA)) )

#define GPIO1PIN_NPCA_PUT(v)            ( bFM_GPIO_PDORC_PA=(uint32_t)(!(v)) )

#define GPIO1PIN_NPCA_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPCA_INITIN(v) \
                                                           : GPIO1PIN_NPCA_INITOUT(v) )

#define GPIO1PIN_NPCA_INITIN(v)         do{ \
                                            bFM_GPIO_PCRC_PA=(v).bPullup; \
                                            bFM_GPIO_DDRC_PA=0u; \
                                            bFM_GPIO_PFRC_PA=0u; }while(0u)

#define GPIO1PIN_NPCA_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORC_PA=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRC_PA=1u; \
                                            bFM_GPIO_PFRC_PA=0u; }while(0u)

/*---- GPIO bit PCB ----*/
#define GPIO1PIN_PCB_GET                ( bFM_GPIO_PDIRC_PB )

#define GPIO1PIN_PCB_PUT(v)             ( bFM_GPIO_PDORC_PB=(v) )

#define GPIO1PIN_PCB_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PCB_INITIN(v) \
                                                           : GPIO1PIN_PCB_INITOUT(v) )

#define GPIO1PIN_PCB_INITIN(v)          do{ \
                                            bFM_GPIO_PCRC_PB=(v).bPullup; \
                                            bFM_GPIO_DDRC_PB=0u; \
                                            bFM_GPIO_PFRC_PB=0u; }while(0u)

#define GPIO1PIN_PCB_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORC_PB=(v).bInitVal; \
                                            bFM_GPIO_DDRC_PB=1u; \
                                            bFM_GPIO_PFRC_PB=0u; }while(0u)

/*---- GPIO bit NPCB ----*/
#define GPIO1PIN_NPCB_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRC_PB)) )

#define GPIO1PIN_NPCB_PUT(v)            ( bFM_GPIO_PDORC_PB=(uint32_t)(!(v)) )

#define GPIO1PIN_NPCB_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPCB_INITIN(v) \
                                                           : GPIO1PIN_NPCB_INITOUT(v) )

#define GPIO1PIN_NPCB_INITIN(v)         do{ \
                                            bFM_GPIO_PCRC_PB=(v).bPullup; \
                                            bFM_GPIO_DDRC_PB=0u; \
                                            bFM_GPIO_PFRC_PB=0u; }while(0u)

#define GPIO1PIN_NPCB_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORC_PB=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRC_PB=1u; \
                                            bFM_GPIO_PFRC_PB=0u; }while(0u)

/*---- GPIO bit PCC ----*/
#define GPIO1PIN_PCC_GET                ( bFM_GPIO_PDIRC_PC )

#define GPIO1PIN_PCC_PUT(v)             ( bFM_GPIO_PDORC_PC=(v) )

#define GPIO1PIN_PCC_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PCC_INITIN(v) \
                                                           : GPIO1PIN_PCC_INITOUT(v) )

#define GPIO1PIN_PCC_INITIN(v)          do{ \
                                            bFM_GPIO_PCRC_PC=(v).bPullup; \
                                            bFM_GPIO_DDRC_PC=0u; \
                                            bFM_GPIO_PFRC_PC=0u; }while(0u)

#define GPIO1PIN_PCC_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORC_PC=(v).bInitVal; \
                                            bFM_GPIO_DDRC_PC=1u; \
                                            bFM_GPIO_PFRC_PC=0u; }while(0u)

/*---- GPIO bit NPCC ----*/
#define GPIO1PIN_NPCC_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRC_PC)) )

#define GPIO1PIN_NPCC_PUT(v)            ( bFM_GPIO_PDORC_PC=(uint32_t)(!(v)) )

#define GPIO1PIN_NPCC_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPCC_INITIN(v) \
                                                           : GPIO1PIN_NPCC_INITOUT(v) )

#define GPIO1PIN_NPCC_INITIN(v)         do{ \
                                            bFM_GPIO_PCRC_PC=(v).bPullup; \
                                            bFM_GPIO_DDRC_PC=0u; \
                                            bFM_GPIO_PFRC_PC=0u; }while(0u)

#define GPIO1PIN_NPCC_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORC_PC=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRC_PC=1u; \
                                            bFM_GPIO_PFRC_PC=0u; }while(0u)

/*---- GPIO bit PCD ----*/
#define GPIO1PIN_PCD_GET                ( bFM_GPIO_PDIRC_PD )

#define GPIO1PIN_PCD_PUT(v)             ( bFM_GPIO_PDORC_PD=(v) )

#define GPIO1PIN_PCD_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PCD_INITIN(v) \
                                                           : GPIO1PIN_PCD_INITOUT(v) )

#define GPIO1PIN_PCD_INITIN(v)          do{ \
                                            bFM_GPIO_PCRC_PD=(v).bPullup; \
                                            bFM_GPIO_DDRC_PD=0u; \
                                            bFM_GPIO_PFRC_PD=0u; }while(0u)

#define GPIO1PIN_PCD_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORC_PD=(v).bInitVal; \
                                            bFM_GPIO_DDRC_PD=1u; \
                                            bFM_GPIO_PFRC_PD=0u; }while(0u)

/*---- GPIO bit NPCD ----*/
#define GPIO1PIN_NPCD_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRC_PD)) )

#define GPIO1PIN_NPCD_PUT(v)            ( bFM_GPIO_PDORC_PD=(uint32_t)(!(v)) )

#define GPIO1PIN_NPCD_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPCD_INITIN(v) \
                                                           : GPIO1PIN_NPCD_INITOUT(v) )

#define GPIO1PIN_NPCD_INITIN(v)         do{ \
                                            bFM_GPIO_PCRC_PD=(v).bPullup; \
                                            bFM_GPIO_DDRC_PD=0u; \
                                            bFM_GPIO_PFRC_PD=0u; }while(0u)

#define GPIO1PIN_NPCD_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORC_PD=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRC_PD=1u; \
                                            bFM_GPIO_PFRC_PD=0u; }while(0u)

/*---- GPIO bit PCE ----*/
#define GPIO1PIN_PCE_GET                ( bFM_GPIO_PDIRC_PE )

#define GPIO1PIN_PCE_PUT(v)             ( bFM_GPIO_PDORC_PE=(v) )

#define GPIO1PIN_PCE_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PCE_INITIN(v) \
                                                           : GPIO1PIN_PCE_INITOUT(v) )

#define GPIO1PIN_PCE_INITIN(v)          do{ \
                                            bFM_GPIO_PCRC_PE=(v).bPullup; \
                                            bFM_GPIO_DDRC_PE=0u; \
                                            bFM_GPIO_PFRC_PE=0u; }while(0u)

#define GPIO1PIN_PCE_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORC_PE=(v).bInitVal; \
                                            bFM_GPIO_DDRC_PE=1u; \
                                            bFM_GPIO_PFRC_PE=0u; }while(0u)

/*---- GPIO bit NPCE ----*/
#define GPIO1PIN_NPCE_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRC_PE)) )

#define GPIO1PIN_NPCE_PUT(v)            ( bFM_GPIO_PDORC_PE=(uint32_t)(!(v)) )

#define GPIO1PIN_NPCE_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPCE_INITIN(v) \
                                                           : GPIO1PIN_NPCE_INITOUT(v) )

#define GPIO1PIN_NPCE_INITIN(v)         do{ \
                                            bFM_GPIO_PCRC_PE=(v).bPullup; \
                                            bFM_GPIO_DDRC_PE=0u; \
                                            bFM_GPIO_PFRC_PE=0u; }while(0u)

#define GPIO1PIN_NPCE_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORC_PE=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRC_PE=1u; \
                                            bFM_GPIO_PFRC_PE=0u; }while(0u)

/*---- GPIO bit PCF ----*/
#define GPIO1PIN_PCF_GET                ( bFM_GPIO_PDIRC_PF )

#define GPIO1PIN_PCF_PUT(v)             ( bFM_GPIO_PDORC_PF=(v) )

#define GPIO1PIN_PCF_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PCF_INITIN(v) \
                                                           : GPIO1PIN_PCF_INITOUT(v) )

#define GPIO1PIN_PCF_INITIN(v)          do{ \
                                            bFM_GPIO_PCRC_PF=(v).bPullup; \
                                            bFM_GPIO_DDRC_PF=0u; \
                                            bFM_GPIO_PFRC_PF=0u; }while(0u)

#define GPIO1PIN_PCF_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORC_PF=(v).bInitVal; \
                                            bFM_GPIO_DDRC_PF=1u; \
                                            bFM_GPIO_PFRC_PF=0u; }while(0u)

/*---- GPIO bit NPCF ----*/
#define GPIO1PIN_NPCF_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRC_PF)) )

#define GPIO1PIN_NPCF_PUT(v)            ( bFM_GPIO_PDORC_PF=(uint32_t)(!(v)) )

#define GPIO1PIN_NPCF_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPCF_INITIN(v) \
                                                           : GPIO1PIN_NPCF_INITOUT(v) )

#define GPIO1PIN_NPCF_INITIN(v)         do{ \
                                            bFM_GPIO_PCRC_PF=(v).bPullup; \
                                            bFM_GPIO_DDRC_PF=0u; \
                                            bFM_GPIO_PFRC_PF=0u; }while(0u)

#define GPIO1PIN_NPCF_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORC_PF=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRC_PF=1u; \
                                            bFM_GPIO_PFRC_PF=0u; }while(0u)

/*---- GPIO bit PD0 ----*/
#define GPIO1PIN_PD0_GET                ( bFM_GPIO_PDIRD_P0 )

#define GPIO1PIN_PD0_PUT(v)             ( bFM_GPIO_PDORD_P0=(v) )

#define GPIO1PIN_PD0_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PD0_INITIN(v) \
                                                           : GPIO1PIN_PD0_INITOUT(v) )

#define GPIO1PIN_PD0_INITIN(v)          do{ \
                                            bFM_GPIO_PCRD_P0=(v).bPullup; \
                                            bFM_GPIO_DDRD_P0=0u; \
                                            bFM_GPIO_PFRD_P0=0u; }while(0u)

#define GPIO1PIN_PD0_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORD_P0=(v).bInitVal; \
                                            bFM_GPIO_DDRD_P0=1u; \
                                            bFM_GPIO_PFRD_P0=0u; }while(0u)

/*---- GPIO bit NPD0 ----*/
#define GPIO1PIN_NPD0_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRD_P0)) )

#define GPIO1PIN_NPD0_PUT(v)            ( bFM_GPIO_PDORD_P0=(uint32_t)(!(v)) )

#define GPIO1PIN_NPD0_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPD0_INITIN(v) \
                                                           : GPIO1PIN_NPD0_INITOUT(v) )

#define GPIO1PIN_NPD0_INITIN(v)         do{ \
                                            bFM_GPIO_PCRD_P0=(v).bPullup; \
                                            bFM_GPIO_DDRD_P0=0u; \
                                            bFM_GPIO_PFRD_P0=0u; }while(0u)

#define GPIO1PIN_NPD0_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORD_P0=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRD_P0=1u; \
                                            bFM_GPIO_PFRD_P0=0u; }while(0u)

/*---- GPIO bit PD1 ----*/
#define GPIO1PIN_PD1_GET                ( bFM_GPIO_PDIRD_P1 )

#define GPIO1PIN_PD1_PUT(v)             ( bFM_GPIO_PDORD_P1=(v) )

#define GPIO1PIN_PD1_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PD1_INITIN(v) \
                                                           : GPIO1PIN_PD1_INITOUT(v) )

#define GPIO1PIN_PD1_INITIN(v)          do{ \
                                            bFM_GPIO_PCRD_P1=(v).bPullup; \
                                            bFM_GPIO_DDRD_P1=0u; \
                                            bFM_GPIO_PFRD_P1=0u; }while(0u)

#define GPIO1PIN_PD1_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORD_P1=(v).bInitVal; \
                                            bFM_GPIO_DDRD_P1=1u; \
                                            bFM_GPIO_PFRD_P1=0u; }while(0u)

/*---- GPIO bit NPD1 ----*/
#define GPIO1PIN_NPD1_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRD_P1)) )

#define GPIO1PIN_NPD1_PUT(v)            ( bFM_GPIO_PDORD_P1=(uint32_t)(!(v)) )

#define GPIO1PIN_NPD1_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPD1_INITIN(v) \
                                                           : GPIO1PIN_NPD1_INITOUT(v) )

#define GPIO1PIN_NPD1_INITIN(v)         do{ \
                                            bFM_GPIO_PCRD_P1=(v).bPullup; \
                                            bFM_GPIO_DDRD_P1=0u; \
                                            bFM_GPIO_PFRD_P1=0u; }while(0u)

#define GPIO1PIN_NPD1_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORD_P1=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRD_P1=1u; \
                                            bFM_GPIO_PFRD_P1=0u; }while(0u)

/*---- GPIO bit PD2 ----*/
#define GPIO1PIN_PD2_GET                ( bFM_GPIO_PDIRD_P2 )

#define GPIO1PIN_PD2_PUT(v)             ( bFM_GPIO_PDORD_P2=(v) )

#define GPIO1PIN_PD2_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PD2_INITIN(v) \
                                                           : GPIO1PIN_PD2_INITOUT(v) )

#define GPIO1PIN_PD2_INITIN(v)          do{ \
                                            bFM_GPIO_PCRD_P2=(v).bPullup; \
                                            bFM_GPIO_DDRD_P2=0u; \
                                            bFM_GPIO_PFRD_P2=0u; }while(0u)

#define GPIO1PIN_PD2_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORD_P2=(v).bInitVal; \
                                            bFM_GPIO_DDRD_P2=1u; \
                                            bFM_GPIO_PFRD_P2=0u; }while(0u)

/*---- GPIO bit NPD2 ----*/
#define GPIO1PIN_NPD2_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRD_P2)) )

#define GPIO1PIN_NPD2_PUT(v)            ( bFM_GPIO_PDORD_P2=(uint32_t)(!(v)) )

#define GPIO1PIN_NPD2_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPD2_INITIN(v) \
                                                           : GPIO1PIN_NPD2_INITOUT(v) )

#define GPIO1PIN_NPD2_INITIN(v)         do{ \
                                            bFM_GPIO_PCRD_P2=(v).bPullup; \
                                            bFM_GPIO_DDRD_P2=0u; \
                                            bFM_GPIO_PFRD_P2=0u; }while(0u)

#define GPIO1PIN_NPD2_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORD_P2=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRD_P2=1u; \
                                            bFM_GPIO_PFRD_P2=0u; }while(0u)

/*---- GPIO bit PE0 ----*/
#define GPIO1PIN_PE0_GET                ( bFM_GPIO_PDIRE_P0 )

#define GPIO1PIN_PE0_PUT(v)             ( bFM_GPIO_PDORE_P0=(v) )

#define GPIO1PIN_PE0_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PE0_INITIN(v) \
                                                           : GPIO1PIN_PE0_INITOUT(v) )

#define GPIO1PIN_PE0_INITIN(v)          do{ \
                                            bFM_GPIO_PCRE_P0=(v).bPullup; \
                                            bFM_GPIO_DDRE_P0=0u; \
                                            bFM_GPIO_PFRE_P0=0u; }while(0u)

#define GPIO1PIN_PE0_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORE_P0=(v).bInitVal; \
                                            bFM_GPIO_DDRE_P0=1u; \
                                            bFM_GPIO_PFRE_P0=0u; }while(0u)

/*---- GPIO bit NPE0 ----*/
#define GPIO1PIN_NPE0_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRE_P0)) )

#define GPIO1PIN_NPE0_PUT(v)            ( bFM_GPIO_PDORE_P0=(uint32_t)(!(v)) )

#define GPIO1PIN_NPE0_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPE0_INITIN(v) \
                                                           : GPIO1PIN_NPE0_INITOUT(v) )

#define GPIO1PIN_NPE0_INITIN(v)         do{ \
                                            bFM_GPIO_PCRE_P0=(v).bPullup; \
                                            bFM_GPIO_DDRE_P0=0u; \
                                            bFM_GPIO_PFRE_P0=0u; }while(0u)

#define GPIO1PIN_NPE0_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORE_P0=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRE_P0=1u; \
                                            bFM_GPIO_PFRE_P0=0u; }while(0u)

/*---- GPIO bit PE2 ----*/
#define GPIO1PIN_PE2_GET                ( bFM_GPIO_PDIRE_P2 )

#define GPIO1PIN_PE2_PUT(v)             ( bFM_GPIO_PDORE_P2=(v) )

#define GPIO1PIN_PE2_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PE2_INITIN(v) \
                                                           : GPIO1PIN_PE2_INITOUT(v) )

#define GPIO1PIN_PE2_INITIN(v)          do{ \
                                            PINCONFIG_SET_REG(FM_GPIO->SPSR, 2u, 2u, 0u); \
                                            bFM_GPIO_PCRE_P2=(v).bPullup; \
                                            bFM_GPIO_DDRE_P2=0u; \
                                            bFM_GPIO_PFRE_P2=0u; }while(0u)

#define GPIO1PIN_PE2_INITOUT(v)         do{ \
                                            PINCONFIG_SET_REG(FM_GPIO->SPSR, 2u, 2u, 0u); \
                                            bFM_GPIO_PDORE_P2=(v).bInitVal; \
                                            bFM_GPIO_DDRE_P2=1u; \
                                            bFM_GPIO_PFRE_P2=0u; }while(0u)

/*---- GPIO bit NPE2 ----*/
#define GPIO1PIN_NPE2_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRE_P2)) )

#define GPIO1PIN_NPE2_PUT(v)            ( bFM_GPIO_PDORE_P2=(uint32_t)(!(v)) )

#define GPIO1PIN_NPE2_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPE2_INITIN(v) \
                                                           : GPIO1PIN_NPE2_INITOUT(v) )

#define GPIO1PIN_NPE2_INITIN(v)         do{ \
                                            PINCONFIG_SET_REG(FM_GPIO->SPSR, 2u, 2u, 0u); \
                                            bFM_GPIO_PCRE_P2=(v).bPullup; \
                                            bFM_GPIO_DDRE_P2=0u; \
                                            bFM_GPIO_PFRE_P2=0u; }while(0u)

#define GPIO1PIN_NPE2_INITOUT(v)        do{ \
                                            PINCONFIG_SET_REG(FM_GPIO->SPSR, 2u, 2u, 0u); \
                                            bFM_GPIO_PDORE_P2=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRE_P2=1u; \
                                            bFM_GPIO_PFRE_P2=0u; }while(0u)

/*---- GPIO bit PE3 ----*/
#define GPIO1PIN_PE3_GET                ( bFM_GPIO_PDIRE_P3 )

#define GPIO1PIN_PE3_PUT(v)             ( bFM_GPIO_PDORE_P3=(v) )

#define GPIO1PIN_PE3_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PE3_INITIN(v) \
                                                           : GPIO1PIN_PE3_INITOUT(v) )

#define GPIO1PIN_PE3_INITIN(v)          do{ \
                                            PINCONFIG_SET_REG(FM_GPIO->SPSR, 2u, 2u, 0u); \
                                            bFM_GPIO_PCRE_P3=(v).bPullup; \
                                            bFM_GPIO_DDRE_P3=0u; \
                                            bFM_GPIO_PFRE_P3=0u; }while(0u)

#define GPIO1PIN_PE3_INITOUT(v)         do{ \
                                            PINCONFIG_SET_REG(FM_GPIO->SPSR, 2u, 2u, 0u); \
                                            bFM_GPIO_PDORE_P3=(v).bInitVal; \
                                            bFM_GPIO_DDRE_P3=1u; \
                                            bFM_GPIO_PFRE_P3=0u; }while(0u)

/*---- GPIO bit NPE3 ----*/
#define GPIO1PIN_NPE3_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRE_P3)) )

#define GPIO1PIN_NPE3_PUT(v)            ( bFM_GPIO_PDORE_P3=(uint32_t)(!(v)) )

#define GPIO1PIN_NPE3_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPE3_INITIN(v) \
                                                           : GPIO1PIN_NPE3_INITOUT(v) )

#define GPIO1PIN_NPE3_INITIN(v)         do{ \
                                            PINCONFIG_SET_REG(FM_GPIO->SPSR, 2u, 2u, 0u); \
                                            bFM_GPIO_PCRE_P3=(v).bPullup; \
                                            bFM_GPIO_DDRE_P3=0u; \
                                            bFM_GPIO_PFRE_P3=0u; }while(0u)

#define GPIO1PIN_NPE3_INITOUT(v)        do{ \
                                            PINCONFIG_SET_REG(FM_GPIO->SPSR, 2u, 2u, 0u); \
                                            bFM_GPIO_PDORE_P3=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRE_P3=1u; \
                                            bFM_GPIO_PFRE_P3=0u; }while(0u)

/******************************************************************************
   PIN RELOCATION
*******************************************************************************/

/*--- ADTG_0_ADC0 ---*/
#define SetPinFunc_ADTG_0_ADC0(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 12u, 4u,  1u ); \
                                            bFM_GPIO_PFRA_PE = 1u; \
                                        }while (0u)

/*--- ADTG_0_ADC1 ---*/
#define SetPinFunc_ADTG_0_ADC1(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 16u, 4u,  1u ); \
                                            bFM_GPIO_PFRA_PE = 1u; \
                                        }while (0u)

/*--- ADTG_0_ADC2 ---*/
#define SetPinFunc_ADTG_0_ADC2(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 20u, 4u,  1u ); \
                                            bFM_GPIO_PFRA_PE = 1u; \
                                        }while (0u)

/*--- ADTG_1_ADC0 ---*/
#define SetPinFunc_ADTG_1_ADC0(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 12u, 4u,  2u ); \
                                            bFM_GPIO_PFR0_PA = 1u; \
                                        }while (0u)

/*--- ADTG_1_ADC1 ---*/
#define SetPinFunc_ADTG_1_ADC1(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 16u, 4u,  2u ); \
                                            bFM_GPIO_PFR0_PA = 1u; \
                                        }while (0u)

/*--- ADTG_1_ADC2 ---*/
#define SetPinFunc_ADTG_1_ADC2(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 20u, 4u,  2u ); \
                                            bFM_GPIO_PFR0_PA = 1u; \
                                        }while (0u)

/*--- ADTG_2_ADC0 ---*/
#define SetPinFunc_ADTG_2_ADC0(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 12u, 4u,  3u ); \
                                            bFM_GPIO_PFR3_P8 = 1u; \
                                        }while (0u)

/*--- ADTG_2_ADC1 ---*/
#define SetPinFunc_ADTG_2_ADC1(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 16u, 4u,  3u ); \
                                            bFM_GPIO_PFR3_P8 = 1u; \
                                        }while (0u)

/*--- ADTG_2_ADC2 ---*/
#define SetPinFunc_ADTG_2_ADC2(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 20u, 4u,  3u ); \
                                            bFM_GPIO_PFR3_P8 = 1u; \
                                        }while (0u)

/*--- ADTG_3_ADC0 ---*/
#define SetPinFunc_ADTG_3_ADC0(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 12u, 4u,  4u ); \
                                            bFM_GPIO_PFR6_P3 = 1u; \
                                        }while (0u)

/*--- ADTG_3_ADC1 ---*/
#define SetPinFunc_ADTG_3_ADC1(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 16u, 4u,  4u ); \
                                            bFM_GPIO_PFR6_P3 = 1u; \
                                        }while (0u)

/*--- ADTG_3_ADC2 ---*/
#define SetPinFunc_ADTG_3_ADC2(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 20u, 4u,  4u ); \
                                            bFM_GPIO_PFR6_P3 = 1u; \
                                        }while (0u)

/*--- ADTG_4_ADC0 ---*/
#define SetPinFunc_ADTG_4_ADC0(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 12u, 4u,  5u ); \
                                            bFM_GPIO_PFR2_P1 = 1u; \
                                        }while (0u)

/*--- ADTG_4_ADC1 ---*/
#define SetPinFunc_ADTG_4_ADC1(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 16u, 4u,  5u ); \
                                            bFM_GPIO_PFR2_P1 = 1u; \
                                        }while (0u)

/*--- ADTG_4_ADC2 ---*/
#define SetPinFunc_ADTG_4_ADC2(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 20u, 4u,  5u ); \
                                            bFM_GPIO_PFR2_P1 = 1u; \
                                        }while (0u)

/*--- ADTG_5_ADC0 ---*/
#define SetPinFunc_ADTG_5_ADC0(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 12u, 4u,  6u ); \
                                            bFM_GPIO_PFR6_PE = 1u; \
                                        }while (0u)

/*--- ADTG_5_ADC1 ---*/
#define SetPinFunc_ADTG_5_ADC1(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 16u, 4u,  6u ); \
                                            bFM_GPIO_PFR6_PE = 1u; \
                                        }while (0u)

/*--- ADTG_5_ADC2 ---*/
#define SetPinFunc_ADTG_5_ADC2(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 20u, 4u,  6u ); \
                                            bFM_GPIO_PFR6_PE = 1u; \
                                        }while (0u)

/*--- ADTG_7_ADC0 ---*/
#define SetPinFunc_ADTG_7_ADC0(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 12u, 4u,  8u ); \
                                            bFM_GPIO_PFR7_PE = 1u; \
                                        }while (0u)

/*--- ADTG_7_ADC1 ---*/
#define SetPinFunc_ADTG_7_ADC1(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 16u, 4u,  8u ); \
                                            bFM_GPIO_PFR7_PE = 1u; \
                                        }while (0u)

/*--- ADTG_7_ADC2 ---*/
#define SetPinFunc_ADTG_7_ADC2(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 20u, 4u,  8u ); \
                                            bFM_GPIO_PFR7_PE = 1u; \
                                        }while (0u)

/*--- ADTG_8_ADC0 ---*/
#define SetPinFunc_ADTG_8_ADC0(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 12u, 4u,  9u ); \
                                            bFM_GPIO_PFR7_P0 = 1u; \
                                        }while (0u)

/*--- ADTG_8_ADC1 ---*/
#define SetPinFunc_ADTG_8_ADC1(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 16u, 4u,  9u ); \
                                            bFM_GPIO_PFR7_P0 = 1u; \
                                        }while (0u)

/*--- ADTG_8_ADC2 ---*/
#define SetPinFunc_ADTG_8_ADC2(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 20u, 4u,  9u ); \
                                            bFM_GPIO_PFR7_P0 = 1u; \
                                        }while (0u)

/*--- AIN0_0 ---*/
#define SetPinFunc_AIN0_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 0u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P0 = 1u; \
                                        }while (0u)

/*--- AIN0_2 ---*/
#define SetPinFunc_AIN0_2(dummy)        do{ \
                                            bFM_GPIO_ADE_AN00=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 0u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_P0 = 1u; \
                                        }while (0u)

/*--- AIN1_0 ---*/
#define SetPinFunc_AIN1_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 6u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_P5 = 1u; \
                                        }while (0u)

/*--- AIN1_2 ---*/
#define SetPinFunc_AIN1_2(dummy)        do{ \
                                            bFM_GPIO_ADE_AN05=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 6u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_P5 = 1u; \
                                        }while (0u)

/*--- AIN2_0 ---*/
#define SetPinFunc_AIN2_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR14, 0u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P0 = 1u; \
                                        }while (0u)

/*--- AIN2_1 ---*/
#define SetPinFunc_AIN2_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR14, 0u, 2u,  2u ); \
                                            bFM_GPIO_PFR0_PA = 1u; \
                                        }while (0u)

/*--- AIN3_0 ---*/
#define SetPinFunc_AIN3_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR18, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_PC = 1u; \
                                        }while (0u)

/*--- AIN3_1 ---*/
#define SetPinFunc_AIN3_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR18, 4u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_P9 = 1u; \
                                        }while (0u)

/*--- BIN0_0 ---*/
#define SetPinFunc_BIN0_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 2u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P1 = 1u; \
                                        }while (0u)

/*--- BIN0_2 ---*/
#define SetPinFunc_BIN0_2(dummy)        do{ \
                                            bFM_GPIO_ADE_AN01=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 2u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_P1 = 1u; \
                                        }while (0u)

/*--- BIN1_0 ---*/
#define SetPinFunc_BIN1_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 8u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_P6 = 1u; \
                                        }while (0u)

/*--- BIN1_2 ---*/
#define SetPinFunc_BIN1_2(dummy)        do{ \
                                            bFM_GPIO_ADE_AN06=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 8u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_P6 = 1u; \
                                        }while (0u)

/*--- BIN2_0 ---*/
#define SetPinFunc_BIN2_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR14, 2u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P1 = 1u; \
                                        }while (0u)

/*--- BIN2_1 ---*/
#define SetPinFunc_BIN2_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR14, 2u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_P2 = 1u; \
                                        }while (0u)

/*--- BIN3_0 ---*/
#define SetPinFunc_BIN3_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR18, 6u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_PD = 1u; \
                                        }while (0u)

/*--- BIN3_1 ---*/
#define SetPinFunc_BIN3_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR18, 6u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PA = 1u; \
                                        }while (0u)

/*--- CROUT_0 ---*/
#define SetPinFunc_CROUT_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 1u, 2u,  1u ); \
                                            bFM_GPIO_PFR2_P1 = 1u; \
                                        }while (0u)

/*--- CROUT_1 ---*/
#define SetPinFunc_CROUT_1(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 1u, 2u,  2u ); \
                                            bFM_GPIO_PFRC_P7 = 1u; \
                                        }while (0u)

/*--- CTS4_1 ---*/
#define SetPinFunc_CTS4_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 2u, 2u,  2u ); \
                                            bFM_GPIO_PFRD_P2 = 1u; \
                                        }while (0u)

/*--- CTS5_0 ---*/
#define SetPinFunc_CTS5_0(dummy)        do{ \
                                            bFM_GPIO_ADE_AN24=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 30u, 2u,  1u ); \
                                            bFM_GPIO_PFR2_PA = 1u; \
                                        }while (0u)

/*--- DA0 ---*/
#define SetPinFunc_DA0(dummy)           do{ \
                                            /* bFM_DAC0_DACR_DAE=1u; */ \
                                        }while (0u)

/*--- DA1 ---*/
#define SetPinFunc_DA1(dummy)           do{ \
                                            /* bFM_DAC1_DACR_DAE=1u; */ \
                                        }while (0u)

/*--- DTTI0X_0 ---*/
#define SetPinFunc_DTTI0X_0(dummy)      do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 16u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_P8 = 1u; \
                                        }while (0u)

/*--- DTTI1X_0 ---*/
#define SetPinFunc_DTTI1X_0(dummy)      do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 16u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_PD = 1u; \
                                        }while (0u)

/*--- DTTI2X_0 ---*/
#define SetPinFunc_DTTI2X_0(dummy)      do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 16u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P6 = 1u; \
                                        }while (0u)

/*--- FRCK0_0 ---*/
#define SetPinFunc_FRCK0_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 18u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_P3 = 1u; \
                                        }while (0u)

/*--- FRCK1_0 ---*/
#define SetPinFunc_FRCK1_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 18u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_PE = 1u; \
                                        }while (0u)

/*--- FRCK2_0 ---*/
#define SetPinFunc_FRCK2_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 18u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_PB = 1u; \
                                        }while (0u)

/*--- FRCK2_1 ---*/
#define SetPinFunc_FRCK2_1(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 18u, 2u,  2u ); \
                                            bFM_GPIO_PFRD_P2 = 1u; \
                                        }while (0u)

/*--- IC00_0 ---*/
#define SetPinFunc_IC00_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 20u, 3u,  1u ); \
                                            bFM_GPIO_PFR3_P7 = 1u; \
                                        }while (0u)

/*--- IC00_LSYN0 ---*/
#define SetPinFunc_IC00_LSYN0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 20u, 3u,  4u ); \
                                        }while (0u)

/*--- IC00_LSYN4 ---*/
#define SetPinFunc_IC00_LSYN4(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 20u, 3u,  5u ); \
                                        }while (0u)

/*--- IC01_0 ---*/
#define SetPinFunc_IC01_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 23u, 3u,  1u ); \
                                            bFM_GPIO_PFR3_P6 = 1u; \
                                        }while (0u)

/*--- IC01_LSYN1 ---*/
#define SetPinFunc_IC01_LSYN1(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 23u, 3u,  4u ); \
                                        }while (0u)

/*--- IC01_LSYN5 ---*/
#define SetPinFunc_IC01_LSYN5(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 23u, 3u,  5u ); \
                                        }while (0u)

/*--- IC02_0 ---*/
#define SetPinFunc_IC02_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 26u, 3u,  1u ); \
                                            bFM_GPIO_PFR3_P5 = 1u; \
                                        }while (0u)

/*--- IC02_LSYN2 ---*/
#define SetPinFunc_IC02_LSYN2(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 26u, 3u,  4u ); \
                                        }while (0u)

/*--- IC02_LSYN6 ---*/
#define SetPinFunc_IC02_LSYN6(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 26u, 3u,  5u ); \
                                        }while (0u)

/*--- IC03_0 ---*/
#define SetPinFunc_IC03_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 29u, 3u,  1u ); \
                                            bFM_GPIO_PFR3_P4 = 1u; \
                                        }while (0u)

/*--- IC03_LSYN3 ---*/
#define SetPinFunc_IC03_LSYN3(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 29u, 3u,  4u ); \
                                        }while (0u)

/*--- IC03_LSYN7 ---*/
#define SetPinFunc_IC03_LSYN7(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 29u, 3u,  5u ); \
                                        }while (0u)

/*--- IC10_0 ---*/
#define SetPinFunc_IC10_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 20u, 3u,  1u ); \
                                            bFM_GPIO_PFR7_P8 = 1u; \
                                        }while (0u)

/*--- IC10_LSYN0 ---*/
#define SetPinFunc_IC10_LSYN0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 20u, 3u,  4u ); \
                                        }while (0u)

/*--- IC10_LSYN4 ---*/
#define SetPinFunc_IC10_LSYN4(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 20u, 3u,  5u ); \
                                        }while (0u)

/*--- IC11_0 ---*/
#define SetPinFunc_IC11_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 23u, 3u,  1u ); \
                                            bFM_GPIO_PFR7_P9 = 1u; \
                                        }while (0u)

/*--- IC11_LSYN1 ---*/
#define SetPinFunc_IC11_LSYN1(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 23u, 3u,  4u ); \
                                        }while (0u)

/*--- IC11_LSYN5 ---*/
#define SetPinFunc_IC11_LSYN5(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 23u, 3u,  5u ); \
                                        }while (0u)

/*--- IC12_0 ---*/
#define SetPinFunc_IC12_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 26u, 3u,  1u ); \
                                            bFM_GPIO_PFR7_PA = 1u; \
                                        }while (0u)

/*--- IC12_LSYN2 ---*/
#define SetPinFunc_IC12_LSYN2(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 26u, 3u,  4u ); \
                                        }while (0u)

/*--- IC12_LSYN6 ---*/
#define SetPinFunc_IC12_LSYN6(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 26u, 3u,  5u ); \
                                        }while (0u)

/*--- IC13_0 ---*/
#define SetPinFunc_IC13_0(dummy)        do{ \
                                            bFM_DAC1_DACR_DAE=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 29u, 3u,  1u ); \
                                            bFM_GPIO_PFR7_PB = 1u; \
                                        }while (0u)

/*--- IC13_LSYN3 ---*/
#define SetPinFunc_IC13_LSYN3(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 29u, 3u,  4u ); \
                                        }while (0u)

/*--- IC13_LSYN7 ---*/
#define SetPinFunc_IC13_LSYN7(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 29u, 3u,  5u ); \
                                        }while (0u)

/*--- IC20_0 ---*/
#define SetPinFunc_IC20_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 20u, 3u,  1u ); \
                                            bFM_GPIO_PFRA_P7 = 1u; \
                                        }while (0u)

/*--- IC20_LSYN0 ---*/
#define SetPinFunc_IC20_LSYN0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 20u, 3u,  4u ); \
                                        }while (0u)

/*--- IC20_LSYN4 ---*/
#define SetPinFunc_IC20_LSYN4(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 20u, 3u,  5u ); \
                                        }while (0u)

/*--- IC21_0 ---*/
#define SetPinFunc_IC21_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 23u, 3u,  1u ); \
                                            bFM_GPIO_PFRA_P8 = 1u; \
                                        }while (0u)

/*--- IC21_LSYN1 ---*/
#define SetPinFunc_IC21_LSYN1(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 23u, 3u,  4u ); \
                                        }while (0u)

/*--- IC21_LSYN5 ---*/
#define SetPinFunc_IC21_LSYN5(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 23u, 3u,  5u ); \
                                        }while (0u)

/*--- IC22_0 ---*/
#define SetPinFunc_IC22_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 26u, 3u,  1u ); \
                                            bFM_GPIO_PFRA_P9 = 1u; \
                                        }while (0u)

/*--- IC22_LSYN2 ---*/
#define SetPinFunc_IC22_LSYN2(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 26u, 3u,  4u ); \
                                        }while (0u)

/*--- IC22_LSYN6 ---*/
#define SetPinFunc_IC22_LSYN6(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 26u, 3u,  5u ); \
                                        }while (0u)

/*--- IC23_0 ---*/
#define SetPinFunc_IC23_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 29u, 3u,  1u ); \
                                            bFM_GPIO_PFRA_PA = 1u; \
                                        }while (0u)

/*--- IC23_1 ---*/
#define SetPinFunc_IC23_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 29u, 3u,  2u ); \
                                            bFM_GPIO_PFR6_PE = 1u; \
                                        }while (0u)

/*--- IC23_LSYN3 ---*/
#define SetPinFunc_IC23_LSYN3(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 29u, 3u,  4u ); \
                                        }while (0u)

/*--- IC23_LSYN7 ---*/
#define SetPinFunc_IC23_LSYN7(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 29u, 3u,  5u ); \
                                        }while (0u)

/*--- INT00_0 ---*/
#define SetPinFunc_INT00_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 0u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P0 = 1u; \
                                        }while (0u)

/*--- INT00_1 ---*/
#define SetPinFunc_INT00_1(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 0u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_P4 = 1u; \
                                        }while (0u)

/*--- INT01_0 ---*/
#define SetPinFunc_INT01_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 2u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P5 = 1u; \
                                        }while (0u)

/*--- INT01_1 ---*/
#define SetPinFunc_INT01_1(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 2u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_P5 = 1u; \
                                        }while (0u)

/*--- INT02_0 ---*/
#define SetPinFunc_INT02_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P8 = 1u; \
                                        }while (0u)

/*--- INT02_1 ---*/
#define SetPinFunc_INT02_1(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 4u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_P6 = 1u; \
                                        }while (0u)

/*--- INT03_0 ---*/
#define SetPinFunc_INT03_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 6u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_PB = 1u; \
                                        }while (0u)

/*--- INT03_1 ---*/
#define SetPinFunc_INT03_1(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 6u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_P7 = 1u; \
                                        }while (0u)

/*--- INT04_0 ---*/
#define SetPinFunc_INT04_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 8u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P3 = 1u; \
                                        }while (0u)

/*--- INT04_1 ---*/
#define SetPinFunc_INT04_1(dummy)       do{ \
                                            bFM_DAC0_DACR_DAE=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 8u, 2u,  2u ); \
                                            bFM_GPIO_PFR7_PC = 1u; \
                                        }while (0u)

/*--- INT05_0 ---*/
#define SetPinFunc_INT05_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 10u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_PD = 1u; \
                                        }while (0u)

/*--- INT06_0 ---*/
#define SetPinFunc_INT06_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 12u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_P0 = 1u; \
                                        }while (0u)

/*--- INT07_0 ---*/
#define SetPinFunc_INT07_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 14u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_P2 = 1u; \
                                        }while (0u)

/*--- INT08_0 ---*/
#define SetPinFunc_INT08_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN00=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 16u, 2u,  1u ); \
                                            bFM_GPIO_PFR1_P0 = 1u; \
                                        }while (0u)

/*--- INT09_0 ---*/
#define SetPinFunc_INT09_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN05=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 18u, 2u,  1u ); \
                                            bFM_GPIO_PFR1_P5 = 1u; \
                                        }while (0u)

/*--- INT10_0 ---*/
#define SetPinFunc_INT10_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN08=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 20u, 2u,  1u ); \
                                            bFM_GPIO_PFR1_P8 = 1u; \
                                        }while (0u)

/*--- INT11_0 ---*/
#define SetPinFunc_INT11_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN11=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 22u, 2u,  1u ); \
                                            bFM_GPIO_PFR1_PB = 1u; \
                                        }while (0u)

/*--- INT12_0 ---*/
#define SetPinFunc_INT12_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 24u, 2u,  1u ); \
                                            bFM_GPIO_PFRC_PF = 1u; \
                                        }while (0u)

/*--- INT13_0 ---*/
#define SetPinFunc_INT13_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 26u, 2u,  1u ); \
                                            bFM_GPIO_PFRC_P7 = 1u; \
                                        }while (0u)

/*--- INT14_0 ---*/
#define SetPinFunc_INT14_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 28u, 2u,  1u ); \
                                            bFM_GPIO_PFRC_PD = 1u; \
                                        }while (0u)

/*--- INT15_0 ---*/
#define SetPinFunc_INT15_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 30u, 2u,  1u ); \
                                            bFM_GPIO_PFRC_PE = 1u; \
                                        }while (0u)

/*--- INT16_0 ---*/
#define SetPinFunc_INT16_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 0u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_PF = 1u; \
                                        }while (0u)

/*--- INT16_1 ---*/
#define SetPinFunc_INT16_1(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 0u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_P9 = 1u; \
                                        }while (0u)

/*--- INT17_0 ---*/
#define SetPinFunc_INT17_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 2u, 2u,  1u ); \
                                            bFM_GPIO_PFR0_P8 = 1u; \
                                        }while (0u)

/*--- INT17_1 ---*/
#define SetPinFunc_INT17_1(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 2u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PA = 1u; \
                                        }while (0u)

/*--- INT18_0 ---*/
#define SetPinFunc_INT18_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFR0_P9 = 1u; \
                                        }while (0u)

/*--- INT18_1 ---*/
#define SetPinFunc_INT18_1(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 4u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PB = 1u; \
                                        }while (0u)

/*--- INT19_0 ---*/
#define SetPinFunc_INT19_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 6u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_P2 = 1u; \
                                        }while (0u)

/*--- INT19_1 ---*/
#define SetPinFunc_INT19_1(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 6u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PC = 1u; \
                                        }while (0u)

/*--- INT20_0 ---*/
#define SetPinFunc_INT20_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 8u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_P5 = 1u; \
                                        }while (0u)

/*--- INT21_0 ---*/
#define SetPinFunc_INT21_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 10u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_P8 = 1u; \
                                        }while (0u)

/*--- INT22_0 ---*/
#define SetPinFunc_INT22_0(dummy)       do{ \
                                            bFM_DAC1_DACR_DAE=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 12u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_PB = 1u; \
                                        }while (0u)

/*--- INT23_0 ---*/
#define SetPinFunc_INT23_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 14u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P0 = 1u; \
                                        }while (0u)

/*--- INT24_0 ---*/
#define SetPinFunc_INT24_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN27=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 16u, 2u,  1u ); \
                                            bFM_GPIO_PFR2_P7 = 1u; \
                                        }while (0u)

/*--- INT24_1 ---*/
#define SetPinFunc_INT24_1(dummy)       do{ \
                                            bFM_GPIO_ADE_AN09=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 16u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_P9 = 1u; \
                                        }while (0u)

/*--- INT25_0 ---*/
#define SetPinFunc_INT25_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN28=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 18u, 2u,  1u ); \
                                            bFM_GPIO_PFR2_P5 = 1u; \
                                        }while (0u)

/*--- INT25_1 ---*/
#define SetPinFunc_INT25_1(dummy)       do{ \
                                            bFM_GPIO_ADE_AN03=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 18u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_P3 = 1u; \
                                        }while (0u)

/*--- INT26_0 ---*/
#define SetPinFunc_INT26_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN31=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 20u, 2u,  1u ); \
                                            bFM_GPIO_PFR2_P2 = 1u; \
                                        }while (0u)

/*--- INT26_1 ---*/
#define SetPinFunc_INT26_1(dummy)       do{ \
                                            bFM_GPIO_ADE_AN14=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 20u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_PE = 1u; \
                                        }while (0u)

/*--- INT27_0 ---*/
#define SetPinFunc_INT27_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 22u, 2u,  1u ); \
                                            bFM_GPIO_PFR2_P1 = 1u; \
                                        }while (0u)

/*--- INT27_1 ---*/
#define SetPinFunc_INT27_1(dummy)       do{ \
                                            bFM_GPIO_ADE_AN15=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 22u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_PF = 1u; \
                                        }while (0u)

/*--- INT28_0 ---*/
#define SetPinFunc_INT28_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 24u, 2u,  1u ); \
                                            bFM_GPIO_PFRC_PB = 1u; \
                                        }while (0u)

/*--- INT29_0 ---*/
#define SetPinFunc_INT29_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 26u, 2u,  1u ); \
                                            bFM_GPIO_PFR6_PE = 1u; \
                                        }while (0u)

/*--- INT30_0 ---*/
#define SetPinFunc_INT30_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 28u, 2u,  1u ); \
                                            bFM_GPIO_PFR6_P3 = 1u; \
                                        }while (0u)

/*--- INT30_1 ---*/
#define SetPinFunc_INT30_1(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 28u, 2u,  2u ); \
                                            bFM_GPIO_PFRD_P0 = 1u; \
                                        }while (0u)

/*--- INT31_0 ---*/
#define SetPinFunc_INT31_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 30u, 2u,  1u ); \
                                            bFM_GPIO_PFR6_P0 = 1u; \
                                        }while (0u)

/*--- INT31_1 ---*/
#define SetPinFunc_INT31_1(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR15, 30u, 2u,  2u ); \
                                            bFM_GPIO_PFRD_P1 = 1u; \
                                        }while (0u)

/*--- MAD00_0 ---*/
#define SetPinFunc_MAD00_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 14u, 1u,  1u ); \
                                            bFM_GPIO_PFR7_P1 = 1u; \
                                        }while (0u)

/*--- MAD01_0 ---*/
#define SetPinFunc_MAD01_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 2u, 1u,  1u ); \
                                            bFM_GPIO_PFR7_P2 = 1u; \
                                        }while (0u)

/*--- MAD02_0 ---*/
#define SetPinFunc_MAD02_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 3u, 1u,  1u ); \
                                            bFM_GPIO_PFR7_P3 = 1u; \
                                        }while (0u)

/*--- MAD03_0 ---*/
#define SetPinFunc_MAD03_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 4u, 1u,  1u ); \
                                            bFM_GPIO_PFR7_P4 = 1u; \
                                        }while (0u)

/*--- MAD04_0 ---*/
#define SetPinFunc_MAD04_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 5u, 1u,  1u ); \
                                            bFM_GPIO_PFR7_P5 = 1u; \
                                        }while (0u)

/*--- MAD05_0 ---*/
#define SetPinFunc_MAD05_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 6u, 1u,  1u ); \
                                            bFM_GPIO_PFR7_P6 = 1u; \
                                        }while (0u)

/*--- MAD06_0 ---*/
#define SetPinFunc_MAD06_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 7u, 1u,  1u ); \
                                            bFM_GPIO_PFR7_P7 = 1u; \
                                        }while (0u)

/*--- MAD07_0 ---*/
#define SetPinFunc_MAD07_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 8u, 1u,  1u ); \
                                            bFM_GPIO_PFR7_P8 = 1u; \
                                        }while (0u)

/*--- MAD08_0 ---*/
#define SetPinFunc_MAD08_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 15u, 1u,  1u ); \
                                            bFM_GPIO_PFR7_P9 = 1u; \
                                        }while (0u)

/*--- MAD09_0 ---*/
#define SetPinFunc_MAD09_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 16u, 1u,  1u ); \
                                            bFM_GPIO_PFR7_PA = 1u; \
                                        }while (0u)

/*--- MAD10_0 ---*/
#define SetPinFunc_MAD10_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN14=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 17u, 1u,  1u ); \
                                            bFM_GPIO_PFR1_PE = 1u; \
                                        }while (0u)

/*--- MAD11_0 ---*/
#define SetPinFunc_MAD11_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN15=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 18u, 1u,  1u ); \
                                            bFM_GPIO_PFR1_PF = 1u; \
                                        }while (0u)

/*--- MAD12_0 ---*/
#define SetPinFunc_MAD12_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN24=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 19u, 1u,  1u ); \
                                            bFM_GPIO_PFR2_PA = 1u; \
                                        }while (0u)

/*--- MAD13_0 ---*/
#define SetPinFunc_MAD13_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN25=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 20u, 1u,  1u ); \
                                            bFM_GPIO_PFR2_P9 = 1u; \
                                        }while (0u)

/*--- MAD14_0 ---*/
#define SetPinFunc_MAD14_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN26=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 21u, 1u,  1u ); \
                                            bFM_GPIO_PFR2_P8 = 1u; \
                                        }while (0u)

/*--- MAD15_0 ---*/
#define SetPinFunc_MAD15_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN27=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 22u, 1u,  1u ); \
                                            bFM_GPIO_PFR2_P7 = 1u; \
                                        }while (0u)

/*--- MAD16_0 ---*/
#define SetPinFunc_MAD16_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 23u, 1u,  1u ); \
                                            bFM_GPIO_PFR2_P6 = 1u; \
                                        }while (0u)

/*--- MAD17_0 ---*/
#define SetPinFunc_MAD17_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN28=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 24u, 1u,  1u ); \
                                            bFM_GPIO_PFR2_P5 = 1u; \
                                        }while (0u)

/*--- MAD18_0 ---*/
#define SetPinFunc_MAD18_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN29=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 25u, 1u,  1u ); \
                                            bFM_GPIO_PFR2_P4 = 1u; \
                                        }while (0u)

/*--- MAD19_0 ---*/
#define SetPinFunc_MAD19_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 26u, 1u,  1u ); \
                                            bFM_GPIO_PFR3_PE = 1u; \
                                        }while (0u)

/*--- MAD20_0 ---*/
#define SetPinFunc_MAD20_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 27u, 1u,  1u ); \
                                            bFM_GPIO_PFR3_PD = 1u; \
                                        }while (0u)

/*--- MAD21_0 ---*/
#define SetPinFunc_MAD21_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 28u, 1u,  1u ); \
                                            bFM_GPIO_PFR3_PC = 1u; \
                                        }while (0u)

/*--- MAD22_0 ---*/
#define SetPinFunc_MAD22_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 29u, 1u,  1u ); \
                                            bFM_GPIO_PFR3_PB = 1u; \
                                        }while (0u)

/*--- MAD23_0 ---*/
#define SetPinFunc_MAD23_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 30u, 1u,  1u ); \
                                            bFM_GPIO_PFR3_PA = 1u; \
                                        }while (0u)

/*--- MAD24_0 ---*/
#define SetPinFunc_MAD24_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 31u, 1u,  1u ); \
                                            bFM_GPIO_PFR3_P9 = 1u; \
                                        }while (0u)

/*--- MADATA00_0 ---*/
#define SetPinFunc_MADATA00_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 9u, 1u,  1u ); \
                                            bFM_GPIO_PFRA_P0 = 1u; \
                                        }while (0u)

/*--- MADATA01_0 ---*/
#define SetPinFunc_MADATA01_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 10u, 1u,  1u ); \
                                            bFM_GPIO_PFRA_P1 = 1u; \
                                        }while (0u)

/*--- MADATA02_0 ---*/
#define SetPinFunc_MADATA02_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 11u, 1u,  1u ); \
                                            bFM_GPIO_PFRA_P2 = 1u; \
                                        }while (0u)

/*--- MADATA03_0 ---*/
#define SetPinFunc_MADATA03_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 12u, 1u,  1u ); \
                                            bFM_GPIO_PFRA_P3 = 1u; \
                                        }while (0u)

/*--- MADATA04_0 ---*/
#define SetPinFunc_MADATA04_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 13u, 1u,  1u ); \
                                            bFM_GPIO_PFRA_P4 = 1u; \
                                        }while (0u)

/*--- MADATA05_0 ---*/
#define SetPinFunc_MADATA05_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 14u, 1u,  1u ); \
                                            bFM_GPIO_PFRA_P5 = 1u; \
                                        }while (0u)

/*--- MADATA06_0 ---*/
#define SetPinFunc_MADATA06_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 15u, 1u,  1u ); \
                                            bFM_GPIO_PFRA_P6 = 1u; \
                                        }while (0u)

/*--- MADATA07_0 ---*/
#define SetPinFunc_MADATA07_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 16u, 1u,  1u ); \
                                            bFM_GPIO_PFRA_P7 = 1u; \
                                        }while (0u)

/*--- MADATA08_0 ---*/
#define SetPinFunc_MADATA08_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 17u, 1u,  1u ); \
                                            bFM_GPIO_PFRA_P8 = 1u; \
                                        }while (0u)

/*--- MADATA09_0 ---*/
#define SetPinFunc_MADATA09_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 18u, 1u,  1u ); \
                                            bFM_GPIO_PFRA_P9 = 1u; \
                                        }while (0u)

/*--- MADATA10_0 ---*/
#define SetPinFunc_MADATA10_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 19u, 1u,  1u ); \
                                            bFM_GPIO_PFRA_PA = 1u; \
                                        }while (0u)

/*--- MADATA11_0 ---*/
#define SetPinFunc_MADATA11_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 20u, 1u,  1u ); \
                                            bFM_GPIO_PFRA_PB = 1u; \
                                        }while (0u)

/*--- MADATA12_0 ---*/
#define SetPinFunc_MADATA12_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 21u, 1u,  1u ); \
                                            bFM_GPIO_PFRA_PC = 1u; \
                                        }while (0u)

/*--- MADATA13_0 ---*/
#define SetPinFunc_MADATA13_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 22u, 1u,  1u ); \
                                            bFM_GPIO_PFRA_PD = 1u; \
                                        }while (0u)

/*--- MADATA14_0 ---*/
#define SetPinFunc_MADATA14_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 23u, 1u,  1u ); \
                                            bFM_GPIO_PFRA_PE = 1u; \
                                        }while (0u)

/*--- MADATA15_0 ---*/
#define SetPinFunc_MADATA15_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 24u, 1u,  1u ); \
                                            bFM_GPIO_PFRA_PF = 1u; \
                                        }while (0u)

/*--- MALE_0 ---*/
#define SetPinFunc_MALE_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 0u, 1u,  1u ); \
                                            bFM_GPIO_PFR6_P1 = 1u; \
                                        }while (0u)

/*--- MCLKOUT_0 ---*/
#define SetPinFunc_MCLKOUT_0(dummy)     do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 2u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_PA = 1u; \
                                        }while (0u)

/*--- MCSX0_0 ---*/
#define SetPinFunc_MCSX0_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 1u, 1u,  1u ); \
                                            bFM_GPIO_PFR7_PE = 1u; \
                                        }while (0u)

/*--- MCSX1_0 ---*/
#define SetPinFunc_MCSX1_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 7u, 1u,  1u ); \
                                            bFM_GPIO_PFR7_PD = 1u; \
                                        }while (0u)

/*--- MCSX2_0 ---*/
#define SetPinFunc_MCSX2_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 8u, 1u,  1u ); \
                                            bFM_GPIO_PFR4_P5 = 1u; \
                                        }while (0u)

/*--- MCSX3_0 ---*/
#define SetPinFunc_MCSX3_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 9u, 1u,  1u ); \
                                            bFM_GPIO_PFR4_P4 = 1u; \
                                        }while (0u)

/*--- MCSX4_0 ---*/
#define SetPinFunc_MCSX4_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 10u, 1u,  1u ); \
                                            bFM_GPIO_PFR4_P3 = 1u; \
                                        }while (0u)

/*--- MCSX5_0 ---*/
#define SetPinFunc_MCSX5_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 11u, 1u,  1u ); \
                                            bFM_GPIO_PFR4_P2 = 1u; \
                                        }while (0u)

/*--- MCSX6_0 ---*/
#define SetPinFunc_MCSX6_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 12u, 1u,  1u ); \
                                            bFM_GPIO_PFR4_P1 = 1u; \
                                        }while (0u)

/*--- MCSX7_0 ---*/
#define SetPinFunc_MCSX7_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 13u, 1u,  1u ); \
                                            bFM_GPIO_PFR4_P0 = 1u; \
                                        }while (0u)

/*--- MDQM0_0 ---*/
#define SetPinFunc_MDQM0_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 4u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_P8 = 1u; \
                                        }while (0u)

/*--- MDQM1_0 ---*/
#define SetPinFunc_MDQM1_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 4u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_P9 = 1u; \
                                        }while (0u)

/*--- MNALE_0 ---*/
#define SetPinFunc_MNALE_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 6u, 1u,  1u ); \
                                            bFM_GPIO_PFR3_PB = 1u; \
                                        }while (0u)

/*--- MNCLE_0 ---*/
#define SetPinFunc_MNCLE_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 6u, 1u,  1u ); \
                                            bFM_GPIO_PFR3_PC = 1u; \
                                        }while (0u)

/*--- MNREX_0 ---*/
#define SetPinFunc_MNREX_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 6u, 1u,  1u ); \
                                            bFM_GPIO_PFR3_PE = 1u; \
                                        }while (0u)

/*--- MNWEX_0 ---*/
#define SetPinFunc_MNWEX_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 6u, 1u,  1u ); \
                                            bFM_GPIO_PFR3_PD = 1u; \
                                        }while (0u)

/*--- MOEX_0 ---*/
#define SetPinFunc_MOEX_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 5u, 1u,  1u ); \
                                            bFM_GPIO_PFR6_P3 = 1u; \
                                        }while (0u)

/*--- MRDY_0 ---*/
#define SetPinFunc_MRDY_0(dummy)        do{ \
                                            bFM_GPIO_PFR7_P0 = 1u; \
                                        }while (0u)

/*--- MWEX_0 ---*/
#define SetPinFunc_MWEX_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 3u, 1u,  1u ); \
                                            bFM_GPIO_PFR6_P2 = 1u; \
                                        }while (0u)

/*--- NMIX ---*/
#define SetPinFunc_NMIX(dummy)          do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 0u, 1u,  1u ); \
                                            bFM_GPIO_PFR2_P0 = 1u; \
                                        }while (0u)

/*--- RTCCO_0 ---*/
#define SetPinFunc_RTCCO_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFR6_P1 = 1u; \
                                        }while (0u)

/*--- RTO00_0 ---*/
#define SetPinFunc_RTO00_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 0u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_P9 = 1u; \
                                        }while (0u)

/*--- RTO01_0 ---*/
#define SetPinFunc_RTO01_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 2u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_PA = 1u; \
                                        }while (0u)

/*--- RTO02_0 ---*/
#define SetPinFunc_RTO02_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_PB = 1u; \
                                        }while (0u)

/*--- RTO03_0 ---*/
#define SetPinFunc_RTO03_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 6u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_PC = 1u; \
                                        }while (0u)

/*--- RTO04_0 ---*/
#define SetPinFunc_RTO04_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 8u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_PD = 1u; \
                                        }while (0u)

/*--- RTO05_0 ---*/
#define SetPinFunc_RTO05_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 10u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_PE = 1u; \
                                        }while (0u)

/*--- RTO10_0 ---*/
#define SetPinFunc_RTO10_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 0u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P0 = 1u; \
                                        }while (0u)

/*--- RTO11_0 ---*/
#define SetPinFunc_RTO11_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 2u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P1 = 1u; \
                                        }while (0u)

/*--- RTO12_0 ---*/
#define SetPinFunc_RTO12_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P2 = 1u; \
                                        }while (0u)

/*--- RTO13_0 ---*/
#define SetPinFunc_RTO13_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 6u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P3 = 1u; \
                                        }while (0u)

/*--- RTO14_0 ---*/
#define SetPinFunc_RTO14_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 8u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P4 = 1u; \
                                        }while (0u)

/*--- RTO15_0 ---*/
#define SetPinFunc_RTO15_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 10u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P5 = 1u; \
                                        }while (0u)

/*--- RTO20_0 ---*/
#define SetPinFunc_RTO20_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 0u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P0 = 1u; \
                                        }while (0u)

/*--- RTO21_0 ---*/
#define SetPinFunc_RTO21_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 2u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P1 = 1u; \
                                        }while (0u)

/*--- RTO22_0 ---*/
#define SetPinFunc_RTO22_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P2 = 1u; \
                                        }while (0u)

/*--- RTO23_0 ---*/
#define SetPinFunc_RTO23_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 6u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P3 = 1u; \
                                        }while (0u)

/*--- RTO24_0 ---*/
#define SetPinFunc_RTO24_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 8u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P4 = 1u; \
                                        }while (0u)

/*--- RTO25_0 ---*/
#define SetPinFunc_RTO25_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 10u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P5 = 1u; \
                                        }while (0u)

/*--- RTS4_0 ---*/
#define SetPinFunc_RTS4_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 0u, 2u,  1u ); \
                                            bFM_GPIO_PFR6_P3 = 1u; \
                                        }while (0u)

/*--- RTS4_1 ---*/
#define SetPinFunc_RTS4_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 0u, 2u,  2u ); \
                                            bFM_GPIO_PFRC_PF = 1u; \
                                        }while (0u)

/*--- RTS5_0 ---*/
#define SetPinFunc_RTS5_0(dummy)        do{ \
                                            bFM_GPIO_ADE_AN15=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 28u, 2u,  1u ); \
                                            bFM_GPIO_PFR1_PF = 1u; \
                                        }while (0u)

/*--- RX0_0 ---*/
#define SetPinFunc_RX0_0(dummy)         do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 24u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_PB = 1u; \
                                        }while (0u)

/*--- RX1_0 ---*/
#define SetPinFunc_RX1_0(dummy)         do{ \
                                            bFM_GPIO_ADE_AN28=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 28u, 2u,  1u ); \
                                            bFM_GPIO_PFR2_P5 = 1u; \
                                        }while (0u)

/*--- RX1_1 ---*/
#define SetPinFunc_RX1_1(dummy)         do{ \
                                            bFM_GPIO_ADE_AN03=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 28u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_P3 = 1u; \
                                        }while (0u)

/*--- RX2_0 ---*/
#define SetPinFunc_RX2_0(dummy)         do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR25, 0u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_PD = 1u; \
                                        }while (0u)

/*--- SCK0_0 ---*/
#define SetPinFunc_SCK0_0(dummy)        do{ \
                                            bFM_GPIO_ADE_AN30=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 8u, 2u,  1u ); \
                                            bFM_GPIO_PFR2_P3 = 1u; \
                                        }while (0u)

/*--- SCK10_0 ---*/
#define SetPinFunc_SCK10_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN02=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR16, 20u, 2u,  1u ); \
                                            bFM_GPIO_PFR1_P2 = 1u; \
                                        }while (0u)

/*--- SCK11_0 ---*/
#define SetPinFunc_SCK11_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN07=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR16, 26u, 2u,  1u ); \
                                            bFM_GPIO_PFR1_P7 = 1u; \
                                        }while (0u)

/*--- SCK12_0 ---*/
#define SetPinFunc_SCK12_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN13=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR17, 8u, 2u,  1u ); \
                                            bFM_GPIO_PFR1_PD = 1u; \
                                        }while (0u)

/*--- SCK13_0 ---*/
#define SetPinFunc_SCK13_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR17, 14u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_PE = 1u; \
                                        }while (0u)

/*--- SCK14_0 ---*/
#define SetPinFunc_SCK14_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR17, 20u, 2u,  1u ); \
                                            bFM_GPIO_PFR0_PA = 1u; \
                                        }while (0u)

/*--- SCK15_0 ---*/
#define SetPinFunc_SCK15_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR17, 26u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P5 = 1u; \
                                        }while (0u)

/*--- SCK1_0 ---*/
#define SetPinFunc_SCK1_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 14u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P7 = 1u; \
                                        }while (0u)

/*--- SCK1_1 ---*/
#define SetPinFunc_SCK1_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 14u, 2u,  2u ); \
                                            bFM_GPIO_PFR7_PD = 1u; \
                                        }while (0u)

/*--- SCK2_0 ---*/
#define SetPinFunc_SCK2_0(dummy)        do{ \
                                            bFM_GPIO_ADE_AN10=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 20u, 2u,  1u ); \
                                            bFM_GPIO_PFR1_PA = 1u; \
                                        }while (0u)

/*--- SCK2_1 ---*/
#define SetPinFunc_SCK2_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 20u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PB = 1u; \
                                        }while (0u)

/*--- SCK3_0 ---*/
#define SetPinFunc_SCK3_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 26u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_PD = 1u; \
                                        }while (0u)

/*--- SCK3_1 ---*/
#define SetPinFunc_SCK3_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 26u, 2u,  2u ); \
                                            bFM_GPIO_PFR4_P2 = 1u; \
                                        }while (0u)

/*--- SCK4_0 ---*/
#define SetPinFunc_SCK4_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 8u, 2u,  1u ); \
                                            bFM_GPIO_PFR6_P2 = 1u; \
                                        }while (0u)

/*--- SCK4_1 ---*/
#define SetPinFunc_SCK4_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 8u, 2u,  2u ); \
                                            bFM_GPIO_PFR6_PE = 1u; \
                                        }while (0u)

/*--- SCK5_0 ---*/
#define SetPinFunc_SCK5_0(dummy)        do{ \
                                            bFM_GPIO_ADE_AN25=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 14u, 2u,  1u ); \
                                            bFM_GPIO_PFR2_P9 = 1u; \
                                        }while (0u)

/*--- SCK6_0 ---*/
#define SetPinFunc_SCK6_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 20u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_PA = 1u; \
                                        }while (0u)

/*--- SCK7_0 ---*/
#define SetPinFunc_SCK7_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 26u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_PA = 1u; \
                                        }while (0u)

/*--- SCK8_0 ---*/
#define SetPinFunc_SCK8_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR16, 8u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_P7 = 1u; \
                                        }while (0u)

/*--- SCK9_0 ---*/
#define SetPinFunc_SCK9_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR16, 14u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_P4 = 1u; \
                                        }while (0u)

/*--- SCS60_0 ---*/
#define SetPinFunc_SCS60_0(dummy)       do{ \
                                            bFM_DAC1_DACR_DAE=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR23, 0u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_PB = 1u; \
                                        }while (0u)

/*--- SCS61_0 ---*/
#define SetPinFunc_SCS61_0(dummy)       do{ \
                                            bFM_DAC0_DACR_DAE=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR23, 2u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_PC = 1u; \
                                        }while (0u)

/*--- SCS70_0 ---*/
#define SetPinFunc_SCS70_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR23, 8u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_PB = 1u; \
                                        }while (0u)

/*--- SCS71_0 ---*/
#define SetPinFunc_SCS71_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR23, 10u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_PC = 1u; \
                                        }while (0u)

/*--- SIN0_0 ---*/
#define SetPinFunc_SIN0_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFR2_P1 = 1u; \
                                        }while (0u)

/*--- SIN10_0 ---*/
#define SetPinFunc_SIN10_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN00=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR16, 16u, 2u,  1u ); \
                                            bFM_GPIO_PFR1_P0 = 1u; \
                                        }while (0u)

/*--- SIN11_0 ---*/
#define SetPinFunc_SIN11_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN05=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR16, 22u, 2u,  1u ); \
                                            bFM_GPIO_PFR1_P5 = 1u; \
                                        }while (0u)

/*--- SIN12_0 ---*/
#define SetPinFunc_SIN12_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN11=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR17, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFR1_PB = 1u; \
                                        }while (0u)

/*--- SIN13_0 ---*/
#define SetPinFunc_SIN13_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR17, 10u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_PC = 1u; \
                                        }while (0u)

/*--- SIN14_0 ---*/
#define SetPinFunc_SIN14_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR17, 16u, 2u,  1u ); \
                                            bFM_GPIO_PFR0_P8 = 1u; \
                                        }while (0u)

/*--- SIN15_0 ---*/
#define SetPinFunc_SIN15_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR17, 22u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P3 = 1u; \
                                        }while (0u)

/*--- SIN1_0 ---*/
#define SetPinFunc_SIN1_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 10u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P5 = 1u; \
                                        }while (0u)

/*--- SIN1_1 ---*/
#define SetPinFunc_SIN1_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 10u, 2u,  2u ); \
                                            bFM_GPIO_PFR7_P0 = 1u; \
                                        }while (0u)

/*--- SIN2_0 ---*/
#define SetPinFunc_SIN2_0(dummy)        do{ \
                                            bFM_GPIO_ADE_AN08=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 16u, 2u,  1u ); \
                                            bFM_GPIO_PFR1_P8 = 1u; \
                                        }while (0u)

/*--- SIN2_1 ---*/
#define SetPinFunc_SIN2_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 16u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_P9 = 1u; \
                                        }while (0u)

/*--- SIN3_0 ---*/
#define SetPinFunc_SIN3_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 22u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_PF = 1u; \
                                        }while (0u)

/*--- SIN3_1 ---*/
#define SetPinFunc_SIN3_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 22u, 2u,  2u ); \
                                            bFM_GPIO_PFR4_P0 = 1u; \
                                        }while (0u)

/*--- SIN4_0 ---*/
#define SetPinFunc_SIN4_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFR6_P0 = 1u; \
                                        }while (0u)

/*--- SIN4_1 ---*/
#define SetPinFunc_SIN4_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 4u, 2u,  2u ); \
                                            bFM_GPIO_PFRC_PE = 1u; \
                                        }while (0u)

/*--- SIN5_0 ---*/
#define SetPinFunc_SIN5_0(dummy)        do{ \
                                            bFM_GPIO_ADE_AN27=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 10u, 2u,  1u ); \
                                            bFM_GPIO_PFR2_P7 = 1u; \
                                        }while (0u)

/*--- SIN6_0 ---*/
#define SetPinFunc_SIN6_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 16u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_P8 = 1u; \
                                        }while (0u)

/*--- SIN6_1 ---*/
#define SetPinFunc_SIN6_1(dummy)        do{ \
                                            bFM_GPIO_ADE_AN03=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 16u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_P3 = 1u; \
                                        }while (0u)

/*--- SIN7_0 ---*/
#define SetPinFunc_SIN7_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 22u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P8 = 1u; \
                                        }while (0u)

/*--- SIN8_0 ---*/
#define SetPinFunc_SIN8_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR16, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_P5 = 1u; \
                                        }while (0u)

/*--- SIN9_0 ---*/
#define SetPinFunc_SIN9_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR16, 10u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_P2 = 1u; \
                                        }while (0u)

/*--- SOT0_0 ---*/
#define SetPinFunc_SOT0_0(dummy)        do{ \
                                            bFM_GPIO_ADE_AN31=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 6u, 2u,  1u ); \
                                            bFM_GPIO_PFR2_P2 = 1u; \
                                        }while (0u)

/*--- SOT10_0 ---*/
#define SetPinFunc_SOT10_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN01=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR16, 18u, 2u,  1u ); \
                                            bFM_GPIO_PFR1_P1 = 1u; \
                                        }while (0u)

/*--- SOT11_0 ---*/
#define SetPinFunc_SOT11_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN06=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR16, 24u, 2u,  1u ); \
                                            bFM_GPIO_PFR1_P6 = 1u; \
                                        }while (0u)

/*--- SOT12_0 ---*/
#define SetPinFunc_SOT12_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN12=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR17, 6u, 2u,  1u ); \
                                            bFM_GPIO_PFR1_PC = 1u; \
                                        }while (0u)

/*--- SOT13_0 ---*/
#define SetPinFunc_SOT13_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR17, 12u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_PD = 1u; \
                                        }while (0u)

/*--- SOT14_0 ---*/
#define SetPinFunc_SOT14_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR17, 18u, 2u,  1u ); \
                                            bFM_GPIO_PFR0_P9 = 1u; \
                                        }while (0u)

/*--- SOT15_0 ---*/
#define SetPinFunc_SOT15_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR17, 24u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P4 = 1u; \
                                        }while (0u)

/*--- SOT1_0 ---*/
#define SetPinFunc_SOT1_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 12u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P6 = 1u; \
                                        }while (0u)

/*--- SOT1_1 ---*/
#define SetPinFunc_SOT1_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 12u, 2u,  2u ); \
                                            bFM_GPIO_PFR7_P1 = 1u; \
                                        }while (0u)

/*--- SOT2_0 ---*/
#define SetPinFunc_SOT2_0(dummy)        do{ \
                                            bFM_GPIO_ADE_AN09=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 18u, 2u,  1u ); \
                                            bFM_GPIO_PFR1_P9 = 1u; \
                                        }while (0u)

/*--- SOT2_1 ---*/
#define SetPinFunc_SOT2_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 18u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PA = 1u; \
                                        }while (0u)

/*--- SOT3_0 ---*/
#define SetPinFunc_SOT3_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 24u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_PE = 1u; \
                                        }while (0u)

/*--- SOT3_1 ---*/
#define SetPinFunc_SOT3_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 24u, 2u,  2u ); \
                                            bFM_GPIO_PFR4_P1 = 1u; \
                                        }while (0u)

/*--- SOT4_0 ---*/
#define SetPinFunc_SOT4_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 6u, 2u,  1u ); \
                                            bFM_GPIO_PFR6_P1 = 1u; \
                                        }while (0u)

/*--- SOT4_1 ---*/
#define SetPinFunc_SOT4_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 6u, 2u,  2u ); \
                                            bFM_GPIO_PFRC_PD = 1u; \
                                        }while (0u)

/*--- SOT5_0 ---*/
#define SetPinFunc_SOT5_0(dummy)        do{ \
                                            bFM_GPIO_ADE_AN26=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 12u, 2u,  1u ); \
                                            bFM_GPIO_PFR2_P8 = 1u; \
                                        }while (0u)

/*--- SOT6_0 ---*/
#define SetPinFunc_SOT6_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 18u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_P9 = 1u; \
                                        }while (0u)

/*--- SOT6_1 ---*/
#define SetPinFunc_SOT6_1(dummy)        do{ \
                                            bFM_GPIO_ADE_AN04=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 18u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_P4 = 1u; \
                                        }while (0u)

/*--- SOT7_0 ---*/
#define SetPinFunc_SOT7_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 24u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P9 = 1u; \
                                        }while (0u)

/*--- SOT8_0 ---*/
#define SetPinFunc_SOT8_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR16, 6u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_P6 = 1u; \
                                        }while (0u)

/*--- SOT9_0 ---*/
#define SetPinFunc_SOT9_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR16, 12u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_P3 = 1u; \
                                        }while (0u)

/*--- SUBOUT_0 ---*/
#define SetPinFunc_SUBOUT_0(dummy)      do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 6u, 2u,  1u ); \
                                            bFM_GPIO_PFR6_P1 = 1u; \
                                        }while (0u)

/*--- SWCLK ---*/
#define SetPinFunc_SWCLK(dummy)         do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 16u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_P1 = 1u; \
                                        }while (0u)

/*--- SWDIO ---*/
#define SetPinFunc_SWDIO(dummy)         do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 16u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_P3 = 1u; \
                                        }while (0u)

/*--- SWO ---*/
#define SetPinFunc_SWO(dummy)           do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 16u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_P4 = 1u; \
                                        }while (0u)

/*--- S_CD_0 ---*/
#define SetPinFunc_S_CD_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR18, 26u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_P9 = 1u; \
                                        }while (0u)

/*--- S_CLK_0 ---*/
#define SetPinFunc_S_CLK_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR18, 14u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_P4 = 1u; \
                                        }while (0u)

/*--- S_CMD_0 ---*/
#define SetPinFunc_S_CMD_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR18, 16u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_P5 = 1u; \
                                        }while (0u)

/*--- S_DATA0_0 ---*/
#define SetPinFunc_S_DATA0_0(dummy)     do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR18, 18u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_P3 = 1u; \
                                        }while (0u)

/*--- S_DATA1_0 ---*/
#define SetPinFunc_S_DATA1_0(dummy)     do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR18, 20u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_P2 = 1u; \
                                        }while (0u)

/*--- S_DATA2_0 ---*/
#define SetPinFunc_S_DATA2_0(dummy)     do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR18, 22u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_P7 = 1u; \
                                        }while (0u)

/*--- S_DATA3_0 ---*/
#define SetPinFunc_S_DATA3_0(dummy)     do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR18, 24u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_P6 = 1u; \
                                        }while (0u)

/*--- S_WP_0 ---*/
#define SetPinFunc_S_WP_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR18, 28u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_P8 = 1u; \
                                        }while (0u)

/*--- TCK ---*/
#define SetPinFunc_TCK(dummy)           do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 16u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_P1 = 1u; \
                                        }while (0u)

/*--- TDI ---*/
#define SetPinFunc_TDI(dummy)           do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 17u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_P2 = 1u; \
                                        }while (0u)

/*--- TDO ---*/
#define SetPinFunc_TDO(dummy)           do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 16u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_P4 = 1u; \
                                        }while (0u)

/*--- TIOA0_0_OUT ---*/
#define SetPinFunc_TIOA0_0_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 2u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P0 = 1u; \
                                        }while (0u)

/*--- TIOA0_1_OUT ---*/
#define SetPinFunc_TIOA0_1_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 2u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_P9 = 1u; \
                                        }while (0u)

/*--- TIOA0_2_OUT ---*/
#define SetPinFunc_TIOA0_2_OUT(dummy)   do{ \
                                            bFM_GPIO_ADE_AN00=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 2u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_P0 = 1u; \
                                        }while (0u)

/*--- TIOA10_0_OUT ---*/
#define SetPinFunc_TIOA10_0_OUT(dummy)  do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR12, 18u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P2 = 1u; \
                                        }while (0u)

/*--- TIOA11_0_IN ---*/
#define SetPinFunc_TIOA11_0_IN(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR12, 24u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P3 = 1u; \
                                        }while (0u)

/*--- TIOA11_0_OUT ---*/
#define SetPinFunc_TIOA11_0_OUT(dummy)  do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR12, 26u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P3 = 1u; \
                                        }while (0u)

/*--- TIOA12_0_OUT ---*/
#define SetPinFunc_TIOA12_0_OUT(dummy)  do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR13, 2u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P4 = 1u; \
                                        }while (0u)

/*--- TIOA13_0_IN ---*/
#define SetPinFunc_TIOA13_0_IN(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR13, 8u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P5 = 1u; \
                                        }while (0u)

/*--- TIOA13_0_OUT ---*/
#define SetPinFunc_TIOA13_0_OUT(dummy)  do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR13, 10u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P5 = 1u; \
                                        }while (0u)

/*--- TIOA13_1_IN ---*/
#define SetPinFunc_TIOA13_1_IN(dummy)   do{ \
                                            bFM_GPIO_ADE_AN29=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR13, 8u, 2u,  2u ); \
                                            bFM_GPIO_PFR2_P4 = 1u; \
                                        }while (0u)

/*--- TIOA13_1_OUT ---*/
#define SetPinFunc_TIOA13_1_OUT(dummy)  do{ \
                                            bFM_GPIO_ADE_AN29=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR13, 10u, 2u,  2u ); \
                                            bFM_GPIO_PFR2_P4 = 1u; \
                                        }while (0u)

/*--- TIOA14_0_OUT ---*/
#define SetPinFunc_TIOA14_0_OUT(dummy)  do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR13, 18u, 2u,  1u ); \
                                            bFM_GPIO_PFRC_P6 = 1u; \
                                        }while (0u)

/*--- TIOA15_0_IN ---*/
#define SetPinFunc_TIOA15_0_IN(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR13, 24u, 2u,  1u ); \
                                            bFM_GPIO_PFRC_PA = 1u; \
                                        }while (0u)

/*--- TIOA15_0_OUT ---*/
#define SetPinFunc_TIOA15_0_OUT(dummy)  do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR13, 26u, 2u,  1u ); \
                                            bFM_GPIO_PFRC_PA = 1u; \
                                        }while (0u)

/*--- TIOA1_0_IN ---*/
#define SetPinFunc_TIOA1_0_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 8u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P1 = 1u; \
                                        }while (0u)

/*--- TIOA1_0_OUT ---*/
#define SetPinFunc_TIOA1_0_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 10u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P1 = 1u; \
                                        }while (0u)

/*--- TIOA1_1_IN ---*/
#define SetPinFunc_TIOA1_1_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 8u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PA = 1u; \
                                        }while (0u)

/*--- TIOA1_1_OUT ---*/
#define SetPinFunc_TIOA1_1_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 10u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PA = 1u; \
                                        }while (0u)

/*--- TIOA1_2_IN ---*/
#define SetPinFunc_TIOA1_2_IN(dummy)    do{ \
                                            bFM_GPIO_ADE_AN02=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 8u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_P2 = 1u; \
                                        }while (0u)

/*--- TIOA1_2_OUT ---*/
#define SetPinFunc_TIOA1_2_OUT(dummy)   do{ \
                                            bFM_GPIO_ADE_AN02=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 10u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_P2 = 1u; \
                                        }while (0u)

/*--- TIOA2_0_OUT ---*/
#define SetPinFunc_TIOA2_0_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 18u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P2 = 1u; \
                                        }while (0u)

/*--- TIOA2_1_OUT ---*/
#define SetPinFunc_TIOA2_1_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 18u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PB = 1u; \
                                        }while (0u)

/*--- TIOA2_2_OUT ---*/
#define SetPinFunc_TIOA2_2_OUT(dummy)   do{ \
                                            bFM_GPIO_ADE_AN06=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 18u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_P6 = 1u; \
                                        }while (0u)

/*--- TIOA3_0_IN ---*/
#define SetPinFunc_TIOA3_0_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 24u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P3 = 1u; \
                                        }while (0u)

/*--- TIOA3_0_OUT ---*/
#define SetPinFunc_TIOA3_0_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 26u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P3 = 1u; \
                                        }while (0u)

/*--- TIOA3_1_IN ---*/
#define SetPinFunc_TIOA3_1_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 24u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PC = 1u; \
                                        }while (0u)

/*--- TIOA3_1_OUT ---*/
#define SetPinFunc_TIOA3_1_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 26u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PC = 1u; \
                                        }while (0u)

/*--- TIOA3_2_IN ---*/
#define SetPinFunc_TIOA3_2_IN(dummy)    do{ \
                                            bFM_GPIO_ADE_AN08=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 24u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_P8 = 1u; \
                                        }while (0u)

/*--- TIOA3_2_OUT ---*/
#define SetPinFunc_TIOA3_2_OUT(dummy)   do{ \
                                            bFM_GPIO_ADE_AN08=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 26u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_P8 = 1u; \
                                        }while (0u)

/*--- TIOA4_0_OUT ---*/
#define SetPinFunc_TIOA4_0_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 2u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P4 = 1u; \
                                        }while (0u)

/*--- TIOA4_1_OUT ---*/
#define SetPinFunc_TIOA4_1_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 2u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PD = 1u; \
                                        }while (0u)

/*--- TIOA4_2_OUT ---*/
#define SetPinFunc_TIOA4_2_OUT(dummy)   do{ \
                                            bFM_GPIO_ADE_AN10=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 2u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_PA = 1u; \
                                        }while (0u)

/*--- TIOA5_0_IN ---*/
#define SetPinFunc_TIOA5_0_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 8u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P5 = 1u; \
                                        }while (0u)

/*--- TIOA5_0_OUT ---*/
#define SetPinFunc_TIOA5_0_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 10u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P5 = 1u; \
                                        }while (0u)

/*--- TIOA5_1_IN ---*/
#define SetPinFunc_TIOA5_1_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 8u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PE = 1u; \
                                        }while (0u)

/*--- TIOA5_1_OUT ---*/
#define SetPinFunc_TIOA5_1_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 10u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PE = 1u; \
                                        }while (0u)

/*--- TIOA5_2_IN ---*/
#define SetPinFunc_TIOA5_2_IN(dummy)    do{ \
                                            bFM_GPIO_ADE_AN12=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 8u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_PC = 1u; \
                                        }while (0u)

/*--- TIOA5_2_OUT ---*/
#define SetPinFunc_TIOA5_2_OUT(dummy)   do{ \
                                            bFM_GPIO_ADE_AN12=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 10u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_PC = 1u; \
                                        }while (0u)

/*--- TIOA6_0_OUT ---*/
#define SetPinFunc_TIOA6_0_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 18u, 2u,  1u ); \
                                            bFM_GPIO_PFRC_P2 = 1u; \
                                        }while (0u)

/*--- TIOA7_0_IN ---*/
#define SetPinFunc_TIOA7_0_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 24u, 2u,  1u ); \
                                            bFM_GPIO_PFRC_P4 = 1u; \
                                        }while (0u)

/*--- TIOA7_0_OUT ---*/
#define SetPinFunc_TIOA7_0_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 26u, 2u,  1u ); \
                                            bFM_GPIO_PFRC_P4 = 1u; \
                                        }while (0u)

/*--- TIOA8_0_OUT ---*/
#define SetPinFunc_TIOA8_0_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR12, 2u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P0 = 1u; \
                                        }while (0u)

/*--- TIOA8_1_OUT ---*/
#define SetPinFunc_TIOA8_1_OUT(dummy)   do{ \
                                            bFM_GPIO_ADE_AN14=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR12, 2u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_PE = 1u; \
                                        }while (0u)

/*--- TIOA9_0_IN ---*/
#define SetPinFunc_TIOA9_0_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR12, 8u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P1 = 1u; \
                                        }while (0u)

/*--- TIOA9_0_OUT ---*/
#define SetPinFunc_TIOA9_0_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR12, 10u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P1 = 1u; \
                                        }while (0u)

/*--- TIOB0_0_IN ---*/
#define SetPinFunc_TIOB0_0_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 4u, 3u,  1u ); \
                                            bFM_GPIO_PFR7_P2 = 1u; \
                                        }while (0u)

/*--- TIOB0_2_IN ---*/
#define SetPinFunc_TIOB0_2_IN(dummy)    do{ \
                                            bFM_GPIO_ADE_AN01=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 4u, 3u,  3u ); \
                                            bFM_GPIO_PFR1_P1 = 1u; \
                                        }while (0u)

/*--- TIOB10_0_IN ---*/
#define SetPinFunc_TIOB10_0_IN(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR12, 20u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_PE = 1u; \
                                        }while (0u)

/*--- TIOB11_0_IN ---*/
#define SetPinFunc_TIOB11_0_IN(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR12, 28u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_PF = 1u; \
                                        }while (0u)

/*--- TIOB12_0_IN ---*/
#define SetPinFunc_TIOB12_0_IN(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR13, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFR0_P8 = 1u; \
                                        }while (0u)

/*--- TIOB13_0_IN ---*/
#define SetPinFunc_TIOB13_0_IN(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR13, 12u, 2u,  1u ); \
                                            bFM_GPIO_PFR0_P9 = 1u; \
                                        }while (0u)

/*--- TIOB13_1_IN ---*/
#define SetPinFunc_TIOB13_1_IN(dummy)   do{ \
                                            bFM_GPIO_ADE_AN30=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR13, 12u, 2u,  2u ); \
                                            bFM_GPIO_PFR2_P3 = 1u; \
                                        }while (0u)

/*--- TIOB14_0_IN ---*/
#define SetPinFunc_TIOB14_0_IN(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR13, 20u, 2u,  1u ); \
                                            bFM_GPIO_PFRC_P5 = 1u; \
                                        }while (0u)

/*--- TIOB15_0_IN ---*/
#define SetPinFunc_TIOB15_0_IN(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR13, 28u, 2u,  1u ); \
                                            bFM_GPIO_PFRC_P9 = 1u; \
                                        }while (0u)

/*--- TIOB1_0_IN ---*/
#define SetPinFunc_TIOB1_0_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 12u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_P3 = 1u; \
                                        }while (0u)

/*--- TIOB1_2_IN ---*/
#define SetPinFunc_TIOB1_2_IN(dummy)    do{ \
                                            bFM_GPIO_ADE_AN05=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 12u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_P5 = 1u; \
                                        }while (0u)

/*--- TIOB2_0_IN ---*/
#define SetPinFunc_TIOB2_0_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 20u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_P4 = 1u; \
                                        }while (0u)

/*--- TIOB2_2_IN ---*/
#define SetPinFunc_TIOB2_2_IN(dummy)    do{ \
                                            bFM_GPIO_ADE_AN07=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 20u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_P7 = 1u; \
                                        }while (0u)

/*--- TIOB3_0_IN ---*/
#define SetPinFunc_TIOB3_0_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 28u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_P5 = 1u; \
                                        }while (0u)

/*--- TIOB3_2_IN ---*/
#define SetPinFunc_TIOB3_2_IN(dummy)    do{ \
                                            bFM_GPIO_ADE_AN09=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 28u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_P9 = 1u; \
                                        }while (0u)

/*--- TIOB4_0_IN ---*/
#define SetPinFunc_TIOB4_0_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_P6 = 1u; \
                                        }while (0u)

/*--- TIOB4_2_IN ---*/
#define SetPinFunc_TIOB4_2_IN(dummy)    do{ \
                                            bFM_GPIO_ADE_AN11=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 4u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_PB = 1u; \
                                        }while (0u)

/*--- TIOB5_0_IN ---*/
#define SetPinFunc_TIOB5_0_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 12u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_P7 = 1u; \
                                        }while (0u)

/*--- TIOB5_2_IN ---*/
#define SetPinFunc_TIOB5_2_IN(dummy)    do{ \
                                            bFM_GPIO_ADE_AN13=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 12u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_PD = 1u; \
                                        }while (0u)

/*--- TIOB6_0_IN ---*/
#define SetPinFunc_TIOB6_0_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 20u, 2u,  1u ); \
                                            bFM_GPIO_PFRC_P1 = 1u; \
                                        }while (0u)

/*--- TIOB7_0_IN ---*/
#define SetPinFunc_TIOB7_0_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 28u, 2u,  1u ); \
                                            bFM_GPIO_PFRC_P3 = 1u; \
                                        }while (0u)

/*--- TIOB8_0_IN ---*/
#define SetPinFunc_TIOB8_0_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR12, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_PC = 1u; \
                                        }while (0u)

/*--- TIOB8_1_IN ---*/
#define SetPinFunc_TIOB8_1_IN(dummy)    do{ \
                                            bFM_GPIO_ADE_AN15=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR12, 4u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_PF = 1u; \
                                        }while (0u)

/*--- TIOB9_0_IN ---*/
#define SetPinFunc_TIOB9_0_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR12, 12u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_PD = 1u; \
                                        }while (0u)

/*--- TMS ---*/
#define SetPinFunc_TMS(dummy)           do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 16u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_P3 = 1u; \
                                        }while (0u)

/*--- TRACECLK ---*/
#define SetPinFunc_TRACECLK(dummy)      do{ \
                                            bFM_GPIO_ADE_AN09=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 24u, 1u,  1u ); \
                                            bFM_GPIO_PFR1_P9 = 1u; \
                                        }while (0u)

/*--- TRACED0 ---*/
#define SetPinFunc_TRACED0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN10=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 24u, 1u,  1u ); \
                                            bFM_GPIO_PFR1_PA = 1u; \
                                        }while (0u)

/*--- TRACED1 ---*/
#define SetPinFunc_TRACED1(dummy)       do{ \
                                            bFM_GPIO_ADE_AN11=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 24u, 1u,  1u ); \
                                            bFM_GPIO_PFR1_PB = 1u; \
                                        }while (0u)

/*--- TRACED2 ---*/
#define SetPinFunc_TRACED2(dummy)       do{ \
                                            bFM_GPIO_ADE_AN12=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 25u, 1u,  1u ); \
                                            bFM_GPIO_PFR1_PC = 1u; \
                                        }while (0u)

/*--- TRACED3 ---*/
#define SetPinFunc_TRACED3(dummy)       do{ \
                                            bFM_GPIO_ADE_AN13=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 25u, 1u,  1u ); \
                                            bFM_GPIO_PFR1_PD = 1u; \
                                        }while (0u)

/*--- TRSTX ---*/
#define SetPinFunc_TRSTX(dummy)         do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 17u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_P0 = 1u; \
                                        }while (0u)

/*--- TX0_0 ---*/
#define SetPinFunc_TX0_0(dummy)         do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 26u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_PC = 1u; \
                                        }while (0u)

/*--- TX1_0 ---*/
#define SetPinFunc_TX1_0(dummy)         do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 30u, 2u,  1u ); \
                                            bFM_GPIO_PFR2_P6 = 1u; \
                                        }while (0u)

/*--- TX1_1 ---*/
#define SetPinFunc_TX1_1(dummy)         do{ \
                                            bFM_GPIO_ADE_AN04=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 30u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_P4 = 1u; \
                                        }while (0u)

/*--- TX2_0 ---*/
#define SetPinFunc_TX2_0(dummy)         do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR25, 2u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_PE = 1u; \
                                        }while (0u)

/*--- UDM0 ---*/
#define SetPinFunc_UDM0(dummy)          do{ \
                                            bFM_GPIO_SPSR_USB0C=1u; \
                                        }while (0u)

/*--- UDM1 ---*/
#define SetPinFunc_UDM1(dummy)          do{ \
                                            bFM_GPIO_SPSR_USB1C=1u; \
                                        }while (0u)

/*--- UDP0 ---*/
#define SetPinFunc_UDP0(dummy)          do{ \
                                            bFM_GPIO_SPSR_USB0C=1u; \
                                        }while (0u)

/*--- UDP1 ---*/
#define SetPinFunc_UDP1(dummy)          do{ \
                                            bFM_GPIO_SPSR_USB1C=1u; \
                                        }while (0u)

/*--- UHCONX0 ---*/
#define SetPinFunc_UHCONX0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 9u, 1u,  1u ); \
                                            bFM_GPIO_PFR6_P1 = 1u; \
                                        }while (0u)

/*--- UHCONX1 ---*/
#define SetPinFunc_UHCONX1(dummy)       do{ \
                                            bFM_GPIO_ADE_AN30=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 13u, 1u,  1u ); \
                                            bFM_GPIO_PFR2_P3 = 1u; \
                                        }while (0u)

/*--- VREGCTL ---*/
#define SetPinFunc_VREGCTL(dummy)       do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBPFR_VPFR0=0u; \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while (0u)

/*--- VWAKEUP ---*/
#define SetPinFunc_VWAKEUP(dummy)       do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBPFR_VPFR1=0u; \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while (0u)

/*--- WKUP0 ---*/
#define SetPinFunc_WKUP0(dummy)         do{ \
                                            bFM_GPIO_PFR2_P0 = 1u; \
                                        }while (0u)

/*--- WKUP1 ---*/
#define SetPinFunc_WKUP1(dummy)         do{ \
                                            bFM_GPIO_PFRA_P8 = 1u; \
                                        }while (0u)

/*--- WKUP2 ---*/
#define SetPinFunc_WKUP2(dummy)         do{ \
                                            bFM_GPIO_PFR7_PD = 1u; \
                                        }while (0u)

/*--- WKUP3 ---*/
#define SetPinFunc_WKUP3(dummy)         do{ \
                                            bFM_GPIO_PFR6_P0 = 1u; \
                                        }while (0u)

/*--- X0 ---*/
#define SetPinFunc_X0(dummy)            do{ \
                                            PINCONFIG_SET_REG(FM_GPIO->SPSR, 2u, 2u, 1u); \
                                        }while (0u)

/*--- X0A ---*/
#define SetPinFunc_X0A(dummy)           do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            PINCONFIG_SET_REG(FM_RTC->VBPFR, 4u, 2u, 0x00u); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while (0u)

/*--- X1 ---*/
#define SetPinFunc_X1(dummy)            do{ \
                                            PINCONFIG_SET_REG(FM_GPIO->SPSR, 2u, 2u, 1u); \
                                        }while (0u)

/*--- X1A ---*/
#define SetPinFunc_X1A(dummy)           do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            PINCONFIG_SET_REG(FM_RTC->VBPFR, 4u, 2u, 0x00u); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while (0u)

/*--- ZIN0_0 ---*/
#define SetPinFunc_ZIN0_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P2 = 1u; \
                                        }while (0u)

/*--- ZIN0_2 ---*/
#define SetPinFunc_ZIN0_2(dummy)        do{ \
                                            bFM_GPIO_ADE_AN02=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 4u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_P2 = 1u; \
                                        }while (0u)

/*--- ZIN1_0 ---*/
#define SetPinFunc_ZIN1_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 10u, 2u,  1u ); \
                                            bFM_GPIO_PFR7_P7 = 1u; \
                                        }while (0u)

/*--- ZIN1_2 ---*/
#define SetPinFunc_ZIN1_2(dummy)        do{ \
                                            bFM_GPIO_ADE_AN07=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 10u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_P7 = 1u; \
                                        }while (0u)

/*--- ZIN2_0 ---*/
#define SetPinFunc_ZIN2_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR14, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_P2 = 1u; \
                                        }while (0u)

/*--- ZIN2_1 ---*/
#define SetPinFunc_ZIN2_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR14, 4u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_P3 = 1u; \
                                        }while (0u)

/*--- ZIN3_0 ---*/
#define SetPinFunc_ZIN3_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR18, 8u, 2u,  1u ); \
                                            bFM_GPIO_PFRA_PE = 1u; \
                                        }while (0u)

/*--- ZIN3_1 ---*/
#define SetPinFunc_ZIN3_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR18, 8u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PB = 1u; \
                                        }while (0u)

/******************************************************************************
   ANALOG PINS
*******************************************************************************/

/*--- AN00 ---*/
#define SetPinFunc_AN00(dummy)          do{ \
                                            bFM_GPIO_ADE_AN00=1u; \
                                        }while (0u)

/*--- AN01 ---*/
#define SetPinFunc_AN01(dummy)          do{ \
                                            bFM_GPIO_ADE_AN01=1u; \
                                        }while (0u)

/*--- AN02 ---*/
#define SetPinFunc_AN02(dummy)          do{ \
                                            bFM_GPIO_ADE_AN02=1u; \
                                        }while (0u)

/*--- AN03 ---*/
#define SetPinFunc_AN03(dummy)          do{ \
                                            bFM_GPIO_ADE_AN03=1u; \
                                        }while (0u)

/*--- AN04 ---*/
#define SetPinFunc_AN04(dummy)          do{ \
                                            bFM_GPIO_ADE_AN04=1u; \
                                        }while (0u)

/*--- AN05 ---*/
#define SetPinFunc_AN05(dummy)          do{ \
                                            bFM_GPIO_ADE_AN05=1u; \
                                        }while (0u)

/*--- AN06 ---*/
#define SetPinFunc_AN06(dummy)          do{ \
                                            bFM_GPIO_ADE_AN06=1u; \
                                        }while (0u)

/*--- AN07 ---*/
#define SetPinFunc_AN07(dummy)          do{ \
                                            bFM_GPIO_ADE_AN07=1u; \
                                        }while (0u)

/*--- AN08 ---*/
#define SetPinFunc_AN08(dummy)          do{ \
                                            bFM_GPIO_ADE_AN08=1u; \
                                        }while (0u)

/*--- AN09 ---*/
#define SetPinFunc_AN09(dummy)          do{ \
                                            bFM_GPIO_ADE_AN09=1u; \
                                        }while (0u)

/*--- AN10 ---*/
#define SetPinFunc_AN10(dummy)          do{ \
                                            bFM_GPIO_ADE_AN10=1u; \
                                        }while (0u)

/*--- AN11 ---*/
#define SetPinFunc_AN11(dummy)          do{ \
                                            bFM_GPIO_ADE_AN11=1u; \
                                        }while (0u)

/*--- AN12 ---*/
#define SetPinFunc_AN12(dummy)          do{ \
                                            bFM_GPIO_ADE_AN12=1u; \
                                        }while (0u)

/*--- AN13 ---*/
#define SetPinFunc_AN13(dummy)          do{ \
                                            bFM_GPIO_ADE_AN13=1u; \
                                        }while (0u)

/*--- AN14 ---*/
#define SetPinFunc_AN14(dummy)          do{ \
                                            bFM_GPIO_ADE_AN14=1u; \
                                        }while (0u)

/*--- AN15 ---*/
#define SetPinFunc_AN15(dummy)          do{ \
                                            bFM_GPIO_ADE_AN15=1u; \
                                        }while (0u)

/*--- AN24 ---*/
#define SetPinFunc_AN24(dummy)          do{ \
                                            bFM_GPIO_ADE_AN24=1u; \
                                        }while (0u)

/*--- AN25 ---*/
#define SetPinFunc_AN25(dummy)          do{ \
                                            bFM_GPIO_ADE_AN25=1u; \
                                        }while (0u)

/*--- AN26 ---*/
#define SetPinFunc_AN26(dummy)          do{ \
                                            bFM_GPIO_ADE_AN26=1u; \
                                        }while (0u)

/*--- AN27 ---*/
#define SetPinFunc_AN27(dummy)          do{ \
                                            bFM_GPIO_ADE_AN27=1u; \
                                        }while (0u)

/*--- AN28 ---*/
#define SetPinFunc_AN28(dummy)          do{ \
                                            bFM_GPIO_ADE_AN28=1u; \
                                        }while (0u)

/*--- AN29 ---*/
#define SetPinFunc_AN29(dummy)          do{ \
                                            bFM_GPIO_ADE_AN29=1u; \
                                        }while (0u)

/*--- AN30 ---*/
#define SetPinFunc_AN30(dummy)          do{ \
                                            bFM_GPIO_ADE_AN30=1u; \
                                        }while (0u)

/*--- AN31 ---*/
#define SetPinFunc_AN31(dummy)          do{ \
                                            bFM_GPIO_ADE_AN31=1u; \
                                        }while (0u)

#endif // #ifndef __GPIO_S6E2C5XH_H__


/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

