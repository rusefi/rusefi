
#include "efifeatures.h"

#if EFI_CAN_SUPPORT

#include "can_msg_tx.h"

#include "global.h"
#include "engine.h"

#include "engine_configuration.h"
#include "mpu_util.h"

EXTERN_ENGINE;

CanTxMessage::CanTxMessage(uint32_t eid) {
	m_frame.IDE = CAN_IDE_STD;
	m_frame.EID = eid;
	m_frame.RTR = CAN_RTR_DATA;
	m_frame.DLC = 8;sss
	memset(m_frame.data8, 0, sizeof(m_frame.data8));
}

CanTxMessage::~CanTxMessage() {
	CANDriver *device = detectCanDevice(CONFIG(canRxPin),
			CONFIG(canTxPin));

	if (!device) {
		warning(CUSTOM_ERR_CAN_CONFIGURATION, "CAN configuration issue");
		return;
	}

	// 100 ms timeout
	canTransmit(device, CAN_ANY_MAILBOX, &m_frame, TIME_MS2I(100));
}

uint8_t& CanTxMessage::operator[](size_t index) {
	return m_frame.data8[index];
}

void CanTxMessage::setShortValue(uint16_t value, size_t offset) {
	m_frame.data8[offset] = value & 0xFF;
	m_frame.data8[offset + 1] = value >> 8;
}

void CanTxMessage::setBit(size_t byteIdx, size_t bitIdx) {
	m_frame.data8[byteIdx] |= 1 << bitIdx;
}

#endif // EFI_CAN_SUPPORT
