/**
 * @file	can_msg_tx.cpp
 *
 * CAN message transmission
 * 
 * @date Mar 13, 2020
 * @author Matthew Kennedy, (c) 2012-2020
 */

#include "efifeatures.h"
#include "global.h"

#if EFI_CAN_SUPPORT
#include "can_msg_tx.h"


/*static*/ CANDriver* CanTxMessage::s_device = nullptr;

/*static*/ void CanTxMessage::setDevice(CANDriver* device) {
	s_device = device;
}

CanTxMessage::CanTxMessage(uint32_t eid) {
	m_frame.IDE = CAN_IDE_STD;
	m_frame.EID = eid;
	m_frame.RTR = CAN_RTR_DATA;
	m_frame.DLC = 8;
	memset(m_frame.data8, 0, sizeof(m_frame.data8));
}

CanTxMessage::~CanTxMessage() {
	auto device = s_device;

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
