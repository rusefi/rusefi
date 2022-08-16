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
/******************************************************************************/
/** \file UsbDeviceMassStorage.c
 **
 ** CONST USB_ASSISTANT_OFF
 **
 ** A detailed description is available at 
 ** @link UsbDeviceMassStorageGroup USB Device Mass Storage Module description @endlink
 **
 ** History:
 ** - 2014-08-19     1.0  MSc   First Version
 ** - 2014-10-31     1.1  MSc   Fix for __STC_LINKED_LIST_ITEM_T__ while use with 
 **                             USB host
 ** - 2015-04-16     1.2  MSCH  depending on compiler optimization,
 **                             #pragma pack(1) added also for GCC
 **                             so structs don't getting destroyed
 ** - 2016-07-07     1.3  MSCH  fixing wrong max lun inforation (decreased by one)
 *****************************************************************************/

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "UsbDeviceMassStorage.h"

#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICEMASSSTORAGE_ENABLED == ON))

#include "base_types.h"
#include "usbdevice.h"
#include "usb.h"


/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/

static void RxTxCallback(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint);
static boolean_t UsbDeviceMassStorage_SendData(uint8_t* pu8Buffer, uint32_t u32DataSize);
static void ProcessNextTransfer(stc_usbn_t* pstcUsb, stc_msd_lun_command_status_t* pstcCommandStatus);
static void ProcessCbwDeviceSide(stc_msd_lun_t* pstcLun, stc_msd_cbw_t* pstcCbw, uint8_t** ppu8Data, uint32_t* pu32BlockSize, uint32_t* pu32Length,en_usb_device_massstorage_direction_t* penDir);
static void ProcessCbwHostSide(stc_msd_cbw_t* pstcCbw, uint32_t* pu32Length,en_usb_device_massstorage_direction_t* penDir);
static en_result_t CallbackRead10(stc_msd_lun_t* pstcLun);
static en_result_t CallbackWrite10(stc_msd_lun_t* pstcLun);
static en_result_t LunRead(stc_msd_lun_t* pstcLun, uint32_t u32BlockAddress, uint8_t* pu8Data, uint32_t u32Length);
static en_result_t LunWrite(stc_msd_lun_t* pstcLun, uint32_t u32BlockAddress, uint8_t* pu8Data, uint32_t u32Length);

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/

static stc_usbn_endpoint_buffer_t stcEndpointBufferOUT; //struct for custom buffer EPOUT
static uint8_t pu8EndpointBufferOUT[BUFFER_SIZE_ENDPOINTOUT]; // custom buffer EPOUT
static stc_usbn_endpoint_data_t* pstcEndpointOUT;
static stc_usbn_endpoint_data_t* pstcEndpointIN;
static volatile boolean_t bDataSent = TRUE;
static stc_usbn_t* pstcUsbHandle = NULL;
static boolean_t bReady = FALSE;
static stc_usbn_endpoint_buffer_t stcEndpointBufferIN; //struct for custom buffer EPIN
static pfn_msd_lun_read_t  LunReadCallback = LunRead;
static pfn_msd_lun_write_t LunWriteCallback = LunWrite;

static stc_msd_lun_command_status_t stcCommandStatus;
static stc_msd_lun_list_t stcLunList = {NULL,0};

static stc_scsi_mode_parameter_header6_t stcModeParameterHeader6 =
{
	sizeof(stc_scsi_mode_parameter_header6_t) - 1,        //!< Length of mode page data is 0x03
	SCSI_MSD_MEDIUM_TYPE_DIRECT_ACCESS_BLOCK_DEVICE, //!< Direct-access block device
	0,                                          //!< Reserved bits
	0,                                          //!< DPO/FUA not supported
	0,                                          //!< Reserved bits
	0,                                          //!< Medium is not write-protected
	0                                           //!< No block descriptor
};

static stc_scsi_inquiry_data_t stcInquiryData =
{
	SCSI_MSD_DIRECT_ACCESS_BLOCK_DEVICE,  // Direct-access block device
	SCSI_MSD_PERIPHERAL_DEVICE_CONNECTED, // Peripheral device is connected
	0x00,                            // Reserved bits
	0x01,                            // Media is removable
	SCSI_MSD_SPC_VERSION_4,               // SPC-4 supported
	0x2,                             // Response data format, must be 0x2
	0,                               // Hierarchical addressing not supported
	0,                               // ACA not supported
	0x0,                             // Obsolete bits
	sizeof(stc_scsi_inquiry_data_t) - 5,      // Additional Length
	0,                               // No embedded SCC
	0,                               // No access control coordinator
	SCSI_MSD_TPGS_NONE,                   // No target port support group
	0,                               // Third-party copy not supported
	0x0,                             // Reserved bits
	0,                               // Protection information not supported
        0,
	0x0,                             // Obsolete bit
	0,                               // No embedded enclosure service component
	0x0,                             // ???
	0,                               // Device is not multi-port
	0x0,                             // Obsolete bits
	0x0,                             // Unused feature
	0x0,                             // Unused features
	0,                               // Task management model not supported
	0x0,                             // ???
        0,
        0,
        0,
        0,
	{'S','P','A','N','S','I','O','N'},
        {'U','S','B',' ','S','t','o','r','a','g','e',' ',' ',' ',' ',' '},
        {'0','.','0','1'},
	{'S','P','A','N','S','I','O','N','-','U','S','B','-','D','R','I','V','E',' ',' '},
	0x00,                            // Unused features
	0x00,                            // Reserved bits
	{SCSI_MSD_VERSION_DESCRIPTOR_SBC_3},  // SBC-3 compliant device
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // Reserved
};


/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 ******************************************************************************
 ** \brief Set callbacks for LUN read / write, if not used, default read / write
 **        functionality will be used.
 **
 ** \param pfnRead Read callback, if this is NULL, default callback LunRead will be used
 **
 ** \param pfnWrite Write callback, if this is NULL, default callback LunWrite will be used
 **
 ** \result Ok on success
 **
 *****************************************************************************/
en_result_t UsbDeviceMassStorage_SetLunReadWrite(pfn_msd_lun_read_t pfnRead, pfn_msd_lun_write_t pfnWrite)
{
    if (pfnRead != NULL)
    {
        LunReadCallback = pfnRead;
    }
    else
    {
        LunReadCallback = LunRead;
    }
    if (pfnWrite != NULL)
    {
        LunWriteCallback = pfnWrite;
    }
    else
    {
        LunWriteCallback = LunWrite;
    }
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get the pointer to a LUN via ID / index of a LUN
 **
 ** \param u8ID ID / index of the LUN
 **
 ** \result NULL, if no LUN was found for the ID, else the pointer to the LUN
 **
 *****************************************************************************/
stc_msd_lun_t* UsbDeviceMassStorage_GetLun(uint8_t u8Id)
{
    stc_msd_lun_list_item_t* pstcCurrent = stcLunList.pstcRoot;
    
    while((pstcCurrent != NULL) && (pstcCurrent->u8Id != u8Id))
    {
        pstcCurrent = pstcCurrent->pstcNext;
    }
    if (pstcCurrent == NULL)
    {
        return NULL;
    }
    else
    {
        return pstcCurrent->pstcLun;
    }
}

/**
 ******************************************************************************
 ** \brief Add LUN to a list of LUNs
 **
 ** \param pstcLunItem List Item, containing pointer of LUN handle
 **
 ** \param pstcDisk Pointer of disk / media handle 
 **
 ** \param pstcCache Pointer to disk cache
 **
 ** \param u32BaseAddress LUN Base Address
 **
 ** \param u32Size Size of the media / LUN
 **
 ** \param u32BlockSize Blocksize of the media / LUN
 **
 ** \result Ok on success
 **
 *****************************************************************************/
en_result_t UsbDeviceMassStorage_AddLun(stc_msd_lun_list_item_t* pstcLunItem, stc_msd_lun_disk_t* pstcDisk, stc_msd_lun_cache_t* pstcCache, uint32_t u32BaseAddress, uint32_t u32Size, uint32_t u32BlockSize)
{
    uint32_t u32LogicalBlockAddress = (u32Size / u32BlockSize) - 1;
    stc_msd_lun_t* pstcLun = pstcLunItem->pstcLun;
    stc_msd_lun_list_item_t* pstcCurrent = stcLunList.pstcRoot;
    
    if (stcLunList.pstcRoot == NULL)             //Check for first item in the list
    {
        stcLunList.pstcRoot = pstcLunItem;       //Add first item in the list
    }
    else
    {
        while(pstcCurrent->pstcNext == NULL)     //Search for last item in the list
        {
            pstcCurrent = pstcCurrent->pstcNext;
        }
        pstcCurrent->pstcNext = pstcLunItem;     //Add last item to the list
        pstcLunItem->pstcNext = NULL;
        pstcLunItem->pstcPrev = pstcCurrent;
    }
    pstcLunItem->u8Id = stcLunList.u8Count;      //Set index of this element
    stcLunList.u8Count++;                        //Increment max data in list
    
    pstcLun->pstcDisk = pstcDisk;                //Initialize disk (media) pointer
    pstcLun->u32BaseAddress    = u32BaseAddress; //Setup base address
    pstcLun->u32Size           = u32Size;        //Setup size
    pstcLun->u32BlockSize      = u32BlockSize;   //Setup block size
    pstcLun->pstcCache         = pstcCache;      //Setup cache
    
    //Initialize Request Sense Data
    ZERO_STRUCT(pstcLun->stcRequestSenseData);   
    pstcLun->stcRequestSenseData.u7ResponseCode = SCSI_MSD_SENSE_DATA_FIXED_CURRENT;
    pstcLun->stcRequestSenseData.bValid = TRUE;
    pstcLun->stcRequestSenseData.u4SenseKey = SCSI_MSD_SENSE_KEY_NO_SENSE;
    pstcLun->stcRequestSenseData.u8AdditionalSenseLength = sizeof(stc_scsi_request_sense_data_t) - 8;
    
    //Initialize Read Capacity Data
    U32TOAU8(u32LogicalBlockAddress,pstcLun->stcReadCapacityData.au8LogicalBlockAddress);
    U32TOAU8(u32BlockSize,pstcLun->stcReadCapacityData.au8LogicalBlockLength);
             
    pstcLun->pstcInquiryData = &stcInquiryData;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Decode setup request (class specific) handler
 **
 ** \param pstcUsb USB handle
 **
 ** \param pstcSetup pointer of setup request
 *****************************************************************************/
static void DecodeSetupRequest(stc_usbn_t* pstcUsb, stc_usb_request_t* pstcSetup)
{
    uint8_t au8Buffer[1];
    au8Buffer[0] = stcLunList.u8Count - 1;
    switch (pstcSetup->bRequest)
    {
        case MSD_GET_MAX_LUN:
            if ((pstcSetup->wValue  == 0) && (pstcSetup->wIndex  == 0) && (pstcSetup->wLength == 1))
	    {
                UsbDevice_SendDataControl(pstcUsb, au8Buffer, 1);
            }
            else
            {
                pstcUsbHandle->EP0C_f.STAL = 1;
            }
            break;
        case MSD_BULK_ONLY_RESET:
            if ((pstcSetup->wValue  == 0) && (pstcSetup->wIndex  == 0) && (pstcSetup->wLength == 0))
            {
                UsbDevice_SendDataControl(pstcUsb, NULL, 0);
            }
            else
            {
                pstcUsbHandle->EP0C_f.STAL = 1;
            }
            break;     
        default:
            pstcUsbHandle->EP0C_f.STAL = 1;
            break;
    }
}

/**
 ******************************************************************************
 ** \brief Configuration changed callback
 **
 ** \param pstcUsb USB handle
 **
 *****************************************************************************/
static void ConfCallback(stc_usbn_t* pstcUsb)
{
    if (UsbDevice_GetStatus(pstcUsb) == UsbConfigured)
    {
        bReady = TRUE;
    } else
    {
        bReady = FALSE;
    }
}

/**
 ******************************************************************************
 ** \brief Init Mass Storage Class during class initialization callback
 **
 ** \param pstcUsb USB handle
 **
 *****************************************************************************/
void UsbDeviceMassStorage_Init(stc_usbn_t* pstcUsb)
{
    stc_usbdevice_class_config_t stcUsbClassConfig;
    stc_usbdevice_endpoint_config_t stcEndpointConfig;
    uint8_t* pu8Interface = NULL;
    uint8_t u8InterfaceLength = 0;
    uint8_t u8i = 0;
    uint8_t u8NumEndpoints = 0;
    pstcUsbHandle = pstcUsb;

    stcUsbClassConfig.u8InterfaceNumber          = 0xFF;                          // do not use fix interface number, choose by class, subclass, protocol
    stcUsbClassConfig.u8InterfaceClass           = 0x08;                          // CDC Class
    stcUsbClassConfig.u8InterfaceSubClass        = 0x06;                          // Custom Sub Class
    stcUsbClassConfig.u8InterfaceProtocoll       = 0x50;                          // Custom Protocol
    stcUsbClassConfig.pfnSetupRequestCallback    = DecodeSetupRequest;            // setup requests handled
    stcUsbClassConfig.pfnConnectionCallback      = NULL;                          // No connection callback handled
    stcUsbClassConfig.pfnDisconnectionCallback   = NULL;                          // No disconnection callback handled
    stcUsbClassConfig.pfnConfCallback            = ConfCallback;                  // Callback for configuration set
    UsbDevice_RegisterVendorClass(pstcUsb,&stcUsbClassConfig,&pu8Interface,&u8InterfaceLength);
        
    for(u8i = 0;u8i < u8InterfaceLength;)
    {
        u8i += pu8Interface[u8i];
        if (USBDESCR_ENDPOINT == pu8Interface[u8i + 1])
        {
            stcEndpointConfig.u8EndpointAddress = pu8Interface[u8i + 2];
            stcEndpointConfig.pfnRxTxCallback = RxTxCallback;
            if (((stcEndpointConfig.u8EndpointAddress) & 0x80) == 0)
            {
            
                stcEndpointConfig.pstcEndpointBuffer = &stcEndpointBufferOUT;
                pstcEndpointOUT = UsbDevice_SetupEndpoint(pstcUsb, &stcEndpointConfig);

                UsbDevice_ReceiveData(pstcUsb, pstcEndpointOUT, pu8EndpointBufferOUT, BUFFER_SIZE_ENDPOINTOUT, UsbIRQ);
            }
            else
            {
                stcEndpointConfig.pstcEndpointBuffer = &stcEndpointBufferIN;
                pstcEndpointIN = UsbDevice_SetupEndpoint(pstcUsb, &stcEndpointConfig);
            }
            u8NumEndpoints++;
        }
    }
}

/**
 ******************************************************************************
 ** \brief Send data via USB (IN Transfer)
 **
 ** \param pu8Buffer   Buffer
 ** \param u32DataSize Datasize
 **
 ** \return TRUE at success
 **
 *****************************************************************************/
static boolean_t UsbDeviceMassStorage_SendData(uint8_t* pu8Buffer, uint32_t u32DataSize)
{
    if (pstcUsbHandle == NULL)
    {
        return FALSE;
    }
    if (bDataSent == FALSE)
    {
        return FALSE;
    }
    if (UsbDevice_GetStatus(pstcUsbHandle) == UsbConfigured)
    {
        bDataSent = FALSE;
        
        UsbDevice_SendData(pstcUsbHandle, pstcEndpointIN, pu8Buffer, u32DataSize, UsbIRQ);
        
        return TRUE;
    }
    return FALSE;
}

/**
 ******************************************************************************
 ** \brief RX / TX data callback
 **
 ** \param pstcUsb      USB handle
 ** \param pstcEndpoint Endpoint handle
 **
 *****************************************************************************/
static void RxTxCallback(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint)
{
    volatile uint32_t u32i;
    en_usb_device_massstorage_direction_t enHostDir;
    en_usb_device_massstorage_direction_t enDeviceDir;
    uint32_t u32HostLength = 0;
    uint32_t u32DeviceLength = 0;
    uint32_t u32BlockDeviceLength = 0;
    uint8_t* pu8Data = NULL;
    stc_msd_cbw_t* pstcCbw = ((stc_msd_cbw_t*)pstcEndpoint->pstcEndpointBuffer->pu8Buffer);
    stc_msd_csw_t* pstcCsw = &stcCommandStatus.stcCsw;
    boolean_t bCbwReceived = FALSE;
    uint32_t u32DataSize = pstcEndpoint->pstcEndpointBuffer->u32DataSize;
    if ((pstcEndpoint->u8EndpointAddress) & 0x80)
    {
        SPANSION_TRACE(1,">IN\r\n");
        bDataSent = TRUE;
    } 
    else
    {
        SPANSION_TRACE(1,">OUT\r\n");
        if ((u32DataSize == sizeof(stc_msd_cbw_t)) && (pstcCbw->dCBWSignature == MSD_CBW_SIGNATURE))
        {
            bCbwReceived = TRUE;
            SPANSION_TRACE(1,"\r\n------------ TRANSFER START --------------\r\n");
            SPANSION_TRACE(1,"CBW received       \r\n");
            SPANSION_TRACE(1," -bCBWLUN:               (0x%08X) \r\n",pstcCbw->bCBWLUN);
            SPANSION_TRACE(1," -dCBWDataTransferLength:(0x%08X) \r\n",pstcCbw->dCBWDataTransferLength);
            u32HostLength = stcCommandStatus.enStatus = enScsiCommandStateUninitialized;
            if (pstcCbw->bCBWLUN < stcLunList.u8Count)
            {
                ProcessCbwHostSide(pstcCbw,&u32HostLength, &enHostDir);
                
                ProcessCbwDeviceSide(UsbDeviceMassStorage_GetLun(pstcCbw->bCBWLUN),pstcCbw,&pu8Data,&u32BlockDeviceLength, &u32DeviceLength, &enDeviceDir);
                
                SPANSION_TRACE(1," -Device Datalength:(0x%08X) \r\n",u32DeviceLength);
                if (enDeviceDir == enUsbDeviceMassStorageDeviceToHost)
                {
                    SPANSION_TRACE(1," -Device : Device -> Host\r\n");
                    stcCommandStatus.enStatus = enScsiCommandStateWaitWrite;
                } else if (enDeviceDir == enUsbDeviceMassStorageHostToDevice)
                {
                    SPANSION_TRACE(1," -Device : Host -> Device\r\n");
                    stcCommandStatus.enStatus = enScsiCommandStateWaitRead;
                } else
                {
                    SPANSION_TRACE(1," -Device : No transfer\r\n");
                    stcCommandStatus.enStatus = enScsiCommandStateNextBlock;
                }
                
                if ((enHostDir == enDeviceDir) && (u32HostLength == u32DeviceLength)) //state 1, 6 and 12
                {
                    pstcCsw->dCSWDataResidue = 0;
                    if (enDeviceDir == enUsbDeviceMassStorageDeviceToHost)
                    {
                        SPANSION_TRACE(1," - OK: Device -> Host\r\n");
                        stcCommandStatus.enStatus = enScsiCommandStateWaitWrite;
                    } else if (enDeviceDir == enUsbDeviceMassStorageHostToDevice)
                    {
                        SPANSION_TRACE(1," - OK: Host -> Device\r\n");
                        stcCommandStatus.enStatus = enScsiCommandStateWaitRead;
                    } else
                    {
                        SPANSION_TRACE(1," - OK: No transfer\r\n");
                        stcCommandStatus.enStatus = enScsiCommandStateNextBlock;
                    }
                } else if ((u32HostLength == 0) && (u32DeviceLength != 0)) // state 2 and 3
                {
                    SPANSION_TRACE(1," - Error state 2 / 3\r\n");
                    pstcCsw->bCSWStatus = MSD_CSW_PHASE_ERROR;
                    stcCommandStatus.enStatus = enScsiCommandStateNextBlock;
                    UsbDevice_HalStallEndpoint(pstcEndpointIN);
                } else if ((enHostDir == enUsbDeviceMassStorageDeviceToHost) && (u32HostLength > u32DeviceLength) && ((enDeviceDir == enUsbDeviceMassStorageNoTransfer) || (enDeviceDir == enUsbDeviceMassStorageDeviceToHost))) // case 4 and 5
                {
                    SPANSION_TRACE(1," - Error state 4 / 5\r\n")
                    pstcCsw->dCSWDataResidue = u32HostLength - u32DeviceLength;
                    stcCommandStatus.enStatus = enScsiCommandStateWaitWrite;
                    //UsbDevice_HalStallEndpoint(pstcEndpointIN);
                } else if ((enHostDir == enUsbDeviceMassStorageDeviceToHost) && (u32DeviceLength != u32HostLength)) // case 7 and 8
                {
                    SPANSION_TRACE(1," - Error state 7 / 8\r\n")
                    stcCommandStatus.enStatus = enScsiCommandStateNextBlock;
                    UsbDevice_HalStallEndpoint(pstcEndpointIN);
                } else if ((enHostDir == enUsbDeviceMassStorageHostToDevice) &&  (u32DeviceLength > u32HostLength))
                {
                    SPANSION_TRACE(1," - Error state ) / 11 / 12\r\n")
                    stcCommandStatus.enStatus = enScsiCommandStateWaitRead;
                    pstcCsw->dCSWDataResidue = u32HostLength - u32DeviceLength;
                } else if ((enHostDir == enUsbDeviceMassStorageHostToDevice) &&  (u32DeviceLength != u32HostLength))
                {
                    SPANSION_TRACE(1," - Error state 10 / 13\r\n")
                    pstcCsw->bCSWStatus = MSD_CSW_PHASE_ERROR;
                    stcCommandStatus.enStatus = enScsiCommandStateNextBlock;
                    UsbDevice_HalStallEndpoint(pstcEndpointOUT);
                } else
                {
                    SPANSION_TRACE(1," - NOT CATCHED CASE\r\n")
                }
                if (u32BlockDeviceLength == 0) 
                {
                    u32BlockDeviceLength = u32DeviceLength;
                }
                stcCommandStatus.u32Size = u32BlockDeviceLength;
                stcCommandStatus.u32Remaining = u32DeviceLength;
                stcCommandStatus.pu8Data = pu8Data;
            }
            else
            {
                SPANSION_TRACE(1,"Requesting unknown LUN\r\n");
                UsbDevice_HalStallEndpoint(pstcEndpointOUT);
            }
            
        }
    } 
    
    if ((bCbwReceived == FALSE) && (stcCommandStatus.stcCurrentTransfer.pfnCallback != NULL))
    {
        stcCommandStatus.stcCurrentTransfer.pfnCallback(UsbDeviceMassStorage_GetLun(stcCommandStatus.stcCbw.bCBWLUN));
    }
    
    ProcessNextTransfer(pstcUsb, &stcCommandStatus);
    
    SPANSION_TRACE(1,"<IN/OUT\r\n");
}

/**
 ******************************************************************************
 ** \brief Process and decode CBW from host
 **
 ** \param pstcCbw      pointer of CBW
 **
 ** \param pu32Length   Return data pointer of expected length from host
 **
 ** \param penDir       Return direction pointer of expected transfer direction from host
 **
 *****************************************************************************/
static void ProcessCbwHostSide(stc_msd_cbw_t* pstcCbw, uint32_t* pu32Length,en_usb_device_massstorage_direction_t* penDir)
{
    *pu32Length = pstcCbw->dCBWDataTransferLength;
    if (pstcCbw->dCBWDataTransferLength == 0)
    {
        *penDir = enUsbDeviceMassStorageNoTransfer;
        SPANSION_TRACE(1," - Host: no transfer\r\n");
    } else if (pstcCbw->bmCBWFlags & MSD_CBW_DEVICE_TO_HOST)
    {
        *penDir = enUsbDeviceMassStorageDeviceToHost;
        SPANSION_TRACE(1," - Host: device -> host\r\n");
    } else
    {
        *penDir = enUsbDeviceMassStorageHostToDevice;
        SPANSION_TRACE(1," - Host: host -> device\r\n");
    }
}

/**
 ******************************************************************************
 ** \brief Process next data transfer defined by pstcCommandStatus
 **
 ** \param pstcUsb            USB handle
 **
 ** \param pstcCommandStatus  Command Transport Package
 **
 *****************************************************************************/
static void ProcessNextTransfer(stc_usbn_t* pstcUsb, stc_msd_lun_command_status_t* pstcCommandStatus)
{
    switch(pstcCommandStatus->enStatus)
    {
        case enScsiCommandStateWaitWrite:
            SPANSION_TRACE(1,"/>START WRITE Size: (0x%04X)\r\n",pstcCommandStatus->u32Size);
            UsbDeviceMassStorage_SendData (pstcCommandStatus->pu8Data, pstcCommandStatus->u32Size);
            pstcCommandStatus->enStatus = enScsiCommandStateNextBlock;
            break;
        case enScsiCommandStateWaitRead:
            SPANSION_TRACE(1,"/>START READ Size: (0x%04X)\r\n",pstcCommandStatus->u32Size);
            stcEndpointBufferOUT.u32BufferSize = stcCommandStatus.u32Size;
            UsbDevice_ReceiveData(pstcUsb, pstcEndpointOUT, pstcCommandStatus->pu8Data, pstcCommandStatus->u32Size, UsbIRQ);
            pstcCommandStatus->enStatus = enScsiCommandStateNextBlock;
            break;
        case enScsiCommandStateNextBlock:
            pstcCommandStatus->stcCurrentTransfer.pfnCallback = NULL;
            SPANSION_TRACE(1,"/>END, sending CSW\r\n");
            UsbDeviceMassStorage_SendData ((uint8_t*)&pstcCommandStatus->stcCsw, sizeof(stc_msd_csw_t));
            pstcCommandStatus->enStatus = enScsiCommandStateUninitialized;
            SPANSION_TRACE(1,"\r\n------------ TRANSFER STOP --------------\r\n");
            break;
        case enScsiCommandStateUninitialized:
            pstcCommandStatus->stcCurrentTransfer.pfnCallback = NULL;
            SPANSION_TRACE(1,"/>Wait for data...\r\n");
            stcEndpointBufferOUT.u32BufferSize = sizeof(pu8EndpointBufferOUT);
            UsbDevice_ReceiveData(pstcUsb, pstcEndpointOUT, pu8EndpointBufferOUT, pstcEndpointOUT->u16EndpointSize, UsbIRQ);
            break;
        default:
            pstcCommandStatus->stcCurrentTransfer.pfnCallback = NULL;
            SPANSION_TRACE(1,"/>WARNING: unknown state, waiting for data...\r\n");
            stcEndpointBufferOUT.u32BufferSize = sizeof(pu8EndpointBufferOUT);
            UsbDevice_ReceiveData(pstcUsb, pstcEndpointOUT, pu8EndpointBufferOUT, pstcEndpointOUT->u16EndpointSize, UsbIRQ);
            pstcCommandStatus->enStatus = enScsiCommandStateUninitialized;
            break;
    }
}


/**
 ******************************************************************************
 ** \brief Process and decode CBW from device side
 **
 ** \param pstcLun       LUN pointer
 **
 ** \param pstcCbw       pointer of CBW
 **
 ** \param ppu8Data      data pointer returned
 ** 
 ** \param pu32BlockSize block to transfer
 **
 ** \param pu32Length    Return data pointer of expected length from device
 **
 ** \param penDir        Return direction pointer of expected transfer direction from device
 **
 *****************************************************************************/
static void ProcessCbwDeviceSide(stc_msd_lun_t* pstcLun, stc_msd_cbw_t* pstcCbw, uint8_t** ppu8Data, uint32_t* pu32BlockSize, uint32_t* pu32Length,en_usb_device_massstorage_direction_t* penDir)
{
    stc_scsi_command_t* pstcCommand = (stc_scsi_command_t*)pstcCbw->au16Command;
    stc_msd_csw_t* pstcCsw = &stcCommandStatus.stcCsw;
    uint32_t u32LogicalBlockAddres;
      
    pstcCsw->dCSWSignature = 0x53425355;
    pstcCsw->dCSWTag = pstcCbw->dCBWTag;
    pstcCsw->bCSWStatus = MSD_CSW_COMMAND_FAILED;
    COPY_STRUCT(stcCommandStatus.stcCbw,*pstcCbw);
    stcCommandStatus.enStatus = enScsiCommandStateUninitialized;
    stcCommandStatus.pu8Data = NULL;
    *pu32Length = 0;
    *pu32BlockSize = 0;
    *penDir = enUsbDeviceMassStorageNoTransfer;
    stcCommandStatus.u32Remaining = 0;
    switch (pstcCommand->u8OperationCode)
    {
        case SCSI_MSD_INQUIRY:
            *pu32Length = sizeof(stc_scsi_inquiry_data_t);
            pstcCsw->bCSWStatus = MSD_CSW_COMMAND_PASSED;
            *ppu8Data = (uint8_t*)pstcLun->pstcInquiryData;
            *penDir = enUsbDeviceMassStorageDeviceToHost;
            if (pstcCbw->dCBWDataTransferLength < *pu32Length)
            {
                *pu32Length = pstcCbw->dCBWDataTransferLength;
            }
            SPANSION_TRACE(1," ->SCSI_MSD_INQUIRY\r\n");
            break;
        case SCSI_MSD_MODE_SENSE_6:
            if (pstcCommand->stcModeSense6.u6PageCode == SCSI_MSD_PAGE_RETURN_ALL)
            {
                *pu32Length = sizeof(stcModeParameterHeader6);
                pstcCsw->bCSWStatus = MSD_CSW_COMMAND_PASSED;
                *ppu8Data = (uint8_t*)&stcModeParameterHeader6;
                if (pstcLun->pstcDisk->pfnWrite == NULL)
                {
                    stcModeParameterHeader6.bWP = TRUE;
                }
                else
                {
                    stcModeParameterHeader6.bWP = FALSE;
                }
                *penDir = enUsbDeviceMassStorageDeviceToHost;
                if (pstcCommand->stcModeSense6.u8AllocationLength < sizeof(stcModeParameterHeader6))
		{
		    *pu32Length = pstcCommand->stcModeSense6.u8AllocationLength;
		}
            }
            SPANSION_TRACE(1," ->SCSI_MSD_MODE_SENSE_6\r\n");
            break;
        case SCSI_MSD_PREVENT_ALLOW_MEDIUM_REMOVAL:
            SPANSION_TRACE(1," ->SCSI_MSD_PREVENT_ALLOW_MEDIUM_REMOVAL\r\n");
            pstcCsw->bCSWStatus = MSD_CSW_COMMAND_PASSED;
            break;
        case SCSI_MSD_REQUEST_SENSE:
            SPANSION_TRACE(1," ->SCSI_MSD_REQUEST_SENSE\r\n");
            *pu32Length = pstcCommand->stcRequestSense.u8AllocationLength;
            pstcCsw->bCSWStatus = MSD_CSW_COMMAND_PASSED;
            *ppu8Data = (uint8_t*)&pstcLun->stcRequestSenseData;
            *penDir = enUsbDeviceMassStorageDeviceToHost;
            break;
        case SCSI_MSD_TEST_UNIT_READY:
            switch(pstcLun->pstcDisk->enState)
            {
                case enMsdLunDiskStateReady:
                    pstcCsw->bCSWStatus = MSD_CSW_COMMAND_PASSED;
                    break;
                case enMsdLunDiskStateBusy:
                    pstcLun->stcRequestSenseData.u4SenseKey = SCSI_MSD_SENSE_KEY_NOT_READY;
                    pstcLun->stcRequestSenseData.u8AdditionalSenseCode = 0;
                    pstcLun->stcRequestSenseData.u8AdditionalSenseCodeQualifier = 0;
                    break;
                default:
                    pstcLun->stcRequestSenseData.u4SenseKey = SCSI_MSD_SENSE_KEY_NOT_READY;
                    pstcLun->stcRequestSenseData.u8AdditionalSenseCode = SCSI_MSD_ASC_MEDIUM_NOT_PRESENT;
                    pstcLun->stcRequestSenseData.u8AdditionalSenseCodeQualifier = 0;
                    break; 
            }
            SPANSION_TRACE(1," ->SCSI_MSD_TEST_UNIT_READY\r\n");
            break;
        case SCSI_MSD_READ_CAPACITY_10:
            SPANSION_TRACE(1," ->SCSI_MSD_READ_CAPACITY_10\r\n");
            *pu32Length = sizeof(pstcLun->stcReadCapacityData);
            pstcCsw->bCSWStatus = MSD_CSW_COMMAND_PASSED;
            *ppu8Data = (uint8_t*)&pstcLun->stcReadCapacityData;
            *penDir = enUsbDeviceMassStorageDeviceToHost;
            break;
        case SCSI_MSD_READ_10:
            u32LogicalBlockAddres = AU8TOU32(pstcCommand->stcRead10.au8LogicalBlockAddress);
            if (pstcLun->pstcDisk->pfnRead == NULL)
            {
                pstcCsw->bCSWStatus = MSD_CSW_COMMAND_FAILED;
                SPANSION_TRACE(1,"   ->FAILED, no read callback\r\n");
            }
            else
            {
                SPANSION_TRACE(1," ->SCSI_MSD_READ_10\r\n");
                *pu32Length = (AU8TOU16(pstcCommand->stcRead10.au8TransferLength))* (pstcLun->u32BlockSize);
                SPANSION_TRACE(1,"   ->prebuffering one block\r\n");
                if (LunReadCallback(pstcLun,AU8TOU32(pstcCommand->stcRead10.au8LogicalBlockAddress),pstcLun->pstcCache->pu8Buffer,1) == Ok)
                {
                    *ppu8Data = pstcLun->pstcCache->pu8Buffer;
                    *pu32BlockSize = pstcLun->pstcCache->u32CacheSize;
                    *penDir = enUsbDeviceMassStorageDeviceToHost;
                    stcCommandStatus.stcCurrentTransfer.pfnCallback = CallbackRead10;
                    SPANSION_TRACE(1,"   ->LOGICAL BLOCK (0x%08X)\r\n",AU8TOU32(pstcCommand->stcRead10.au8LogicalBlockAddress));
                    SPANSION_TRACE(1,"   ->read 1 block\r\n");
                    u32LogicalBlockAddres++;
                    U32TOAU8(u32LogicalBlockAddres,((stc_scsi_command_t*)stcCommandStatus.stcCbw.au16Command)->stcRead10.au8LogicalBlockAddress);
                    pstcCsw->bCSWStatus = MSD_CSW_COMMAND_PASSED;
                }
                else
                {
                    SPANSION_TRACE(1,"   ->FAILED\r\n");
                }
            }
            break;
        case SCSI_MSD_WRITE_10:
            if (pstcLun->pstcDisk->pfnWrite == NULL)
            {
                pstcCsw->bCSWStatus = MSD_CSW_COMMAND_FAILED;
                SPANSION_TRACE(1,"   ->FAILED, no write callback\r\n");
            }
            else
            {
                pstcCsw->bCSWStatus = MSD_CSW_COMMAND_PASSED;
                *pu32Length = AU8TOU16(pstcCommand->stcWrite10.au8TransferLength) * pstcLun->u32BlockSize;
                *pu32BlockSize = pstcLun->pstcCache->u32CacheSize;
                *ppu8Data = pstcLun->pstcCache->pu8Buffer;
                *penDir = enUsbDeviceMassStorageHostToDevice;
                stcCommandStatus.stcCurrentTransfer.pfnCallback = CallbackWrite10;
                SPANSION_TRACE(1," ->SCSI_MSD_WRITE_10\r\n");
            }
            break;
        case SCSI_MSD_VERIFY_10:
            SPANSION_TRACE(1," ->SCSI_MSD_VERIFY_10\r\n");
            pstcCsw->bCSWStatus = MSD_CSW_COMMAND_PASSED;
            break;
        default:
            SPANSION_TRACE(2," ->CMD UNKNOWN (0x%02X)\r\n",pstcCommand->u8OperationCode);
            *pu32Length = 0;
            *ppu8Data = NULL;
            break;
    }
    
}

/**
 ******************************************************************************
 ** \brief Callback to process next data package for READ10 command
 **
 ** \param pstcLun       LUN pointer
 **
 ** \return Ok, if successful
 **
 *****************************************************************************/
static en_result_t CallbackRead10(stc_msd_lun_t* pstcLun)
{
    stc_msd_csw_t* pstcCsw = &stcCommandStatus.stcCsw;
    stc_scsi_command_t* pstcCommand = (stc_scsi_command_t*)(stcCommandStatus.stcCbw.au16Command);
    uint32_t u32LogicalBlockAddres = AU8TOU32(pstcCommand->stcRead10.au8LogicalBlockAddress);
    stcCommandStatus.u32Remaining -= pstcLun->u32BlockSize;
    if (stcCommandStatus.u32Remaining != 0)
    {
        if (LunReadCallback(pstcLun,AU8TOU32(pstcCommand->stcRead10.au8LogicalBlockAddress),pstcLun->pstcCache->pu8Buffer,1) == Ok)
        {
            SPANSION_TRACE(1,"   ->LOGICAL BLOCK (0x%08X)\r\n",AU8TOU32(pstcCommand->stcRead10.au8LogicalBlockAddress));
            SPANSION_TRACE(1,"   ->read 1 block\r\n");
            pstcCsw->bCSWStatus = MSD_CSW_COMMAND_PASSED;
            stcCommandStatus.u32Size = pstcLun->u32BlockSize;
            stcCommandStatus.pu8Data = pstcLun->pstcCache->pu8Buffer;
            stcCommandStatus.enStatus = enScsiCommandStateWaitWrite;
            u32LogicalBlockAddres++;
            U32TOAU8(u32LogicalBlockAddres,((stc_scsi_command_t*)stcCommandStatus.stcCbw.au16Command)->stcRead10.au8LogicalBlockAddress);
        }
    }
    else
    {
        stcCommandStatus.enStatus = enScsiCommandStateNextBlock;
    }
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Callback to process next data package for WRITE10 command
 **
 ** \param pstcLun       LUN pointer
 **
 ** \return Ok, if successful
 **
 *****************************************************************************/
static en_result_t CallbackWrite10(stc_msd_lun_t* pstcLun)
{
    stc_msd_csw_t* pstcCsw = &stcCommandStatus.stcCsw;
    stc_scsi_command_t* pstcCommand = (stc_scsi_command_t*)(stcCommandStatus.stcCbw.au16Command);
    uint32_t u32LogicalBlockAddres = AU8TOU32(pstcCommand->stcWrite10.au8LogicalBlockAddress);
    SPANSION_TRACE(1,"   ->EP Received Size (0x%08X)\r\n",pstcEndpointOUT->pstcEndpointBuffer->u32DataSize);
    if (LunWriteCallback(pstcLun,AU8TOU32(pstcCommand->stcWrite10.au8LogicalBlockAddress),pstcLun->pstcCache->pu8Buffer,1) == Ok)
    {
        SPANSION_TRACE(1,"   ->LOGICAL BLOCK (0x%08X)\r\n",AU8TOU32(pstcCommand->stcWrite10.au8LogicalBlockAddress));
        SPANSION_TRACE(1,"   ->write 1 block\r\n");
        pstcCsw->bCSWStatus = MSD_CSW_COMMAND_PASSED;
        stcCommandStatus.u32Size = pstcLun->u32BlockSize;
        stcCommandStatus.pu8Data = pstcLun->pstcCache->pu8Buffer;
        stcCommandStatus.enStatus = enScsiCommandStateWaitRead;
        u32LogicalBlockAddres++;
        U32TOAU8(u32LogicalBlockAddres,((stc_scsi_command_t*)stcCommandStatus.stcCbw.au16Command)->stcWrite10.au8LogicalBlockAddress);
    }
    stcCommandStatus.u32Remaining -= pstcLun->u32BlockSize;
    if (stcCommandStatus.u32Remaining == 0)
    {
        stcCommandStatus.enStatus = enScsiCommandStateNextBlock;
    }
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Default callback for LUN read
 **
 ** \param pstcLun         LUN pointer
 **
 ** \param u32BlockAddress Block Address
 **
 ** \param pu8Data         Data
 **
 ** \param u32Length       Data Size
 **
 ** \return Ok, if successful
 **
 *****************************************************************************/
static en_result_t LunRead(stc_msd_lun_t* pstcLun, uint32_t u32BlockAddress, uint8_t* pu8Data, uint32_t u32Length)
{
    uint32_t u32Address = (uint32_t)pstcLun->pstcDisk->u32BaseAddress + pstcLun->u32BaseAddress + ((uint32_t)u32BlockAddress * pstcLun->u32BlockSize);
    if ((u32Length * pstcLun->u32BlockSize) > (pstcLun->u32Size - pstcLun->u32BlockSize * u32BlockAddress))
    {
        return Error;
    }
    else
    {
        pstcLun->pstcDisk->pfnRead(pstcLun->pstcDisk,u32Address,pu8Data,u32Length * pstcLun->u32BlockSize,NULL,NULL);
    }
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Default callback for LUN write
 **
 ** \param pstcLun         LUN pointer
 **
 ** \param u32BlockAddress Block Address
 **
 ** \param pu8Data         Data
 **
 ** \param u32Length       Data Size
 **
 ** \return Ok, if successful
 **
 *****************************************************************************/
static en_result_t LunWrite(stc_msd_lun_t* pstcLun, uint32_t u32BlockAddress, uint8_t* pu8Data, uint32_t u32Length)
{
    uint32_t u32Address = (uint32_t)pstcLun->pstcDisk->u32BaseAddress + pstcLun->u32BaseAddress + ((uint32_t)u32BlockAddress * pstcLun->u32BlockSize);
    if ((u32Length * pstcLun->u32BlockSize) > (pstcLun->u32Size - pstcLun->u32BlockSize * u32BlockAddress))
    {
        return Error;
    }
    else
    {
        pstcLun->pstcDisk->pfnWrite(pstcLun->pstcDisk,u32Address,pu8Data,u32Length * pstcLun->u32BlockSize,NULL,NULL);
    }
    return Ok;
}


#endif /* ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICEMASSSTORAGE_ENABLED == ON)) */





