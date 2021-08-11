/**
 * @file	can_msg_tx.h
 *
 * CAN message transmission
 * 
 * @date Mar 13, 2020
 * @author Matthew Kennedy, (c) 2012-2020
 */

#pragma once

#include <cstdint>
#include <cstddef>

#include "os_access.h"

/**
 * Represent a message to be transmitted over CAN.
 * 
 * Usage:
 *   * Create an instance of CanTxMessage
 *   * Set any data you'd like to transmit either using the subscript operator to directly access bytes, or any of the helper functions.
 *   * Upon destruction, the message is transmitted.
 */
class CanTxMessage
{
public:
	/**
	 * Create a new CAN message, with the specified extended ID.
	 */
	explicit CanTxMessage(uint32_t eid, uint8_t dlc = 8, bool isExtended = false);

	/**
	 * Destruction of an instance of CanTxMessage will transmit the message over the wire.
	 */
	~CanTxMessage();

#if EFI_CAN_SUPPORT
	/**
	 * Configures the device for all messages to transmit from.
	 */
	static void setDevice(CANDriver* device);
#endif // EFI_CAN_SUPPORT
	/**
	 * @brief Read & write the raw underlying 8-byte buffer.
	 */
	uint8_t& operator[](size_t);

	/**
	 * @brief Write a 16-bit short value to the buffer. Note: this writes in big endian byte order.
	 */
	void setShortValue(uint16_t value, size_t offset);

	/**
	 * @brief Set a single bit in the transmit buffer.  Useful for single-bit flags.
	 */
	void setBit(size_t byteIdx, size_t bitIdx);

	void setDlc(uint8_t dlc);

protected:
#if EFI_CAN_SUPPORT
	CANTxFrame m_frame;
#else // not EFI_CAN_SUPPORT
	uint8_t m_data8[8];
#endif // EFI_CAN_SUPPORT

private:
#if EFI_CAN_SUPPORT
	static CANDriver* s_device;
#endif // EFI_CAN_SUPPORT
};

/**
 * A CAN message based on a type, removing the need for manually flipping bits/bytes.
 */
template <typename TData>
class CanTxTyped final : public CanTxMessage
{
#if EFI_CAN_SUPPORT
	static_assert(sizeof(TData) <= sizeof(CANTxFrame::data8));
#endif // EFI_CAN_SUPPORT

public:
	explicit CanTxTyped(uint32_t eid) : CanTxMessage(eid, sizeof(TData)) { }

#if EFI_CAN_SUPPORT
	/**
	 * Access members of the templated type.  
	 * 
	 * So you can do:
	 * CanTxTyped<MyType> d;
	 * d->memberOfMyType = 23;
	 */
	TData* operator->() {
		return reinterpret_cast<TData*>(&m_frame.data8);
	}

	TData& get() {
		return *reinterpret_cast<TData*>(&m_frame.data8);
	}
#endif // EFI_CAN_SUPPORT
};

template <typename TData>
void transmitStruct(uint32_t eid)
{
	CanTxTyped<TData> frame(eid);
	// Destruction of an instance of CanTxMessage will transmit the message over the wire.
	// see CanTxMessage::~CanTxMessage()
	populateFrame(frame.get());
}
