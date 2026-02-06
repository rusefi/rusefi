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

#include "can_category.h"
#include "can.h"

#if EFI_SIMULATOR || EFI_UNIT_TEST
#include "fifo_buffer.h"
extern fifo_buffer<CANTxFrame, TEST_CAN_BUFFER_SIZE> txCanBuffer;
#endif // EFI_SIMULATOR

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
	explicit CanTxMessage(CanCategory category, uint32_t eid, uint8_t dlc = 8, size_t bus = 0, bool isExtended = false);

	/**
	 * Destruction of an instance of CanTxMessage will transmit the message over the wire.
	 */
	~CanTxMessage();

    CanCategory category;

#if EFI_CAN_SUPPORT
	/**
	 * Configures the device for all messages to transmit from.
	 */
	static void setDevice(size_t idx, CANDriver* device);
#endif // EFI_CAN_SUPPORT

	size_t busIndex = 0;

	/**
	 * @brief Read & write the raw underlying 8-byte buffer.
	 */
	uint8_t& operator[](size_t);

	/**
	 * @brief Write a 16-bit short value to the buffer. Note: this writes in Intel little endian byte order.
	 */
	void setShortValue(uint16_t value, size_t offset);

	/**
	 Same as above but big endian Motorola
	 * for instance DBC 8|16@0
	 */
	void setShortValueMsb(uint16_t value, size_t offset);

	/**
	 * @brief Set a single bit in the transmit buffer.  Useful for single-bit flags.
	 */
	void setBit(size_t byteIdx, size_t bitIdx);

	void setDlc(uint8_t dlc);

	void setBus(size_t bus);

#if HAS_CAN_FRAME
	const CANTxFrame *getFrame() const {
		return &m_frame;
	}

    void setArray(const uint8_t *data, size_t len) {
        for (size_t i = 0; i < std::min(len, size_t(8)); i++) {
            m_frame.data8[i] = data[i];
        }
    }

    template<size_t N>
    void setArray(const uint8_t (&data)[N]) {
        setArray(data, N);
    }

#endif // HAL_USE_CAN || EFI_UNIT_TEST

protected:
#if HAS_CAN_FRAME
	CANTxFrame m_frame;
#endif // HAL_USE_CAN || EFI_UNIT_TEST

private:
#if EFI_CAN_SUPPORT
	static CANDriver* s_devices[EFI_CAN_BUS_COUNT];
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
	explicit CanTxTyped(CanCategory p_category, uint32_t p_id, bool p_isExtended, size_t canChannel) : CanTxMessage(p_category, p_id, sizeof(TData), canChannel, p_isExtended) { }

#if HAS_CAN_FRAME
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
#endif // HAS_CAN_FRAME
};

template <typename TData>
void transmitStruct(CanCategory category, uint32_t id, bool isExtended, bool canChannel)
{
	CanTxTyped<TData> frame(category, id, isExtended, canChannel);
	// Destruction of an instance of CanTxMessage will transmit the message over the wire.
	// see CanTxMessage::~CanTxMessage()
	populateFrame(frame.get());
}
