/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    CANv2/hal_can_lld.h
 * @brief   Cypress S6E2Cx CAN subsystem low level driver header.
 * @author  andreika <prometheus.pcb@gmail.com>
 *
 * @addtogroup CAN
 * @{
 */

#ifndef HAL_CAN_LLD_H
#define HAL_CAN_LLD_H

#if HAL_USE_CAN || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   This switch defines whether the driver implementation supports
 *          a low power switch mode with automatic an wakeup feature.
 */
#define CAN_SUPPORTS_SLEEP          TRUE

/**
 * @brief   This implementation supports three transmit mailboxes.
 */
#define CAN_TX_MAILBOXES            3 // < CANFD_MESSAGE_TXBUFFER_COUNT

/**
 * @brief   This implementation supports two receive mailboxes.
 */
#define CAN_RX_MAILBOXES            3 // < CANFD_MESSAGE_RXBUFFER_COUNT

// check mailbox counts
#if CAN_TX_MAILBOXES > CANFD_MESSAGE_TXBUFFER_COUNT
#error CAN_TX_MAILBOXES > CANFD_MESSAGE_TXBUFFER_COUNT
#endif

#if CAN_RX_MAILBOXES > CANFD_MESSAGE_RXBUFFER_COUNT
#error CAN_RX_MAILBOXES > CANFD_MESSAGE_RXBUFFER_COUNT
#endif

#define CAN_IDE_STD                 0           /**< @brief Standard id.    */
#define CAN_IDE_EXT                 1           /**< @brief Extended id.    */

#define CAN_RTR_DATA                0           /**< @brief Data frame.     */
#define CAN_RTR_REMOTE              1           /**< @brief Remote frame.   */
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   CAN pedantic errors report.
 * @details Use of this option is IRQ-intensive.
 */
#if !defined(CYPRESS_CAN_REPORT_ALL_ERRORS) || defined(__DOXYGEN__)
#define CYPRESS_CAN_REPORT_ALL_ERRORS         FALSE
#endif

/**
 * @brief   CAN0 driver enable switch.
 * @details If set to @p TRUE the support for CAN0 is included.
 */
#if !defined(CYPRESS_CAN_USE_CAN0) || defined(__DOXYGEN__)
#define CYPRESS_CAN_USE_CAN0                  FALSE
#endif

/**
 * @brief   CAN1 driver enable switch.
 * @details If set to @p TRUE the support for CAN1 is included.
 */
#if !defined(CYPRESS_CAN_USE_CAN1) || defined(__DOXYGEN__)
#define CYPRESS_CAN_USE_CAN1                  FALSE
#endif

/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !CYPRESS_CAN_USE_CAN0 && !CYPRESS_CAN_USE_CAN1
#error "CAN driver activated but no CAN peripheral assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing an CAN driver.
 */
typedef struct CANDriver CANDriver;

/**
 * @brief   Type of a transmission mailbox index.
 */
typedef uint32_t canmbx_t;

#if (CAN_ENFORCE_USE_CALLBACKS == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Type of a CAN notification callback.
 *
 * @param[in] canp      pointer to the @p CANDriver object triggering the
 *                      callback
 * @param[in] flags     flags associated to the mailbox callback
 */
typedef void (*can_callback_t)(CANDriver *canp, uint32_t flags);
#endif

/**
 * @brief   CAN transmission frame.
 * @note    Accessing the frame data as word16 or word32 is not portable because
 *          machine data endianness, it can be still useful for a quick filling.
 */
typedef struct {
  struct {
    uint8_t                 DLC:4;          /**< @brief Data length.        */
    uint8_t                 RTR:1;          /**< @brief Frame type.         */
    uint8_t                 IDE:1;          /**< @brief Identifier type.    */
  };
  union {
    struct {
      uint32_t              SID:11;         /**< @brief Standard identifier.*/
    };
    struct {
      uint32_t              EID:29;         /**< @brief Extended identifier.*/
    };
  };
  union {
    uint8_t                 data8[8];       /**< @brief Frame data.         */
    uint16_t                data16[4];      /**< @brief Frame data.         */
    uint32_t                data32[2];      /**< @brief Frame data.         */
    uint64_t                data64[1];      /**< @brief Frame data.         */
  };
} CANTxFrame;

/**
 * @brief   CAN received frame.
 * @note    Accessing the frame data as word16 or word32 is not portable because
 *          machine data endianness, it can be still useful for a quick filling.
 */
typedef struct {
  struct {
    uint8_t                 FMI;            /**< @brief Filter id.          */
    uint16_t                TIME;           /**< @brief Time stamp.         */
  };
  struct {
    uint8_t                 DLC:4;          /**< @brief Data length.        */
    uint8_t                 RTR:1;          /**< @brief Frame type.         */
    uint8_t                 IDE:1;          /**< @brief Identifier type.    */
  };
  union {
    struct {
      uint32_t              SID:11;         /**< @brief Standard identifier.*/
    };
    struct {
      uint32_t              EID:29;         /**< @brief Extended identifier.*/
    };
  };
  union {
    uint8_t                 data8[8];       /**< @brief Frame data.         */
    uint16_t                data16[4];      /**< @brief Frame data.         */
    uint32_t                data32[2];      /**< @brief Frame data.         */
    uint64_t                data64[1];      /**< @brief Frame data.         */
  };
} CANRxFrame;

/**
 * @brief   Driver configuration structure.
 */
typedef struct {
  /**
   * @brief   CAN MCR register initialization data.
   * @note    Some bits in this register are enforced by the driver regardless
   *          their status in this field.
   */
  uint32_t                  mcr;
  /**
   * @brief   CAN BTR register initialization data.
   * @note    Some bits in this register are enforced by the driver regardless
   *          their status in this field.
   */
  uint32_t                  btr;
} CANConfig;

/**
 * @brief   Structure representing an CAN driver.
 */
struct CANDriver {
  /**
   * @brief   Driver state.
   */
  canstate_t                state;
  /**
   * @brief   Current configuration data.
   */
  const CANConfig           *config;
  /**
   * @brief   Transmission threads queue.
   */
  threads_queue_t           txqueue;
  /**
   * @brief   Receive threads queue.
   */
  threads_queue_t           rxqueue;
#if (CAN_ENFORCE_USE_CALLBACKS == FALSE) || defined(__DOXYGEN__)
  /**
   * @brief   One or more frames become available.
   * @note    After broadcasting this event it will not be broadcasted again
   *          until the received frames queue has been completely emptied. It
   *          is <b>not</b> broadcasted for each received frame. It is
   *          responsibility of the application to empty the queue by
   *          repeatedly invoking @p canReceive() when listening to this event.
   *          This behavior minimizes the interrupt served by the system
   *          because CAN traffic.
   * @note    The flags associated to the listeners will indicate which
   *          receive mailboxes become non-empty.
   */
  event_source_t            rxfull_event;
  /**
   * @brief   One or more transmission mailbox become available.
   * @note    The flags associated to the listeners will indicate which
   *          transmit mailboxes become empty.
   * @note    The upper 16 bits are transmission error flags associated
   *          to the transmit mailboxes.
   */
  event_source_t            txempty_event;
  /**
   * @brief   A CAN bus error happened.
   * @note    The flags associated to the listeners will indicate that
   *          receive error(s) have occurred.
   * @note    In this implementation the upper 16 bits are filled with the
   *          unprocessed content of the ESR register.
   */
  event_source_t            error_event;
#if CAN_USE_SLEEP_MODE || defined (__DOXYGEN__)
  /**
   * @brief   Entering sleep state event.
   */
  event_source_t            sleep_event;
  /**
   * @brief   Exiting sleep state event.
   */
  event_source_t            wakeup_event;
#endif /* CAN_USE_SLEEP_MODE */
#else /* CAN_ENFORCE_USE_CALLBACKS == TRUE */
  /**
   * @brief   One or more frames become available.
   * @note    After calling this function it will not be called again
   *          until the received frames queue has been completely emptied. It
   *          is <b>not</b> called for each received frame. It is
   *          responsibility of the application to empty the queue by
   *          repeatedly invoking @p chTryReceiveI().
   *          This behavior minimizes the interrupt served by the system
   *          because CAN traffic.
   */
  can_callback_t            rxfull_cb;
  /**
   * @brief   One or more transmission mailbox become available.
   * @note    The flags associated to the callback will indicate which
   *          transmit mailboxes become empty.
   */
  can_callback_t            txempty_cb;
  /**
   * @brief   A CAN bus error happened.
   */
  can_callback_t            error_cb;
#if (CAN_USE_SLEEP_MODE == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief   Exiting sleep state.
   */
  can_callback_t            wakeup_cb;
#endif
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief   Pointer to the CAN registers.
   */
  stc_canfdn_t             *can;

  /**
   * @brief   PDL CAN config structure.
   */
  stc_canfd_config_t        stcCanfdConfig;

  /**
   * @brief   Used to control the buffers' emptiness.
   */
  bool                      is_rx_mailbox_occupied[CAN_RX_MAILBOXES];
  bool                      is_tx_mailbox_occupied[CAN_TX_MAILBOXES];
  
  /**
   * @brief   Temporary used to copy the received message from the interrupt handler to the caller (under the sys-lock).
   */
  stc_canfd_msg_t           stcRxMsg[CAN_RX_MAILBOXES];
  /**
   * @brief   Temporary used to send the message
   */
  stc_canfd_msg_t		    stcTxMsg;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if CYPRESS_CAN_USE_CAN0 || defined(__DOXYGEN__)
extern CANDriver CAND1;
#endif

#if CYPRESS_CAN_USE_CAN1 || defined(__DOXYGEN__)
extern CANDriver CAND2;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void can_lld_init(void);
  void can_lld_start(CANDriver *canp);
  void can_lld_stop(CANDriver *canp);
  bool can_lld_is_tx_empty(CANDriver *canp, canmbx_t mailbox);
  void can_lld_transmit(CANDriver *canp,
                        canmbx_t mailbox,
                        const CANTxFrame *crfp);
  bool can_lld_is_rx_nonempty(CANDriver *canp, canmbx_t mailbox);
  void can_lld_receive(CANDriver *canp,
                       canmbx_t mailbox,
                       CANRxFrame *ctfp);
  void can_lld_abort(CANDriver *canp,
                     canmbx_t mailbox);
#if CAN_USE_SLEEP_MODE
  void can_lld_sleep(CANDriver *canp);
  void can_lld_wakeup(CANDriver *canp);
#endif /* CAN_USE_SLEEP_MODE */

#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_CAN */

#endif /* HAL_CAN_LLD_H */

/** @} */
