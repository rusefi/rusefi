#include "pch.h"

#if EFI_CAN_SUPPORT
#include "can.h"
#include "can_msg_tx.h"

#define HONDA_SPEED_158 0x158
#define HONDA_TACH_1DC 0x1DC

uint8_t calculateHondaChecksum(uint16_t canId, uint8_t length, CanTxMessage* data) {
	int sum = 0;
	//copy the array to a local array this is bad codesmell but what i am doing for testing.
    uint8_t* dataCopy = data->getFrame()->data8;
	// Sum all bytes except the checksum nibble
	for (int i = 0; i < length; i++) {
		// we only include the upper nibble in calculation
		uint8_t value = (i == length - 1) ? (dataCopy[i] & 0xF0) : dataCopy[i];

		sum += (value >> 4) & 0xF;
		if (i < length - 1) {
			sum += value & 0xF;
		}
	}

	sum += (canId >> 8) & 0xF;
	sum += (canId >> 4) & 0xF;
	sum += (canId) & 0xF;
    //Magic number bullshit
	sum += 8;

	// Calculate final checksum: 0x10 - (sum & 0xF) & 0xF
	uint8_t checksum = (0x10 - (sum & 0xF)) & 0xF;

	return checksum;
}

void canDashboardHondaK(CanCycle cycle) {
	static int rollingId = 0;

	if (cycle.isInterval(CI::_50ms)) {
		{
            CanTxMessage msg(CanCategory::NBC, HONDA_TACH_1DC, 4);
			msg[0] = 0x02; //This is constant i think
			msg.setShortValueMsb(Sensor::getOrZero(SensorType::Rpm), /*offset*/ 1);
			rollingId = (rollingId + 1) & 0x3;
			msg[3] = (rollingId << 4);
			uint8_t checksum = calculateHondaChecksum(0x1DC, 4, msg);
			msg[3] |= checksum & 0xF;
		}
	}
}

#endif