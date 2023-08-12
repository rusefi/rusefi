
#include "pch.h"
#include "can_bench_test.h"
#include "can_msg_tx.h"

#define CAN_BENCH_HEADER 0x66
#define CAN_BENCH_GET_COUNT 0
#define CAN_BENCH_GET_SET 1
#define CAN_BENCH_GET_CLEAR 2

#if EFI_CAN_SUPPORT

static void setPin(const CANRxFrame& frame, int value) {
		int index = frame.data8[1];
		if (index >= getBoardMetaOutputsCount())
			return;
		Gpio pin = getBoardMetaOutputs()[index];
#if EFI_GPIO_HARDWARE && EFI_PROD_CODE
		palWritePad(getHwPort("can_write", pin), getHwPin("can_write", pin), value);
#endif // EFI_GPIO_HARDWARE && EFI_PROD_CODE
}

void processCanBenchTest(const CANRxFrame& frame) {
	if (CAN_EID(frame) == BENCH_TEST_EVENT_COUNTERS) {
		CanTxMessage msg(CanCategory::BENCH_TEST, BENCH_TEST_RAW_ANALOG, 8);

		int primaryFall = engine->triggerCentral.getHwEventCounter((int)SHAFT_PRIMARY_FALLING);
		int primaryRise = engine->triggerCentral.getHwEventCounter((int)SHAFT_PRIMARY_RISING);
		int secondaryFall = engine->triggerCentral.getHwEventCounter((int)SHAFT_SECONDARY_FALLING);
		int secondaryRise = engine->triggerCentral.getHwEventCounter((int)SHAFT_SECONDARY_RISING);

		msg[0] = (primaryRise + primaryFall) & 0xff;
		msg[1] = (secondaryRise + secondaryFall) & 0xff;

		for (int camIdx = 0; camIdx < 4; camIdx++) {
			int vvtRise = 0, vvtFall = 0;
			if (camIdx < CAM_INPUTS_COUNT) {
				vvtRise = engine->triggerCentral.vvtEventRiseCounter[camIdx];
				vvtFall = engine->triggerCentral.vvtEventFallCounter[camIdx];
			}

			msg[2 + camIdx] = (vvtRise + vvtFall) & 0xff;
		}
		
		return;
	}
	if (CAN_EID(frame) != CAN_ECU_HW_META) {
		return;
	}
	if (frame.data8[0] != CAN_BENCH_HEADER) {
		return;
	}
	uint8_t command = frame.data8[1];
	if (command == CAN_BENCH_GET_COUNT) {
		CanTxMessage msg(CanCategory::BENCH_TEST, CAN_ECU_HW_META + 1, 8);
		msg[0] = CAN_BENCH_HEADER;
		msg[1] = 0;
		msg[2] = getBoardMetaOutputsCount();

	} else if (command == CAN_BENCH_GET_SET) {
	    setPin(frame, 1);
	} else if (command == CAN_BENCH_GET_CLEAR) {
	    setPin(frame, 0);
	}
}

#endif // EFI_CAN_SUPPORT
