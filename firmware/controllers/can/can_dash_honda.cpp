#include "pch.h"

// can_dash_honda.cpp

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST
#include "can.h"
#include "can_msg_tx.h"

#define HONDA_SPEED_158 0x158
#define HONDA_TACH_1DC 0x1DC
#define HONDA_TEMP_324 0x324


#define HONDA_IMA_166 0x166
#define HONDA_AIRBAG_039 0x039
#define HONDA_EPS_18E 0x18E
#define HONDA_TPMS_333 0x333
#define HONDA_CEL_17C 0x17C
#define HONDA_VSA_1A4 0x1A4

uint8_t calculateHondaChecksum(uint16_t canId, uint8_t length, uint8_t* data) {
	int sum = 0;
	// Sum all bytes except the checksum nibble
	for (int i = 0; i < length; i++) {
		// we only include the upper nibble in calculation
		uint8_t value = (i == length - 1) ? (data[i] & 0xF0) : data[i];

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

    if (cycle.isInterval(CI::_20ms)) {
        // RPM message (HONDA_TACH_1DC)
        CanTxMessage msg(CanCategory::NBC, HONDA_TACH_1DC, 4);
        msg[0] = 0x02; //This is constant i think
        msg.setShortValueMsb(Sensor::getOrZero(SensorType::Rpm), /*offset*/ 1);
        rollingId = (rollingId + 1) & 0x3;
        msg[3] = (rollingId << 4);

        uint8_t tempBuffer1[4];
        for (int i = 0; i < 4; i++) {
            tempBuffer1[i] = msg[i];
        }

        uint8_t checksum1 = calculateHondaChecksum(HONDA_TACH_1DC, 4, tempBuffer1);
        msg[3] |= checksum1 & 0xF;

        // Speed message (HONDA_SPEED_158)
        CanTxMessage msg2(CanCategory::NBC, HONDA_SPEED_158, 8);
        msg2.setShortValueMsb(Sensor::getOrZero(SensorType::VehicleSpeed) * 160, /*offset*/ 0);
        msg2.setShortValueMsb(Sensor::getOrZero(SensorType::VehicleSpeed) * 160, /*offset*/ 4);
        msg2[7] = (rollingId << 4);

        uint8_t tempBuffer2[8];
        for (int i = 0; i < 8; i++) {
            tempBuffer2[i] = msg2[i];
        }

        uint8_t checksum2 = calculateHondaChecksum(HONDA_SPEED_158, 8, tempBuffer2);
        msg2[7] |= checksum2 & 0xF;

        // CEL message (HONDA_CEL_17C)
        CanTxMessage msg3(CanCategory::NBC, HONDA_CEL_17C, 8);
        msg3[0] = 0x00;
        msg3[1] = 0xEA;
        msg3[2] = 0x00;
        msg3[3] = 0x00;
        msg3[4] = 0x00;
        msg3[5] = 0x00;
        msg3[6] = 0x00;
        rollingId = (rollingId + 1) & 0x3;
        msg3[7] = (rollingId << 4);

        uint8_t tempBuffer3[8];
        for (int i = 0; i < 8; i++) {
            tempBuffer3[i] = msg3[i];
        }

        uint8_t checksum3 = calculateHondaChecksum(0x17c, 8, tempBuffer3);
        msg3[7] |= checksum3 & 0xF;

        // IMA message (HONDA_IMA_166)
        CanTxMessage msg4(CanCategory::NBC, HONDA_IMA_166, 4);
        msg4[0] = 0x50;
        msg4[1] = 0x1d;
        msg4[2] = 0xec;
        msg4[3] = (rollingId << 4);

        uint8_t tempBuffer4[4];
        for (int i = 0; i < 4; i++) {
            tempBuffer4[i] = msg4[i];
        }

        uint8_t checksum4 = calculateHondaChecksum(HONDA_IMA_166, 4, tempBuffer4);
        msg4[3] |= checksum4 & 0xF;

        // Airbag message (HONDA_AIRBAG_039)
        CanTxMessage msg5(CanCategory::NBC, HONDA_AIRBAG_039, 2);
        msg5[0] = 0x00;
        msg5[1] = (rollingId << 4);

        uint8_t tempBuffer5[2];
        for (int i = 0; i < 2; i++) {
            tempBuffer5[i] = msg5[i];
        }

        uint8_t checksum5 = calculateHondaChecksum(HONDA_AIRBAG_039, 2, tempBuffer5);
        msg5[1] |= checksum5 & 0xF;

        // EPS message (HONDA_EPS_18E)
        CanTxMessage msg6(CanCategory::NBC, HONDA_EPS_18E, 3);
        msg6[0] = 0x00;
        msg6[1] = 0x00;
        msg6[2] = (rollingId << 4);

        uint8_t tempBuffer6[3];
        for (int i = 0; i < 3; i++) {
            tempBuffer6[i] = msg6[i];
        }

        uint8_t checksum6 = calculateHondaChecksum(HONDA_EPS_18E, 3, tempBuffer6);
        msg6[2] |= checksum6 & 0xF;

        // TPMS message (HONDA_TPMS_333)
        CanTxMessage msg7(CanCategory::NBC, HONDA_TPMS_333, 8);
        msg7[0] = 0x00;
        msg7[1] = 0x00;
        msg7[2] = 0x00;
        msg7[3] = 0x00;
        msg7[4] = 0x00;
        msg7[5] = 0x00;
        msg7[6] = 0x00;
        msg7[7] = (rollingId << 4);

        uint8_t tempBuffer7[8];
        for (int i = 0; i < 8; i++) {
            tempBuffer7[i] = msg7[i];
        }

        uint8_t checksum7 = calculateHondaChecksum(HONDA_TPMS_333, 8, tempBuffer7);
        msg7[7] |= checksum7 & 0xF;

        // VSA message (HONDA_VSA_1A4)
        CanTxMessage msg8(CanCategory::NBC, HONDA_VSA_1A4, 8);
        msg8[7] = (rollingId << 4);

        uint8_t tempBuffer8[8];
        for (int i = 0; i < 8; i++) {
            tempBuffer8[i] = msg8[i];
        }

        uint8_t checksum8 = calculateHondaChecksum(HONDA_VSA_1A4, 8, tempBuffer8);
        msg8[7] |= checksum8 & 0xF;

        // Temperature message (HONDA_TEMP_324)
        CanTxMessage msg9(CanCategory::NBC, HONDA_TEMP_324, 8);
        msg9[0] = Sensor::getOrZero(SensorType::Clt) + 40;
        msg9[1] = Sensor::getOrZero(SensorType::Iat) + 40;
        msg9[7] = (rollingId << 4);

        uint8_t tempBuffer9[8];
        for (int i = 0; i < 8; i++) {
            tempBuffer9[i] = msg9[i];
        }

        uint8_t checksum9 = calculateHondaChecksum(HONDA_TEMP_324, 8, tempBuffer9);
        msg9[7] |= checksum9 & 0xF;
    }
}

#endif
