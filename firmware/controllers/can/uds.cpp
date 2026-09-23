#include "pch.h"
#include "uds.h"

#if EFI_UDS && (EFI_CAN_SUPPORT || EFI_UNIT_TEST)
#include "can_msg_tx.h"

void udsOnCanPacketRx(const CANRxFrame& frame, size_t busIndex) {
	// Only implement ReadDataByIdentifier for our own DID. In particular, do
	// not reject other services/DIDs that a board's diagnostic hook may handle.
	if (CAN_ISX(frame) || CAN_ISRTR(frame) || CAN_SID(frame) != 0x7E0
		|| frame.DLC < 4 || frame.DLC > 8 || (frame.data8[0] & 0xF0) != 0
		|| frame.data8[1] != 0x22 || frame.data8[2] != 0xF1 || frame.data8[3] != 0xA4) {
		return;
	}

	CanTxMessage response(CanCategory::OBD, 0x7E8, 8, busIndex);
	if (frame.data8[0] != 3) {
		const uint8_t invalidLength[] = {3, 0x7F, 0x22, 0x13, 0, 0, 0, 0};
		response.setArray(invalidLength);
		return;
	}

	// Private DID F1A4, exactly four ASCII bytes "rEFI". Keep the identity
	// independent of version, bootloader type, and application readiness.
	const uint8_t identity[] = {7, 0x62, 0xF1, 0xA4, 'r', 'E', 'F', 'I'};
	response.setArray(identity);
}
#endif
