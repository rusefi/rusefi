#include "pch.h"

#if EFI_CAN_SUPPORT
#include "can.h"
#include "can_msg_tx.h"

#define FIESTA_RPM_SPD 0x201
#define FIESTA_ODO_ECT 0X420

void canDashboardFiestaRocam(CanCycle cycle) {
	uint8_t speedVar = Sensor::getOrZero(SensorType::VehicleSpeed);
	uint16_t rpmVar = Sensor::getOrZero(SensorType::Rpm);
	uint8_t ectVar = Sensor::getOrZero(SensorType::Clt);

	if (cycle.isInterval(CI::_10ms)) {
		{
			CanTxMessage msg(CanCategory::NBC, FIESTA_RPM_SPD, 8);

			msg[0] = rpmVar / 63;
			msg[1] = rpmVar % 63;

			msg[4] = 0x26 + (uint8_t)(speedVar/2.55);
			msg[5] = 0x26 + (uint8_t)(speedVar * 100 % 255);
			
			msg[7] = 0xfe;

		}
	}

	if (cycle.isInterval(CI::_50ms)) {
		{


		}
	}


}

#endif
