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
/** \file UsbDeviceMassStorage.h
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

#ifndef __USBDEVICEMASSSTORAGE_H__
#define __USBDEVICEMASSSTORAGE_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "usb.h"

#ifndef USBDEVICEMASSSTORAGE_ENABLED
    #define USBDEVICEMASSSTORAGE_ENABLED OFF
#endif
     
#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICEMASSSTORAGE_ENABLED == ON))

#include "usbdevice.h"
#include "base_types.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

  /**
 ******************************************************************************
 ** \defgroup UsbDeviceMassStorageGroup USB Device Middleware: Mass Storage
 **
 ** Provided functions of USB module:
 ** 
 ** - UsbDeviceMassStorage_SetLunReadWrite()
 ** - UsbDeviceMassStorage_GetLun()
 ** - UsbDeviceMassStorage_AddLun()
 ** - UsbDeviceMassStorage_Init() 
 **
 ** UsbDeviceMassStorage_AddLun is called before initializing USB. 
 ** UsbDeviceMassStorage_SetLunReadWrite adds the possibility to add own LUN read/write routines.
 ** UsbDeviceMassStorage_Init called in usb class initialization phase.
 **
 ******************************************************************************/
//@{
  
#define NO_TRACE

#define USBDEVICEMASSSTORAGE_VERSION 0100

#if defined(__ICCARM__)     // IAR Workbench
    #pragma pack(1)             
    #define __attribute__(...)  
#elif defined(__CC_ARM)    // ARM MDK / Keil �Vision
    #pragma pack(1)             
    #define __attribute__(...) 
#elif defined(__GNUC__)    // GNU Compiler
    #pragma pack(1)
    /* nothing else needed */
#endif


#ifndef __MSD__
#define __MSD__
/* Mass Storage structures and defines                   */
/* - 2014-08-11     V10     MSc First Version            */

#define MSD_VERSION 0100

/* Class-specific requests */
#define MSD_BULK_ONLY_RESET                     0xFF
#define MSD_GET_MAX_LUN                         0xFE

/* Subclass codes */
#define MSD_SUBCLASS_RBC                        0x01   //<-- Reduced Block Commands (RBC) T10
#define MSD_SUBCLASS_SFF_MCC                    0x02   //<-- C/DVD devices
#define MSD_SUBCLASS_QIC                        0x03   //<-- Tape device
#define MSD_SUBCLASS_UFI                        0x04   //<-- Floppy disk drive (FDD) device
#define MSD_SUBCLASS_SFF                        0x05   //<-- Floppy disk drive (FDD) device
#define MSD_SUBCLASS_SCSI                       0x06   //<-- SCSI transparent command set

/* Table 3.1 - Mass Storage Transport Protocol (see usb_msc_overview_1.2.pdf) */
#define MSD_PROTOCOL_CBI_COMPLETION             0x00
#define MSD_PROTOCOL_CBI                        0x01
#define MSD_PROTOCOL_BULK_ONLY                  0x50

/* Test unit control: */
#define MSD_CTRL_NOT_READY                      0x00
#define MSD_CTRL_GOOD                           0x01
#define MSD_CTRL_BUSY                           0x02

/* Command block wrapper */
#define MSD_CBW_SIZE                            31          //<-- Command Block Wrapper Size
#define MSD_CBW_SIGNATURE                       0x43425355  //<-- 'USBC' 0x43425355

/* Command status wrapper */
#define MSD_CSW_SIZE                            13
#define MSD_CSW_SIGNATURE                       0x53425355

/* Table 5.3 - Command Block Status Values (usbmassbulk_10.pdf) */
#define MSD_CSW_COMMAND_PASSED                  0
#define MSD_CSW_COMMAND_FAILED                  1
#define MSD_CSW_PHASE_ERROR                     2

/* CBW bmCBWFlags field */
#define MSD_CBW_DEVICE_TO_HOST                  (1 << 7)

/**************************************************************************
 **  \brief Command Block Wrapper
 **
 **  Table 5.1 - Command Block Wrapper (see usbmassbulk_10.pdf)
 **  The CBW shall start on a packet boundary and shall end as a
 **  short packet with exactly 31 (1Fh) bytes transferred.
 **************************************************************************/
typedef struct stc_msd_cbw
{
    uint32_t  dCBWSignature;          //<-- 'USBC' 0x43425355 (little endian)
    uint32_t  dCBWTag;                //<-- Must be the same as dCSWTag
    uint32_t  dCBWDataTransferLength; //<-- Number of bytes transfer
    uint8_t   bmCBWFlags;             //<-- Indicates the directin of the
									  //    transfer: 0x80=IN=device-to-host,
									  //    0x00=OUT=host-to-device
    uint8_t   bCBWLUN        :4;      //<-- bits 0->3: bCBWLUN
    uint8_t   bReserved1     :4;      //<-- reserved
    uint8_t   bCBWCBLength   :5;      //<-- bits 0->4: bCBWCBLength
    uint8_t   bReserved2     :3;      //<-- reserved
    uint8_t   au16Command[16];        //<-- Command block
} stc_msd_cbw_t;

/**************************************************************************//**
 **  \brief Command Status Wrapper
 **
 **  Table 5.2 - Command Status Wrapper (CSW) (usbmassbulk_10.pdf)
 *****************************************************************************/
typedef struct stc_msd_csw
{
    uint32_t  dCSWSignature;   //<-- 'USBS' 0x53425355 (little endian)
    uint32_t  dCSWTag;         //<-- Must be the same as dCBWTag
    uint32_t  dCSWDataResidue; //<-- For Data-Out the device shall report in the dCSWDataResidue the difference between the amount of
                                 //    data expected as stated in the dCBWDataTransferLength, and the actual amount of data processed by
                                 //    the device. For Data-In the device shall report in the dCSWDataResidue the difference between the
                                 //    amount of data expected as stated in the dCBWDataTransferLength and the actual amount of relevant
                                 //    data sent by the device. The dCSWDataResidue shall not exceed the value sent in the dCBWDataTransferLength.
    uint8_t  bCSWStatus;       //<-- Indicates the success or failure of the command.
} stc_msd_csw_t;

#endif /* __MSD__ */


#ifndef __SCSI_MSD__
#define __SCSI_MSD__
/* see also Segate SCSI Commands Reference Manual Rev. A */
/* - 2014-08-11     V10     MSc First Version            */

#define SCSI_MSD_VERSION 0100


/* Operation codes of commands described in the SBC-3 standard */
#define SCSI_MSD_INQUIRY                                0x12
#define SCSI_MSD_READ_10                                0x28
#define SCSI_MSD_READ_CAPACITY_10                       0x25
#define SCSI_MSD_REQUEST_SENSE                          0x03
#define SCSI_MSD_TEST_UNIT_READY                        0x00
#define SCSI_MSD_WRITE_10                               0x2A

/* Optional according to the standard, required by Windows */
#define SCSI_MSD_PREVENT_ALLOW_MEDIUM_REMOVAL           0x1E
#define SCSI_MSD_MODE_SENSE_6                           0x1A
#define SCSI_MSD_VERIFY_10                              0x2F

/* Peripheral qualifier values specified in the INQUIRY data */
#define SCSI_MSD_PERIPHERAL_DEVICE_CONNECTED            0x00
#define SCSI_MSD_PERIPHERAL_DEVICE_NOT_CONNECTED        0x01
#define SCSI_MSD_PERIPHERAL_DEVICE_NOT_SUPPORTED        0x03

/* Peripheral device types specified in the INQUIRY data */
#define SCSI_MSD_DIRECT_ACCESS_BLOCK_DEVICE		0x00
#define SCSI_MSD_SEQUENTIAL_ACCESS_DEVICE		0x01
#define SCSI_MSD_PRINTER_DEVICE				0x02
#define SCSI_MSD_PROCESSOR_DEVICE			0x03
#define SCSI_MSD_WRITE_ONCE_DEVICE			0x04
#define SCSI_MSD_CD_DVD_DEVICE				0x05
#define SCSI_MSD_SCANNER_DEVICE				0x06
#define SCSI_MSD_OPTICAL_MEMORY_DEVICE			0x07
#define SCSI_MSD_MEDIA_CHANGER_DEVICE			0x08
#define SCSI_MSD_COMMUNICATION_DEVICE			0x09
#define SCSI_MSD_STORAGE_ARRAY_CONTROLLER_DEVICE        0x0C
#define SCSI_MSD_ENCLOSURE_SERVICES_DEVICE              0x0D
#define SCSI_MSD_SIMPLIFIED_DIRECT_ACCESS_DEVICE        0x0E
#define SCSI_MSD_OPTICAL_CARD_READER_WRITER_DEVICE      0x0F
#define SCSI_MSD_BRIDGE_CONTROLLER_COMMANDS             0x10
#define SCSI_MSD_OBJECT_BASED_STORAGE_DEVICE            0x11

/* Version value for the SBC-3 specification */
#define SCSI_MSD_SPC_VERSION_4                          0x06

/* Values for the TPGS field returned in INQUIRY data */
#define SCSI_MSD_TPGS_NONE                              0x0
#define SCSI_MSD_TPGS_ASYMMETRIC                        0x1
#define SCSI_MSD_TPGS_SYMMETRIC                         0x2
#define SCSI_MSD_TPGS_BOTH                              0x3

/* Version descriptor value for the SBC-3 specification */
#define SCSI_MSD_VERSION_DESCRIPTOR_SBC_3               0x04C0

/* Sense data response codes returned in REQUEST SENSE data */
#define SCSI_MSD_SENSE_DATA_FIXED_CURRENT               0x70
#define SCSI_MSD_SENSE_DATA_FIXED_DEFERRED              0x71
#define SCSI_MSD_SENSE_DATA_DESCRIPTOR_CURRENT          0x72
#define SCSI_MSD_SENSE_DATA_DESCRIPTOR_DEFERRED         0x73

/* Sense key values returned in the REQUEST SENSE data */
#define SCSI_MSD_SENSE_KEY_NO_SENSE                     0x00
#define SCSI_MSD_SENSE_KEY_RECOVERED_ERROR              0x01
#define SCSI_MSD_SENSE_KEY_NOT_READY                    0x02
#define SCSI_MSD_SENSE_KEY_MEDIUM_ERROR                 0x03
#define SCSI_MSD_SENSE_KEY_HARDWARE_ERROR               0x04
#define SCSI_MSD_SENSE_KEY_ILLEGAL_REQUEST              0x05
#define SCSI_MSD_SENSE_KEY_UNIT_ATTENTION               0x06
#define SCSI_MSD_SENSE_KEY_DATA_PROTECT                 0x07
#define SCSI_MSD_SENSE_KEY_BLANK_CHECK                  0x08
#define SCSI_MSD_SENSE_KEY_VENDOR_SPECIFIC              0x09
#define SCSI_MSD_SENSE_KEY_COPY_ABORTED                 0x0A
#define SCSI_MSD_SENSE_KEY_ABORTED_COMMAND              0x0B
#define SCSI_MSD_SENSE_KEY_VOLUME_OVERFLOW              0x0D
#define SCSI_MSD_SENSE_KEY_MISCOMPARE                   0x0E

/* Additional sense code values returned in REQUEST SENSE data */
#define SCSI_MSD_ASC_LOGICAL_UNIT_NOT_READY             0x04
#define SCSI_MSD_ASC_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE 0x21
#define SCSI_MSD_ASC_INVALID_FIELD_IN_CDB               0x24
#define SCSI_MSD_ASC_WRITE_PROTECTED                    0x27
#define SCSI_MSD_ASC_FORMAT_CORRUPTED                   0x31
#define SCSI_MSD_ASC_INVALID_COMMAND_OPERATION_CODE     0x20
#define SCSI_MSD_ASC_TOO_MUCH_WRITE_DATA                0x26
#define SCSI_MSD_ASC_NOT_READY_TO_READY_CHANGE          0x28
#define SCSI_MSD_ASC_MEDIUM_NOT_PRESENT                 0x3A

/* MEDIUM TYPE field value for direct-access block devices */
#define SCSI_MSD_MEDIUM_TYPE_DIRECT_ACCESS_BLOCK_DEVICE 0x00

/* MRIE field values */
#define SCSI_MSD_MRIE_NO_REPORTING                      0x00
#define SCSI_MSD_MRIE_ASYNCHRONOUS                      0x01
#define SCSI_MSD_MRIE_GENERATE_UNIT_ATTENTION           0x02
#define SCSI_MSD_MRIE_COND_GENERATE_RECOVERED_ERROR     0x03
#define SCSI_MSD_MRIE_UNCOND_GENERATE_RECOVERED_ERROR   0x04
#define SCSI_MSD_MRIE_GENERATE_NO_SENSE                 0x05
#define SCSI_MSD_MRIE_ON_REQUEST                        0x06

/* Supported mode pages */
#define SCSI_MSD_PAGE_READ_WRITE_ERROR_RECOVERY         0x01
#define SCSI_MSD_PAGE_INFORMATIONAL_EXCEPTIONS_CONTROL  0x1C
#define SCSI_MSD_PAGE_RETURN_ALL                        0x3F
#define SCSI_MSD_PAGE_VENDOR_SPECIFIC                   0x00

/**************************************************************************
 **  \brief Command States
 **
 **************************************************************************/
typedef enum en_scsi_command_state
{
    enScsiCommandStateUninitialized = 0,
    enScsiCommandStateRead          = 1,
    enScsiCommandStateWaitRead      = 2,
    enScsiCommandStateWrite         = 3,
    enScsiCommandStateWaitWrite     = 4,
    enScsiCommandStateNextBlock     = 5
} en_scsi_command_state_t;

/**************************************************************************
 **  \brief Inquiry Command
 **
 **************************************************************************/
typedef struct stc_scsi_inquiry
{
    uint8_t  u8OperationCode;           //<-- must be 0x12
	uint8_t  bEVPD:1;                   //<-- EVPD (Enable Vital Product Data) bit
	uint8_t  u8Reserved1:7;
	uint8_t  u8PageCode;
	uint16_t u16AllocationLength;
	uint8_t  u8Control;
} __attribute__ ((packed)) stc_scsi_inquiry_t;

/**************************************************************************
 **  \brief Command Inquiry Command Response Data
 **
 **************************************************************************/
typedef struct stc_scsi_inquiry_data
{
    uint8_t  u8PeripheralDeviceType:5;  //<-- Peripheral Device Type
    uint8_t  u8PeripheralQualifier :3;  //<-- Peripheral Qualifier
    
    uint8_t  u7Reserved1           :7;  //<-- Reserved
    uint8_t  bRMB                  :1;  //<-- RMB (Removable Media) bit
    
    uint8_t  u8Version;                 //<-- Version
    
    uint8_t  u8ResponseDataFormat  :4; 	//<-- Must be 0x2
    uint8_t  bHiSup                :1;  //<-- HISUP (Hierarchical Support) bit
    uint8_t  bNormACA              :1;  //<-- NORMACA (Normal ACA Supported)
    uint8_t  u2Obsolete1           :2;  //<-- Obsolete
    
    uint8_t  u8AdditionalLength;        //<-- (N - 4): Length of remaining INQUIRY data
    
    uint8_t  bProtect              :1;  //<-- PROTECT bit
    uint8_t  u2Reserved2           :2;  //<-- Reserved
    uint8_t  b3PC                  :1;  //<-- 3PC (Third-Party Copy) bit
    uint8_t  u2TPGS                :2;  //<-- TPGS (Target Port Group Support) field
    uint8_t  bACC                  :1;	//<-- ACC (Access Controls Coordinator) bit
    uint8_t  bSCCS                 :1;	//<-- SCCS (SCC Supported) bit
    
    uint8_t  bAddr16               :1;  //<--
    uint8_t  u2Obsolete2           :2;  //<-- Obsolete
    uint8_t  bMCHNGR               :1;  //<-- MCHNGR (Medium Changer) bit
    uint8_t  bMultiP               :1;  //<-- MULTIP (Multi Port) bit
    uint8_t  bVS1                  :1;  //<--
    uint8_t  bEncServ              :1;  //<-- ENCSERV (Enclosure Services) bit
    uint8_t  bBQue                 :1;  //<-- BQUE (Basic Queuing) bit
    
    uint8_t  bVS2                  :1;  //<--
    uint8_t  bCmdQue               :1;  //<-- CMDQUE (Command Queuing) bit
    uint8_t  bObsolete3            :1;  //<-- Obsolete
    uint8_t  bLinked               :1;  //<-- LINKED (Linked Command) bit
    uint8_t  bSync                 :1;  //<-- 
    uint8_t  bWBUS16               :1;  //<-- 
    uint8_t  u2Obsolet4            :2;  //<-- Obsolete
    
    uint8_t  auVendorID[8];             //<-- T10 vendor identification
    uint8_t  auProductID[16];           //<-- Vendor-defined product ID
    uint8_t  auProductRevisionLevel[4];	//<-- Vendor-defined product revision
    uint8_t  auVendorSpecific[20];      //<-- Vendor-specific data
    uint8_t  u8Unused3;                 //<-- Unused features
    uint8_t  u8Reserved3;               //<-- Reserved bits
    uint16_t au16VersionDescriptors[8]; //<-- Standards the device complies to
    uint8_t  pu8Reserved4[22];          //<-- Reserved bytes
} __attribute__ ((packed)) stc_scsi_inquiry_data_t;

/**************************************************************************
 **  \brief Command Read10
 **
 **************************************************************************/
typedef struct stc_scsi_read10
{
    uint8_t  u8OperationCode;           //<-- must be 0x28
    uint8_t  bObsolete1            :1;  //<-- Obsolete 
    uint8_t  bFUA_NV               :1;  //<-- Cache control bit
    uint8_t  bReserved1            :1;  //<-- Reserved
    uint8_t  bFUA                  :1;  //<-- Cache control bit
    uint8_t  bDPO                  :1;  //<-- Cache control bit
    uint8_t  u3RdProtect           :3; 	//<-- RDPROTECT field
	union
	{
        uint8_t  au8LogicalBlockAddress[4];  //<-- Index of first block to read
        uint32_t u32LogicalBlockAddress;     //<-- Index of first block to read
	} __attribute__ ((packed));
    uint8_t  u5GroupNumber         :5;  //<-- Information grouping
    uint8_t  u3Reserved2           :3;  //<-- Reserved bits
    union
	{
        uint8_t  au8TransferLength[2];	//<-- Number of blocks to transmit
        uint16_t u16TransferLength;     //<-- Number of blocks to transmit
	} __attribute__ ((packed));
    uint8_t u8Control;                  //<-- 0x00

} __attribute__ ((packed)) stc_scsi_read10_t;

/**************************************************************************
 **  \brief Command Read12
 **
 **************************************************************************/
typedef struct stc_scsi_read12
{
    uint8_t  u8OperationCode;           //<-- must be 0xa8
    uint8_t  bObsolete1            :1;  //<-- Obsolete 
    uint8_t  bFUA_NV               :1;  //<-- Cache control bit
    uint8_t  bReserved1            :1;  //<-- Reserved
    uint8_t  bFUA                  :1;  //<-- Cache control bit
    uint8_t  bDPO                  :1;  //<-- Cache control bit
    uint8_t  u3RdProtect           :3; 	//<-- RDPROTECT field
    union
    {
        uint8_t  au8LogicalBlockAddress[4];  //<-- Index of first block to read
        uint32_t u32LogicalBlockAddress;     //<-- Index of first block to read
    } __attribute__ ((packed));
    union
    {
        uint8_t  au8TransferLength[4];	//<-- Number of blocks to transmit
        uint32_t u32TransferLength;	    //<-- Number of blocks to transmit
    } __attribute__ ((packed));
    uint8_t  u5GroupNumber         :5;  //<-- Information grouping
    uint8_t  u2Reserved2           :2;  //<-- Reserved bits
    uint8_t  bMMC4                 :1;  //<-- MMC4
    uint8_t u8Control;                  //<-- 0x00
} __attribute__ ((packed)) stc_scsi_read12_t;

/**************************************************************************
 **  \brief Command Read Capaxity10
 **
 **************************************************************************/
typedef struct stc_scsi_read_capacity10
{
    uint8_t  u8OperationCode;           //<-- must be 0x25
    uint8_t  bObsolete1            :1;  //<-- Obsolete
    uint8_t  u7Reserved1           :7;  //<-- Reserved
    union
    {
        uint8_t au8LogicalBlockAddress[4]; 	//<-- Block to evaluate if PMI is set
        uint32_t u32LogicalBlockAddress; 	//<-- Block to evaluate if PMI is set
    } __attribute__ ((packed));
    uint8_t au8Reserved2[2];            //<-- Reserved bytes
    uint8_t bPMI                   :1;  //<-- Partial medium indicator bit
    uint8_t u7Reserved3            :7;  //<-- Reserved bits
    uint8_t u8Control;                  //<-- 0x00
} __attribute__ ((packed)) stc_scsi_read_capacity10_t;

/**************************************************************************
 **  \brief Command Read Capacity10 Response Data
 **
 **************************************************************************/
typedef struct stc_scsi_read_capacity10_data
{
    union
    {
	    uint8_t  au8LogicalBlockAddress[4];//<-- Address of last logical block
        uint32_t u32LogicalBlockAddress;   //<-- Address of last logical block
    } __attribute__ ((packed));
    union
    {
	    uint8_t  au8LogicalBlockLength[4]; //<-- Length of last logical block
        uint32_t u32LogicalBlockLength;    //<-- Length of last logical block
    } __attribute__ ((packed));
} __attribute__ ((packed)) stc_scsi_read_capacity10_data_t;

/**************************************************************************
 **  \brief Command Request Sense
 **
 **************************************************************************/
typedef struct stc_scsi_request_sense
{
    uint8_t  u8OperationCode;           //<-- must be 0x03
    uint8_t  bDesc	             :1;    //<-- Type of information expected
    uint8_t  u7Reserved1         :7;    //<-- Reserved bits
    uint8_t  au8Reserved2[2];           //<-- Reserved bytes
    uint8_t  u8AllocationLength;        //<-- Size of host buffer
    uint8_t  u8Control;		            //<-- x00
} __attribute__ ((packed)) stc_scsi_request_sense_t;

/**************************************************************************
 **  \brief Command Request Sense Response Data
 **
 **************************************************************************/
typedef struct stc_scsi_request_sense_data
{
    uint8_t  u7ResponseCode       :7;   //<-- Sense data format
    uint8_t  bValid               :1;   //<-- Information field is standard
    uint8_t  u8Obsolete1;               //<-- Obsolete
    uint8_t  u4SenseKey           :4;   //<-- Generic error information
    uint8_t  bReserved1           :1;   //<-- Reserved bit
    uint8_t  bILI	              :1;   //<-- SSC
    uint8_t  bEOM	              :1;   //<-- SSC
    uint8_t  bFilemark            :1;   //<-- SSC
    union
    {
        uint8_t  au8Information[4];     //<-- Command-specific
        uint32_t u32Information;        //<-- Command-specific
    } __attribute__ ((packed));
    uint8_t  u8AdditionalSenseLength;   //<-- sizeof(stc_scsi_request_sense_data_t)-8
    union
    {
        uint8_t  au8CommandSpecificInformation[4]; //<-- Command-specific
        uint32_t u32CommandSpecificInformation;    //<-- Command-specific
    } __attribute__ ((packed));
    uint8_t  u8AdditionalSenseCode;		//<-- Additional error information
    uint8_t  u8AdditionalSenseCodeQualifier; //<-- Further error information
    uint8_t  u8FieldReplaceableUnitCode;	//<-- Specific component code
    uint8_t  u7SenseKeySpecific    :7;  //<-- Additional exception info
    uint8_t  bSKSV		           :1;  //<-- Is sense key specific valid?
    uint8_t  au8SenseKeySpecific[2];    //<-- Additional exception info
} __attribute__ ((packed)) stc_scsi_request_sense_data_t;

/**************************************************************************
 **  \brief CommandTest Unit Ready
 **
 **************************************************************************/
typedef struct stc_scsi_test_unit_ready
{
    uint8_t  bOperationCode;              //<-- must be 0x00
    uint8_t  au8Reserved1[4];             //<-- Reserved
    uint8_t  u8Control;                   //<-- 0x00
} __attribute__ ((packed)) stc_scsi_test_unit_ready_t;

/**************************************************************************
 **  \brief Command Write10
 **
 **************************************************************************/
typedef struct stc_scsi_write10
{
    uint8_t  u8OperationCode;             //<-- must be 0x2A
    uint8_t  bObsolete1           :1;     //<-- Obsolete bit
    uint8_t  bFUA_NV              :1;     //<-- Cache control bit
    uint8_t  bReserved1           :1;     //<-- Reserved bit
    uint8_t  bFUA                 :1;     //<-- Cache control bit
    uint8_t  bDPO                 :1;     //<-- Cache control bit
    uint8_t  u3WrProtect          :3;     //<-- Protection information to send
    union
    {
        uint8_t  au8LogicalBlockAddress[4]; //<-- First block to write
	    uint32_t u32LogicalBlockAddress;    //<-- First block to write     
    };
    uint8_t  u5GroupNumber         :5; 		//<-- Information grouping
    uint8_t  u3Reserved2           :3; 		//<-- Reserved bits
    union
    {
        uint8_t  au8TransferLength[2];    //<-- Number of blocks to write
        uint16_t u16TransferLength;
    };
    uint8_t  u8Control;                   //<-- 0x00
} stc_scsi_write10_t;

/**************************************************************************
 **  \brief Command Medium Removal
 **
 **************************************************************************/
typedef struct stc_scsi_medium_removal
{
    uint8_t  u32OperationCode;           //<-- must be 0x1E
    uint8_t  au8Reserved1[3];            //<-- must be Reserved bytes
    uint8_t  u2Prevent          :2;      //<-- must be Accept/prohibit removal
    uint8_t  u6Reserved2        :6;      //<-- must be Reserved bits
    uint8_t  u8Control;                  //<-- must be 0x00
} __attribute__ ((packed)) stc_scsi_medium_removal_t;

/**************************************************************************
 **  \brief Command Mode Sense6
 **
 **************************************************************************/
typedef struct stc_scsi_mode_sense6
{
    uint8_t  bOperationCode;             //<-- must be 0x1A
    uint8_t  bReserved1         :3;      //<-- Reserved
    uint8_t  bDBD               :1;      //<-- Disable block descriptors bit
    uint8_t  u4Reserved2        :4;      //<-- Reserved
    uint8_t  u6PageCode         :6;      //<-- Mode page to return
    uint8_t  u2PC               :2;      //<-- Type of parameter values to return
    uint8_t  u8SubpageCode;              //<-- Mode subpage to return
    uint8_t  u8AllocationLength;         //<-- Host buffer allocated size
    uint8_t  u8Control;                  //<-- 0x00
} __attribute__ ((packed)) stc_scsi_mode_sense6_t;

/**************************************************************************
 **  \brief Command Mode Sense6 Response Data Header
 **
 **************************************************************************/
typedef struct stc_scsi_mode_parameter_header6
{
    uint8_t  u8ModeDataLength;		     //<-- Length of mode data to follow
    uint8_t  u8MediumType;			     //<-- Type of medium (SCSI_MSD_MEDIUM_TYPE_DIRECT_ACCESS_BLOCK_DEVICE)
    uint8_t  u4Reserved1            :4;  //<-- Reserved bits
    uint8_t  bDPOFUA                :1;  //<-- DPO/FUA bits supported ?
    uint8_t  u2Reserved2            :2;  //<-- Reserved 
    uint8_t  bWP                    :1;  //<-- Is medium write-protected ?
    uint8_t  u8BlockDescriptorLength;    //<-- Length of all block descriptors
} __attribute__ ((packed)) stc_scsi_mode_parameter_header6_t;

/**************************************************************************
 **  \brief Command Response Data
 **
 **************************************************************************/
typedef union
{
    uint8_t u8OperationCode; 
    stc_scsi_inquiry_t                  stcInquiry;
    stc_scsi_read10_t                   stcRead10;
    stc_scsi_read_capacity10_t          stcReadCapacity10;     
    stc_scsi_request_sense_t            stcRequestSense;
    stc_scsi_test_unit_ready_t          stcTestUnitReady;      
    stc_scsi_write10_t		        stcWrite10;	
    stc_scsi_medium_removal_t           stcMediumRemoval;       
    stc_scsi_mode_sense6_t              stcModeSense6;	         
} stc_scsi_command_t;

#endif /* __SCSI_MSD__ */

#ifndef __MSD_LUN__
#define __MSD_LUN__
/* - 2014-08-11     V10     MSc First Version            */
#define MSD_LUN_VERSION 0100


#define MSD_LUN_DISK_STATUS_SUCCESS      0x00
#define MSD_LUN_DISK_STATUS_ERROR        0x01
#define MSD_LUN_DISK_STATUS_BUSY         0x02

#define MSD_LUN_DISK_STATE_READY         0x00
#define MSD_LUN_DISK_STATE_BUSY          0x01

/**************************************************************************
 **  \brief LUN Disk Status
 **
 **************************************************************************/
typedef enum en_msd_lun_disk_status
{
    enMsdLunDiskStatusSuccess = MSD_LUN_DISK_STATUS_SUCCESS,
    enMsdLunDiskStatusError =   MSD_LUN_DISK_STATUS_ERROR,
    enMsdLunDiskStatusBusy =    MSD_LUN_DISK_STATUS_BUSY,
} en_msd_lun_disk_status_t;

/**************************************************************************
 **  \brief LUN Disk State
 **
 **************************************************************************/
typedef enum en_msd_lun_disk_state
{
    enMsdLunDiskStateReady =   MSD_LUN_DISK_STATE_READY,
    enMsdLunDiskStateBusy =    MSD_LUN_DISK_STATE_BUSY,
} en_msd_lun_disk_state_t;

typedef struct stc_msd_lun_disk stc_msd_lun_disk_t;
typedef struct stc_msd_lun_command_status stc_msd_lun_command_status_t;
typedef struct stc_msd_lun stc_msd_lun_t;

typedef en_result_t (*pfn_msd_lun_disk_callback_t)(stc_msd_lun_t* pstcLun);
typedef uint8_t (*pfn_msd_lun_disk_write_t)(stc_msd_lun_disk_t* pstcDisk, uint32_t u32Address, void* pData, uint32_t u32Length, pfn_msd_lun_disk_callback_t pfnCallback, void* pArgument);
typedef uint8_t (*pfn_msd_lun_disk_read_t)(stc_msd_lun_disk_t* pstcDisk, uint32_t u32Address, void* pData, uint32_t u32Length, pfn_msd_lun_disk_callback_t pfnCallback, void* pArgument);
typedef en_result_t (*pfn_msd_lun_read_t)(stc_msd_lun_t* pstcLun, uint32_t u32BlockAddress, uint8_t* pu8Data, uint32_t u32Length);
typedef en_result_t (*pfn_msd_lun_write_t)(stc_msd_lun_t* pstcLun, uint32_t u32BlockAddress, uint8_t* pu8Data, uint32_t u32Length);

typedef struct stc_msd_lun_transfer
{
    void*                       pData;
    uint32_t                    u32Address;
    uint32_t                    u32Length;
    pfn_msd_lun_disk_callback_t pfnCallback;
    void*                       pArgument;
} stc_msd_lun_transfer_t;

struct stc_msd_lun_command_status
{
    stc_msd_lun_transfer_t                stcCurrentTransfer;   
    stc_msd_csw_t                         stcCsw;
    stc_msd_cbw_t                         stcCbw;
    uint8_t*                              pu8Data;
    uint32_t                              u32Remaining;
    uint32_t                              u32Size;
    en_scsi_command_state_t               enStatus;
};

struct stc_msd_lun_disk
{
    pfn_msd_lun_disk_read_t       pfnRead;
    pfn_msd_lun_disk_write_t      pfnWrite;
    uint32_t                      u32BaseAddress;
    uint32_t                      u32Size;
    stc_msd_lun_transfer_t*       pstcCurrentTransfer;
    void*                         pInterface;
    en_msd_lun_disk_state_t       enState;
}; // stc_msd_lun_disk_t;

typedef struct stc_msd_lun_cache
{
    uint8_t*    pu8Buffer;
    uint32_t    u32CacheSize;
} stc_msd_lun_cache_t;

struct stc_msd_lun
{
    stc_scsi_inquiry_data_t*         pstcInquiryData;
    stc_scsi_request_sense_data_t    stcRequestSenseData;
    stc_scsi_read_capacity10_data_t  stcReadCapacityData;
    stc_msd_lun_cache_t*             pstcCache;
    uint32_t                         u32BaseAddress;
    uint32_t                         u32Size;
    uint32_t                         u32BlockSize;
    stc_msd_lun_disk_t*              pstcDisk;
};

typedef struct stc_msd_lun_list_item stc_msd_lun_list_item_t;

struct stc_msd_lun_list_item
{
    stc_msd_lun_t* pstcLun;
    uint8_t u8Id;
    stc_msd_lun_list_item_t* pstcPrev;
    stc_msd_lun_list_item_t* pstcNext;
};

typedef struct stc_msd_lun_list
{
    stc_msd_lun_list_item_t* pstcRoot;
    uint8_t u8Count;
} stc_msd_lun_list_t;

#endif /* __MSD_LUN__ */


#if defined(__ICCARM__)   // IAR
#pragma pack()			
#elif defined(__CC_ARM)    // ARM MDK / Keil �Vision
    //#pragma pack(0)     
#elif defined(__GNUC__)    // GNU Compiler
    /* nothing needed */
#endif

#ifndef __STC_LINKED_LIST_ITEM_T__
#define __STC_LINKED_LIST_ITEM_T__
struct stc_linked_list_item;

typedef struct stc_linked_list_item
{
    uint32_t u32Id;
    struct stc_linked_list_item* pstcPrev;
    struct stc_linked_list_item* pstcNext;
    void* pHandle;
} stc_linked_list_item_t;

#endif

#ifndef COPY_STRUCT
#define COPY_STRUCT(dest,src) memcpy(&(dest),&(src),sizeof(src))
#endif

#ifndef AU8TOU16
#define AU8TOU16(bytes)  ((uint16_t) ((bytes[0] << 8) | bytes[1]))
#endif

#ifndef AU8TOU32
#define AU8TOU32(bytes)  ((uint32_t) ((bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]))
#endif

#ifndef U32TOAU8
#define U32TOAU8(ui,bytes) \
	bytes[0] = (uint8_t) (((ui) >> 24) & 0xFF); \
	bytes[1] = (uint8_t) (((ui) >> 16) & 0xFF); \
	bytes[2] = (uint8_t) (((ui) >> 8) & 0xFF); \
	bytes[3] = (uint8_t) ((ui) & 0xFF);
#endif
        
#ifndef U16TOAU8
#define U16TOAU8(ui,bytes) \
	bytes[0] = (uint8_t) (((ui) >> 8) & 0xFF); \
	bytes[1] = (uint8_t) ((ui) & 0xFF);
#endif        


#ifndef SPANSION_TRACE
    #if defined(NO_TRACE)
        #define SPANSION_TRACE(level,...)
    #else
        #include "uart.h"
        #ifndef SPANSION_TRACE_LEVEL
            #define SPANSION_TRACE_LEVEL 0
        #endif
        #define SPANSION_TRACE(level,...)                  \
        {                                                  \
            if (level >= SPANSION_TRACE_LEVEL)             \
            {                                              \
                char_t tracebuffer[100+1];                 \
                tracebuffer[100] = '\0';                   \
                snprintf(tracebuffer, 100, __VA_ARGS__);   \
                puts(tracebuffer);                         \
            }                                              \
        }
    #endif
#endif
    
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/



#define BUFFER_SIZE_ENDPOINTOUT 64



/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/
typedef enum en_usb_device_massstorage_direction
{
    enUsbDeviceMassStorageNoTransfer = 0,
    enUsbDeviceMassStorageDeviceToHost = 1,
    enUsbDeviceMassStorageHostToDevice = 2,
} en_usb_device_massstorage_direction_t;


/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/



/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

en_result_t UsbDeviceMassStorage_SetLunReadWrite(pfn_msd_lun_read_t pfnRead, pfn_msd_lun_write_t pfnWrite);
stc_msd_lun_t* UsbDeviceMassStorage_GetLun(uint8_t u8Id);
en_result_t UsbDeviceMassStorage_AddLun(stc_msd_lun_list_item_t* pstcLunItem, stc_msd_lun_disk_t* pstcDisk, stc_msd_lun_cache_t* pstcCache, uint32_t u32BaseAddress, uint32_t u32Size, uint32_t u32BlockSize);
void UsbDeviceMassStorage_Init(stc_usbn_t* pstcUsb);
#ifdef __cplusplus
}
#endif

//@} // UsbDeviceMassStorageGroup

#else
#define UsbDeviceMassStorage_Init(x) ;
#endif /* ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICEMASSSTORAGE_ENABLED == ON)) */
#endif /* __USBDEVICEMASSSTORAGE_H__ */
