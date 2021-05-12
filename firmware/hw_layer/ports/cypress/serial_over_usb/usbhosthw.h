/*******************************************************************************
* Copyright (C) 2013-2016, Cypress Semiconductor Corporation or a              *
* subsidiary of Cypress Semiconductor Corporation.  All rights reserved.       *
*                                                                              *
* This software, including source code, documentation and related              *
* materials ("Software"), is owned by Cypress Semiconductor Corporation or     *
* one of its subsidiaries ("Cypress") and is protected by and subject to       *
* worldwide patent protection (United States and foreign), United States       *
* copyright laws and international treaty provisions. Therefore, you may use   *
* this Software only as provided in the license agreement accompanying the     *
* software package from which you obtained this Software ("EULA").             *
*                                                                              *
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,     *
* non-transferable license to copy, modify, and compile the                    *
* Software source code solely for use in connection with Cypress's             *
* integrated circuit products.  Any reproduction, modification, translation,   *
* compilation, or representation of this Software except as specified          *
* above is prohibited without the express written permission of Cypress.       *
*                                                                              *
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO                         *
* WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING,                         *
* BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED                                 *
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A                              *
* PARTICULAR PURPOSE. Cypress reserves the right to make                       *
* changes to the Software without notice. Cypress does not assume any          *
* liability arising out of the application or use of the Software or any       *
* product or circuit described in the Software. Cypress does not               *
* authorize its products for use in any products where a malfunction or        *
* failure of the Cypress product may reasonably be expected to result in       *
* significant property damage, injury or death ("High Risk Product"). By       *
* including Cypress's product in a High Risk Product, the manufacturer         *
* of such system or application assumes all risk of such use and in doing      *
* so agrees to indemnify Cypress against all liability.                        *
*******************************************************************************/
/************************************************************************/
/** \file UsbHostHW.h
 **
 ** USB hardware specific settings
 **
 ** History:
 **   - 2010-10-14    1.0  MSc  First version  (works with 16FX,FR80)
 **   - 2011-03-30    1.1  MSc  Compatible with FSEU Host drivers 2011-03-30
 *****************************************************************************/

#ifndef __USBHOSTHW_H__
#define __USBHOSTHW_H__

#define HOST0VBUS_DEINIT                                  
#define HOST0VBUS_INIT                                    
#define HOST0VBUS_SET                                     
#define HOST0VBUS_CLEAR                                   
#define HOST0VBUS_GET                                     
#define HOST0VBUS_ENABLED                                 0

#define HOST0OTGPULLDOWN_DEINIT                           
#define HOST0OTGPULLDOWN_INIT                             
#define HOST0OTGPULLDOWN_SET                              
#define HOST0OTGPULLDOWN_CLEAR                            
#define HOST0OTGPULLDOWN_GET                              1
#define HOST0OTGPULLDOWN_ENABLED                          0

#define HOST0OVERCURRENT_DISABLEISR                       
#define HOST0OVERCURRENT_ENABLEISR                        
#define HOST0OVERCURRENT_CLEARISRFLAG                     
#define HOST0OVERCURRENT_ISRISSET                         0
#define HOST0OVERCURRENT_SETLOWDETECT                     
#define HOST0OVERCURRENT_SETHIGHDETECT                    
#define HOST0OVERCURRENT_HIGHDETECT                       0
#define HOST0OVERCURRENT_INIT                             
#define HOST0OVERCURRENT_DEINIT                           
#define HOST0OVERCURRENT_ENABLED                          0


#define HOST1VBUS_DEINIT                                  
#define HOST1VBUS_INIT                                    
#define HOST1VBUS_SET                                     
#define HOST1VBUS_CLEAR                                   
#define HOST1VBUS_GET                                     0
#define HOST1VBUS_ENABLED                                 0

#define HOST1OTGPULLDOWN_DEINIT                           
#define HOST1OTGPULLDOWN_INIT                             
#define HOST1OTGPULLDOWN_SET                              
#define HOST1OTGPULLDOWN_CLEAR                            
#define HOST1OTGPULLDOWN_GET                              1
#define HOST1OTGPULLDOWN_ENABLED                          0

#define HOST1OVERCURRENT_DISABLEISR                       
#define HOST1OVERCURRENT_ENABLEISR                        
#define HOST1OVERCURRENT_CLEARISRFLAG                     
#define HOST1OVERCURRENT_ISRISSET                         0
#define HOST1OVERCURRENT_SETLOWDETECT                     
#define HOST1OVERCURRENT_SETHIGHDETECT                    
#define HOST1OVERCURRENT_HIGHDETECT                       0
#define HOST1OVERCURRENT_INIT                             
#define HOST1OVERCURRENT_DEINIT                           
#define HOST0OVERCURRENT_ENABLED                          0


#endif
