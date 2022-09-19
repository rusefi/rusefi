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

#if (HAL_USE_CAN == TRUE) || defined(__DOXYGEN__)

#include <linux/can.h>
#include <net/if.h>
#include <sys/ioctl.h>

#include <cstring>
#include <queue>

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

	CAND1.sock = -1;
}

static std::vector<CANDriver*> instances;

/**
 * @brief   Configures and activates the CAN peripheral.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @notapi
 */
void can_lld_start(CANDriver *canp) {
	(void)canp;

	// Check that this device is not already started
	osalDbgCheck(canp->sock <= 0);

	// Check that a name is set
	osalDbgCheck(canp->deviceName != nullptr);

	// create socket
	int sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);

	if (sock < 0) {
		// TODO: handle
		return;
	}

	sockaddr_can addr;

	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;

	{
		// Determine index of the CAN device with the requested name
		ifreq ifr;
		strcpy(ifr.ifr_name, canp->deviceName);
		ioctl(sock, SIOCGIFINDEX, &ifr);
		addr.can_ifindex = ifr.ifr_ifindex;
	}

	if (bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
		// TODO: handle
		return;
	}

	canp->sock = sock;

	// Initialize the rx queue
	canp->rx = new std::queue<can_frame>;

	// Add this instance so it will have receive listened to by the "interrupt handler"
	instances.push_back(canp);

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

	// Remove from the "interrupt handler" list
	std::remove(instances.begin(), instances.end(), canp);

	// Close the socket.
	close(canp->sock);
	canp->sock = -1;

	// Free the rx queue
	delete reinterpret_cast<std::queue<can_frame>*>(canp->rx);
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

	// The queue is practically infinitely deep, so it is always safe to call can_lld_transmit.
	// Therefore, just return whether or not the CAN interface actually got initialized
	return canp->sock >= 0;
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

	can_frame frame;

	memcpy(frame.data, ctfp->data8, 8);
	frame.can_dlc = ctfp->DLC;

	frame.can_id = ctfp->IDE ? ctfp->EID : ctfp->SID;
	// bit 31 is 1 for extended, 0 for standard
	frame.can_id |= ctfp->IDE ? (1 << 31) : 0;

	int res = write(canp->sock, &frame, sizeof(frame));

	if (res != sizeof(frame)) {
		// TODO: handle err
		return;
	}
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
	(void)mailbox;

	// CAN init failed, claim that the queue is full.
	if (canp->sock < 0) {
		return false;
	}

	return !reinterpret_cast<std::queue<can_frame>*>(canp->rx)->empty();
}

bool check_can_isr() {
	bool intOccured = false;

	for (auto canp : instances) {
		can_frame frame;

		// nonblocking read so it fails instantly in case no frame is queued
		int result = recv(canp->sock, &frame, sizeof(frame), MSG_DONTWAIT);

		// no frame received, nothing to do
		if (result != sizeof(frame)) {
			continue;
		}

		intOccured = true;

		CH_IRQ_PROLOGUE();
		reinterpret_cast<std::queue<can_frame>*>(canp->rx)->push(frame);
		_can_rx_full_isr(canp, 0);
		CH_IRQ_EPILOGUE();
	}

	return intOccured;
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

	auto queue = reinterpret_cast<std::queue<can_frame>*>(canp->rx);

	can_frame frame = queue->front();
	queue->pop();

	crfp->DLC = frame.can_dlc;

	memcpy(crfp->data8, frame.data, crfp->DLC);

	// If <8 byte packet, pad with zeroes to avoid spilling stack state or garbage data from the returned frame
	if (crfp->DLC < 8) {
		memset(crfp->data8 + crfp->DLC, 0, 8 - crfp->DLC);
	}

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
