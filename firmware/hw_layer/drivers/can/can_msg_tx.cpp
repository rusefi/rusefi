/**
 * @file	can_msg_tx.cpp
 *
 * CAN message transmission
 *
 * @date Mar 13, 2020
 * @author Matthew Kennedy, (c) 2012-2020
 */

#include "pch.h"

#include "can_msg_tx.h"
#include "auto_generated_can_category.h"

#include "can.h"

#if EFI_SIMULATOR || EFI_UNIT_TEST
#include "fifo_buffer.h"
fifo_buffer<CANTxFrame, 1024> txCanBuffer;
#endif // EFI_SIMULATOR

#if EFI_CAN_SUPPORT
/*static*/ CANDriver* CanTxMessage::s_devices[2] = {nullptr, nullptr};

/*static*/ void CanTxMessage::setDevice(CANDriver* device1, CANDriver* device2) {
	s_devices[0] = device1;
	s_devices[1] = device2;
}
#endif // EFI_CAN_SUPPORT

CanTxMessage::CanTxMessage(CanCategory p_category, uint32_t eid, uint8_t dlc, size_t bus, bool isExtended) {
    category = p_category;
#if HAS_CAN_FRAME
#ifndef STM32H7XX
	// ST bxCAN device
	m_frame.IDE = isExtended ? CAN_IDE_EXT : CAN_IDE_STD;
	m_frame.RTR = CAN_RTR_DATA;
#else /* if STM32H7XX */
	// Bosch M_CAN FDCAN device
	m_frame.common.XTD = isExtended;
	m_frame.common.RTR = 0;
#endif

	if (isExtended) {
		CAN_EID(m_frame) = eid;
	} else {
	    if (eid >= 0x800) {
	        criticalError("Looks like extended CAN ID %x %s", eid, getCanCategory(category));
	        return;
	    }
		CAN_SID(m_frame) = eid;
	}

	setDlc(dlc);

	setBus(bus);

	setArrayValues(m_frame.data8, 0);
#endif // HAS_CAN_FRAME
}

CanTxMessage::~CanTxMessage() {
#if EFI_SIMULATOR || EFI_UNIT_TEST
	txCanBuffer.put(m_frame);

#if EFI_UNIT_TEST
	printf("%s Sending CAN%d message: ID=%x/l=%x %x %x %x %x %x %x %x %x \n",
		   getCanCategory(category),
		   busIndex + 1,
#ifndef STM32H7XX
		   (unsigned int)((m_frame.IDE == CAN_IDE_EXT) ? CAN_EID(m_frame) : CAN_SID(m_frame)),
#else
		   (unsigned int)(m_frame.common.XTD ? CAN_EID(m_frame) : CAN_SID(m_frame)),
#endif
		   m_frame.DLC,
		   m_frame.data8[0], m_frame.data8[1],
		   m_frame.data8[2], m_frame.data8[3],
		   m_frame.data8[4], m_frame.data8[5],
		   m_frame.data8[6], m_frame.data8[7]);
#endif
#endif // EFI_SIMULATOR

#if EFI_CAN_SUPPORT
	auto device = s_devices[busIndex];

	if (!device) {
		criticalError("Send: CAN%d device not configured", busIndex + 1);
		return;
	}

	if (!engine->allowCanTx) {
		return;
	}

	bool verboseCan0 = engineConfiguration->verboseCan && busIndex == 0;
	bool verboseCan1 = engineConfiguration->verboseCan2 && busIndex == 1;

	if (verboseCan0 || verboseCan1) {
		efiPrintf("%s Sending CAN%d message: ID=%x/l=%x %x %x %x %x %x %x %x %x",
				getCanCategory(category),
				busIndex + 1,
#ifndef STM32H7XX
				(unsigned int)((m_frame.IDE == CAN_IDE_EXT) ? CAN_EID(m_frame) : CAN_SID(m_frame)),
#else
				(unsigned int)(m_frame.common.XTD ? CAN_EID(m_frame) : CAN_SID(m_frame)),
#endif
				m_frame.DLC,
				m_frame.data8[0], m_frame.data8[1],
				m_frame.data8[2], m_frame.data8[3],
				m_frame.data8[4], m_frame.data8[5],
				m_frame.data8[6], m_frame.data8[7]);
	}

	// 100 ms timeout
	msg_t msg = canTransmit(device, CAN_ANY_MAILBOX, &m_frame, TIME_MS2I(100));
#if EFI_TUNER_STUDIO
	if (msg == MSG_OK) {
		engine->outputChannels.canWriteOk++;
	} else {
		engine->outputChannels.canWriteNotOk++;
	}
#endif // EFI_TUNER_STUDIO
#endif /* EFI_CAN_SUPPORT */
}

#if HAS_CAN_FRAME
void CanTxMessage::setDlc(uint8_t dlc) {
	m_frame.DLC = dlc;
}

void CanTxMessage::setBus(size_t bus) {
	busIndex = bus;
}

// LSB Little-endian System, "Intel"
void CanTxMessage::setShortValue(uint16_t value, size_t offset) {
	m_frame.data8[offset] = value & 0xFF;
	m_frame.data8[offset + 1] = value >> 8;
}

// MOTOROLA order, MSB (Most Significant Byte/Big Endian) comes first.
void CanTxMessage::setShortValueMsb(uint16_t value, size_t offset) {
	m_frame.data8[offset] = value >> 8;
	m_frame.data8[offset + 1] = value & 0xFF;
}

void CanTxMessage::setBit(size_t byteIdx, size_t bitIdx) {
	m_frame.data8[byteIdx] |= 1 << bitIdx;
}

uint8_t& CanTxMessage::operator[](size_t index) {
	return m_frame.data8[index];
}
#endif // HAS_CAN_FRAME

