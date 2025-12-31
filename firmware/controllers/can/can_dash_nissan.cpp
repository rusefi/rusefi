#include "pch.h"

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST
#include "can.h"
#include "can_msg_tx.h"

//https://www.drive2.ru/b/500679938089681452/
#define NISSAN_STEERING_WHEEL 0x002

#define NISSAN_ENGINE_2 0x231_561

#define NISSAN_ENGINE_7_233_563 0x233

#define NISSAN_ENGINE_1_RPM_1F9_505 0x1F9

// Nissan z33 350Z and else
// 0x23d = 573
#define NISSAN_ENGINE_3_23D_573       0x23D
#define NISSAN_ENGINE_4_23E_574 0x23E

#define NISSAN_TCU_1 0x251
#define NISSAN_TCU_2 0x253

// 640
#define NISSAN_VEHICLE_SPEED_280 0x280
// wheel speed see "102 CAN Communication decoded"
// 19500 value would be 100 kph
// 644
#define NISSAN_WHEEL_SPEED1 0x284
// 645
#define NISSAN_WHEEL_SPEED2 0x285

// 670
#define NISSAN_UNKNOWN_4 0x29E

#define NISSAN_ABS 0x2A0

// 833 doors
#define NISSAN_BCM 0x341

// https://www.drive2.com/l/530057789272229668/
// 852
#define NISSAN_VEHICLE_SPEED 0x354

#define NISSAN_ENGINE_5_CLT_551_1361 0x551
// 1408
#define NISSAN_RPM_AGAIN 0x580
#define NISSAN_ODOMETER 0x5C5
// 1549
#define NISSAN_BCM_2 0x60D


void canDashboardNissanVQ(CanCycle cycle) {
	static int rollingId = 0;

	if (cycle.isInterval(CI::_50ms)) {
		{
			CanTxMessage msg(CanCategory::NBC, NISSAN_ENGINE_1_RPM_1F9_505, 8);
			msg.setShortValueMsb(Sensor::getOrZero(SensorType::Rpm) * 8, /*offset*/ 2);
		}

		{
			CanTxMessage msg(CanCategory::OBD, NISSAN_ENGINE_5_CLT_551_1361, 8);

			int clt = Sensor::getOrZero(SensorType::Clt);
			msg[0] = clt + 45;
		}


		{
			CanTxMessage msg(CanCategory::NBC, NISSAN_ENGINE_3_23D_573, 8);

			rollingId = (rollingId + 1) % 4;
			const uint8_t magicByte[4] = {0x03, 0x23, 0x42, 0x63};

			msg[0] = magicByte[rollingId];
			msg[1] = (int)(Sensor::getOrZero(SensorType::AcceleratorPedal) * 255 / 100);

			// thank you "102 CAN Communication decoded"
#define CAN_23D_RPM_MULT 3.15
			int rpm315 = (int)(Sensor::getOrZero(SensorType::Rpm) / CAN_23D_RPM_MULT);
			msg.setShortValue(rpm315, /*offset*/ 3);

			msg[7] = 0x70; // todo: CLT decoding?
		}
	}
}

#endif
