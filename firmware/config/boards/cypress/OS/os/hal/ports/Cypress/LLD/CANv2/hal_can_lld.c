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
 * @file    CANv2/hal_can_lld.c
 * @brief   Cypress S6E2Cx CAN subsystem low level driver header.
 * @author  andreika <prometheus.pcb@gmail.com>
 *
 * @addtogroup CAN
 * @{
 */

#include <string.h>

#include "hal.h"

//!!!!!!!!!!!!!!!!!!!!!!!
extern void toggleLed(int led, int mode);

#if HAL_USE_CAN || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define CAN_BUFFER_NOT_FOUND 0xff

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief CAN0 driver identifier.*/
#if CYPRESS_CAN_USE_CAN0 || defined(__DOXYGEN__)
CANDriver CAND1;
#endif

/** @brief CAN1 driver identifier.*/
#if CYPRESS_CAN_USE_CAN1 || defined(__DOXYGEN__)
CANDriver CAND2;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static uint8_t can_get_mailbox(canmbx_t mailbox, const bool *isOccupiedBuf, size_t bufSize, bool isFree) {
	uint8_t i;
	if (mailbox == CAN_ANY_MAILBOX) {
		for (i = 0; i < bufSize; i++) {
			// return the first unoccupied mailbox
			if ((!isOccupiedBuf[i] && isFree) || (isOccupiedBuf[i] && !isFree)) {
				return i;
			}
		}
		return CAN_BUFFER_NOT_FOUND;
	}
	return (uint8_t)mailbox - 1;
}

// called from interrupt
static void can_occupy_rx_mailboxI(CANDriver *can, uint8_t idx) {
	chSysLockFromISR();
	can->is_rx_mailbox_occupied[idx] = true;
	chSysUnlockFromISR();
}

// called from already locked PAL function
static uint8_t can_get_and_free_rx_mailbox(CANDriver *can, canmbx_t mailbox) {
	uint8_t idx = can_get_mailbox(mailbox, can->is_rx_mailbox_occupied, sizeof(can->is_rx_mailbox_occupied), false);
	if (idx != CAN_BUFFER_NOT_FOUND) {
		can->is_rx_mailbox_occupied[idx] = false;
	}
	return idx;
}

// called from already locked PAL function
static uint8_t can_get_tx_mailbox_and_occupy(CANDriver *can, canmbx_t mailbox) {
	uint8_t idx = can_get_mailbox(mailbox, can->is_tx_mailbox_occupied, sizeof(can->is_tx_mailbox_occupied), true);
	if (idx != CAN_BUFFER_NOT_FOUND) {
		can->is_tx_mailbox_occupied[idx] = true;
	}
	return idx;
}

// called from interrupt
static void can_free_tx_mailboxI(CANDriver *can, uint8_t u8MsgBuf) {
	chSysLockFromISR();
	can->is_tx_mailbox_occupied[u8MsgBuf] = false;
	chSysUnlockFromISR();
}

static void can_copy_message(stc_canfd_msg_t *pstcSrc, stc_canfd_msg_t *pstcDst) {
	uint8_t	u8Idx;

	// Copy if both source and destination pointers are valid.
	if (pstcSrc != NULL && pstcDst != NULL) {
		// Copy ID members.
		pstcDst->stcIdentifier.u32Identifier = pstcSrc->stcIdentifier.u32Identifier;
		pstcDst->stcIdentifier.bExtended     = pstcSrc->stcIdentifier.bExtended;

		// Copy data members.
		for (u8Idx = 0; u8Idx < CANFD_MESSAGE_DATA_BUFFER_SIZEW; u8Idx++) {
			pstcDst->stcData.au32Data[u8Idx] = pstcSrc->stcData.au32Data[u8Idx];
		}
		pstcDst->stcData.u8DataLengthCode = pstcSrc->stcData.u8DataLengthCode;

		// Copy extended DLC flag.
		pstcDst->bCanfd = pstcSrc->bCanfd;
	}
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if CYPRESS_CAN_USE_CAN0 || defined(__DOXYGEN__)

static void can_tx_complete_callback0(uint8_t u8MsgBuf) {
	
	//!!!!!!!!!!!!
	toggleLed(3, 1);
		
	can_free_tx_mailboxI(&CAND1, u8MsgBuf);
	/* Signaling flags and waking up threads waiting for a transmission slot.*/
	_can_tx_empty_isr(&CAND1, 0);
}

static void can_rx_received_callback0(uint8_t u8MsgBuf, stc_canfd_msg_t* pstcRxMsg) {

	//!!!!!!!!!!!!
	toggleLed(2, 1);

	can_occupy_rx_mailboxI(&CAND1, u8MsgBuf);
	can_copy_message(pstcRxMsg, &CAND1.stcRxMsg[u8MsgBuf]);

	_can_rx_full_isr(&CAND1, /*flags*/0);
}

static void can_status_callback0(en_canfd_status_t enCanfdStatus) {
	// BusOff state.
	if (enCanfdStatus == CanfdBusOff) {
		Canfd_Restart((volatile stc_canfdn_t*)(&CANFD0));
	}
}

static void can_error_callback0(uint32_t u32CanfdErrorFlags) {
	// todo: use u32CanfdErrorFlags
	(void)u32CanfdErrorFlags;

	//!!!!!!!!!!!!
	toggleLed(4, 1);

	_can_error_isr(&CAND1, CAN_OVERFLOW_ERROR);

	Canfd_Restart((volatile stc_canfdn_t*)(&CANFD0));
}

#endif /* CYPRESS_CAN_USE_CAN0 */

#if CYPRESS_CAN_USE_CAN1 || defined(__DOXYGEN__)

static void can_tx_complete_callback1(uint8_t u8MsgBuf) {
	can_free_tx_mailboxI(&CAND2, u8MsgBuf);
	/* Signaling flags and waking up threads waiting for a transmission slot.*/
	_can_tx_empty_isr(&CAND2, 0);
}

static void can_rx_received_callback1(uint8_t u8MsgBuf, stc_canfd_msg_t* pstcRxMsg) {
	can_occupy_rx_mailboxI(&CAND2, u8MsgBuf);
	can_copy_message(pstcRxMsg, &CAND2.stcRxMsg[u8MsgBuf]);

	_can_rx_full_isr(&CAND2, /*flags*/0);
}

static void can_status_callback1(en_canfd_status_t enCanfdStatus) {
	// BusOff state.
	if (enCanfdStatus == CanfdBusOff) {
		Canfd_Restart((volatile stc_canfdn_t*)(&CANFD1));
	}
}

static void can_error_callback1(uint32_t u32CanfdErrorFlags) {
	// todo: use u32CanfdErrorFlags
	(void)u32CanfdErrorFlags;

	_can_error_isr(&CAND2, CAN_OVERFLOW_ERROR);

	Canfd_Restart((volatile stc_canfdn_t*)(&CANFD1));
}

#endif /* CYPRESS_CAN_USE_CAN1 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level CAN driver initialization.
 *
 * @notapi
 */
void can_lld_init(void) {

#if CYPRESS_CAN_USE_CAN0
  /* Driver initialization.*/
  canObjectInit(&CAND1);
  CAND1.can = (stc_canfdn_t *)(&CANFD0);
#endif

#if CYPRESS_CAN_USE_CAN1
  /* Driver initialization.*/
  canObjectInit(&CAND2);
  CAND2.can = (stc_canfdn_t *)(&CANFD1);
#endif
}

/**
 * @brief   Configures and activates the CAN peripheral.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @notapi
 */
void can_lld_start(CANDriver *canp) {

  memset(&canp->stcCanfdConfig, 0, sizeof(canp->stcCanfdConfig));

#if CYPRESS_CAN_USE_CAN0
  if (&CAND1 == canp) {
  	canp->can = (stc_canfdn_t*)&CANFD0;
    canp->stcCanfdConfig.pfnReceiveMsgCallback  = can_rx_received_callback0;
    canp->stcCanfdConfig.pfnTransmitMsgCallback = can_tx_complete_callback0;
    canp->stcCanfdConfig.pfnStatusCallback      = can_status_callback0;
    canp->stcCanfdConfig.pfnErrorCallback       = can_error_callback0;
  }
#endif

#if CYPRESS_CAN_USE_CAN1
  if (&CAND2 == canp) {
  	canp->can = (stc_canfdn_t*)&CANFD1;
    canp->stcCanfdConfig.pfnReceiveMsgCallback  = can_rx_received_callback1;
    canp->stcCanfdConfig.pfnTransmitMsgCallback = can_tx_complete_callback1;
    canp->stcCanfdConfig.pfnStatusCallback      = can_status_callback1;
    canp->stcCanfdConfig.pfnErrorCallback       = can_error_callback1;
  }
#endif

  memset(canp->is_rx_mailbox_occupied, 0, sizeof(canp->is_rx_mailbox_occupied));
  memset(canp->is_tx_mailbox_occupied, 0, sizeof(canp->is_tx_mailbox_occupied));

  // Configuring CAN
  Canpre_Init(CanfdPreDiv16);	// =1/6 (192MHz / 6 = 32MHz), see CYPRESS_PLL_FREQ

  // Set configuration parameters of CAN FD.
  canp->stcCanfdConfig.enCanfdMode = CanfdModeClassic;
  canp->stcCanfdConfig.enCanfdClock = CanfdClock32MHz;	// CAN operation clock: 32MHz

  // Initialize CAN FD.
  en_result_t res = Canfd_Init(canp->can, &canp->stcCanfdConfig);

  osalDbgAssert(res == Ok, "CAN: Canfd_Init failed");

  Canfd_Start(canp->can);
}

/**
 * @brief   Deactivates the CAN peripheral.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @notapi
 */
void can_lld_stop(CANDriver *canp) {
  /* If in ready state then disables the CAN peripheral.*/
  if (canp->state == CAN_READY) {
    Canfd_Stop(canp->can);
  }
}

/**
 * @brief   Determines whether a frame can be transmitted.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] mailbox   mailbox number, @p CAN_ANY_MAILBOX for any mailbox
 *
 * @return              The queue space availability.
 * @retval false        no space in the transmit queue.
 * @retval true         transmit slot available.
 *
 * @notapi
 */
bool can_lld_is_tx_empty(CANDriver *canp, canmbx_t mailbox) {
	uint8_t idx = can_get_mailbox(mailbox, canp->is_tx_mailbox_occupied, sizeof(canp->is_tx_mailbox_occupied), true);
	if (idx == CAN_BUFFER_NOT_FOUND)
		return false;
    return !canp->is_tx_mailbox_occupied[idx];
}

/**
 * @brief   Inserts a frame into the transmit queue.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] ctfp      pointer to the CAN frame to be transmitted
 * @param[in] mailbox   mailbox number,  @p CAN_ANY_MAILBOX for any mailbox
 *
 * @notapi
 */
void can_lld_transmit(CANDriver *canp,
                      canmbx_t mailbox,
                      const CANTxFrame *ctfp) {
	int i;

	// Request to send
	uint8_t bufIdx = can_get_tx_mailbox_and_occupy(canp, mailbox);
	// if all buffers are occupied, we cannot send
	if (bufIdx == CAN_BUFFER_NOT_FOUND)
		return;

	canp->stcTxMsg.stcIdentifier.bExtended = ctfp->IDE;
	canp->stcTxMsg.stcIdentifier.u32Identifier = ctfp->IDE ? ctfp->EID : ctfp->SID;
	// copy the data
	for (i = 0; i < ctfp->DLC; i++)
		canp->stcTxMsg.stcData.au8Data[i] = ctfp->data8[i];
	// CanfdModeClassic
	canp->stcTxMsg.stcData.u8DataLengthCode = ctfp->DLC;
	canp->stcTxMsg.bCanfd = FALSE;

	Canfd_TransmitMsg(canp->can, bufIdx, &canp->stcTxMsg);
}

/**
 * @brief   Determines whether a frame has been received.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] mailbox   mailbox number, @p CAN_ANY_MAILBOX for any mailbox
 *
 * @return              The queue space availability.
 * @retval false        no space in the transmit queue.
 * @retval true         transmit slot available.
 *
 * @notapi
 */
bool can_lld_is_rx_nonempty(CANDriver *canp, canmbx_t mailbox) {
	uint8_t idx = can_get_mailbox(mailbox, canp->is_rx_mailbox_occupied, sizeof(canp->is_rx_mailbox_occupied), false);
	if (idx == CAN_BUFFER_NOT_FOUND)
		return false;
    return canp->is_rx_mailbox_occupied[idx];

}

/**
 * @brief   Receives a frame from the input queue.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] mailbox   mailbox number, @p CAN_ANY_MAILBOX for any mailbox
 * @param[out] crfp     pointer to the buffer where the CAN frame is copied
 *
 * @notapi
 */
void can_lld_receive(CANDriver *canp,
                     canmbx_t mailbox,
                     CANRxFrame *crfp) {
  int i;
  // we receive all messages from all mailboxes
  Canfd_ReceiveMsg(canp->can);
  Canfd_GetBusStatus(canp->can);

  uint8_t bufIdx = can_get_and_free_rx_mailbox(canp, mailbox);
  // no occupied buffers left
  if (bufIdx == CAN_BUFFER_NOT_FOUND)
  	return;

  stc_canfd_msg_t *stcRxMsg = &canp->stcRxMsg[bufIdx];
  
  // copy the frame
  crfp->IDE = stcRxMsg->stcIdentifier.bExtended;
  if (crfp->IDE)
    crfp->EID = stcRxMsg->stcIdentifier.u32Identifier;
  else
    crfp->SID = stcRxMsg->stcIdentifier.u32Identifier;
  crfp->DLC = stcRxMsg->stcData.u8DataLengthCode;
  // copy the data
  for (i = 0; i < crfp->DLC; i++)
    crfp->data8[i] = stcRxMsg->stcData.au8Data[i];

  // todo:
}

/**
 * @brief   Tries to abort an ongoing transmission.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] mailbox   mailbox number
 *
 * @notapi
 */
void can_lld_abort(CANDriver *canp,
                   canmbx_t mailbox) {
  /* tries, but fails.... someone, please implement this. */
}

#if CAN_USE_SLEEP_MODE || defined(__DOXYGEN__)
/**
 * @brief   Enters the sleep mode.
 * @param[in] canp      pointer to the @p CANDriver object
 */
void can_lld_sleep(CANDriver *canp) {
}

/**
 * @brief   Enforces leaving the sleep mode.
 * @param[in] canp      pointer to the @p CANDriver object
 */
void can_lld_wakeup(CANDriver *canp) {
}
#endif /* CAN_USE_SLEEP_MODE */

#endif /* HAL_USE_CAN */

/** @} */
