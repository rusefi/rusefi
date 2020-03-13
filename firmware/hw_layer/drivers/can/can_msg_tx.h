#pragma once

#include <cstdint>
#include <cstddef>

#include "os_access.h"

class CanTxMessage
{
public:
	CanTxMessage(uint32_t eid);
	~CanTxMessage();

	uint8_t& operator[](size_t);

	void setShortValue(uint16_t value, size_t offset);
	void setBit(size_t byteIdx, size_t bitIdx);

protected:
	CANTxFrame m_frame;
};

template <typename TData>
class CanTxTyped final : public CanTxMessage
{
	static_assert(sizeof(TData) == sizeof(CANTxFrame::data8));

public:
	CanTxTyped(uint32_t eid) : CanTxMessage(eid) { }

	// So you can do:
	// CanTxTyped<MyType> d;
	// d->memberOfMyType = 23;
	TData* operator->() {
		return reinterpret_cast<TData*>(&m_frame.data8);
	}
};
