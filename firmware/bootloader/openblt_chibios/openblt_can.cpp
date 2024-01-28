#include "pch.h"

#include "hal.h"

#include "can_hw.h"

extern "C" {
	#include "boot.h"
}

/************************************************************************************//**
** \brief     Initializes the CAN controller and synchronizes it to the CAN bus.
** \return    none.
**
****************************************************************************************/
extern "C" void CanInit(void)
{
	// TODO: init pins?

	auto cfg = findCanConfig(B500KBPS);
	canStart(&CAND1, cfg);
}


/************************************************************************************//**
** \brief     Transmits a packet formatted for the communication interface.
** \param     data Pointer to byte array with data that it to be transmitted.
** \param     len  Number of bytes that are to be transmitted.
** \return    none.
**
****************************************************************************************/
extern "C" void CanTransmitPacket(blt_int8u *data, blt_int8u len)
{
	blt_int32u txMsgId = BOOT_COM_CAN_TX_MSG_ID;
	CANTxFrame frame;

	if ((txMsgId & 0x80000000) == 0)
	{
		/* set the 11-bit CAN identifier. */
		frame.SID = txMsgId;
		frame.IDE = false;
	}
	else
	{
		txMsgId &= ~0x80000000;
		/* set the 29-bit CAN identifier. */
		frame.EID = txMsgId & ~0x80000000; // negate the ID-type bit
		frame.IDE = true;
	}

	// Copy data/DLC
	frame.DLC = len;
	memcpy(frame.data8, data, len);

	canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &frame, TIME_MS2I(100));
}

/************************************************************************************//**
** \brief     Receives a communication interface packet if one is present.
** \param     data Pointer to byte array where the data is to be stored.
** \param     len Pointer where the length of the packet is to be stored.
** \return    BLT_TRUE is a packet was received, BLT_FALSE otherwise.
**
****************************************************************************************/
extern "C" blt_bool CanReceivePacket(blt_int8u *data, blt_int8u *len)
{
	constexpr blt_int32u rxMsgId = BOOT_COM_CAN_RX_MSG_ID;
	blt_bool result = BLT_FALSE;
	CANRxFrame frame;

	if (MSG_OK != canReceiveTimeout(&CAND1, CAN_ANY_MAILBOX, &frame, TIME_IMMEDIATE)) {
		// no message was waiting
		return BLT_FALSE;
	}

	// Check that the ID type matches this frame (std vs ext)
	constexpr bool configuredAsExt = (rxMsgId & 0x80000000) == 0;
	if (configuredAsExt != frame.IDE) {
		// Wrong frame type
		return BLT_FALSE;
	}

	// Check that the frame's ID matches
	if (frame.IDE) {
		if (frame.EID != (rxMsgId & ~0x80000000)) {
			// Wrong ID
			return BLT_FALSE;
		}
	} else {
		if (frame.SID != rxMsgId) {
			// Wrong ID
			return BLT_FALSE;
		}
	}

	// Copy data and length out
	*len = frame.DLC;
	memcpy(data, frame.data8, frame.DLC);

	return BLT_TRUE;
}
