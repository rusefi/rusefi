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

/**
 * @file    usb_msd.c
 * @brief   USB Mass Storage Driver code.
 *
 * @addtogroup MSD_USB
 * @{
 */

#define HAL_USE_MASS_STORAGE_USB true

#include "ch.h"
#include "hal.h"
#include "usb_msd.h"
#include "string.h"
#include "chprintf.h"


#if HAL_USE_MASS_STORAGE_USB || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define MSD_ENABLE_PERF_DEBUG_GPIOS    FALSE
#define MSD_DEBUG_NESTING              FALSE
#define MSD_DEBUG                      FALSE
//#define MSD_DEBUG                      (palReadPad(GPIOI, GPIOI_PIN4))

#define msd_debug_print(chp_arg, args ...) if (MSD_DEBUG && chp_arg != NULL ) { chprintf(chp_arg, args); }
#define msd_debug_nest_print(chp_arg, args ...) if ( MSD_DEBUG_NESTING && chp_arg != NULL ) { chprintf(chp_arg, args); }
#define msd_debug_err_print(chp_arg, args ...) if ( chp_arg != NULL ) { chprintf(chp_arg, args); }





//#define MSD_DEBUG_SCSI_COMMAND_STATE_STRING(description)    g_debug_info.msd.scsi_command_state = description;

#if !defined(MSD_RW_LED_ON)
#define MSD_RW_LED_ON()
#endif

#if !defined(MSD_RW_LED_OFF)
#define MSD_RW_LED_OFF()
#endif

#if !defined(MSD_R_LED_ON)
#define MSD_R_LED_ON()
#endif

#if !defined(MSD_R_LED_OFF)
#define MSD_R_LED_OFF()
#endif

#if !defined(MSD_W_LED_ON)
#define MSD_W_LED_ON()
#endif

#if !defined(MSD_W_LED_OFF)
#define MSD_W_LED_OFF()
#endif



/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/



static THD_FUNCTION(MassStorageUSBTransferThd, arg);
static THD_FUNCTION(MassStorageThd, arg);

static thread_t *msdThd = NULL;
static thread_t *msdUSBTransferThd = NULL;

#define WAIT_ISR_SUCCESS                     0
#define WAIT_ISR_BUSS_RESET_OR_RECONNECT     1

static uint8_t msdWaitForISR(USBMassStorageDriver *msdp, const bool check_reset, const msd_wait_mode_t wait_mode);
static void msdSetDefaultSenseKey(USBMassStorageDriver *msdp);

#define BLOCK_SIZE_INCREMENT                 512
#define BLOCK_WRITE_ITTERATION_COUNT         16


typedef enum {
  MSD_USB_TRANSFER_STATUS_RUNNING = 0,
  MSD_USB_TRANSFER_STATUS_DONE_SUCCESSFUL,
  MSD_USB_TRANSFER_STATUS_DONE_FAILED,
} msd_usb_transfer_status_t;

typedef struct {
  //uint8_t is_transfer_done;
  msd_usb_transfer_status_t transfer_status;
  /*Number of blocks actually read from USB IN endpoint that should be written to SD card*/
  int num_blocks_to_write;
  /*Number of blocks to read from USB IN endpoint*/
  int max_blocks_to_read;
  uint8_t buf[(BLOCK_SIZE_INCREMENT * BLOCK_WRITE_ITTERATION_COUNT)];
} rw_usb_sd_buffer_t;

static volatile rw_usb_sd_buffer_t rw_ping_pong_buffer[2];
static uint8_t read_buffer[2][BLOCK_SIZE_INCREMENT];


inline uint32_t swap_uint32( uint32_t val ) {
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
    return ((val << 16) & 0xFFFF0000) | ((val >> 16) & 0x0000FFFF);
}

#define swap_uint16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))

inline uint32_t swap_4byte_buffer(uint8_t *buff) {
  //Note: this is specifically to avoid pointer aliasing and de-referencing words on non-word boundaries
  uint32_t temp = 0;
  memcpy(&temp, buff, sizeof(temp));
  return(swap_uint32(temp));
}

inline uint16_t swap_2byte_buffer(uint8_t *buff) {
  //Note: this is specifically to avoid pointer aliasing and de-referencing words on non-half-word boundaries
  uint16_t temp = 0;
  memcpy(&temp, buff, sizeof(temp));
  return(swap_uint16(temp));
}

/**
 * @brief USB Event handler calback
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        USB Endpoint Number
 *
 * @api
 */
void msdBulkInCallbackComplete(USBDriver *usbp, usbep_t ep) {
  (void)usbp;
  (void)ep;

  if (ep > 0 && usbp->in_params[ep - 1] != NULL) {
    USBMassStorageDriver *msdp = (USBMassStorageDriver *)usbp->in_params[ep - 1];

    chSysLockFromISR();
    chBSemSignalI(&(msdp->bsem));

    msdp->bulk_in_interupt_flag = true;


    chSysUnlockFromISR();
  }
}

/**
 * @brief USB Event handler calback
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        USB Endpoint Number
 *
 * @api
 */
void msdBulkOutCallbackComplete(USBDriver *usbp, usbep_t ep) {
  (void)usbp;
  (void)ep;

  if (ep > 0 && usbp->in_params[ep - 1] != NULL) {
    USBMassStorageDriver *msdp = (USBMassStorageDriver *)usbp->in_params[ep - 1];

    chSysLockFromISR();
    chBSemSignalI(&(msdp->bsem));

    msdp->bulk_out_interupt_flag = true;

    chSysUnlockFromISR();
  }
}


/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/


/**
 * @brief This function will initialize a USBMassStorageDriver structure.
 *
 * @pre Upon entry, the BlockDevice state should be BLK_READY. If it's not BLK_READY, then this will
 * wait untile it becomes BLK_READY.
 *
 * @param[in] usbp            pointer to the @p USBDriver object
 * @param[in] bbdp            pointer to the @p BaseBlockDevice object, such as an SDCDriver object
 * @param[in] msdp            pointer to the @p USBMassStorageDriver object
 * @param[in] ms_ep_number    USB Endpoint Number to be used by the mass storage endpoint
 *
 * @init
 */
usb_msd_driver_state_t msdInit(USBDriver *usbp, BaseBlockDevice *bbdp, USBMassStorageDriver *msdp,
             const usbep_t ms_ep_number, const uint16_t msd_interface_number) {

  msdp->usbp = usbp;
  msdp->driver_state = USB_MSD_DRIVER_OK;
  msdp->state = MSD_STATE_IDLE;
  msdp->trigger_transfer_index = UINT32_MAX;
  msdp->bbdp = bbdp;
  msdp->ms_ep_number = ms_ep_number;
  msdp->msd_interface_number = msd_interface_number;
  msdp->chp = NULL;
  msdp->enable_media_removial = true;
  msdp->block_dev_info_valid_flag = false;

  chEvtObjectInit(&msdp->evt_connected);
  chEvtObjectInit(&msdp->evt_ejected);

  /* Initialize binary semaphore as taken, will cause the thread to initially
   * wait on the  */
  chBSemObjectInit(&msdp->bsem, TRUE);
  /* Initialize binary semaphore as NOT taken */
  chBSemObjectInit(&msdp->usb_transfer_thread_bsem, FALSE);
  chBSemObjectInit(&msdp->mass_sorage_thd_bsem, FALSE);

  /* Initialize sense structure to zero */
  memset(&msdp->sense, 0, sizeof(msdp->sense));

  /* Response code = 0x70, additional sense length = 0x0A */
  msdp->sense.byte[0] = 0x70;//FIXME use #define, what is this???
  msdp->sense.byte[7] = 0x0A;//FIXME use #define, what is this???

  /* make sure block device is working and get info */
  msdSetDefaultSenseKey(msdp);

  const uint32_t sleep_ms = 50;
  uint32_t t;
  for(t = 0; t <= 250 && (blkGetDriverState(bbdp) != BLK_READY); t += sleep_ms ) {
    chThdSleepMilliseconds(sleep_ms);
  }

  if( blkGetDriverState(bbdp) == BLK_READY ) {
    blkGetInfo(bbdp, &msdp->block_dev_info);
    msdp->block_dev_info_valid_flag = true;
  } else {
    //msdp->driver_state = USB_MSD_DRIVER_ERROR_BLK_DEV_NOT_READY;
  }

  usbp->in_params[ms_ep_number - 1] = (void *)msdp;

  return(msdp->driver_state);
}

/**
 * @brief Starts data handling threads for USB mass storage driver
 *
 * @param[in] msdp      pointer to the @p USBMassStorageDriver object
 *
 * @api
 */
usb_msd_driver_state_t msdStart(USBMassStorageDriver *msdp) {
  /*upon entry, USB bus should be disconnected*/

  if (msdThd == NULL ) {
    msdThd = chThdCreateStatic(msdp->waMassStorage, sizeof(msdp->waMassStorage), NORMALPRIO,
                               MassStorageThd, msdp);
  }

  if (msdUSBTransferThd == NULL  ) {
    msdUSBTransferThd = chThdCreateStatic(msdp->waMassStorageUSBTransfer,
                                          sizeof(msdp->waMassStorageUSBTransfer),
                                          NORMALPRIO, MassStorageUSBTransferThd,
                                          msdp);
  }

  return(msdp->driver_state);
}

usb_msd_driver_state_t msdStop(USBMassStorageDriver *msdp) {
  usb_msd_driver_state_t final_state = USB_MSD_DRIVER_STOPPED;

  if (msdThd != NULL) {
    chThdTerminate(msdThd);
    int i;
    for(i = 0; i < 20 && msdThd->p_state != CH_STATE_FINAL; i++ ) {
      chThdSleepMilliseconds(20);
    }

    if( msdThd->p_state == CH_STATE_FINAL ) {
      final_state = USB_MSD_DRIVER_ERROR;
    }
    msdThd = NULL;
  }


  if (msdUSBTransferThd == NULL) {
    chThdTerminate(msdUSBTransferThd);
    int i;
    for(i = 0; i < 20 && msdUSBTransferThd->p_state != CH_STATE_FINAL; i++ ) {
      chThdSleepMilliseconds(20);
    }

    if( msdUSBTransferThd->p_state == CH_STATE_FINAL ) {
      final_state = USB_MSD_DRIVER_ERROR;
    }
    msdUSBTransferThd = NULL;
  }


  msdp->driver_state = final_state;
  return(msdp->driver_state);
}


/**
 * @brief   Default requests hook.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @return              The hook status.
 * @retval TRUE         Message handled internally.
 * @retval FALSE        Message not handled.
 *
 * @api
 */
bool msdRequestsHook(USBDriver *usbp) {
  return(msdRequestsHook2(usbp, NULL));
}

/**
 * @brief   Alternate request hook, useful for USB composite devices
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] msdp      pointer to the @p USBMassStorageDriver object
 * @return              The hook status.
 * @retval TRUE         Message handled internally.
 * @retval FALSE        Message not handled.
 *
 * @api
 */
bool msdRequestsHook2(USBDriver *usbp, USBMassStorageDriver *msdp) {
  if (((usbp->setup[0] & USB_RTYPE_TYPE_MASK) == USB_RTYPE_TYPE_CLASS)
      && ((usbp->setup[0] & USB_RTYPE_RECIPIENT_MASK)
          == USB_RTYPE_RECIPIENT_INTERFACE)) {
    /* check that the request is for the MSD interface number.*/
    if( msdp != NULL ) {
      if (MSD_SETUP_INDEX(usbp->setup) != msdp->msd_interface_number)
        return FALSE;
    } else if (MSD_SETUP_INDEX(usbp->setup) != 0 ) {
        return FALSE;
    }

    /* act depending on bRequest = setup[1] */
    switch (usbp->setup[1]) {
      case MSD_REQ_RESET:
        /* check that it is a HOST2DEV request */
        if (((usbp->setup[0] & USB_RTYPE_DIR_MASK) != USB_RTYPE_DIR_HOST2DEV)
            || (MSD_SETUP_LENGTH(usbp->setup) != 0)
            || (MSD_SETUP_VALUE(usbp->setup) != 0))
          return FALSE;

        /* reset all endpoints */
        /* TODO!*/
        /* The device shall NAK the status stage of the device request until
         * the Bulk-Only Mass Storage Reset is complete.
         */
        return TRUE;
      case MSD_GET_MAX_LUN:
        /* check that it is a DEV2HOST request */
        if (((usbp->setup[0] & USB_RTYPE_DIR_MASK) != USB_RTYPE_DIR_DEV2HOST)
            || (MSD_SETUP_LENGTH(usbp->setup) != 1)
            || (MSD_SETUP_VALUE(usbp->setup) != 0))
          return FALSE;

        //static uint8_t len_buf[1] = {0};
        msdp->data.max_lun_len_buf[0] = 0;
        usbSetupTransfer(usbp, msdp->data.max_lun_len_buf, 1, NULL);
        return TRUE;
      default:
        return FALSE;
        break;
    }
  }
  return FALSE;
}


const char* usb_msd_driver_state_t_to_str(const usb_msd_driver_state_t driver_state) {
  switch (driver_state) {
    case USB_MSD_DRIVER_UNINITIALIZED:
      return ("USB_MSD_DRIVER_UNINITIALIZED");
    case USB_MSD_DRIVER_ERROR:
      return ("USB_MSD_DRIVER_ERROR");
    case USB_MSD_DRIVER_OK:
      return ("USB_MSD_DRIVER_OK");
    case USB_MSD_DRIVER_STOPPED:
      return("USB_MSD_DRIVER_STOPPED");
    case USB_MSD_DRIVER_ERROR_BLK_DEV_NOT_READY:
      return ("USB_MSD_DRIVER_ERROR_BLK_DEV_NOT_READY");
  }
  return ("USB_MSD_DRIVER_UNKNOWN");
}


/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/


/* Event Flow Functions */


static uint8_t msdWaitForISR(USBMassStorageDriver *msdp, const bool check_reset, const msd_wait_mode_t wait_mode) {
  uint8_t ret = WAIT_ISR_SUCCESS;
  /* sleep until the ISR completes */
  chSysLock();
  msd_debug_print(msdp->chp, "WaitISR(mode=%d)\r\n", wait_mode);
  for (;;) {
    const msg_t m = chBSemWaitTimeoutS(&msdp->bsem, 1);
    if (m == MSG_OK && wait_mode == MSD_WAIT_MODE_NONE ) {
      break;
    }

    if( wait_mode == MSD_WAIT_MODE_BULK_IN && msdp->bulk_in_interupt_flag ) {
      break;
    } else if( wait_mode == MSD_WAIT_MODE_BULK_OUT && msdp->bulk_out_interupt_flag ) {
      break;
    }

    if (check_reset && msdp->reconfigured_or_reset_event) {
      ret = WAIT_ISR_BUSS_RESET_OR_RECONNECT;
      break;
    }

    if( chThdShouldTerminateX() ) {
      break;
    }
  }
  chSysUnlock();
  return (ret);
}


static void WaitForUSBTransferComplete(USBMassStorageDriver *msdp,
                                       const int ping_pong_buffer_index) {
  msd_debug_nest_print(msdp->chp, "A");
  while (TRUE) {
    chBSemWaitTimeout(&msdp->mass_sorage_thd_bsem, MS2ST(1));

    if (rw_ping_pong_buffer[ping_pong_buffer_index].transfer_status != MSD_USB_TRANSFER_STATUS_RUNNING) {
      break;
    } else {
      //chThdSleepMilliseconds(1);
    }
  }
  msd_debug_nest_print(msdp->chp, "a");
}



/* SCSI Functions */

static inline void SCSISetSense(USBMassStorageDriver *msdp, uint8_t key,
                                uint8_t acode, uint8_t aqual) {
  msdp->sense.byte[2] = key;
  msdp->sense.byte[12] = acode;
  msdp->sense.byte[13] = aqual;
}


static void msdSetDefaultSenseKey(USBMassStorageDriver *msdp) {
  SCSISetSense(msdp, SCSI_SENSE_KEY_GOOD,
             SCSI_ASENSE_NO_ADDITIONAL_INFORMATION,
             SCSI_ASENSEQ_NO_QUALIFIER);
}


#ifndef USB_MASS_STORAGE_INQUIRY_RESPONSE_STRING_0
#  define USB_MASS_STORAGE_INQUIRY_RESPONSE_STRING_0  "Chibios"
#endif

#ifndef USB_MASS_STORAGE_INQUIRY_RESPONSE_STRING_1
#  define USB_MASS_STORAGE_INQUIRY_RESPONSE_STRING_1  "Mass Storage"
#endif

#ifndef USB_MASS_STORAGE_INQUIRY_RESPONSE_STRING_2
#  define USB_MASS_STORAGE_INQUIRY_RESPONSE_STRING_2  {'v', CH_KERNEL_MAJOR + '0', '.', CH_KERNEL_MINOR + '0'}
#endif

static const scsi_inquiry_response_t default_scsi_inquiry_response =
      {0x00, /* peripheral, direct access block device */
       0x80, /* removable */
       0x04, /* version, SPC-2 */
       0x02, /* response data format */
       0x20, /* additional_length, response has 0x20 + 4 bytes */
       0x00, /* sccstp*/
       0x00, /* bqueetc*/
       0x00, /* cmdqueue*/
       USB_MASS_STORAGE_INQUIRY_RESPONSE_STRING_0,
       USB_MASS_STORAGE_INQUIRY_RESPONSE_STRING_1,
       USB_MASS_STORAGE_INQUIRY_RESPONSE_STRING_2,
      };


static msd_wait_mode_t SCSICommandInquiry(USBMassStorageDriver *msdp) {
  msd_cbw_t *cbw = &(msdp->cbw);
  msdp->data.scsi_inquiry_response = default_scsi_inquiry_response;

  if ((cbw->scsi_cmd_data[1] & ((1 << 0) | (1 << 1))) || cbw->scsi_cmd_data[2]) {
    /* Optional but unsupported bits set - update the SENSE key and fail
     * the request */
    msd_debug_err_print(msdp->chp, " INQ ERR 0x%X 0x%X\r\n", cbw->scsi_cmd_data[1], cbw->scsi_cmd_data[2]);
    SCSISetSense(msdp, SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                 SCSI_ASENSE_INVALID_FIELD_IN_CDB, SCSI_ASENSEQ_NO_QUALIFIER);

    //we should indicate that the command failed to the host, but still return data
    msdp->command_succeeded_flag = false;
  }

  usbStartTransmitI(msdp->usbp, msdp->ms_ep_number, (uint8_t *)&msdp->data.scsi_inquiry_response,
                     sizeof(scsi_inquiry_response_t));

  /* wait for ISR */
  return MSD_WAIT_MODE_BULK_IN;
}

static msd_wait_mode_t SCSICommandRequestSense(USBMassStorageDriver *msdp) {
  //This command should not affect the sense key
  usbStartTransmitI(msdp->usbp, msdp->ms_ep_number, (uint8_t *)&msdp->sense,
                     sizeof(scsi_sense_response_t));

  /* wait for ISR */
  return MSD_WAIT_MODE_BULK_IN;
}

static msd_wait_mode_t SCSICommandReadFormatCapacity(USBMassStorageDriver *msdp) {
  msdSetDefaultSenseKey(msdp);

  const uint32_t formated_capactiy_descriptor_code = 0x02;//see usbmass-ufi10.doc for codes

  msdp->data.format_capacity_response.payload_byte_length[3] = 8;
  msdp->data.format_capacity_response.last_block_addr = swap_uint32(msdp->block_dev_info.blk_num - 1);
  msdp->data.format_capacity_response.block_size = swap_uint32(msdp->block_dev_info.blk_size) | formated_capactiy_descriptor_code;

  usbStartTransmitI(msdp->usbp, msdp->ms_ep_number, (uint8_t *)&msdp->data.format_capacity_response,
                     sizeof(msdp->data.format_capacity_response));

  /* wait for ISR */
  return MSD_WAIT_MODE_BULK_IN;
}

static msd_wait_mode_t SCSICommandReadCapacity10(USBMassStorageDriver *msdp) {
  msdSetDefaultSenseKey(msdp);

  msdp->data.read_capacity10_response.block_size = swap_uint32(msdp->block_dev_info.blk_size);
  msdp->data.read_capacity10_response.last_block_addr = swap_uint32(msdp->block_dev_info.blk_num - 1);

  usbStartTransmitI(msdp->usbp, msdp->ms_ep_number, (uint8_t *)&msdp->data.read_capacity10_response,
                     sizeof(msdp->data.read_capacity10_response));

  /* wait for ISR */
  return MSD_WAIT_MODE_BULK_IN;
}

static msd_wait_mode_t SCSICommandSendDiagnostic(USBMassStorageDriver *msdp) {
  msd_cbw_t *cbw = &(msdp->cbw);

  if ((!cbw->scsi_cmd_data[1]) & (1 << 2)) {
    /* Only self-test supported - update SENSE key and fail the command */
    SCSISetSense(msdp, SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                 SCSI_ASENSE_INVALID_FIELD_IN_CDB, SCSI_ASENSEQ_NO_QUALIFIER);

    msdp->command_succeeded_flag = false;
    return MSD_WAIT_MODE_NONE;
  }

  /* TODO: actually perform the test */

  /* don't wait for ISR */
  return MSD_WAIT_MODE_NONE;
}

static void SCSIWriteTransferPingPong(USBMassStorageDriver *msdp,
                                      volatile rw_usb_sd_buffer_t *dest_buffer) {
  msd_debug_nest_print(msdp->chp, "B");
  int cnt;
  dest_buffer->transfer_status = MSD_USB_TRANSFER_STATUS_RUNNING;
  dest_buffer->num_blocks_to_write = 0;

#if MSD_ENABLE_PERF_DEBUG_GPIOS
  palSetPad(GPIOH, GPIOH_LED2);
#endif

  for (cnt = 0;
      cnt < BLOCK_WRITE_ITTERATION_COUNT
          && cnt < dest_buffer->max_blocks_to_read; cnt++) {

    msdp->transfer_thread_state = "RX-Prep";
    usbStartReceiveI(msdp->usbp, msdp->ms_ep_number,
                      (uint8_t*)&dest_buffer->buf[cnt * BLOCK_SIZE_INCREMENT],
                      (msdp->block_dev_info.blk_size));

    msdp->transfer_thread_state = "RX";

    msdp->transfer_thread_state = "RX-Wait";
    msdWaitForISR(msdp, FALSE, MSD_WAIT_MODE_BULK_OUT);

    dest_buffer->num_blocks_to_write++;
  }
  dest_buffer->transfer_status = MSD_USB_TRANSFER_STATUS_DONE_SUCCESSFUL;
  msdp->transfer_thread_state = "RX-Done";
  //FIXME we need to handle setting the status to error if something failed, like a usb reset or something

#if MSD_ENABLE_PERF_DEBUG_GPIOS
  palClearPad(GPIOH, GPIOH_LED2);
#endif

  msd_debug_nest_print(msdp->chp, "b");
}


static msd_wait_mode_t SCSICommandStartReadWrite10(USBMassStorageDriver *msdp) {
  msd_cbw_t *cbw = &(msdp->cbw);
  int read_success;
  int retry_count;

  msdSetDefaultSenseKey(msdp);

  if ((cbw->scsi_cmd_data[0] == SCSI_CMD_WRITE_10) && blkIsWriteProtected(msdp->bbdp)) {
    msd_debug_err_print(msdp->chp, "\r\nWrite Protected!\r\n");
    /* device is write protected and a write has been issued */
    /* Block address is invalid, update SENSE key and return command fail */
    SCSISetSense(msdp, SCSI_SENSE_KEY_NOT_READY, SCSI_ASENSE_WRITE_PROTECTED,
                 SCSI_ASENSEQ_NO_QUALIFIER);

    msdp->command_succeeded_flag = false;
    msdp->stall_in_endpoint = true;
    return MSD_WAIT_MODE_NONE;
  }

  //FIXME, which of these?
  //uint32_t rw_block_address = swap_4byte_buffer(&cbw->scsi_cmd_data[2]);
  //const uint16_t total_blocks = swap_2byte_buffer(&cbw->scsi_cmd_data[7]);
  uint32_t rw_block_address = swap_uint32(*(uint32_t *)&cbw->scsi_cmd_data[2]);
  const uint16_t total_blocks = swap_uint16(*(uint16_t *)&cbw->scsi_cmd_data[7]);
  const uint32_t rw_block_address_origional = rw_block_address;
  uint16_t i = 0;

  if (rw_block_address >= msdp->block_dev_info.blk_num) {
    msd_debug_err_print(msdp->chp, "\r\nBlock Address too large %u > %u\r\n", rw_block_address, msdp->block_dev_info.blk_num);
    /* Block address is invalid, update SENSE key and return command fail */
    SCSISetSense(msdp, SCSI_SENSE_KEY_ILLEGAL_REQUEST, SCSI_ASENSE_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE,
                 SCSI_ASENSEQ_NO_QUALIFIER);

    msdp->command_succeeded_flag = false;
    msdp->stall_in_endpoint = true;

    /* don't wait for ISR */
    return MSD_WAIT_MODE_NONE;
  }

  /*initialized ping pong buffer*/
  for (i = 0; i < 2; i++) {
    rw_ping_pong_buffer[i].max_blocks_to_read = 0;
    rw_ping_pong_buffer[i].num_blocks_to_write = 0;
    rw_ping_pong_buffer[i].transfer_status = MSD_USB_TRANSFER_STATUS_RUNNING;
  }

  msd_debug_nest_print(msdp->chp, "\r\nG");
  if (cbw->scsi_cmd_data[0] == SCSI_CMD_WRITE_10) {
    /* loop over each block */

    uint32_t ping_pong_buffer_index = 0;
    /*initiate a transfer*/
    rw_ping_pong_buffer[ping_pong_buffer_index].transfer_status = MSD_USB_TRANSFER_STATUS_RUNNING;
    rw_ping_pong_buffer[ping_pong_buffer_index].max_blocks_to_read = total_blocks;

    /*Trigger the transfer in the other thread*/
    msdp->trigger_transfer_index = ping_pong_buffer_index;

    /*wake other thread on semaphore to trigger the transfer*/
    chBSemSignal(&msdp->usb_transfer_thread_bsem);

    WaitForUSBTransferComplete(msdp, ping_pong_buffer_index);

    for (i = 0; i < total_blocks;) {
      const int done_buffer_index = ping_pong_buffer_index;
      const int empty_buffer_index = ((ping_pong_buffer_index + 1) % 2);

      /*initiate another transfer in the other ping pong buffer*/
      //const bool queue_another_transfer = ((i + BLOCK_WRITE_ITTERATION_COUNT) < total_blocks);
      const bool queue_another_transfer = ((i + rw_ping_pong_buffer[done_buffer_index].num_blocks_to_write) < total_blocks);

      msd_debug_nest_print(msdp->chp, "D");
      if (queue_another_transfer) {
        while (TRUE) {
          if (msdp->trigger_transfer_index == UINT32_MAX) {
            rw_ping_pong_buffer[empty_buffer_index].max_blocks_to_read =
                total_blocks - i - rw_ping_pong_buffer[done_buffer_index].num_blocks_to_write;

            msdp->trigger_transfer_index = empty_buffer_index;

            /*wake other thread on semaphore to trigger the transfer*/
            chBSemSignal(&msdp->usb_transfer_thread_bsem);
            break;
          } else {
            chThdSleepMilliseconds(1);
          }
        }
      }
      msd_debug_nest_print(msdp->chp, "d");

      if (rw_ping_pong_buffer[done_buffer_index].num_blocks_to_write <= 0) {
        /*This should never happen!!! Something is seriously wrong!*/
        msd_debug_err_print(
              msdp->chp, "\r\nCant write 0 blocks, this should not happen, halting\r\n");
        chThdSleepMilliseconds(50);
        chSysHalt("MSD: Cant write 0 blocks");
      }

      /* now write the block to the block device */
      MSD_W_LED_ON();
      msd_debug_nest_print(msdp->chp, "E");
      if (blkWrite(msdp->bbdp, rw_block_address,
          (uint8_t*)rw_ping_pong_buffer[done_buffer_index].buf,
          rw_ping_pong_buffer[done_buffer_index].num_blocks_to_write)
          == HAL_FAILED) {
        msd_debug_err_print(msdp->chp, "\r\nSD Block Write Error\r\n");
        chThdSleepMilliseconds(50);
        msdp->write_error_count++;

        msdp->command_succeeded_flag = false;

        SCSISetSense(msdp, SCSI_SENSE_KEY_MEDIUM_ERROR,
                             SCSI_ASENSE_PEREPHERIAL_DEVICE_WRITE_FAULT,
                             SCSI_ASENSEQ_PEREPHERIAL_DEVICE_WRITE_FAULT);


#define OLD_WRITE_ERROR_HANDLING    FALSE
#if OLD_WRITE_ERROR_HANDLING
        /*
         * I think that this mode of error handling is incorrect, and was causing ACM0 usb device resets in the event of EMMC write errors.
         * I confirmed using the Beagle USB 480 analyzer that the host gets a failed staus, and retries the write to the given error block, at which point the block write succeeds for thist test case.
         * This code should be purged at some point
        */
        msdp->stall_out_endpoint = true;

        if (queue_another_transfer) {
          /*Let the previous queued transfer finish and ignore it.*/
          WaitForUSBTransferComplete(msdp, empty_buffer_index);
        }

        MSD_W_LED_OFF();
        return (MSD_WAIT_MODE_NONE);
#endif
      } else {
        msdp->write_success_count++;
      }
      msd_debug_nest_print(msdp->chp, "e");
      MSD_W_LED_OFF();

      rw_block_address += rw_ping_pong_buffer[done_buffer_index].num_blocks_to_write;
      i += rw_ping_pong_buffer[done_buffer_index].num_blocks_to_write;
      rw_ping_pong_buffer[done_buffer_index].transfer_status = MSD_USB_TRANSFER_STATUS_RUNNING;
      rw_ping_pong_buffer[done_buffer_index].num_blocks_to_write = 0;

      msd_debug_nest_print(msdp->chp, "F");
      if (queue_another_transfer) {
        if( !( i< total_blocks) ) {
          msd_debug_err_print(msdp->chp, "\r\nERROR: Queued another transfer but not going to rx the data\r\n");
        }
        WaitForUSBTransferComplete(msdp, empty_buffer_index);
      }
      msd_debug_nest_print(msdp->chp, "f");

      /*Swap the ping pong buffers*/
      ping_pong_buffer_index = empty_buffer_index;
    }

    if( i != total_blocks ) {
      msd_debug_err_print(msdp->chp, "\r\ni!=total_blocks, %u, %u\r\n", i, total_blocks);
    }

    msd_debug_nest_print(msdp->chp, "(%u,%u,%u)", rw_block_address_origional, total_blocks, i);

  } else {
    /* FIXME: For some reason, when doing a blkRead on a SanDisk 8g sd card, it takes 2.5ms to read a block, limiting
     * max read performance to about 200k/s. However, when using an 8g transcend SD card, we can get up to 2.0 megabytes/sec
     * read through put. What's the difference? configuration for the SD driver?
     */

    i = 0;
    /* read the first block from block device */
    read_success = FALSE;
    for (retry_count = 0; retry_count < 3; retry_count++) {
      if (blkRead(msdp->bbdp, rw_block_address, read_buffer[i % 2], 1)
          == HAL_FAILED) {
        msd_debug_err_print(msdp->chp, "\r\nSD Block Read Error: block # %u\r\n", rw_block_address);
        msdp->read_error_count++;
      } else {
        msdp->read_success_count++;
        if( retry_count > 0 ) {
          msd_debug_err_print(msdp->chp, "Successful Block Read Retry\r\n");
        }
        read_success = TRUE;
        break;
      }
    }


    if ((!read_success) ) {
      msd_debug_err_print(msdp->chp, "\r\nSD Block Read Error 1, breaking read sequence, block # %u\r\n", rw_block_address);

      /*wait for printing to finish*/
      chThdSleepMilliseconds(10);

      msdp->command_succeeded_flag = false;
      msdp->stall_in_endpoint = true;

      msd_debug_err_print(
            msdp->chp, "\r\nSetting sense code %u\r\n", SCSI_SENSE_KEY_MEDIUM_ERROR);

      SCSISetSense(msdp, SCSI_SENSE_KEY_MEDIUM_ERROR,
                   SCSI_ASENSE_UNRECOVERED_READ_ERROR,
                   SCSI_ASENSEQ_NO_QUALIFIER);

      return MSD_WAIT_MODE_NONE;
    }

    rw_block_address++;

    /* loop over each block */
    for (i = 0; i < total_blocks; i++) {

      /* transmit the block */
      //while (usbGetTransmitStatusI(msdp->usbp, msdp->ms_ep_number)) {
          //wait for the prior transmit to complete
      //}
      usbStartTransmitI(msdp->usbp, msdp->ms_ep_number, read_buffer[i % 2],
                         msdp->block_dev_info.blk_size);

      if (i < (total_blocks - 1)) {
        /* there is at least one more block to be read from device */
        /* so read that while the USB transfer takes place */
        read_success = FALSE;
        MSD_R_LED_ON();
        for (retry_count = 0; retry_count < 3; retry_count++) {
          if (blkRead(msdp->bbdp, rw_block_address, read_buffer[(i+1) % 2], 1)
              == HAL_FAILED) {
            msd_debug_err_print(msdp->chp, "\r\nSD Block Read Error 2: block # %u\r\n", rw_block_address);

            msdp->read_error_count++;
          } else {
            if( retry_count > 0 ) {
              msd_debug_err_print(msdp->chp, "Successful Block Read Retry: block # %u\r\n", rw_block_address);
            }
            read_success = TRUE;
            msdp->read_success_count++;
            break;
          }
        }
        MSD_R_LED_OFF();

        if( !read_success ) {
          msd_debug_err_print(
                msdp->chp, "\r\nSD Block Read Error 22, addr=%d, halting\r\n", rw_block_address);

          /*wait for printing to finish*/
          chThdSleepMilliseconds(70);

          msdp->command_succeeded_flag = false;
          msdp->stall_in_endpoint = true;

          msd_debug_err_print(
                msdp->chp, "\r\nSetting sense code %u\r\n", SCSI_SENSE_KEY_MEDIUM_ERROR);

          SCSISetSense(msdp, SCSI_SENSE_KEY_MEDIUM_ERROR,
                       SCSI_ASENSE_UNRECOVERED_READ_ERROR,
                       SCSI_ASENSEQ_NO_QUALIFIER);
          return MSD_WAIT_MODE_NONE;
        }

        rw_block_address++;
      }

      /*FIXME In the event that the USB connection is unplugged while we're waiting for a bulk
       * endpoint ISR, this will never return, and when re-plugged into the host, the drive will
       * not show back up on the host. We need a way to break out of this loop when disconnected from the bus.
       */

      if (msdWaitForISR(msdp, TRUE, MSD_WAIT_MODE_BULK_IN) == WAIT_ISR_BUSS_RESET_OR_RECONNECT) {
        //fixme are we handling the reset case properly
        return MSD_WAIT_MODE_NONE;
      }
    }
  }
  msd_debug_nest_print(msdp->chp, "g");

  /* don't wait for ISR */
  return MSD_WAIT_MODE_NONE;
}

static msd_wait_mode_t SCSICommandStartStopUnit(USBMassStorageDriver *msdp) {
  scsi_start_stop_unit_request_t *ssu =
      (scsi_start_stop_unit_request_t *)&(msdp->cbw.scsi_cmd_data);

  if ((ssu->loej_start & 0x3) == 2) {
    /* device has been ejected */
    if (!msdp->disable_usb_bus_disconnect_on_eject) {
      chEvtBroadcast(&msdp->evt_ejected);
      msdp->state = MSD_STATE_EJECTED;
    }
  }

  /* don't wait for ISR */
  return MSD_WAIT_MODE_NONE;
}

static msd_wait_mode_t SCSICommandPreventAllowMediumRemovial(USBMassStorageDriver *msdp) {
  msd_cbw_t *cbw = &(msdp->cbw);

  if( (cbw->scsi_cmd_data[4] & 0x01) ) {
    //prohibit media removal
    if( msdp->enable_media_removial ) {
      //this can have positive performance
      msdp->command_succeeded_flag = false;
      SCSISetSense(msdp, SCSI_SENSE_KEY_ILLEGAL_REQUEST,
          SCSI_ASENSE_INVALID_COMMAND, SCSI_ASENSEQ_NO_QUALIFIER);
    }
  }

  /* don't wait for ISR */
  return MSD_WAIT_MODE_NONE;
}



static msd_wait_mode_t SCSICommandModeSense6(USBMassStorageDriver *msdp) {
  //FIXME check for unsupported mode page set sense code, see page 161(144) of USB Mass storage book
  memset(&msdp->data.mode_sense6_response, 0, sizeof(msdp->data.mode_sense6_response));
  msdp->data.mode_sense6_response.mode_data_length = 3;
  if( blkIsWriteProtected(msdp->bbdp) ) {
    msdp->data.mode_sense6_response.device_specifc_paramters |= (1<<7);
  }

  usbStartTransmitI(msdp->usbp, msdp->ms_ep_number, (uint8_t*)&msdp->data.mode_sense6_response, 4);

  /* wait for ISR */
  return MSD_WAIT_MODE_BULK_IN;
}

static msd_wait_mode_t msdWaitForCommandBlock(USBMassStorageDriver *msdp) {
  usbStartReceiveI(msdp->usbp, msdp->ms_ep_number, (uint8_t *)&msdp->cbw,
                    sizeof(msd_cbw_t));

  msdp->state = MSD_STATE_READ_CMD_BLOCK;

  return(MSD_WAIT_MODE_BULK_OUT);/* wait for ISR */
}

/*  */
/**
 * @brief   A command block has been received

 *
 * @param[in] p1        description of parameter one
 * @param[out] p2       description of parameter two
 * @param[in,out] p3    description of parameter three
 * @return              Description of the returned value, must be omitted if
 *                      a function returns void.
 * @retval TRUE         On success
 * @retval FALSE        On failure
 *
 */
static msd_wait_mode_t msdProcessCommandBlock(USBMassStorageDriver *msdp) {
  msd_cbw_t *cbw = &(msdp->cbw);

  /* by default transition back to the idle state */
  msdp->state = MSD_STATE_IDLE;

  msd_debug_print(msdp->chp, " CMD 0x%X\r\n", cbw->scsi_cmd_data[0]);
  msdp->command_succeeded_flag = true;
  msdp->stall_in_endpoint = false;
  msdp->stall_out_endpoint = false;
  msd_wait_mode_t wait_mode = MSD_WAIT_MODE_NONE;


  /* check the command */
  if ((cbw->signature != MSD_CBW_SIGNATURE) || (cbw->lun > 0)
      || ((cbw->data_len > 0) && (cbw->flags & 0x1F))
      || (cbw->scsi_cmd_len == 0) || (cbw->scsi_cmd_len > 16))
  {

    msdp->scsi_command_state = "Bad CBW";
    msd_debug_err_print(msdp->chp, "Bad CBW, sig=0x%X, lun=0x%X, data_len=0x%X, flags=0x%X, scsi_cmd_len=0x%X\r\n", cbw->signature, cbw->lun, cbw->data_len, cbw->flags, cbw->scsi_cmd_len);
    /* stall both IN and OUT endpoints */
    msdp->stall_out_endpoint = true;
    msdp->stall_in_endpoint = true;
    msdp->command_succeeded_flag = false;
    SCSISetSense(msdp, SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                 SCSI_ASENSE_INVALID_FIELD_IN_CDB, SCSI_ASENSEQ_NO_QUALIFIER);

#if 0
    chSysLock();
    msdp->bulk_out_interupt_flag = false;
    usbStallReceiveI(msdp->usbp, msdp->ms_ep_number);
    //usbStallTransmitI(msdp->usbp, msdp->ms_ep_number);
    chSysUnlock();

    //wait for the host to clear the stall
    msdWaitForISR(msdp, TRUE, MSD_WAIT_MODE_BULK_OUT);

    /* don't wait for ISR */
    return MSD_WAIT_MODE_NONE;
#endif
  }


  if( msdp->command_succeeded_flag ) {
    switch ( (msd_scsi_command_t) cbw->scsi_cmd_data[0] ) {
      case SCSI_CMD_INQUIRY:
        msdp->scsi_command_state = "CMD_INQ";
        msd_debug_print(msdp->chp, "CMD_INQ\r\n");
        wait_mode = SCSICommandInquiry(msdp);
        msdp->scsi_command_state = "CMD_INQ-Done";
        break;
      case SCSI_CMD_REQUEST_SENSE_6:
        msdp->scsi_command_state = "CMD_RS";
        msd_debug_print(msdp->chp, "\r\nCMD_RS\r\n");
        wait_mode = SCSICommandRequestSense(msdp);
        msdp->scsi_command_state = "CMD_RS-Done";
        break;
      case SCSI_CMD_READ_FORMAT_CAPACITY:
        msdp->scsi_command_state = "CMD_RFC";
        msd_debug_print(msdp->chp, "CMD_RFC\r\n");
        wait_mode = SCSICommandReadFormatCapacity(msdp);
        msdp->scsi_command_state = "CMD_RFC-Done";
        break;
      case SCSI_CMD_READ_CAPACITY_10:
        msdp->scsi_command_state = "CMD_RC10";
        msd_debug_print(msdp->chp, "CMD_RC10\r\n");
        wait_mode = SCSICommandReadCapacity10(msdp);
        msdp->scsi_command_state = "CMD_RC10-Done";
        break;
      case SCSI_CMD_READ_10:
      case SCSI_CMD_WRITE_10:
        msdp->scsi_command_state = "CMD_RW";
        msd_debug_print(msdp->chp, "CMD_RW\r\n");
        MSD_RW_LED_ON();
        wait_mode = SCSICommandStartReadWrite10(msdp);
        MSD_RW_LED_OFF();
        msdp->scsi_command_state = "CMD_RW-Done";
        break;
      case SCSI_CMD_SEND_DIAGNOSTIC:
        msdp->scsi_command_state = "CMD_DIA";
        msd_debug_print(msdp->chp, "CMD_DIA\r\n");
        wait_mode = SCSICommandSendDiagnostic(msdp);
        msdp->scsi_command_state = "CMD_DIA-Done";
        break;
      case SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL:
        msdp->scsi_command_state = "CMD_PAMR";
        msd_debug_print(msdp->chp, "CMD_PAMR\r\n");
        wait_mode = SCSICommandPreventAllowMediumRemovial(msdp);
        msdp->scsi_command_state = "CMD_PAMR-Done";
        break;
      case SCSI_CMD_TEST_UNIT_READY:
      case SCSI_CMD_VERIFY_10:
        msdp->scsi_command_state = "CMD_00_1E_2F";
        msd_debug_print(msdp->chp, "CMD_00_1E_2F\r\n");
        msdp->scsi_command_state = "CMD_00_1E_2F-Done";
        /* don't handle */
        break;
      case SCSI_CMD_MODE_SENSE_6:
        msdp->scsi_command_state = "CMD_S6";
        msd_debug_print(msdp->chp, "\r\nCMD_S6\r\n");
        wait_mode = SCSICommandModeSense6(msdp);
        msdp->scsi_command_state = "CMD_S6-Done";
        break;
      case SCSI_CMD_START_STOP_UNIT:
        msdp->scsi_command_state = "CMD_STOP";
        msd_debug_print(msdp->chp, "CMD_STOP\r\n");
        wait_mode = SCSICommandStartStopUnit(msdp);
        msdp->scsi_command_state = "CMD_STOP-Done";
        break;
      case SCSI_CMD_SYNCHRONIZE_CACHE_10:
        msdp->scsi_command_state = "SYNC_10";
        msd_debug_print(msdp->chp, "SYNC_10\r\n");
        //FIXME impliment this and flush data to the MMC card, Linux sends this command. We are implicitly synchronized
        //in our writes since we never leave data in RAM

        SCSISetSense(msdp, SCSI_SENSE_KEY_GOOD,
                     SCSI_ASENSE_NO_ADDITIONAL_INFORMATION, SCSI_ASENSEQ_NO_QUALIFIER);
        wait_mode = MSD_WAIT_MODE_NONE;

        msdp->scsi_command_state = "SYNC_10-Done";
        break;
      default:
        msdp->last_bad_scsi_command = cbw->scsi_cmd_data[0];

        msdp->scsi_command_state = "CMD_Def";
        msd_debug_err_print(msdp->chp, "CMD Unknown: 0x%X, using default CMD handler\r\n", cbw->scsi_cmd_data[0]);
        msdp->command_succeeded_flag = false;
        SCSISetSense(msdp, SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                     SCSI_ASENSE_INVALID_COMMAND, SCSI_ASENSEQ_NO_QUALIFIER);

#if 0
        /* stall IN endpoint */
        chSysLock()
        msdp->bulk_in_interupt_flag = false;
        usbStallTransmitI(msdp->usbp, msdp->ms_ep_number);
        chSysUnlock()

        msdWaitForISR(msdp, TRUE, MSD_WAIT_MODE_BULK_IN);
#else
        //msdp->stall_out_endpoint = true;
        //msdp->stall_in_endpoint = true;
#endif

        msdp->scsi_command_state = "CMD_Def-Done";
        cbw->data_len = 0;
#if 0
        return MSD_WAIT_MODE_NONE;
#endif
    }
  }

  cbw->data_len = 0;


  if( msdp->stall_in_endpoint || msdp->stall_out_endpoint ) {
    msdp->scsi_command_state = "Stall";
    /* stall IN endpoint */
    chSysLock();
    if( msdp->stall_in_endpoint ) {
      msd_debug_err_print(msdp->chp, "stalling IN endpoint\r\n");
      msdp->bulk_in_interupt_flag = false;
      usbStallTransmitI(msdp->usbp, msdp->ms_ep_number);
    }
    if( msdp->stall_out_endpoint ) {
      msd_debug_err_print(msdp->chp, "stalling OUT endpoint\r\n");
      msdp->bulk_out_interupt_flag = false;
      usbStallReceiveI(msdp->usbp, msdp->ms_ep_number);
    }
    chSysUnlock();

    if( msdp->stall_in_endpoint ) {
      msdWaitForISR(msdp, TRUE, MSD_WAIT_MODE_BULK_IN);
    }

    if( msdp->stall_out_endpoint ) {
      msdWaitForISR(msdp, TRUE, MSD_WAIT_MODE_BULK_OUT);
    }
  }

  msdp->scsi_command_state = "Wait";

  if (wait_mode != MSD_WAIT_MODE_NONE ) {
    msd_debug_nest_print(msdp->chp, "H");
    if (msdWaitForISR(msdp, TRUE, wait_mode) == WAIT_ISR_BUSS_RESET_OR_RECONNECT) {
      msd_debug_nest_print(msdp->chp, "h");
      return (MSD_WAIT_MODE_NONE);
    }
    msd_debug_nest_print(msdp->chp, "h");
  }

  msdp->scsi_command_state = "Wait-Done";

  msd_csw_t *csw = &(msdp->csw);


  csw->status = (msdp->command_succeeded_flag) ? MSD_COMMAND_PASSED : MSD_COMMAND_FAILED;
  csw->signature = MSD_CSW_SIGNATURE;
  csw->data_residue = cbw->data_len;
  csw->tag = cbw->tag;

  msdp->scsi_command_state = "TX";
  msd_debug_nest_print(msdp->chp, "I");
  usbStartTransmitI(msdp->usbp, msdp->ms_ep_number, (uint8_t *)csw,
                     sizeof(msd_csw_t));

  chSysLock();
  msdp->bulk_out_interupt_flag = false;
  msdWaitForCommandBlock(msdp);

  msdp->bulk_in_interupt_flag = false;
  chSysUnlock();
  msd_debug_nest_print(msdp->chp, "i");

  msdWaitForISR(msdp, TRUE, MSD_WAIT_MODE_BULK_IN);//wait for our status to be sent back

  msdp->scsi_command_state = "Done All";

  /* wait on ISR */
  //return MSD_WAIT_MODE_BULK_IN;
  return MSD_WAIT_MODE_BULK_OUT;
}





/*===========================================================================*/
/* Threads                                                   */
/*===========================================================================*/

/**
 * @brief   This thread is responsible for triggering a USB write of date
 *          from the MCU to the host. It is run as a separate thread to allow
 *          for concurrent RXing of data and writing of data to the SD card to
 *          thus significantly improve performance.
 *
 * @param[in] arg       pointer to the @p USBMassStorageDriver object
 *
 * @special
 */
THD_FUNCTION(MassStorageUSBTransferThd, arg) {
  USBMassStorageDriver *msdp = (USBMassStorageDriver *)arg;

  chRegSetThreadName("MSD-Transfer");

  while ( !chThdShouldTerminateX() ) {
    if (msdp->suspend_threads_callback != NULL && msdp->suspend_threads_callback()) {
      /* Suspend the thread for power savings mode */
      chSysLock();
      chSchGoSleepS(CH_STATE_SUSPENDED);
      chSysUnlock();
    }

    if (msdp->trigger_transfer_index != UINT32_MAX) {
      msdp->transfer_thread_state = "PP";
      SCSIWriteTransferPingPong(
          msdp, &rw_ping_pong_buffer[msdp->trigger_transfer_index]);
      msdp->trigger_transfer_index = UINT32_MAX;
      /*notify other thread*/
      chBSemSignal(&msdp->mass_sorage_thd_bsem);
    }

    chBSemWaitTimeout(&msdp->usb_transfer_thread_bsem, MS2ST(1));
  }

}



THD_FUNCTION(MassStorageThd, arg) {
  USBMassStorageDriver *msdp = (USBMassStorageDriver *)arg;
  chRegSetThreadName("MSD");

  msd_wait_mode_t wait_for_isr = MSD_WAIT_MODE_NONE;

  /* wait for the usb to be initialized */
  msd_debug_print(msdp->chp, "Y");
  msdWaitForISR(msdp, FALSE, MSD_WAIT_MODE_NONE);
  msd_debug_print(msdp->chp, "y");

  while ( !chThdShouldTerminateX() ) {

#if 0
    if( msdp->suspend_threads_callback != NULL && msdp->suspend_threads_callback() ) {
      /* Suspend the thread for power savings mode */
      chSysLock();
      chSchGoSleepS(CH_STATE_SUSPENDED);
      chSysUnlock();
    }
#endif


    wait_for_isr = MSD_WAIT_MODE_NONE;

    if (msdp->reconfigured_or_reset_event) {
      /*If the devices is unplugged and re-plugged but did not have a CPU reset,
       * we must set the state back to idle.*/
      msdp->reconfigured_or_reset_event = FALSE;
      msdp->state = MSD_STATE_IDLE;

      msdSetDefaultSenseKey(msdp);
    }

    bool enable_msd = true;
    if (msdp->enable_msd_callback != NULL) {
      enable_msd = msdp->enable_msd_callback();
    }

    if( msdp->driver_state != USB_MSD_DRIVER_OK ) {
      enable_msd = false;
    }
    msdp->debug_enable_msd = enable_msd;

    if ( enable_msd ) {

      if( ! msdp->block_dev_info_valid_flag ) {
        if( blkGetDriverState(msdp->bbdp) == BLK_READY ) {
            blkGetInfo(msdp->bbdp, &msdp->block_dev_info);
            msdp->block_dev_info_valid_flag = true;
        }
      }

      msd_debug_print(msdp->chp, "state=%d\r\n", msdp->state);
      /* wait on data depending on the current state */
      switch (msdp->state) {
        case MSD_STATE_IDLE:
          msdp->msd_thread_state = "IDL";
          msd_debug_print(msdp->chp, "IDL");
          wait_for_isr = msdWaitForCommandBlock(msdp);
          msd_debug_print(msdp->chp, "x\r\n");
          break;
        case MSD_STATE_READ_CMD_BLOCK:
          msdp->msd_thread_state = "RCB";
          msd_debug_print(msdp->chp, "RCB");
          wait_for_isr = msdProcessCommandBlock(msdp);
          msd_debug_print(msdp->chp, "x\r\n");
          break;
        case MSD_STATE_EJECTED:
          /* disconnect usb device */
          msd_debug_print(msdp->chp, "ejected\r\n");
          if (!msdp->disable_usb_bus_disconnect_on_eject) {
            msdp->msd_thread_state = "Ejected";
            chThdSleepMilliseconds(70);
            usbDisconnectBus(msdp->usbp);
            usbStop(msdp->usbp);
            chThdExit(0);
          }
      }
    }

    msdp->debug_wait_for_isr = wait_for_isr;

    if( enable_msd ) {
      msd_debug_nest_print(msdp->chp, "L");
    } else {
      msd_debug_nest_print(msdp->chp, "M");
    }

    if (wait_for_isr) {
      msd_debug_nest_print(msdp->chp, "J");
    } else {
      msd_debug_nest_print(msdp->chp, "K");
    }

    if (wait_for_isr && (!msdp->reconfigured_or_reset_event)) {
      /* wait until the ISR wakes thread */
      msd_debug_print(msdp->chp, "W%d,%d", wait_for_isr, msdp->state);
      msdWaitForISR(msdp, TRUE, wait_for_isr);
      msd_debug_print(msdp->chp, "w\r\n");
    } else if( ! enable_msd ) {
      chThdSleepMilliseconds(5);
    }


    if (wait_for_isr) {
      msd_debug_nest_print(msdp->chp, "j");
    } else {
      msd_debug_nest_print(msdp->chp, "k");
    }
  }

}





#endif /* HAL_USE_MASS_STORAGE_USB */
