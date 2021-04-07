
#pragma once

#include "can_listener.h"
class EGTtoCAN final : public CanListener {
public:
	EGTtoCAN(uint8_t EGTIndex);

protected:
	void decodeFrame(const CANRxFrame& frame, efitick_t nowNt) override;

private:
	uint8_t m_egtIndex;
};
