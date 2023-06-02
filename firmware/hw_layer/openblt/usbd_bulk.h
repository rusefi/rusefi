/**
  ******************************************************************************
  * @file    usbd_bulk.h
  * @author  MCD Application Team
  * @version V2.4.2
  * @date    11-December-2015
  * @brief   header file for the usbd_bulk.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 
 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_BULK_H
#define __USB_BULK_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */
  
/** @defgroup usbd_cdc
  * @brief This file is the Header file for usbd_cdc.c
  * @{
  */ 


/** @defgroup usbd_cdc_Exported_Defines
  * @{
  */ 
#define BULK_IN_EP                                  0x81  /* EP1 for data IN */
#define BULK_OUT_EP                                 0x01  /* EP1 for data OUT */

/* CDC Endpoints parameters: you can fine tune these values depending on the needed baudrates and performance. */
#define BULK_DATA_MAX_PACKET_SIZE                   64  /* Endpoint IN & OUT Packet size */

#define USB_BULK_CONFIG_DESC_SIZ                    32
#define USB_BULK_DESC_SIZ                           (32-9)

#define BULK_DESCRIPTOR_TYPE                        0x21


#define BULK_DATA_FS_IN_PACKET_SIZE                 BULK_DATA_MAX_PACKET_SIZE
#define BULK_DATA_FS_OUT_PACKET_SIZE                BULK_DATA_MAX_PACKET_SIZE


/**
  * @}
  */ 


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */

/**
  * @}
  */ 


/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */ 
  
/**
  * @}
  */ 

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */ 

extern USBD_ClassTypeDef   USBD_Bulk;
/**
  * @}
  */ 

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
uint8_t * USBD_Bulk_GetRxBufferPtr(void);
/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif  /* __USB_BULK_H */
/**
  * @}
  */ 

/**
  * @}
  */ 
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
