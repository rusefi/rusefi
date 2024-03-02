/**
 * @file	can_listener.h
 *
 * @date March 31, 2021
 * @author Matthew Kennedy, (c) 2021
 */

#pragma once

#include "can.h"

class CanListener {
public:
	CanListener(uint32_t id)
		: m_id(id)
	{
	}

	CanListener* processFrame(const CANRxFrame& frame, efitick_t nowNt) {
		if (acceptFrame(frame)) {
			decodeFrame(frame, nowNt);
		}

		return m_next;
	}

	uint32_t getId() {
		return m_id;
	}

	void setNext(CanListener* next) {
		m_next = next;
	}

	virtual CanListener* request() {
		return m_next;
	}

	bool hasNext() const {
		return m_next;
	}

	// Return true if the provided frame should be accepted for processing by the listener.
	// Override if you need more complex logic than comparing to a single ID.
	virtual bool acceptFrame(const CANRxFrame& frame) const {
		return CAN_ID(frame) == m_id;
	}

protected:
	virtual void decodeFrame(const CANRxFrame& frame, efitick_t nowNt) = 0;

private:
	CanListener* m_next = nullptr;

	const uint32_t m_id;
};
