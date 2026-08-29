#include "pch.h"

#include "hal.h"

#include "can.h"
#include "can_hw.h"

extern "C" {
	#include "shared_params.h"
}

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
	#if defined(STM32_CAN_USE_CAN1) || defined(STM32_CAN_USE_FDCAN1)
		#define OPENBLT_CAND CAND1
	#else
		#error STM32_CAN_USE_CAN1/STM32_CAN_USE_FDCAN1 is not enabled for CAN index 0
	#endif
#elif (BOOT_COM_CAN_CHANNEL_INDEX == 1)
	#if defined(STM32_CAN_USE_CAN2) || defined(STM32_CAN_USE_FDCAN2)
		#define OPENBLT_CAND CAND2
	#else
		#error STM32_CAN_USE_CAN2/STM32_CAN_USE_FDCAN2 is not enabled for CAN index 1
	#endif
#else
	#error Unknown BOOT_COM_CAN_CHANNEL_INDEX.
#endif

#ifndef CAN_IDE_STD
#define CAN_IDE_STD 0
#endif

#ifndef CAN_IDE_EXT
#define CAN_IDE_EXT 1
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

// ---------------------------------------------------------------------
// rusEFI extension: runtime CAN baudrate switch (XCP_CMD_SET_CAN_BAUDRATE).
//
// The switch is implemented as a REBOOT, not a runtime canStop/canStart:
// the runtime restart path wedges the CAN peripheral on the AT32 port
// (observed on the bench - the link dies at both speeds and only a power
// cycle recovers). The boot-time CanInit is the only exercised path.
//
// Flow: SET_CAN_BAUDRATE is answered at the old speed; the main loop stores
// the requested rate in SharedParams slot 4 and resets; CanInit applies it
// on the next boot. A 5 s no-traffic timer at 1 Mbit reboots back to 500k,
// so the ECU can never be stranded at a speed the host does not follow.
// ---------------------------------------------------------------------
static can_baudrate_e currentBaudrate = B500KBPS;
static can_baudrate_e requestedBaudrate = B500KBPS;
static systime_t baudSwitchTime;
static volatile blt_bool canTrafficSinceSwitch = BLT_FALSE;

/** Boot-time baudrate request: set by the bootloader main() from
 *  SharedParams slot 4 before BootInit(), applied by CanInit(). */
blt_int8u bootBaudrateRequest = 0;

/**
 * Called from xcp.c when SET_CAN_BAUDRATE arrives. Only records the request;
 * the main loop reboots into it after the response left the wire.
 */
extern "C" void XcpSetCanBaudrateHook(blt_int8u rate) {
	requestedBaudrate = (rate == 1) ? B1MBPS : B500KBPS;
}

/**
 * Applies a pending baudrate request by rebooting into it, and provides the
 * safety fallback: if no valid traffic arrives for 5 s at 1 Mbit, reboot
 * back to 500k so the console/car bus always recovers by itself.
 */
extern "C" void OpenBltCanApplyBaudrate(void) {
	if (requestedBaudrate != currentBaudrate) {
		/* the SET_CAN_BAUDRATE response has been transmitted; store the
		 * request, let the frame leave the wire, and reboot into it */
		SharedParamsWriteByIndex(4, (requestedBaudrate == B1MBPS) ? 1 : 0);
		chThdSleepMilliseconds(5);
		NVIC_SystemReset();
	} else if ((currentBaudrate != B500KBPS) &&
			(canTrafficSinceSwitch == BLT_FALSE) &&
			(TIME_I2MS(chVTGetSystemTime() - baudSwitchTime) > 5000)) {
		/* no traffic at the switched speed: reboot back to the default */
		SharedParamsWriteByIndex(4, 0);
		NVIC_SystemReset();
	}
}

/************************************************************************************//**
** \brief     Initializes the CAN controller and synchronizes it to the CAN bus.
** \return    none.
**
****************************************************************************************/
extern "C" void CanInit(void) {
	/* init pins */
	palSetPadMode(OPENBLT_CAN_TX_PORT, OPENBLT_CAN_TX_PIN, PAL_MODE_ALTERNATE(EFI_CAN_TX_AF));
	palSetPadMode(OPENBLT_CAN_RX_PORT, OPENBLT_CAN_RX_PIN, PAL_MODE_ALTERNATE(EFI_CAN_RX_AF));

	currentBaudrate = (bootBaudrateRequest == 1) ? B1MBPS : B500KBPS;
	requestedBaudrate = currentBaudrate;
	auto cfg = findCanConfig(currentBaudrate);

	/* Program a default accept-all filter BEFORE canStart(). On bxCAN the
	 * reset state has every filter bank inactive (FA1R=0), so without this
	 * call the controller receives nothing at all - the bootloader never
	 * sees the host's CONNECT and no CAN update can ever start. The app
	 * does the same in can_hw.cpp; the bootloader just never did.
	 */
	canSTM32SetFilters(&OPENBLT_CAND, STM32_CAN_MAX_FILTERS / 2, 0, NULL);

	canStart(&OPENBLT_CAND, cfg);

	baudSwitchTime = chVTGetSystemTime();
	canTrafficSinceSwitch = BLT_FALSE;
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
	CANTxFrame frame = {};

	if ((txMsgId & 0x80000000) == 0)
	{
		/* set the 11-bit CAN identifier. */
		CAN_SID(frame) = txMsgId;
		CAN_ISX(frame) = CAN_IDE_STD;
	}
	else
	{
		txMsgId &= ~0x80000000;
		/* set the 29-bit CAN identifier. */
		CAN_EID(frame) = txMsgId;
		CAN_ISX(frame) = CAN_IDE_EXT;
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

#ifdef BOOTLOADER_CAN_LISTENER
extern void boardCanListener(CANRxFrame *frame);
#endif

extern "C" blt_bool CanReceivePacket(blt_int8u *data, blt_int8u *len)
{
	constexpr blt_int32u rxMsgId = BOOT_COM_CAN_RX_MSG_ID;
	CANRxFrame frame;

	if (MSG_OK != canReceiveTimeout(&OPENBLT_CAND, CAN_ANY_MAILBOX, &frame, TIME_IMMEDIATE)) {
		// no message was waiting
		return BLT_FALSE;
	}

	// The baudrate-fallback timer is reset only by a frame that actually
	// passed the ID/type validation below - garbage decoded at a mismatched
	// bus speed must NOT count as traffic, otherwise the 5 s fallback to
	// 500k would never fire while the host hammers the wrong speed.

	// Check that the ID type matches this frame (std vs ext)
	constexpr bool configuredAsExt = (rxMsgId & 0x80000000) != 0;
	if (configuredAsExt != CAN_ISX(frame)) {
		// Wrong frame type
		goto wrong;
	}

	// Check that the frame's ID matches
	if (CAN_ISX(frame)) {
		if (CAN_EID(frame) != (rxMsgId & ~0x80000000)) {
			// Wrong ID
			goto wrong;
		}
	} else {
		if (CAN_SID(frame) != rxMsgId) {
			// Wrong ID
			goto wrong;
		}
	}

	// Copy data and length out
	*len = frame.DLC;
	memcpy(data, frame.data8, frame.DLC);

	canTrafficSinceSwitch = BLT_TRUE;

	return BLT_TRUE;

wrong:

#ifdef BOOTLOADER_CAN_LISTENER
	boardCanListener(&frame);
#endif

	return BLT_FALSE;
}
