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
 * @file    hal_can_lld.c
 * @brief   PLATFORM CAN subsystem low level driver source.
 *
 * @addtogroup CAN
 * @{
 */

#include "hal.h"

#include <string.h>
#include <linux/can.h>

#if (HAL_USE_CAN == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   CAN1 driver identifier.
 */
CANDriver CAND1;

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level CAN driver initialization.
 *
 * @notapi
 */
void can_lld_init(void) {
	/* Driver initialization.*/
	canObjectInit(&CAND1);
}

/**
 * @brief   Configures and activates the CAN peripheral.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @notapi
 */
void can_lld_start(CANDriver *canp) {
	(void)canp;

	// create socket
	canp->sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);

	if (canp->sock < 0) {
		// TODO: handle
		return;
	}

	struct sockaddr_can addr;

	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;

	// TODO: what index? is 0 right?
	addr.can_ifindex = 0;

	int result = bind(canp->sock, (struct sockaddr*)&addr, sizeof(addr));

	if (result < 0) {
		// TODO: handle
		return;
	}

	// TODO: can we even set bitrate from userspace?
}

/**
 * @brief   Deactivates the CAN peripheral.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @notapi
 */
void can_lld_stop(CANDriver *canp) {
	(void)canp;

	// TODO: close socket
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
	(void)canp;
	(void)mailbox;

	return true;
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
	(void)mailbox;

	if (canp->sock < 0) {
		return;
	}

	struct can_frame frame;

	memcpy(frame.data, ctfp->data8, 8);
	frame.can_dlc = ctfp->DLC;

	frame.can_id = ctfp->IDE ? ctfp->EID : ctfp->SID;
	// bit 31 is 1 for extended, 0 for standard
	frame.can_id |= ctfp->IDE << 31;

	write(canp->sock, &frame, sizeof(frame));
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
	(void)canp;
	(void)mailbox;

	return false;
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
	(void)mailbox;

	if (canp->sock < 0) {
		return;
	}

	struct can_frame frame;
	int nBytes = read(canp->sock, &frame, sizeof(frame));

	if (nBytes < 1) {
		return;
	}

	memcpy(crfp->data8, frame.data, 8);
	
	crfp->DLC = frame.can_dlc;

	// SID bits overlap with EID, no reason to copy both, but mask off err/rtr/etc bits
	crfp->EID = CAN_ERR_MASK & frame.can_id;

	crfp->IDE = (frame.can_id &  CAN_EFF_FLAG) != 0;
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
	(void)canp;
	(void)mailbox;
}

#endif /* HAL_USE_CAN == TRUE */

/** @} */
