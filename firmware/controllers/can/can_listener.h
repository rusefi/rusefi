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
	CanListener(uint32_t eid)
		: m_eid(eid)
	{
	}

	CanListener* processFrame(const CANRxFrame& frame, efitick_t nowNt) {
		if (CAN_EID(frame) == m_eid) {
			decodeFrame(frame, nowNt);
		}

		return m_next;
	}

	uint32_t getEid() {
		return m_eid;
	}

	void setNext(CanListener* next) {
		m_next = next;
	}

	virtual CanListener* request() {
		return m_next;
	}

protected:
	virtual void decodeFrame(const CANRxFrame& frame, efitick_t nowNt) = 0;
	CanListener* m_next = nullptr;

private:
	const uint32_t m_eid;
};
