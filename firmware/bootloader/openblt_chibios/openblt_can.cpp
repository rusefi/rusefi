#include "pch.h"

#include "hal.h"

#include "can_hw.h"

extern "C" {
	#include "boot.h"
}

// CAN1 PB8+PB9 and CAN2 PB5+PB6 pins are commonly used by Hellen.
// CAN2 PB5+PB13 pins can be used for ST-bootloader compatibility.
//
// Other STM32 CAN pin combinations:
// CAN1_RX: { PI9, PA11, PH14, PD0, PB8 }, CAN1_TX: { PA12, PH13, PD1, PB9 }
// CAN2_RX: { PB5, PB12 }, CAN2_TX: { PB6, PB13 }

#ifndef BOOT_COM_CAN_CHANNEL_INDEX
  #error BOOT_COM_CAN_CHANNEL_INDEX is not defined.
#elif (BOOT_COM_CAN_CHANNEL_INDEX == 0)
  #ifndef STM32_CAN_USE_CAN1
  #error STM32_CAN_USE_CAN1 is not enabled for CAN index 0
  #endif
  #undef OPENBLT_CAND
  #define OPENBLT_CAND CAND1
#elif (BOOT_COM_CAN_CHANNEL_INDEX == 1)
  #ifndef STM32_CAN_USE_CAN2
  #error STM32_CAN_USE_CAN2 is not enabled for CAN index 1
  #endif
  #undef OPENBLT_CAND
  #define OPENBLT_CAND CAND2
#else
  #error Unknown BOOT_COM_CAN_CHANNEL_INDEX.
#endif

#if !defined(OPENBLT_CAN_RX_PIN) || !defined(OPENBLT_CAN_RX_PORT) || !defined(OPENBLT_CAN_TX_PIN) || !defined(OPENBLT_CAN_TX_PORT)
#if (BOOT_COM_CAN_CHANNEL_INDEX == 0)
  // default pins for CAN1 (compatible with Hellen)
  #define OPENBLT_CAN_RX_PORT GPIOB
  #define OPENBLT_CAN_RX_PIN 8
  #define OPENBLT_CAN_TX_PORT GPIOB
  #define OPENBLT_CAN_TX_PIN 9
#elif (BOOT_COM_CAN_CHANNEL_INDEX == 1)
  // default pins for CAN2 (compatible with ST-bootloader)
  #define OPENBLT_CAN_RX_PORT GPIOB
  #define OPENBLT_CAN_RX_PIN 5
  #define OPENBLT_CAN_TX_PORT GPIOB
  #define OPENBLT_CAN_TX_PIN 13
#endif
#endif

extern const CANConfig *findCanConfig(can_baudrate_e rate);

/************************************************************************************//**
** \brief     Initializes the CAN controller and synchronizes it to the CAN bus.
** \return    none.
**
****************************************************************************************/
extern "C" void CanInit(void) {
	// init pins
	palSetPadMode(OPENBLT_CAN_TX_PORT, OPENBLT_CAN_TX_PIN, PAL_MODE_ALTERNATE(EFI_CAN_TX_AF));
	palSetPadMode(OPENBLT_CAN_RX_PORT, OPENBLT_CAN_RX_PIN, PAL_MODE_ALTERNATE(EFI_CAN_RX_AF));

	auto cfg = findCanConfig(B500KBPS);
	canStart(&OPENBLT_CAND, cfg);
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
		frame.IDE = CAN_IDE_STD;
	}
	else
	{
		txMsgId &= ~0x80000000;
		/* set the 29-bit CAN identifier. */
		frame.EID = txMsgId;
		frame.IDE = CAN_IDE_EXT;
	}

	// Copy data/DLC
	frame.DLC = len;
	memcpy(frame.data8, data, len);

	canTransmitTimeout(&OPENBLT_CAND, CAN_ANY_MAILBOX, &frame, TIME_MS2I(100));
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
	CANRxFrame frame;

	if (MSG_OK != canReceiveTimeout(&OPENBLT_CAND, CAN_ANY_MAILBOX, &frame, TIME_IMMEDIATE)) {
		// no message was waiting
		return BLT_FALSE;
	}

	// Check that the ID type matches this frame (std vs ext)
	constexpr bool configuredAsExt = (rxMsgId & 0x80000000) != 0;
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
