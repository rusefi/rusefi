/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _USB_MSD_H_
#define _USB_MSD_H_

#include "hal.h"

/* Default to disabled for USB Mass Storage */
#if !defined(HAL_USE_MASS_STORAGE_USB)
#  define HAL_USE_MASS_STORAGE_USB   TRUE
#endif

#if HAL_USE_MASS_STORAGE_USB || defined(__DOXYGEN__)

#ifndef PACK_STRUCT_BEGIN
#define PACK_STRUCT_BEGIN
#endif

#ifndef PACK_STRUCT_STRUCT
#define PACK_STRUCT_STRUCT
#endif

#ifndef PACK_STRUCT_END
#define PACK_STRUCT_END
#endif

#if 0
#define MSD_RW_LED_ON()   palSetPad(GPIOI, GPIOI_TRI_LED_BLUE)
#define MSD_RW_LED_OFF()  palClearPad(GPIOI, GPIOI_TRI_LED_BLUE)
#define MSD_R_LED_ON()   palSetPad(GPIOI, GPIOI_TRI_LED_BLUE)
#define MSD_R_LED_OFF()  palClearPad(GPIOI, GPIOI_TRI_LED_BLUE)
#define MSD_W_LED_ON()   palSetPad(GPIOH, GPIOI_TRI_LED_BLUE)
#define MSD_W_LED_OFF()  palClearPad(GPIOH, GPIOI_TRI_LED_BLUE)
#else
#define MSD_RW_LED_ON()
#define MSD_RW_LED_OFF()
#define MSD_R_LED_ON()
#define MSD_R_LED_OFF()
#define MSD_W_LED_ON()
#define MSD_W_LED_OFF()
#endif


#if STM32_USB_USE_OTG2 && STM32_USE_USB_OTG2_HS
#  define USB_MS_EP_SIZE 512
#else
#  define USB_MS_EP_SIZE 64
#endif

#define MSD_THREAD_STACK_SIZE     1024

#define MSD_REQ_RESET		0xFF
#define MSD_GET_MAX_LUN		0xFE
#define MSD_CBW_SIGNATURE                               0x43425355
#define MSD_CSW_SIGNATURE                               0x53425355
#define MSD_COMMAND_DIR_DATA_OUT                        (0 << 7)
#define MSD_COMMAND_DIR_DATA_IN                         (1 << 7)

#define MSD_SETUP_WORD(setup, index) (uint16_t)(((uint16_t)setup[index+1] << 8) | (setup[index] & 0x00FF))

#define MSD_SETUP_VALUE(setup) MSD_SETUP_WORD(setup, 2)
#define MSD_SETUP_INDEX(setup) MSD_SETUP_WORD(setup, 4)
#define MSD_SETUP_LENGTH(setup) MSD_SETUP_WORD(setup, 6)

typedef enum {
    SCSI_CMD_TEST_UNIT_READY                = 0x00,
    SCSI_CMD_REQUEST_SENSE_6                = 0x03,
    SCSI_CMD_INQUIRY                        = 0x12,
    SCSI_CMD_MODE_SENSE_6                   = 0x1A,
    SCSI_CMD_START_STOP_UNIT                = 0x1B,
    SCSI_CMD_SEND_DIAGNOSTIC                = 0x1D,
    SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL   = 0x1E,
    SCSI_CMD_READ_FORMAT_CAPACITY           = 0x23,
    SCSI_CMD_READ_CAPACITY_10               = 0x25,
    SCSI_CMD_READ_10                        = 0x28,
    SCSI_CMD_WRITE_10                       = 0x2A,
    SCSI_CMD_VERIFY_10                      = 0x2F,
    SCSI_CMD_SYNCHRONIZE_CACHE_10           = 0x35,
} msd_scsi_command_t;



#define MSD_COMMAND_PASSED                               0x00
#define MSD_COMMAND_FAILED                               0x01
#define MSD_COMMAND_PHASE_ERROR                          0x02

#define SCSI_SENSE_KEY_GOOD                              0x00
#define SCSI_SENSE_KEY_RECOVERED_ERROR                   0x01
#define SCSI_SENSE_KEY_NOT_READY                         0x02
#define SCSI_SENSE_KEY_MEDIUM_ERROR                      0x03
#define SCSI_SENSE_KEY_HARDWARE_ERROR                    0x04
#define SCSI_SENSE_KEY_ILLEGAL_REQUEST                   0x05
#define SCSI_SENSE_KEY_UNIT_ATTENTION                    0x06
#define SCSI_SENSE_KEY_DATA_PROTECT                      0x07
#define SCSI_SENSE_KEY_BLANK_CHECK                       0x08
#define SCSI_SENSE_KEY_VENDOR_SPECIFIC                   0x09
#define SCSI_SENSE_KEY_COPY_ABORTED                      0x0A
#define SCSI_SENSE_KEY_ABORTED_COMMAND                   0x0B
#define SCSI_SENSE_KEY_VOLUME_OVERFLOW                   0x0D
#define SCSI_SENSE_KEY_MISCOMPARE                        0x0E

#define SCSI_ASENSE_NO_ADDITIONAL_INFORMATION            0x00
#define SCSI_ASENSE_PEREPHERIAL_DEVICE_WRITE_FAULT       0x03
#define SCSI_ASENSE_LOGICAL_UNIT_NOT_READY               0x04
#define SCSI_ASENSE_UNRECOVERED_READ_ERROR               0x11
#define SCSI_ASENSE_INVALID_COMMAND                      0x20
#define SCSI_ASENSE_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE   0x21
#define SCSI_ASENSE_INVALID_FIELD_IN_CDB                 0x24
#define SCSI_ASENSE_WRITE_PROTECTED                      0x27
#define SCSI_ASENSE_NOT_READY_TO_READY_CHANGE            0x28
#define SCSI_ASENSE_FORMAT_ERROR                         0x31
#define SCSI_ASENSE_MEDIUM_NOT_PRESENT                   0x3A

#define SCSI_ASENSEQ_NO_QUALIFIER                        0x00
#define SCSI_ASENSEQ_PEREPHERIAL_DEVICE_WRITE_FAULT      0x00
#define SCSI_ASENSEQ_FORMAT_COMMAND_FAILED               0x01
#define SCSI_ASENSEQ_INITIALIZING_COMMAND_REQUIRED       0x02
#define SCSI_ASENSEQ_OPERATION_IN_PROGRESS               0x07


PACK_STRUCT_BEGIN typedef struct {
	uint32_t signature;
	uint32_t tag;
	uint32_t data_len;
	uint8_t flags;
	uint8_t lun;
	uint8_t scsi_cmd_len;
	uint8_t scsi_cmd_data[16];
} PACK_STRUCT_STRUCT msd_cbw_t PACK_STRUCT_END;

PACK_STRUCT_BEGIN typedef struct {
	uint32_t signature;
	uint32_t tag;
	uint32_t data_residue;
	uint8_t status;
} PACK_STRUCT_STRUCT msd_csw_t PACK_STRUCT_END;

typedef struct {
		uint8_t byte[18];
} PACK_STRUCT_STRUCT scsi_sense_response_t;

typedef struct {
  uint8_t mode_data_length; //Number of bytes that follow
  uint8_t medium_type; //0x00 for SBC devices
  uint8_t device_specifc_paramters; //bit 7 is the write protect bit
  uint8_t block_descriptor_length; //Length in bytes of all block descriptors in the mode parameter list.
} PACK_STRUCT_STRUCT scsi_mode_sense6_response_t;

PACK_STRUCT_BEGIN typedef struct
{
	uint8_t peripheral;
	uint8_t removable;
	uint8_t version;
	uint8_t response_data_format;
	uint8_t additional_length;
	uint8_t sccstp;
	uint8_t bqueetc;
	uint8_t cmdque;
	uint8_t vendorID[8];
	uint8_t productID[16];
	uint8_t productRev[4];
} PACK_STRUCT_STRUCT scsi_inquiry_response_t PACK_STRUCT_END;

PACK_STRUCT_BEGIN typedef struct {
    uint8_t payload_byte_length[4];
    uint32_t last_block_addr;
    uint32_t block_size;
} PACK_STRUCT_STRUCT scsi_read_format_capacity_response_t PACK_STRUCT_END;

PACK_STRUCT_BEGIN typedef struct {
	uint32_t last_block_addr;
	uint32_t block_size;
} PACK_STRUCT_STRUCT scsi_read_capacity10_response_t PACK_STRUCT_END;

PACK_STRUCT_BEGIN typedef struct {
	uint8_t op_code;
	uint8_t lun_immed;
	uint8_t res1;
	uint8_t res2;
	uint8_t loej_start;
	uint8_t control;
} PACK_STRUCT_STRUCT scsi_start_stop_unit_request_t;



typedef enum {
  MSD_WAIT_MODE_NONE = 0,
  MSD_WAIT_MODE_BULK_IN,
  MSD_WAIT_MODE_BULK_OUT
} msd_wait_mode_t;

typedef enum {
  MSD_STATE_IDLE = 0,
  MSD_STATE_READ_CMD_BLOCK,
  MSD_STATE_EJECTED
} msd_state_t;

typedef enum {
  USB_MSD_DRIVER_UNINITIALIZED = 0,
  USB_MSD_DRIVER_ERROR,
  USB_MSD_DRIVER_OK,
  USB_MSD_DRIVER_STOPPED,
  USB_MSD_DRIVER_ERROR_BLK_DEV_NOT_READY,
} usb_msd_driver_state_t;

typedef struct USBMassStorageDriver USBMassStorageDriver;

struct USBMassStorageDriver {
    /* Driver Setup Data */
	USBDriver                 *usbp;
	BaseBlockDevice *bbdp;
	event_source_t evt_connected;
	event_source_t evt_ejected;
	BlockDeviceInfo block_dev_info;
	bool block_dev_info_valid_flag;
	usb_msd_driver_state_t driver_state;
    usbep_t  ms_ep_number;
    uint16_t msd_interface_number;
    bool (*enable_msd_callback)(void);
    bool (*suspend_threads_callback)(void);

    /* Externally modifiable settings */
    bool enable_media_removial;
    bool disable_usb_bus_disconnect_on_eject;
    BaseSequentialStream *chp; /*For debug logging*/

    /*Internal data for operation of the driver */
    binary_semaphore_t bsem;
    binary_semaphore_t usb_transfer_thread_bsem;
    binary_semaphore_t mass_sorage_thd_bsem;
    volatile uint32_t trigger_transfer_index;

    volatile bool bulk_in_interupt_flag;
    volatile bool bulk_out_interupt_flag;

    struct {
      scsi_read_format_capacity_response_t format_capacity_response;
      scsi_read_capacity10_response_t read_capacity10_response;
      scsi_mode_sense6_response_t mode_sense6_response;
      uint8_t max_lun_len_buf[1];
      scsi_inquiry_response_t scsi_inquiry_response;
    } data;

    msd_state_t state;
    msd_cbw_t cbw;
    msd_csw_t csw;
    scsi_sense_response_t sense;

    volatile bool reconfigured_or_reset_event;

    bool command_succeeded_flag;
    bool stall_in_endpoint;
    bool stall_out_endpoint;


    /*Debugging Information*/
    volatile uint32_t read_error_count;
    volatile uint32_t write_error_count;
    volatile uint32_t read_success_count;
    volatile uint32_t write_success_count;
    char *msd_thread_state;
    char *transfer_thread_state;
    char *scsi_command_state;
    volatile uint8_t last_bad_scsi_command;

    /* Externally readable values */
    volatile bool debug_enable_msd;
    volatile msd_wait_mode_t debug_wait_for_isr;

    THD_WORKING_AREA(waMassStorage, MSD_THREAD_STACK_SIZE);
    THD_WORKING_AREA(waMassStorageUSBTransfer, MSD_THREAD_STACK_SIZE);
};


#ifdef __cplusplus
extern "C" {
#endif
usb_msd_driver_state_t msdInit(USBDriver *usbp, BaseBlockDevice *bbdp, USBMassStorageDriver *msdp, const usbep_t ms_ep_number, const uint16_t msd_interface_number);
usb_msd_driver_state_t msdStart(USBMassStorageDriver *msdp);
usb_msd_driver_state_t msdStop(USBMassStorageDriver *msdp);
void msdBulkInCallbackComplete(USBDriver *usbp, usbep_t ep);
void msdBulkOutCallbackComplete(USBDriver *usbp, usbep_t ep);
bool msdRequestsHook(USBDriver *usbp);
bool msdRequestsHook2(USBDriver *usbp, USBMassStorageDriver *msdp);
const char* usb_msd_driver_state_t_to_str(const usb_msd_driver_state_t driver_state);
#ifdef __cplusplus
}
#endif


#endif /* HAL_USE_MASS_STORAGE_USB */

#endif /* _USB_MSD_H_ */
