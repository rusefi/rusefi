#include "pch.h"

#if EFI_CAN_SUPPORT

#include "can_msg_tx.h"
#include "sandero_eletro_pump.h"

#define PUMP_CAR_DATA 0x1f6
#define PUMP_CAR_SPEED 0x2c6
#define PUMP_STEER_SPEED 0x0c6

void sendEletroPumpInfo() {
	uint16_t rpmVar = Sensor::getOrZero(SensorType::Rpm);
	bool pumpStatus = false;
	if(rpmVar > 450) {
		pumpStatus = true;
	}

	{
		CanTxMessage msg(CanCategory::NBC, PUMP_CAR_DATA, 8);

		msg[0] = 0x00;
		if(pumpStatus) {
			msg[1] = 0x20;
			msg[2] = 0x80;
		} else {
			msg[1] = 0x00;
			msg[2] = 0x40;
		}
		msg[3] = 0x42;
		msg[4] = 0x01;
		msg[5] = 0xff;
		msg[6] = 0xff;
		msg[7] = 0xfe;
	}

	{
		CanTxMessage msg(CanCategory::NBC, PUMP_STEER_SPEED, 8);
		msg[0] = 0x7f;
		msg[1] = 0x94;
		msg[2] = 0xa0;
		msg[3] = 0xff;
		msg[4] = 0x7f;
		msg[5] = 0xf4;
		msg[6] = 0xac;
		msg[7] = 0x5d;

	}

	{
		CanTxMessage msg(CanCategory::NBC, PUMP_CAR_SPEED, 7);
		msg[0] = 0x00;
		msg[1] = 0x00;
		msg[2] = 0xff;
		msg[3] = 0xff;
		msg[4] = 0xff;
		msg[5] = 0xff;
		msg[6] = 0xfe;

	}

	

}

#endif 
