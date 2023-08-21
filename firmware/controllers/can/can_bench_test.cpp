
#include "pch.h"
#include "board_id.h"
#include "can_bench_test.h"
#include "can_msg_tx.h"
#include "can_common.h"
#include "frequency_sensor.h"

#define TRUNCATE_TO_BYTE(i) ((i) & 0xff)
// raw values are 0..5V, convert it to 8-bit (0..255)
#define RAW_TO_BYTE(v) TRUNCATE_TO_BYTE((int)(v * 255.0 / 5.0))

#define RAW_ANALOG_VALUES_COUNT 8

bool qcDirectPinControlMode = false;

#if EFI_CAN_SUPPORT

static void setPin(const CANRxFrame& frame, int value) {
		int index = frame.data8[1];
		if (index >= getBoardMetaOutputsCount())
			return;
		Gpio pin = getBoardMetaOutputs()[index];
#if EFI_GPIO_HARDWARE && EFI_PROD_CODE
		palWritePad(getHwPort("can_write", pin), getHwPin("can_write", pin), value);
		// todo: add smart chip support support
#endif // EFI_GPIO_HARDWARE && EFI_PROD_CODE
}

void sendEventCounters() {
#if EFI_SHAFT_POSITION_INPUT
	CanTxMessage msg(CanCategory::BENCH_TEST, BENCH_TEST_EVENT_COUNTERS, 8, /*bus*/0, /*isExtended*/true);

	int primaryFall = engine->triggerCentral.getHwEventCounter((int)SHAFT_PRIMARY_FALLING);
	int primaryRise = engine->triggerCentral.getHwEventCounter((int)SHAFT_PRIMARY_RISING);
	int secondaryFall = engine->triggerCentral.getHwEventCounter((int)SHAFT_SECONDARY_FALLING);
	int secondaryRise = engine->triggerCentral.getHwEventCounter((int)SHAFT_SECONDARY_RISING);

	msg[0] = TRUNCATE_TO_BYTE(primaryRise + primaryFall);
	msg[1] = TRUNCATE_TO_BYTE(secondaryRise + secondaryFall);

	for (int camIdx = 0; camIdx < 4; camIdx++) {
		int vvtRise = 0, vvtFall = 0;
		if (camIdx < CAM_INPUTS_COUNT) {
			vvtRise = engine->triggerCentral.vvtEventRiseCounter[camIdx];
			vvtFall = engine->triggerCentral.vvtEventFallCounter[camIdx];
		}

		msg[2 + camIdx] = TRUNCATE_TO_BYTE(vvtRise + vvtFall);
	}

	extern FrequencySensor vehicleSpeedSensor;
	msg[6] = TRUNCATE_TO_BYTE(vehicleSpeedSensor.eventCounter);
#endif // EFI_SHAFT_POSITION_INPUT
}

void sendRawAnalogValues() {
	const float values[RAW_ANALOG_VALUES_COUNT] = { 
		engine->outputChannels.rawTps1Primary, 
		engine->outputChannels.rawTps1Secondary,
		engine->outputChannels.rawPpsPrimary,
		engine->outputChannels.rawPpsSecondary,
		engine->outputChannels.rawMap,
		engine->outputChannels.rawClt,
		engine->outputChannels.rawIat,
		engine->outputChannels.rawBattery,
	};

	// send the first packet
	{
		CanTxMessage msg(CanCategory::BENCH_TEST, BENCH_TEST_RAW_ANALOG, 8, /*bus*/0, /*isExtended*/true);
		for (int valueIdx = 0; valueIdx < 8; valueIdx++) {
			msg[valueIdx] = (valueIdx < RAW_ANALOG_VALUES_COUNT) ? RAW_TO_BYTE(values[valueIdx]) : 0;
		}
	}
	// todo: send the second packet
}

void sendBoardStatus() {
#if EFI_PROD_CODE
	CanTxMessage msg(CanCategory::BENCH_TEST, BENCH_TEST_BOARD_STATUS, 8, /*bus*/0, /*isExtended*/true);

	int boardId = getBoardId();
	msg[0] = TRUNCATE_TO_BYTE(boardId >> 8);
	msg[1] = TRUNCATE_TO_BYTE(boardId);
	
	int numSecondsSinceReset = getTimeNowS();
	msg[2] = TRUNCATE_TO_BYTE(numSecondsSinceReset >> 16);
	msg[3] = TRUNCATE_TO_BYTE(numSecondsSinceReset >> 8);
	msg[4] = TRUNCATE_TO_BYTE(numSecondsSinceReset);

    int engineType = (int) engineConfiguration->engineType;
	msg[5] = engineType >> 8;
	msg[6] = engineType;
#endif // EFI_PROD_CODE
}

void processCanBenchTest(const CANRxFrame& frame) {
	if (CAN_EID(frame) != BENCH_TEST_IO_CONTROL) {
		return;
	}
	if (frame.data8[0] != CAN_BENCH_HEADER) {
		return;
	}
	qcDirectPinControlMode = true;
	uint8_t command = frame.data8[1];
	if (command == CAN_BENCH_GET_COUNT) {
		CanTxMessage msg(CanCategory::BENCH_TEST, BENCH_TEST_IO_META_INFO, 8, /*bus*/0, /*isExtended*/true);
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
